// Project 3: Minesweeper (Random.h)
// last edited: 11/28/22

#include <random>
#include <ctime>
#pragma once

class Random {

    // static variables must be redeclared in global space! (std::mt19937 Random::random(time(0));)
    // time(0) gives seed value relative to computer's time
    static std::mt19937 random;

public:

    // static: one and only one of this function for all objects of this class
    // static variables/functions exist for the lifetime of your program
    // how to call: make object and call OR just Random::Number(a, b);
    // "this" does not exist in static functions (can only access static variables)
    static int RandomInt(int min, int max);

};

int Random::RandomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(random);
}