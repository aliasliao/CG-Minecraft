// cg_minecraft.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cstdio>
#include "Texture.h"
#include "Store.h"
#include "Shader.h"
#include "Camera.h"
#include "Text.h"


const int winWidth = 1000;
const int winHeight = 600;
const int xCenter = winWidth / 2;
const int yCenter = winHeight / 2;

glm::vec3 getCursorCubePos(const glm::vec3 &objcoord, const glm::vec3 &cameraPos);  // for removeCube
glm::vec3 getNextCubePos(const glm::vec3 &objcoord, const glm::vec3 &cameraPos);  // for addCube

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
	Text text;

	cubeShader.use();
	cubeShader.setInt("texes", 0);

	TEX.uploadTextures();
	cubeStore.addCube(glm::vec3(-4, 0, 0), cub::GRASS, TEX);
	cubeStore.addCube(glm::vec3(2, 0, 0), cub::CRAFTING_TABLE, TEX);
	cubeStore.addCube(glm::vec3(-2, 0, 0), cub::BRICK, TEX);
	cubeStore.saveState("debug1.obj");
	groudStore.saveState("debug2.obj");
	text.init_resources();
	//////////////////////////////////////////////////////////////////////////


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	sf::Clock clock;
	glm::ivec2 oldPos, newPos;
	cub currentCube = static_cast<cub>(0);
	int cubeTotal = static_cast<int>(cub::LAST);
	int curInd;
	float sx = 2.0f / (float)winWidth;
	float sy = 2.0f / (float)winHeight;
	GLfloat red[4] = { 1, 0, 0, 1 };
	GLfloat white[4] = { 1, 1, 1, 1 };
	// for mouse click
	GLfloat depth;
	glm::vec3 wincoord;
	glm::vec3 objcoord;
	// initial STATIC mats
	glm::vec4 viewport = glm::vec4(0, 0, winWidth, winHeight);
	glm::mat4 model = glm::mat4();
	glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
	glm::vec3 lightPos = glm::vec3(5, 5, 5);
	glm::vec3 lightColor = glm::vec3(1, 1, 1);

	// center mouse, set oldPos
	sf::Mouse::setPosition(sf::Vector2i(xCenter, yCenter), window);
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

					if (newPos.x <= 0 || newPos.x >= winWidth - 1 || newPos.y <= 0 || newPos.y >= winHeight - 1)
						sf::Mouse::setPosition(sf::Vector2i(xCenter, yCenter), window);
					if (newPos != glm::ivec2(xCenter, yCenter))
						camera.processMouseMove(newPos - oldPos);

					oldPos = newPos;
					break;

				case sf::Event::MouseButtonPressed:
					glReadPixels(xCenter, winHeight - yCenter - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
					wincoord = glm::vec3(xCenter, winHeight - yCenter - 1, depth);
					objcoord = glm::unProject(wincoord, camera.getViewMat(), projection, viewport);
					if (event.mouseButton.button == sf::Mouse::Left) {
						cubeStore.addCube(
							getNextCubePos(objcoord, camera.getPosVec()), currentCube, TEX
						);
					}
					else if (event.mouseButton.button == sf::Mouse::Right) {
						cubeStore.removeCube(
							getCursorCubePos(objcoord, camera.getPosVec())
						);
					}
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

		cubeShader.setMat4("model", model);
		cubeShader.setMat4("view", camera.getViewMat());
		cubeShader.setMat4("projection", projection);
		cubeShader.setVec3("lightPos", lightPos);
		cubeShader.setVec3("viewPos", camera.getPosVec());
		cubeShader.setVec3("lightColor", lightColor);

		///////
		glUseProgram(text.program);

		FT_Set_Pixel_Sizes(text.face, 0, 48);
		glUniform4fv(text.uniform_color, 1, red);
		glDepthMask(GL_FALSE);
		text.render_text("+", -1 + 488 * sx, 1 - 319 * sy, sx, sy);
		glDepthMask(GL_TRUE);
		FT_Set_Pixel_Sizes(text.face, 0, 24);
		glUniform4fv(text.uniform_color, 1, white);
		text.render_text(TEX.cubName[currentCube].c_str(), -1 + 8 * sx, 1 - 580 * sy, sx, sy);

		cubeShader.use();
		///////

		window.display();
	}

	window.close();

    return 0;
}

glm::vec3 getCursorCubePos(const glm::vec3 &objcoord, const glm::vec3 &cameraPos)  // for removeCube
{
	if (objcoord.y < 0.01)
		return glm::vec3(objcoord.x, -1, objcoord.z);

	glm::vec3 objRound = glm::round(objcoord);
	glm::vec3 accurate = glm::abs(objRound - objcoord);

	if (accurate.x < 0.005) {  // face axis x
		if (objcoord.x < cameraPos.x)  // +x
			return glm::vec3(objRound.x-1, floor(objcoord.y), floor(objcoord.z));
		else
			return glm::vec3(objRound.x, floor(objcoord.y), floor(objcoord.z));
	}

	if (accurate.y < 0.005) {
		if (objcoord.y < cameraPos.y)
			return glm::vec3(floor(objcoord.x), objRound.y-1, floor(objcoord.z));
		else
			return glm::vec3(floor(objcoord.x), objRound.y, floor(objcoord.z));
	}

	if (accurate.z < 0.005) {
		if (objcoord.z < cameraPos.z)
			return glm::vec3(floor(objcoord.x), floor(objcoord.y), objRound.z - 1);
		else
			return glm::vec3(floor(objcoord.x), floor(objcoord.y), objRound.z);
	}

	else  // not a regular cube
		return glm::vec3(0, -1, 0);
}

glm::vec3 getNextCubePos(const glm::vec3 &objcoord, const glm::vec3 &cameraPos)  // for addCube
{
	//if (objcoord.y < 0.01)
	//	return glm::vec3(objcoord.x, 0, objcoord.z);

	glm::vec3 objRound = glm::round(objcoord);
	glm::vec3 accurate = glm::abs(objRound - objcoord);

	if (accurate.x < 0.005) {  // face axis x
		if (objcoord.x < cameraPos.x)  // +x
			return glm::vec3(objRound.x, floor(objcoord.y), floor(objcoord.z));
		else
			return glm::vec3(objRound.x-1, floor(objcoord.y), floor(objcoord.z));
	}

	if (accurate.y < 0.005) {
		if (objcoord.y < cameraPos.y)
			return glm::vec3(floor(objcoord.x), objRound.y, floor(objcoord.z));
		else
			return glm::vec3(floor(objcoord.x), objRound.y-1, floor(objcoord.z));
	}

	if (accurate.z < 0.005) {
		if (objcoord.z < cameraPos.z)
			return glm::vec3(floor(objcoord.x), floor(objcoord.y), objRound.z);
		else
			return glm::vec3(floor(objcoord.x), floor(objcoord.y), objRound.z-1);
	}

	else  // not a regular cube
		return glm::vec3(0, -1, 0);
}
