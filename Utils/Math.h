#define PI (3.1415927f)
#include <math.h>

struct Vec4 {
	float w, x, y, z;

	Vec4(float w, float x, float y, float z) {
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

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