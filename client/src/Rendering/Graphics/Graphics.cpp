//
// Created by Ted on 11/26/2021.
//

#include <SDL2/SDL.h>
#include "Graphics.hpp"

#include "Utilities/Math/Math.hpp"

// TY Kind stranger! (https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-sdl)
//
// draw one quadrant arc, and mirror the other 4 quadrants
void Graphics::drawEllipse(SDL_Renderer *r, int x0, int y0, int radiusX, int radiusY,
                           Graphics::corner type) {
    float pi = 3.14159265358979323846264338327950288419716939937510;
    float pih = pi / 2.0; //half of pi

    //drew  28 lines with   4x4  circle with precision of 150 0ms
    //drew 132 lines with  25x14 circle with precision of 150 0ms
    //drew 152 lines with 100x50 circle with precision of 150 3ms
    const int prec = 42; // precision value; value of 1 will draw a diamond, 27 makes pretty smooth circles.
    float theta = 0;     // angle that will be increased each loop

    //starting point
    int x = (float) radiusX * cos(theta);//start point
    int y = (float) radiusY * sin(theta);//start point
    int x1 = x;
    int y1 = y;

    //repeat until theta >= 90;
    float step = pih / (float) prec; // amount to add to theta each time (degrees)
    for (theta = step; theta <= pih; theta += step)//step through only a 90 arc (1 quadrant)
    {
        //get new point location
        x1 = (float) radiusX * cosf(theta) + 0.5; //new point (+.5 is a quick rounding method)
        y1 = (float) radiusY * sinf(theta) + 0.5; //new point (+.5 is a quick rounding method)

        //draw line from previous point to new point, ONLY if point incremented
        if ((x != x1) || (y != y1))//only draw if coordinate changed
        {
            switch (type) {
                case Graphics::all:
                    SDL_RenderDrawLine(r, x0 + x, y0 - y, x0 + x1, y0 - y1);//quadrant TR
                    SDL_RenderDrawLine(r, x0 - x, y0 - y, x0 - x1, y0 - y1);//quadrant TL
                    SDL_RenderDrawLine(r, x0 - x, y0 + y, x0 - x1, y0 + y1);//quadrant BL
                    SDL_RenderDrawLine(r, x0 + x, y0 + y, x0 + x1, y0 + y1);//quadrant BR
                    break;
                case Graphics::topLeft:
                    SDL_RenderDrawLine(r, x0 - x, y0 - y, x0 - x1, y0 - y1);//quadrant TL
                    break;
                case Graphics::topRight:
                    SDL_RenderDrawLine(r, x0 + x, y0 - y, x0 + x1, y0 - y1);//quadrant TR
                    break;
                case Graphics::bottomLeft:
                    SDL_RenderDrawLine(r, x0 - x, y0 + y, x0 - x1, y0 + y1);//quadrant BL
                    break;
                case Graphics::bottomRight:
                    SDL_RenderDrawLine(r, x0 + x, y0 + y, x0 + x1, y0 + y1);//quadrant BR
                    break;

            }

        }
        //save previous points
        x = x1;//save new previous point
        y = y1;//save new previous point
    }
    //arc did not finish because of rounding, so finish the arc
    if (x != 0) {
        x = 0;
        SDL_RenderDrawLine(r, x0 + x, y0 - y, x0 + x1, y0 - y1);//quadrant TR
        SDL_RenderDrawLine(r, x0 - x, y0 - y, x0 - x1, y0 - y1);//quadrant TL
        SDL_RenderDrawLine(r, x0 - x, y0 + y, x0 - x1, y0 + y1);//quadrant BL
        SDL_RenderDrawLine(r, x0 + x, y0 + y, x0 + x1, y0 + y1);//quadrant BR
    }
}

void Graphics::drawLine(SDL_Renderer *r, int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(r, x1, y1, x2, y2);
}

void Graphics::setColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
