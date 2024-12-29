#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
    int bomb;
    int flag;
    int opened;
} fieldStruct;

// ASCI art from:
// https://patorjk.com/software/taag/#p=display&h=1&v=1&f=Modular&t=%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20MINE%0ASWEEPER%0A%0A
void logoPrint() {
    char *str =
        "\n\
                 __   __  ___  __    _  _______                 \n\
                |  |_|  ||   ||  |  | ||       |                \n\
                |       ||   ||   |_| ||    ___|                \n\
                |       ||   ||       ||   |___                 \n\
                |       ||   ||  _    ||    ___|                \n\
                | ||_|| ||   || | |   ||   |___                 \n\
                |_|   |_||___||_|  |__||_______|                \n\
 _______  _     _  _______  _______  _______  _______  ______   \n\
|       || | _ | ||       ||       ||       ||       ||    _ |  \n\
|  _____|| || || ||    ___||    ___||    _  ||    ___||   | ||  \n\
| |_____ |       ||   |___ |   |___ |   |_| ||   |___ |   |_||_ \n\
|_____  ||       ||    ___||    ___||    ___||    ___||    __  |\n\
 _____| ||   _   ||   |___ |   |___ |   |    |   |___ |   |  | |\n\
|_______||__| |__||_______||_______||___|    |_______||___|  |_|\n\
\n \
                           Press any key to continue.";

    // mvwaddstr(stdscr, (LINES / 2 - 16 / 2), (COLS / 2 - 65 / 2), str);
    printw("%s", str);
    wrefresh(stdscr);
    getch();
}

int pointCheck(int fieldy, int fieldx, fieldStruct *f, int gameH, int gameW) {
    int y;
    int x;
    int bCount = 0;
    for (int i = 0; i < 9; i++) {
        if (i != 4) { /*omit checked point*/
            y = i / 3 - 1;
            x = i % 3 - 1;
            if (fieldy + y < gameH && fieldy + y >= 0 && fieldx + x >= 0 && fieldx + x < gameW)
                if (f[((fieldy + y) * gameW) + (fieldx + x)].bomb == 1)
                    bCount++;
        }
    }
    return bCount;
}

int openField(int fieldy, int fieldx, fieldStruct *f, int gameH, int gameW, int starty, int startx) {
    int p = 0;
    int x = 0, y = 0;

    if (f[fieldy * gameW + fieldx].bomb == 1) {
        mvwaddch(stdscr, fieldy + starty, (fieldx * 2) + startx, 'B');
        f[fieldy * gameW + fieldx].opened = 1;
        wrefresh(stdscr);
        return -1;
    }
    if ((p = pointCheck(fieldy, fieldx, f, gameH, gameW)) == 0) {
        f[fieldy * gameW + fieldx].opened = 1;
        attrset(COLOR_PAIR(1));
        mvwaddch(stdscr, fieldy + starty, (fieldx * 2) + startx, '.');
        attrset(A_NORMAL);
        wrefresh(stdscr);
        for (int i = 0; i < 9; i++) {
            if (i != 4) { /*omit checked point*/
                y = i / 3 - 1;
                x = i % 3 - 1;
                if (fieldy + y < gameH && fieldy + y >= 0 && fieldx + x >= 0 && fieldx + x < gameW) {
                    if (f[((fieldy + y) * gameW) + fieldx + x].opened == 0 && f[(fieldy + y) * gameW + fieldx + x].bomb != 1)
                        openField(fieldy + y, fieldx + x, f, gameH, gameW, starty, startx);
                }
            }
        }
    } else {
        f[fieldy * gameW + fieldx].opened = 1;
        char num[5];
        sprintf(num, "%d", p);
        attrset(COLOR_PAIR(9 + p));
        mvwaddch(stdscr, fieldy + starty, (fieldx * 2) + startx, num[0]);
        attrset(A_NORMAL);
        wrefresh(stdscr);
        return p;
    }
    return 0;
}

int getRandomNumer(int min, int max) {
    return (min + rand() / (RAND_MAX / (max - min + 1) + 1));
}

