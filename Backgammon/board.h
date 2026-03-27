#ifndef boardH
#define boardH
#pragma once
#include <array>
#include "bgpoint.h"

class Board {
    std::array<BgPoint, 24> points;
    std::array<BgPoint, 2> bar;
    std::array<BgPoint, 2> home;
public:
    Board();
    void setupBoard();
    void moveChecker(int from, int to);
    bool canMove(int from, int to) const;
    bool checkWin(char clr) const;
    void toBar(char clr);
    void placeChecker(char clr, int to);
    bool canPlace(char clr, int to) const;
    void bearOff(char clr, int from);
    bool pieceInBar(char clr) const;
    bool canBearOff(char clr, int diceVal) const;
    bool allCheckersInHome(char clr) const;
	BgPoint getPoint(int index) const;
	BgPoint getHomePoint(int index) const;
	BgPoint getBarPoint(int index) const;
    static int getIndex(char clr);
};
#endif
