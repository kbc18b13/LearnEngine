#pragma once
namespace LearnEngine {
constexpr float PI = 3.141592653f;

static inline constexpr float DegToRad(float deg) {
	return deg / 180.0f * PI;
}

static inline constexpr float RadToDeg(float rad) {
	return rad / PI * 180.0f;
}

}