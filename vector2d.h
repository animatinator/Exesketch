// 2D vector

#ifndef Vector2d_H
#define Vector2d_H

#include <iostream>
#include <math.h>

using namespace std;


class Vector2d
{
	public:
		float x, y;
		
		Vector2d();
		Vector2d(float vx, float vy);
		Vector2d(const Vector2d& another);
		
		Vector2d operator+(const Vector2d& rhs) const;
		Vector2d operator-(const Vector2d& rhs) const;
		Vector2d operator-() const;
		Vector2d operator*(const float& rhs) const;
		Vector2d operator/(const float& rhs) const;
		void operator+=(const Vector2d& rhs);
		void operator-=(const Vector2d& rhs);
		Vector2d operator=(const Vector2d& rhs);
		bool operator==(const Vector2d& rhs) const;
		float operator*(const Vector2d& rhs) const;
		
		static float DotProduct(const Vector2d& vec1, const Vector2d& vec2);
		static float Magnitude(const Vector2d& vector);
		static Vector2d Normalise(const Vector2d& vector);
		static float Distance(const Vector2d& vec1, const Vector2d& vec2);
		
		friend ostream& operator<<(ostream& os, const Vector2d& vector);
};

#endif
