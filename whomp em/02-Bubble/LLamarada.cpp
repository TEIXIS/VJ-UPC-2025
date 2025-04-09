#include "Llamarada.h"
#include <iostream>

Llamarada::Llamarada() {}

Llamarada::~Llamarada() {
    if (sprite) delete sprite;
}

void Llamarada::init(const glm::vec2& position, const glm::ivec2& tileMapOffset, ShaderProgram& shader, Direction dir) {
    pos = position;
    tileMapDispl = tileMapOffset;
    direction = dir;

    loadTexture(dir);

    if (dir == UP || dir == DOWN)size = glm::ivec2(16, 32);
    else size = glm::ivec2(32, 16); 
    sprite = Sprite::createSprite(size, glm::vec2(0.25f, 1.0f), &spritesheet, &shader);
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(0, 8); 
    for (int i = 0; i < totalFrames; ++i) {
        sprite->addKeyframe(0, glm::vec2(i * 0.25f, 0.f));
    }

    sprite->changeAnimation(0);
    sprite->setKeyframe(0); 
    sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

    phase = INVISIBLE;
    elapsedTime = 0.f;
}

void Llamarada::loadTexture(Direction dir) {
    std::string path;
    switch (dir) {
    case UP: path = "images/FUp.png"; break;
    case DOWN: path = "images/FDown.png"; break;
    case LEFT: path = "images/FLeft.png"; break;
    case RIGHT: path = "images/FRight.png"; break;
    }

    if (!spritesheet.loadFromFile(path, TEXTURE_PIXEL_FORMAT_RGBA)) {
        std::cerr << "No se pudo cargar la textura: " << path << std::endl;
    }
}

void Llamarada::update(int deltaTime) {
    float dt = deltaTime / 1000.f;
    elapsedTime += dt;

    updateAnimationFrame();

    sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
}

void Llamarada::updateAnimationFrame() {
    if (phase == INVISIBLE) {
        if (elapsedTime >= invisibleTime) {
            phase = CHARGING;
            elapsedTime = 0.f;
            sprite->setKeyframe(1);
        }
    }
    else if (phase == CHARGING) {
        if (elapsedTime >= 1.0f) {
            phase = ACTIVE;
            elapsedTime = 0.f;
        }
    }
    else if (phase == ACTIVE) {
        int activeFrame = (int(elapsedTime * 2)) % 2; 
        sprite->setKeyframe(2 + activeFrame); 

        if (elapsedTime >= 3.0f) {
            phase = INVISIBLE;
            elapsedTime = 0.f;
            sprite->setKeyframe(0); 
        }
    }
}

bool Llamarada::collidesWithPlayer(const Player& player) const {
    if (phase != ACTIVE) return false;

    glm::vec2 playerPos = player.getPosition();
    glm::ivec2 playerSize(32, 32); 

    glm::vec2 flamePos = pos;
    glm::ivec2 flameSize = size;

    bool overlapX = playerPos.x + playerSize.x > flamePos.x &&
        playerPos.x < flamePos.x + flameSize.x;

    bool overlapY = playerPos.y + playerSize.y > flamePos.y &&
        playerPos.y < flamePos.y + flameSize.y;

    return overlapX && overlapY;
}

void Llamarada::setInvisibleTime(float seconds) {
    invisibleTime = seconds;
}

void Llamarada::reset() {
    phase = INVISIBLE;
    elapsedTime = 0.f;
    sprite->setKeyframe(0);
}

void Llamarada::render() {
	if (sprite) {
		sprite->render();
	}
}
