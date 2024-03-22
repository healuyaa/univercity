#include <vector>
#include <random>
#include <iostream>

#include "SFML/Graphics.hpp"

struct Textures {
    sf::Texture github_icon;
    sf::Texture steam_icon;
    sf::Texture discord_icon;
};

class Loader {
    public:
    virtual void LoadTexturesFromFile(Textures &textures) = 0;
    virtual void SmoothTextures(Textures &textures) = 0;
};

class Icons : public Loader {
    public:
    void LoadTexturesFromFile(Textures &textures) override {
        if (!textures.github_icon.loadFromFile(github_icon_path)) {
            std::cout << "Err load icon: github\n";
            return;
        }

        if (!textures.steam_icon.loadFromFile(steam_icon_path)) {
            std::cout << "Err load icon: steam\n";
            return;
        }

        if (!textures.discord_icon.loadFromFile(discord_icon_path)) {
            std::cout << "Err load icon: discord\n";
            return;
        }
    }

    void SmoothTextures(Textures &textures) override {
        textures.github_icon.setSmooth(true);
        textures.steam_icon.setSmooth(true);
        textures.discord_icon.setSmooth(true);
    }

    private:
    std::string github_icon_path = "../../assets/icons/github25.png";
    std::string steam_icon_path = "../../assets/icons/steam25.png";
    std::string discord_icon_path = "../../assets/icons/discord25.png";
};

