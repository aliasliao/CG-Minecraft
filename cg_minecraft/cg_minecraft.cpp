// cg_minecraft.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glew.h>
#include <SFML/Window.hpp>


int main()
{
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
			}
		}

		window.display();
	}

	window.close();

    return 0;
}

