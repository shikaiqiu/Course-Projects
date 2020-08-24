#ifndef CGL_STATICSCENE_LIGHT_H
#define CGL_STATICSCENE_LIGHT_H

#include "CGL/vector3D.h"
#include "CGL/matrix3x3.h"
#include "CGL/spectrum.h"
#include "pathtracer/sampler.h" // UniformHemisphereSampler3D, UniformGridSampler2D
#include "util/image.h"   // HDRImageBuffer

#include "scene.h"  // SceneLight
#include "object.h" // Mesh, SphereObject

namespace CGL { namespace SceneObjects {

// Directional Light //
class DirectionalLight : public SceneLight {
 public:
  DirectionalLight(const Spectrum& rad, const Vector3D& lightDir);
  Spectrum sample_L(const Vector3D& p, Vector3D* wi, float* distToLight,
                    float* pdf) const;
  bool is_delta_light() const { return true; }

 private:
  Spectrum radiance;
  Vector3D dirToLight;

}; // class Directional Light

// Infinite Hemisphere Light //

class InfiniteHemisphereLight : public SceneLight {
 public:
  InfiniteHemisphereLight(const Spectrum& rad);
  Spectrum sample_L(const Vector3D& p, Vector3D* wi, float* distToLight,
                    float* pdf) const;
  bool is_delta_light() const { return false; }

 private:
  Spectrum radiance;
  Matrix3x3 sampleToWorld;
  UniformHemisphereSampler3D sampler;

}; // class InfiniteHemisphereLight


// Point Light //

class PointLight : public SceneLight {
 public:
  PointLight(const Spectrum& rad, const Vector3D& pos);
  Spectrum sample_L(const Vector3D& p, Vector3D* wi, float* distToLight,
                    float* pdf) const;
  bool is_delta_light() const { return true; }

 private:
  Spectrum radiance;
  Vector3D position;

}; // class PointLight

// Spot Light //

class SpotLight : public SceneLight {
 public:
  SpotLight(const Spectrum& rad, const Vector3D& pos,
            const Vector3D& dir, float angle);
  Spectrum sample_L(const Vector3D& p, Vector3D* wi, float* distToLight,
                    float* pdf) const;
  bool is_delta_light() const { return true; }

 private:
  Spectrum radiance;
  Vector3D position;
  Vector3D direction;
  float angle;

}; // class SpotLight

// Area Light //

class AreaLight : public SceneLight {
 public:
  AreaLight(const Spectrum& rad,
            const Vector3D& pos,   const Vector3D& dir,
            const Vector3D& dim_x, const Vector3D& dim_y);
  Spectrum sample_L(const Vector3D& p, Vector3D* wi, float* distToLight,
                    float* pdf) const;

	double sample_L(float wavelength, const Vector3D &p, Vector3D *wi, float *distToLight, float *pdf);

  bool is_delta_light() const { return false; }

 private:
  Spectrum radiance;
  Vector3D position;
  Vector3D direction;
  Vector3D dim_x;
  Vector3D dim_y;
  UniformGridSampler2D sampler;
  float area;


}; // class AreaLight

// Sphere Light //

class SphereLight : public SceneLight {
 public:
  SphereLight(const Spectrum& rad, const SphereObject* sphere);
  Spectrum sample_L(const Vector3D& p, Vector3D* wi, float* distToLight,
                    float* pdf) const;
  bool is_delta_light() const { return false; }

 private:
  const SphereObject* sphere;
  Spectrum radiance;
  UniformHemisphereSampler3D sampler;

}; // class SphereLight

// Mesh Light

class MeshLight : public SceneLight {
 public:
  MeshLight(const Spectrum& rad, const Mesh* mesh);
  Spectrum sample_L(const Vector3D& p, Vector3D* wi, float* distToLight,
                    float* pdf) const;
  bool is_delta_light() const { return false; }

 private:
  const Mesh* mesh;
  Spectrum radiance;

}; // class MeshLight

} // namespace SceneObjects
} // namespace CGL

#endif  // CGL_STATICSCENE_BSDF_H
