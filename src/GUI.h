// GUI.h
#pragma once

#include <SFML/Graphics.hpp>
#include "Board.h"

// Constants for the GUI
constexpr int DIMENSION = 20;
constexpr int NUM_MINES = ((DIMENSION * DIMENSION) / 4) - 1;
constexpr int CELL_SIZE = 40;
constexpr int CELL_SPACING = 1;
constexpr int TIMER_HEIGHT = 55;
constexpr float TEXT_OFFSET_RATIO_Y = 0.2f;
constexpr float TEXT_OFFSET_RATIO_X = 0.4f;
constexpr float TIMER_OFFSET_Y = 1.f;

class GUI {
public:
    GUI(int dimension);
    ~GUI();

    bool isWindowOpen() const;
    void closeWindow();
    std::optional<sf::Event> pollEvent();

    void updateBoardDisplay(const Board& board);
    void drawBoard(const Board& board);
    void drawTimer(int seconds);

    // New methods for resize handling and dynamic cell info
    void handleResize(unsigned int width, unsigned int height);  // Updates visual elements on window resize
    float getCellSize() const;  // Getter for current dynamic cell size
    float getCellSizeWithSpacing() const;  // Getter for cell size including spacing, for click handling

    void clear();
    void display();

    sf::RenderWindow& getWindow();

private:
    void initializeBoard();

    sf::RenderWindow m_window;
    sf::Font m_font = sf::Font("../../src/courier-normal.ttf");

    // Board visual elements
    std::vector<std::vector<sf::RectangleShape>> m_cellShapes;
    std::vector<std::vector<sf::Text>> m_cellTexts;

    // New members for dynamic resizing
    int m_dimension;
    float m_cellSize = CELL_SIZE;  // Dynamic cell size, updated on resize (initially set to constant)
    float m_textSize = 20.0f;
    static constexpr float MIN_CELL_SIZE = 20.0f;  // Minimum cell size to ensure usability (prevents tiny cells)

    // Timer display
    sf::Text m_timerText;
};