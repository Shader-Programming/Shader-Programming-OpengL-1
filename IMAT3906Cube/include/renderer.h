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
#include "skybox.h"

class Renderer {
public:
	Renderer(const unsigned int sWidth, const unsigned int sHeight);
	void RenderScene( Camera camera);
	void RenderShadowMap();
	std::vector<Shader> shaders;
	void assignCamera(Camera& cam);
	unsigned int FBO,FBODepth,FBOBlur, FBODepthMap;
	void PrepareFrameBuffers();
	void setFBODepth();
	Quad quad;
	unsigned int colourAttachment[2], depthAttachment,blurredTextures[3],depthMap;
	void updatePointLightUniforms();
	void updateSpotLightUniforms();
	void setUniforms(Shader& shader, Camera camera);
	glm::mat4 lightSpaceMatrix;
private:
	void loadShaders();
	void loadTextures();
	SkyBox skybox;

	unsigned int loadTexture(char const* path);


	Cube cube1;
	Plane plane1;
	Camera* camera;
	glm::vec3 cubeColor = glm::vec3(1.0, 0.4, 0.4);
	glm::vec3 floorColor = glm::vec3(0.1, 0.3, 0.3);
	
	glm::mat4 lightProjection;
	glm::mat4 lightView;

	unsigned int screenWidth, screenHeight;
	

};