void bombDraw(int fieldy, int fieldx, fieldStruct *f, int gameH, int gameW,
              int bombCount) {
    int b;
    srand(time(NULL));
    for (int i = 0; i < bombCount; i++) {
        while ((b = getRandomNumer(0, ((gameH) * (gameW)-1))) == (fieldy * gameW + fieldx) || (f[b].bomb == 1)) {
        }
        f[b].bomb = 1;
        // mvwprintw(stdscr, 3, 2, "y: %d, x: %d", b/gameH, b%gameH);
        // wrefresh(stdscr);
        // getch();
    }
}

// testfunction
void showBombs(fieldStruct *f, int gameH, int gameW, int starty, int startx) {
    attrset(COLOR_PAIR(3) | A_BOLD);
    for (int i = 0; i < gameH; i++) {
        for (int j = 0; j < gameW; j++) {
            if (f[i * gameW + j].bomb == 1)
                mvwaddch(stdscr, i + starty, (j * 2) + startx, '*');
        }
    }
    attrset(A_NORMAL);
    wrefresh(stdscr);
}

int flagCount(fieldStruct *field, int gameH, int gameW) {
    int c = 0;
    for (int i = 0; i < gameH * gameW; i++) {
        if (field[i].flag == 1)
            c++;
    }
    return c;
}

int openedCount(fieldStruct *field, int gameH, int gameW) {
    int c = 0;
    for (int i = 0; i < gameH * gameW; i++) {
        if (field[i].opened == 1)
            c++;
    }
    return c;
}

int checkFlags(fieldStruct *field, int gameH, int gameW) {
    for (int i = 0; i < gameH * gameW; i++) {
        if (field[i].bomb != 1 && field[i].flag == 1)
            return 0;  // bumb count <> flag count
        if (field[i].bomb == 1 && field[i].flag != 1)
            return 0;  // bumb count <> flag count
    }
    return 1;
}

int checkResult(fieldStruct *field, int gameH, int gameW, int bombCount) {
    int fc = flagCount(field, gameH, gameW);
    int oc = openedCount(field, gameH, gameW);
    if (fc == bombCount && (fc + oc) == (gameH * gameW) && checkFlags(field, gameH, gameW) == 1)
        return 1;  // won
    else
        return 0;
}
void initColor() {
    /* configure colors */
    init_pair(1, COLOR_WHITE, COLOR_BLUE);      // for open tiles
    init_pair(2, COLOR_BLACK, COLOR_GREEN);     // status field for coordinates
    init_pair(3, COLOR_RED, COLOR_BLACK);       // show bombs
    init_pair(4, COLOR_WHITE, COLOR_RED);       // status field for result
    init_pair(10, COLOR_RED, COLOR_BLACK);      // 1
    init_pair(11, COLOR_YELLOW, COLOR_BLACK);   // 2
    init_pair(12, COLOR_CYAN, COLOR_BLACK);     // 3
    init_pair(13, COLOR_MAGENTA, COLOR_BLACK);  // 4
    init_pair(14, COLOR_GREEN, COLOR_BLACK);    // 5
    init_pair(15, COLOR_BLUE, COLOR_BLACK);     // 6
    init_pair(16, COLOR_WHITE, COLOR_BLACK);    // 7
    init_pair(17, COLOR_WHITE, COLOR_BLACK);    // 8
}

void initField(fieldStruct *field, int gameH, int gameW) {
    for (int i = 0; i < gameH * gameW; i++) {
        field[i].bomb = 0;
        field[i].flag = 0;
        field[i].opened = 0;
    }
}

volatile int clockStop = 0;
volatile int clockTime = 0;

void *clockChild() {
    while (clockStop == 0) {
        sleep(1);
        clockTime++;
    }
    return NULL;
}

char *strWON =
    "\
 _     _  _______  __    _  \n\
| | _ | ||       ||  |  | |\n\
| || || ||   _   ||   |_| |\n\
|       ||  | |  ||       |\n\
|       ||  |_|  ||  _    |\n\
|   _   ||       || | |   |\n\
|__| |__||_______||_|  |__|";

char *strLOST =
    "\
 ___      _______  _______  _______ \n\
|   |    |       ||       ||       |\n\
|   |    |   _   ||  _____||_     _|\n\
|   |    |  | |  || |_____   |   |  \n\
|   |___ |  |_|  ||_____  |  |   |  \n\
|       ||       | _____| |  |   |  \n\
|_______||_______||_______|  |___|";

