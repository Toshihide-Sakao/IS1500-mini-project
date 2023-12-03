void draw_player(vec2 pos, double angle, uint8_t shot, int *score, uint32_t *map, uint8_t map2d[8][16]);
void move_player(vec2 *pos, double angle, uint32_t *map, uint8_t map2d[8][16]);
void rotate_player(double *angle, int whichway);