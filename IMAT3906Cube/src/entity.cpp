#include "..\include\entity.h"

Entity::Entity()
{
	Vertex v;
}

void Entity::CalculateTanAndBitan(unsigned int* indicesData, int indiciesLength)
{
	for (int i = 0; i < indiciesLength; i = i + 3)
	{
		Vertex& a = verticies.at(indicesData[i]);
		Vertex& b = verticies.at(indicesData[i+1]);
		Vertex& c = verticies.at(indicesData[i+2]);

		glm::vec3 deltaPos1 = b.pos - a.pos;
		glm::vec3 deltaPos2 = c.pos - a.pos;

		glm::vec2 deltaUV1 = b.uv - a.uv;
		glm::vec2 deltaUV2 = c.uv - a.uv;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

		glm::vec3 thisTangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 thisBitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		a.tan = a.tan + thisTangent;
		a.biTan = a.biTan + thisBitangent;

		b.tan = b.tan + thisTangent;
		b.biTan = b.biTan + thisBitangent;

		c.tan =c.tan + thisTangent;
		c.biTan = c.biTan + thisBitangent;
	}
	for (Vertex& v : verticies)
	{
		v.tan = glm::normalize(v.tan);
		v.biTan = glm::normalize(v.biTan);
	}
}

void Entity::UnpackVerticies()
{
	int stride = 14;
	for (Vertex& v : verticies)
	{
		float vertexData[] =
		{
			v.pos.x, v.pos.y, v.pos.z,
			v.normal.x,v.normal.y,v.normal.z,
			v.uv.x,v.uv.y,
			v.tan.x,v.tan.y,v.tan.z,
			v.biTan.x,v.biTan.y,v.biTan.z
		};
		for (int i = 0; i < stride; i++)
			updatedVertexData.push_back(vertexData[i]);

	}
}

std::vector<float> Entity::GetUpdatedVextexData()
{
	return updatedVertexData;
}

void Entity::extractVertices(float* vertexData, int length)
{
	Vertex v; 
	for (int i = 0; i < length; i = i + 8)
	{
		v.pos = glm::vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]); 
		v.normal = glm::vec3(vertexData[i + 3], vertexData[i + 4], vertexData[i + 5]);
		v.uv = glm::vec2(vertexData[i + 6], vertexData[i + 7]);
		v.tan = glm::vec3(0, 0, 0);
		v.biTan = glm::vec3(0, 0, 0);
		verticies.push_back(v);
	}
}

void Entity::UpdateVertexData(float* vertexData, int length, unsigned int* indicesData, int indLenth)
{
	extractVertices(vertexData, length);
	CalculateTanAndBitan(indicesData, indLenth);
	UnpackVerticies();
}
