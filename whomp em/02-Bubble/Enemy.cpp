#include "Enemy.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>

#define MOVE_SPEED 2
#define JUMP_ANGLE_STEP 8
#define JUMP_HEIGHT 16
#define FALL_STEP 5


enum EnemyAnims { terraDreta, saltantDreta, terraEsquerra, saltantEsquerra };

void Enemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    bJumping = false;
    spritesheet.loadFromFile("images/enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
    groundTimer = 0;
    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(4);

    sprite->setAnimationSpeed(terraDreta, 8);
    sprite->addKeyframe(terraDreta, glm::vec2(0.f, 0.0f));

    sprite->setAnimationSpeed(saltantDreta, 8);
    sprite->addKeyframe(saltantDreta, glm::vec2(0.0625f, 0.0f));

    sprite->setAnimationSpeed(terraEsquerra, 8);
    sprite->addKeyframe(terraEsquerra, glm::vec2(0.1875f, 0.0f));

    sprite->setAnimationSpeed(saltantEsquerra, 8);
    sprite->addKeyframe(saltantEsquerra, glm::vec2(0.125f, 0.0f));

    sprite->changeAnimation(0);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
    movingRight = true;
}

//moures sense saltar
//void Enemy::update(int deltaTime)
//{
//    sprite->update(deltaTime);
//    // Lógica de movimiento y comportamiento del enemigo
//
//	if (map->collisionMoveRight(posEnemy, glm::ivec2(16, 16)))
//	{
//		
//		posEnemy.x -= MOVE_SPEED;
//		movingRight = false;
//	}
//	else if (map->collisionMoveLeft(posEnemy, glm::ivec2(16, 16)) || posEnemy.x==0)
//	{
//		
//		posEnemy.x += MOVE_SPEED;
//		movingRight = true;
//	}
//	else if (movingRight)
//	{
//		
//		posEnemy.x += MOVE_SPEED;
//	}
//	else
//	{
//		
//		posEnemy.x -= MOVE_SPEED;
//	}
//
//
//    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
//}

void Enemy::update(int deltaTime)
{
	if (vida <= 0) return;
    sprite->update(deltaTime);

    if (bJumping)
    {
        jumpAngle += JUMP_ANGLE_STEP;
        if (jumpAngle == 180)
        {
            bJumping = false;
            posEnemy.y = startY;
            groundTimer = 1000;
        }
        else
        {
            posEnemy.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));

            if (jumpAngle < 90)
            {
                if (map->collisionMoveUp(posEnemy, glm::ivec2(16, 16), &posEnemy.y))
                {
                    bJumping = false;
                    jumpAngle = 180;
                    groundTimer = 1000;
                }
            }
            else
            {
                if (map->collisionMoveDown(posEnemy, glm::ivec2(16, 16), &posEnemy.y)) {
                    bJumping = false;
                    groundTimer = 1000;
                }
                else bJumping = true;
            }
        }
    }
    else
    {
        if (groundTimer > 0)
        {
            groundTimer -= deltaTime;
        }
        else
        {

            posEnemy.y += FALL_STEP;
            if (map->collisionMoveDown(posEnemy, glm::ivec2(16, 16), &posEnemy.y))
            {
                bJumping = true;
                jumpAngle = 0;
                startY = posEnemy.y;
            }
        }
    }
    if (groundTimer <= 0) {
        if (movingRight)
        {
            posEnemy.x += MOVE_SPEED;
            if (map->collisionMoveRight(posEnemy, glm::ivec2(16, 16)))
            {
                posEnemy.x -= MOVE_SPEED;
                movingRight = false;
            }
        }
        else
        {
            posEnemy.x -= MOVE_SPEED;
            if (map->collisionMoveLeft(posEnemy, glm::ivec2(16, 16)) || posEnemy.x == 0)
            {
                posEnemy.x += MOVE_SPEED;
                movingRight = true;
            }
        }
    }


    //if (checkCollision(player.getPosition(), glm::ivec2(32, 32)))
    //{
    //    std::cout << "Colision con el jugador!" << std::endl;
    //    // Manejar la colisión con el jugador
    //}


    //if (checkCollision(player.getLanzaPosition(), player.getLanzaSize()))
    //{
    //    std::cout << "Colision con la lanza!" << std::endl;
    //    // Manejar la colisión con la lanza
    //}

    if (movingRight)
    {
        if (!bJumping)
            sprite->changeAnimation(terraDreta);
        else
            sprite->changeAnimation(saltantDreta);
    }
    else
    {
        if (!bJumping)
            sprite->changeAnimation(terraEsquerra);
        else
            sprite->changeAnimation(saltantEsquerra);
    }


    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::render()
{
	if (vida <= 0) return;
    sprite->render();
}

void Enemy::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Enemy::setPosition(const glm::vec2& pos)
{
    posEnemy = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}
//
//bool Enemy::checkCollision(const glm::vec2& pos, const glm::ivec2& size) const
//{
//    glm::ivec2 enemySize(16, 16); // Tamaño del enemigo
//    return (posEnemy.x < pos.x + size.x &&
//        posEnemy.x + enemySize.x > pos.x &&
//        posEnemy.y < pos.y + size.y &&
//        posEnemy.y + enemySize.y > pos.y);
//}

glm::vec2 Enemy::getPosition() const
{
    return posEnemy;
}

void Enemy::restarVida()
{
	if (vida > 0)
        vida--;
	if (vida <= 0) {
		std::cout << "Enemy defeated!" << std::endl;
		posEnemy.x = -100; 
	}
}
