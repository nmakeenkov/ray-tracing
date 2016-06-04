#include "MyGL.h"

MyGL::Color::Color() { }

MyGL::Color::Color(unsigned char red, unsigned char green, unsigned char blue) : mRed(red),
                                                                                 mGreen(green),
                                                                                 mBlue(blue) { }

MyGL::Color MyGL::Color::operator*(double c) const {
    return MyGL::Color(std::min(255., c * mRed), std::min(255., c * mGreen), std::min(255., c * mBlue));
}

MyGL::Color MyGL::Color::operator+(MyGL::Color const &other) const {
    return Color(std::min(255., mRed + other.mRed), std::min(255., mGreen + other.mGreen), std::min(255., mBlue + other.mBlue));
}
