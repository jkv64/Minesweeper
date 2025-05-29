// Board.h
#pragma once

#include <vector>
#include <string>

// Forward declaration
class GUI;

// Cell represents a single square on the board
struct Cell {
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    int adjacentMines = 0;
};

class Board {
public:
    Board(int dimension, int numMines);

    void assignMines();
    void assignNumbers();
    void reveal(int x, int y);
    void toggleFlag(int x, int y);
    void revealAllMines();

    bool isMine(int x, int y) const;
    bool isRevealed(int x, int y) const;
    bool isFlagged(int x, int y) const;
    int getNumberAt(int x, int y) const;
    int getDimension() const;
    int getNumRevealed() const;

    // Friend declaration to allow GUI to access board data
    friend class GUI;

private:
    std::vector<std::vector<Cell>> m_cells;
    std::vector<std::pair<int, int>> m_minePositions;
    int m_dimension;
    int m_numMines;
    int m_numRevealed;
};