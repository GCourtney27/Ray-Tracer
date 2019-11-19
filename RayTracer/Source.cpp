#include <iostream>
#include <fstream>
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include <limits>
#include <random>
#include <thread>
#include <future>

#define MAXFLOAT 0.001f, std::numeric_limits<float>::max()

Vector3 random_in_unit_sphere()
{
	Vector3 p;
	do
	{
		p = 2.0f * Vector3((rand() / (float)RAND_MAX), (rand() / (float)RAND_MAX), (rand() / (float)RAND_MAX)) - Vector3(1.0f, 1.0f, 1.0f);
	} while (p.squared_length() >= 1.0f);
	return p;
}

Vector3 Reflect(const Vector3& v, const Vector3& n)
{
	return v - 2 * dot(v, n) * n;
}

bool Refract(const Vector3& v, const Vector3& n, float ni_over_nt, Vector3& refracted)
{
	Vector3 uv = unit_vector(v);
	float dt = dot(uv, n);

	float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
	{
		return false;
	}
}

float Schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 + r0) * pow((1 - cosine), 5);
}

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const = 0;
};

class Metal : public Material
{
public:
	Metal(const Vector3& a, float f) : albedo(a) { if (f < 1.0f) fuzz = f; else fuzz = 1; }
	virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const
	{
		Vector3 reflected = Reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return(dot(scattered.direction(), rec.normal) > 0);
	}
	Vector3 albedo;
	float fuzz;
};

class Lambertian : public Material
{
public:
	Lambertian(const Vector3& a) : albedo(a) {}
	virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const
	{
		Vector3 target = rec.normal + random_in_unit_sphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
	Vector3 albedo;
};

class Dielectric : public Material
{
public:
	Dielectric(float ri) : ref_idx(ri) {}
	virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const
	{
		Vector3 outward_normal;
		Vector3 reflected = Reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = Vector3(1.0f, 1.0f, 1.0f);
		Vector3 refracted;
		float reflect_prob;
		float cosine;
		if (dot(r_in.direction(), rec.normal) > 0.0f)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0f / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		if (Refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = Schlick(cosine, ref_idx);
		}
		else
		{
			scattered = Ray(rec.p, reflected);
			reflect_prob = 1.0f;
		}
		if (rand() / (float)RAND_MAX < reflect_prob)
		{
			scattered = Ray(rec.p, reflected);
		}
		else
		{
			scattered = Ray(rec.p, refracted);
		}
		return true;
	}

	float ref_idx;
};

Vector3 Color(const Ray& r, Hitable* world, int depth)
{
	hit_record rec;
	if (world->hit(r, MAXFLOAT, rec))
	{
		Ray scattered;
		Vector3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * Color(scattered, world, depth + 1);
		}
		else
		{
			return Vector3(0.0f, 0.0f, 0.0f);
		}
		//Vector3 target = rec.p + rec.normal + random_in_unit_sphere();
		//return 0.5f * Color(Ray(rec.p, target - rec.p), world);
		//Vector3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);//
	}
	else
	{
		Vector3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * Vector3(1.0f, 1.0f, 1.0f) + t * Vector3(0.5f, 0.7f, 1.0f);
	}
}

#define RAND (float)(rand() / (float)RAND_MAX)
Hitable* random_scene()
{
	int n = 500;
	Hitable** list = new Hitable*[n++];
	list[0] = new Sphere(Vector3(0, -1000, 0), 1000, new Lambertian(Vector3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = RAND;
			Vector3 center(a + 0.9f * RAND, 0.2f, b + 0.9f * RAND);
			if ((center - Vector3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new Sphere(
						center, 0.2f,
						new Lambertian(Vector3(RAND * RAND, RAND * RAND, RAND * RAND)));
				}
				else if (choose_mat < 0.95f) { // metal
					list[i++] = new Sphere(
						center, 0.2f,
						new Metal(Vector3(0.5f * (1 + RAND),
							0.5f * (1 + RAND),
							0.5f * (1 + RAND)),
							0.5f * RAND)
					);
				}
				else {  // glass
					list[i++] = new Sphere(center, 0.2f, new Dielectric(1.5f));
				}
			}
		}
	}

	list[i++] = new Sphere(Vector3(0.0f, 1.0f, 0.0f), 1.0f, new Dielectric(1.5f));
	list[i++] = new Sphere(Vector3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(Vector3(0.4f, 0.2f, 0.1f)));
	list[i++] = new Sphere(Vector3(4.0f, 1.0f, 0.0f), 1.0f, new Metal(Vector3(0.7f, 0.6f, 0.5f), 0.0f));

	return new hitable_list(list, i);
}

int nx = 200; // Width
int ny = 100; // Height
int ns = 10; // Number of samples per pixel

int i;
int j;

Camera cam;
Hitable* world;

Vector3 Render()
{
	Vector3 color;

	for (int s = 0; s < ns; s++)
	{
		float u = float(i + (rand() / (float)RAND_MAX) + 1.0f) / float(nx);
		float v = float(j + (rand() / (float)RAND_MAX) + 1.0f) / float(ny);

		Ray r = cam.get_ray(u, v);
		Vector3 p = r.point_at_parameter(2.0f);
		color += Color(r, world, 0);
	}

	return color;
}

int main()
{

	std::ofstream outStream("output.ppm");
	std::cout << "Initializing...";

	
	outStream << "P3\n" << nx << " " << ny << "\n255\n";
	
	const int listSize = 5;
	Hitable* list[listSize];
	list[0] = new Sphere(Vector3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(Vector3(0.1f, 0.2f, 0.5f)));
	list[1] = new Sphere(Vector3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(Vector3(0.8f, 0.8f, 0.0f)));
	list[2] = new Sphere(Vector3(1.0f, 0.0f, -1.0f), 0.5f, new Metal(Vector3(0.8f, 0.6f, 0.2f), 0.0f));
	list[3] = new Sphere(Vector3(-1.0f, 0.0f, -1.0f), 0.5f, new Dielectric(1.5f));
	list[4] = new Sphere(Vector3(-1.0f, 0.0f, -1.0f), -0.45f, new Dielectric(1.5f));
	
	 world = random_scene();//new hitable_list(list, listSize);
	Vector3 lookFrom(0.0f, 0.0f, 0.0f);
	Vector3 lookAt(0.0f, 0.0f, -1.0f);
	float dist_to_focus = (lookFrom - lookAt).length();
	float aperture = 2.0f;
	cam.Initialize(lookFrom, lookAt, Vector3(0.0f, 1.0f, 0.0f), 90.0f, float(nx)/float(ny), aperture, dist_to_focus);

	std::cout << "Completed.\nCalculating..." << std::endl;
	for (j = ny - 1; j >= 0; j--)
	{
		for (i = 0; i < nx; i++)
		{
			Vector3 color(0.0f, 0.0f, 0.0f);

			//std::thread worker(Render);
			//std::future<Vector3> col = std::async(Render);
			//color = col.get();
			color = Render();
			color /= float(ns);
			color = Vector3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
			int ir = int(255.99 * color[0]);
			int ig = int(255.99 * color[1]);
			int ib = int(255.99 * color[2]);

			outStream << ir << " " << ig << " " << ib << "\n";
		}
	}

	std::cout << "Operation completed!" << std::endl;
	return 0;
}
