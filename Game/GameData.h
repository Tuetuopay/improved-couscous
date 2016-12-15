#ifndef _GAME_GAME_DATA_H
#define _GAME_GAME_DATA_H

#include <glm/glm.hpp>

namespace Game {

typedef struct {
	double dt, currentTime;

	glm::vec3 playerPos;
	double yaw, pitch;
} GameData;

}

#endif

