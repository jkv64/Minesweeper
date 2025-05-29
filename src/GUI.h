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
constexpr float TEXT_OFFSET_Y = 8.0f;
constexpr float TEXT_OFFSET_X = 16.0f;
constexpr float TIMER_OFFSET_Y = 1.f;
constexpr int TIMER_OFFSET_X_ONE_DIGIT = 45;
constexpr int TIMER_OFFSET_X_TWO_DIGIT = 67;
constexpr int TIMER_OFFSET_X_THREE_DIGIT = 90;
constexpr int TIMER_OFFSET_X_FOUR_DIGIT = 112;

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

    void clear();
    void display();

    sf::RenderWindow& getWindow();

private:
    void initializeBoard(int dimension);

    sf::RenderWindow m_window;
    sf::Font m_font = sf::Font("../../src/courier-normal.ttf");

    // Board visual elements
    std::vector<std::vector<sf::RectangleShape>> m_cellShapes;
    std::vector<std::vector<sf::Text>> m_cellTexts;

    // Timer display
    sf::Text m_timerText;
};