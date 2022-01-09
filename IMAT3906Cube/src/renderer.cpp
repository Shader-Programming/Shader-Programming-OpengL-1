#include "..\include\renderer.h"



Renderer::Renderer(const unsigned int sWidth, const unsigned int sHeight)
{
	screenHeight = sHeight;
	screenWidth = sWidth;

	loadShaders();
	loadTextures();

	plane1 = Plane(m_shaders[1]);
	plane1.assignTexture(3);
	plane1.assignNormalMap(5);
	plane1.assignDispMap(4);

	cube1 = Cube(m_shaders[0]);
	cube1.assignTexture(0);
	cube1.assignNormalMap(1);
	cube1.assignSpecularMap(2);

	quad = Quad(m_shaders[2]);

	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.f, 20.f);
	lightView = glm::lookAt(LightParams::lightPos, glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	skybox.createSkyBox();
}

void Renderer::RenderScene( Camera camera)
{

	shaders["Cube"].use();
	shaders["Cube"].setBool("toggleNormalMap", LightParams::useNormalMap);
	setUniforms(shaders["Cube"], camera);
	setUniforms(shaders["Floor"], camera);

	shaders["Floor"].use();
	shaders["Floor"].setBool("toggleDispMap", LightParams::useDispMap);
	shaders["Floor"].setBool("toggleNormalMap", LightParams::useNormalMap);

	shaders["PostProcessing"].use();
	shaders["PostProcessing"].setInt("image", colourAttachment[0]);

	updateSpotLightUniforms();


	m_shaders[0].use();
	// MVP 
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();

	shaders["Cube"].setMat4("projection", projection);
	shaders["Cube"].setMat4("view", view);
	shaders["Cube"].setVec3("viewPos", camera.Position);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	shaders["Cube"].setMat4("model", model);
	shaders["Cube"].setInt("depthMap", 10);
	shaders["Cube"].setMat4("lightSpaceMatrix", lightSpaceMatrix);

	shaders["Floor"].use();
	shaders["Floor"].setMat4("projection", projection);
	shaders["Floor"].setMat4("view", view);
	shaders["Floor"].setVec3("viewPos", camera.Position);
	shaders["Floor"].setMat4("model", model);
	shaders["Floor"].setInt("depthMap", 10);
	shaders["Floor"].setMat4("lightSpaceMatrix", lightSpaceMatrix);

	shaders["SkyBox"].use();
	shaders["SkyBox"].setMat4("projection", projection);
	shaders["SkyBox"].setMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glActiveTexture(GL_TEXTURE7);

	plane1.assignShader(shaders["Floor"]);
	cube1.assignShader(shaders["Cube"]);

	skybox.renderSkyBox(shaders["SkyBox"]);
	plane1.Render();
	cube1.Render();
}

void Renderer::RenderShadowMap()
{

	if(LightParams::dirLightDir == glm::vec3(0,-1,0))
		LightParams::dirLightDir = glm::vec3(0.0001,-1,0);

	LightParams::lightPos = LightParams::dirLightDir * glm::vec3(-0.7);

	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.f, 20.f);
	lightView = glm::lookAt(LightParams::lightPos, glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	shaders["ShadowMap"].use();
	shaders["ShadowMap"].setMat4("lightSpaceMatrix", lightSpaceMatrix);

	plane1.assignShader(shaders["ShadowMap"]);
	cube1.assignShader(shaders["ShadowMap"]);

	plane1.Render();
	cube1.Render();


}

void Renderer::RenderBlurAndBloom()
{
	shaders["Blur"].use();
	shaders["Blur"].setBool("blurToggle", LightParams::toggleBlur);
	shaders["Blur"].setBool("depthOfFieldToggle", LightParams::toggleDepthOfField);
	shaders["Blur"].setInt("image", 7);
	shaders["Blur"].setInt("image2", 8);
	shaders["Blur"].setInt("depthMap", 9);
	shaders["Blur"].setBool("horizontal", true);
	quad.Draw(shaders["Blur"], colourAttachment[0], colourAttachment[1]);
	shaders["Blur"].setBool("horizontal", false);
	quad.Draw(shaders["Blur"], colourAttachment[0], colourAttachment[1]);


	shaders["Bloom"].use();
	shaders["Bloom"].setInt("image", 7);
	shaders["Bloom"].setInt("bloomBlur", 8);
}

void Renderer::RenderQuadWithPostProcessing()
{
	shaders["PostProcessing"].use();
	shaders["PostProcessing"].setInt("image", 7);

	if (LightParams::toggleBloom)
	{
		LightParams::toggleBlur = true;
		quad.Draw(shaders["Bloom"], colourAttachment[0], blurredTextures[1]);
	}
	else
		quad.Draw(shaders["PostProcessing"], blurredTextures[0]);

}

void Renderer::assignCameraAndSetUniforms(Camera& cam)
{
	camera = &cam;
	setUniforms(shaders["Cube"], *camera);
	setUniforms(shaders["Floor"], *camera);
}

