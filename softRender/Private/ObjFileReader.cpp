#include "ObjFileReader.h"

//obj�ļ���ʽ��һ��ʼ��������˵�����ݣ������ͺã�v��ͷ�ı���Ƕ���λ�����ݣ�vt��ָUV���꣬vn�Ƿ��߷���
//			f�Ƕ�Ӧ�õ��� v/vt/vn ��indexֵ��ʵ������6�飬ÿ��4������ζ������������������棬ÿ�������������Σ������������غϣ�
//# Blender v2.83.5 OBJ File : ''
//# www.blender.org
//mtllib Cube.mtl
//o Cube
//v 1.000000 1.000000 - 1.000000
//v 1.000000 - 1.000000 - 1.000000
//v 1.000000 1.000000 1.000000
//v 1.000000 - 1.000000 1.000000
//v - 1.000000 1.000000 - 1.000000
//v - 1.000000 - 1.000000 - 1.000000
//v - 1.000000 1.000000 1.000000
//v - 1.000000 - 1.000000 1.000000
//vt 0.625000 0.500000
//vt 0.875000 0.500000
//vt 0.875000 0.750000
//vt 0.625000 0.750000
//vt 0.375000 0.750000
//vt 0.625000 1.000000
//vt 0.375000 1.000000
//vt 0.375000 0.000000
//vt 0.625000 0.000000
//vt 0.625000 0.250000
//vt 0.375000 0.250000
//vt 0.125000 0.500000
//vt 0.375000 0.500000
//vt 0.125000 0.750000
//vn 0.0000 1.0000 0.0000
//vn 0.0000 0.0000 1.0000
//vn - 1.0000 0.0000 0.0000
//vn 0.0000 - 1.0000 0.0000
//vn 1.0000 0.0000 0.0000
//vn 0.0000 0.0000 - 1.0000
//usemtl Material
//s off
//f 1/1/1 5/2/1 7/3/1 3/4/1
//f 4/5/2 3/4/2 7/6/2 8/7/2
//f 8/8/3 7/9/3 5/10/3 6/11/3
//f 6/12/4 2/13/4 4/5/4 8/14/4
//f 2/13/5 1/1/5 3/4/5 4/5/5
//f 6/11/6 5/10/6 1/1/6 2/13/6

void StringSplit(string s, char splitchar, vector<string>& vec)
{
	if(vec.size() > 0) vec.clear();
	int length = s.length();
	int start = 0;
	for (int i = 0; i < length; ++i)
	{
		if(s[i] == splitchar && i == 0) ++start;//��һ���������ָ��
		else if (s[i] == splitchar)
		{
			vec.push_back(s.substr(start,i-start));
			start = i + 1;
		}
		else if (i == length - 1)
		{
			vec.push_back(s.substr(start,i+1-start));
		}
	}
}

void ReadObjFile(string path, Mesh* obj)
{
	ifstream in(path);
	string txt = "";
	if (in)
	{
		while (getline(in, txt))
		{
			if (txt[0] == 'v' && txt[1] == ' ')//��������
			{
				vector<string>num;
				txt.erase(0,2);
				StringSplit(txt,' ',num);
				Vector3f pos = Vector3f((float)atof(num[0].c_str()),(float)atof(num[1].c_str()),(float)atof(num[1].c_str()));
				obj->positionBuffer.push_back(pos);
			}
			else if (txt[0] == 'v' && txt[1] == 'n')//���㷨��
			{
				vector<string>num;
				txt.erase(0,3);
				StringSplit(txt,' ',num);
				Vector3f n = Vector3f((float)atof(num[0].c_str()), (float)atof(num[1].c_str()), (float)atof(num[1].c_str()),0.0);
				obj->normalBuffer.push_back(n);
			}
			else if (txt[0] == 'v' && txt[1] == 't')//uv
			{
				vector<string> num;
				txt.erase(0, 3);
				StringSplit(txt, ' ', num);
				obj->uvBuffer.push_back(Vector2((float)atof(num[0].c_str()), (float)atof(num[1].c_str())));
			}
			else if (txt[0] == 'f' && txt[1] == ' ')//������ v/vt/vn
			{
				vector<string>num;
				txt.erase(0,2);
				StringSplit(txt,' ',num);
				for (int i = 0; i < num.size();++i)
				{
					vector<string>threeIndex;
					StringSplit(num[i],'/',threeIndex);
					Vector3i indexes = { atoi(threeIndex[0].c_str()), atoi(threeIndex[1].c_str()), atoi(threeIndex[2].c_str()) };
					obj->indexBuffer.push_back(indexes);
				}
			}
		}
	}
	else
	{
		cout << "no file" << endl;
	}
}

