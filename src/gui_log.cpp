#include "student_db.hpp"
#include "card_reader.hpp"
#include <curses.h>

int main(void)
{
    initscr();
    printw("Hello World!!!");
    refresh();
    getch();
    endwin();
    return 0;
}
