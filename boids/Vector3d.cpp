#include <math.h> 
#include "Vector3d.h"

Vector3d::Vector3d()
{
}

Vector3d::Vector3d(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3d::~Vector3d()
{
}

Vector3d Vector3d::operator+(Vector3d vec)
{
	Vector3d result;
	
	result.x = x + vec.x;
	result.y = y + vec.y;
	result.z = z + vec.z;

	return result;
}

Vector3d Vector3d::operator-(Vector3d vec)
{
	Vector3d result;
	
	result.x = x - vec.x;
	result.y = y - vec.y;
	result.z = z - vec.z;

	return result;
}

Vector3d Vector3d::operator*(float factor)
{
	Vector3d result;
	
	result.x = x * factor;
	result.y = y * factor;
	result.z = z * factor;

	return result;
}

Vector3d Vector3d::operator/(float div)
{
	Vector3d result;
	
	result.x = x / div;
	result.y = y / div;
	result.z = z / div;

	return result;
}

float Vector3d::length()
{
	return sqrt( x*x + y*y + z*z );
}

Vector3d Vector3d::rotate(float angle)
{
	Vector3d result;

	result.x = x * cos(angle) + y * sin(angle);
	result.y = -1 * x * sin(angle) + y * cos(angle);
	result.z = z;

	return result;
}
