// cg_minecraft.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <iostream>
#include "test.h"


int main()
{
	test();
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	sf::Window window(sf::VideoMode(800, 600, 32), "Minecraft", sf::Style::Titlebar | sf::Style::Close, settings);

	glewExperimental = GL_TRUE;
	glewInit();

	bool running = true;
	while (running)
	{
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type) {
				case sf::Event::Closed:
					running = false;
					break;
				case sf::Event::MouseMoved:
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
						std::cout << windowEvent.mouseMove.x << " - " << windowEvent.mouseMove.y << std::endl;
					break;
				case sf::Event::MouseButtonPressed:
					std::cout << "mouse pressed" << std::endl;
					break;
				case sf::Event::MouseButtonReleased:
					std::cout << "mouse released" << std::endl;
					break;
				case sf::Event::MouseWheelScrolled:
					if (windowEvent.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
						std::cout << "wheel type: vertical" << std::endl;
					else if (windowEvent.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
						std::cout << "wheel type: horizontal" << std::endl;
					else
						std::cout << "wheel type: unknown" << std::endl;
					std::cout << "wheel movement: " << windowEvent.mouseWheelScroll.delta << std::endl;
					std::cout << "mouse x: " << windowEvent.mouseWheelScroll.x << std::endl;
					std::cout << "mouse y: " << windowEvent.mouseWheelScroll.y << std::endl;
					break;
			}
		}

		window.display();
	}

	window.close();

    return 0;
}

