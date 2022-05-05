#pragma once
#include "Color.h"

class Texture
{
private:
	int width,height;
	Color textureData[1024][1024];

public:
	Texture();
	~Texture();

	void LoadTexture(const char*path);//�������
	Color Sample(float u, float v);//��ɢ����
	float Clamp(float min,float max,float value);//�޶���Χ
};