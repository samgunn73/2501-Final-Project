#include "collectible_game_object.h"

namespace game {

    CollectibleGameObject::CollectibleGameObject(const glm::vec3& position,
        Geometry* geometry, Shader* shader, GLuint texture,CollectibleType type)
        : GameObject(position, geometry, shader, texture),
        collected_(false),type_(type) {
    }

    bool CollectibleGameObject::IsCollected() const {
        return collected_;
    }

    void CollectibleGameObject::Collect() {
        collected_ = true;
    }

}