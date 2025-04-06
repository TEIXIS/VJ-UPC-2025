#include "Fenix.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>

#define MOVE_SPEED 1


enum FenixAnims { volantEsquerra, volantDreta };

void Fenix::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    bJumping = false;
    attacking = true; // Comienza atacando
    spritesheet.loadFromFile("images/enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
    groundTimer = 0;
    sprite = Sprite::createSprite(glm::ivec2(32, 16), glm::vec2(0.125, 0.0625), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(2);
    sprite->setAnimationSpeed(volantEsquerra, 8);
    sprite->addKeyframe(volantEsquerra, glm::vec2(0.0f, 0.3125f));
	sprite->addKeyframe(volantEsquerra, glm::vec2(0.125f, 0.3125f));
	sprite->addKeyframe(volantEsquerra, glm::vec2(0.25f, 0.3125f));

	sprite->setAnimationSpeed(volantDreta, 8);
	sprite->addKeyframe(volantDreta, glm::vec2(0.375f, 0.3125f));
	sprite->addKeyframe(volantDreta, glm::vec2(0.5f, 0.3125f));
	sprite->addKeyframe(volantDreta, glm::vec2(0.625f, 0.3125f));

    sprite->changeAnimation(volantEsquerra);
    tileMapDispl = tileMapPos;
    //originalHeight = glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y));
    sprite->setPosition(originalHeight);
    movingRight = false;
	attacking = false;
	habaixat = false;
    vida = 1;
}

void Fenix::update(int deltaTime)
{
    if (vida <= 0) return;
    sprite->update(deltaTime);


	if (posEnemy.x <= posPlayer.x + 40 && !attacking) {
		if (posEnemy.y <= posPlayer.y - 5 && !habaixat) {
            posEnemy.y += MOVE_SPEED;
		}
		else habaixat = true;
		if (habaixat) {
			posEnemy.y -= MOVE_SPEED;
		}

        if (posEnemy.y <= originalHeight.y) {
			attacking = true;
        }
		
	}
	
    
    if (map->collisionMoveRight(posEnemy, glm::ivec2(16, 16)))
 	{
 		posEnemy.x -= MOVE_SPEED;
 		movingRight = false;
 	}
 	else if (map->collisionMoveLeft(posEnemy, glm::ivec2(16, 16)) || posEnemy.x==0)
 	{
 		posEnemy.x += MOVE_SPEED;
 		movingRight = true;
 	}
 	else if (movingRight)
 	{
 		posEnemy.x += MOVE_SPEED;
 	}
 	else
 	{	
 		posEnemy.x -= MOVE_SPEED;
 	}

	if (movingRight && sprite->animation()!= volantDreta)
		sprite->changeAnimation(volantDreta);
	else if (!movingRight && sprite->animation() != volantEsquerra)
		sprite->changeAnimation(volantEsquerra);

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Fenix::render()
{
    if (vida <= 0) return;
    sprite->render();
}

void Fenix::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Fenix::setPosition(const glm::vec2& pos)
{
    posEnemy = pos;
	originalHeight = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::vec2 Fenix::getPosition() const
{
    return posEnemy;
}

void Fenix::restarVida()
{
    if (vida > 0)
        vida--;
    if (vida <= 0) {
        std::cout << "Enemy defeated!" << std::endl;
        posEnemy.x = -100;
    }
}

void Fenix::getPosPlayer(glm::vec2 pos)
{
	posPlayer = pos;
}
