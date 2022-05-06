#pragma once
#include "Vertex.h"
#include "Light.h"
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "Buffer.h"

using namespace std;

class Renderer
{
private:
	int deviceWidth;
	int deviceHeight;
	HDC screenHDC;
	Texture* tex;
public:
	vector<DirectionLight>directionlights;
	vector<PointLight>pointlights;
	Camera* camera;

public:
	Renderer(HDC hdc, int screenWidth, int screenHeight, Camera* cam);
	~Renderer();
	void Clear(DepthBuffer* zbuffer);

	void DrawByIndex(Mesh *m, IShader *shader, DepthBuffer *zbuffer);
	void DrawByArray(Mesh *m, IShader *shader, DepthBuffer *zbuffer);
	void DrawMesh(Mesh* m, IShader* shader, DepthBuffer* zbuffer);
	void DrawPrimitive(Vertex v0, Vertex v1, Vertex v2, IShader* shader, DepthBuffer* zbuffer);
	void RasterizeTrangle(Vertex v0, Vertex v1, Vertex v2, IShader* shader, DepthBuffer* zbuffer);
	void DrawTopFlatTrangle(Vertex v0, Vertex v1, Vertex v2, IShader* shader, DepthBuffer* zbuffer, Vertex v3, int swapIndex);
	void DrawBottomFlatTrangle(Vertex v0, Vertex v1, Vertex v2, IShader* shader, DepthBuffer* zbuffer, Vertex v3, int swapIndex);
	void DrawLine(Vertex v0, Vertex v1, IShader* shader, DepthBuffer* zbuffer, Vector3f p0, Vector3f p1, Vector3f p2);
	void DrawPixel(int x, int y, const Color& color);
	bool ZTestAndWrite(int x, int y, float depth, DepthBuffer* zbuffer);
	bool SimpleCVVCullCheck(const Vertex& vertex);	//��CVV�޳���ֻ��������������ڵ������δ���߽����������¹���
	void PrepareRasterization(Vertex& vertex, Buffer* buffer);	//׼����դ����͸��ͶӰ�������ӿ�ӳ�䣬����������׼��

};