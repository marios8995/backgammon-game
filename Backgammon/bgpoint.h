#ifndef bgpointH
#define bgpointH
#pragma once

class BgPoint {
    char colour;
    int number;
public:
    BgPoint();
    void set(char clr, int num);
    char getColour() const;
    int getNumber() const;
    void addPiece(char clr);
    void removePiece();
    bool isEmpty() const;
    bool operator==(const BgPoint& chk) const;
    bool operator!=(const BgPoint& chk) const;
};

#endif
