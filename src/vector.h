#ifndef GLOBAL_H
#define GLOBAL_H

typedef struct {
	int x, y;
} Vec2I;

typedef struct {
	int x, y, z;
} Vec3I;

typedef struct {
	float x, y;
} Vec2F;

Vec2F NormalizeVec2F(Vec2F vec);

#endif