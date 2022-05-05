#include "Shader.h"
using namespace std;

void PhongShader::VS(Vertex& v0, Vertex& v1, Vertex& v2)
{
	v2f.v0 = v0;
	v2f.v1 = v1;
	v2f.v2 = v2;
	Vertex* v = &v0;
	for (int i = 0; i < 3; ++i)
	{
		switch (i)//对光源做mvp变换
		{
			case 0:
				v = &v0;
				v2f.lp0 = v0.pos;
				v2f.lp0 = v2f.m.MultiplyVector3(v2f.lp0);
				v2f.lp0 = v2f.lightV.MultiplyVector3(v2f.lp0);
				v2f.lp0 = v2f.lightP.MultiplyVector3(v2f.lp0);
				break;
			case 1:
				v = &v1;
				v2f.lp1 = v1.pos;
				//v2f.lp1.Print();
				v2f.lp1 = v2f.m.MultiplyVector3(v2f.lp1);
				v2f.lp1 = v2f.lightV.MultiplyVector3(v2f.lp1);
				v2f.lp1 = v2f.lightP.MultiplyVector3(v2f.lp1);
				break;
			case 2:
				v = &v2;
				v2f.lp2 = v2.pos;
				v2f.lp2 = v2f.m.MultiplyVector3(v2f.lp2);
				v2f.lp2 = v2f.lightV.MultiplyVector3(v2f.lp2);
				v2f.lp2 = v2f.lightP.MultiplyVector3(v2f.lp2);
				break;
		}
		v->pos = v2f.m.MultiplyVector3(v->pos);
		Matrix nm = v2f.m;
		//去掉平移.不考虑变形缩放的情况下使用，正常情况下使用逆的转置
		nm.value[0][3] = 0;
		nm.value[1][3] = 0;
		nm.value[2][3] = 0;
		v->normal = nm.MultiplyVector3(v->normal).Normalize();//法线变换到世界坐标

		float diffuse = 0.2,specular = 0.2;//漫反射，镜面
		//diffuse + specular超过1会出现渲染错误
		float ambient = 0.1;//环境光

		////平行光源
		//for (auto light : v2f.dirlights)
		//{
		////最终颜色 = 直射光颜色 * pow(max(0, dot(反射光方向, 视野方向)), 光泽度(gloss)) + 漫反射颜色(dot(光线，法线)*光强) + 环境光颜色
		//	Vector3f l = light.GetDirection().Normalize();//光线的反向向量
		//	diffuse += max(0,l.Dot(v->normal))*light.intensity;
		//	Vector3f h = ((v2f.cameraPos - v->pos).Normalize() + l).Normalize();//视线和光线的半角向量
		//	specular += pow(max(0,v->normal.Dot(h)),1)*light.intensity;
		//}

		//点光源
		for (auto light : v2f.pointlights)
		{
			Vector3f l = (light.position - v->pos).Normalize();
			diffuse += max(0,l.Dot(v->normal))*light.intensity*min(1,1/(light.position - v->pos).Magnitude());
			Vector3f h = ((v2f.cameraPos - v->pos).Normalize() + (light.position - v->pos).Normalize()).Normalize();
			specular += pow(max(0,v->normal.Dot(h)),1)*light.intensity * min(1,1/(light.position - v->pos).Magnitude());
		}

		v->color = v->color * min(1,specular+diffuse+ambient);
		v->pos = v2f.v.MultiplyVector3(v->pos);
		v->pos = v2f.p.MultiplyVector3(v->pos);
		v->pos.standardization();//v在投影坐标
	}
	if (v1.pos.y < v0.pos.y)
	{
		std::swap(v2f.lp0, v2f.lp1);
		std::swap(v1, v0);
	}

	if (v2.pos.y < v0.pos.y)
	{
		std::swap(v2f.lp0, v2f.lp2);
		std::swap(v2, v0);
	}

	if (v2.pos.y < v1.pos.y)
	{
		std::swap(v2f.lp1, v2f.lp2);
		std::swap(v2, v1);
	}
}

bool PhongShader::FS(Vertex& v, Vector3f g)
{
	v.color = v.color * frag.baseTex->Sample(v.uv.x, v.uv.y);
	Vector3f normal = v2f.v0.normal * g.x + v2f.v1.normal * g.y + v2f.v2.normal * g.z;
	Vector3f posLightSpace = v2f.lp0 * g.x + v2f.lp1 * g.y + v2f.lp2 * g.z;
	float bias = 0.005;//偏置
	if (v2f.dirlights.size() > 0)
	{
		bias = max(0.02 * (1.0 - abs(Vector3f::Dot(normal.Normalize() , v2f .dirlights[0].GetDirection().Normalize()))),0.005);
	}
	float depth = CalcuteShadow(posLightSpace,bias);//光源坐标系的深度
	v.color = v.color * (1-depth);
	return v.color.a > 0;//透明度 > 0
}

float PhongShader::CalcuteShadow(Vector3f posLightSpace, double bias)
{
	float reciprocalW = 1.0f/posLightSpace.w; //w的倒数
	posLightSpace.x = (posLightSpace.x * reciprocalW + 1.0f) * 0.5f * (frag.depthBuffer->width  - 1) + 0.5;
	posLightSpace.y = (posLightSpace.y * reciprocalW + 1.0f) * 0.5f * (frag.depthBuffer->height - 1) + 0.5;
	float depth = (posLightSpace.z + 1.0) / 2.0;

	float shadow = 0.0;
	//普通阴影
	//float closestDepth = frag.depthBuffer->Sample(posLightSpace.y, posLightSpace.x);
	//shadow = depth - bias > closestDepth ? 1 : 0;

	//PCF
	for (int x = -10; x <= 10; ++x)
	{
		for (int y = -10; y <= 10; ++y)
		{
			float pcfDepth = frag.depthBuffer->Sample(posLightSpace.y+y, posLightSpace.x+x);
			shadow += depth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 441;
	return shadow;
}

void ShadowShader::VS(Vertex& v0, Vertex& v1, Vertex& v2)
{
	Vertex *v = &v1;
	for (int i = 0; i < 3; ++i)
	{
		switch (i)
		{
			case 0:
				v = &v0;
				break;
			case 1:
				v = &v1;
				break;
			case 2:
				v = &v2;
				break;
		}
		v->pos = v2f.m.MultiplyVector3(v->pos);
		v->pos = v2f.lightV.MultiplyVector3(v->pos);
		v->pos = v2f.lightP.MultiplyVector3(v->pos);
		v->pos.standardization();
	}
	if (v1.pos.y < v0.pos.y)
	{
		std::swap(v1, v0);
	}

	if (v2.pos.y < v0.pos.y)
	{
		std::swap(v2, v0);
	}

	if (v2.pos.y < v1.pos.y)
	{
		std::swap(v2, v1);
	}
}

bool ShadowShader::FS(Vertex& v, Vector3f g)
{
	return false;
}
