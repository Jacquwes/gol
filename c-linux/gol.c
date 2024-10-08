#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef char cell;

enum cell_state : cell
{
  ALIVE_CELL,
  DEAD_CELL,
};

void clear_screen()
{
  printf("\x1b[H\x1b[J");
}

uint8_t count_neighbors(cell* grid, uint32_t height, uint32_t width, uint32_t x, uint32_t y)
{
  uint8_t neighbor_count = 0;
  for (uint8_t y_offset = -1; y_offset <= 1; y_offset++)
  {
    uint32_t y_index = (y + y_offset) % height;

    for (uint8_t x_offset = -1; x_offset <= 1; x_offset++)
    {
      uint32_t x_index = (x + x_offset) % width;

      if (grid[y_index * width + x_index] == ALIVE_CELL)
        neighbor_count++;
    }
  }
  return neighbor_count;
}

void display_game_state(cell* grid, uint32_t height, uint32_t width)
{
  
}

void init_pattern(cell* grid, uint32_t height, uint32_t width)
{
  for (uint32_t y = 0; y < height; y++)
    for (uint32_t x = 0; x < width; x++)
        grid[y * width + x] = 
          rand() & 0b1111 == 0b1111 ? // ~7% of alive cells
          ALIVE_CELL :
          DEAD_CELL;
}

void game_loop(cell* grid, uint32_t height, uint32_t width)
{
  cell* next_grid = malloc(sizeof(cell) * height * width);

  for (uint32_t y = 0; y < height; y++)
    for (uint32_t x = 0; x < width; x++)
    {
      uint8_t neighbor_count = count_neighbors(grid, height, width, x, y);

      cell current_cell_state = grid[y * width + x];
      cell* next_cell_state = &next_grid[y * width + x];

      if (current_cell_state == DEAD_CELL && neighbor_count == 3)
        *next_cell_state = ALIVE_CELL;
      else if (current_cell_state == ALIVE_CELL && (neighbor_count == 2 || neighbor_count == 3))
        *next_cell_state = ALIVE_CELL;
      else
        *next_cell_state = DEAD_CELL;
    }

  memmove(grid, next_grid, height * width);
  free(next_grid);
}

int main()
{
  srand(time(NULL));

  uint32_t height = 25;
  uint32_t width = 80;

  cell* grid = malloc(sizeof(cell) * height * width);
  init_pattern(grid, height, width);
  clear_screen();

  while (1)
  {
    game_loop(grid, height, width);
    display_game_state(grid, height, width);
  }
}
