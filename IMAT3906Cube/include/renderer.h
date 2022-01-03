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
#include "Quad.h"
#include "LightParams.h"

class Renderer {
public:
	Renderer(const unsigned int sWidth, const unsigned int sHeight);
	void RenderScene( Camera camera);
	std::vector<Shader> shaders;
	void assignCamera(Camera& cam);
	unsigned int FBO,FBODepth,FBOBlur, FBODepthOfField;
	void setFBOColour();
	void setFBODepth();
	Quad quad;
	unsigned int colourAttachment[2], depthAttachment,blurredTextures[2],dofTexture;
	void updatePointLightUniforms();
	void updateSpotLightUniforms();
	void setUniforms(Shader& shader, Camera camera);
private:
	void loadShaders();
	void loadTextures();
	
	

	unsigned int loadTexture(char const* path);


	Cube cube1;
	Plane plane1;
	Camera* camera;
	glm::vec3 cubeColor = glm::vec3(1.0, 0.4, 0.4);
	glm::vec3 floorColor = glm::vec3(0.1, 0.3, 0.3);

	unsigned int screenWidth, screenHeight;
	

};




