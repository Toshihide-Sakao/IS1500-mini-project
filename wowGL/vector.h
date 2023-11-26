typedef struct { double x; double y; } vec2;

vec2 vec_add(vec2 a, vec2 b);
vec2 vec_sub(vec2 a, vec2 b);
vec2 vec_mul(vec2 a, double b);
double vec_mag(vec2 a);
vec2 vec_norm(vec2 a);
double vec_dot(vec2 a, vec2 b);
double vec_dist(vec2 a, vec2 b);
