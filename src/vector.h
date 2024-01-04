#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdbool.h>

typedef struct {
	int x, y;
} Vec2I;

typedef struct {
	int x, y, z;
} Vec3I;

typedef struct {
	float x, y;
} Vec2F;

typedef struct {
	float x, y, z;
} Vec3F;

typedef struct {
	bool x, y;
} Vec2B;

Vec2F NormalizeVec2F(Vec2F vec);

#endif