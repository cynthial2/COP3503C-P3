// Project 3: Minesweeper (Game.h)
// last edited: 12/7/22

#include <SFML/Graphics.hpp>
using namespace std;
#pragma once

class Game {

public:

    /// Setup Functions ///
    static void LoadPresetBoard(vector<vector<char>> &board, int &boardNum, int &cols, int &rows);
    static void LoadNormalBoard(int &cols, int &rows, int &counter);

    /// Game Functions ///
    static void PlaceFlag(Tile &tile, int &counter);
    static void RevealTile(Tile &tile, int &tile_count);
    static void RecursiveReveal(Tile &tile, int &tile_count);

};

/// Implementation ///

void Game::LoadPresetBoard(vector<vector<char>> &board, int &boardNum, int &cols, int &rows) {
    // Load correct board
    string name;
    if (boardNum == 1) {
        name = "boards/testboard1.brd";
        //name = "boards/lots_o_mines.brd";
    }
    else if (boardNum == 2) {
        name = "boards/testboard2.brd";
        //name = "boards/recursion_test.brd";
    }
    else {
        name = "boards/testboard3.brd";
    }
    ifstream file(name);
    // read data create board vector
    if (file.is_open()) {
        string line;
        rows = 0;
        while (getline(file, line, '\n')) {
            cols = line.length();
            vector<char> temp;
            for (int i = 0; i < cols; ++i) {
                temp.push_back(line.at(i));
            }
            board.push_back(temp);
            rows += 1;
        }
    }
    else {
        cout << "file not open" << endl;
    }
}

void Game::LoadNormalBoard(int &cols, int &rows, int &counter) {
    // Initialize board data (no preset)
    ifstream file("boards/config.cfg");
    string a, b, c;
    getline(file, a, '\n');
    cols = stoi(a);
    getline(file, b, '\n');
    rows = stoi(b);
    getline(file, c, '\n');
    counter = stoi(c);
}

void Game::PlaceFlag(Tile &tile, int &counter) {
    // if tile has no flag, change Flag to true (place flag)
    if (!tile.DrawFlag) {
        cout << "placed flag" << endl;
        tile.DrawFlag = true;
        counter -= 1;
    }
        // if tile has flag, change Flag to false (remove flag)
    else {
        cout << "removed flag" << endl;
        tile.DrawFlag = false;
        counter += 1;
    }
}

void Game::RevealTile(Tile &tile, int &tile_count) {
    // if adjacent mines = 0
    if (tile.adjacentMines == 0) {
        tile.DrawRevealed = true;
    }
    // if adjacent mines = 1
    if (tile.adjacentMines == 1) {
        tile.DrawN1 = true;
    }
    // if adjacent mines = 2
    if (tile.adjacentMines == 2) {
        tile.DrawN2 = true;
    }
    // if adjacent mines = 3
    if (tile.adjacentMines == 3) {
        tile.DrawN3 = true;
    }
    // if adjacent mines = 4
    if (tile.adjacentMines == 4) {
        tile.DrawN4 = true;
    }
    // if adjacent mines = 5
    if (tile.adjacentMines == 5) {
        tile.DrawN5 = true;
    }
    // if adjacent mines = 6
    if (tile.adjacentMines == 6) {
        tile.DrawN6 = true;
    }
    // if adjacent mines = 7
    if (tile.adjacentMines == 7) {
        tile.DrawN7 = true;
    }
    // if adjacent mines = 8
    if (tile.adjacentMines == 8) {
        tile.DrawN8 = true;
    }
    tile_count -= 1;
    tile.ExistUnrevealed = false;
    tile.DrawRevealed = true;
}

void Game::RecursiveReveal(Tile &tile, int &tile_count) {
    if (tile.adjacentMines == 0) {
        for (int i = 0; i < tile.adjacentTiles.size(); ++i) {
            //cout << "adjacent tile #" << i << endl;
            if ((tile.adjacentTiles[i]->ExistUnrevealed) && (!tile.adjacentTiles[i]->DrawFlag)) {
                RevealTile((*tile.adjacentTiles[i]), tile_count);
              if (tile.adjacentTiles[i]->adjacentMines == 0) {
                  RecursiveReveal((*tile.adjacentTiles[i]), tile_count);
              }
            }
        }
    }
}

