// Collectible.h
#pragma once
#include "Player.h"
#include "Sprite.h"
#include "Texture.h"
#include <glm/glm.hpp>

class Collectible {
public:
    virtual ~Collectible() {
        if (sprite) delete sprite;
    }

    virtual void init(const glm::vec2& position, const glm::ivec2& tileMapOffset, ShaderProgram& shader, const std::string& textureFile) {
        pos = position;
        tileMapDispl = tileMapOffset;

        if (!spritesheet.loadFromFile(textureFile, TEXTURE_PIXEL_FORMAT_RGBA)) {
            std::cerr << "Error al cargar textura: " << textureFile << std::endl;
        }

        sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.0f, 1.0f), &spritesheet, &shader);
        sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
    }

    virtual void onCollect(Player& player) = 0;

    void update(int deltaTime) {
        if (!collected && sprite)
            sprite->update(deltaTime);
    }

    void render() {
        if (!collected && sprite)
            sprite->render();
    }

    void destroy() { collected = true; }
    bool isCollected() const { return collected; }

    glm::vec2 getPosition() const { return pos; }

protected:
    glm::vec2 pos;
    glm::ivec2 tileMapDispl;
    Texture spritesheet;
    Sprite* sprite = nullptr;
    bool collected = false;
};
