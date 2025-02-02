//#pragma once
#ifndef HITABLEH
#define HITABLEH
#include "Ray.h"

class Material;

struct hit_record
{
	float t = 0.0f;
	Vector3 p;
	Vector3 normal;
	Material* mat_ptr = nullptr;
};

class Hitable {
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif // !HITABLEH
