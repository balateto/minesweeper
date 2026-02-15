#include "Storage.h"
#include "Config.h"
#include <Preferences.h>

Preferences preferences;

int saveScore(int difficulty, unsigned long time)
{
  int record = 0;
  preferences.begin(PREF_NAMESPACE, false);

  String diff = "";
  if(difficulty == 0) diff = "Beginner";
  else if(difficulty == 1) diff = "Intermediate";
  else diff = "Expert";

  // Use 99999999 as placeholder
  if(preferences.getULong(diff.c_str(), 99999999) > time) 
  {
    preferences.putULong(diff.c_str(), time);
    record = 1;
  }

  preferences.end();
  return record;
}

std::vector<unsigned long> getScores()
{
  preferences.begin("minesweeper", true);

  std::vector<unsigned long> scores;
  scores.reserve(3);
  scores.push_back(preferences.getULong("Beginner", 99999999));
  scores.push_back(preferences.getULong("Intermediate", 99999999));
  scores.push_back(preferences.getULong("Expert", 99999999));

  preferences.end();
  return scores;
}