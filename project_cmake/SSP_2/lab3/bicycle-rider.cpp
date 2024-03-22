#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

const int WINDOW_WIDTH = 380;
const int WINDOW_HEIGHT = 365;

const int FRAME_RATE = 60;

class Bicycle {
public:
    Bicycle() {
        if (!texture.loadFromFile("../../assets/dreamstimemaximum_290104741.png")) {
            std::cerr << "Failed to load bicycle sprite!" << std::endl;
        }

        sprite.setTexture(texture);
        sprite.setScale(1.0f, 1.0f);
        currentFrame = 0;
    }

    void update() {
        currentFrame = (currentFrame + 1) % 10;
        int row = currentFrame < 5 ? 0 : 1;
        int col = currentFrame % 5;
        sprite.setTextureRect(sf::IntRect(col * 380, row * 365, 380, 365));
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

private:
    sf::Texture texture;
    sf::Sprite sprite;
    int currentFrame;
};

void pedal(Bicycle& bicycle) {
    while (true) {
        bicycle.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Bicycle Animation with SFML");

    Bicycle bicycle;

    std::thread pedalingThread(pedal, std::ref(bicycle));

    window.setFramerateLimit(FRAME_RATE);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        bicycle.draw(window);
        window.display();
    }

    pedalingThread.join();
    return 0;
}
