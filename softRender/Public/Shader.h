#pragma once
#include "MyMath.h"
#include "Vertex.h"
#include "Uniform.h"

struct IShader
{
public:
	virtual void VS(Vertex& v0, Vertex& v1, Vertex& v2) = 0; //vertex shader
	virtual bool FS(Vertex& v, Vector3f g) = 0; //fragment shader
};

struct PhongShader :public IShader
{
public:
	PhongVert v2f;
	PhongFrag frag;

	PhongShader(PhongVert v2f, PhongFrag frag) :v2f(v2f), frag(frag) {}
	~PhongShader(){}
	virtual void VS(Vertex& v0, Vertex& v1, Vertex& v2)override;
	virtual bool FS(Vertex& v, Vector3f g)override;
	float CalcuteShadow(Vector3f posLightSpace, double bias);
};

class ShadowShader :public IShader
{
public:
	ShadowVert v2f;

	ShadowShader() {}
	~ShadowShader() {}
	virtual void VS(Vertex& v0, Vertex& v1, Vertex& v2) override;
	virtual bool FS(Vertex& v, Vector3f g) override;
};