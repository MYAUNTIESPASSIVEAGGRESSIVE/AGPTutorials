#include "Collision.h"
#include <math.h>

bool Collision::CircleCollision(float c1X, float c1Y, float r1, float c2X, float c2Y, float r2)
{
	// calculate deltas
	float dx = c2X - c1X;
	float dy = c2Y - c1Y;

	// calculate distance
	float distance = sqrt((dx * dx) + (dy * dy));

	return distance <= r1 + r2;
}
