#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;

struct AnimatedTile {
	glm::ivec2 pos;              // posición del tile (i, j)
	std::vector<int> frames;     // secuencia de tile IDs
	int currentFrame = 0;
	float timePerFrame = 0.4f;   // tiempo entre frames
	float timer = 0.0f;
};

vector<AnimatedTile> animatedTiles;



TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const std::string &levelFile)
{
	std::ifstream fin(levelFile.c_str());
	if (!fin.is_open()) {
		std::cout << "No se pudo abrir el archivo: " << levelFile << std::endl;
		return false;
	}

	std::string line;
	// Leer la primera línea: "TILEMAP"
	if (!std::getline(fin, line))
		return false;
	if (line != "TILEMAP")
		return false;

	// Leer la segunda línea: dimensiones del mapa (ancho, alto)
	if (!std::getline(fin, line))
		return false;
	{
		std::stringstream ss(line);
		std::string token;
		if (std::getline(ss, token, ','))
			mapSize.x = std::stoi(token);
		if (std::getline(ss, token, ','))
			mapSize.y = std::stoi(token);
	}

	// Leer la tercera línea: tamaño del tile y blockSize (16,16)
	if (!std::getline(fin, line))
		return false;
	{
		std::stringstream ss(line);
		std::string token;
		if (std::getline(ss, token, ','))
			tileSize = std::stoi(token);
		if (std::getline(ss, token, ','))
			blockSize = std::stoi(token);
	}

	// Leer la cuarta línea: ruta al tilesheet
	if (!std::getline(fin, line))
		return false;
	std::string tilesheetFile = line;
	std::cout << tilesheetFile << " \n";
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	// Leer la quinta línea: dimensiones del tilesheet (columnas, filas)
	if (!std::getline(fin, line))
		return false;
	{
		std::stringstream ss(line);
		std::string token;
		if (std::getline(ss, token, ','))
			tilesheetSize.x = std::stoi(token);
		if (std::getline(ss, token, ','))
			tilesheetSize.y = std::stoi(token);
	}
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	// Reservar memoria para el mapa
	int totalTiles = mapSize.x * mapSize.y;
	map = new int[totalTiles];

	// Leer los datos del mapa: una línea por cada fila, con los valores separados por comas.
	for (int j = 0; j < mapSize.y; j++) {
		if (!std::getline(fin, line))
			break;  // O retornar false si se requiere tener todas las filas
		std::stringstream ss(line);
		for (int i = 0; i < mapSize.x; i++) {
			std::string token;
			if (!std::getline(ss, token, ','))
				break;
			int tile = std::stoi(token);
			map[j * mapSize.x + i] = tile;
			//std::cout << map[j * mapSize.x + i] << " ";
		}
	}

	for (int j = 0; j < mapSize.y; ++j) {
		for (int i = 0; i < mapSize.x; ++i) {
			int index = j * mapSize.x + i;
			int tile = map[index];

			if (tile == 18) {
				map[index] = 97;  // Cambiar inmediatamente el tile en el mapa

				AnimatedTile anim;
				anim.pos = { i, j };
				anim.frames = { 97, 99 };
				animatedTiles.push_back(anim);
			}
			else if (tile == 19) {
				map[index] = 98;

				AnimatedTile anim;
				anim.pos = { i, j };
				anim.frames = { 98, 100 };
				animatedTiles.push_back(anim);
			}
		}
	}




	fin.close();
	return true;
}

void TileMap::update(float deltaTime)
{
	bool changed = false;

	for (auto& tile : animatedTiles) {
		tile.timer += deltaTime;
		if (tile.timer >= tile.timePerFrame) {
			tile.timer = 0.0f;
			tile.currentFrame = (tile.currentFrame + 1) % tile.frames.size();

			int index = tile.pos.y * mapSize.x + tile.pos.x;
			map[index] = tile.frames[tile.currentFrame];

			/*std::cout << "Tile en (" << tile.pos.x << ", " << tile.pos.y << ") cambia a "
				<< tile.frames[tile.currentFrame] << std::endl;*/

			changed = true;
		}
	}


	if (changed) {
		//std::cout << "Recalculando vértices para tiles animados\n";
		prepareArrays(lastMinCoords, *lastProgram);
	}

}





void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	// Al comienzo de prepareArrays
	lastMinCoords = minCoords;
	lastProgram = &program;

	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (vertices.empty()) std::cout << "F\n";
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(collisions.find(map[y*mapSize.x+x]) != collisions.end())
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(collisions.find(map[y*mapSize.x + x]) != collisions.end())
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(collisions.find(map[y*mapSize.x + x]) != collisions.end())
		{
			if(*posY - tileSize * y + size.y <= 16)
			{
				*posY = tileSize * y - size.y;
				if (lava.find(map[y * mapSize.x + x]) != lava.end()) {
					std::cout << "lava\n";
				}
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;

	for (int x = x0; x <= x1; x++)
	{
		if (collisions.find(map[y * mapSize.x + x]) != collisions.end())
		{
			if (tileSize * (y + 1) - *posY <= 16) // margen de 16 px, opcional
			{
				*posY = tileSize * (y + 1);
				return true;
			}
		}
	}

	return false;
}































