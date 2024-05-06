// Project 3: Minesweeper (Textures.h)
// last edited: 11/29/22

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using namespace std;
#pragma once

class TextureManager {

    static unordered_map<string, sf::Texture> textures;
    static void LoadTexture(string fileName);

public:

    static sf::Texture& GetTexture(string textureName);
    static void Clear(); // call once at end of main()

};

void TextureManager::LoadTexture(string fileName) {
    string path = "images/";
    path += fileName + ".png";
    textures[fileName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName) {
    // if texture does not exist... load first?
    if (textures.find(textureName) == textures.end()) {
        LoadTexture(textureName);
    }
    return textures[textureName];
}

void TextureManager::Clear() {
    textures.clear();
}