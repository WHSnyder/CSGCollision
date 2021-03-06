#include <stdlib.h>
#include <iostream>
#include <ctype.h>
#define NDEBUG
#include "PxPhysicsAPI.h"


#include "glm/glm.hpp"
#include "glm/ext.hpp"

using namespace physx;


class Simu {

private:

	PxDefaultAllocator gAllocator;
	PxDefaultErrorCallback gErrorCallback;

	PxFoundation *gFoundation = nullptr;
	PxPhysics *gPhysics	= nullptr;
	PxCooking *cook = nullptr;

	PxDefaultCpuDispatcher *gDispatcher = nullptr;
	PxScene *gScene	= nullptr;

	PxMaterial *gMaterial = nullptr;

	PxRigidDynamic *body1, *body2;

public:

	void getModelMats();

	void initSimu();
	void stepSimu(float timestep);
	void cleanupSimu();

	void addCube(glm::vec3 center, float extent, int tag, void *objectptr=nullptr);
	void addSphere(glm::vec3 center, float extent, int tag, void *objectptr=nullptr);
	void addTerrain(std::int32_t *data, int dim, glm::vec3 scale);

	Simu(){
		initSimu();
	}

	~Simu(){
		cleanupSimu();
	}
};