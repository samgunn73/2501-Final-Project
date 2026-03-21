#include "projectile_game_object.h"
#include <glm/gtx/rotate_vector.hpp>

namespace game {

    ProjectileGameObject::ProjectileGameObject(const glm::vec3& position,
        Geometry* geom,
        Shader* shader,
        GLuint texture)
        : GameObject(position, geom, shader, texture)
    {
        velocity_ = glm::vec3(0.0f);
        previous_position_ = position;

        lifetime_ = 0.0f;
        max_lifetime_ = 3.0f;   // projectile lives for 3 seconds
    }

    void ProjectileGameObject::Update(double delta_time)
    {
        // Store previous position (needed for ray-circle collision)
        previous_position_ = GetPosition();

        // Move projectile
        glm::vec3 new_pos = GetPosition() + velocity_ * (float)delta_time;
        SetPosition(new_pos);

        // Update orientation to face velocity direction
        if (glm::length(velocity_) > 0.0001f) {
            glm::vec3 dir = glm::normalize(velocity_);
            float angle = atan2(dir.y, dir.x);
            SetRotation(angle);
        }

        // Update lifetime
        lifetime_ += (float)delta_time;
    }

    void ProjectileGameObject::SetVelocity(const glm::vec3& velocity)
    {
        velocity_ = velocity;
    }

    glm::vec3 ProjectileGameObject::GetVelocity() const
    {
        return velocity_;
    }

    glm::vec3 ProjectileGameObject::GetPreviousPosition() const
    {
        return previous_position_;
    }

    bool ProjectileGameObject::IsExpired() const
    {
        return lifetime_ >= max_lifetime_;
    }

} // namespace game