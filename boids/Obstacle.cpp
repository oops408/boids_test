#include "Obstacle.h"

Obstacle::Obstacle()
{
}

Obstacle::Obstacle(Vector3d center, float radius)
{
	this->center = center;
	this->radius = radius;
}

Obstacle::~Obstacle()
{
}
