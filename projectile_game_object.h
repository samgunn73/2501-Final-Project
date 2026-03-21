#ifndef PROJECTILE_GAME_OBJECT_H_
#define PROJECTILE_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    class ProjectileGameObject : public GameObject {
    public:
        ProjectileGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        void Update(double delta_time) override;

        void SetVelocity(const glm::vec3& velocity);
        glm::vec3 GetVelocity() const;

        glm::vec3 GetPreviousPosition() const;

        bool IsExpired() const;

    private:
        glm::vec3 velocity_;
        glm::vec3 previous_position_;

        float lifetime_;        // time since created
        float max_lifetime_;    // time before destruction
    };

}

#endif