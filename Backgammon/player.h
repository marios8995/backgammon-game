#ifndef playerH
#define playerH
#pragma once
#include <string>

class Player {
protected:
    char colour;
    std::string name;

public:
    Player(char colour, const std::string &name);
    virtual ~Player() {};
    char getColour() const;
    std::string getName() const;
    virtual bool isLocal() const = 0;
};

class LocalPlayer : public Player {
public:
    LocalPlayer(char colour, const std::string &name);
    bool isLocal() const override;
};

class NetworkPlayer : public Player {
    int socketID;
public:
    NetworkPlayer(char colour, const std::string &name, int socket);
    bool isLocal() const override;
    int getSocketID() const;
};

#endif
