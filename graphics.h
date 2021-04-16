/** Graphics.h
 * ===========================================================
 * Name: CS220
 * Project: PEX5
 * Purpose: The definition of functions needed to draw graphic
 *          elements on a window.
 * ===========================================================
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>

// An array of graphic pens -- so they can be created only once
#define SOLID_THIN_WHITE 0
#define SOLID_THIN_BLACK 1
#define SOLID_THICK_BLACK 2
#define SOLID_THIN_RED 3
#define SOLID_THICK_RED 4
#define SOLID_THIN_GREEN 5
#define NUMBER_PENS 6  // Update this if you add new pens

// An array of graphic brushes -- so they can be created only once
#define MY_BLACK_BRUSH 0
#define MY_RED_BRUSH 1
#define MY_GREEN_BRUSH 2
#define MY_WHITE_BRUSH 3
#define NUMBER_BRUSHES 4  // Update this if you add new brushes

void createGraphicsWindow(char *windowTitle, WNDPROC messageFunction, int startX, int startY,
                          int width, int height);
/**
 * Cause the window's contents to be redrawn. It actually
 * creates a new "PAINT" message and sends the message to the
 * event queue. When the event loop processes the event. The
 * window will be redrawn.
 */
void redraw();

/**
 * Clear the background of the window by drawing a rectangle over
 * the entire area.
 * @param hwnd handle to the window
 * @param hdc handle to a device context
 * @param whichPen an index into the Pens array
 * @param whichBrush an index into the Brushes array
 */
void clearBackground(HWND hwnd, HDC hdc, int whichPen, int whichBrush);

/**
 * Change the color of an individual pixel.
 * @param hdc handle to a device context
 * @param pixelX the x coordinate of the pixel (from upper-left corner)
 * @param pixelY the y coordinate of the pixel (from upper-left corner)
 * @param color the RGB color to put into the pixel
 */
void drawPixel(HDC hdc, int pixelX, int pixelY, COLORREF color);

/**
 * Draw a line
 * @param hdc handle to a device context
 * @param fromX starting x coordinate
 * @param fromY starting y coordinate
 * @param toX ending x coordinate
 * @param toY ending y coordinate
 * @param whichPen an index into the Pens array
 */
void drawLine(HDC hdc, int fromX, int fromY, int toX, int toY, int whichPen);

/**
 * Draw a circle
 * @param hdc handle to a device context
 * @param centerX center x coordinate
 * @param centerY center y coordinate
 * @param radius radius of the circle
 * @param whichPen an index in to the Pens array
 * @param whichBrush an index in the Brushes array
 */
void drawCircle(HDC hdc, int centerX, int centerY, int radius, int whichPen, int whichBrush);

/**
 * Draw text
 * @param hdc handle to a device context
 * @param x the x coordinate of the upper-left corner of the text
 * @param y the y coordinate of the upper-left corner of the text
 * @param text the text to display
 */
void drawText(HDC hdc, int x, int y, char *text);

#endif  // GRAPHICS_H
