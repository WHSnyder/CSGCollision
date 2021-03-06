#ifndef glmi
#define glmi
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#endif 

#include <math.h>
#include "Obj.h"

#ifndef sphere
#define sphere
using namespace glm;


class Sphere : public Obj {

	public:
		float radius;
		vec3 color;
		Sphere(vec3 center, vec3 color, float r);
		virtual RayHit *intersect_ray(Ray& r);
		virtual cv::Vec3b shade(RayHit *rh, cv::Mat *image, Scene *scene);
};
#endif