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
            bool isAirborne() { return in_air_; }
            void SetAirborne(bool ab) { in_air_ = ab; }
            glm::vec3 GetVelocity() { return velocity_; }
            void SetVelocity(glm::vec3 velocity) { velocity_ = velocity; }
            bool CanShoot() { return ammo > 0; }
            void AddAmmo(int addition) { ammo = ammo + addition; }
            void FireAmmo() { if (ammo > 0) { ammo = ammo - 1; }; }
            int GetAmmo() { return ammo; }
            const float gravity = 9.8f;
            const float restitution = 0.5f;
            

        private:
            glm::vec3 velocity_;
            bool in_air_ = false;
            float jump_velocity_ = 6.0f;

            float ground_y_ = -2.0f;
            float stop_threshold_ = 0.3f;
            int ammo = 0;
    };

}

#endif
