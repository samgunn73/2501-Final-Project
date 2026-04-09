#pragma once

#include "game_object.h"

const enum CollectibleType {
    INVINCIBILITY = 0,
    AMMO = 1,
    HEART = 2,
    CRAB = 3
};

namespace game {

    class CollectibleGameObject : public GameObject {
    public:
        // Constructor
        CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture,CollectibleType type);
        
        bool IsCollected() const;
        void Collect();
        void SetType(int type) { type_ = static_cast<CollectibleType>(type); }
        int GetType() { return static_cast<int>(type_); }

    private:
        bool collected_;
        CollectibleType type_;
    };

}
