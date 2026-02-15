#ifndef CONFIG_H
#define CONFIG_H

//Pin layout for tft (declared in TFT_eSPI/User_Setup.h)
/*
#define MISO 39
#define SCK 5
#define MOSI 6
#define DC 15
#define RESET 46
#define CS 7
*/

// Pin layout for buttons and tft led
#define PIN_LED    18
#define PIN_LEFT   20
#define PIN_RIGHT  21
#define PIN_UP     47
#define PIN_DOWN   48
#define PIN_CLICK  45
#define PIN_FLAG   40

// Game settings
#define TILE_SIZE  16      // Size of one tile in pixels
#define COLS       20      // number of columns
#define ROWS       14      // number of rows 
#define SCREEN_W   320     
#define SCREEN_H   240

// Colors
#define GAME_DARK_GREY   0x8410

// Timings
#define MOVE_DELAY      180
#define FIRST_CLICK_DELAY 300
  
// NVS Namespace
#define PREF_NAMESPACE  "minesweeper"

#endif