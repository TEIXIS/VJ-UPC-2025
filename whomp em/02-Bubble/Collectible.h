#pragma once
#include "Player.h"
#include "Sprite.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include <string>

class Collectible {
public:
    virtual ~Collectible();

    virtual void init(const glm::vec2& position, const glm::ivec2& tileMapOffset,
        ShaderProgram& shader, const std::string& textureFile);

    virtual void update(int deltaTime);
    virtual void render();
    virtual void onCollect(Player& player) = 0;
    virtual void applyGravity(int deltaTime, TileMap* map);

    void destroy();
    bool isCollected() const;
    bool collidesWith(const Player& player) const;

    glm::vec2 getPosition() const;

protected:
    glm::vec2 pos;
    glm::ivec2 tileMapDispl;
    Texture spritesheet;
    Sprite* sprite = nullptr;
    bool collected = false;
    glm::vec2 velocity = glm::vec2(0.f);  // Velocidad (x, y)
    float gravity = 300.f;
};
