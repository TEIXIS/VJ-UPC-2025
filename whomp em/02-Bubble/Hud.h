#ifndef _HUD_INCLUDE
#define _HUD_INCLUDE


// HUD.h

#include "Texture.h"
#include "Sprite.h"
#include "Player.h"

class HUD {
public:
    HUD();
    void init(ShaderProgram& shaderProgram, Player* player);
    void render();

private:
    ShaderProgram* shader;
    Player* player;

    Texture heartTexture;
    Sprite* heartSprite;

    Texture weaponTexture;
    Sprite* weaponSprite;
};



#endif