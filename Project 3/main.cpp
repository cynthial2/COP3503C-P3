// Project 3: Minesweeper
// main.cpp
// requires: TextureManager.h, Random.h, Tile.h, Game.h
// last edited: 12/7/22

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>
#include "TextureManager.h"
#include "Random.h"
#include "Tile.h"
#include "Game.h"
std::mt19937 Random::random(time(0));
unordered_map<string, sf::Texture> TextureManager::textures;
using namespace std;

int main () {

    // Load initial board and window
    int cols, rows, counter = 0;
    Game::LoadNormalBoard(cols, rows, counter);
    int window_height = (rows * 32) + 100;
    int window_width = cols * 32;

    // Load Sprites
    sf::Sprite debugSprite(TextureManager::GetTexture("debug"));
    sf::Sprite happySprite(TextureManager::GetTexture("face_happy"));
    sf::Sprite loseSprite(TextureManager::GetTexture("face_lose"));
    sf::Sprite winSprite(TextureManager::GetTexture("face_win"));
    sf::Sprite flagSprite(TextureManager::GetTexture("flag"));
    sf::Sprite mineSprite(TextureManager::GetTexture("mine"));
    sf::Sprite number1Sprite(TextureManager::GetTexture("number_1"));
    sf::Sprite number2Sprite(TextureManager::GetTexture("number_2"));
    sf::Sprite number3Sprite(TextureManager::GetTexture("number_3"));
    sf::Sprite number4Sprite(TextureManager::GetTexture("number_4"));
    sf::Sprite number5Sprite(TextureManager::GetTexture("number_5"));
    sf::Sprite number6Sprite(TextureManager::GetTexture("number_6"));
    sf::Sprite number7Sprite(TextureManager::GetTexture("number_7"));
    sf::Sprite number8Sprite(TextureManager::GetTexture("number_8"));
    sf::Sprite test1Sprite(TextureManager::GetTexture("test_1"));
    sf::Sprite test2Sprite(TextureManager::GetTexture("test_2"));
    sf::Sprite test3Sprite(TextureManager::GetTexture("test_3"));
    sf::Sprite unrevealedSprite(TextureManager::GetTexture("tile_hidden"));
    sf::Sprite revealedSprite(TextureManager::GetTexture("tile_revealed"));
    sf::Sprite digitsSprite(TextureManager::GetTexture("digits"));

    // Create window
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Minesweeper");

    // Initialize variables
    Tile *tiles[cols][rows];
    vector<vector<char>> board;

    bool boardOn = false;
    int boardNum = 1;
    bool reset = true;
    bool gameOver = false;
    bool gameWin = false;
    bool debug_mode = false;
    int mine_count = counter;
    int tile_count = rows*cols - mine_count;

    // When window is open
    while (window.isOpen()) {

        /// Reset Board ///
        if (reset) {

            gameOver = false;
            gameWin = false;
            debug_mode = false;
            mine_count = counter;

            if (boardOn) {
                cout << "loading preset board " << boardNum << endl;
                Game::LoadPresetBoard(board, boardNum, cols, rows);
            }

            // Create all tiles
            for (int i = 0; i < cols; ++i) {
                for (int j = 0; j < rows; ++j) {
                    tiles[i][j] = new Tile;
                    tiles[i][j]->xCoord = i * 32;
                    tiles[i][j]->yCoord = j * 32;
                }
            }

            // Determine adjacent tiles
            for (int i = 0; i < cols; ++i) {
                for (int j = 0; j < rows; ++j) {
                    // top left
                    if ((i - 1 >= 0) && (j - 1 >= 0)) {
                        tiles[i][j]->adjacentTiles.push_back(tiles[i - 1][j - 1]);
                    }
                    // top middle
                    if (j - 1 >= 0) {
                        tiles[i][j]->adjacentTiles.push_back(tiles[i][j - 1]);
                    }
                    // top right
                    if ((i + 1 < cols) && (j - 1 >= 0)) {
                        tiles[i][j]->adjacentTiles.push_back(tiles[i + 1][j - 1]);
                    }
                    // left
                    if (i - 1 >= 0) {
                        tiles[i][j]->adjacentTiles.push_back(tiles[i - 1][j]);
                    }
                    // right
                    if (i + 1 < cols) {
                        tiles[i][j]->adjacentTiles.push_back(tiles[i + 1][j]);
                    }
                    // bottom left
                    if ((i - 1 >= 0) && (j + 1 < rows)) {
                        tiles[i][j]->adjacentTiles.push_back(tiles[i - 1][j + 1]);
                    }
                    // bottom middle
                    if (j + 1 < rows) {
                        tiles[i][j]->adjacentTiles.push_back(tiles[i][j + 1]);
                    }
                    // bottom right
                    if ((i + 1 < cols) && (j + 1 < rows)) {
                        tiles[i][j]->adjacentTiles.push_back(tiles[i + 1][j + 1]);
                    }
                }
            }

            // Create mine tiles
            if (!boardOn) {
                int count = 0;
                while (count != counter) {
                    // generator random x and y tile values to place mine
                    int randomX = Random::RandomInt(0, window.getSize().x) / 32;
                    int randomY = Random::RandomInt(0, window.getSize().y - 100) / 32;
                    if (tiles[randomX][randomY]->ExistMine) {
                        continue;
                    }
                    tiles[randomX][randomY]->ExistMine = true;
                    count += 1;
                }
            } else {
                mine_count = 0;
                for (int i = 0; i < cols; ++i) {
                    for (int j = 0; j < rows; ++j) {
                        if (board.at(j).at(i) == '1') {
                            tiles[i][j]->ExistMine = true;
                            mine_count += 1;
                        }
                    }
                }
            }

            // Determine how many mines around each tile
            for (int i = 0; i < cols; ++i) {
                for (int j = 0; j < rows; ++j) {
                    // check adjacent tiles
                    for (int k = 0; k < tiles[i][j]->adjacentTiles.size(); ++k) {
                        if (tiles[i][j]->adjacentTiles.at(k)->ExistMine) {
                            tiles[i][j]->adjacentMines += 1;
                        }
                    }
                }
            }

            boardOn = false;
            reset = false;
            tile_count = rows*cols - mine_count;
        }

        /// Events ///
        sf::Event event;
        while (window.pollEvent(event)) {

            // close window
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // mouse click
            if (event.type == sf::Event::MouseButtonPressed) {
                auto coords = sf::Mouse::getPosition(window);
                int x_tile = coords.x / 32;
                int y_tile = coords.y / 32;

                // Left click: Reveal tile/Select menu option
                if (event.mouseButton.button == sf::Mouse::Left) {
                    cout << "left click: ";
                    // Left click on tiles
                    if ((sf::Mouse::getPosition(window).y < window_height - 100) && (!gameOver) && (!gameWin)) {
                        // if tile is unrevealed AND not flagged, reveal tile
                        if ((tiles[x_tile][y_tile]->ExistUnrevealed) && (!tiles[x_tile][y_tile]->DrawFlag)) {
                            // if not a mine, change NUMBER OF MINES ADJACENT to true
                            if (!tiles[x_tile][y_tile]->ExistMine) {
                                cout << "revealed tile" << endl;
                                Game::RevealTile(*tiles[x_tile][y_tile], tile_count);
                                Game::RecursiveReveal(*tiles[x_tile][y_tile], tile_count);
                                // if all tiles revealed... (GAME WIN!)
                                if (tile_count == 0) {
                                    cout << "game win! :)" << endl;
                                    gameWin = true;
                                    // Draw all flags at mines
                                    for (int i = 0; i < cols; ++i) {
                                        for (int j = 0; j < rows; ++j) {
                                            if ((tiles[i][j]->ExistMine) && (!tiles[i][j]->DrawFlag)) {
                                                tiles[i][j]->DrawFlag = true;
                                                mine_count -= 1;
                                            }
                                        }
                                    }
                                }
                            }
                                // if is a mine, change MINE to true (GAME OVER!)
                            else {
                                tiles[x_tile][y_tile]->DrawMine = true;
                                cout << "game over :(" << endl;
                                gameOver = true;
                                // Draw all mines (over flags placed)
                                for (int i = 0; i < cols; ++i) {
                                    for (int j = 0; j < rows; ++j) {
                                        if ((tiles[i][j]->ExistMine) && (!tiles[i][j]->DrawMine)) {
                                            tiles[i][j]->DrawMine = true;
                                            tiles[i][j]->DrawRevealed = true;
                                        }
                                    }
                                }
                            }
                            // tile is no longer unrevealed -> cannot flag
                            tiles[x_tile][y_tile]->ExistUnrevealed = false;
                            tiles[x_tile][y_tile]->DrawRevealed = true;
                        }
                    }
                    // Left click in menu
                    if (sf::Mouse::getPosition(window).y > window_height - 100) {
                        // click face (reset game)
                        if ((sf::Mouse::getPosition(window).x >= window_width / 2 - 32) &&
                            (sf::Mouse::getPosition(window).x < window_width / 2 + 64)) {
                            cout << "clicked face" << endl;
                            reset = true;
                        }
                        // click debug mode
                        if ((sf::Mouse::getPosition(window).x >= window_width / 2 + 96) &&
                            (sf::Mouse::getPosition(window).x < window_width / 2 + 160) && (!gameOver) && (!gameWin)) {
                            cout << "clicked debug" << endl;
                            // iterate through all tiles, reveal/unreveal all mines
                            if (!debug_mode) {
                                for (int i = 0; i < cols; ++i) {
                                    for (int j = 0; j < rows; ++j) {
                                        if (tiles[i][j]->ExistMine) {
                                            tiles[i][j]->DrawMine = true;
                                        }
                                    }
                                }
                                debug_mode = true;
                            }
                            else {
                                for (int i = 0; i < cols; ++i) {
                                    for (int j = 0; j < rows; ++j) {
                                        if (tiles[i][j]->ExistMine) {
                                            tiles[i][j]->DrawMine = false;
                                        }
                                    }
                                }
                                debug_mode = false;
                            }
                        }
                        // click test board 1
                        if ((sf::Mouse::getPosition(window).x >= window_width / 2 + 160) &&
                            (sf::Mouse::getPosition(window).x < window_width / 2 + 224)) {
                            cout << "clicked test board 1" << endl;
                            boardOn = true;
                            boardNum = 1;
                            reset = true;
                        }
                        // click test board 2
                        if ((sf::Mouse::getPosition(window).x >= window_width / 2 + 224) &&
                            (sf::Mouse::getPosition(window).x < window_width / 2 + 288)) {
                            cout << "clicked test board 2" << endl;
                            boardOn = true;
                            boardNum = 2;
                            reset = true;
                        }
                        // click test board 3
                        if ((sf::Mouse::getPosition(window).x >= window_width / 2 + 288) &&
                            (sf::Mouse::getPosition(window).x < window_width / 2 + 352)) {
                            cout << "clicked test board 3" << endl;
                            boardOn = true;
                            boardNum = 3;
                            reset = true;
                        }
                    }
                }
                // Right click: place flag
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    cout << "right click: ";
                    // Right click on tiles (only option)
                    if ((sf::Mouse::getPosition(window).y < window_height - 100) && (!gameOver) && (!gameWin)) {
                        if (tiles[x_tile][y_tile]->ExistUnrevealed) {
                            Game::PlaceFlag(*tiles[x_tile][y_tile], mine_count);
                        }
                    }
                }
            }
        }

        if (reset) {
            cout << "resetting..." << endl;
            // Delete all tiles
            for (int i = 0; i < cols; ++i) {
                for (int j = 0; j < rows; ++j) {
                    delete tiles[i][j];
                }
            }
            // Clear preset board
            board.clear();
            continue;
        }

        /// Display ///
        window.clear(sf::Color::White);

        // Draw bottom row buttons
        if (gameOver) {
            loseSprite.setPosition(window_width / 2 - 32, rows * 32);
            window.draw(loseSprite);
        } else if (gameWin) {
            winSprite.setPosition(window_width / 2 - 32, rows * 32);
            window.draw(winSprite);
        } else {
            happySprite.setPosition(window_width / 2 - 32, rows * 32);
            window.draw(happySprite);
        }
        debugSprite.setPosition(window_width / 2 + 96, rows * 32);
        window.draw(debugSprite);
        test1Sprite.setPosition(window_width / 2 + 160, rows * 32);
        window.draw(test1Sprite);
        test2Sprite.setPosition(window_width / 2 + 224, rows * 32);
        window.draw(test2Sprite);
        test3Sprite.setPosition(window_width / 2 + 288, rows * 32);
        window.draw(test3Sprite);

        // obtain and draw ones digit
        int offset1 = abs(mine_count % 10);
        digitsSprite.setTextureRect(sf::IntRect(offset1 * 21, 0, 21, 32));
        digitsSprite.setPosition(window_width / 2 - 293, rows * 32);
        window.draw(digitsSprite);
        // obtain and draw tens digit
        int offset2 = abs(((mine_count - mine_count % 10) % 100) / 10);
        digitsSprite.setTextureRect(sf::IntRect(offset2 * 21, 0, 21, 32));
        digitsSprite.setPosition(window_width / 2 - 314, rows * 32);
        window.draw(digitsSprite);
        // obtain and draw hundreds digit
        if (mine_count < 0) {
            // draw negative sign
            digitsSprite.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
        } else {
            // obtain and draw hundreds digit
            int offset3 = abs(mine_count / 100);
            digitsSprite.setTextureRect(sf::IntRect(offset3 * 21, 0, 21, 32));
        }
        digitsSprite.setPosition(window_width / 2 - 335, rows * 32);
        window.draw(digitsSprite);

        // Iterate thru tiles[x][y] and decide what should be drawn for that tile
        for (int i = 0; i < cols; ++i) {
            for (int j = 0; j < rows; ++j) {
                // draw unrevealed tile?
                if (tiles[i][j]->DrawUnrevealed) {
                    tiles[i][j]->DrawSprite(window, unrevealedSprite);
                }
                // draw revealed tile? (on bottommost layer!)
                if (tiles[i][j]->DrawRevealed) {
                    tiles[i][j]->DrawSprite(window, revealedSprite);
                }
                // draw flag?
                if (tiles[i][j]->DrawFlag) {
                    tiles[i][j]->DrawSprite(window, flagSprite);
                }
                // draw number 1?
                if (tiles[i][j]->DrawN1) {
                    tiles[i][j]->DrawSprite(window, number1Sprite);
                }
                // draw number 2?
                if (tiles[i][j]->DrawN2) {
                    tiles[i][j]->DrawSprite(window, number2Sprite);
                }
                // draw number 3?
                if (tiles[i][j]->DrawN3) {
                    tiles[i][j]->DrawSprite(window, number3Sprite);
                }
                // draw number 4?
                if (tiles[i][j]->DrawN4) {
                    tiles[i][j]->DrawSprite(window, number4Sprite);
                }
                // draw number 5?
                if (tiles[i][j]->DrawN5) {
                    tiles[i][j]->DrawSprite(window, number5Sprite);
                }
                // draw number 6?
                if (tiles[i][j]->DrawN6) {
                    tiles[i][j]->DrawSprite(window, number6Sprite);
                }
                // draw number 7?
                if (tiles[i][j]->DrawN7) {
                    tiles[i][j]->DrawSprite(window, number7Sprite);
                }
                // draw number 8?
                if (tiles[i][j]->DrawN8) {
                    tiles[i][j]->DrawSprite(window, number8Sprite);
                }
                // draw mine? (on topmost layer!)
                if (tiles[i][j]->DrawMine) {
                    tiles[i][j]->DrawSprite(window, mineSprite);
                }
            }
        }

        window.display();

    }

    // Clear out any textures before program ends
    TextureManager::Clear();
    return 0;

}
