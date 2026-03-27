#pragma hdrstop
#include "game.h"

Game::Game() {
	gameEnded = false;
	p1Wins = 0;
	p2Wins = 0;
	player1 = new LocalPlayer('W', "Player1");
    player2 = new NetworkPlayer('B', "Player2", 3000);
    dice1 = new OnlineDice();
	dice2 = new OnlineDice();
	isOnl = false;
    start();
}

Game::Game(bool debugMode) {
	gameEnded = false;
	p1Wins = 0;
	p2Wins = 0;
    player1 = new LocalPlayer('W', "Player1");
	player2 = new NetworkPlayer('B', "Player2", 3000);
	if (debugMode) {
		dice1 = new OnlineDice();
		dice2 = new OnlineDice();
	}
	else {
		dice1 = new StandardDice();
		dice2 = new StandardDice();
	}
    start();
}

Game::~Game() {
    delete player1;
    delete player2;
    delete dice1;
    delete dice2;
}

void Game::start() {
	gameEnded = false;
	OnlineDice* d1 = (OnlineDice*)dice1;
	OnlineDice* d2 = (OnlineDice*)dice2;
	if (d1) d1->setFixedValue(0);
	if (d2) d2->setFixedValue(0);
	board.setupBoard();
	turnCounter = 1;
	setFirstPlayerTurn();
}

void Game::setFirstPlayerTurn() {
	((OnlineDice*)dice1)->setFixedValue(0);
	((OnlineDice*)dice2)->setFixedValue(0);

	int dVal1, dVal2;
    int safetyCounter = 0;
    do {
        dice1->roll();
        dice2->roll();
        dVal1 = dice1->getValue();
		dVal2 = dice2->getValue();

        safetyCounter++;
		if (safetyCounter > 50) {
			dVal1 = 2;
			dVal2 = 4;
		}
    } while (dVal1 == dVal2);

    if (dVal1 > dVal2) {
        currTurn = 'W';
    }
    else {
        currTurn = 'B';
    }
    calculateMoves();
}

void Game::nextTurn() {
    ++turnCounter;
    if (currTurn == 'W') {
        currTurn = 'B';
    }
    else {
        currTurn = 'W';
    }
}

void Game::rollDice() {
    dice1->roll();
    dice2->roll();
    calculateMoves();
}

void Game::useDice(DiceInterface* dice) {
    dice->use();
}

bool Game::hasUnusedDice() {
    return !(dice1->isUsed() && dice2->isUsed());
}

void Game::isGameOver() {
	if (board.checkWin('W')) {
		gameEnded = true;
		++p1Wins;
		if(board.getHomePoint(0).isEmpty()) {
			++p1Wins;
		}
		stop();
	}
	if (board.checkWin('B')) {
		gameEnded = true;
		++p2Wins;
		if(board.getHomePoint(1).isEmpty()) {
			++p2Wins;
		}
        stop();
    }
}

void Game::calculateMoves() {
    moves.clear();
    int dVal1, dVal2;
    dVal1 = dice1->getValue();
    dVal2 = dice2->getValue();

    if (dVal1 == dVal2) {
        for (int i = 0; i < 4; ++i) {
            moves.push_back(dVal1);
        }
    }
    else {
        moves.push_back(dVal1);
        moves.push_back(dVal2);
    }
    dice1->reset();
    dice2->reset();
}

char Game::getCurrentPlayer() const {
    return currTurn;
}

bool Game::attemptMove(int from, int to) {
	if (from == -1) {
        return tryPlaceFromBar(to);
    }

	if (to == 24) {
        return tryBearOff(from);
    }

    return tryMove(from, to);
}

const Board &Game::getBoard() const {
    return board;
}


bool Game::tryMove(const int from, const int to) {
    int distance = 0;
    if (currTurn == 'W') {
		distance = to - from;
    }
    else {
        distance = from - to;
	}

	if (distance <= 0) return false;

	int moveIndex = canUseDieForMove(distance);
	if (moveIndex == -1) return false;

	if (board.canMove(from, to)) {
		board.moveChecker(from, to);

		useDieForMove(distance, moveIndex);

		isGameOver();
		return true;
	}
	return false;
}

bool Game::tryPlaceFromBar(int to) {
    char clr = getCurrentPlayer();
    int distance = (clr == 'W') ? (to + 1) : (24 - to);
    int moveIndex = canUseDieForMove(distance);
    if (moveIndex == -1) return false;

    if (board.canPlace(clr, to)) {
		board.placeChecker(clr, to);
        useDieForMove(distance, moveIndex);
        isGameOver();
        return true;
    }
    return false;
}

