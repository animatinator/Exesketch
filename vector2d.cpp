// 2D vector


#include "vector2d.h"


Vector2d::Vector2d():
	x(0),
	y(0)
{}

Vector2d::Vector2d(float vx, float vy):
	x(vx),
	y(vy)
{}

Vector2d::Vector2d(const Vector2d& another):
	x(another.x),
	y(another.y)
{}

Vector2d Vector2d::operator+(const Vector2d& rhs) const
{
	float newx, newy;
	
	newx = x+rhs.x;
	newy = y+rhs.y;
	
		return Vector2d(newx, newy);
}

Vector2d Vector2d::operator-(const Vector2d& rhs) const
{
	float newx, newy;
	
	newx = x-rhs.x;
	newy = y-rhs.y;
	
		return Vector2d(newx, newy);
}

Vector2d Vector2d::operator-() const
{
	return Vector2d(-x, -y);
}

Vector2d Vector2d::operator*(const float& rhs) const
{
	return Vector2d(x * rhs, y * rhs);
}

Vector2d Vector2d::operator/(const float& rhs) const
{
	return Vector2d(x / rhs, y / rhs);
}

void Vector2d::operator+=(const Vector2d& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vector2d::operator-=(const Vector2d& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

Vector2d Vector2d::operator=(const Vector2d& rhs)
{
	x = rhs.x;
	y = rhs.y;
}

bool Vector2d::operator==(const Vector2d& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

float Vector2d::operator*(const Vector2d& rhs) const
{
	return DotProduct((*this), rhs);
}


float Vector2d::DotProduct(const Vector2d& vec1, const Vector2d& vec2)
{
	return ((vec1.x*vec2.x)+(vec1.y*vec2.y));
}

float Vector2d::Magnitude(const Vector2d& vector)
{
	return sqrt((vector.x*vector.x) + (vector.y*vector.y));
}

Vector2d Vector2d::Normalise(const Vector2d& vector)
{
	float mag = Magnitude(vector);
	
		return Vector2d((vector.x / mag), (vector.y / mag));
}

float Vector2d::Distance(const Vector2d& vec1, const Vector2d& vec2)
{
	return sqrt(((vec2.x-vec1.x)*(vec2.x-vec1.x))+((vec2.y-vec1.y)*(vec2.y-vec1.y)));
}


ostream& operator<<(ostream& os, const Vector2d& vector)
{
	os << "Vector2d object: (" << vector.x << ", " << vector.y << ")";
	
		return os;
}
