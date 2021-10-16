#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"

class Renderer {
public:
	Renderer(const unsigned int sWidth, const unsigned int sHeight);
	void RenderScene(Shader& shader, Camera camera);

private:
	void RenderCube(Shader& shader);
	void RenderPlane(Shader& shader);

	void CreateCube();
	void CreateFloor();

	unsigned int cubeVAO, cubeVBO, cubeEBO, planeVAO, planeVBO, planeEBO;


	glm::vec3 cubeColor = glm::vec3(1.0, 0.4, 0.4);
	glm::vec3 floorColor = glm::vec3(0.1, 0.3, 0.3);

	unsigned int screenWidth, screenHeight;

};




