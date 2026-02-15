#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <TFT_eSPI.h>
#include <vector>
#include "Config.h"
#include "Icons.h"

extern TFT_eSPI tft; //TODO: separate graphics and game logic

/**
 * @class Tile
 * @brief Represents a single cell on the Minesweeper grid.
 */
class Tile {
  int w, h, x, y, neighbours;
  bool flag, clicked, bomb;
public:
  Tile(int w, int h, int x, int y);
  void setNeighbours(int neighbours);
  void setFlag(bool flag);
  void setClicked(bool clicked);
  void setBomb(bool bomb);
  void select(bool selected);
  void draw();
  void drawBomb();
  bool isBombClicked();
  bool getClicked() const;
  bool getFlag() const;
  bool getBomb() const;
  int getNeighbours() const;
};

// Game Algorithms
void countNeighbours(int cols, int rows, std::vector<Tile>& grid);
void FloodFill(int cols, int rows, std::vector<Tile>& grid, std::vector<int>& zeros);
int winCondition(int cols, int rows, int bombs, std::vector<Tile>& grid);
void reset(int cols, int rows, std::vector<Tile>& grid);
void setBombs(int cols, int rows, int difficulty, int index, std::vector<Tile>& grid);

#endif