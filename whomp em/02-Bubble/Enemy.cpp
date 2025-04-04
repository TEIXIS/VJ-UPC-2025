#include "Enemy.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>

#define MOVE_SPEED 1
#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 25
#define FALL_STEP 5

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
    movingRight = true;
}


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
    sprite->update(deltaTime);

    if (bJumping)
    {
        jumpAngle += JUMP_ANGLE_STEP;
        if (jumpAngle == 180)
        {
            bJumping = false;
            posEnemy.y = startY;
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
                }
            }
            else
            {
                bJumping = !map->collisionMoveDown(posEnemy, glm::ivec2(16, 16), &posEnemy.y);
            }
        }
    }
    else
    {
        // Apply gravity
        posEnemy.y += FALL_STEP;
        if (map->collisionMoveDown(posEnemy, glm::ivec2(16, 16), &posEnemy.y))
        {
            bJumping = true;
            jumpAngle = 0;
            startY = posEnemy.y;
        }
    }

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

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
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
