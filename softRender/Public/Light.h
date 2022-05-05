#pragma once
#include "Matrix.h"

class DirectionLight
{
private:
	Vector3f direction;
	Vector3f position;
public:
	float intensity;//����ǿ��
	bool forVertex;//�𶥵�������ع�����ɫ
	DirectionLight():intensity(1),forVertex(true){}
	DirectionLight(const Vector3f& dir, const Vector3f& pos, float Intensity = 1, bool forV = true)
		:direction(dir),position(pos),intensity(Intensity),forVertex(forV){}
	~DirectionLight(){}

	Vector3f GetDirection()
	{
		return Vector3f(0,0,0) - direction;//ȡ��
	}
	Matrix LookAt(const Vector3f& upAxis);
};

class PointLight
{
public:
	float intensity;
	Vector3f position;
	PointLight():intensity(1.0){}
	PointLight(const Vector3f& pos, float inten) :position(pos), intensity(inten) {}
	~PointLight() {}
};