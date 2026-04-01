#include "platform_game_object.h"

namespace game {

    PlatformGameObject::PlatformGameObject(const glm::vec3& position,
        Geometry* geometry, Shader* shader, GLuint texture)
        : GameObject(position, geometry, shader, texture){
        player_ontop = false;
    }


}