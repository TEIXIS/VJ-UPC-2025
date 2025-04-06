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

private:
	TileMap *map;
	Player *player;
	Seta* seta;
	Fenix* fenix;
	Mag* mag;
	HUD* hud;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::mat4 view;

};


#endif // _SCENE_INCLUDE

