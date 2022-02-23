#pragma once

#include <iostream>

#ifndef WH
#define WH false
#endif

#ifndef WW
#define WW true
#endif

struct Vector2f {
    Vector2f() : x(0.0f), y(0.0f) {};

    Vector2f(float p_x, float p_y) : x(p_x), y(p_y) {};

    void print() {
        std::cout << "Vector2f(x,y): " << x << ", " << y << std::endl;
    };

    float x, y;
};

struct Vector4f {
    Vector4f() : x(0.0f), y(0.0f), w(0.0f), h(0.0f) {};

    Vector4f(float p_x, float p_y, float p_w, float p_h) : x(p_x), y(p_y), w(p_w), h(p_h) {};

    void print() {
        std::cout << "Vector4f(x,y,w,h): " << x << ", " << y << ", " << w << ", " << h << std::endl;
    };

    float x, y, w, h;
};

struct Size {
    Size() : val(0.0f), type(WW) {};

    Size(float p_val, bool p_type) : val(p_val), type(p_type) {};

    void print() {
        std::cout << "Size(val, type): " << val << ", " << (type == true ? "WW" : "WH") << std::endl;
    };

    float val; // Percentage
    bool type; // Width, Height
};

struct MultiSize {
    MultiSize() : x(Size()), y(Size()), w(Size()), h(Size()) {};

    MultiSize(Size p_x, Size p_y, Size p_w, Size p_h) : x(p_x), y(p_y), w(p_w), h(p_h) {};

    void setX(float p_val) {
        x.val = p_val;
    }

    void setY(float p_val) {
        y.val = p_val;
    }

    void setW(float p_val) {
        w.val = p_val;
    }

    void setH(float p_val) {
        h.val = p_val;
    }

    Size x, y, w, h;
};