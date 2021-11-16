#include "..\include\renderer.h"

Renderer::Renderer(const unsigned int sWidth, const unsigned int sHeight)
{
	screenHeight = sHeight;
	screenWidth = sWidth;
}

Renderer::Renderer(const unsigned int sWidth, const unsigned int sHeight, Shader& shader)
{
	screenHeight = sHeight;
	screenWidth = sWidth;
	CreateFloor();

	unsigned int cubeDiffTexture = loadTexture("..\\resources\\metalPlate\\diffuse.jpg");
	unsigned int planeDiffTexture = loadTexture("..\\resources\\metalRust\\diffuse.jpg");
	unsigned int cubeNormalM = loadTexture("..\\resources\\metalPlate\\normal.jpg");


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeDiffTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, planeDiffTexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, cubeNormalM);



	plane1 = Plane(shader);
	plane1.assignTexture(1);

	cube1 = Cube(shader);
	cube1.assignTexture(0);
	cube1.assignNormalMap(2);
}

void Renderer::RenderScene(Shader& shader, Camera camera)
{
	
	glm::vec3 lightDirection = glm::vec3(0, -1, 0);
	glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   // what happens if we change to GL_LINE?

	shader.setVec3("lightCol", lightColor);
	shader.setVec3("lightDir", lightDirection);



	glm::vec3 pointLightPos = glm::vec3(0, 0, 0);
	glm::vec3 pointLightColor = glm::vec3(2, 0, 2);

	shader.setVec3("pLight.position",pointLightPos);
	shader.setVec3("pLight.color", pointLightColor);
	shader.setFloat("pLight.Kc", 1);
	shader.setFloat("pLight.Kl", 0.35f);
	shader.setFloat("pLight.Ke", 0.044f);


	glm::vec3 spotLightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	shader.setVec3("sLight.position", camera.Position);
	shader.setVec3("sLight.direction", camera.Front);
	shader.setVec3("sLight.color", spotLightColor);
	shader.setFloat("sLight.Kc", 1);
	shader.setFloat("sLight.Kl;", 0.027f);
	shader.setFloat("sLight.Ke", 0.0028f);
	shader.setFloat("sLight.innerRad", glm::cos(glm::radians(12.5f)));
	shader.setFloat("sLight.outerRad", glm::cos(glm::radians(17.5f)));


	// MVP 
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setVec3("viewPos", camera.Position);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	RenderPlane(shader);
	cube1.Render();
	plane1.Render();
}

void Renderer::RenderCube(Shader& shader)
{
	
}

void Renderer::RenderPlane(Shader& shader)
{
}

void Renderer::CreateCube()
{
}

void Renderer::CreateFloor()
{
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
