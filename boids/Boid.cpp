#include "Boid.h"

Boid::Boid()
{
}

Boid::Boid(char c, float posX, float posY, float posZ, float velX, float velY, float velZ)
{
	this->c = c;
	this->pos.x = posX;
	this->pos.y = posY;
	this->pos.z = posZ;
	this->vel.x = velX;
	this->vel.y = velY;
	this->vel.z = velZ;
}

Boid::~Boid()
{
}
