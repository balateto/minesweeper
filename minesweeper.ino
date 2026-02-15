#include <TFT_eSPI.h>
#include <SPI.h>
#include <vector>
#include <Bounce2.h>
#include "Config.h"
#include "Storage.h"
#include "Graphics.h"
#include "GameLogic.h"

// TFT objects
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite hud = TFT_eSprite(&tft);
TFT_eSprite options = TFT_eSprite(&tft);

// Inputs
Bounce btnLeft = Bounce();
Bounce btnRight = Bounce();
Bounce btnUp = Bounce();
Bounce btnDown = Bounce();
Bounce btnClick = Bounce();
Bounce btnFlag = Bounce();

// Game state
std::vector<Tile> grid;
std::vector<Tile>::iterator it;
int Index = 0;
int bombs = 40;
int flags = 0;
int state = 3; // starts in options state (0: gameplay, 1: game over, 2: win, 3: options)
int difficulty = 0;
int hudHeight = 0; // calculated in setup
int show = 1;
int firstClick = 1;

// Options state
int optState = 0;
int optSelect = 0;
int changed = 1;

// Timing
unsigned long lastTime = 0;
unsigned long Time = 0;
unsigned long lastMove = 0;

void setup()
{
  Serial.begin(115200);

  // Display setup
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  // Sprites setup
  hudHeight = SCREEN_H - (ROWS * TILE_SIZE);
  hud.setColorDepth(16); 
  hud.createSprite(320, hudHeight);

  options.setColorDepth(16);
  options.createSprite(tft.width(), tft.height());

  // Input setup
  btnLeft.attach(PIN_LEFT, INPUT);
  btnRight.attach(PIN_RIGHT, INPUT);
  btnUp.attach(PIN_UP, INPUT);
  btnDown.attach(PIN_DOWN, INPUT);
  btnClick.attach(PIN_CLICK, INPUT);
  btnFlag.attach(PIN_FLAG, INPUT);

  btnLeft.interval(5); 
  btnRight.interval(5);
  btnUp.interval(5);
  btnDown.interval(5);
  btnClick.interval(5);
  btnFlag.interval(5);

  // Grid setup
  grid.reserve(ROWS * COLS);

  for(int i = 0; i < ROWS; i++) 
  {
    for(int j = 0; j < COLS; j++)
    {
      Tile tile(TILE_SIZE, TILE_SIZE, j*TILE_SIZE, (i*TILE_SIZE) + hudHeight);
      grid.push_back(tile);
    }
  }

  lastTime = millis();

}

