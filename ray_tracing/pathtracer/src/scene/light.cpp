#include "light.h"

#include <iostream>

#include "pathtracer/sampler.h"

namespace CGL { namespace SceneObjects {

// Directional Light //
static float gaussian(float x, float mu, float sigma) {
					return 1. / (sigma * sqrt(2 * PI)) * exp(-0.5 * pow((x - mu) / sigma, 2));
		}

		constexpr static float mu_red = 600;
    constexpr static float mu_green = 550;
    constexpr static float mu_blue = 450;
    constexpr static float sigma_red = 25;
    constexpr static float sigma_green = 25;
    constexpr static float sigma_blue = 15;

DirectionalLight::DirectionalLight(const Spectrum& rad,
                                   const Vector3D& lightDir)
    : radiance(rad) {
  dirToLight = -lightDir.unit();
}

Spectrum DirectionalLight::sample_L(const Vector3D& p, Vector3D* wi,
                                    float* distToLight, float* pdf) const {
  *wi = dirToLight;
  *distToLight = INF_D;
  *pdf = 1.0;
  return radiance;
}

// Infinite Hemisphere Light //

InfiniteHemisphereLight::InfiniteHemisphereLight(const Spectrum& rad)
    : radiance(rad) {
  sampleToWorld[0] = Vector3D(1,  0,  0);
  sampleToWorld[1] = Vector3D(0,  0, -1);
  sampleToWorld[2] = Vector3D(0,  1,  0);
}

Spectrum InfiniteHemisphereLight::sample_L(const Vector3D& p, Vector3D* wi,
                                           float* distToLight,
                                           float* pdf) const {
  Vector3D dir = sampler.get_sample();
  *wi = sampleToWorld* dir;
  *distToLight = INF_D;
  *pdf = 1.0 / (2.0 * PI);
  return radiance;
}

// Point Light //

PointLight::PointLight(const Spectrum& rad, const Vector3D& pos) : 
  radiance(rad), position(pos) { }

Spectrum PointLight::sample_L(const Vector3D& p, Vector3D* wi,
                             float* distToLight,
                             float* pdf) const {
  Vector3D d = position - p;
  *wi = d.unit();
  *distToLight = d.norm();
  *pdf = 1.0;
  return radiance;
}


// Spot Light //

SpotLight::SpotLight(const Spectrum& rad, const Vector3D& pos,
                     const Vector3D& dir, float angle) {

}

Spectrum SpotLight::sample_L(const Vector3D& p, Vector3D* wi,
                             float* distToLight, float* pdf) const {
  return Spectrum();
}


// Area Light //

AreaLight::AreaLight(const Spectrum& rad, 
                     const Vector3D& pos,   const Vector3D& dir, 
                     const Vector3D& dim_x, const Vector3D& dim_y)
  : radiance(rad), position(pos), direction(dir),
    dim_x(dim_x), dim_y(dim_y), area(dim_x.norm() * dim_y.norm()) { }

Spectrum AreaLight::sample_L(const Vector3D& p, Vector3D* wi, 
                             float* distToLight, float* pdf) const {

  Vector2D sample = sampler.get_sample() - Vector2D(0.5f, 0.5f);
  Vector3D d = position + sample.x * dim_x + sample.y * dim_y - p;
  float cosTheta = dot(d, direction);
  float sqDist = d.norm2();
  float dist = sqrt(sqDist);
  *wi = d / dist;
  *distToLight = dist;
  *pdf = sqDist / (area * fabs(cosTheta));
  return cosTheta < 0 ? radiance : Spectrum();
}

double AreaLight::sample_L(float wavelength, const Vector3D& p, Vector3D* wi,
                             float* distToLight, float* pdf) {

  Vector2D sample = sampler.get_sample() - Vector2D(0.5f, 0.5f);
  Vector3D d = position + sample.x * dim_x + sample.y * dim_y - p;
  float cosTheta = dot(d, direction);
  float sqDist = d.norm2();
  float dist = sqrt(sqDist);
  *wi = d / dist;
  *distToLight = dist;
  *pdf = sqDist / (area * fabs(cosTheta));
  if (cosTheta >= 0)
  	return 0;
//  cout << "sampling in area with pdf: " << *pdf << endl;
//  double r, g, b;
//  r = radiance.r != 0 ? radiance.r * gaussian(wavelength, mu_red, sigma_red / radiance.r) : 0;
//  g = radiance.g != 0 ? radiance.g * gaussian(wavelength, mu_green, sigma_green / radiance.g) : 0;
//  b = radiance.b != 0 ? radiance.b * gaussian(wavelength, mu_blue, sigma_blue / radiance.b) : 0;
//	return r + g + b;
	return radiance.norm();
}


// Sphere Light //

SphereLight::SphereLight(const Spectrum& rad, const SphereObject* sphere) {

}

Spectrum SphereLight::sample_L(const Vector3D& p, Vector3D* wi, 
                               float* distToLight, float* pdf) const {

  return Spectrum();
}

// Mesh Light

MeshLight::MeshLight(const Spectrum& rad, const Mesh* mesh) {

}

Spectrum MeshLight::sample_L(const Vector3D& p, Vector3D* wi, 
                             float* distToLight, float* pdf) const {
  return Spectrum();
}

} // namespace SceneObjects
} // namespace CGL
