#include "Camera.h"

Matrix Camera::LookAt(const Vector3f& eyePos, const Vector3f& lookat, const Vector3f& upAxis)
{
	Vector3f lookDir = lookat;
	lookDir.Normalize();
	Vector3f rightDir = Vector3f::Cross(upAxis,lookDir);
	rightDir.Normalize();
	Vector3f upDir = Vector3f::Cross(lookDir,rightDir);
	upDir.Normalize();
	Matrix m;
	//将相机转换到原点，面向正确方向，旋转到规定角度
	m.value[0][0] = rightDir.x;  m.value[1][0] = upDir.x;  m.value[2][0] = lookDir.x;  m.value[3][0] = 0;
	m.value[0][1] = rightDir.y;	 m.value[1][1] = upDir.y;  m.value[2][1] = lookDir.y;  m.value[3][1] = 0;
	m.value[0][2] = rightDir.z;  m.value[1][2] = upDir.z;  m.value[2][2] = lookDir.z;  m.value[3][2] = 0;
	m.value[0][3] = -eyePos.x;		 m.value[1][3] = -eyePos.y;   m.value[2][3] = -eyePos.z;	   m.value[3][3] = 1;
	v = m;
	return v;
}

Matrix Camera::Perspective(float fov, float aspect, float nearPanel, float farPanel)//视场角，长宽比，近平面，远平面
{
	float tanValue = tan(0.5f * fov * PI / 180);
	Matrix proj;
	proj.value[0][0] = 1 / (tanValue * aspect);
	proj.value[1][1] = tanValue;
	proj.value[2][2] = (nearPanel+farPanel)/(nearPanel-farPanel);
	proj.value[2][3] = 2 * nearPanel * farPanel / (nearPanel - farPanel);
	proj.value[3][2] = 1;
	proj.value[3][3] = 0;
	p = proj;
	return proj;
}

Matrix Camera::Perspective(float l, float r, float n, float f, float t, float b)//视锥体[left,right,near,far,top,bottom]
{
	Matrix m;
	m.value[0][0] = 2 * n / (r - l);	m.value[0][1] = 0;					m.value[0][2] = (l + r) / (l - r);		m.value[0][3] = 0;
	m.value[1][0] = 0;					m.value[1][1] = 2 * n / (t - b);	m.value[1][2] = (b + t) / (b - t);		m.value[1][3] = 0;
	m.value[2][0] = 0;					m.value[2][1] = 0;					m.value[2][2] = (n + f) / (n - f);      m.value[2][3] = 2 * n * f / (f - n);
	m.value[3][0] = 0;					m.value[3][1] = 0;					m.value[3][2] = 1;	                    m.value[3][3] = 0;
	p = m;
	return m;
}

Matrix Camera::Orthographic(float l, float r, float n, float f, float t, float b)
{
	Matrix m;
	m.value[0][0] = 2 / (r - l);	m.value[0][1] = 0;				m.value[0][2] = 0;				m.value[0][3] = -(r + l) / (r - l);
	m.value[1][0] = 0;				m.value[1][1] = 2 / (t - b);	m.value[1][2] = 0;				m.value[1][3] = -(t + b) / (t - b);
	m.value[2][0] = 0;				m.value[2][1] = 0;				m.value[2][2] = 2 / (n - f);    m.value[2][3] = -(n + f) / (n - f);
	m.value[3][0] = 0;				m.value[3][1] = 0;				m.value[3][2] = 0;				m.value[3][3] = 1;
	p = m;
	return m;
}
