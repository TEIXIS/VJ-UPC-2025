#include "Mag.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>

#define MOVE_SPEED 1


enum MagAnims {
	caminarEsquerra, caminarDreta, tirarFocDepeuEsquerra, tirarFocDepeuDreta, escutEsquerra, escutDreta,
    tirarFocAjupitEsquerra, tirarFocAjupitDreta, COUNT };

void Mag::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{

    spritesheet.loadFromFile("images/enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
    groundTimer = 0;
    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(COUNT);

    sprite->setAnimationSpeed(caminarEsquerra, 8);
    sprite->addKeyframe(0, glm::vec2(0.0f, 0.0625f));
	sprite->addKeyframe(0, glm::vec2(0.125f, 0.0625f));

	sprite->setAnimationSpeed(caminarDreta, 8);
    sprite->addKeyframe(1, glm::vec2(0.875f, 0.1875));
	sprite->addKeyframe(1, glm::vec2(0.750f, 0.1875f));
    


    sprite->changeAnimation(caminarEsquerra);

    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

    movingRight = false;
    attacking = false;
    
    bJumping = false;
    

    vida = 1;
}

void Mag::update(int deltaTime)
{
    if (vida <= 0) return;
    
    sprite->update(deltaTime);
 

    if (map->collisionMoveRight(posEnemy, glm::ivec2(32, 32)))
    {
        posEnemy.x -= MOVE_SPEED;
        movingRight = false;
    }
    else if (map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32)) || posEnemy.x == 0)
    {
        posEnemy.x += MOVE_SPEED;
        movingRight = true;
    }
    else if (movingRight)
    {
        posEnemy.x += MOVE_SPEED;
		if (sprite->animation() != caminarDreta)
			sprite->changeAnimation(caminarDreta);
    }
    else
    {
		if (sprite->animation() != caminarEsquerra)
			sprite->changeAnimation(caminarEsquerra);
        posEnemy.x -= MOVE_SPEED;
    }

    


    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
    
}

void Mag::render()
{
    if (vida <= 0) return;
    else
        sprite->render();
}

void Mag::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Mag::setPosition(const glm::vec2& pos)
{
    posEnemy = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::vec2 Mag::getPosition() const
{
    return posEnemy;
}

void Mag::restarVida()
{
    if (vida > 0)
        vida--;
    if (vida == 0) {
        std::cout << "Enemy defeated!" << std::endl;
        vida--;
    }
}


