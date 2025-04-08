#include "Hud.h"
#include "Game.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

HUD::HUD() {

}

enum HeartAnims
{
    FULL, MFULL, MEMPTY, EMPTY
};

void HUD::init(ShaderProgram& shaderProgram, Player* p)
{
    shader = &shaderProgram;
    player = p;

    heartTexture.loadFromFile("images/HUD.png", TEXTURE_PIXEL_FORMAT_RGBA);
    weaponTexture.loadFromFile("images/HUD.png", TEXTURE_PIXEL_FORMAT_RGBA); // arma actual
    lampTexture.loadFromFile("images/lamp.png", TEXTURE_PIXEL_FORMAT_RGBA);
    capaTexture.loadFromFile("images/capa.png", TEXTURE_PIXEL_FORMAT_RGBA);
    heartSprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.25f, 0.25f), &heartTexture, shader);
    weaponSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.5f, 0.5f), &weaponTexture, shader);
    heartSprite->setNumberAnimations(4);
    heartSprite->setAnimationSpeed(FULL, 8);
    heartSprite->addKeyframe(FULL, glm::vec2(0.f, 0.f));
    heartSprite->setAnimationSpeed(MFULL, 8);
    heartSprite->addKeyframe(MFULL, glm::vec2(0.25f, 0.f));
    heartSprite->setAnimationSpeed(MEMPTY, 8);
    heartSprite->addKeyframe(MEMPTY, glm::vec2(0.5f, 0.f));
    heartSprite->setAnimationSpeed(EMPTY, 8);
    heartSprite->addKeyframe(EMPTY, glm::vec2(0.75f, 0.f));

    weaponSprite->setNumberAnimations(2);
    weaponSprite->setAnimationSpeed(0, 8);
    weaponSprite->addKeyframe(0, glm::vec2(0.f, 0.25f));
    
	weaponSprite->setAnimationSpeed(1, 8);
	weaponSprite->addKeyframe(1, glm::vec2(0.5f, 0.25f));

    weaponSprite->changeAnimation(0);

    lampSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f, 1.f), &lampTexture, shader);
	capaSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f, 1.f), &capaTexture, shader);
}

void HUD::update(int deltaTime)
{
	heartSprite->update(deltaTime);
	weaponSprite->update(deltaTime);

    static bool totemKey = false;

    if (Game::instance().getKey(GLFW_KEY_T)) {
        if (!totemKey) {
			cout << "canvi arma" << endl;
			if (weaponSprite->animation() == 0) {
				weaponSprite->changeAnimation(1);
			}
			else {
				weaponSprite->changeAnimation(0);
			}
            totemKey = true;
        }
    }
    else {
        totemKey = false;
    }
}

void HUD::render()
{

    int numHearts = player->getHmax();
    float health = player->getLives();
    vector<int> x(numHearts);
    for (int i = 0; i < numHearts; ++i) {
        if (health >= 1.f) {
            x[i] = 1;
            health -= 1;
        }
        else if (health >= 0.50f && health <= 0.75) {
            x[i] = 2;
            health = 0.f;
        }
        else if (health >= 0.25f && health <= 0.40) {
            x[i] = 3;
            health = 0.f;
        }
        else x[i] = 4;
    }
    for (int i = 0; i < numHearts; ++i) {
        heartSprite->setPosition(glm::vec2(10.f, 29.f + i * 8.f));
        if(x[i] == 1) heartSprite->changeAnimation(FULL);
        if (x[i] == 2) heartSprite->changeAnimation(MFULL);
        if (x[i] == 3) heartSprite->changeAnimation(MEMPTY);
        if (x[i] == 4) heartSprite->changeAnimation(EMPTY);
        heartSprite->render();
    }

    // arma al lado de los corazones (alineada al primero)
    weaponSprite->setPosition(glm::vec2(10.f, 10.f));
    lampSprite->setPosition(glm::vec2(28, 10.f));
     
    weaponSprite->render();
    
    int nLamps = player->getLamps();
    int i;
    for (i = 0; i < nLamps; ++i) {
        lampSprite->setPosition(glm::vec2(28+i*16, 10.f));
        lampSprite->render();
    }
	if (player->getCapaActiva()) {
		capaSprite->setPosition(glm::vec2(28 + i * 16, 10.f));
		capaSprite->render();
	}
}
