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
#include "Boss.h"
#include <irrKlang.h>
using namespace irrklang;

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
	void spawnRandomCollectible(const glm::vec2& position);

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
	Boss* boss;
	vector<Platform1*> plataformas1;
	vector<Platform2*> plataformas2;
	vector<Llamarada*> llamas;
	vector<Collectible*> collectibles;
	HUD* hud;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::mat4 view;
	Sprite* pantallaTitol;
	Sprite* pantallaControles;
	Sprite* pantallaWin;
	bool jocComencat = false;
	Texture spritesheet;           // Usado para el título
	Texture spritesheetControls; 
	Texture spritesheetWin;// Usado para los controles
	bool s = false;
	bool showControls = false;
	bool spacePressed = false;
	irrklang::ISoundEngine* soundEngine;
	bool win = false;
	int counterW = 1000;

};
#endif // _SCENE_INCLUDE

