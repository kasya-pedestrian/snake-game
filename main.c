#include "game_win.h"
#include "menus.h"

#include <stdlib.h>

int main()
{
    char input;
    int gameboard_size = 20;
    int gamespeed = 200;

    for (;;)
    {
        input = main_menu();

        switch (input)
        {
        case '1':
        {
            // Game loop
            setup_board(gameboard_size);
            int score = 0;

            do
            {
                draw_screen(score);
                sleep(gamespeed);
                player_input();
            } while (check_collision(&score));

            clear_board();

            if (score != gameboard_size * gameboard_size - 1)
            {
                printf("You Lose!");
            }
            else
            {
                draw_screen(score);
                printf("Perfect Run!");
            }

            sleep(3000);
            record_input_menu(score);
            break;
        }
        case '2':
        {
            settings_menu(&gameboard_size, &gamespeed);
            break;
        }
        case '3':
        {
        	scoreboard_menu();
            break;
        }

        case '4':
        {
            exit(EXIT_SUCCESS);
            break;
        }
        default:{
            while(getchar() != '\n');
        }
    }
    }

    return 0;
}
