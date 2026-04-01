#include "player_game_object.h"

namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
	: GameObject(position, geom, shader, texture), velocity_(0.0f, 0.0f, 0.0f), in_air_(true) {}

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {
    // Gravity if in air
    if (in_air_) {
        const float gravity = -9.8f; // downward acceleration
        velocity_.y += gravity * (float)delta_time;
    }

    // Euler integration: position += velocity * dt
    glm::vec3 new_pos = GetPosition() + velocity_ * (float)delta_time;

    // Ground collision
    //const float ground_y = -2.0f;
    //if (new_pos.y <= ground_y) {
    //    new_pos.y = ground_y;

    //    if (in_air_) {
    //        // Bounce with coefficient of restitution
    //        const float restitution = 0.5f;
    //        velocity_.y *= -restitution;

    //        // Stop bouncing if too small
    //        if (fabs(velocity_.y) < 0.1f) {
    //            velocity_.y = 0.0f;
    //            in_air_ = false;
    //        }
    //    }
    //}

    SetPosition(new_pos);

    //// Friction in horizontal plane
    //float damping = 0.98f;
    //velocity_.x *= damping;
    //velocity_.z *= damping;

    //// Stop tiny motions
    //if (glm::length(glm::vec3(velocity_.x, 0, velocity_.z)) < 0.01f)
    //    velocity_.x = velocity_.z = 0.0f;
}

// Add horizontal or vertical velocity
void PlayerGameObject::AddVelocity(const glm::vec3& v) {
    velocity_ += v;

    // Max horizontal speed
    float max_speed = 5.0f;
    glm::vec2 horizontal(velocity_.x, velocity_.z);
    if (glm::length(horizontal) > max_speed) {
        horizontal = glm::normalize(horizontal) * max_speed;
        velocity_.x = horizontal.x;
        velocity_.z = horizontal.y;
    }
}

// Jump function
void PlayerGameObject::Jump(float jump_strength) {
    if (!in_air_) {
        velocity_.y += jump_strength;
        in_air_ = true;
    }
}

} // namespace game
