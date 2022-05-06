#include "Renderer.h"
#include "Matrix.h"
#include <iostream>
using namespace std;
Renderer::Renderer(HDC hdc, int screenWidth, int screenHeight, Camera* cam)
{
	screenHDC = hdc;
	deviceWidth = screenWidth;
	deviceHeight = screenHeight;
	camera = cam;


	tex = new Texture();
	tex->LoadTexture("gezi.bmp");
}


Renderer::~Renderer()
{
}


void Renderer::DrawByIndex(Mesh* m, IShader* shader, DepthBuffer* zbuffer)
{

	for (int i = 0; i < m->indexBuffer.size(); i = i + 4)
	{
		Vertex p1;
		p1.pos = m->positionBuffer[m->indexBuffer[i].x - 1];
		p1.uv = m->uvBuffer[m->indexBuffer[i].y - 1];
		p1.normal = m->normalBuffer[m->indexBuffer[i].z - 1];
		Vertex p2;
		p2.pos = m->positionBuffer[m->indexBuffer[i + 1].x - 1];
		p2.uv = m->uvBuffer[m->indexBuffer[i + 1].y - 1];
		p2.normal = m->normalBuffer[m->indexBuffer[i + 1].z - 1];
		Vertex p3;
		p3.pos = m->positionBuffer[m->indexBuffer[i + 2].x - 1];
		p3.uv = m->uvBuffer[m->indexBuffer[i + 2].y - 1];
		p3.normal = m->normalBuffer[m->indexBuffer[i + 2].z - 1];

		Vertex p4;
		p4.pos = m->positionBuffer[m->indexBuffer[i + 3].x - 1];
		p4.uv = m->uvBuffer[m->indexBuffer[i + 3].y - 1];
		p4.normal = m->normalBuffer[m->indexBuffer[i + 3].z - 1];

		DrawPrimitive(p1, p2, p3, shader, zbuffer);
		DrawPrimitive(p1, p3, p4, shader, zbuffer);
	}
}

void Renderer::DrawByArray(Mesh* m, IShader* shader, DepthBuffer* zbuffer)
{
	for (int i = 0; i < m->vertexBuffer.size(); i = i + 3)
	{
		Vertex p1 = m->vertexBuffer[i];
		Vertex p2 = m->vertexBuffer[i + 1];
		Vertex p3 = m->vertexBuffer[i + 2];

		DrawPrimitive(p1, p2, p3, shader, zbuffer);
	}
}

void Renderer::DrawMesh(Mesh* m, IShader* shader, DepthBuffer* zbuffer)
{
	if (m->indexBuffer.size() > 0)
		DrawByIndex(m, shader, zbuffer);
	else
		DrawByArray(m, shader, zbuffer);
}

//������Ҫ����ֵ���ݣ���Ϊ���ظ�
void Renderer::DrawPrimitive(Vertex v1, Vertex v2, Vertex v3, IShader* shader, DepthBuffer* zbuffer)
{

	shader->VS(v1, v2, v3);

	//����CVV���޳��ж�
	//if (SimpleCVVCullCheck(v1) && SimpleCVVCullCheck(v2) && SimpleCVVCullCheck(v3))
	//	return;

	//͸�ӳ������ӿ�ӳ�䣬����׼����ȫ����Ϊ1/z��
	PrepareRasterization(v1, zbuffer);
	PrepareRasterization(v2, zbuffer);
	PrepareRasterization(v3, zbuffer);
	//�������������غ����Դ������⣬ֱ�Ӳ���Ⱦ,��������
	if (((int)v1.pos.y == (int)v2.pos.y && abs(v2.pos.y - v3.pos.y) <= 1) ||
		((int)v3.pos.y == (int)v2.pos.y && abs(v2.pos.y - v1.pos.y) <= 1) ||
		((int)v1.pos.y == (int)v3.pos.y && abs(v2.pos.y - v3.pos.y) <= 1))
		return;

	RasterizeTrangle(v1, v2, v3, shader, zbuffer);
}

