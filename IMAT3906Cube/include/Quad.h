#pragma once

#include "entity.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Quad
{
public:
	Quad() {};
	Quad(Shader& quadShader);
	unsigned int quadVAO,quadVBO;
	Shader* m_shader;
	void Draw( const unsigned int& textureObj);
	void Draw(Shader& shader, const unsigned int& textureObj);
	void Draw(const int& textureSlot,Shader& shader, const unsigned int& textureObj);
	void Draw(Shader& shader, const unsigned int& textureObj, const unsigned int& textureObj2);
};