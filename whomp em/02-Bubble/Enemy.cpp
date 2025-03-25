#include "Enemy.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>

void Enemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    bJumping = false;
    spritesheet.loadFromFile("images/enemy.png", TEXTURE_PIXEL_FORMAT_RGBA);
        
    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.0f));
    sprite->changeAnimation(0);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}


void Enemy::update(int deltaTime)
{
    sprite->update(deltaTime);
    // Lógica de movimiento y comportamiento del enemigo
}

void Enemy::render()
{
    sprite->render();
}

void Enemy::setTileMap(TileMap *tileMap)
{
    map = tileMap;
}

void Enemy::setPosition(const glm::vec2 &pos)
{
    posEnemy = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}