bool Game::tryBearOff(int from) {
    char clr = getCurrentPlayer();
    if (!board.allCheckersInHome(clr)) return false;

    int neededDie = (clr == 'W') ? (24 - from) : (from + 1);

    int chosenDie = -1;
    int moveIndex = -1;

    for (int i = 0; i < moves.size(); ++i) {
        int die = moves.at(i);
        if (die == neededDie) {
            chosenDie = die;
            moveIndex = i;
            break;
		}
		if (die > neededDie) {
			bool isFurthest = true;
			if (clr == 'W') {
				for (int k = 18; k < from; ++k) {
					if (board.getPoint(k).getColour() == clr) {
						isFurthest = false;
						break;
					}
				}
			}
			else {
				for (int k = 5; k > from; --k) {
                    if (board.getPoint(k).getColour() == clr) {
						isFurthest = false;
						break;
					}
				}
			}
			if (isFurthest) {
                chosenDie = die;
                moveIndex = i;
                break;
            }
        }
    }

    if (chosenDie != -1) {
		board.bearOff(clr, neededDie);
		useDieForMove(chosenDie, moveIndex);
		isGameOver();
        return true;
    }
    return false;
}

bool Game::useDieForMove(const int dist, const int moveIndex) {
    moves.erase(moves.begin() + moveIndex);

    if (dice1->getValue() == dist && !dice1->isUsed()) {
        dice1->use();
    }
    else if (dice2->getValue() == dist && !dice2->isUsed()) {
        dice2->use();
    }

    return true;
}

int Game::canUseDieForMove(const int dist) const {
    for (int i = 0; i < moves.size(); ++i) {
        if (moves.at(i) == dist) {
            return i;
        }
    }
    return -1;
}

void Game::stop() {
	moves.clear();
}

bool Game::hasValidMoves() {
    char clr = getCurrentPlayer();

    if (board.pieceInBar(clr)) {
        if (clr == 'W') {
            for (int i = 0; i < moves.size(); ++i) {
				if (board.canPlace(clr, moves.at(i) - 1)) {
                    return true;
                }
            }
        }
        else {
            for (int i = 0; i < moves.size(); ++i) {
				if (board.canPlace(clr, 24 - moves.at(i))) {
                    return true;
                }
			}
		}
		return false;
    }

	if (board.allCheckersInHome(clr)) {
        for (int i = 0; i < moves.size(); ++i) {
            if (board.canBearOff(clr, moves.at(i))) {
                return true;
            }
        }
    }

	if (clr == 'W') {
		for (int i = 23; i >= 0; --i) {
			BgPoint p = board.getPoint(i);
			if (p.getColour() == clr) {
				for (int j = 0; j < moves.size(); ++j) {
					if (board.canMove(i, i + moves.at(j))) {
						return true;
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < 24; ++i) {
			BgPoint p = board.getPoint(i);
			if (p.getColour() == clr) {
				for (int j = 0; j < moves.size(); ++j) {
					if (board.canMove(i, i - moves.at(j))) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

void Game::setPlayerName(int p, const std::string &name) {
	if (p == 0) {
		char c = player1->getColour();
		delete player1;
		player1 = new LocalPlayer(c, name);
	}
	else {
		char c = player2->getColour();
		delete player2;
		player2 = new LocalPlayer(c, name);
    }
}

DiceInterface* Game::getDice(const int dice) const {
    if(dice == 0) {
        return dice1;
    }
    return dice2;
}

Player* Game::getPlayer(const int player) const {
    if (player == 0) {
        return player1;
    }
    return player2;
}

int Game::getP1Wins() const {
    return p1Wins;
}

int Game::getP2Wins() const {
    return p2Wins;
}

int Game::getTurns() const {
    return turnCounter;
}

std::vector<int> Game::getMoves() const {
    return moves;
}

void Game::onlineRoll(int v1, int v2) {
	OnlineDice* dd1 = dynamic_cast<OnlineDice*>(dice1);
	OnlineDice* dd2 = dynamic_cast<OnlineDice*>(dice2);
	if (dd1 && dd2) {
		dd1->setFixedValue(v1);
		dd2->setFixedValue(v2);
		rollDice();
    }
}

bool Game::hasGameEnded() const {
	return gameEnded;
}

void Game::setTurn(char val) {
    currTurn = val;
}

#pragma package(smart_init)
