#pragma once

#include "game_object.h"

const float CLAW_ATTACK_TIMER = 1.0;
const float CLAW_REFRESH_TIMER = 1.5;


namespace game {

    class WeaponGameObject : public GameObject {
    public:
        // Constructor
        WeaponGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent);
        void Attack(int direction);
        bool IsActive();
        void Update(double delta_time) override;
        
        

    private:
        bool is_active_;
        Timer active_timer_;
        Timer recharge_timer_;
        GameObject* parent_;
        glm::vec3 position_offset_;
    };

}