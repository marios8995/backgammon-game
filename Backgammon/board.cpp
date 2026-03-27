#pragma hdrstop
#include "board.h"

Board::Board() {
    setupBoard();
}

void Board::setupBoard() {
	bar.fill(BgPoint());
	home.fill(BgPoint());
	points.fill(BgPoint());

	points.at(0).set('W',2);
	points.at(5).set('B',5);

	points.at(7).set('B',3);
	points.at(11).set('W',5);

	points.at(12).set('B',5);
	points.at(16).set('W',3);

	points.at(18).set('W',5);
	points.at(23).set('B',2);

	home.at(0).set('B', 0);
	home.at(1).set('W', 0);

	bar.at(0).set('B', 0);
	bar.at(1).set('W', 0);
}

void Board::moveChecker(const int from, const int to) {
	if (!canMove(from, to)) return;

	BgPoint& src = points.at(from);
	BgPoint& dest = points.at(to);

	if (src != dest && !dest.isEmpty() && dest.getNumber() == 1) {
		toBar(dest.getColour());
		dest.set(src.getColour(), 1);
	}
	else {
		dest.addPiece(src.getColour());
    }
	src.removePiece();
}

bool Board::canMove(const int from, const int to) const {
    if (from < 0 || from >= 24 || to < 0 || to >= 24) return false;

	const char clr = points.at(from).getColour();
	if (clr == 'N') return false;
    if (pieceInBar(clr)) return false;

	const BgPoint& dest = points.at(to);
	return clr == dest.getColour() ||
		   dest.isEmpty() ||
		   dest.getNumber() == 1;
}

bool Board::checkWin(const char clr) const {
	return home.at(getIndex(clr)).getNumber() == 15;
}

void Board::toBar(const char clr) {
	int i = getIndex(clr);
	if (bar.at(i).isEmpty()) {
		bar.at(i).set(clr, 1);
    }
	else {
		bar.at(i).addPiece(clr);
	}
}

void Board::placeChecker(const char clr, const int to) {
	if (!canPlace(clr, to)) return;

	const int barIndex = getIndex(clr);
	BgPoint& src = bar.at(barIndex);
	BgPoint& dest = points.at(to);

	if (src != dest && !dest.isEmpty() && dest.getNumber() == 1) {
		toBar(dest.getColour());
		dest.set(clr, 1);
	}
	else {
		dest.addPiece(clr);
    }
	src.removePiece();
}

bool Board::canPlace(const char clr, const int to) const {
	const BgPoint& dest = points.at(to);
	int barIndex = getIndex(clr);
	return (clr == dest.getColour() ||
		   dest.isEmpty() ||
		   dest.getNumber() == 1) &&
		   bar.at(barIndex).getNumber() > 0;
}

void Board::bearOff(const char clr, int diceVal) {
    int homeIndex = getIndex(clr);
	int from = (homeIndex == 1) ? (24 - diceVal) : (diceVal - 1);
    if (canBearOff(clr, diceVal)) {
        home.at(homeIndex).addPiece(clr);
        points.at(from).removePiece();
    }
}

bool Board::canBearOff(const char clr, int diceVal) const {
    int homeIndex = getIndex(clr);

    if (!allCheckersInHome(clr)) return false;

	int targetIndex = (homeIndex == 1) ? (24 - diceVal) : (diceVal - 1);
	if (points.at(targetIndex).getColour() == clr) return true;

	if (homeIndex == 1) {

		for (int i = 18; i < targetIndex; ++i) {
            if (points.at(i).getColour() == clr) return false;
        }
        for (int i = targetIndex + 1; i < 24; ++i) {
            if (points.at(i).getColour() == clr) return true;
        }
    }
    else {
        for (int i = targetIndex + 1; i < 6; ++i) {
            if (points.at(i).getColour() == clr) return false;
        }
        for (int i = targetIndex - 1; i >= 0; --i) {
            if (points.at(i).getColour() == clr) return true;
        }
    }

    return false;
}

bool Board::allCheckersInHome(const char clr) const {
    int homeIndex = getIndex(clr);
    int indexCap = (homeIndex == 1) ? 18 : 6;
    int notInHome = 0;

    if (homeIndex == 1) {
        for (int i = 0; i < indexCap; ++i) {
            if (points.at(i).getColour() == clr) {
                notInHome += points.at(i).getNumber();
            }
        }
    }
    else {
        for (int i = indexCap; i < 24; ++i) {
            if (points.at(i).getColour() == clr) {
                notInHome += points.at(i).getNumber();
            }
        }
    }

    return notInHome == 0;
}

bool Board::pieceInBar(const char clr) const {
    return !bar.at(getIndex(clr)).isEmpty();
}

int Board::getIndex(const char clr) {
    return (clr == 'W') ? 1 : 0;
}

BgPoint Board::getPoint(const int index) const {
    return points.at(index);
}

BgPoint Board::getHomePoint(const int index) const {
	return home.at(index);
}

BgPoint Board::getBarPoint(const int index) const {
	return bar.at(index);
}


#pragma package(smart_init)
