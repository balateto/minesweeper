#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <Arduino.h>

int saveScore(int difficulty, unsigned long time);

std::vector<unsigned long> getScores();

#endif