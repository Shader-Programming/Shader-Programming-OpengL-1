#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stb_image.h>

#include "Shader.h"
#include "Camera.h"
#include "cube.h"
#include "plane.h"


class Renderer {
public:
	Renderer(const unsigned int sWidth, const unsigned int sHeight);
	Renderer(const unsigned int sWidth, const unsigned int sHeight, Shader& shader);
	void RenderScene(Shader& shader, Camera camera);

private:

	unsigned int loadTexture(char const* path);
	void RenderCube(Shader& shader);
	void RenderPlane(Shader& shader);

	void CreateCube();
	void CreateFloor();

	unsigned int  planeVAO, planeVBO, planeEBO;

	Cube cube1;
	Plane plane1;

	glm::vec3 cubeColor = glm::vec3(1.0, 0.4, 0.4);
	glm::vec3 floorColor = glm::vec3(0.1, 0.3, 0.3);

	unsigned int screenWidth, screenHeight;

};




