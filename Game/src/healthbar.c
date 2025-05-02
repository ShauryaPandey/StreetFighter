#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <gl/GLU.h>
#include "SOIL.h"
#include "Object.h"
#include "shape.h"
#include <assert.h>

//we would need to modify our implementation a little if the max health was anything other than 100
static const int _maxHealth = 100;  

/// <summary>
/// Health bar is not actually in charge of maintaining the health of the characters, 
/// It is only responsible for representing the health as a bar on the screen.
/// Health bar is responsible for showing a health out of 100 in percentage or ratio. 
/// So in our case we use 2 health bars each is updated by the characters code.
/// </summary>
typedef struct healthBar_t
{
    Coord2D pos; //screen position
    int health; //since max is hundred every health value can be easily represented as percentage of the whole
}HealthBar;

HealthBar* initHealthbar(float x, float y)
{
    HealthBar* bar = malloc(sizeof(HealthBar));
    assert(bar != NULL);
    bar->health = _maxHealth; //set to 100
    Coord2D pos = { x,y };
    bar->pos = pos;
    return bar;
}

void DeInitHealthBar(HealthBar* bar)
{
    assert(bar != NULL);
    free(bar);
    bar = NULL;
}

/// <summary>
/// Subtracts the given value and update health.
/// </summary>
/// <param name="bar"></param>
/// <param name="delta"></param>
void updateHealthBar(HealthBar* bar, int delta) 
{
    bar->health -= delta;
    if (bar->health < 0)
    {
        bar->health = 0;
    }
}

/// <summary>
/// Resetting or setting back to max value. 
/// </summary>
/// <param name="bar"></param>
void resetHealthBar(HealthBar* bar)
{
    bar->health = _maxHealth;
}

/// <summary>
/// Set it to a certain value. Gives us the capability to update the health bar every frame even 
/// if it is not changing at that rate. 
/// </summary>
/// <param name="bar"></param>
/// <param name="val"></param>
void setHealthBar(HealthBar* bar, int val)//set to specifc value
{
    assert(bar != NULL);
    if (val < 0)
    {
        bar->health = 0;
    }
    else if (val > _maxHealth)
    {
        bar->health = _maxHealth;
    }
    else
    {
        bar->health = val;
    }
}

/// <summary>
/// Draw the health bar.
/// </summary>
/// <param name="bar"></param>
void _healthBarDraw(HealthBar* bar) //THe functions first draws partly red rectangle and remaining rectangle as black rectangle
{
    static float width = 200;
    static float height = 30;
    assert(bar->health >= 0 && bar->health <= _maxHealth);
    float x = bar->pos.x;
    float y = bar->pos.y;
    shapeDrawRectangle(x, y, width, height, 0xFF, 0, 0, true); //we are basically drawing 2 rects. red and remaining black.

    float blackX = x + width / 2.0f;
    blackX -= (((_maxHealth - bar->health) / 2.0f) / (float)_maxHealth) * width;
    shapeDrawRectangle(blackX, y, ((_maxHealth - bar->health) / (float)_maxHealth) * width, height, 0, 0, 0, true);
}