#include "Platform1.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>


Platform1::Platform1()
    : sprite(nullptr)
{
}


Platform1::~Platform1() {
    if (sprite)
        delete sprite;
}


void Platform1::init(const glm::vec2& position, ShaderProgram& shaderProgram, const glm::ivec2& tileMapOffset) {
    pos = position;
    originalPos = position;
    tileMapDispl = tileMapOffset;

    if (!spritesheet.loadFromFile("images/platform2.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
        std::cerr << "Error al cargar la textura de la plataforma." << std::endl;
    }

    sprite = Sprite::createSprite(glm::ivec2(24, 14), glm::vec2(1.0f, 1.0f), &spritesheet, &shaderProgram);
    sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
}

void Platform1::render() {
    if (sprite)
        sprite->render();
}


bool Platform1::checkCollisionFromAbove(const Player& player, float collisionThreshold) {
    glm::vec2 playerPos = player.getPosition();
    glm::ivec2 playerSize(32, 32);


    bool horizontalOverlap = (playerPos.x + playerSize.x > pos.x) && (playerPos.x < pos.x + 24);

    bool verticalCollision = (playerPos.y < pos.y) &&
        (playerPos.y + playerSize.y >= pos.y) &&
        (playerPos.y + playerSize.y <= pos.y + collisionThreshold);
    return horizontalOverlap && verticalCollision;
}

bool Platform1::checkCollisionFromBelow(const Player& player, float collisionThreshold) {
    glm::vec2 playerPos = player.getPosition();
    glm::ivec2 playerSize(32, 32);

    bool horizontalOverlap = (playerPos.x + playerSize.x > pos.x) && (playerPos.x < pos.x + 32);

    bool verticalCollision = (playerPos.y <= pos.y + 16) &&
        (playerPos.y >= pos.y + 16 - collisionThreshold);
    return horizontalOverlap && verticalCollision;
}


void Platform1::moveWithCharacter(float deltaY, Player& player) {
    pos.y += deltaY;
    glm::vec2 playerPos = player.getPosition();
    if (sprite) {
        sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
        player.setPosition(glm::vec2(playerPos.x, playerPos.y + deltaY));
        player.setPlatform(true);  

    }
}


void Platform1::returnToOriginalPosition(float deltaTime) {
    float dt = deltaTime / 1000.f;
    float returnSpeed = 50.0f; 
    if (pos.y > originalPos.y) {
        pos.y -= returnSpeed * dt;
        if (pos.y < originalPos.y)
            pos.y = originalPos.y;
    }
    else if (pos.y < originalPos.y) {
        pos.y += returnSpeed * dt;
        if (pos.y > originalPos.y)
            pos.y = originalPos.y;
    }
    if (sprite) {
        sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

    }
}


glm::vec2 Platform1::getPosition() const {
    return pos;
}
