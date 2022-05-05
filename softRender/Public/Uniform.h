#pragma once
#include "Light.h"
#include "Vector3.h"
#include "Texture.h"
#include <vector>
#include "Matrix.h"
#include "Buffer.h"
//phong 用的结构体 
struct PhongVert
{
	std::vector<DirectionLight>dirlights;
	std::vector<PointLight>pointlights;
	Vector3f cameraPos;
	Vector3f lp0,lp1,lp2;
	Matrix m,v,p;
	Vertex v0,v1,v2;
	Matrix lightV,lightP;
};

struct PhongFrag
{
	PhongFrag(Texture* baseTex, DepthBuffer* depthBuffer) :baseTex(baseTex), depthBuffer(depthBuffer) {}
	~PhongFrag() {}

	Texture* baseTex;
	DepthBuffer* depthBuffer;
};

//shadow 用的结构体
struct ShadowVert
{
	Matrix lightV, lightP;
	Matrix m;
};