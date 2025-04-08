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
    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(COUNT);

    sprite->setAnimationSpeed(caminarEsquerra, 8);
    sprite->addKeyframe(0, glm::vec2(0.0f, 0.0625f));
	sprite->addKeyframe(0, glm::vec2(0.125f, 0.0625f));

	sprite->setAnimationSpeed(caminarDreta, 8);
    sprite->addKeyframe(1, glm::vec2(0.875f, 0.1875f));
	sprite->addKeyframe(1, glm::vec2(0.750f, 0.1875f));

	sprite->setAnimationSpeed(tirarFocDepeuEsquerra, 8);
	sprite->addKeyframe(2, glm::vec2(0.250f, 0.0625f));
	sprite->addKeyframe(2, glm::vec2(0.375f, 0.0625f));

	sprite->setAnimationSpeed(tirarFocDepeuDreta, 8);
	sprite->addKeyframe(3, glm::vec2(0.625f, 0.1875f));
	sprite->addKeyframe(3, glm::vec2(0.500f, 0.1875f));

	sprite->setAnimationSpeed(escutEsquerra, 8);
	sprite->addKeyframe(4, glm::vec2(0.500f, 0.0625f));
	sprite->addKeyframe(4, glm::vec2(0.625f, 0.0625f));

	sprite->setAnimationSpeed(escutDreta, 8);
	sprite->addKeyframe(5, glm::vec2(0.375f, 0.1875f));
	sprite->addKeyframe(5, glm::vec2(0.250f, 0.1875f));

	sprite->setAnimationSpeed(tirarFocAjupitEsquerra, 2);
	sprite->addKeyframe(6, glm::vec2(0.750f, 0.0625f));
	sprite->addKeyframe(6, glm::vec2(0.875f, 0.0625f));

	sprite->setAnimationSpeed(tirarFocAjupitDreta, 2);
	sprite->addKeyframe(7, glm::vec2(0.125f, 0.1875f));
	sprite->addKeyframe(7, glm::vec2(0.0f, 0.1875f));
    

	projectile = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.03125, 0.03125), &spritesheet, &shaderProgram);
	projectile->setNumberAnimations(2);
	
    projectile->setAnimationSpeed(0, 8);
	projectile->addKeyframe(0, glm::vec2(0.25f, 0.0f));
	projectile->addKeyframe(0, glm::vec2(0.28125f, 0.0f));

	projectile->setAnimationSpeed(1, 8);
	projectile->addKeyframe(1, glm::vec2(0.25f, 0.03125f));
	projectile->addKeyframe(1, glm::vec2(0.28125f, 0.03125f));



	projectile->changeAnimation(0);
    sprite->changeAnimation(caminarEsquerra);

    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	posProjectile = glm::vec2(float(-100), float(tileMapDispl.y + posEnemy.y));
    movingRight = false;
    attacking = false;
    invulnerable = false;
    invulnerableTimer = 0;
    bJumping = false;
    
	atacTimer = 0;
    vida = 0;
}

void Mag::update(int deltaTime)
{
    if (vida <= 0) return;

    if (invulnerable) {
        invulnerableTimer -= deltaTime;
        if (invulnerableTimer <= 0) {
            invulnerable = false;
            invulnerableTimer = 0;
        }
    }

	//cout << abs(posEnemy.x - posPlayer.x) << endl;

    if (abs(posEnemy.x - posPlayer.x) > 200) {
        vida = 0;

    }
    
    sprite->update(deltaTime);
	projectile->update(deltaTime);

    if (atacTimer > 0)
    {
        atacTimer -= deltaTime;

        if (atacTimer <= 1500) {
            if (projectilDreta) {
                posProjectile.x += 3;
            }
            else {
                posProjectile.x -= 3;
            }
        }
        

        if (atacTimer <= 1000) {

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
        }
        
    }
    else
    {
        atacTimer = 2000;
        posProjectile = posEnemy;
        posProjectile.y += 16;
        if (posEnemy.x <= posPlayer.x) {
			movingRight = true;
			projectilDreta = true;
			if (sprite->animation() != tirarFocAjupitDreta)
				sprite->changeAnimation(tirarFocAjupitDreta);
			projectile->changeAnimation(1);
        }
        else {
			movingRight = false;
			projectilDreta = false;
			if (sprite->animation() != tirarFocAjupitEsquerra)
				sprite->changeAnimation(tirarFocAjupitEsquerra);
			projectile->changeAnimation(0);
        }

    }
 

    

    

	projectile->setPosition(glm::vec2(float(tileMapDispl.x + posProjectile.x), float(tileMapDispl.y + posProjectile.y)));
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
    
}

void Mag::render()
{
	
    if (vida <= 0) return;
    else
        sprite->render();
    if (posProjectile.x <= posPlayer.x + 450 && posProjectile.x >= posPlayer.x - 210 && atacTimer <= 1500) projectile->render();
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
    if (!invulnerable && vida > 0) {
        vida--;
        if (vida == 0) {
            std::cout << "Enemy defeated!" << std::endl;
			posEnemy = glm::vec2(-100, -100);
			posProjectile = glm::vec2(-100, -100);
			spawnB = false;
        }
        else {
            invulnerable = true;
            invulnerableTimer = 500; // 0.5 segundos
        }
    }
}

void Mag::getPosPlayer(glm::vec2 pos)
{
    posPlayer = pos;
}

glm::vec2 Mag::getPosProjectile()
{
	return posProjectile;
}

void Mag::spawn(int x, int y)
{
	if (vida <= 0) {
        setPosition(glm::vec2((x) * 16, (y) * 16));
        cout << "Mag spawned" << endl;
		//posEnemy = glm::vec2(16, 16);
		vida = 3;
		spawnB = true;
	}
}

