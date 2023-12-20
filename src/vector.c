#include "vector.h"
#include "math.h"

Vec2F NormalizeVec2F(Vec2F vec) {
	float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y);
	Vec2F output;
	if (magnitude != 0) {
		output.x = vec.x / magnitude; 
		output.y = vec.y / magnitude;
	}
	else
		output.x = output.y = 0;
	return output;
}