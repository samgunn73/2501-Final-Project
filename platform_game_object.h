#ifndef PLATFORM_H_
#define PLATFORM_H_



#include "game_object.h"

namespace game {

    class PlatformGameObject : public GameObject {
    public:
        // Constructor
        PlatformGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
        bool IsPlayerOnTop() {return player_ontop;}
        void PlayerOnTop() { player_ontop = true; }
        void NotPlayerOnTop() { player_ontop = false; }
    private:
        bool player_ontop;
    };
}

#endif