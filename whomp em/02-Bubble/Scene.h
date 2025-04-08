#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Seta.h"
#include "Fenix.h"
#include "Hud.h"
#include "Mag.h"
#include "Platform1.h"
#include "Platform2.h"
#include "Collectible.h"
#include "LittleHeart.h"
#include "BigHeart.h"
#include "Calabaza.h"
#include "Lampara.h"
#include "Capa.h"
#include "LLamarada.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void resetLevel();

private:
	TileMap *map;
	Player *player;
	//Seta* seta;
	vector<Seta*> setas;
	//Fenix* fenix;
	vector<Fenix*> fenixes;
	Mag* mag;
	Mag* mag2;
	LittleHeart* cor1;
	BigHeart* cor2;
	Calabaza* calabaza1;
	Lampara* lamp;
	Capa* capa;
	Llamarada* flam;
	vector<Platform1*> plataformas1;
	vector<Platform2*> plataformas2;
	HUD* hud;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::mat4 view;
	Sprite* pantallaTitol;
	bool jocComencat;
	Texture spritesheet;
	bool s = false;
};


#endif // _SCENE_INCLUDE

