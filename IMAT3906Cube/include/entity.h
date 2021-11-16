#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tan;
	glm::vec3 biTan;
};

class Entity
{
public:
	Entity();
	void CalculateTanAndBitan(unsigned int* indicesData, int indiciesLength);
	void UnpackVerticies();
	std::vector<float> GetUpdatedVextexData();
	void extractVertices(float* vertexData, int length);
	void UpdateVertexData(float* vertexData, int length, unsigned int* indicesData, int indLenth);
protected:
	std::vector<Vertex> verticies;
	std::vector<float> updatedVertexData;
	//unsigned int* indicies;
};