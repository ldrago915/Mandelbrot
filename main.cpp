#include "ComplexPlane.h"

using namespace std;

int main()
{
	//Get Resolution
	int resoWidth = (VideoMode::getDesktopMode().width / 2);
	int resoHeight = (VideoMode::getDesktopMode().height / 2);

	//Create and open window
	VideoMode vm(resoWidth, resoHeight);
	RenderWindow window(vm, "Mandelbrot", Style::Default);

	//Create complex plane based on resolution
	ComplexPlane compPlane(resoWidth, resoHeight);

	//Create font and text objects
	Font font;
	if (!font.loadFromFile("fonts/vgasys.ttf"))
	{
		cout << "Error opening font";
	}
	Text messageText;
	messageText.setFont(font);

	//Begin loop of window
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				//Close window when window is X'd
				window.close();
			}
			//Zoom in or out based on mouse click
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					std::cout << "the left button was pressed" << std::endl;
					std::cout << "mouse x: " << event.mouseButton.x << std::endl;
					std::cout << "mouse y: " << event.mouseButton.y << std::endl;
					compPlane.zoomIn();
					compPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
				}
				if (event.mouseButton.button == Mouse::Right)
				{
					std::cout << "the right button was pressed" << std::endl;
					std::cout << "mouse x: " << event.mouseButton.x << std::endl;
					std::cout << "mouse y: " << event.mouseButton.y << std::endl;
					compPlane.zoomOut();
					compPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
				}
			}
			//Mouse Movement
			if (event.type == Event::MouseMoved)
			{
				compPlane.setMouseLocation({ event.mouseMove.x, event.mouseMove.y });
			}
		}
		//Escape to close
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//Update screen
		compPlane.updateRender();
		compPlane.loadText(messageText);

		//Draw
		window.clear();
		window.draw(compPlane);
		window.draw(messageText);
		window.display();
	}


}