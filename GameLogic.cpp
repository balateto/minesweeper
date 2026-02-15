#include "GameLogic.h"
#include <random>
#include <algorithm>

// Tile class implementation
Tile::Tile(int w, int h, int x, int y): w(w), h(h), x(x), y(y)
{
  flag = false;
  clicked = false;
  bomb = false;
  neighbours = 0;
}
void Tile::setNeighbours(int neighbours) { this->neighbours = neighbours; }
void Tile::setFlag(bool flag) { this->flag = flag; }
void Tile::setClicked(bool clicked) { this->clicked = clicked; }
void Tile::setBomb(bool bomb) { this->bomb = bomb; }
bool Tile::getClicked() const { return clicked; }
bool Tile::getFlag() const { return flag; }
bool Tile::getBomb() const { return bomb; }
int Tile::getNeighbours() const { return neighbours; }
bool Tile::isBombClicked() { return bomb && clicked; }

void Tile::select(bool selected)
{
  if (selected) {
     tft.drawRect(x, y, w, h, TFT_BLACK);
     tft.drawRect(x+1, y+1, w-2, h-2, TFT_BLACK);
  }
  else
  {
     this->draw();
  }
}

void Tile::draw() {
  if(flag)
  {
    tft.pushImage(x, y, w, h, icon_flag);
    tft.drawRect(x, y, w, h, TFT_BLACK);
  }
  else if(bomb && clicked)
  {
    tft.pushImage(x, y, w, h, icon_bomb_exploded);
    tft.drawRect(x, y, w, h, TFT_BLACK);
  }
  else if(clicked)
  {
    if (neighbours >= 0 && neighbours <= 8)
    {
      tft.pushImage(x, y, w, h, Icons[neighbours]);
      tft.drawRect(x, y, w, h, TFT_BLACK);
    }
  }
  else
  {
    tft.fillRect(x, y, w, h, GAME_DARK_GREY); 
    tft.drawRect(x, y, w, h, TFT_BLACK);
  }
}

void Tile::drawBomb() {
  tft.pushImage(x, y, w, h, icon_bomb);
  tft.drawRect(x, y, w, h, TFT_BLACK);
}

// Game algorithms

void countNeighbours(int cols, int rows, std::vector<Tile>& grid)
{
  for(int index = 0; index < cols * rows; index++)
  {
     int neighbours = 0;
     for(int i = 0; i < 8; i++)
     {
        switch(i)
        {
          case 0:
            if(index % cols != 0 && index >= cols)
            {
              int p = index - cols - 1;
              if(grid[p].getBomb()) neighbours++;
            }
            break;
          case 1:
            if(index >= cols)
            {
              int p = index - cols;
              if(grid[p].getBomb()) neighbours++;
            }
           break;
         case 2:
            if((index + 1) % cols != 0 && index >= cols)
            {
              int p = index - cols + 1;
              if(grid[p].getBomb()) neighbours++;
            }
            break;
         case 3:
            if((index + 1) % cols != 0)
            {
              int p = index + 1;
              if(grid[p].getBomb()) neighbours++;
            }
           break;
         case 4:
            if((index + 1) % cols != 0 && index < (rows - 1) * cols)
            {
              int p = index + cols + 1;
              if(grid[p].getBomb()) neighbours++;
            }
           break;
          case 5:
            if(index < (rows - 1) * cols)
            {
              int p = index + cols;
              if(grid[p].getBomb()) neighbours++;
            }
            break;
          case 6:
            if(index % cols != 0 && index < (rows - 1) * cols)
            {
              int p = index + cols - 1;
              if(grid[p].getBomb()) neighbours++;
            }
            break;
          case 7:
            if(index % cols != 0)
            {
              int p = index - 1;
              if(grid[p].getBomb()) neighbours++;
            }
            break;
        }
      }
      grid[index].setNeighbours(neighbours);
  }
}

