
#include "Quad.h"

Quad::Quad(Shader& quadShader)
{
	m_shader = &quadShader;
	float quadVerticies[] = {
		//pos			  // texture coords
		-1.f, 1.f, 0.f,   0.f,1.f,
		-1.f, -1.f, 0.f,  0.f,0.f,
		 1.f, 1.f, 0.f,   1.f,1.f,
		 1.f, -1.f, 0.f,   1.f,0.f
	};

	//plane VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerticies), &quadVerticies, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
}

void Quad::Draw (const unsigned int& textureObj)
{
	m_shader->use();
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Quad::Draw(Shader& shader, const unsigned int& textureObj)
{
	shader.use();
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
