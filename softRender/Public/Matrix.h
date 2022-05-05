#pragma once
#include "Vector3.h"

class Matrix
{
public:
	float value[4][4];
	Matrix();
	~Matrix();

	Matrix operator + (const Matrix& right) const;
	Matrix operator - (const Matrix& right) const;
	Matrix operator * (const Matrix& right) const;
	Matrix operator * (float k) const;

	Vector3f MultiplyVector3(const Vector3f& v) const;

	void Identity();
	Matrix transpose() const //×ªÖÃ
	{
		Matrix trans;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				trans.value[i][j] = value[j][i];
			}
		}
		return trans;
	}
	void Print();
};