#pragma once

#include "game_object.h"
#include "timer.h"

namespace game {

    class  EnemyGameObject : public GameObject {
    public:
        enum EnemyState {
            PATROLLING,
            INTERCEPTING
        };

        // Constructor
        EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        // Update the CollectibleGameObject's state. Can be overriden in children
        virtual void Update(double delta_time);

        void SetEllipse(const glm::vec3& center, float width, float height);
        void SetTarget(GameObject* target);

    private:
        // State
        EnemyState state_;

        // Ellipse patrol data
        glm::vec3 ellipse_center_;
        float ellipse_width_;
        float ellipse_height_;
        float theta_;          // parametric angle

        // Intercept data
        GameObject* target_;
        glm::vec3 velocity_;
        float speed_;

        // Reaction timer
        Timer reaction_timer_;
    };

}