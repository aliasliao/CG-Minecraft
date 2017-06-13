// cg_minecraft.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Texture.h"
#include "Store.h"
#include "Shader.h"
#include "Camera.h"


int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	sf::Window window(sf::VideoMode(1000, 600), "Minecraft", sf::Style::Titlebar | sf::Style::Close, settings);
	window.setMouseCursorVisible(false);
	//window.setMouseCursorGrabbed(true);

	glewExperimental = GL_TRUE;
	glewInit();

	//////////////////////////////////////////////////////////////////////////
	Texture TEX = Texture();
	Store groudStore = Store(true);
	Store cubeStore = Store();
	Shader cubeShader = Shader("cube.vert", "cube.frag");
	Shader simpleShader = Shader("simple.vert", "simple.frag");
	Camera camera = Camera(glm::vec3(0,0,3));

	//cubeShader.use();
	//cubeShader.setMat4("model", glm::mat4());
	//cubeShader.setMat4("view", camera.getViewMat());
	//cubeShader.setMat4("projection", glm::perspective(glm::radians(75.0f), 1000.0f / 600.0f, 1.0f, 10.0f));
	//cubeShader.setVec3("lightPos", glm::vec3(5, 5, 5));
	//cubeShader.setVec3("viewPos", glm::vec3(camera.getPosVec()));
	//cubeShader.setVec3("lightColor", glm::vec3(1,1,1));
	//cubeShader.setInt("texes", 0);

	simpleShader.use();
	simpleShader.setMat4("model", glm::mat4());
	simpleShader.setMat4("view", camera.getViewMat());
	simpleShader.setMat4("projection", glm::perspective(glm::radians(75.0f), 1000.0f / 600.0f, 1.0f, 10.0f));
	simpleShader.setInt("texes", 0);

	TEX.uploadTextures();
	cubeStore.addCube(glm::vec3(0, 0, 0), cub::GRASS, TEX);
	cubeStore.addCube(glm::vec3(-1, 4, -4), cub::GRASS, TEX);
	cubeStore.saveState("debug1.obj");
	//////////////////////////////////////////////////////////////////////////


	glEnable(GL_DEPTH_TEST);
	while (window.isOpen())
	{
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type) {
				case sf::Event::Closed:
					window.close();
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

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		groudStore.draw();
		cubeStore.draw();

		window.display();
	}

	window.close();

    return 0;
}

