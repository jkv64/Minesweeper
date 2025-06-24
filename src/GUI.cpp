// GUI.cpp
#include "GUI.h"
#include <string>
#include <algorithm>

GUI::GUI(const int dimension): m_dimension(dimension), m_timerText(m_font, "") {
    // Create the window
    // Updated to use dynamic m_cellSize for initial window calculation
    const int windowWidth = dimension * (m_cellSize + CELL_SPACING) - CELL_SPACING;
    const int windowHeight = windowWidth + TIMER_HEIGHT;
    m_window = sf::RenderWindow(sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)), "Minesweeper");
    // Initialize timer text
    m_timerText.setCharacterSize(45);
    m_timerText.setFillColor(sf::Color::White);
    m_timerText.setPosition({0.f, TIMER_OFFSET_Y});

    // Initialize board visual elements
    initializeBoard();
}

GUI::~GUI() = default;

void GUI::initializeBoard() {
    // Initialize cell shapes and texts
    m_cellShapes.resize(m_dimension, std::vector<sf::RectangleShape>(m_dimension));
    m_cellTexts.resize(m_dimension, std::vector<sf::Text>()); //no default Text constructor

    for (int i = 0; i < m_dimension; i++) {
        for (int j = 0; j < m_dimension; j++) {
            // Create cell shape
            m_cellShapes[i][j].setSize(sf::Vector2f(m_cellSize, m_cellSize));
            // Unrevealed: Light blue for a more engaging default look
            m_cellShapes[i][j].setFillColor(sf::Color(100, 100, 255));  // Light blue
            float x = (m_cellSize + CELL_SPACING) * i;
            float y = (m_cellSize + CELL_SPACING) * j + TIMER_HEIGHT;
            m_cellShapes[i][j].setPosition({x, y});

            // Create cell text
            auto temp_text = sf::Text(m_font, "");
            temp_text.setCharacterSize(m_textSize);
            temp_text.setPosition({x + TEXT_OFFSET_RATIO_X * m_cellSize, y + TEXT_OFFSET_RATIO_Y * m_cellSize});
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
    // Define color palette for numbers (1: Blue, 2: Green, 3: Red, etc.) - expandable for higher numbers
    // Important: Ensures distinct, compatible colors with good contrast against backgrounds
    std::vector<sf::Color> numberColors = {
        sf::Color(0, 0, 255),    // 1: Blue
        sf::Color(0, 128, 0),    // 2: Green
        sf::Color(255, 0, 0),    // 3: Red
        sf::Color(128, 0, 128),  // 4: Purple
        sf::Color(195, 130, 45),  // 5: Orange-Brown
        sf::Color(0, 255, 255),  // 6: Cyan
        sf::Color(255, 255, 0),  // 7: Yellow
        sf::Color(255, 0, 255)   // 8: Magenta
    };

    for (int i = 0; i < board.getDimension(); i++) {
        for (int j = 0; j < board.getDimension(); j++) {
            if (board.isRevealed(i, j)) {
                if (board.isMine(i, j)) {
                    // Revealed mine: Use red for danger/alert
                    m_cellShapes[i][j].setFillColor(sf::Color(255, 80, 80));  // Red
                } else if (board.getNumberAt(i, j) == 0) {
                    // Revealed empty: Light blue-gray for a clean, open feel
                    m_cellShapes[i][j].setFillColor(sf::Color(230, 230, 255));  // Light blue-gray
                } else {
                    // Revealed number: Soft gray-blue background for consistency
                    m_cellShapes[i][j].setFillColor(sf::Color(210, 210, 255));  // Soft gray-blue
                    m_cellTexts[i][j].setString(std::to_string(board.getNumberAt(i, j)));
                    // Set text color based on number value (falls back to black if out of range)
                    int num = board.getNumberAt(i, j);
                    sf::Color textColor = (num > 0 && num <= numberColors.size()) ? numberColors[num - 1] : sf::Color::Black;
                    m_cellTexts[i][j].setFillColor(textColor);
                }
            } else if (board.isFlagged(i, j)) {
                // Flagged: Yellow tint for warning, with "F" for flag (changed from "M" for clarity)
                m_cellShapes[i][j].setFillColor(sf::Color(255, 255, 100));  // Light yellow
                m_cellTexts[i][j].setString("F");
                m_cellTexts[i][j].setFillColor(sf::Color::Black);
            } else {
                // Unrevealed: Light blue for a more engaging default look
                m_cellShapes[i][j].setFillColor(sf::Color(100, 100, 255));  // Light blue
                m_cellTexts[i][j].setString("");
                m_cellTexts[i][j].setFillColor(sf::Color::Transparent);  // Hide text
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

    // Dynamically center the timer based on current window width and text size
    // Important: Handles resizing and varying digit counts without fixed offsets
    float textWidth = m_timerText.getLocalBounds().size.x;
    float windowWidth = static_cast<float>(m_window.getSize().x);
    float centerX = (windowWidth - textWidth) / 2.0f;
    m_timerText.setPosition({centerX, 1.f});  // Y-position remains near top

    m_window.draw(m_timerText);
}

// New method: Handles window resizing by updating cell sizes and positions
void GUI::handleResize(unsigned int width, unsigned int height) {
    // Enforce minimum size to prevent unusable tiny cells (based on grid dimension)
    const unsigned int minWidth = m_dimension * (MIN_CELL_SIZE + CELL_SPACING) - CELL_SPACING;
    const unsigned int minHeight = minWidth + TIMER_HEIGHT;
    width = std::max(width, minWidth);
    height = std::max(height, minHeight);
    //Enforce a square game space
    width = std::min(width, height - TIMER_HEIGHT);
    height = width + TIMER_HEIGHT;
    // Update window with new values
    m_window.setSize({width, height});

    // Recalculate cell size to fit the new window dimensions dynamically
    // Important: Ensures the fixed-dimension grid scales while maintaining aspect ratio
    const float availableWidth = static_cast<float>(width);
    const float availableHeight = static_cast<float>(height) - TIMER_HEIGHT;
    const float oldSize = m_cellSize;
    m_cellSize = std::min(availableWidth / m_dimension, availableHeight / m_dimension) - CELL_SPACING;
    m_cellSize = std::max(m_cellSize, MIN_CELL_SIZE);  // Prevent too-small cells

    m_textSize *= m_cellSize / oldSize;

    // Update positions and sizes of all visual elements to match new cell size
    for (int i = 0; i < m_dimension; i++) {
        for (int j = 0; j < m_dimension; j++) {
            m_cellShapes[i][j].setSize(sf::Vector2f(m_cellSize, m_cellSize));
            float x = (m_cellSize + CELL_SPACING) * i;
            float y = (m_cellSize + CELL_SPACING) * j + TIMER_HEIGHT;
            m_cellShapes[i][j].setPosition({x, y});
            m_cellTexts[i][j].setCharacterSize(m_textSize);
            m_cellTexts[i][j].setPosition({x + TEXT_OFFSET_RATIO_X * m_cellSize, y + TEXT_OFFSET_RATIO_Y * m_cellSize});
        }
    }

    // Important: Update the SFML view to match the new size for proper rendering
    sf::View view = m_window.getDefaultView();
    view.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
    view.setCenter(sf::Vector2f(static_cast<float>(width) / 2.f, static_cast<float>(height) / 2.f));
    m_window.setView(view);
}

// New getter: Returns current dynamic cell size for use in event handling
float GUI::getCellSize() const {
    return m_cellSize;
}

// New getter: Returns cell size including spacing for accurate click coordinate calculation
float GUI::getCellSizeWithSpacing() const {
    return m_cellSize + CELL_SPACING;
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