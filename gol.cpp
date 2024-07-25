
#include "game.hpp"

int main()
{
  CONSOLE_SCREEN_BUFFER_INFO console_info;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_info);
  const uint64_t width = console_info.srWindow.Right - console_info.srWindow.Left + 1;
  const uint64_t height = console_info.srWindow.Bottom - console_info.srWindow.Top + 1;

  gol::game game(width, height);

  /* Glider
  *  #
  *   #
  * ###
  */
  game.set(0, 2, true);
  game.set(1, 2, true);
  game.set(2, 2, true);
  game.set(2, 1, true);
  game.set(1, 0, true);

  while (true)
  {
    game.update();
    game.render();
    Sleep(100);
  }
}
