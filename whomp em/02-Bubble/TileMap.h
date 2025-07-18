#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <set>




class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:

	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	void update(float deltatime);
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool collisionLava(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool isOnLadder(const glm::ivec2& pos, const glm::ivec2& size) const;
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	glm::vec2 lastMinCoords;
	ShaderProgram* lastProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	set<int> collisions = {1,2,3,4,5,6,7,8,9,10,11,13,14,15,18,19,20,21,37,38,39,40,41,42,42,44,45,46,59,60,65,68,73,74,75,81,82,84,85,90,97,98,99,100};
	set<int> lava = {18,19,97,98,99,100};
	set<int> ladder = {67,76,77};

};


#endif // _TILE_MAP_INCLUDE


