#pragma hdrstop
#include "bgpoint.h"
#include <stdexcept>

BgPoint::BgPoint() {
    colour = 'N';
    number = 0;
}

void BgPoint::set(const char clr, const int num) {
    colour = clr;
    number = num;
}

char BgPoint::getColour() const {
    return colour;
}

int BgPoint::getNumber() const {
    return number;
}

void BgPoint::addPiece(const char clr) {
    if (number == 0) {
        colour = clr;
    }
	else if (colour != clr) return;

    number++;
}

void BgPoint::removePiece() {
    if (number > 0) {
        number--;
    }
    if (number == 0) {
        colour = 'N';
    }
}

bool BgPoint::isEmpty() const {
    return colour == 'N' || number == 0;
}

bool BgPoint::operator==(const BgPoint& chk) const {
    return colour == chk.colour;
}

bool BgPoint::operator!=(const BgPoint& chk) const {
    return colour != chk.colour;
}

#pragma package(smart_init)
