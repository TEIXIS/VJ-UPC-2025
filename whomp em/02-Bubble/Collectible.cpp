#include "Collectible.h"
#include <iostream>

Collectible::~Collectible() {
    if (sprite) delete sprite;
}

void Collectible::init(const glm::vec2& position, const glm::ivec2& tileMapOffset,
    ShaderProgram& shader, const std::string& textureFile) {
    pos = position;
    tileMapDispl = tileMapOffset;

    if (!spritesheet.loadFromFile(textureFile, TEXTURE_PIXEL_FORMAT_RGBA)) {
        std::cerr << "Error al cargar textura: " << textureFile << std::endl;
    }

    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f, 1.f), &spritesheet, &shader);
    sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
}

void Collectible::update(int deltaTime) {
    if (!collected && sprite)
        sprite->update(deltaTime);
}

void Collectible::render() {
    if (!collected && sprite)
        sprite->render();
}

void Collectible::destroy() {
    collected = true;
}

bool Collectible::isCollected() const {
    return collected;
}

glm::vec2 Collectible::getPosition() const {
    return pos;
}

bool Collectible::collidesWith(const Player& player) const {
    glm::vec2 playerPos = player.getPosition();
    glm::ivec2 playerSize(32, 32);  // Ajustar según el tamaño real del jugador
    glm::ivec2 colSize(16, 16);     // Tamaño base del collectible

    bool colX = playerPos.x + playerSize.x > pos.x &&
        playerPos.x < pos.x + colSize.x;

    bool colY = playerPos.y + playerSize.y > pos.y &&
        playerPos.y < pos.y + colSize.y;

    return colX && colY;
}

void Collectible::applyGravity(int deltaTime, TileMap* map) {
    if (collected || !sprite) return;

    float dt = deltaTime / 1000.f;
    velocity.y += gravity * dt;

    glm::vec2 nextPos = pos;
    nextPos.y += velocity.y * dt;

    int tempY = int(nextPos.y + 16); // Simula el borde inferior
    int* ptrY = &tempY;

    bool grounded = map->collisionMoveDown(nextPos, glm::ivec2(16, 16), ptrY);
    if (!grounded) {
        pos.y += velocity.y * dt;
    }
    else {
        //pos.y = float(*ptrY) - 16; // Alinear el collectible con el suelo
        velocity.y = 0.f;
    }

    sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
}
