#include "Mesh.h"
Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::AddVertexData(const Vector3f pos, float u, float v, const Color color /*= Color::white*/)
{
	Vertex p (pos,color,Vector2(u,v));
	vertexBuffer.push_back(p);
}

void Mesh::AddVertexData(float posX, float posY, float posZ, float u, float v, const Color color /*= Color::white*/)
{
	AddVertexData(Vector3f(posX, posY, posZ), u, v, color);
}

void Mesh::AddVertexData(float posX, float posY, float posZ, float u, float v, Vector3f nor, const Color color /*= Color::white*/)
{
	Vertex p(Vector3f(posX, posY, posZ), color, nor, Vector2(u, v));
	vertexBuffer.push_back(p);
}

