#include "ComplexPlane.h"

//Public functions
/*	
VertexArray m_vArray; *
State m_state; *
Vector2f m_mouseLocation; /
Vector2i m_pixel_size; * 
Vector2f m_plane_center; *
Vector2f m_plane_size; *
int m_zoomCount; *
float m_aspectRatio; *
*/
ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	m_pixel_size.x = pixelWidth;
	m_pixel_size.y = pixelHeight;
	m_aspectRatio = (float)pixelHeight / pixelWidth;
	m_plane_center = { 0, 0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_state = State::CALCULATING;
	m_vArray = VertexArray(Points, pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
}

void ComplexPlane::zoomIn()
{
	m_zoomCount++;
	float zoom_X = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float zoom_Y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size.x = zoom_X;
	m_plane_size.y = zoom_Y;
	m_state = State::CALCULATING;
}
void ComplexPlane::zoomOut()
{
	m_zoomCount--;
	float zoom_X = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float zoom_Y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size.x = zoom_X;
	m_plane_size.y = zoom_Y;
	m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
	cout << "Setting center\n x: " << mousePixel.x << "y: " << mousePixel.y;
	m_plane_center = ComplexPlane::mapPixelToCoords(mousePixel);
	cout << endl << "Plane center is saying " << m_plane_center.x << ", " << m_plane_center.y;
	m_state = State::CALCULATING;
	cout << "Center set\n";
}
void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	m_mouseLocation = ComplexPlane::mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
	stringstream buildText;
	buildText << "Mandelbrot Set" << endl
		<< fixed << setprecision(6)
		<< "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")" << endl
		<< "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")" << endl
		<< "Left-Click to Zoom in" << endl
		<< "Right-click to Zoom out" << endl;
	text.setString(buildText.str());
}
void ComplexPlane::updateRender()
{
	Vector2i inCoord;
	Vector2f compCoord;
	size_t itCount;
	Uint8 r, g, b;
	if (m_state == State::CALCULATING)
	{
		for (int i = 0; i < m_pixel_size.y; i++)
		{
			for (int j = 0; j < m_pixel_size.x; j++)
			{
				m_vArray[j + i * m_pixel_size.x].position = { (float)j, (float)i };
				inCoord = { j , i };
				compCoord = ComplexPlane::mapPixelToCoords(inCoord);
				itCount = ComplexPlane::countIterations(compCoord);
				ComplexPlane::iterationsToRGB(itCount, r, g, b);
				m_vArray[j + i * m_pixel_size.x].color = { r,g,b };
			}
		}
		cout << "render complete\n";
		m_state = State::DISPLAYING;
	}
}

//Priavte Functions
size_t ComplexPlane::countIterations(Vector2f coord)
{
	complex<float> c(coord.x, coord.y);
	complex<float> z = c;
	int i = 0;
	while ((z.real() * z.real() + z.imag() * z.imag()) < 4.0 && i < MAX_ITER)
	{
		z = z * z + c;
		i++;
	}
	return i;

}


void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	if (count == MAX_ITER) //if within set then set color to black
	{
		r = 0;
		g = 0;
		b = 0;
	}
	else
	{
		float t = ((float)count / MAX_ITER) * (UC_MAX - UC_MIN) + UC_MIN;
		r = sin(t) * 255;
		g = sin(2 * t) * 255;
		b = pow(cos(t), 3) * 255;
	}

}


Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
	Vector2f pCoords;
	float xRatio = float(mousePixel.x) / m_pixel_size.x;
	pCoords.x = (xRatio * m_plane_size.x + m_plane_center.x - m_plane_size.x / 2.0f);
	float yRatio = float(mousePixel.y) / m_pixel_size.y;
	pCoords.y = (m_plane_center.y + m_plane_size.y / 2.0f) - (yRatio * m_plane_size.y);
	return pCoords;
}