#include "vector.h"
#include "math.h"

Vec2F NormalizeVec2F(Vec2F vec) {
	float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y);
	Vec2F output = {vec.x / magnitude, vec.y / magnitude};
	return output;
}