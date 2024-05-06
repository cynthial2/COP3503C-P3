// Testing if SFML installed properly

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    // VideoMode = resolution (how big it is)
    // "string" = title
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Application");

    // initialize circle shape object
    sf::CircleShape shape;
    shape.setRadius(40);
    shape.setPointCount(1000);
    shape.setPosition(100.f, 100.f); //gets overridden later

    // initialize pikachu
    sf::Texture texture;
    texture.loadFromFile("Images/pikachu.jpg");
    sf::Sprite pikachu;
    pikachu.setTexture(texture);
    pikachu.scale(0.75, 0.75);

    std::vector<sf::Sprite> pikas;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {

            // if hit close, window close
            if (event.type == sf::Event::Closed)
                window.close();

            // adding another event...
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i coordinates = sf::Mouse::getPosition(window);
                cout << "x: " << coordinates.x << endl;
                cout << "y: " << coordinates.y << endl;
                pikachu.setPosition(coordinates.x, coordinates.y);
                pikachu.setColor(sf::Color(rand(),rand(),rand()));
                pikas.push_back(pikachu);
                cout << "Pikachu was printed" << endl;
            }
        }

        // 3-step process: clear > draw > display
        window.clear();

        // Draw many circles (will be under the pikachu bc layers)
        for (int i = 0; i < 5; ++i)
        {
            shape.setPosition(0 + 100*i, 100);
            shape.setFillColor(sf::Color::Magenta);
            window.draw(shape);
        }

        for (int j = 0; j < pikas.size(); ++j) {
            srand(j); // random number seed
            pikas[j].setColor(sf::Color(rand(),rand(),rand()));
            //pikas[j].setColor(sf::Color(rand(),rand(),rand(), 200));    The 200 is transparency!
            window.draw(pikas[j]);
        }

        sf::Vector2i coordinates = sf::Mouse::getPosition(window);
        pikachu.setPosition(coordinates.x, coordinates.y);
        window.draw(pikachu);
        window.display();
    }
}