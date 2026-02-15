#include "Graphics.h"
#include "Icons.h"
#include "Storage.h"

String millisToTime(unsigned long ms)
{
  unsigned long allSeconds = ms / 1000;
  int minutes = floor(allSeconds / 60);
  int secs = allSeconds - (minutes * 60);

  String time = String(minutes) + ":" + (secs < 10 ? "0" : "") + String(secs);

  return time;
}

void drawHUD(unsigned long lastTime, int flags, int bombs) 
{
  unsigned long ms = millis() - lastTime;

  hud.fillSprite(GAME_DARK_GREY);

  // Flag counter
  hud.pushImage(20, 0, 16, 16, icon_flag);
  hud.setTextColor(TFT_BLACK, GAME_DARK_GREY); 
  hud.setTextSize(2);
  hud.setTextDatum(TL_DATUM);
  String FlagCounter = String(flags) + "/" + String(bombs);
  hud.drawString(FlagCounter, 40, 0);

  // Timer
  hud.setTextDatum(TR_DATUM);
  hud.drawString(millisToTime(ms), tft.width() - 5, 0);

  // Bottom line
  hud.fillRect(0, 15, tft.width(), 1, TFT_BLACK);

  hud.pushSprite(0, 0);
}

void drawOptions(int optState, int optSelect, int difficulty)
{
  String diff = "";
  if(difficulty == 0) diff = "Beginner";
  else if(difficulty == 1) diff = "Intermediate";
  else diff = "Expert";

  //States of the settings
  switch(optState)
  {
    //Main menu
    case 0: 
    {

      options.fillSprite(GAME_DARK_GREY);

      options.setTextColor(TFT_BLACK);
      options.setTextSize(2);
      options.setTextDatum(MC_DATUM);

      options.drawString("Start Game", tft.width() / 2, tft.height() / 4);
      options.drawString("< " + diff + " >", tft.width() / 2, (tft.height() / 4) * 2);
      options.drawString("High scores", tft.width() / 2, (tft.height() / 4) * 3);

      //Option selected
      switch(optSelect) 
      {
        case 0:
        {
          int border = options.textWidth("Start Game") / 2;
          options.pushImage((tft.width() / 2) - border - 36, (tft.height() / 4) - 8, 16, 16, icon_flag);
          options.pushImage((tft.width() / 2) + border + 20, (tft.height() / 4) - 8, 16, 16, icon_flag);
        }
        break;
        case 1:
        {
          int border = options.textWidth("< " + diff + " >") / 2;
          options.pushImage((tft.width() / 2) - border - 36, ((tft.height() / 4) * 2) - 8, 16, 16, icon_flag);
          options.pushImage((tft.width() / 2) + border + 20, ((tft.height() / 4) * 2) - 8, 16, 16, icon_flag);
        }
        break;
        case 2:
        {
          int border = options.textWidth("High scores") / 2;
          options.pushImage((tft.width() / 2) - border - 36, ((tft.height() / 4) * 3) - 8, 16, 16, icon_flag);
          options.pushImage((tft.width() / 2) + border + 20, ((tft.height() / 4) * 3) - 8, 16, 16, icon_flag);
        }
        break;
      }
    }
    break;

    //High scores
    case 1:
    {
      options.fillSprite(GAME_DARK_GREY);

      options.setTextColor(TFT_BLACK);
      options.setTextSize(2);
      options.setTextDatum(TL_DATUM);
      options.drawString("Exit", 40, 5);

      int border = options.textWidth("Exit");
      int halfHeight = options.fontHeight() / 2;
      options.pushImage(4, 5 + halfHeight - 8, 16, 16, icon_flag);
      options.pushImage(40 + border + 20, 5 + halfHeight - 8, 16, 16, icon_flag);

      std::vector<unsigned long> scores = getScores();

      options.setTextSize(2);
      options.setTextDatum(ML_DATUM);

      for(int i = 0; i < 3; i++)
      {
        String diff = "";
        if(i == 0) diff = "Beginner";
        else if(i == 1) diff = "Intermediate";
        else diff = "Expert";

        unsigned long currentScore = (i < scores.size()) ? scores[i] : 0;

        String score = "";

        if(currentScore == 99999999) score = diff + "   N/A";
        else score = diff + "   " + millisToTime(currentScore);
       
        options.drawString(score, tft.width() / 2 - 130, ((tft.height() / 4) + 20) + i * (tft.height() / 4)); 
      }
    }
    break;
  }

  options.pushSprite(0, 0);
}
