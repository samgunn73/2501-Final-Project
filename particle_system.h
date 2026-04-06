#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "game_object.h"
#include "timer.h"

namespace game {

    // Inherits from GameObject
    class ParticleSystem : public GameObject {

        public:
            ParticleSystem(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GameObject *parent);

            void Update(double delta_time) override;

            void Render(glm::mat4 view_matrix, double current_time) override;

            void StartTimer(int length) { destruction_timer_.Start(length); }

            bool ToDestroy() { return destruction_timer_.Finished(); }

        private:
            GameObject *parent_;
            //Used for when to destroy the object
            Timer destruction_timer_;
            //Used to render the particles based on the time the system starts
            float start_time_;

    }; // class ParticleSystem

} // namespace game

#endif // PARTICLE_SYSTEM_H_
