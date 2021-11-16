#pragma once
#include "entity.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




class Cube : public Entity
{
public:
	Cube();
	Cube(Shader& cubeshader);
	unsigned int cubeVAO, cubeVBO, cubeEBO;
	void Render();
	void assignTexture(unsigned int id);
	void assignNormalMap(unsigned int id);
	void assignShader(Shader& shader);
	static unsigned int cubeIndices[36];
	static float cubeVertices[192];
private:
	Shader* m_shader;
	glm::vec3 m_color;
	unsigned int m_textureId;
	unsigned int m_normalMapId;
};