void Renderer::RasterizeTrangle(Vertex v0, Vertex v1, Vertex v2, IShader* shader, DepthBuffer* zbuffer)
{

	int ty0 = v0.pos.y;
	int ty1 = v1.pos.y;
	int ty2 = v2.pos.y;
	if (ty0 == ty1)	//��������
	{
		DrawTopFlatTrangle(v0, v1, v2, shader, zbuffer, Vertex(), -1);
	}
	else if (ty1 == ty2) //��������
	{

		DrawBottomFlatTrangle(v0, v1, v2, shader, zbuffer, Vertex(), -1);
	}
	else//���Ϊһ��ƽ�������κ�һ��ƽ��������
	{
		//���ĵ�Ϊֱ��(x0, y0)��(x2, y2)��ȡy1�ĵ�
		float x3 = (v1.pos.y - v0.pos.y) * (v2.pos.x - v0.pos.x) / (v2.pos.y - v0.pos.y) + v0.pos.x;
		float y3 = v1.pos.y;
		float t = (y3 - v0.pos.y) / (v2.pos.y - v0.pos.y);

		Vertex v3(Vector3f(x3, y3, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		v3.LerpVertexData(v0, v2, t);
		DrawBottomFlatTrangle(v0, v1, v3, shader, zbuffer, v2, 3);
		DrawTopFlatTrangle(v3, v1, v2, shader, zbuffer, v0, 1);
	}
}

void Renderer::DrawTopFlatTrangle(Vertex v0, Vertex v1, Vertex v2, IShader* shader, DepthBuffer* zbuffer, Vertex v3, int swapIndex)
{
	float x0 = v0.pos.x;
	float y0 = v0.pos.y;
	float x1 = v1.pos.x;
	float y1 = v1.pos.y;
	float x2 = v2.pos.x;
	float y2 = v2.pos.y;
	for (float y = y0; y <= y2; y++)
	{
		float t = (y - y0) / (y2 - y0);
		//��int����Ȼ���ж���
		int xl = (y - y0) * (x2 - x0) / (y2 - y0) + x0;
		Vertex vl(Vector3f(xl, y, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		vl.LerpVertexData(v0, v2, t);
		int xr = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
		Vertex vr(Vector3f(xr, y, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		vr.LerpVertexData(v1, v2, t);

		switch (swapIndex)
		{
		case -1:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v1.pos, v2.pos);
			break;
		case 1:
			DrawLine(vl, vr, shader, zbuffer, v3.pos, v1.pos, v2.pos);
			break;
		case 2:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v3.pos, v2.pos);
			break;
		case 3:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v1.pos, v3.pos);
			break;
		default:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v1.pos, v2.pos);
			break;
		}

	}
}

void Renderer::DrawBottomFlatTrangle(Vertex v0, Vertex v1, Vertex v2, IShader* shader, DepthBuffer* zbuffer, Vertex v3, int swapIndex)
{
	float x0 = v0.pos.x;
	float y0 = v0.pos.y;
	float x1 = v1.pos.x;
	float y1 = v1.pos.y;
	float x2 = v2.pos.x;
	float y2 = v2.pos.y;
	for (float y = y0; y <= y1; y++)
	{
		float t = (y - y0) / (y2 - y0);

		int xl = ((y - y1) * (x0 - x1) / (y0 - y1) + x1);
		Vertex vl(Vector3f(xl, y, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		vl.LerpVertexData(v0, v1, t);

		int xr = ((y - y2) * (x0 - x2) / (y0 - y2) + x2);
		Vertex vr(Vector3f(xr, y, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		vr.LerpVertexData(v0, v2, t);

		switch (swapIndex)
		{
		case -1:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v1.pos, v2.pos);
			break;
		case 1:
			DrawLine(vl, vr, shader, zbuffer, v3.pos, v1.pos, v2.pos);
			break;
		case 2:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v3.pos, v2.pos);
			break;
		case 3:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v1.pos, v3.pos);
			break;
		default:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v1.pos, v2.pos);
			break;
		}
	}
}


//yֵ��Զ��ȵ���
void Renderer::DrawLine(Vertex v0, Vertex v1, IShader* shader, DepthBuffer* zbuffer, Vector3f p0, Vector3f p1, Vector3f p2)
{

	float x0 = v0.pos.x;
	float x1 = v1.pos.x;
	float y0 = v0.pos.y;
	float y1 = v1.pos.y;
	//ֻ����x����ɨ���߼���
	int dx = x1 - x0;
	//	int dy = y1 - y0;
	// 	if(dy!=0)
	// 	cout << dy;
	int stepx = 1;
	//	int stepy = 1;

	if (dx < 0)
	{
		stepx = -1;
		dx = -dx;
	}


	int x = x0;
	int y = y0;

	Vertex frag;
	if (x1 == x0)
	{
		if (ZTestAndWrite(x, y, (v0.pos.z + 1) / 2.0, zbuffer))
		{
			Vector3f g = centerOfGravity(p0, p1, p2, Vector2(x, y));
			if (shader->FS(v0, g))
			{
				DrawPixel(x, y, v0.color);
			}
		}

		return;
	}
	for (int i = 0; i <= dx; i++)
	{
		float s = (x - x0) / (x1 - x0);
		//͸�ӽ���,https://zhuanlan.zhihu.com/p/144331875
		float t = s * v0.pos.z / (s * v0.pos.z + (1 - s) * v1.pos.z);
		float z = Vertex::LerpFloat(v0.pos.z, v1.pos.z, t);
		z = (z + 1) / 2.0;
		//		Vector3f n = Vector3f::Lerp(v0.normal, v1.normal, t);
		//		Vector3f v = Vector3f::Lerp(v0.pos, v1.pos, t);
		//		float realz = 1.0f / z;
				//cout << " " << v0.pos.z << " " << v1.pos.z << " " << z << endl;
				//std::cout <<  t<< std::endl;
		if (ZTestAndWrite(x, y, z, zbuffer))
		{
			Color c = Color::Lerp(v0.color, v1.color, t);
			float u = Vertex::LerpFloat(v0.uv.x, v1.uv.x, t);
			float v = Vertex::LerpFloat(v0.uv.y, v1.uv.y, t);
			frag.pos = Vector3f(x, y, z);
			frag.color = c;
			frag.uv = Vector2(u, v);
			Vector3f g = centerOfGravity(p0, p1, p2, Vector2(x, y));
			if (shader->FS(frag, g))
			{
				DrawPixel(x, y, frag.color);
			}

		}
		x += stepx;
	}
}

bool Renderer::ZTestAndWrite(int x, int y, float depth, DepthBuffer* zbuffer)
{

	if (x >= 0 && x < zbuffer->width && y >= 0 && y < zbuffer->height)
	{
		if (zbuffer->depthBuffer[y][x] >= depth)
		{
			zbuffer->depthBuffer[y][x] = depth;
			return true;
		}
	}
	return false;
}

inline void Renderer::DrawPixel(int x, int y, const Color& color)
{
	SetPixel(screenHDC, x, y, RGB(255 * color.r, 255 * color.g, 255 * color.b));
}




void Renderer::Clear(DepthBuffer* zbuffer)
{
	BitBlt(screenHDC, 0, 0, deviceWidth, deviceHeight, NULL, NULL, NULL, BLACKNESS);
	for (int i = 0; i < zbuffer->height; i++)
	{
		for (int j = 0; j < zbuffer->width; j++)
		{
			zbuffer->depthBuffer[i][j] = 1;
		}
	}
}




inline void Renderer::PrepareRasterization(Vertex& vertex, Buffer* buffer)
{

	float reciprocalW = 1.0f / vertex.pos.w;
	//����0.5��Ϊ�˺���ȡ֤����������
	vertex.pos.x = (vertex.pos.x * reciprocalW + 1.0f) * 0.5f * (buffer->width - 1) + 0.5;
	vertex.pos.y = (vertex.pos.y * reciprocalW + 1.0f) * 0.5f * (buffer->height - 1) + 0.5;

}

inline bool Renderer::SimpleCVVCullCheck(const Vertex& vertex)
{
	float w = vertex.pos.w;
	if (vertex.pos.x < -w || vertex.pos.x > w)
		return true;
	if (vertex.pos.y < -w || vertex.pos.y > w)
		return true;
	if (vertex.pos.z < 0.0f || vertex.pos.z > w)
		return true;
	return false;
}
