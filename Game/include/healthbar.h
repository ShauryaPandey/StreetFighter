#pragma once
#include"object.h"
typedef struct healthBar_t HealthBar;
HealthBar* initHealthbar(float x, float y);
void DeInitHealthBar(HealthBar* bar);
void updateHealthBar(HealthBar* bar, int delta);
void setHealthBar(HealthBar* bar, int val);
void _healthBarDraw(HealthBar* bar);
void resetHealthBar(HealthBar* bar);