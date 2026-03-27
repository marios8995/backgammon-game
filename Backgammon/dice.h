#ifndef diceH
#define diceH
#pragma once

class DiceInterface {
protected:
    int value;
    bool used;
public:
    DiceInterface();
    virtual void roll() = 0;
    int getValue() const;
    virtual ~DiceInterface() {};
    bool isUsed() const;
    void use();
    void reset();
};

class StandardDice : public DiceInterface {
public:
    StandardDice();
    void roll() override;
};

class OnlineDice : public DiceInterface {
public:
	OnlineDice();
    OnlineDice(int value);
    void roll() override;
    void roll(int minVal = 1, int maxVal = 6);
    void setFixedValue(int value);
};

#endif
