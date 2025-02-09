#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "tileMap.h"
#include "slime.h"
#include "icon.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({1920u, 1080u}), "Adventure of the Slime");
    window.setFramerateLimit(30);
    window.setVerticalSyncEnabled(true);

    std::cout << "Window created successfully.\n";

    Icon icon;
    icon.draw(window);
    std::cout << "Icon drawn successfully.\n";

    Slime mainSlime;

    // Create the tilemap from the level definition
    TileMap map;
    if (!map.loadFromFile("assets/map.txt", "assets/tileMap.png", {32, 32})) {
        std::cerr << "Failed to load tile map from file.\n";
        return -1;
    }

    std::cout << "Tile map loaded successfully.\n";

    sf::View view(sf::Vector2f(960.f, 540.f), sf::Vector2f(1920.f, 1080.f));
    view.setCenter(mainSlime.getPosition());

    sf::Clock clock;
    constexpr float animationUpdateInterval = 0.15f; // Adjust the interval to control animation speed
    float timeSinceLastUpdate = 0.0f;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            sf::Vector2f movement(0.f, 0.f);

            // Handle keyboard input for movement
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                constexpr float moveSpeed = 5.f; // Adjust the speed as needed
                if (keyPressed->scancode == sf::Keyboard::Scancode::A) {
                    movement.x -= moveSpeed;
                    mainSlime.setScale("moveLeft", {1.f, 1.f});
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::D) {
                    movement.x += moveSpeed;
                    mainSlime.setScale("moveRight", {1.f, 1.f});
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
                    movement.y -= moveSpeed;
                    mainSlime.setScale("moveUp", {1.f, 1.f});
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
                    movement.y += moveSpeed;
                    mainSlime.setScale("moveDown", {1.f, 1.f});
                }
            }

            // Check collision
            sf::Vector2f newPos = mainSlime.getPosition() + movement;
            if (!map.isCollision(newPos, {32, 32})) {
                mainSlime.move(movement);
            }
        }

        // Calculate time since last update
        float deltaTime = clock.restart().asSeconds();
        timeSinceLastUpdate += deltaTime;

        // Update the animation based on the time interval
        if (timeSinceLastUpdate >= animationUpdateInterval) {
            timeSinceLastUpdate -= animationUpdateInterval;
            // Update slime animation
            mainSlime.updateAnimation();
        }

        // Update view position to follow the player
        view.setCenter(mainSlime.getPosition());

        window.clear();
        window.setView(view);
        window.draw(map);
        mainSlime.draw(window);
        window.display();
    }

    return 0;
}