void showEndResult(WINDOW *winResult, WINDOW *winStatus, fieldStruct *field, pthread_t clockPtr, int won) {
    // mvwprintw(stdscr, 3, 3, "%s",strWON);
    WINDOW *w;
    w = newwin(8, 37, 3, 2);
    wbkgd(w, COLOR_PAIR(4));
    if (won == 1) {
        mvwaddstr(w, 0, 0, strWON);
        mvwprintw(winResult, 0, 1, "%s", "YOU WON");
    } else {
        mvwaddstr(w, 0, 0, strLOST);
        mvwprintw(winResult, 0, 1, "%s", "YOU LOST");
    }
    wrefresh(w);
    wrefresh(winResult);
    free(field);
    clockStop = 1;
    pthread_join(clockPtr, NULL);
    getch();
    clockTime = 0;
    clockStop = 0;
    delwin(winStatus);
    delwin(winResult);
    delwin(w);
}

int minePlay(int gameH, int gameW, int bombCount) {
    int bombDrawVar = 0;
    int move = 0;

    fieldStruct *field;

    field = malloc(gameH * gameW * sizeof(fieldStruct));
    // position in array: fieldy*gameW+fieldx
    initField(field, gameH, gameW);

    clear(); /* clear the screen */
    // box(stdscr,'*','*');
    border('|', '|', '-', '-', '+', '+', '+', '+');

    attron(A_BOLD);
    mvwprintw(stdscr, 1, 2, "%dx%d, mines: %d", gameH, gameW, bombCount);
    attrset(A_NORMAL);
    mvwprintw(stdscr, LINES - 2, 2, "UP, DOWN, LEFT, RIGHT - move; SPACE/ENTER - open; F - flag; q - quit.");
    refresh();

    int starty, startx;
    int fieldy, fieldx;
    starty = (LINES / 2 - gameH / 2);
    startx = (COLS / 2 - gameW);

    WINDOW *winResult, *winStatus;
    winStatus = newwin(4, 15, LINES - 6, 2);
    winResult = newwin(1, 15, LINES - 7, 2);

    wbkgd(winStatus, COLOR_PAIR(2));
    wbkgd(winResult, COLOR_PAIR(4));

    wrefresh(winStatus);
    wrefresh(winResult);

    for (int i = 0; i < gameH; i++) {
        for (int j = 0; j < gameW; j++) {
            mvwaddch(stdscr, i + starty, (j * 2) + startx, '~');
        }
    }

    wmove(stdscr, starty, startx);
    fieldy = 0;
    fieldx = 0;
    curs_set(1);
    wrefresh(stdscr);
    int ch;
    pthread_t clockPtr;
    while ((ch = getch())) {
        if ((ch == KEY_DOWN || ch == 's' || ch == 'S' || ch == KEY_UP || ch == 'w' || ch == 'W' || ch == KEY_LEFT || ch == 'a' || ch == 'A' || ch == KEY_RIGHT || ch == 'd' || ch == 'D' || ch == 'F' || ch == ' ' || ch == 10) && move == 0) {
            // start clock
            pthread_create(&clockPtr, NULL, clockChild, NULL);  // create child
        }

        switch (ch) {
            case KEY_DOWN:
            case 's':
            case 'S':
                if (fieldy < gameH - 1) {
                    fieldy++;
                    move++;
                    wmove(stdscr, starty + fieldy, startx + fieldx * 2);
                }
                break;
            case KEY_UP:
            case 'w':
            case 'W':
                if (fieldy > 0) {
                    fieldy--;
                    move++;
                    wmove(stdscr, starty + fieldy, startx + fieldx * 2);
                }

                break;
            case KEY_LEFT:
            case 'a':
            case 'A':
                if (fieldx > 0) {
                    fieldx--;
                    move++;
                    wmove(stdscr, starty + fieldy, startx + fieldx * 2);
                }
                break;
            case KEY_RIGHT:
            case 'd':
            case 'D':
                if (fieldx < gameW - 1) {
                    fieldx++;
                    move++;
                    wmove(stdscr, starty + fieldy, startx + fieldx * 2);
                }
                break;
            case 'f':
            case 'F':
                move++;
                // put flag
                if (field[fieldy * gameW + fieldx].flag == 0 && field[fieldy * gameW + fieldx].opened != 1) {
                    field[fieldy * gameW + fieldx].flag = 1;
                    waddch(stdscr, 'X');
                    wmove(stdscr, getcury(stdscr), getcurx(stdscr) - 1);
                    if (checkResult(field, gameH, gameW, bombCount) == 1) {
                        curs_set(0);
                        showEndResult(winResult, winStatus, field, clockPtr, 1);
                        return (1);
                    }
                } else {  // remove flag
                    if (field[fieldy * gameW + fieldx].flag == 1) {
                        field[fieldy * gameW + fieldx].flag = 0;
                        waddch(stdscr, '~');
                        wmove(stdscr, getcury(stdscr), getcurx(stdscr) - 1);
                    }
                }

                break;
            case ' ':
            case 10:  // enter key
                move++;
                if (bombDrawVar == 0) {
                    bombDraw(fieldy, fieldx, field, gameH, gameW, bombCount);
                    bombDrawVar = 1;
                    // showBombs(field, gameH, gameW, starty, startx);
                    wmove(stdscr, starty + fieldy, startx + fieldx * 2);
                    // getch();
                    openField(fieldy, fieldx, field, gameH, gameW, starty, startx);
                    wmove(stdscr, starty + fieldy, startx + fieldx * 2);
                } else {
                    if (openField(fieldy, fieldx, field, gameH, gameW, starty, startx) != -1) {
                        wmove(stdscr, starty + fieldy, startx + fieldx * 2);
                        if (checkResult(field, gameH, gameW, bombCount) == 1) {
                            curs_set(0);
                            showEndResult(winResult, winStatus, field, clockPtr, 1);

                            return (1);
                        }
                    } else { /* -1 is a mine*/
                        showBombs(field, gameH, gameW, starty, startx);
                        wmove(stdscr, starty + fieldy, startx + fieldx * 2);
                        // You pressed a mine
                        showEndResult(winResult, winStatus, field, clockPtr, 0);

                        return (-1);
                    }
                }
                break;
            case 'q':
            case 'Q':
                free(field);
                clockStop = 1;
                if (move != 0)
                    pthread_join(clockPtr, NULL);
                clockTime = 0;
                clockStop = 0;
                delwin(winStatus);
                delwin(winResult);
                return ((int)'q');
                break;
            default:
                break;
        }
        mvwprintw(winStatus, 0, 1, "FLAGS: %d  ", flagCount(field, gameH, gameW));
        mvwprintw(winStatus, 1, 1, "MOVE: %d  ", move);
        mvwprintw(winStatus, 2, 1, "Y: %d, X: %d  ", fieldy, fieldx);
        mvwprintw(winStatus, 3, 1, "Time: %d", clockTime);
        wrefresh(winStatus);
        wrefresh(stdscr);
    }
    return 0;
}

