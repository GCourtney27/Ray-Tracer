//#pragma once
#ifndef CAMERAH
#define CAMERAH
#include "Ray.h"

Vector3 random_in_unit_disk()
{
	Vector3 p;
	do {
		p = 2.0f * Vector3((rand() / (float)RAND_MAX), (rand() / (float)RAND_MAX), 0.0f) - Vector3(1.0f, 1.0f, 0.0f);
	} while (dot(p, p) >= 1.0f);
	return p;
}

class Camera
{
public:
	Camera() {}
	/*Camera(Vector3 lookFrom, Vector3 lookAt, Vector3 vup, float vfov, float aspect, float aperture, float focas_dist)
	{
		lens_radius = aperture / 2.0f;
		float theta = vfov * M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookFrom;
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - half_width * focas_dist * u - half_height * focas_dist * v - focas_dist * w;
		horizontal = 2 * half_width * focas_dist * u;
		vertical = 2 * half_height * focas_dist * v;
	}*/

	void Initialize(Vector3 lookFrom, Vector3 lookAt, Vector3 vup, float vfov, float aspect, float aperture, float focas_dist)
	{
		lens_radius = aperture / 2.0f;
		float theta = vfov * M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookFrom;
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - half_width * focas_dist * u - half_height * focas_dist * v - focas_dist * w;
		horizontal = 2 * half_width * focas_dist * u;
		vertical = 2 * half_height * focas_dist * v;
	}

	Ray get_ray(float u, float v)
	{
		Vector3 rd = lens_radius * random_in_unit_disk();
		float o = u * rd.x() + v * rd.y();
		Vector3 offset(o, o, o);
		return Ray(origin + offset, lower_left_corner + u * horizontal + v * vertical - origin - offset);
	}

	Vector3 origin;
	Vector3 lower_left_corner;
	Vector3 horizontal;
	Vector3 vertical;
	Vector3 u, v, w;
	float lens_radius;


	float M_PI = 3.1415f;
};

#endif // !CAMERAH
