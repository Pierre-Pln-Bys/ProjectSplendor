#include "ncurses.h"

struct dimension initialize_screen()
{
    initscr(); // Start nCurses
    raw(); // Disable buffering (i.e. don't wait for CR)
    noecho(); // Don't show typed charected on screen
    keypad(stdscr, TRUE); // Allow F1, F2, F..., Arrows, etc ...

    struct dimension dim;
    // getmaxyx is a macro (thus should be in uppercase).
    // Since it's a macro, it can change directly rows and cols without going
    // through a pointer. It's bad, really so bad ...
    getmaxyx(stdscr, dim.rows, dim.cols); // This is so F* ugly !
    return dim;
}

void release_screen()
{
    endwin(); // Release nCurses
}

void start_delayed_interaction(int delay)
{
    // halfdelay(delay);
    timeout(delay);
}

void stop_delayed_interaction()
{
    cbreak();
}
