#pragma hdrstop
#include "dice.h"
#include <random>
#include <chrono>

// ----------------------------------------------- DICE INTERFACE -----------------------------------------------

DiceInterface::DiceInterface() {
    value = 0;
    used = false;
}

int DiceInterface::getValue() const {
    return value;
}

bool DiceInterface::isUsed() const {
    return used;
}

void DiceInterface::use() {
    used = true;
}

void DiceInterface::reset() {
    used = false;
}

// ----------------------------------------------- STANDARD DICE -----------------------------------------------

StandardDice::StandardDice() {}

void StandardDice::roll() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	static std::mt19937 gen(seed);
	static std::uniform_int_distribution<int> distrib(1, 6);

	value = distrib(gen);
    used = false;
}

// ----------------------------------------------- ONLINE DICE -----------------------------------------------

OnlineDice::OnlineDice() {}

OnlineDice::OnlineDice(const int value) {
    this->value = value;
    used = false;
}

void OnlineDice::roll() {
    if (value == 0) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		static std::mt19937 gen(seed);
		static std::uniform_int_distribution<int> distrib(1, 6);

        value = distrib(gen);
    }
    used = false;
}

void OnlineDice::roll(int minVal, int maxVal) {
    if (value == 0) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		static std::mt19937 gen(seed);
        static std::uniform_int_distribution<int> distrib(1, 6);

        value = distrib(gen);
    }
    used = false;
}

void OnlineDice::setFixedValue(const int value) {
    this->value = value;
    used = false;
}

#pragma package(smart_init)
