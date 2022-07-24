#define PI (3.1415927f)

#include <math.h>

struct Vec2 {
	float x, y;

	Vec2(float x, float y) {
		this->x = x;
		this->y = y;
	}
};

struct Vec3 {
	float x, y, z;

	Vec3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};