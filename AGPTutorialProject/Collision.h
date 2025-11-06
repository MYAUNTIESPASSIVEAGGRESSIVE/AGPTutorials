#pragma once
class Collision
{
public:

	// c1X - centre of first circle, X
	// c1Y - centre of first circle, Y
	// r1 - radius of first circle
	// c2X - centre of second circle, X
	// c2Y - centre of second circle, Y
	// r2 - centre of first circle, X
	static bool CircleCollision(float c1X, float c1Y, float r1, float c2X, float c2Y, float r2);
};

