//
// Created by Ted on 11/26/2021.
//
#include "Utilities/Math/Math.hpp"

#ifndef AURORA_GRAPHICS_HPP
#define AURORA_GRAPHICS_HPP


namespace Graphics {
    // Enums
    enum corner {
        topLeft, topRight, bottomLeft, bottomRight, all
    };

    // Shapes
    struct RoundedRectangle {
        MultiSize rect;
        Size borderRadius{};
    };

    // Utilities:
    void setColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    // Drawing:

    void drawEllipse(SDL_Renderer *r, int x0, int y0, int radiusX, int radiusY, corner type = all);

    void drawLine(SDL_Renderer *r, int x1, int y1, int x2, int y2);
};


#endif //AURORA_GRAPHICS_HPP