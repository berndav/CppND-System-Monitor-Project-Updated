#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#define DISPLAY_NUM_PROCS 10

#include <curses.h>

#include "process.h"
#include "system.h"

namespace NCursesDisplay {
void Display(System& system, int n = DISPLAY_NUM_PROCS);
void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
std::string ProgressBar(float percent);
};  // namespace NCursesDisplay

#endif