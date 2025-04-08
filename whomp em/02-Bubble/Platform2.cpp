#include "Platform2.h"
#include <iostream>

Platform2::Platform2() : sprite(nullptr), speed(250.0f), movingUp(true), lastDeltaY(0.0f) {}

Platform2::~Platform2() {
    if (sprite) delete sprite;
}

void Platform2::init(const glm::vec2& position, ShaderProgram& shaderProgram, const glm::ivec2& tileMapOffset, float maxOffset_) {
    pos = position;
    originalPos = position;
    tileMapDispl = tileMapOffset;
    maxOffset = maxOffset_;

    if (!spritesheet.loadFromFile("images/platform1.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
        std::cerr << "Error al cargar la textura de Platform2.\n";
    }

    sprite = Sprite::createSprite(glm::ivec2(24, 14), glm::vec2(1.0f, 1.0f), &spritesheet, &shaderProgram);
    sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
}

void Platform2::update(int deltaTime) {
    float dt = deltaTime / 1000.f;
    lastDeltaY = (movingUp ? -1 : 1) * speed * dt;

    pos.y += lastDeltaY;

    if (pos.y <= originalPos.y - maxOffset) {
        pos.y = originalPos.y - maxOffset;
        movingUp = false;
    }
    else if (pos.y >= originalPos.y + maxOffset) {
        pos.y = originalPos.y + maxOffset;
        movingUp = true;
    }

    if (sprite) {
        sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
    }
}





void Platform2::render() {
    if (sprite) sprite->render();
}

bool Platform2::checkCollisionFromAbove(const Player& player, float threshold) {
    glm::vec2 playerPos = player.getPosition();
    glm::ivec2 playerSize(32, 32);

    float playerBottom = playerPos.y + playerSize.y;
    float platformTop = pos.y;

    float extraMargin = 3.0f;

    bool horizontal = (playerPos.x + playerSize.x > pos.x) && (playerPos.x < pos.x + 32);
    bool vertical = (playerBottom >= platformTop - extraMargin) && (playerBottom <= platformTop + threshold);

    return horizontal && vertical;
}




bool Platform2::checkCollisionFromBelow(const Player& player, float collisionThreshold) {
    glm::vec2 playerPos = player.getPosition();
    glm::ivec2 playerSize(32, 32);

    bool horizontalOverlap = (playerPos.x + playerSize.x > pos.x) && (playerPos.x < pos.x + 32);
    bool verticalCollision = (playerPos.y <= pos.y + 16) &&
        (playerPos.y >= pos.y + 16 - collisionThreshold);

    return horizontalOverlap && verticalCollision;
}

void Platform2::moveWithCharacter(float deltaY, Player& player) {
    pos.y += deltaY;
    glm::vec2 playerPos = player.getPosition();
    player.setPosition(glm::vec2(playerPos.x, playerPos.y + deltaY));
    player.setPlatform(true);
}


glm::vec2 Platform2::getPosition() const {
    return pos;
}

float Platform2::getLastDeltaY() const {
    return lastDeltaY;
}

