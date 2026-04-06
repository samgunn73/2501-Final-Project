#include "weapon_game_object.h"

namespace game {

    WeaponGameObject::WeaponGameObject(const glm::vec3& position,
        Geometry* geometry, Shader* shader, GLuint texture, GameObject* parent)
        : GameObject(position, geometry, shader, texture),
        is_active_(false), parent_(parent) {
        active_timer_ = Timer();
        recharge_timer_ = Timer();
        position_offset_ = position;
        position_ = parent->GetPosition() + position_offset_;
    }

    //Direction will always be either -1 or 1 depending on which side its attacking from
    void WeaponGameObject::Attack(int direction) {
        if (active_timer_.Finished()) {
            //This is to swap the position of the weapon to which direction u are attacking from
            position_offset_ = glm::vec3(fabs(position_offset_.x)*direction,position_offset_.y,0.0f);
            is_active_ = true;
            active_timer_.Start(CLAW_ATTACK_TIMER);
        }
    }
    //Checking if the weapon has collision
    bool WeaponGameObject::IsActive() {
        if (active_timer_.Finished()) {
            is_active_ = false;
        }
        return is_active_;
    }

    void WeaponGameObject::Update(double delta_time) {
        position_ = parent_->GetPosition() + position_offset_;
        GameObject::Update(delta_time);
    }




}