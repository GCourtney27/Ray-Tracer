//#include "Vector3.h"
//
//inline std::istream& operator>>(std::istream& is, Vector3& t)
//{
//	is >> t.e[0] >> t.e[1] >> t.e[2];
//	return is;
//}
//
//inline std::ostream& operator<<(std::iostream &os, const Vector3 &t)
//{
//	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
//	return os;
//}
//
//inline void Vector3::make_unit_vector()
//{
//	float k = 1.0f / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
//	e[0] *= k;
//	e[1] *= k;
//	e[2] *= k;
//}
//
//inline Vector3 operator+(const Vector3 &v1, const Vector3 &v2)
//{
//	return Vector3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
//}
//
//inline Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
//{
//	return Vector3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
//}
//
//inline Vector3 operator*(const Vector3 &v1, const Vector3 &v2)
//{
//	return Vector3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
//}
//
//inline Vector3 operator/(const Vector3 &v1, const Vector3 &v2)
//{
//	return Vector3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
//}
//
//inline Vector3 operator*(float t, const Vector3 &v)
//{
//	return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
//}
//
//inline Vector3 operator/(Vector3 v, float t)
//{
//	return Vector3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
//}
//
//inline Vector3 operator*(const Vector3& v, float t)
//{
//	return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
//}
//
//inline float dot(const Vector3& v1, const Vector3& v2)
//{
//	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
//}
//
//inline Vector3 cross(const Vector3& v1, const Vector3& v2)
//{
//	return Vector3(
//		( v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1] ),
//		( -( v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0] ) ),
//		( v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0] )
//	);
//}
//
//inline Vector3& Vector3::operator+=(const Vector3& v)
//{
//	e[0] += v.e[0];
//	e[1] += v.e[1];
//	e[2] += v.e[2];
//	return *this;
//}
//
//inline Vector3& Vector3::operator*=(const Vector3& v)
//{
//	e[0] *= v.e[0];
//	e[1] *= v.e[1];
//	e[2] *= v.e[2];
//	return *this;
//}
//
//inline Vector3& Vector3::operator/=(const Vector3& v)
//{
//	e[0] /= v.e[0];
//	e[1] /= v.e[1];
//	e[2] /= v.e[2];
//	return *this;
//}
//
//inline Vector3& Vector3::operator-=(const Vector3& v)
//{
//	e[0] -= v.e[0];
//	e[1] -= v.e[1];
//	e[2] -= v.e[2];
//	return *this;
//}
//
//inline Vector3& Vector3::operator*=(const float t)
//{
//	e[0] *= t;
//	e[1] *= t;
//	e[2] *= t;
//	return *this;
//}
//
//inline Vector3& Vector3::operator/=(const float t)
//{
//	float k = 1.0f / t;
//
//	e[0] *= k;
//	e[1] *= k;
//	e[2] *= k;
//	return *this;
//}
//
//inline Vector3 unit_vector(Vector3 v)
//{
//	return v / v.length();
//}