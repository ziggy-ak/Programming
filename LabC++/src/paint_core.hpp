#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Canvas dimensions (in logic pixels, before scaling)
const int WIDTH  = 800;
const int HEIGHT = 600;

// Scaling factor for display (each logical pixel becomes a small square)
const int PIXEL_SIZE = 1;

// Canvas type: 2D vector of SFML colors
using Canvas = std::vector<std::vector<sf::Color>>;

// ========== Task 3.1 – 3.3 (Basic drawing) ==========
void clearCanvas(Canvas& canvas);
void drawPixel(Canvas& canvas, int x, int y, const sf::Color& color);
void drawBrush(Canvas& canvas, int x, int y, const sf::Color& color, int radius);

// ========== Task 4.1 – 4.3 (Shapes & Undo) ==========
void drawRectangle(Canvas& canvas, int x1, int y1, int x2, int y2, const sf::Color& color);
void drawLine(Canvas& canvas, int x1, int y1, int x2, int y2, const sf::Color& color);
void undo(Canvas& canvas);          // Ctrl+Z
void saveToUndo(const Canvas& canvas);   // called automatically by shape functions

// ========== Task 5.1 – 5.3 (Filters with STL algorithms) ==========
void applyGrayscale(Canvas& canvas);
void applyNegative(Canvas& canvas);
void applyBlur(Canvas& canvas);

// ========== Helper for display ==========
void renderToWindow(sf::RenderWindow& window, const Canvas& canvas);