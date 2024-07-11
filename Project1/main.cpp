#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;

vector<sf::Color> Colors = {
    sf::Color::Red,
    sf::Color::Green,
    sf::Color::Blue,
    sf::Color::Yellow,
    sf::Color::Magenta,
    sf::Color::Cyan
};

class Bullet {
    sf::CircleShape shape;
    sf::Vector2f velocity;

public:
    Bullet(sf::Vector2f position, sf::Vector2f direction) {
        shape.setRadius(5);
        shape.setFillColor(sf::Color::Black);
        shape.setPosition(position);
        velocity = direction * 300.0f; // Bullet speed
    }

    void move(float deltaTime) {
        shape.move(velocity * deltaTime);
    }

    sf::CircleShape& GetShape() {
        return shape;
    }

    sf::Vector2f GetPosition() const {
        return shape.getPosition();
    }
};

class CirclesGenerating {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    int health;

public:
    CirclesGenerating(float radius, sf::Vector2f position, sf::Color color, int h) {
        shape.setRadius(radius);
        shape.setPosition(position);
        shape.setFillColor(color);
        this->health = h;

        // Random velocity direction and speed
        velocity.x = static_cast<float>(rand() % 200 - 100); // Random horizontal velocity (-100 to 100 pixels per second)
        velocity.y = static_cast<float>(rand() % 200 - 100); // Random vertical velocity (-100 to 100 pixels per second)
    }

    void move(float deltaTime) {
        // Update position based on velocity and deltaTime
        sf::Vector2f position = shape.getPosition();
        position += velocity * deltaTime;

        // Check collision with window edges
        if (position.x - shape.getRadius() < 0 || position.x + shape.getRadius() > 800) {
            velocity.x = -velocity.x; // Reverse horizontal velocity
        }
        if (position.y - shape.getRadius() < 0 || position.y + shape.getRadius() > 600) {
            velocity.y = -velocity.y; // Reverse vertical velocity
        }

        // Update shape position
        shape.setPosition(position);
    }

    sf::CircleShape& GetCircleShape() {
        return shape;
    }

    sf::Vector2f GetPosition() const {
        return shape.getPosition();
    }

    float GetRadius() const {
        return shape.getRadius();
    }

    int GetHealth() const {
        return health;
    }

    void DecreaseHealth() {
        health--;
    }

    void HandleCollision(CirclesGenerating& other) {
        // Check distance between centers
        sf::Vector2f direction = other.GetPosition() - GetPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // Calculate minimum distance before they touch
        float minDistance = GetRadius() + other.GetRadius();

        // If they collide (overlap), handle collision response
        if (distance < minDistance) {
            // Normalize direction vector
            direction /= distance;

            // Move circles apart
            sf::Vector2f moveAmount = direction * (minDistance - distance);
            shape.move(-moveAmount / 2.0f);
            other.shape.move(moveAmount / 2.0f);

            // Reverse velocities (simple elastic collision)
            sf::Vector2f tempVelocity = velocity;
            velocity = other.velocity;
            other.velocity = tempVelocity;
        }
    }
};

class Player {
    sf::CircleShape PlayerShape;
    sf::Vector2f lastDirection;
    sf::Clock shotClock; // Clock to track time between shots
    float shotCooldown;
    int health; // Player health

public:
    Player() : health(100), shotCooldown(0.5f) { // Initial health and cooldown
        PlayerShape.setFillColor(sf::Color::Black);
        PlayerShape.setRadius(10);
        PlayerShape.setPosition(400, 300);
        lastDirection = sf::Vector2f(0, 0);
    }

    int& gethealth() {
        return health;
    }

    void HandleCollision(CirclesGenerating& other) {
        // Check distance between centers
        sf::Vector2f direction = other.GetPosition() - PlayerShape.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // Calculate minimum distance before they touch
        float minDistance = PlayerShape.getRadius() + other.GetRadius();

        // If they collide (overlap), handle collision response
        if (distance < minDistance) {
            // Normalize direction vector
            direction /= distance;

            // Move circles apart
            sf::Vector2f moveAmount = direction * (minDistance - distance);
            PlayerShape.move(-moveAmount / 2.0f);

            // Decrease health on collision
            health -= 1;
            if (health < 0) health = 0;

            std::cout << "Player Health: " << health << std::endl;
        }
    }

