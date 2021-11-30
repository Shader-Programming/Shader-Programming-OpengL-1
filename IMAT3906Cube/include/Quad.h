#pragma once

#include "entity.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Quad
{
public:
	Quad(Shader& quadShader);
	unsigned int quadVAO,quadVBO;
	Shader* m_shader;
	void Draw( unsigned int& textureObj);
};