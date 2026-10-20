#pragma once

#include <random>


namespace sg::Core {

struct vec2 {
    int x;
    int y;
    vec2();
    vec2(int _x, int _y);
};

struct uvec2 {
    unsigned int x;
    unsigned int y;
    uvec2();
    uvec2(unsigned int _x, unsigned int _y);
};

struct vec2f {
    float x;
    float y;
    vec2f();
    vec2f(float _x, float _y);
};

}

// min and max are included
int random_int(int min, int max);

// min is included, max is not
float random_float(float min, float max);