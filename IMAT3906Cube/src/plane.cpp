#include "..\include\plane.h"



Plane::Plane()
{
	 std::array<float,32>planeVertices =  {
			 -planeSize, planeLevel,  -planeSize, 0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
			 planeSize, planeLevel,   -planeSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 planeSize, planeLevel,  planeSize, 0.0f, 1.0f, 0.0f, 1.0f,1.0f,
			-planeSize, planeLevel,  planeSize, 0.0f, 1.0f, 0.0f, 0.0f,1.0f
	};


	
	std::array<unsigned int,6> planeIndices = {
		3,2,1,
		3,1,0
	};

	//Floor
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &planeEBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices.data()), planeIndices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
Plane::Plane(Shader& planeShader)
{
	m_shader = &planeShader;

	std::array<float, 32>planeVertices = {
		-planeSize, planeLevel,  -planeSize, 0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		planeSize, planeLevel,   -planeSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		planeSize, planeLevel,  planeSize, 0.0f, 1.0f, 0.0f, 1.0f,1.0f,
	   -planeSize, planeLevel,  planeSize, 0.0f, 1.0f, 0.0f, 0.0f,1.0f
	};



	std::array<unsigned int, 6> planeIndices = {
		3,2,1,
		3,1,0
	};

	//Floor
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &planeEBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, planeVertices.size()*sizeof(float), &planeVertices.at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,planeIndices.size() * sizeof(unsigned int), &planeIndices.at(0), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
void Plane::Render()
{
	m_shader->setInt("diffuseTexture", m_textureId);
	glm::mat4 model = glm::mat4(1.0f);
	m_shader->setMat4("model", model);
	m_shader->setVec3("objectCol", m_color);
	glBindVertexArray(planeVAO);  // bind and draw floor
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Plane::assignTexture(unsigned int id)
{
	m_textureId = id;
}

void Plane::assignShader(Shader& planeShader)
{
	m_shader = &planeShader;
}
