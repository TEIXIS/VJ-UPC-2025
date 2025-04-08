#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Player.h"
#include "Sprite.h"
#include "Texture.h"
#include "ShaderProgram.h"

class Collectible {
public:
    virtual ~Collectible();

    virtual void init(const glm::vec2& position, const glm::ivec2& tileMapOffset,
        ShaderProgram& shader, const std::string& textureFile);

    virtual void update(int deltaTime);
    virtual void render();
    virtual void onCollect(Player& player) = 0;

    void destroy();
    bool isCollected() const;
    glm::vec2 getPosition() const;

protected:
    glm::vec2 pos;
    glm::ivec2 tileMapDispl;
    Texture spritesheet;
    Sprite* sprite = nullptr;
    bool collected = false;
};
#pragma once
