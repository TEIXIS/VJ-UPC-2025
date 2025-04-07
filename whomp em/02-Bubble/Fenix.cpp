#include "Fenix.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>

#define MOVE_SPEED 1


enum FenixAnims { volantEsquerra, volantDreta };

void Fenix::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    
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

	foc = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	foc->setNumberAnimations(1);
	foc->setAnimationSpeed(0, 8);
	foc->addKeyframe(0, glm::vec2(0.75f, 0.3125f));
	foc->addKeyframe(0, glm::vec2(0.8125f, 0.3125f));
	foc->addKeyframe(0, glm::vec2(0.875f, 0.3125f));

    foc2 = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
    foc2->setNumberAnimations(1);
    foc2->setAnimationSpeed(0, 8);
    foc2->addKeyframe(0, glm::vec2(0.75f, 0.3125f));
    foc2->addKeyframe(0, glm::vec2(0.8125f, 0.3125f));
    foc2->addKeyframe(0, glm::vec2(0.875f, 0.3125f));


    sprite->changeAnimation(volantEsquerra);
    foc->changeAnimation(0);
    foc2->changeAnimation(0);

    tileMapDispl = tileMapPos;
    //originalHeight = glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y));
    sprite->setPosition(originalHeight);
    foc->setPosition(originalHeight);
    foc2->setPosition(originalHeight);
    movingRight = false;
	attacking = false;
	habaixat = false;
    bJumping = false;
    focActiu = false;

    vida = 0;
}

void Fenix::update(int deltaTime)
{
    if (vida <= 0 && !focActiu) return;

    //cout << abs(posEnemy.x - posPlayer.x) << endl;
    if (abs(posEnemy.x - posPlayer.x) > 190) {
        vida = 0;
		focActiu = false;
    }

    
    else if (vida <= 0 && focActiu) {
        if (groundTimer > 0) {
            groundTimer -= deltaTime;
            if (groundTimer <= 0) {
                focActiu = false;
				posFoc.x = -100;
            }
        }
        else {
            posFoc.x += 1;
            posFoc.y += 2;
            posFoc2.x -= 1;
            posFoc2.y += 2;

            if (map->collisionMoveDown(posFoc, glm::ivec2(16, 16), &posFoc.y)) {
                groundTimer = 1000;
            }
        }
    }
    sprite->update(deltaTime);
    foc->update(deltaTime);
    foc2->update(deltaTime);


	if (posEnemy.x <= posPlayer.x + 40 && !attacking) {
		if (posEnemy.y <= posPlayer.y - 5 && !habaixat) {
            posEnemy.y += MOVE_SPEED*2;
		}
		else habaixat = true;
		if (habaixat) {
			posEnemy.y -= MOVE_SPEED*2;
		}

        if (posEnemy.y <= originalHeight.y) {
			attacking = true;
        }
		
	}
	
    
    /*if (map->collisionMoveRight(posEnemy, glm::ivec2(16, 16)))
 	{
 		posEnemy.x -= MOVE_SPEED;
 		movingRight = false;
 	}
 	else if (map->collisionMoveLeft(posEnemy, glm::ivec2(16, 16)) || posEnemy.x==0)
 	{
 		posEnemy.x += MOVE_SPEED;
 		movingRight = true;
 	}*/
    if (posEnemy.x >= posPlayer.x + 175) {
        posEnemy.x -= MOVE_SPEED;
        movingRight = false;
    }
    else if (posEnemy.x <= posPlayer.x - 125) {
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
    foc->setPosition(glm::vec2(float(tileMapDispl.x + posFoc.x), float(tileMapDispl.y + posFoc.y)));
    foc2->setPosition(glm::vec2(float(tileMapDispl.x + posFoc2.x), float(tileMapDispl.y + posFoc2.y)));
}

void Fenix::render()
{
    if (vida <= 0 && !focActiu) return;
    else if (vida <=0 && focActiu) {
        foc->render();
        foc2->render();
    }
    else
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
    if (vida == 0) {
        std::cout << "Enemy defeated!" << std::endl;
        focActiu = true;
        posFoc = posEnemy;
        posFoc2 = posEnemy;
        vida--;
		posEnemy.x = -100;
    }
}

void Fenix::getPosPlayer(glm::vec2 pos)
{
	posPlayer = pos;
}

glm::vec2 Fenix::getPosFoc() const
{
    return posFoc;
}
void Fenix::spawn()
{
	if (vida <= 0) {
		vida = 1;
		spawnB = true;
		focActiu = false;
		posFoc.x = -100;
        habaixat = false;
		attacking = false;
        setPosition(glm::vec2((90) * 16, (93) * 16));
        cout << "Fenix spawned" << endl;
	}
}