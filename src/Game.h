// Game.h
#pragma once

#include "Board.h"
#include "GUI.h"
#include "Timer.h"

class MinesweeperGame {
public:
    MinesweeperGame();
    ~MinesweeperGame();

    void run();

private:
    void handleEvents();
    void update();
    void render();
    void handleLeftClick(int x, int y);
    void handleRightClick(int x, int y);
    void checkWin();

    Board m_board;
    GUI m_gui;
    Timer m_timer;

    bool m_firstClick;
    bool m_gameOver;
    bool m_isRunning;
};