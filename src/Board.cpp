// Board.cpp
#include "Board.h"
#include <random>
#include <algorithm>

Board::Board(const int dimension, const int numMines)
    : m_dimension(dimension),
      m_numMines(numMines) {

    // Initialize the board with empty cells
    m_cells.resize(dimension, std::vector<Cell>(dimension));
    m_minePositions.reserve(numMines);
}

void Board::assignMines() {
    // Clear any existing mines
    m_minePositions.clear();
    for (auto& row : m_cells) {
        for (auto& cell : row) {
            cell.isMine = false;
            cell.adjacentMines = 0;
        }
    }

    // Use modern C++ random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, m_dimension - 1);

    // Place mines at random positions
    while (m_minePositions.size() < m_numMines) {
        int x = dist(gen);
        int y = dist(gen);

        // Check if this position already has a mine
        bool alreadyMine = false;
        for (const auto& pos : m_minePositions) {
            if (pos.first == x && pos.second == y) {
                alreadyMine = true;
                break;
            }
        }

        if (!alreadyMine) {
            m_minePositions.push_back({x, y});
            m_cells[x][y].isMine = true;
        }
    }
}

void Board::assignNumbers() {
    // Calculate adjacent mines for each cell
    for (int i = 0; i < m_dimension; i++) {
        for (int j = 0; j < m_dimension; j++) {
            int count = 0;

            // Check all 8 surrounding cells
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    const int ni = i + di;
                    const int nj = j + dj;

                    // Skip if out of bounds
                    if (ni < 0 || ni >= m_dimension || nj < 0 || nj >= m_dimension) {
                        continue;
                    }

                    if (m_cells[ni][nj].isMine) {
                        count++;
                    }
                }
            }

            m_cells[i][j].adjacentMines = count;
        }
    }
}

void Board::reveal(const int x, const int y) {
    // Don't reveal flagged cells or already revealed cells
    if (x < 0 || x >= m_dimension || y < 0 || y >= m_dimension ||
        m_cells[x][y].isRevealed || m_cells[x][y].isFlagged) {
        return;
    }

    // Reveal the cell
    m_cells[x][y].isRevealed = true;

    // If it's a cell with no adjacent mines, reveal all neighbors (flood fill)
    if (m_cells[x][y].adjacentMines == 0 && !m_cells[x][y].isMine) {
        for (int di = -1; di <= 1; di++) {
            for (int dj = -1; dj <= 1; dj++) {
                reveal(x + di, y + dj);
            }
        }
    }
}

void Board::toggleFlag(const int x, const int y) {
    // Can only flag unrevealed cells
    if (!m_cells[x][y].isRevealed) {
        m_cells[x][y].isFlagged = !m_cells[x][y].isFlagged;
    }
}

void Board::revealAllMines() {
    for (const auto& pos : m_minePositions) {
        m_cells[pos.first][pos.second].isRevealed = true;
    }
}

bool Board::isMine(const int x, const int y) const {
    if (x >= 0 && x < m_dimension && y >= 0 && y < m_dimension) {
        return m_cells[x][y].isMine;
    }
    return false;
}

bool Board::isRevealed(const int x, const int y) const {
    if (x >= 0 && x < m_dimension && y >= 0 && y < m_dimension) {
        return m_cells[x][y].isRevealed;
    }
    return false;
}

bool Board::isFlagged(const int x, const int y) const {
    if (x >= 0 && x < m_dimension && y >= 0 && y < m_dimension) {
        return m_cells[x][y].isFlagged;
    }
    return false;
}

int Board::getNumberAt(const int x, const int y) const {
    if (x >= 0 && x < m_dimension && y >= 0 && y < m_dimension) {
        return m_cells[x][y].adjacentMines;
    }
    return 0;
}

int Board::getDimension() const {
    return m_dimension;
}