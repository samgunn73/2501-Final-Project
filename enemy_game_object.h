#pragma once

#include "game_object.h"
#include "timer.h"
enum EnemyState {
    PATROLLING,
    INTERCEPTING,
    HOOK,
    NET,
    NUM_ENEMY_TYPES = 3
};
const float WANDER_RING_DISTANCE = 6.0;
const float WANDER_RING_RADIUS = 0.5;

namespace game {

    class  EnemyGameObject : public GameObject {
    public:
        //Gonna Reuse States, Heat-seeking Harpoon is gonna take PATROLLING AND INTERCEPTING
        //Hook and Net are just gonna have their own
        //Hook is going to go up and down in a set position and then Intercept
        //Net is going to wander around with priority on going down
        
        

        // Constructor
        EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture,GameObject* target,EnemyState state);

        virtual void Update(double delta_time);

        void SetEllipse(const glm::vec3& center, float width, float height);
        void SetTarget(GameObject* target);
        

    private:
        // State
        EnemyState state_;

        // Ellipse patrol data, only used for objects that gain the INTERCEPTING STATE
        glm::vec3 ellipse_center_;
        float ellipse_width_;
        float ellipse_height_;
        float theta_;          // parametric angle

        // Intercept data
        GameObject* target_;
        glm::vec3 velocity_;
        glm::vec3 accel_;
        float speed_;

        //Spawn Postion (For Hook to see when it needs to change velocity
        glm::vec3 spawn_position_;

        
        // Reaction timer
        Timer reaction_timer_;
    };

}