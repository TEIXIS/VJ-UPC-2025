#include "Seta.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>

#define MOVE_SPEED 2
#define JUMP_ANGLE_STEP 8
#define JUMP_HEIGHT 16
#define FALL_STEP 5

enum SetaAnims { terraDreta, saltantDreta, terraEsquerra, saltantEsquerra };

void Seta::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    bJumping = false;
    spritesheet.loadFromFile("images/enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
    groundTimer = -1;
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
    movingRight = false;
}

void Seta::update(int deltaTime)
{
    if (vida <= 0) return;

    //cout << abs(posEnemy.y - posPlayer.y) << endl;
    if (abs(posEnemy.x - posPlayer.x) > 200 || abs(posEnemy.y - posPlayer.y)>180) {
        vida = 0;

    }

    sprite->update(deltaTime);

    if (bJumping)
    {
		//cout << "Jumping" << endl;
        jumpAngle += JUMP_ANGLE_STEP;
		//cout << jumpAngle << endl;
        if (jumpAngle == 184)
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
        /*posEnemy.y += FALL_STEP;
        if (map->collisionMoveDown(posEnemy, glm::ivec2(32, 32), &posEnemy.y)) {
            
            bJumping = true;
            jumpAngle = 0;
            startY = posEnemy.y;
            
        }*/

        
        


        if (groundTimer > 0)
        {
            groundTimer -= deltaTime;
            posEnemy.y += FALL_STEP;
            if (map->collisionMoveDown(posEnemy, glm::ivec2(16, 16), &posEnemy.y))
            {
                jumpAngle = 0;
                startY = posEnemy.y;
            }
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
	//cout << "posEnemy: " << posEnemy.x << ", " << posEnemy.y << endl;
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

    // Update animation based on state
    if (movingRight)
    {
        sprite->changeAnimation(bJumping ? saltantDreta : terraDreta);
    }
    else
    {
        sprite->changeAnimation(bJumping ? saltantEsquerra : terraEsquerra);
    }

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Seta::render()
{
    if (vida <= 0) return;
    sprite->render();
}

void Seta::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Seta::setPosition(const glm::vec2& pos)
{
    posEnemy = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::vec2 Seta::getPosition() const
{
    return posEnemy;
}

void Seta::restarVida()
{
    if (vida > 0)
        vida--;
    if (vida <= 0) {
        std::cout << "Enemy defeated!" << std::endl;
        posEnemy.x = -100;
    }
}
void Seta::spawn(int x, int y)
{
    if (vida <= 0) {
        setPosition(glm::vec2((x) * 16, (y) * 16));
        cout << "Seta spawned" << endl;
        //posEnemy = glm::vec2(16, 16);
        vida = 1;
        
    }
}
void Seta::getPosPlayer(glm::vec2 pos)
{
    posPlayer = pos;
}