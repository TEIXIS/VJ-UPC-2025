#ifndef _HUD_INCLUDE
#define _HUD_INCLUDE


// HUD.h

#include "Texture.h"
#include "Sprite.h"
#include "Player.h"
#include <GLFW/glfw3.h>


class HUD {
public:
    HUD();
    void init(ShaderProgram& shaderProgram, Player* player);
    void render();
	void update(int deltaTime);
    void setBoss(bool a);
    void setHBoss(float h);

private:
    ShaderProgram* shader;
    Player* player;

    Texture heartTexture;
    Sprite* heartSprite;

	Texture bHeartTexture;
	Sprite* bHeartSprite;

    Texture weaponTexture;
    Sprite* weaponSprite;

    Texture lampTexture;
    Sprite* lampSprite;

    Texture capaTexture;
	Sprite* capaSprite;

    bool showBoss = false;
	float hBoss = 8.f;
};



#endif