#include "ncurses_display.h"
#include "system.h"

#include <iostream>

int main() {
  //  std::cout << "\nMaking system...\n";
  System system;
  NCursesDisplay::Display(system);
}