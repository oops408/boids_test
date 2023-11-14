#ifndef _BOID_H
#define _BOID_H

#include "Vector3d.h"

//Boid Class

class Boid
{
public:
	char c;		//character represented by the boid

	Vector3d pos;
	Vector3d vel;

	Boid();
	Boid(char c, float posX, float posY, float posZ, float velX, float velY, float velZ);
	~Boid();
};

#endif
