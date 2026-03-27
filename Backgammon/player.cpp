#pragma hdrstop
#include "player.h"

// ----------------------------------------------- PLAYER (INTERFACE) -----------------------------------------------

Player::Player(const char colour, const std::string &name) {
    this->colour = colour;
    this->name = name;
}

char Player::getColour() const {
    return colour;
}

std::string Player::getName() const {
    return name;
}

// ----------------------------------------------- LOCAL PLAYER -----------------------------------------------

LocalPlayer::LocalPlayer(char colour, const std::string &name) : Player(colour, name) {}

bool LocalPlayer::isLocal() const {
    return true;
}

// ----------------------------------------------- NETWORK PLAYER -----------------------------------------------

NetworkPlayer::NetworkPlayer(char colour, const std::string &name, int socket) : Player(colour, name) {
    socketID = socket;
}

bool NetworkPlayer::isLocal() const {
    return false;
}

int NetworkPlayer::getSocketID() const {
    return socketID;
}

#pragma package(smart_init)
