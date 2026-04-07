#pragma once

#include "game_object.h"


namespace game {

    class ObjectiveGameObject : public GameObject {
    public:
        // Constructor
        ObjectiveGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
   
    };

}