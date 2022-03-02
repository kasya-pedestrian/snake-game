#include "menus.h"
#include "game_win.h"

#include <stdio.h>
#include <limits.h>
#include <stdio.h>
#include <time.h>

#define LANGUAGE en

#ifdef LANGUAGE jp

char main_menu(){
    clear_screen();
    fflush(stdin);

    printf("######Snake!\n");
    printf("1. 遊ぶ！   \n");
    printf("2. 設定！ \n");
    printf("3. スコア! \n");
    printf("4. 終了 \n");
    printf("選択の数字を入力してください\n");

    return getchar();
}

void settings_menu(int* board_size, int* speed){
    clear_screen();
    int input;

    printf("######Settings!\n");
    printf("ゲーム盤のサイズを入力してください（ます単位）\n");

    scanf("%d", &input);

    if(input < 3){
        printf("小さすぎです\n");
    } else if(input > CHAR_MAX){
        printf("大きすぎです\n");
    } else {
        *board_size = input;
        printf("盤のサイズは%dますになります", input);
    }

    sleep(2000);

    clear_screen();

    printf("ヘビの速度を入力してください（１秒ごとに何ます）\n");

    scanf("%d", &input);

    if(input > 100){
        printf("速すぎです\n");
    }else if(input <= 0){
        printf("遅すぎです\n");
    }else{
        *speed = 1000/input;
        printf("速度は%dになります", input);
    }

    sleep(2000);
    fflush(stdin);
}

void scoreboard_menu(){
    FILE *fp;
    int i, score, records = 0;
    char initials[3];
    time_t timestamp;
    struct tm *date;

    clear_screen();
    printf("######Score!\n");

    if((fp = fopen("scoreboard.txt", "r")) != 0){
        //Determine how many records exist in the scoreboard
        for(i = 0; i < 10; i++){
        	if(fscanf(fp, "%d. %3c %d %ld", &records, initials, &score, &timestamp) == EOF){
        		break;
        	}

        	date = localtime(&timestamp);

        	if(date->tm_year / 100) date->tm_year += 1900;

        	printf("%d. %3s %5d %d/%d/%d\n", records, initials, score, date->tm_year, date->tm_mon, date->tm_mday);
        }

        fclose(fp);
    }

    if(!records) printf("スコア記録が空いています");

    sleep(5000);
}

void record_input_menu(int score){
    clear_screen();
    char initials[4];

    printf("スコア：%d\n", score);
    fflush(stdin);
    printf("頭文字を入力してください\n");
    scanf("%3c", initials);

    scoreboard_add_record(initials, score);
    scoreboard_menu();
}

#endif

#ifdef LANGUAGE en

char main_menu(){
    clear_screen();
    fflush(stdin);

    printf("######Snake!\n");
    printf("1. Snake!   \n");
    printf("2. Settings! \n");
    printf("3. Score! \n");
    printf("4. Stop... \n");
    printf("Your choice:\n");

    return getchar();
}

void settings_menu(int* board_size, int* speed){
    clear_screen();
    int input;

    printf("######Settings!\n");
    printf("Set gameboard size!\n");
    printf("Your choice:");

    scanf("%d", &input);

    if(input < 3){
        printf("That's too small.\n");
    } else if(input > CHAR_MAX){
        printf("That's too big!\n");
    } else {
        *board_size = input;
        printf("Board size set to %d", input);
    }

    sleep(2000);

    clear_screen();

    printf("Set snake speed!\n");
    printf("Your choice in blocks per second:");

    scanf("%d", &input);

    if(input > 100){
        printf("That's too fast!\n");
    }else if(input <= 0){
        printf("That's too slow...\n");
    }else{
        *speed = 1000/input;
        printf("Speed set to %d", input);
    }

    sleep(2000);
    fflush(stdin);
}

void scoreboard_menu(){
    FILE *fp;
    int i, score, records = 0;
    char initials[3];
    time_t timestamp;
    struct tm *date;

    clear_screen();
    printf("######Score!\n");

    if((fp = fopen("scoreboard.txt", "r")) != 0){
        //Determine how many records exist in the scoreboard
        for(i = 0; i < 10; i++){
        	if(fscanf(fp, "%d. %3c %d %ld", &records, initials, &score, &timestamp) == EOF){
        		break;
        	}

        	date = localtime(&timestamp);

        	if(date->tm_year / 100) date->tm_year += 1900;

        	printf("%d. %3s %5d %d/%d/%d\n", records, initials, score, date->tm_year, date->tm_mon, date->tm_mday);
        }

        fclose(fp);
    }

    if(!records) printf("No high scores...");

    sleep(5000);
}

void record_input_menu(int score){
    clear_screen();
    char initials[4];

    printf("Your score: %d\n", score);
    fflush(stdin);
    printf("Input initials: ");
    scanf("%3c", initials);

    scoreboard_add_record(initials, score);
    scoreboard_menu();
}

#endif

void scoreboard_add_record(char* initials_in, int score_in){
    FILE *fp;
    int i, j, rec_count = 0;

    char initials[9][4] = {"   \0", "   \0", "   \0", "   \0", "   \0", \
    						"   \0", "   \0", "   \0", "   \0",};
    int score[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
    time_t timestamp[9];

    if((fp = fopen("scoreboard.txt", "r")) != 0){
        //Determine how many records exist in the scoreboard
        for(i = 0; i < 9; i++){
        	if(fscanf(fp, "%3c %d %ld\n", initials[i], &score[i], &timestamp[i]) != 4){
        		break;
        	}
		rec_count++;
        }

        fclose(fp);
    }

    //Find insertion index of new score
    for(i = 0; i < 9; i++){
    	if(score_in > score[i]) break;
    	else if(score_in == score[i]){
    		i ++;
		break;
    	}
    }

    j = i + 1;
    rec_count++;

    if((fp = fopen("scoreboard.txt", "w")) != 0){
        //Write records
        for(j = 0; j < rec_count; j++){
        	if(j < i){
        		fprintf(fp, "%3s %d %ld\n", initials[j], score[j], timestamp[j]);
        	} else if(j > i) {
        		fprintf(fp, "%3s %d %ld\n", initials[j-1], score[j-1], timestamp[j-1]);
        	} else {
        		time_t temp;
        		time(&temp);
        		fprintf(fp, "%3s %d %ld\n", initials_in, score_in, temp);
        	}
        }

        fclose(fp);
    }
}