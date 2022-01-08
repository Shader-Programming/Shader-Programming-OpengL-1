#pragma once
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"

class SkyBox
{
public:
	SkyBox();
	void createSkyBox();
	void renderSkyBox(Shader& shader);
private:
	void loadCubeMapTextures();
	unsigned int loadCubemap(std::vector<std::string> faces);
	void createVAO();

	std::vector<std::string> faces;
	unsigned int skyBoxTexObj;
	unsigned int skyBoxVAO;
};