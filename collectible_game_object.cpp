#include "collectible_game_object.h"

namespace game {

    CollectibleGameObject::CollectibleGameObject(const glm::vec3& position,
        Geometry* geometry, Shader* shader, GLuint texture)
        : GameObject(position, geometry, shader, texture),
        collected_(false) {
    }

    bool CollectibleGameObject::IsCollected() const {
        return collected_;
    }

    void CollectibleGameObject::Collect() {
        collected_ = true;
    }

}