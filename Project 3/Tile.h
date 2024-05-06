// Project 3: Minesweeper (Tile.h)
// last edited: 11/29/22

#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;
#pragma once

struct Tile {

    /// Tile position & adjacent mines ///
    int xCoord = 0;
    int yCoord = 0;

    vector<Tile*> adjacentTiles;
    int adjacentMines = 0;

    /// Determines if sprite currently exists and if should draw ///
    bool DrawUnrevealed = true;
    bool ExistUnrevealed = true;

    bool DrawFlag = false;
    bool DrawMine = false;
    bool ExistMine = false;

    bool DrawRevealed = false;
    bool DrawN1 = false;
    bool DrawN2 = false;
    bool DrawN3 = false;
    bool DrawN4 = false;
    bool DrawN5 = false;
    bool DrawN6 = false;
    bool DrawN7 = false;
    bool DrawN8 = false;

    /// Functions ///
    void DrawSprite(sf::RenderWindow &window, sf::Sprite &spriteName) const;

};

/// Implementation ///
void Tile::DrawSprite(sf::RenderWindow &window, sf::Sprite &spriteName) const {
    spriteName.setPosition(xCoord, yCoord);
    window.draw(spriteName);
}
