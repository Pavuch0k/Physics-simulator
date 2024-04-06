#include <SFML/Graphics.hpp>
#include <cmath>

const float GRAVITY = 0.9f;
const float FRICTION = 0.98f;
const float ELASTICITY = 0.7f;

struct Cube {
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    sf::Vector2f offset; // Смещение относительно мыши

    Cube(float size, sf::Vector2f position) {
        shape.setSize(sf::Vector2f(size, size));
        shape.setFillColor(sf::Color::White);
        shape.setPosition(position);
        velocity = sf::Vector2f(0, 0);
        offset = sf::Vector2f(0, 0);
    }
};

void handleCollisions(Cube& cube, sf::RenderWindow& window) {
    sf::FloatRect bounds = cube.shape.getGlobalBounds();
    if (bounds.left < 0) {
        cube.velocity.x = std::abs(cube.velocity.x) * ELASTICITY;
        cube.shape.setPosition(0, bounds.top);
    }
    else if (bounds.left + bounds.width > window.getSize().x) {
        cube.velocity.x = -std::abs(cube.velocity.x) * ELASTICITY;
        cube.shape.setPosition(window.getSize().x - bounds.width, bounds.top);
    }
    if (bounds.top < 0) {
        cube.velocity.y = std::abs(cube.velocity.y) * ELASTICITY;
        cube.shape.setPosition(bounds.left, 0);
    }
    else if (bounds.top + bounds.height > window.getSize().y) {
        cube.velocity.y = -std::abs(cube.velocity.y) * ELASTICITY;
        cube.shape.setPosition(bounds.left, window.getSize().y - bounds.height);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Physics simulator");
    window.setFramerateLimit(60);

    Cube cube(50, sf::Vector2f(375, 275));
    sf::Vector2f previousMousePos;
    bool isDragging = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (cube.shape.getGlobalBounds().contains(mousePos)) {
                    isDragging = true;
                    cube.offset = mousePos - cube.shape.getPosition(); // Сохраняем смещение относительно мыши
                    cube.velocity = sf::Vector2f(0, 0);
                    previousMousePos = mousePos;
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isDragging = false;
            }
        }

        if (isDragging) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::Vector2f mouseDelta = mousePos - previousMousePos;
            cube.velocity = mouseDelta;
            cube.shape.setPosition(mousePos - cube.offset); // Обновляем позицию кубика относительно мыши и смещения
            previousMousePos = mousePos;
        }
        else {
            cube.velocity.y += GRAVITY;
            cube.velocity *= FRICTION;
            cube.shape.move(cube.velocity);
            handleCollisions(cube, window);
        }

        window.clear(sf::Color::Black);
        window.draw(cube.shape);
        window.display();
    }


}