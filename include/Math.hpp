#pragma once

#include <iostream>

struct Vector2f {
	Vector2f() :x(0.0f), y(0.0f) {};
	Vector2f(float p_x, float p_y) :x(p_x), y(p_y) {};

	void print() {
		std::cout << "Vector2f(x,y): " << x << ", " << y << std::endl;
	};

	float x,y;
};

struct Vector4f {
	Vector4f() :x(0.0f), y(0.0f), w(0.0f), h(0.0f) {};
	Vector4f(float p_x, float p_y, float p_w, float p_h) :x(p_x), y(p_y), w(p_w), h(p_h) {};

	void print() {
		std::cout << "Vector4f(x,y,w,h): " << x << ", " << y << ", " << w << ", " << h << std::endl;
	};

	float x,y,w,h;
};