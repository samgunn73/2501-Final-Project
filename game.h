#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "game_object.h"
#include "player_game_object.h"


namespace game {

    // A class for holding the main game objects
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();

            // Call Init() before calling any other method
            // Initialize graphics libraries and main window
            void Init(void); 

            // Set up the game world (scene, game objects, etc.)
            void SetupGameWorld(void);

            // Destroy the game world
            void DestroyGameWorld(void);

            // Run the game (keep the game active)
            void MainLoop(void); 

        private:
            // Main window: pointer to the GLFW window structure
            GLFWwindow *window_;

            // Sprite geometry
            Geometry *sprite_;

            // Particle geometry
            Geometry* particles_;

            // Shader for rendering sprites in the scene
            Shader sprite_shader_;

            // Shader for rendering background
            Shader bg_shader_;

            // Shader for rendering particles
            Shader particle_shader_;

            // References to textures
            // This needs to be a pointer
            GLuint *tex_;

            // List of game objects that have collision
            std::vector<GameObject*> game_objects_;

            // List of game objects for the hud
            std::vector<GameObject*> hud_objects_;

            // List of game objects that are the background
            std::vector<GameObject*> background_objects_;

            // List of game objects that are particle systems
            std::vector<GameObject*> particle_objects_;

            // Keep track of game time
            double current_time_;

            // Camera zoom factor
            float camera_zoom_;

            // Handle user input
            void HandleControls(double delta_time);

            void FireProjectile(PlayerGameObject* player);

            // Update all the game objects
            void Update(double delta_time);
 
            // Render the game world
            void Render(void);

            // Get mouse position in OpenGL coordinates
            // Returns "true" if position could be gathered, "false" otherwise
            // Position is saved to mouse.x and mouse.y
            bool GetMousePosition(glm::vec2 &mouse);

            // Callback for when the window is resized
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Set a specific texture
            void SetTexture(GLuint w, const char *fname);

            // Load all textures
            void LoadTextures(std::vector<std::string> &textures);

            // For explosions
            std::vector<GameObject*> enemy_explosions_;
            std::vector<Timer> enemy_explosion_timers_;

            // Player damage
            int player_damage_ = 0;

            // Timer
            Timer player_explosion_timer_;
            Timer invincible_timer_;

            // Flags
            bool player_explosion_active_ = false;
            bool invincible_ = false;

            // Track explosion object
            GameObject* player_explosion_ = nullptr;

            // Enemy spawning
            Timer enemy_spawn_timer_;
            bool enemy_spawn_timer_started_ = false;

            // Collectibles
            int collected_items_ = 0;

            GLuint normal_player_tex_;
            GLuint invincible_player_tex_;

            Timer shoot_cooldown_timer_;
            bool shoot_timer_started_ = false;

            // Blade pointer
            GameObject* blade_;
    }; // class Game

} // namespace game

#endif // GAME_H_
