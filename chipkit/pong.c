#include <stdlib.h>
#include <math.h>
#include <stdint.h>

int bwidth = 2;
float ballx = 63;
float bally = 15;
float dirx = 2;
float diry = 1;

int last_p1x = 4;
int last_p1y = 15;
int p1x = 4;
int p1y = 15;

int last_p2x = 123;
int last_p2y = 15;
int p2x = 123;
int p2y = 15;
int pwidth = 2;
int pheight = 10;

void pong_init()
{
    ballx = 63;
    bally = 15;

    p1x = 4;
    p1y = 15;

    p2x = 123;
    p2y = 15;
}

void update_ball()
{
    clr_pos((int)ballx, (int)bally);
    clr_pos((int)ballx + 1, (int)bally);
    clr_pos((int)ballx, (int)bally + 1);
    clr_pos((int)ballx + 1, (int)bally + 1);

    if (bally + diry < 1 || bally + diry > 29)
    {
        diry *= -1;
    }
    else if ((ballx + dirx <= 4 && bally > p1y - 1 - (pheight / 2) && bally < p1y + (pheight / 2)) || (ballx + dirx >= 122 && bally > p2y - 1 - (pheight / 2) && bally < p2y + (pheight / 2)))
    {
        dirx *= -1;
    }

    ballx += dirx;
    bally += diry;

    set_pos((int)ballx, (int)bally);
    set_pos((int)ballx + 1, (int)bally);
    set_pos((int)ballx, (int)bally + 1);
    set_pos((int)ballx + 1, (int)bally + 1);
}

draw_player_pong()
{
    int i, j;
    for ( i = 0; i < pwidth; i++)
    {
        for ( j = 0; j < pheight; j++)
        {
            clr_pos((int)last_p1x - 1 + i, (int)last_p1y - (pheight/2) + j);
            clr_pos((int)last_p2x + i, (int)last_p2y - (pheight / 2) + j);
        }
    }

    for ( i = 0; i < pwidth; i++)
    {
        for ( j = 0; j < pheight; j++)
        {
            set_pos((int)p1x - 1 + i, (int)p1y - (pheight/2) + j);
            set_pos((int)p2x + i, (int)p2y - (pheight / 2) + j);
        }
    }
}

void move_p1_up() {
    if (p1y - (pheight / 2) > 1)
    {
        last_p1y = p1y;
        p1y -= 1;
    }
}

void move_p1_down() {
    if (p1y + (pheight / 2) < 30)
    {
        last_p1y = p1y;
        p1y += 1;
    }
}

void move_p2_up() {
    if (p2y - (pheight / 2) > 1)
    {
        last_p2y = p2y;
        p2y -= 1;
    }
}

void move_p2_down() {
    if (p2y + (pheight / 2) < 30)
    {
        last_p2y = p2y;
        p2y += 1;
    }
}

draw_board_pong()
{
    int i, j;
    for ( i = 4; i < 124; i++)
    {
        set_pos(i, 0);
        set_pos(i, 31);
    }
}



void pong_game()
{
    draw_board_pong();
    draw_player_pong();
    update_ball();

    delay(10000);
}