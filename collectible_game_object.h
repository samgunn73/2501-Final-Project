#pragma once

#include "game_object.h"

namespace game {

    class CollectibleGameObject : public GameObject {
    public:
        // Constructor
        CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
        
        bool IsCollected() const;
        void Collect();

    private:
        bool collected_;
    };

}
