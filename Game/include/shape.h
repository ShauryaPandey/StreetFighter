#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void shapeDrawCircle(float radius, float x, float y, uint8_t r, uint8_t g, uint8_t b, bool filled);
void shapeDrawLine(float startX, float startY, float endX, float endY, uint8_t r, uint8_t g, uint8_t b);
//void shapeDrawRectangle(Coord2D topLeft, Coord2D bottomRight, uint8_t r, uint8_t g, uint8_t b, bool filled);
void shapeDrawRectangle(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, bool filled);

#ifdef __cplusplus
}
#endif