void Renderer::loadShaders()
{

	Shader cubeShader("..\\shaders\\plainVert.vs", "..\\shaders\\plainFrag.fs");
	Shader floorShader("..\\shaders\\plainVert.vs", "..\\shaders\\floorFrag.fs");

	Shader postProcessingShader("..\\shaders\\postprocessing.vs", "..\\shaders\\postprocessing.fs");
	Shader depthShader("..\\shaders\\postprocessing.vs", "..\\shaders\\renderDepth.fs");
	Shader blurShader("..\\shaders\\postprocessing.vs", "..\\shaders\\blur.fs");
	Shader depthOfFieldShader("..\\shaders\\postprocessing.vs", "..\\shaders\\depthOfField.fs");
	Shader bloomShader("..\\shaders\\postprocessing.vs", "..\\shaders\\bloom.fs");
	Shader shadowMapShader("..\\shaders\\shadowPass.vs", "..\\shaders\\shadowPass.fs");
	Shader skyBoxShader("..\\shaders\\skyBox.vs", "..\\shaders\\skyBox.fs");

	m_shaders.push_back(cubeShader);
	m_shaders.push_back(floorShader);
	m_shaders.push_back(postProcessingShader);
	m_shaders.push_back(depthShader);
	m_shaders.push_back(blurShader);
	m_shaders.push_back(depthOfFieldShader);
	m_shaders.push_back(bloomShader);
	m_shaders.push_back(shadowMapShader);
	m_shaders.push_back(skyBoxShader);

	shaders["Cube"] = m_shaders[0];
	shaders["Floor"] = m_shaders[1];
	shaders["PostProcessing"] = m_shaders[2];
	shaders["Depth"] = m_shaders[3];
	shaders["Blur"] = m_shaders[4];
	shaders["DOF"] = m_shaders[5];
	shaders["Bloom"] = m_shaders[6];
	shaders["ShadowMap"] = m_shaders[7];
	shaders["SkyBox"] = m_shaders[8];
}

void Renderer::loadTextures()
{
	unsigned int cubeDiffTexture = loadTexture("..\\resources\\metalPlate\\diffuse.jpg");
	unsigned int cubeSpecular = loadTexture("..\\resources\\metalPlate\\specular.jpg");
	unsigned int cubeNormalM = loadTexture("..\\resources\\metalPlate\\normal.jpg");

	unsigned int planeDiffTexture = loadTexture("..\\resources\\bricks\\diffuse.jpg");
	unsigned int planeDisp = loadTexture("..\\resources\\bricks\\displ.png");
	unsigned int planeNormalM = loadTexture("..\\resources\\bricks\\normal.jpg");


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeDiffTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, cubeNormalM);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, cubeSpecular);


	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, planeDiffTexture);


	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, planeDisp);


	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, planeNormalM);

	glActiveTexture(GL_TEXTURE6);
}

void Renderer::setUniforms(Shader& shader, Camera camera)
{
	shader.use();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	shader.setVec3("dLight.lightCol", LightParams::dirLightCol);
	shader.setVec3("dLight.lightDir", glm::normalize(LightParams::dirLightDir));
	shader.setInt("selectedLight", LightParams::selectedLight);


	shader.setVec3("pLight.position", LightParams::pointLightPos);
	shader.setVec3("pLight.color", LightParams::pointLightCol);
	shader.setFloat("pLight.intensity", LightParams::pLightIntensity);
	
	shader.setFloat("pLight.Kc", 1);
	shader.setFloat("pLight.Kl", 0.22f);
	shader.setFloat("pLight.Ke", 0.020f);



	shader.setVec3("sLight.position", camera.Position);
	shader.setVec3("sLight.direction", camera.Front);
	shader.setVec3("sLight.color", LightParams::spotLightCol);
	shader.setFloat("sLight.Kc", 1);
	shader.setFloat("sLight.Kl;", 0.07f);
	shader.setFloat("sLight.Ke", 0.0017f);
	shader.setFloat("sLight.innerRad", glm::cos(glm::radians(LightParams::spotLightInnerRad)));
	shader.setFloat("sLight.outerRad", glm::cos(glm::radians(LightParams::spotLightOuterRad)));
}

void Renderer::PrepareFrameBuffers()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);


	glGenTextures(2, colourAttachment);

	for (int i = 0; i < 2; i++)
	{
	glBindTexture(GL_TEXTURE_2D, colourAttachment[i]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, colourAttachment[i], 0);
	}


	glGenTextures(1, &depthAttachment);
	glBindTexture(GL_TEXTURE_2D, depthAttachment);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(2, attachments);

	glGenFramebuffers(1, &FBOBlur);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOBlur);
	glGenTextures(3, blurredTextures);

	for (int i = 0; i < 3; i++)
	{
		glBindTexture(GL_TEXTURE_2D, blurredTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, blurredTextures[i], 0);
	}

	glDrawBuffers(3, attachments);

	const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;

	glGenFramebuffers(1, &FBODepthMap);
	glBindFramebuffer(GL_FRAMEBUFFER, FBODepthMap);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::setFBODepth()
{
	glGenFramebuffers(1, &FBODepth);
	glBindFramebuffer(GL_FRAMEBUFFER, FBODepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::updatePointLightUniforms()
{
}

void Renderer::updateSpotLightUniforms()
{
	if (LightParams::selectedLight == 2)
	{
		LightParams::spotLightPos = camera->Position;
		m_shaders[0].use();
		m_shaders[0].setVec3("sLight.position", LightParams::spotLightPos);
		m_shaders[0].setVec3("sLight.direction", camera->Front);

		m_shaders[1].use();
		m_shaders[1].setVec3("sLight.position", LightParams::spotLightPos);
		m_shaders[1].setVec3("sLight.direction", camera->Front);
	}
	else
	{
		lightView = glm::lookAt(LightParams::lightPos, glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));
	}

}

unsigned int Renderer::loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //s = x axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //t = y axis , r if 3d
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
		std::cout << "Loaded texture at path: " << path << " width  " << width << " id  " << textureID << std::endl;
	}
	else
	{
		stbi_image_free(data);
		std::cout << "Texture failed to load at path " << path << std::endl;
	}
	return textureID;
}
