#include "Boss.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include <irrKlang.h>

#define PI 3.14159f

void Boss::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    spritesheet.loadFromFile("images/boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25f, 1.0f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(3);
    sprite->setAnimationSpeed(0, 2.f);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.25f, 0.f));
    sprite->changeAnimation(0);
    soundEngine = irrklang::createIrrKlangDevice();
	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(2, 8);
	sprite->addKeyframe(2, glm::vec2(0.75f, 0.f));

    tileMapDispl = tileMapPos;
    position = glm::vec2(3750, 100);
    velocity = glm::vec2(2.0f, -2.0f);
    position = glm::vec2(3750, 100);
    infinityCenter = position; 
    inmortalTimer = 0;
    health = 8.0f;
    alive = true;
    attackCooldown = 0;
    phaseTimer = 0;
    timeAccum = 0.f;
    currentPhase = PHASE_INFINITE;
}

void Boss::update(int deltaTime, Player& player) {
    if (!alive) return;

    sprite->update(deltaTime);
    timeAccum += deltaTime / 1000.f;
    phaseTimer += deltaTime;
    attackCooldown -= deltaTime;
	inmortalTimer -= deltaTime;
	frameCount++;

    if (phaseTimer >= 8000) {
        float distanceToCenter = glm::distance(position, infinityCenter);


        if (distanceToCenter <= 5.f) {
            currentPhase = (currentPhase == PHASE_INFINITE) ? PHASE_BOUNCE : PHASE_INFINITE;
            phaseTimer = 0;

            if (currentPhase == PHASE_BOUNCE) {
                velocity = glm::vec2(2.5f, -2.0f);
                sprite->changeAnimation(1);
            }
            else {
                sprite->changeAnimation(0);
            }
			soundEngine->play2D("media/fire.wav", false);
        }
    }


    if (currentPhase == PHASE_INFINITE) {

        float a = 100.0f;
        float t = timeAccum;
        float x = a * cos(t) / (1 + sin(t) * sin(t));
        float y = a * sin(t) * cos(t) / (1 + sin(t) * sin(t));
        position = infinityCenter + glm::vec2(x, y);

    }
    else if (currentPhase == PHASE_BOUNCE) {
        position += velocity;

        if (position.x <= 3620 || position.x >= 3800) velocity.x *= -1;
        if (position.y <= 20 || position.y >= 150) velocity.y *= -1;
		if (position.y >= 140) sprite->changeAnimation(2);
		else sprite->changeAnimation(1);
    }

    sprite->setPosition(position);


    glm::vec2 playerPos = player.getPosition();
    glm::vec2 size = glm::vec2(32, 32);
    glm::vec2 playerSize = glm::vec2(32, 32);
    bool colX = position.x + size.x > playerPos.x && position.x < playerPos.x + playerSize.x;
    bool colY = position.y + size.y > playerPos.y && position.y < playerPos.y + playerSize.y;

    if (colX && colY && attackCooldown <= 0) {
        if (!player.getCapaActiva() && !player.playerIsPlorant() && !player.isGod() && inmortalTimer <= 0) {
            player.setPlorantTimer();
            player.takeDamage(1.0f);
        }
    }
}

void Boss::render() {
    if (alive) {
        if (inmortalTimer <= 0 || frameCount % 3 == 0) {
            sprite->render();
        }
    }
}

void Boss::setTileMap(TileMap* tileMap) {
    map = tileMap;
}

void Boss::setPosition(const glm::vec2& pos) {
    position = pos;
    sprite->setPosition(pos);
}

glm::vec2 Boss::getPosition() const {
    return position;
}

bool Boss::isAlive() const {
    return alive;
}

void Boss::takeDamage(float dmg) {
    health -= dmg;
	inmortalTimer = 1000;
    if (health <= 0.f) alive = false;
}

int Boss::getCooldown() const {
	return inmortalTimer;
}

float Boss::getHealth() const {
	return health;
}