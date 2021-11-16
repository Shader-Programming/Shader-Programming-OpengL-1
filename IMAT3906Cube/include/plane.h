#pragma once
#include "entity.h"
#include "Shader.h"
#include <array>
class Plane : public Entity
{
public:
	Plane();
	Plane(Shader& planeShader);
	unsigned int planeVAO, planeVBO, planeEBO;
	void Render();
	void assignTexture(unsigned int id);
	void assignShader(Shader& planeShader);
	static unsigned int planeIndices[36];
	static float planeVertices[192];
private:
	Shader* m_shader;
	glm::vec3 m_color;
	unsigned int m_textureId;
	float planeSize = 5.0f;
	float planeLevel = -2.0f;
	//std::array<float,32> planeVertices;
	//std::array<unsigned int, 6> planeIndices;
};