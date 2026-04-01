#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"
#include "geometry.h"
#include "timer.h"

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        public:
            // Constructor
            GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            virtual ~GameObject();

            // Update the GameObject's state. Can be overriden in children
            virtual void Update(double delta_time);

            // Renders the GameObject. Can also be overriden in children
            virtual void Render(glm::mat4 view_matrix, double current_time);

            // Getters
            //
            // Get position in OpenGL coordinates
            inline glm::vec3 GetPosition(void) const { return position_; }
            // Get scale (size) of the object *updated
            glm::vec2 GetScale(void) const { return scale_; }
            // Get current orientation
            inline float GetRotation(void) const { return angle_; }

            // Get bearing direction (vector pointing to the direction in which
            // the game object is facing)
            glm::vec3 GetBearing(void) const;

            // Get vector pointing to the right side of the game object
            glm::vec3 GetRight(void) const;

            // Setters
            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            inline void SetScale(glm::vec2& scale) { scale_ = scale; }
            void SetRotation(float angle);
            void SetTexture(GLuint texture);
            void SetTilingFactor(glm::vec2 tf) { tiling_factor_ = tf; }

            //ghost getter/setter
            bool IsGhost() const;
            void SetGhost(bool g);


        protected:
            // Object's Transform Variables
            glm::vec3 position_;
            glm::vec2 scale_; // updated
            float angle_;
            glm::vec2 tiling_factor_;

            // Geometry
            Geometry *geometry_;
 
            // Shader
            Shader *shader_;

            // Object's texture reference
            GLuint texture_;

            //Added Timer
        public:
            Timer explosion_timer;
            bool is_explosion = false;
            bool destroy_ = false;
            bool ghost_;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
