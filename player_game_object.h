#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            void AddVelocity(const glm::vec3& v);
            void Jump(float jump_strength);
            const float gravity = 9.8f;
            const float restitution = 0.5f;

        private:
            glm::vec3 velocity_;
            bool in_air_ = false;
            float jump_velocity_ = 6.0f;

            float ground_y_ = -2.0f;
            float stop_threshold_ = 0.3f;
    };

}

#endif
