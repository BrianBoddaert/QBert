#include "Structs.h"

using namespace dae;

Vector3 dae::operator-(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3 dae::operator*(const Vector3& a, float scalar)
{
	return Vector3(a.x * scalar, a.y * scalar, a.z * scalar);
}
Vector2 dae::operator-(const Vector2& a, const Vector2& b)
{
	return Vector2(a.x - b.x, a.y - b.y);
}


Vector2 dae::operator+(const Vector2& a, const Vector2& b)
{
	return Vector2(a.x + b.x, a.y + b.y);
}
Vector2 dae::operator*(const Vector2& a, float scalar)
{
	return Vector2(a.x * scalar, a.y * scalar);
}