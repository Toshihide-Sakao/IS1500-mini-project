#include <math.h>
#include "vector.h"

vec2 vec_add(vec2 a, vec2 b)
{
    vec2 val = { a.x + b.x, a.y + b.y };
    return val;
}

vec2 vec_sub(vec2 a, vec2 b)
{
    vec2 val = { a.x - b.x, a.y - b.y};
    return val;
}

vec2 vec_mul(vec2 a, double m)
{
    vec2 val = { a.x * m, a.y * m};
    return val;
}

// double vec_mag(vec2 a){
//     return sqrt(a.x * a.x + a.y * a.y);
// }

// vec2 vec_norm(vec2 a) {
//     double mag = vec_mag(a);
//     vec2 val = { a.x / mag, a.y / mag };
//     return val;
// }

double vec_dot(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

// double vec_dist(vec2 a, vec2 b) {
//     return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
// }