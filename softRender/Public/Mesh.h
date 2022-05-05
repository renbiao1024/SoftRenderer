#pragma once
#include "Vertex.h"
#include "Transform.h"
#include <vector>

using namespace std;

class Mesh//顶点，位置，法线，uv，索引
{
public:
	Transform transform;
	vector<Vertex>vertexBuffer;
	vector<Vector3f>positionBuffer;
	vector<Vector3f>normalBuffer;
	vector<Vector2>uvBuffer;
	vector<Vector3i>indexBuffer;

	Mesh();
	~Mesh();
	Transform GetTransform() {return transform; }
	int GetIndexBufferLength() {return indexBuffer.size(); }
	void SetTransform(Transform& t) {transform = t; }
	Matrix GetObjectToWorld() {return transform.objectToWorld; }
	void SetObjectToWorld(const Matrix &m) { transform.objectToWorld = m; }

	void AddVertexData(const Vector3f pos, float u, float v, const Color color = Color::white);
	void AddVertexData(float posX, float posY, float posZ, float u, float v, const Color color = Color::white);
	void AddVertexData(float posX, float posY, float posZ, float u, float v, Vector3f nor, const Color color = Color::white);
};