void FloodFill(int cols, int rows, std::vector<Tile>& grid, std::vector<int>& zeros)
{
  while(!zeros.empty())
  {
    int index = zeros.back();
    zeros.pop_back();
    if(grid[index].getClicked()) continue;
    grid[index].setClicked(true);
    grid[index].draw();
    for(int i = 0; i < 8; i++)
    {
        switch(i) 
        {
          case 0:
            if(index % cols != 0 && index >= cols)
            {
              int p = index - cols - 1;
              if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() == 0) {
                zeros.push_back(p);
              }
              else if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() != 0) {
                grid[p].setClicked(true);
                grid[p].draw();
              }
            }
            break;
          case 1:
            if(index >= cols)
            {
              int p = index - cols;
              if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() == 0) {
                zeros.push_back(p);
              }
              else if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() != 0) {
                grid[p].setClicked(true);
                grid[p].draw();
              }
            }
           break;
         case 2:
            if((index + 1) % cols != 0 && index >= cols)
            {
              int p = index - cols + 1;
             if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() == 0) {
                zeros.push_back(p);
              }
              else if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() != 0) {
                grid[p].setClicked(true);
                grid[p].draw();
              }
            }
            break;
         case 3:
            if((index + 1) % cols != 0)
            {
              int p = index + 1;
              if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() == 0) {
                zeros.push_back(p);
              }
              else if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() != 0) {
                grid[p].setClicked(true);
                grid[p].draw();
              }
            }
           break;
         case 4:
            if((index + 1) % cols != 0 && index < (rows - 1) * cols)
            {
              int p = index + cols + 1;
              if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() == 0) {
                zeros.push_back(p);
              }
              else if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() != 0) {
                grid[p].setClicked(true);
                grid[p].draw();
              }
            }
           break;
          case 5:
            if(index < (rows - 1) * cols)
            {
              int p = index + cols;
              if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() == 0) {
                zeros.push_back(p);
              }
              else if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() != 0) {
                grid[p].setClicked(true);
                grid[p].draw();
              }
            } 
            break;
          case 6:
            if(index % cols != 0 && index < (rows - 1) * cols)
            {
              int p = index + cols - 1;
              if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() == 0) {
                zeros.push_back(p);
              }
              else if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() != 0) {
                grid[p].setClicked(true);
                grid[p].draw();
              }
            }
            break;
          case 7:
            if(index % cols != 0)
            {
              int p = index - 1;
              if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() == 0) {
                zeros.push_back(p);
              }
              else if(!grid[p].getClicked() && !grid[p].getFlag() && grid[p].getNeighbours() != 0) {
                grid[p].setClicked(true);
                grid[p].draw();
              }
            }
            break;
        }
    }
  }
}

int winCondition(int cols, int rows, int bombs, std::vector<Tile>& grid)
{
  int sum = 0;
  std::vector<Tile>::iterator it;
  for(it = grid.begin(); it != grid.end(); it++)
  {
    if(!(*it).getBomb() && (*it).getClicked()) sum++;
  }
  if (sum == (cols * rows - bombs)) return 2;
  else return 0;
}

void reset(int cols, int rows, std::vector<Tile>& grid)
{
  std::vector<Tile>:: iterator it;
  for(it = grid.begin(); it != grid.end(); it++)
  {
    (*it).setFlag(false);
    (*it).setBomb(false);
    (*it).setClicked(false);
    (*it).setNeighbours(0);
  }

  for(it = grid.begin(); it != grid.end(); it++)
  {
    (*it).draw();
  }

}

void setBombs(int cols, int rows, int difficulty, int index, std::vector<Tile>& grid)
{
  std::vector<int> indexes;
  

  indexes.push_back(index);

  for(int i = 0; i < 8; i++)
    {
        switch(i) 
        {
          case 0:
            if(index % cols != 0 && index >= cols)
            {
              int p = index - cols - 1;
              indexes.push_back(p);
            }
            break;
          case 1:
            if(index >= cols)
            {
              int p = index - cols;
              indexes.push_back(p);
            }
           break;
         case 2:
            if((index + 1) % cols != 0 && index >= cols)
            {
              int p = index - cols + 1;
              indexes.push_back(p);
            }
            break;
         case 3:
            if((index + 1) % cols != 0)
            {
              int p = index + 1;
              indexes.push_back(p);
            }
           break;
         case 4:
            if((index + 1) % cols != 0 && index < (rows - 1) * cols)
            {
              int p = index + cols + 1;
              indexes.push_back(p);
            }
           break;
          case 5:
            if(index < (rows - 1) * cols)
            {
              int p = index + cols;
              indexes.push_back(p);
            } 
            break;
          case 6:
            if(index % cols != 0 && index < (rows - 1) * cols)
            {
              int p = index + cols - 1;
              indexes.push_back(p);
            }
            break;
          case 7:
            if(index % cols != 0)
            {
              int p = index - 1;
              indexes.push_back(p);
            }
            break;
        }
    }

  std::vector<int> numbers;

  for(int i = 0; i < (cols * rows); i++) numbers.push_back(i);

  for(std::vector<int>::iterator iit = indexes.begin(); iit != indexes.end(); iit++)
  {
    for(std::vector<int>::iterator nit = numbers.begin(); nit != numbers.end(); nit++) {
      if((*iit) == (*nit))
      {
        numbers.erase(nit);
        break;
      }
    } 
  }

  int bombs = 0;

  if(difficulty == 0) bombs = 25;
  else if(difficulty == 1) bombs = 40;
  else bombs = 60;

  unsigned seed = esp_random();
  shuffle(numbers.begin(), numbers.end(), std::default_random_engine(seed));
  for(int i=0; i < bombs; i++)
  {
    grid[numbers[i]].setBomb(true);
  }

  countNeighbours(cols, rows, grid);
}

