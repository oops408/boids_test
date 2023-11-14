#ifndef _OBSTACLE_H
#define _OBSTACLE_H

#include "Vector3d.h"

class Obstacle
{
	public:
		Vector3d center;
		float radius;
		
		Obstacle();
		Obstacle(Vector3d center, float radius);
		~Obstacle();
};

#endif
