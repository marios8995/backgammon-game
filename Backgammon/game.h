#ifndef gameH
#define gameH
#pragma once
#include "dice.h"
#include "player.h"
#include "board.h"
#include <vector>

class Game {
	Board board;
	Player* player1;
	Player* player2;
	char currTurn;
	int turnCounter;
	DiceInterface* dice1;
	DiceInterface* dice2;
	std::vector<int> moves;
	int p1Wins;
	int p2Wins;
	bool gameEnded;
public:
	Game();
	Game(bool debugMode);
	~Game();
	void start();
	void setFirstPlayerTurn();
	void nextTurn();
	void rollDice();
	void useDice(DiceInterface* dice);
	bool hasUnusedDice();
	void isGameOver();
	char getCurrentPlayer() const;
	void stop();
	void calculateMoves();
	bool attemptMove(int from, int to);
	const Board& getBoard() const;
	void setPlayerName(int p, const std::string &name);
	DiceInterface* getDice(int dice) const;
	Player* getPlayer(int player) const;
	int getP1Wins() const;
	int getP2Wins() const;
	int getTurns() const;
	std::vector<int> getMoves() const;
	bool hasValidMoves();
	void onlineRoll(int v1, int v2);
	bool hasGameEnded() const;
	bool isOnl;
	void setTurn(char val);
private:
	bool tryPlaceFromBar(int to);
	bool tryBearOff(int from);
	bool tryMove(int from, int to);
	int canUseDieForMove(int dist) const;
	bool useDieForMove(int dist, int moveIndex);
};

#endif
