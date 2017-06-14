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


const int winWidth = 1000;
const int winHeight = 600;

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	sf::Window window(sf::VideoMode(winWidth, winHeight), "Minecraft", sf::Style::Titlebar | sf::Style::Close, settings);
	window.setMouseCursorVisible(false);
	window.setMouseCursorGrabbed(true);

	glewExperimental = GL_TRUE;
	glewInit();

	//////////////////////////////////////////////////////////////////////////
	Texture TEX = Texture();
	Store groudStore = Store(true);
	Store cubeStore = Store();
	Shader cubeShader = Shader("cube.vert", "cube.frag");
	Shader simpleShader = Shader("simple.vert", "simple.frag");
	Camera camera = Camera(glm::vec3(0,2,9));

	cubeShader.use();
	cubeShader.setInt("texes", 0);

	TEX.uploadTextures();
	cubeStore.addCube(glm::vec3(-4, 0, 0), cub::GRASS, TEX);
	cubeStore.addCube(glm::vec3(2, 0, 0), cub::CRAFTING_TABLE, TEX);
	cubeStore.addCube(glm::vec3(-2, 0, 0), cub::BRICK, TEX);
	cubeStore.saveState("debug1.obj");
	groudStore.saveState("debug2.obj");
	//////////////////////////////////////////////////////////////////////////


	glEnable(GL_DEPTH_TEST);
	sf::Clock clock;
	glm::ivec2 oldPos, newPos;
	cub currentCube = static_cast<cub>(0);
	int cubeTotal = static_cast<int>(cub::LAST);
	int curInd;

	// center mouse, set oldPos
	sf::Mouse::setPosition(sf::Vector2i(winHeight / 2, winWidth / 2), window);
	sf::Vector2i tmp = sf::Mouse::getPosition(window);
	oldPos = glm::ivec2(tmp.x, tmp.y);

	while (window.isOpen())
	{
		sf::Event event;
		float deltaTime = clock.restart().asSeconds();
		while (window.pollEvent(event))
		{
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
						window.close();
					break;
				case sf::Event::MouseMoved:
					newPos = glm::ivec2(event.mouseMove.x, event.mouseMove.y);
					camera.processMouseMove(newPos - oldPos);
					oldPos = newPos;
					break;
				case sf::Event::MouseWheelScrolled:
					curInd = static_cast<int>(currentCube);
					curInd += (int)event.mouseWheelScroll.delta;
					if (curInd < 0) curInd = cubeTotal - 1;
					else if (curInd == cubeTotal) curInd = 0;
					currentCube = static_cast<cub>(curInd);
					break;
				default:
					break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			camera.processKeyboard(cam::front, deltaTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			camera.processKeyboard(cam::back, deltaTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			camera.processKeyboard(cam::left, deltaTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			camera.processKeyboard(cam::right, deltaTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			camera.processKeyboard(cam::zoomIn, deltaTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
			camera.processKeyboard(cam::zoomOut, deltaTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			camera.processKeyboard(cam::reset, deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cubeStore.drawArrays();
		groudStore.drawArrays();

		cubeShader.setMat4("model", glm::mat4());
		cubeShader.setMat4("view", camera.getViewMat());
		cubeShader.setMat4("projection", glm::perspective(glm::radians(camera.getZoom()), (float)winWidth/(float)winHeight, 1.0f, 100.0f));
		cubeShader.setVec3("lightPos", glm::vec3(5, 5, 5));
		cubeShader.setVec3("viewPos", glm::vec3(camera.getPosVec()));
		cubeShader.setVec3("lightColor", glm::vec3(1, 1, 1));


		window.display();
	}

	window.close();

    return 0;
}

