// Game.cpp
#include "Game.h"
#include <SFML/Graphics.hpp>

MinesweeperGame::MinesweeperGame()
    : m_board(DIMENSION, NUM_MINES),
      m_gui(DIMENSION),
      m_firstClick(true),
      m_gameOver(false),
      m_isRunning(true)
    {
}

MinesweeperGame::~MinesweeperGame() = default;

void MinesweeperGame::run() {
    while (m_isRunning && m_gui.isWindowOpen()) {
        handleEvents();
        update();
        render();
    }
}

void MinesweeperGame::handleEvents() {
    while (const std::optional event = m_gui.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_gui.closeWindow();
            m_isRunning = false;
        }
        else if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
            // Get board coordinates from mouse position
            const int i = mouseButton->position.x / (CELL_SIZE + CELL_SPACING);
            const int j = (mouseButton->position.y - TIMER_HEIGHT) / (CELL_SIZE + CELL_SPACING);

            // Check if click is within board bounds
            if (!m_gameOver && i >= 0 && i < DIMENSION && j >= 0 && j < DIMENSION) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    handleLeftClick(i, j);
                }
                else if (mouseButton->button == sf::Mouse::Button::Right) {
                    handleRightClick(i, j);
                }
            }

        }
    }
}

void MinesweeperGame::handleLeftClick(const int x, const int y) {
    if (m_firstClick) {
        // Initialize board with first click being safe
        do {
            m_board.assignMines();
            m_board.assignNumbers();
        } while (m_board.getNumberAt(x, y) != 0);

        m_firstClick = false;
        m_timer.start();
    }

    if (m_board.isMine(x, y)) {
        // Game over - reveal all mines
        m_board.revealAllMines();
        m_gameOver = true;
        m_gui.updateBoardDisplay(m_board);
    }
    else {
        // Reveal the clicked cell and potentially its neighbors
        m_board.reveal(x, y);
        checkWin();
        m_gui.updateBoardDisplay(m_board);
    }
}

void MinesweeperGame::checkWin() {
    if (m_board.getNumRevealed() == (DIMENSION * DIMENSION - NUM_MINES)) {
        m_gameOver = true;
    }
}

void MinesweeperGame::handleRightClick(int i, int j) {
    m_board.toggleFlag(i, j);
    m_gui.updateBoardDisplay(m_board);
}

void MinesweeperGame::update() {
    if (!m_firstClick && !m_gameOver) {
        m_timer.update();
    }
}

void MinesweeperGame::render() {
    m_gui.clear();
    m_gui.drawBoard(m_board);
    m_gui.drawTimer(m_timer.getElapsedSeconds());
    m_gui.display();
}