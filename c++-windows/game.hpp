#pragma once

#include <cstdint>
#include <vector>
#include <Windows.h>

#include "grid.hpp"

namespace gol
{
  class game
  {
  public:
    game(uint64_t width, uint64_t height)
      : grid(width, height)
    {}

    constexpr void update()
    {
      gol::grid new_grid(this->grid.get_width(), this->grid.get_height());

      for (uint64_t y = 0; y < this->grid.get_height(); y++)
        for (uint64_t x = 0; x < this->grid.get_width(); x++)
        {
          bool alive = this->grid.get(x, y);
          uint8_t neighbors = this->get_neighbors(x, y);

          if (alive && (neighbors == 2 || neighbors == 3))
            new_grid.set(x, y, true);
          else if (!alive && neighbors == 3)
            new_grid.set(x, y, true);
          else
            new_grid.set(x, y, false);
        }

      this->grid = new_grid;
    }

    void render() const
    {
      CONSOLE_SCREEN_BUFFER_INFO console_info;

      GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_info);

      uint64_t buffer_size = this->grid.get_width() * this->grid.get_height();
      std::vector<CHAR_INFO> cells(buffer_size);
      for (uint64_t y = 0; y < this->grid.get_height(); y++)
        for (uint64_t x = 0; x < this->grid.get_width(); x++)
        {
          CHAR_INFO& cell = cells[y * this->grid.get_width() + x];
          cell.Char.AsciiChar = this->grid.get(x, y) ? '#' : ' ';
          cell.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        }

      COORD coordinates = { 0, 0 };
      COORD size = { static_cast<SHORT>(this->grid.get_width()), static_cast<SHORT>(this->grid.get_height()) };

      WriteConsoleOutputA(
        GetStdHandle(STD_OUTPUT_HANDLE),
        cells.data(),
        size,
        coordinates,
        &console_info.srWindow);
    }

    constexpr bool get(uint64_t x, uint64_t y) const
    {
      return this->grid.get(x, y);
    }

    constexpr void set(uint64_t x, uint64_t y, bool value)
    {
      this->grid.set(x, y, value);
    }

    constexpr uint64_t get_width() const
    {
      return this->grid.get_width();
    }

    constexpr uint64_t get_height() const
    {
      return this->grid.get_height();
    }

  private:
    grid grid;

    constexpr uint8_t get_neighbors(uint64_t x, uint64_t y) const
    {
      uint8_t neighbors = 0;

      for (uint64_t neighbor_y = 0; neighbor_y < 3; neighbor_y++)
        for (uint64_t neighbor_x = 0; neighbor_x < 3; neighbor_x++)
        {
          // Skip the cell itself
          if (neighbor_x == 1 && neighbor_y == 1)
            continue;

          int64_t cell_x = x + neighbor_x - 1;
          int64_t cell_y = y + neighbor_y - 1;

          // The left of the console wraps around to the right
          if (cell_x < 0)
            cell_x = this->grid.get_width() - 1;
          else if (cell_x >= static_cast<int64_t>(this->grid.get_width()))
            cell_x = 0;

          // The top of the console wraps around to the bottom
          if (cell_y < 0)
            cell_y = this->grid.get_height() - 1;
          else if (cell_y >= static_cast<int64_t>(this->grid.get_height()))
            cell_y = 0;

          if (this->grid.get(cell_x, cell_y))
            neighbors++;
        }

      return neighbors;
    }
  };
}