    void PlayerInput(float dt) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            PlayerShape.move(-100 * dt, 0); // Move left
            lastDirection = sf::Vector2f(-1, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            PlayerShape.move(100 * dt, 0); // Move right
            lastDirection = sf::Vector2f(1, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            PlayerShape.move(0, -100 * dt); // Move up
            lastDirection = sf::Vector2f(0, -1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            PlayerShape.move(0, 100 * dt); // Move down
            lastDirection = sf::Vector2f(0, 1);
        }
    }

    bool CanShoot() {
        if (shotClock.getElapsedTime().asSeconds() >= shotCooldown) {
            shotClock.restart();
            return true;
        }
        return false;
    }

    void DecreaseHealth() {
        health--;
    }

    sf::CircleShape& getShape() {
        return PlayerShape;
    }

    sf::Vector2f GetPosition() const {
        return PlayerShape.getPosition();
    }

    sf::Vector2f GetLastDirection() const {
        return lastDirection;
    }

    int GetHealth() const {
        return health;
    }
};

void spawnEnemy(vector<CirclesGenerating>& shapes, float& enemyRadius, int& enemyHealth) {
    sf::Vector2f position(static_cast<float>(rand() % 700 + 50), static_cast<float>(rand() % 500 + 50)); // Random position within screen bounds
    sf::Color color = Colors[rand() % Colors.size()]; // Random color
    shapes.emplace_back(enemyRadius, position, color, enemyHealth);
    enemyRadius += 2; // Gradually increase radius
    enemyHealth += 5; // Gradually increase health
}

int main() {
    sf::Font middlefont;
    if (!(middlefont.loadFromFile("/home/sujal/Documents/SFML/Project1/Foldit/Foldit-VariableFont_wght.ttf"))) {
        std::cout << "middlefont loaded" << std::endl;
    }

    sf::Text middletext;
    middletext.setFont(middlefont);
    middletext.setString("SpaceBar to shoot\n WASD to move");
    middletext.setPosition(10, 10);
    middletext.setCharacterSize(50);
    middletext.setFillColor(sf::Color::Black); // Corrected the text color setting
    middletext.setStyle(sf::Text::Bold); // Corrected the text style setting
    middletext.setLetterSpacing(1);

    sf::Text health;
    health.setFont(middlefont);
    health.setPosition(100, 500);
    health.setCharacterSize(50);
    health.setFillColor(sf::Color::Black); // Corrected the text color setting
    health.setStyle(sf::Text::Bold); // Corrected the text style setting
    health.setLetterSpacing(1);

    sf::Text scoreText;
    scoreText.setFont(middlefont);
    scoreText.setPosition(600, 10);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(sf::Color::Black); // Corrected the text color setting
    scoreText.setStyle(sf::Text::Bold); // Corrected the text style setting
    scoreText.setLetterSpacing(1);

    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Colliding Circles");
    sf::Clock clock;
    sf::Clock spawnClock; // Clock to track enemy spawning

    Player player;
    vector<CirclesGenerating> shapes;
    vector<Bullet> bullets;

    float enemyRadius = 20.0f; // Starting radius for new enemies
    int enemyHealth = 10; // Starting health for new enemies
    int score = 0;

    for (int i = 0; i < 10; ++i) {
        spawnEnemy(shapes, enemyRadius, enemyHealth);
    }

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        string str = "Health: " + to_string(player.gethealth());
        health.setString(str);

        string scoreStr = "Score: " + to_string(score);
        scoreText.setString(scoreStr);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.PlayerInput(dt);

        // Shoot bullet if cooldown allows
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && player.CanShoot()) {
            sf::Vector2f playerPosition = player.GetPosition();
            sf::Vector2f direction = player.GetLastDirection();
            if (direction != sf::Vector2f(0, 0)) { // Ensure the direction is valid
                bullets.emplace_back(playerPosition, direction);
            }
        }

        // Move bullets
        for (auto& bullet : bullets) {
            bullet.move(dt);
        }

        // Move all circles
        for (auto& shape : shapes) {
            shape.move(dt);
        }

        // Handle collisions between circles
        for (size_t i = 0; i < shapes.size(); ++i) {
            for (size_t j = i + 1; j < shapes.size(); ++j) {
                shapes[i].HandleCollision(shapes[j]);
            }
        }

        // Handle bullet collisions with circles
        for (auto& bullet : bullets) {
            for (auto& shape : shapes) {
                sf::Vector2f direction = bullet.GetPosition() - shape.GetPosition();
                float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (distance < shape.GetRadius()) {
                    shape.DecreaseHealth();
                }
            }
        }

        // Handle player collisions with circles
        for (auto& shape : shapes) {
            player.HandleCollision(shape);
        }

        // Remove dead circles and increase score
        shapes.erase(std::remove_if(shapes.begin(), shapes.end(), [&](CirclesGenerating& shape) {
            if (shape.GetHealth() <= 0) {
                score++;
                return true;
            }
            return false;
        }), shapes.end());

        // Spawn new enemy if enough time has passed
        if (spawnClock.getElapsedTime().asSeconds() >= 2.0f) {
            spawnEnemy(shapes, enemyRadius, enemyHealth);
            spawnClock.restart();
        }

        // Draw everything
        window.clear(sf::Color::White);
        for (auto& shape : shapes) {
            window.draw(shape.GetCircleShape());
        }
        for (auto& bullet : bullets) {
            window.draw(bullet.GetShape());
        }
        window.draw(player.getShape());
        window.draw(middletext);
        window.draw(health);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
