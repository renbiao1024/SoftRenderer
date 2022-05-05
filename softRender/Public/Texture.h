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

	void LoadTexture(const char*path);//导入材质
	Color Sample(float u, float v);//离散采样
	float Clamp(float min,float max,float value);//限定范围
};