#include "game_win.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

struct snake static player;

static unsigned char *gameboard;
static unsigned char board_size;

int check_collision(int *score)
{
    int x_dest, y_dest;
    char *dest;

    x_dest = player.x_cord[player.head];
    y_dest = player.y_cord[player.head];

    // Move the snake
    if (player.dir != NONE)
    {
        gameboard[x_dest + y_dest * board_size] = SNAKEBODY;

        switch (player.dir)
        {
        case LEFT:
        {
            x_dest--;
            break;
        }
        case RIGHT:
        {
            x_dest++;
            break;
        }
        case UP:
        {
            y_dest--;
            break;
        }
        case DOWN:
        {
            y_dest++;
            break;
        }
        }
    }

    // Check for the snake head going out of bounds
    if (x_dest < 0 || x_dest >= board_size || y_dest < 0 || y_dest >= board_size)
    {
        return 0;
    }

    // Use the gameboard to detect a collision and update the gameboard
    dest = &gameboard[x_dest + y_dest * board_size];

    if (*dest == SNAKEBODY)
    {
        return 0;
    }
    else if (*dest == ESA)
    {
        int i, newloc;
        char lx_tmp = 0, ly_tmp = 0, rx_tmp = 0, ry_tmp = 0;

        player.limit++;
        player.head = (player.head + 1) % player.limit;
        
        //Shift the remainder of the array forward by one to make room for the new (growing) tail
        for(i = player.head; i < player.limit; i++){
            if(i%2){
                lx_tmp = player.x_cord[i];
                ly_tmp = player.y_cord[i];

                player.x_cord[i] = rx_tmp;
                player.y_cord[i] = ry_tmp;
            }else{
                rx_tmp = player.x_cord[i];
                ry_tmp = player.y_cord[i];

                player.x_cord[i] = lx_tmp;
                player.y_cord[i] = ly_tmp;
            }
        }

        *score += 1;
        putchar('\a');

        while (gameboard[newloc = (rand() % (board_size * board_size))] != NOTHING)
            if(*score == board_size * board_size - 1) return 0;
        
        gameboard[newloc] = ESA;
    }
    else
    {
        player.head = (player.head + 1) % player.limit;
        gameboard[player.x_cord[player.head] + player.y_cord[player.head] * board_size] = NOTHING;
    }

    *dest = SNAKEHEAD;

    player.x_cord[player.head] = x_dest;
    player.y_cord[player.head] = y_dest;

    return 1;
}

void clear_board()
{
    free(gameboard);
    free(player.x_cord);
    free(player.y_cord);
}

void clear_screen(){
    system("cls");
}

void draw_screen(int score)
{
    int i, j;
    enum gameobjects item;

    // Clear the screen
    clear_screen();

    // Print the score
    printf("Score:%d | ", score);

    switch (player.dir)
    {
    case UP:
    {
        printf("UP");
        break;
    }
    case DOWN:
    {
        printf("DOWN");
        break;
    }
    case LEFT:
    {
        printf("LEFT");
        break;
    }
    case RIGHT:
    {
        printf("RIGHT");
        break;
    }
    default:
    {
        printf("NONE");
        break;
    }
    }

    putchar('\n');
    putchar('+');
    for (i = 0; i < board_size; i++)
    {
        putchar('-');
    }
    putchar('+');
    putchar('\n');

    for (i = 0; i < board_size; i++)
    {
        putchar('|');

        for (j = 0; j < board_size; j++)
        {
            item = gameboard[j + i * board_size];

            switch (item)
            {
            case NOTHING:
            {
                putchar(' ');
                break;
            }
            case SNAKEBODY:
            {
                putchar('#');
                break;
            }
            case SNAKEHEAD:
            {
                putchar('S');
                break;
            }
            case ESA:
            {
                putchar('X');
                break;
            }
            }
        }

        putchar('|');
        putchar('\n');
    }

    putchar('+');
    for (i = 0; i < board_size; i++)
    {
        putchar('-');
    }
    putchar('+');
    putchar('\n');
}

void player_input()
{
    char inkey;
    enum direction indir = NONE;

    // Check for WASD keys
    if (kbhit())
    {
        inkey = getch();
        switch (inkey)
        {
        //TODO Figure out why the arrow keys are so damn slow
        /*case 72:
        { // Up arrow key
            indir = UP;
            break;
        }
        case 75:
        { // Left arrow key
            indir = LEFT;
            break;
        }
        case 77:
        { // Right arrow key
            indir = RIGHT;
            break;
        }
        case 80:
        { // Down arrow key
            indir = DOWN;
            break;
        } */
        case 97:
        { // A key
            indir = LEFT;
            break;
        }
        case 100:
        { // D key
            indir = RIGHT;
            break;
        }
        case 115:
        { // S key
            indir = DOWN;
            break;
        }
        case 119:
        { // W key
            indir = UP;
            break;
        }
        }
    }

    // Check if direction is valid (180 degree turns are invalid)
    switch (indir)
    {
    case UP:
    {
        if (player.dir != DOWN)
        {
            player.dir = indir;
        }
        break;
    }
    case DOWN:
    {
        if (player.dir != UP)
        {
            player.dir = indir;
        }
        break;
    }
    case LEFT:
    {
        if (player.dir != RIGHT)
        {
            player.dir = indir;
        }
        break;
    }
    case RIGHT:
    {
        if (player.dir != LEFT)
        {
            player.dir = indir;
        }
        break;
    }
    }
}

void setup_board(unsigned char setsize)
{
    int i;
    board_size = setsize;

    // Allocate memory for the game board
    gameboard = malloc(board_size * board_size * sizeof(char));

    for (i = 0; i < board_size * board_size; i++)
    {
        gameboard[i] = NOTHING;
    }

    gameboard[(board_size / 3) + ((board_size / 2) * board_size)] = SNAKEHEAD;

    gameboard[(board_size / 2) + ((board_size / 2) * board_size)] = ESA;

    // Setup snake
    player.x_cord = malloc(board_size * board_size * sizeof(char));
    player.y_cord = malloc(board_size * board_size * sizeof(char));

    player.head = 0;
    player.limit = 1;

    player.x_cord[player.head] = (board_size / 3);
    player.y_cord[player.head] = (board_size / 2);

    player.dir = NONE;

    // Setup randomizer
    srand(time(0));
}

void sleep(int ms_duration)
{
    clock_t prevt = clock();
    long wait_dur = (CLOCKS_PER_SEC / 1000) * ms_duration;

    while (clock() < prevt + wait_dur)
        ;
}