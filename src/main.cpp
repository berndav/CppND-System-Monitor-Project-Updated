#include <iostream>
#include <vector>

#include <chrono>
#include <thread>

#include "ncurses_display.h"
#include "process.h"
#include "system.h"

int main() {

  System system;
  NCursesDisplay::Display(system);
}