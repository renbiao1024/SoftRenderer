#pragma once
#include "Matrix.h"
#include "MyMath.h"

class Transform
{
public:
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;
	Matrix objectToWorld;
	Matrix Translate(const Vector3f& v);//ƽ��
	Matrix RotateX(float angle);
	Matrix RotateY(float angle);
	Matrix RotateZ(float angle);
	Matrix Rotate(const Vector3f& rotAngle);//��ת
	Matrix Scale(const Vector3f& s);//����
	Transform(Vector3f pos, Vector3f rot, Vector3f s):position(pos), rotation(rot),scale(s){}
	Transform(){objectToWorld.Identity();}
};