void loop()
{
  btnLeft.update();
  btnRight.update();
  btnUp.update();
  btnDown.update();
  btnClick.update();
  btnFlag.update();

    switch(state)
    {
      // Gameplay
      case 0:
      {
      bool moved = false;
      int oldIndex = Index;
    
      drawHUD(lastTime, flags, bombs);
    
      if (millis() - lastMove > MOVE_DELAY) {
       if(btnLeft.read() == HIGH)
       {
         if(Index % COLS == 0)
         {
           Index += (COLS - 1);
         }
        else
         {
           Index--;
         }
         moved = true;
         lastMove = millis();
       }
    
       else if(btnRight.read() == HIGH)
       {
       if((Index + 1) % COLS == 0)
         {
           Index -= (COLS - 1);
         }
         else
         {
            Index++;
          }
          moved = true;
         lastMove = millis();
       }
    
       else if(btnUp.read() == HIGH)
       {
         if(Index < COLS)
         {
            Index += (ROWS - 1) * COLS;
         }
          else
         { 
           Index -= COLS;
         }
         moved = true;
         lastMove = millis();
        }
    
        else if(btnDown.read() == HIGH)
        {
          if(Index >= (ROWS - 1) * COLS)
          {
            Index -= (ROWS - 1) * COLS;
          }
         else
         {
           Index += COLS;
          }
         moved = true;
          lastMove = millis();
       }
      }
    
      if(btnFlag.rose())
      {
        if(!grid[Index].getClicked())
        {
          bool currentStatus = grid[Index].getFlag();
          if(!currentStatus) flags++;
          else flags--;
          grid[Index].setFlag(!currentStatus);
          grid[Index].draw();
          grid[Index].select(true); 
        }
      }
    
    
      if (btnClick.rose()) {
        if(firstClick) {
          setBombs(COLS, ROWS, difficulty, Index, grid);
          firstClick = 0;
        }
        if(!grid[Index].getFlag() && !grid[Index].getClicked()) 
        {
         if(grid[Index].getNeighbours() == 0 && !grid[Index].getBomb())
          {
            std::vector<int> zeros;
            zeros.push_back(Index);
            FloodFill(COLS, ROWS, grid, zeros);
          }
          else
          {
            grid[Index].setClicked(true);
            grid[Index].draw();
            if(grid[Index].isBombClicked()) state = 1;
          }
          grid[Index].select(true);
          if(state != 1) state = winCondition(COLS, ROWS, bombs, grid);
          if(state)
          {
           lastMove = millis();
           show = 1;
          }
       }  
      }
    
      if(moved) {
        grid[Index].select(true);
        grid[oldIndex].select(false);
      }
      }
      break;

      // Game over
      case 1:
      {
      if(show)
      {
        for(it = grid.begin(); it != grid.end(); it++)
        {
          if((*it).getBomb() && !(*it).getFlag() && !(*it).getClicked()) {
            (*it).drawBomb();
          }
        }
    
        show = 0;
        lastMove = millis();
      }
    
    
      if(millis() - lastMove > FIRST_CLICK_DELAY)
      {
        if(btnClick.rose() || btnFlag.rose())
        {
          state = 3;
          show = 1;
          Index = 0;
          firstClick = 1;
          optState = 0;
          optSelect = 0;
          changed = 1;
          lastMove = millis();
          lastTime = millis();
        }
      }
      }
      break;

      // win
      case 2:
      {
        if(show)
        {
        Time = millis() - lastTime;
        int record = saveScore(difficulty, Time);
        if (record)
        {
          tft.fillScreen(TFT_GREEN);
          tft.setTextColor(TFT_BLACK);
          tft.setTextDatum(MC_DATUM);
          tft.setTextSize(4);
          tft.drawString("Uj rekord!", 160, 100);
          tft.setTextSize(2);
          String time = "Ido: " + millisToTime(Time);
          tft.drawString(time, 160, 150);
        }
        else
        {
          tft.fillScreen(TFT_GREEN);
          tft.setTextColor(TFT_BLACK);
          tft.setTextDatum(MC_DATUM);
          tft.setTextSize(4);
          tft.drawString("Nyertel!", 160, 100);
          tft.setTextSize(2);
          String time = "Ido: " + millisToTime(Time);
          tft.drawString(time, 160, 150);
        }
        show = 0;
        lastMove = millis();
        }
    
        if(millis() - lastMove > FIRST_CLICK_DELAY)
        {
          if(btnClick.rose() || btnFlag.rose())
          {
            state = 3;
            show = 1;
            Index = 0;
            firstClick = 1;
            optState = 0;
            optSelect = 0;
            changed = 1;
            lastMove = millis();
            lastTime = millis();
          }
        } 
    
      }
      break;

      // Options
      case 3:
      {
        // Redraw only when state or select has been changed
        if (changed) {
             drawOptions(optState, optSelect, difficulty);
             changed = 0; 
        }

        // Main menu with "start game" selected 
        if(optState == 0 && optSelect == 0)
        {
          
          if(btnClick.rose())
          {
            if(millis() - lastMove > FIRST_CLICK_DELAY)
            {
              state = 0;
              Index = 0;
              firstClick = 1;
              flags = 0;
              lastMove = millis();
              lastTime = millis();
              if(difficulty == 0) bombs = 25;
              else if(difficulty == 1) bombs = 40;
              else bombs = 60;
              tft.fillScreen(TFT_BLACK);
              drawHUD(lastTime, flags, bombs);
              reset(COLS, ROWS, grid);
              grid[Index].select(true);
            }
          }
          else if(btnUp.rose())
          {
            optSelect = 2;
            changed = 1;
          }
          else if(btnDown.rose())
          {
            optSelect = 1;
            changed = 1;
          }
          
        }

        // Main menu with difficulty selected
        else if(optState == 0 && optSelect == 1)
        {
          if(btnLeft.rose())
          {
            if(difficulty > 0) difficulty--;
            else difficulty = 2;
            changed = 1;
          }
          else if(btnRight.rose())
          {
            if(difficulty < 2) difficulty++;
            else difficulty = 0;
            changed = 1;
          }
          else if(btnUp.rose())
          {
            optSelect = 0;
            changed = 1;
          }
          else if(btnDown.rose())
          {
            optSelect = 2;
            changed = 1;
          }
        }

        // Main menu with "High scores" selected
        else if(optState == 0 && optSelect == 2)
        {
          if(btnClick.rose())
          {
            optSelect = 1;
            optState = 1;
            changed = 1;
          }
          else if(btnUp.rose())
          {
            optSelect = 1;
            changed = 1;
          }
          else if(btnDown.rose())
          {
            optSelect = 0;
            changed = 1;
          }
        }

        // High scores
        else if(optState == 1)
        {
          if(btnClick.rose())
          {
            optSelect = 0;
            optState = 0;
            changed = 1;
          }
        }
    
      }
      break;
    }

}



