#pragma once

#include <math.h>
#include <vector>
using std::vector;


namespace utility
{
	// 2���� ���� ����ü.
	struct sVector
	{
		float x, y;
		sVector() {}
		sVector(float _x, float _y) : x(_x), y(_y) {}
		sVector& operator=(const sVector &rhs) // �Ҵ� ������.
		{
			if (this != &rhs)
			{
				x = rhs.x;
				y = rhs.y;
			}
			return *this;
		}
		sVector operator-(const sVector &rhs) const // - ������.
		{
			sVector v;
			v.x = x - rhs.x;
			v.y = y - rhs.y;
			return v;
		}
		sVector operator+(const sVector &rhs) const // + ������
		{
			sVector v;
			v.x = x + rhs.x;
			v.y = y + rhs.y;
			return v;
		}
		sVector operator*(const sVector &rhs) const // * ������
		{
			sVector v;
			v.x = x * rhs.x;
			v.y = y * rhs.y;
			return v;
		}
		template<class T>
		sVector operator*(const T &f) const // ��Į�� �� ������.
		{
			sVector v;
			v.x = x * f;
			v.y = y * f;
			return v;
		}
		float length() { return (float)sqrt(x*x + y*y); } // ���� ���� ����
		void normalize() // ���� ����ȭ 
		{
			const float L = length();
			x /= L;
			y /= L;
		}
		void interpol( const sVector &from, const sVector &to, float f ) // ���� ����.
		{
			*this = from*(1.f-f) + to*f;
			normalize();
		}
	};


	void lerp(sVector &out, const sVector &a, const sVector &b, float t);
	void bezier(sVector &out, const vector<sVector> &points, const float t);

}