int selectGame() {
    int ch;

    do {
        curs_set(0);
        clear();
        box(stdscr, '*', '*');
        refresh();
        mvprintw(
            LINES / 2 - 4, COLS / 2 - 16,
            "Select game:"); /* print the message at the center of the screen */
        mvprintw(LINES / 2 - 2, COLS / 2 - 16, "1. Beginer 9x9, mines: 10");
        mvprintw(LINES / 2 - 1, COLS / 2 - 16, "2. Intermediate, 16x16, mines: 40");
        mvprintw(LINES / 2 - 0, COLS / 2 - 16, "3. Expert, 16x30, mines: 99");
        mvprintw(LINES / 2 + 1, COLS / 2 - 16, "q. Quit");
        refresh();
        ch = getch();

        switch (ch) {
            case '1':
                minePlay(9, 9, 10);
                break;
            case '2':
                minePlay(16, 16, 40);
                break;
            case '3':
                minePlay(16, 30, 99);
                break;
            case 'q':
            case 'Q':
                endwin();
                return 0;
                break;
        }
    } while (ch != 'q');
    return 0;
}

int main(void) {
    initscr(); /* Start curses mode            */

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    } else {
        start_color();
        initColor();
    }

    raw();    /* Line buffering disabled      */
    cbreak(); /* Line buffering disabled, Pass on
               * every thing to me            */
    noecho();
    keypad(stdscr, TRUE); /* I need that nifty F1         */
    logoPrint();

    selectGame();

    endwin();
    return 0;
}
