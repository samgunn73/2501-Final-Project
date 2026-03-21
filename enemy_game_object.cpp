#include "enemy_game_object.h"

namespace game {

    // Constructor
    EnemyGameObject::EnemyGameObject(const glm::vec3& position,
        Geometry* geometry, Shader* shader, GLuint texture) : GameObject(position, geometry, shader, texture), 
        state_(PATROLLING),
        ellipse_center_(position),
        ellipse_width_(1.0f),
        ellipse_height_(0.5f),
        theta_(0.0f),
        target_(nullptr),
        speed_(0.5f) 
    {
        reaction_timer_.Start(2.0);
    }

    void EnemyGameObject::Update(double delta_time)
    {
        if (state_ == PATROLLING) {

            // Advance angle
            theta_ += delta_time;

            // Elliptical motion
            glm::vec3 pos;
            pos.x = ellipse_center_.x + ellipse_width_ * cos(theta_);
            pos.y = ellipse_center_.y + ellipse_height_ * sin(theta_);
            pos.z = ellipse_center_.z;

            SetPosition(pos);

            // Check distance to player
            if (target_) {
                float dist = glm::length(target_->GetPosition() - GetPosition());
                if (dist < 2.5f) {
                    state_ = INTERCEPTING;
                    glm::vec3 direction = target_->GetPosition() - GetPosition();
                    direction = glm::normalize(direction);
                    velocity_ = direction * speed_;

                    reaction_timer_.Start(2.0);
                }
            }
        }
        else if (state_ == INTERCEPTING) {

            // Recalculate direction every 2 seconds
            if (reaction_timer_.Finished()) {

                glm::vec3 direction = target_->GetPosition() - GetPosition();

                direction = glm::normalize(direction);
                velocity_ = direction * speed_;

                reaction_timer_.Start(2.0);
            }

            // Euler integration
            glm::vec3 new_pos = GetPosition() + velocity_ * (float)delta_time;

            SetPosition(new_pos);
        }

        GameObject::Update(delta_time);
    }

    void EnemyGameObject::SetEllipse(const glm::vec3& center,
        float width,
        float height)
    {
        ellipse_center_ = center;
        ellipse_width_ = width;
        ellipse_height_ = height;
    }

    void EnemyGameObject::SetTarget(GameObject* target)
    {
        target_ = target;
    }
}
