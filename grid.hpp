#pragma once

#include <cstdint>
#include <vector>

namespace gol
{
  class grid
  {
  public:
    grid() = delete;

    constexpr grid(uint64_t width, uint64_t height)
      : cells(width* height), width(width), height(height)
    {}

    constexpr bool get(uint64_t x, uint64_t y) const
    {
      return this->cells[y * this->width + x];

    }

    constexpr void set(uint64_t x, uint64_t y, bool value)
    {
      this->cells[y * this->width + x] = value;
    }

    constexpr uint64_t get_width() const
    {
      return this->width;
    }

    constexpr uint64_t get_height() const
    {
      return this->height;
    }

    constexpr const std::vector<bool>& get_cells() const
    {
      return this->cells;
    }

  private:
    std::vector<bool> cells;
    uint64_t width;
    uint64_t height;
  };
}
