#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdint.h>

#include "vector.h"
#include "player.h"

#define PI 3.14159265535

#define mapX 16  // map width
#define mapY 8 // map height
#define squareS 8  // map cube size

#define pixS 10 // map cube size

uint8_t map2d[8][16] =
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1},
		{1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

vec2 player_pos = {50, 9};
double player_angle = PI * (0.0 / 4.0);

void set_pos(int x, int y)
{
    int x_offset = x * squareS;
    int y_offset = y * squareS;
    int offset = pixS;

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glVertex2i(x_offset + 2, y_offset + 2);
    glVertex2i(x_offset + 2, offset + y_offset - 2);
    glVertex2i(offset + x_offset - 2, offset + y_offset - 2);
    glVertex2i(offset + x_offset - 2, y_offset + 2);
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
    int x_offset = x * squareS;
    int y_offset = y * squareS;
    int offset = pixS;

    glColor3f(0.3, 0.3, 0.3);

    glBegin(GL_QUADS);
    glVertex2i(x_offset + 2, y_offset + 2);
    glVertex2i(x_offset + 2, offset + y_offset - 2);
    glVertex2i(offset + x_offset - 2, offset + y_offset - 2);
    glVertex2i(offset + x_offset - 2, y_offset + 2);
    glEnd();
}

void drawMap2D()
{
    int x, y;
    for (y = 0; y < mapY * 2; y++)
    {
        for (x = 0; x < mapX *2; x++)
        {
            if (map2d[y / 2][x / 2] == 1)
            {
                set_pos(x + 96, y + 1);
            }
        }
    }
}

void Buttons(unsigned char key, int x, int y)
{
    if (key == 'a')
    {
    }
    if (key == 'd')
    {
    }
    if (key == 'w')
    {
        move_player(&player_pos, player_angle);
    }
    if (key == 's')
    {
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
