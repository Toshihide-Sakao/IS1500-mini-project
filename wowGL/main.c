#include <stdlib.h>
// #include <GLUT/glut.h>
#include <GL/glut.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
#include "vector.h"
#include "player.h"

#define PI 3.14159265535
#define P2 PI / 2
#define P3 3 * PI / 2

//-----------------------------MAP----------------------------------------------
#define mapX 16   // map width
#define mapY 8    // map height
#define squareS 8 // map cube size

#define pixS 10 // map cube size

uint8_t map2d[8][16] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

vec2 player_pos = {50, 9};
vec2 enemy_pos = {51, 10};
double player_angle = PI * (0.0 / 4.0);
double enemy_angle = PI * (0.0 / 4.0);

void set_pos(int x, int y)
{
    int xo = x * squareS;
    int yo = y * squareS;
    int offset = pixS;

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glVertex2i(xo + 2, yo + 2);
    glVertex2i(xo + 2, offset + yo - 2);
    glVertex2i(offset + xo - 2, offset + yo - 2);
    glVertex2i(offset + xo - 2, yo + 2);
    glEnd();
}

void draw_rects(int startX, int startY, int endX, int endY)
{
    for (int i = startX; i < endX; i++)
    {
        for (int j = startY; j < endY; j++)
        {
            set_pos(i, j);
        }
    }
}

void clr_pos(int x, int y)
{
    int xo = x * squareS;
    int yo = y * squareS;
    int offset = pixS;

    glColor3f(0.3, 0.3, 0.3);

    glBegin(GL_QUADS);
    glVertex2i(xo + 2, yo + 2);
    glVertex2i(xo + 2, offset + yo - 2);
    glVertex2i(offset + xo - 2, offset + yo - 2);
    glVertex2i(offset + xo - 2, yo + 2);
    glEnd();
}

void drawMap2D()
{
    int x, y;
    for (y = 0; y < mapY * 2; y++)
    {
        for (x = 0; x < mapX * 2; x++)
        {
            if (map2d[y / 2][x / 2] == 1)
            {
                set_pos(x + 96, y + 1);
            }
        }
    }
} //-----------------------------------------------------------------------------

spawn_enemies(uint8_t map2d[8][16])
{
    srand(time(0));
    int randomNumberx = (rand() & 16);
    int randomNumbery = (rand() & 8);
    if (map2d[randomNumbery][randomNumberx] == 0)
    {
        map2d[randomNumbery][randomNumberx] = 2;
    }
    else
    {
        while (map2d[randomNumbery][randomNumberx] == 1)
        {
            int randomNumberx = (rand() & 16);
            int randomNumbery = (rand() & 8);
        }
        map2d[randomNumbery][randomNumberx] = 2;
    }
}

void Buttons(unsigned char key, int x, int y)
{
    if (key == 'a')
    {
        // pa += 5;
        // pa = FixAng(pa);
        // pdx = cos(degToRad(pa));
        // pdy = -sin(degToRad(pa));
    }
    if (key == 'd')
    {
        // pa -= 5;
        // pa = FixAng(pa);
        // pdx = cos(degToRad(pa));
        // pdy = -sin(degToRad(pa));
    }
    if (key == 'w')
    {
        move_player(&player_pos, player_angle);
    }
    if (key == 's')
    {
        // px -= pdx * 5;
        // py -= pdy * 5;
    }
    if (key == 'q')
    {
        rotate_player(&player_angle);
    }

    glutPostRedisplay();
} //-----------------------------------------------------------------------------

void init()
{
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, 1024, 510, 0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2D();
    draw_player(player_pos, player_angle, map2d);
    draw_enemy(enemy_pos, enemy_angle, map2d);
    // drawRays2D();
    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 500);
    glutCreateWindow("testing window");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(Buttons);
    glutMainLoop();
}
