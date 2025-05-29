// GUI.cpp
#include "GUI.h"
#include <string>

GUI::GUI(const int dimension): m_timerText(m_font, "") {
    // Create the window
    const int windowWidth = dimension * (CELL_SIZE + CELL_SPACING) - CELL_SPACING;
    const int windowHeight = windowWidth + TIMER_HEIGHT;
    m_window = sf::RenderWindow(sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)), "Minesweeper");

    // Initialize timer text
    m_timerText.setCharacterSize(45);
    m_timerText.setFillColor(sf::Color::White);
    m_timerText.setPosition({0.f, TIMER_OFFSET_Y});

    // Initialize board visual elements
    initializeBoard(dimension);
}

GUI::~GUI() = default;

void GUI::initializeBoard(const int dimension) {
    // Initialize cell shapes and texts
    m_cellShapes.resize(dimension, std::vector<sf::RectangleShape>(dimension));
    m_cellTexts.resize(dimension, std::vector<sf::Text>()); //no default Text constructor

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            // Create cell shape
            m_cellShapes[i][j].setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            m_cellShapes[i][j].setFillColor(sf::Color::White);
            float x = (CELL_SIZE + CELL_SPACING) * i;
            float y = (CELL_SIZE + CELL_SPACING) * j + TIMER_HEIGHT;
            m_cellShapes[i][j].setPosition({x, y});

            // Create cell text
            auto temp_text = sf::Text(m_font, "");
            temp_text.setCharacterSize(20);
            temp_text.setPosition({x + TEXT_OFFSET_X, y + TEXT_OFFSET_Y});
            temp_text.setFillColor(sf::Color::White); // Initially hidden

            m_cellTexts[i].push_back(temp_text);
        }
    }
}

bool GUI::isWindowOpen() const {
    return m_window.isOpen();
}

void GUI::closeWindow() {
    m_window.close();
}

std::optional<sf::Event> GUI::pollEvent() {
    return m_window.pollEvent();
}

void GUI::updateBoardDisplay(const Board& board) {
    for (int i = 0; i < board.getDimension(); i++) {
        for (int j = 0; j < board.getDimension(); j++) {
            // Update cell appearance based on board state
            if (board.isRevealed(i, j)) {
                if (board.isMine(i, j)) {
                    // Revealed mine
                    m_cellShapes[i][j].setFillColor(sf::Color::Black);
                } else if (board.getNumberAt(i, j) == 0) {
                    // Empty cell
                    m_cellShapes[i][j].setFillColor(sf::Color(100, 100, 100));
                } else {
                    // Number cell
                    m_cellShapes[i][j].setFillColor(sf::Color(200, 200, 200));
                    m_cellTexts[i][j].setString(std::to_string(board.getNumberAt(i, j)));
                    m_cellTexts[i][j].setFillColor(sf::Color::Black);
                }
            } else if (board.isFlagged(i, j)) {
                // Flagged cell
                m_cellTexts[i][j].setString("M");
                m_cellTexts[i][j].setFillColor(sf::Color::Black);
            } else {
                // Unrevealed cell
                m_cellShapes[i][j].setFillColor(sf::Color::White);
                m_cellTexts[i][j].setString("");
                m_cellTexts[i][j].setFillColor(sf::Color::White);
            }
        }
    }
}

void GUI::drawBoard(const Board& board) {
    for (int i = 0; i < board.getDimension(); i++) {
        for (int j = 0; j < board.getDimension(); j++) {
            m_window.draw(m_cellShapes[i][j]);

            // Only draw visible text
            if (m_cellTexts[i][j].getFillColor() != sf::Color::White) {
                m_window.draw(m_cellTexts[i][j]);
            }
        }
    }
}

void GUI::drawTimer(int seconds) {
    m_timerText.setString(std::to_string(seconds));

    // Adjust timer position based on number of digits
    float windowWidth = m_window.getSize().x;
    if (seconds < 10) {
        m_timerText.setPosition({(windowWidth / 2) - TIMER_OFFSET_X_ONE_DIGIT, 1.f});
    } else if (seconds < 100) {
        m_timerText.setPosition({(windowWidth / 2) - TIMER_OFFSET_X_TWO_DIGIT, 1.f});
    } else if (seconds < 1000) {
        m_timerText.setPosition({(windowWidth / 2) - TIMER_OFFSET_X_THREE_DIGIT, 1.f});
    } else {
        m_timerText.setPosition({(windowWidth / 2) - TIMER_OFFSET_X_FOUR_DIGIT, 1.f});
    }

    m_window.draw(m_timerText);
}

void GUI::clear() {
    m_window.clear();
}

void GUI::display() {
    m_window.display();
}

sf::RenderWindow& GUI::getWindow() {
    return m_window;
}