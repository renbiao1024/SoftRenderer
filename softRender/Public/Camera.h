#pragma once
#include "Transform.h"

class Camera
{
public:
	Transform transform;
	Matrix v, p;

	Camera(Transform t):transform(t){}
	Camera(){}
	Matrix LookAt(const Vector3f& eyePos, const Vector3f& lookat, const Vector3f& upAxis);
	//透视投影
	Matrix Perspective(float fov, float aspect, float nearPanel, float farPanel);
	Matrix Perspective(float l, float r, float n, float f, float t, float b);
	//正交投影
	Matrix Orthographic(float l, float r, float n, float f, float t, float b);
};