#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>

#include <path_config.h>

#include <cstdlib>
#include <ctime>

#include "sprite.h"
#include "shader.h"
#include "player_game_object.h"
#include "collectible_game_object.h"
#include "enemy_game_object.h"
#include "game.h"
#include "projectile_game_object.h"
#include "particles.h"
#include "particle_system.h"
#include "platform_game_object.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Game Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;

// Declare all the textures here
std::vector<std::string> textures;
enum TextureID {
    tex_crab = 0,
    tex_background1 = 1,
    tex_background2 = 2,
    tex_platform = 3
};

void Game::SetupGameWorld(void)
{

    // Setup the game world

    // **** Load all the textures that we will need
    //
    // Add the textures in the same order as the enum above 
    textures.push_back("/textures/crab1.png");
    textures.push_back("/textures/parallax4.png");
    textures.push_back("/textures/parallax3.png");
    textures.push_back("/textures/platform.png");
    // Load all the textures
    LoadTextures(textures);


    // **** Setup all the game objects in the world

    // Setup the player object (position, geometry, shader, texture)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector 
    //
    // Create the player object
    PlayerGameObject *player = new PlayerGameObject(glm::vec3(-2.5f, -2.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_crab]);
    // Add the player to the list of all game objects in the game
    game_objects_.push_back(player);
    // Make the object larger
    player->SetScale(glm::vec2(1.0f, 1.0f));

    PlatformGameObject *platform = new PlatformGameObject(glm::vec3(-2.5f, -3.0f, 0.0f), sprite_, &bg_shader_, tex_[tex_platform]);
    
    game_objects_.push_back(platform);
    platform->SetPosition(glm::vec3(.0f, -5.0f, 0.0f));
    platform->SetScale(glm::vec2(10.0f, 1.0f));
    platform->SetTilingFactor(glm::vec2(10.0f, 1.0f));

    std::cout << game_objects_.size() << std::endl;
    

    // Setup other objects
    // Shorter code than the setup of the player object
    //EnemyGameObject *enemy = new EnemyGameObject(glm::vec3(2.0f, 1.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_shrimp]);
    //enemy->SetEllipse(enemy->GetPosition(), 1.0f, 0.5f);
    //enemy->SetTarget(player);
    //game_objects_.push_back(enemy);

    //EnemyGameObject* enemy2 = new EnemyGameObject(glm::vec3(3.0f, -0.5f, 0.0f), sprite_, &sprite_shader_, tex_[tex_shrimp]);
    //enemy2->SetEllipse(enemy2->GetPosition(), 1.0f, 0.5f);
    //enemy2->SetTarget(player);
    //game_objects_.push_back(enemy2);

    //// Big shrimp using different scaling
    //EnemyGameObject* enemy3 = new EnemyGameObject(glm::vec3(4.0f, 3.5f, 0.0f), sprite_, &sprite_shader_, tex_[tex_shrimp]);
    //enemy3->SetScale(glm::vec2(3.5f, 2.0f)); // here!
    //enemy3->SetEllipse(enemy3->GetPosition(), 1.0f, 0.5f);
    //enemy3->SetTarget(player);
    //game_objects_.push_back(enemy3);

    //// Collectibles
    //game_objects_.push_back(new CollectibleGameObject(glm::vec3(-4.0f, -3.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_collectible]));
    //game_objects_.push_back(new CollectibleGameObject(glm::vec3(3.0f, -1.5f, 0.0f), sprite_, &sprite_shader_, tex_[tex_collectible]));
    //game_objects_.push_back(new CollectibleGameObject(glm::vec3(0.0f, -2.5f, 0.0f), sprite_, &sprite_shader_, tex_[tex_collectible]));
    //game_objects_.push_back(new CollectibleGameObject(glm::vec3(-3.0f, -2.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_collectible]));
    //game_objects_.push_back(new CollectibleGameObject(glm::vec3(4.0f, -3.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_collectible]));

    //// Setup blade
    //blade_ = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_blade]);
    //blade_->SetScale(glm::vec2(1.0f, 1.0f));
    //game_objects_.push_back(blade_);

    // Setup particle systems
   

    // Setup background
    // In this specific implementation, the background is always the
    // last object
    GameObject *background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &bg_shader_, tex_[tex_background1]);
    background->SetScale(glm::vec2(100,12));


    GameObject* background2 = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &bg_shader_, tex_[tex_background2]);
    background2->SetScale(glm::vec2(100,12));
    background2->SetTilingFactor(glm::vec2(12, 1));
    background_objects_.push_back(background2);
    background_objects_.push_back(background);

}


void Game::DestroyGameWorld(void)
{
    // Free memory for all objects
    // Only need to delete objects that are not automatically freed
    for (int i = 0; i < game_objects_.size(); i++){
        delete game_objects_[i];
    }
}


void Game::HandleControls(double delta_time)
{
    // Get player game object
    PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(game_objects_[0]);
    if (!player) return;

    glm::vec3 forward(1.0f, 0.0f, 0.0f); // x-forward

    float acceleration = 2.0f;  // impulse strength
    // Get current position and angle
    glm::vec3 curpos = player->GetPosition();
    float angle = player->GetRotation();
    // Compute current bearing direction
    glm::vec3 dir = player->GetBearing();
    // Adjust motion increment and angle increment 
    // if translation or rotation is too slow
    float speed = delta_time*500.0;
    float motion_increment = 0.001*speed;
    float angle_increment = (glm::pi<float>() / 1800.0f)*speed;

    // Terrain parameters
    float terrain_height = -2.1;

    // Check for player input and make changes accordingly
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        player->Jump(5.0f);
    }

    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        player->AddVelocity(acceleration * player->GetRight());
    }

    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        player->AddVelocity(glm::vec3(2.0f, 0.0f, 0.0f));
    }

    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        player->AddVelocity(glm::vec3(-2.0f, 0.0f, 0.0f));
    }

    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
        player->SetRotation(angle + angle_increment);
    }
    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
        player->SetRotation(angle - angle_increment);
    }
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        std::cout << "Hooray!" << std::endl;
    }

    // Aim at mouse
    glm::vec2 mouse_pos_2d;
    if (GetMousePosition(mouse_pos_2d)) {
        glm::vec3 player_pos = player->GetPosition();
        glm::vec3 look_dir(mouse_pos_2d.x - player_pos.x, 0.0f, mouse_pos_2d.y - player_pos.z);
        if (glm::length(look_dir) > 0.0001f) {
            float angle = atan2(look_dir.z, look_dir.x); // horizontal plane
            player->SetRotation(angle);
        }
    }

    // Start shoot timer once
    if (!shoot_timer_started_) {
        shoot_cooldown_timer_.Start(0.0);
        shoot_timer_started_ = true;
    }

    // Fire projectile
    if (glfwGetKey(window_, GLFW_KEY_F) == GLFW_PRESS) {
        if (shoot_cooldown_timer_.Finished()) {
            FireProjectile(player);
            // Restart cooldown (0.5 seconds)
            shoot_cooldown_timer_.Start(0.5);
        }
    }
}
void Game::FireProjectile(PlayerGameObject* player) {

    glm::vec3 forward = player->GetBearing(); // or use your GetBearing()
    float spawn_offset = player->GetScale().x;  // or a small number like 0.5
    glm::vec3 spawn_pos = player->GetPosition() + forward * spawn_offset;

    float projectile_speed = 10.0f;
    //ProjectileGameObject* proj = new ProjectileGameObject(spawn_pos, sprite_, &sprite_shader_, tex_[tex_bullet]);
    //proj->SetVelocity(forward * projectile_speed);
    //game_objects_.insert(game_objects_.end() - 1, proj);
}

// Function for collision checking
bool CheckCollision(GameObject* a, GameObject* b) {
    // Get positions
    glm::vec3 posA = a->GetPosition();
    glm::vec3 posB = b->GetPosition();

    // Get sizes (used as radius)
    float radiusA = a->GetScale().x * 0.5f;
    float radiusB = b->GetScale().x * 0.5f;

    // Distance between centers
    float dx = posA.x - posB.x;
    float dy = posA.y - posB.y;
    float distance = sqrt((dx * dx) + (dy * dy));
    
    // Collision test
    return distance < (radiusA + radiusB);
}

bool RayCircleCollision(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& center,
    float radius)
{
    glm::vec2 start(P0.x, P0.y);
    glm::vec2 end(P1.x, P1.y);
    glm::vec2 C(center.x, center.y);

    glm::vec2 d = end - start;       // segment direction
    glm::vec2 f = start - C;

    float a = glm::dot(d, d);
    float b = 2.0f * glm::dot(f, d);
    float c = glm::dot(f, f) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return false; // no intersection

    discriminant = sqrt(discriminant);

    float t1 = (-b - discriminant) / (2 * a);
    float t2 = (-b + discriminant) / (2 * a);

    // Check if intersection happens within segment
    if (t1 >= 0.0f && t1 <= 1.0f) return true;
    if (t2 >= 0.0f && t2 <= 1.0f) return true;

    return false;
}

void player_platform_collision(PlayerGameObject* player, PlatformGameObject* platform) {
    bool player_airborne = player->isAirborne();
    //If airborne and colliding with platform with negative y velocity, make player land on platform
    //If it has positive y velocity, it should move through the platform
    if (player_airborne && player->GetVelocity().y < 0) {
        if (player->GetPosition().y - (player->GetScale().y/2) < platform->GetPosition().y + (platform->GetScale().y/2)) {
            if ((player->GetPosition().x - (player->GetScale().x / 2) < platform->GetPosition().x + platform->GetScale().x / 2) && (player->GetPosition().x + (player->GetScale().x / 2) > platform->GetPosition().x - platform->GetScale().x / 2)) {


                player->SetAirborne(false);
                player->SetVelocity(glm::vec3(player->GetVelocity().x, 0.0, 0.0));
                platform->PlayerOnTop();
                return;
            }
        }
    }
    //If player is ontop of a platform, and becomes airborne, make it leave the platform
    // Example:: player jumps when they are ontop of a platform
    if (player_airborne && platform->IsPlayerOnTop()) {
        platform->NotPlayerOnTop();
        return;
    }
    //If player is nolonger ontop of platform horizontally, make them airborne and off the platform
    if (!player_airborne && platform->IsPlayerOnTop()) {
        if (!(player->GetPosition().x - (player->GetScale().x / 2) < platform->GetPosition().x + platform->GetScale().x / 2) || !(player->GetPosition().x + (player->GetScale().x / 2) > platform->GetPosition().x - platform->GetScale().x / 2)) {


            player->SetAirborne(true);
            platform->NotPlayerOnTop();
            return;
        }
    }
   
}

void Game::Update(double delta_time)
{

    // Start enemy spawn timer once
    if (!enemy_spawn_timer_started_) {
        enemy_spawn_timer_.Start(10.0);
        enemy_spawn_timer_started_ = true;
    }

    // Update all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

        // Update the current game object
        current_game_object->Update(delta_time);

        GameObject* player = game_objects_[0];

        glm::vec3 player_pos = player->GetPosition();
        float player_angle = player->GetRotation();
        double current_time = glfwGetTime();

        

       

        // Check for collision with other game objects
        //
        

        for (int j = i + 1; j < (game_objects_.size()); j++) {
            GameObject* other_game_object = game_objects_[j];


            //Player and Platform

            PlatformGameObject* platform =
                dynamic_cast<PlatformGameObject*>(other_game_object);
            if (platform) {
                player_platform_collision(dynamic_cast<PlayerGameObject*>(player), platform);
            }
            

            //Collectible collision check
            CollectibleGameObject* collectible =
                dynamic_cast<CollectibleGameObject*>(other_game_object);

            if (collectible) {

                if (CheckCollision(current_game_object, collectible)) {

                    // Prevent recollection
                    if (collectible->IsCollected()) continue;
                    // turn to ghost
                    collectible->Collect();
                    collectible->SetGhost(true);

                    // Increase item count
                    collected_items_++;


                    // Activate invincibility
                    if (collected_items_ >= 5 && !invincible_) {
                        invincible_ = true;
                        collected_items_ = 0;

                        invincible_timer_.Start(10.0);
                        game_objects_[0]->SetTexture(invincible_player_tex_);
                    }
                    break;
                }
                continue;
            }

            

         

            // Perform collision detection here
            // between current_game_object and other_game_object
            if (CheckCollision(current_game_object, other_game_object)) {
                // Get enemy position
                glm::vec3 enemy_pos = other_game_object->GetPosition();

                // Create enemy explosion
                //GameObject* explosion =
                //    new GameObject(enemy_pos, sprite_, &sprite_shader_, tex_[tex_explosion]);
                //explosion->SetScale(glm::vec2(1.5f));
                //// Insert before background
                //game_objects_.insert(game_objects_.end() - 1, explosion);

                //// Store explosion and timer
                //enemy_explosions_.push_back(explosion);
                Timer t;
                t.Start(5.0);
                enemy_explosion_timers_.push_back(t);

                // Remove enemy
                //delete other_game_object;
                //game_objects_.erase(game_objects_.begin() + j);

                // Register player damage
                if (!invincible_) {
                    player_damage_++;
                }

                break;
            }

        }
    }
    for (int i = enemy_explosions_.size() - 1; i >= 0; i--) {

        if (enemy_explosion_timers_[i].Finished()) {

            // Remove explosion object
            auto it = std::find(game_objects_.begin(),
                game_objects_.end(),
                enemy_explosions_[i]);

            if (it != game_objects_.end()) {
                delete enemy_explosions_[i];
                game_objects_.erase(it);
            }

            // Remove timer and pointer
            enemy_explosions_.erase(enemy_explosions_.begin() + i);
            enemy_explosion_timers_.erase(enemy_explosion_timers_.begin() + i);
        }
    }
    // projectile-enemy collisions
    for (int i = game_objects_.size() - 1; i >= 0; i--) {

        ProjectileGameObject* proj =
            dynamic_cast<ProjectileGameObject*>(game_objects_[i]);

        if (!proj) continue;

        for (int j = game_objects_.size() - 1; j >= 0; j--) {

            EnemyGameObject* enemy =
                dynamic_cast<EnemyGameObject*>(game_objects_[j]);

            if (!enemy) continue;

            float enemy_radius = enemy->GetScale().x * 0.5f;

            if (RayCircleCollision(
                proj->GetPreviousPosition(),
                proj->GetPosition(),
                enemy->GetPosition(),
                enemy_radius))
            {
                // Destroy enemy and projectiles
                enemy->destroy_ = true;
                proj->destroy_ = true;

                break;
            }
        }
    }

    for (int i = game_objects_.size() - 1; i >= 0; i--) {

        ProjectileGameObject* proj =
            dynamic_cast<ProjectileGameObject*>(game_objects_[i]);

        if (proj && proj->IsExpired()) {

            delete proj;
            game_objects_.erase(game_objects_.begin() + i);
        }
    }

    // Player explodes after 3 hits
    if (player_damage_ >= 3 && !player_explosion_active_) {
        // Create player explosion
        //GameObject* player = game_objects_[0];
        //player_explosion_ = new GameObject(player->GetPosition(), sprite_, &sprite_shader_, tex_[tex_explosion]);
        //player_explosion_->SetScale(glm::vec2(2.0f, 2.0f));
        //game_objects_.insert(game_objects_.end() - 1, player_explosion_);

        // Hide player and start timer
        //player->SetScale(glm::vec2(0.0f));
        //player_explosion_timer_.Start(5.0);
        //player_explosion_active_ = true;

    }

    // End game after player explosion finishes
    if (player_explosion_active_ && player_explosion_timer_.Finished()) {
        // Remove player from game objects
        delete game_objects_[0];
        game_objects_.erase(game_objects_.begin());

        // Game over and close window
        std::cout << "Game Over!" << std::endl;
        glfwSetWindowShouldClose(window_, true);
    }

    const float MIN_SPAWN_DISTANCE = 2.5f;
    // Spawn new enemy every 10 seconds
    if (enemy_spawn_timer_.Finished()) {

        GameObject* player = game_objects_[0];
        glm::vec3 spawn_pos;
        float dist;
        int attempts = 0;

        do {
            // Random position within visible area
            float x = ((float)rand() / RAND_MAX) * 6.0f - 3.0f;  // [-3, 3]
            float y = ((float)rand() / RAND_MAX) * 4.0f - 2.0f;  // [-2, 2]
            spawn_pos = glm::vec3(x, y, 0.0f);
            dist = glm::length(spawn_pos - player->GetPosition());

            attempts++;

        } while (dist < MIN_SPAWN_DISTANCE && attempts < 50);

        //EnemyGameObject* new_enemy = new EnemyGameObject(spawn_pos, sprite_, &sprite_shader_, tex_[tex_shrimp]);
        //new_enemy->SetEllipse(new_enemy->GetPosition(), 1.0f, 0.5f);
        //new_enemy->SetTarget(player);

        //// Insert before background
        //game_objects_.insert(game_objects_.end() - 1, new_enemy);

        // Restart timer
        enemy_spawn_timer_.Start(10.0);
    }

    // End invincibility
    if (invincible_ && invincible_timer_.Finished()) {
        invincible_ = false;

        // Restore player texture
        game_objects_[0]->SetTexture(normal_player_tex_);
    }

    for (int i = game_objects_.size() - 1; i >= 0; i--) {
        if (game_objects_[i]->destroy_) {
            delete game_objects_[i];
            game_objects_.erase(game_objects_.begin() + i);
        }
    }

    // Update current game time
    current_time_ += delta_time;
}


void Game::Render(void){

    // Clear background
    glClearColor(viewport_background_color_g.r,
                 viewport_background_color_g.g,
                 viewport_background_color_g.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use aspect ratio to properly scale the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    glm::mat4 window_scale_matrix(1.0f);
    if (width > height){
        float aspect_ratio = ((float) width)/((float) height);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
    } else {
        float aspect_ratio = ((float) height)/((float) width);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f/aspect_ratio, 1.0f));
    }

    // Set view to zoom out, centered by default at 0,0
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom_, camera_zoom_, camera_zoom_));
    
    GameObject* player = game_objects_[0];
    glm::vec3 player_pos = player->GetPosition();

    glm::mat4 camera_translation = glm::translate(glm::mat4(1.0f), glm::vec3(-player_pos.x, -player_pos.y, 0.0f));

    // Continuous scrolling
    glm::mat4 view_matrix = window_scale_matrix * camera_zoom_matrix * camera_translation;

    // Render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        game_objects_[i]->Render(view_matrix, current_time_);
    }
    for (int i = 0; i < background_objects_.size(); i++) {
        background_objects_[i]->Render(view_matrix, current_time_);
    }
}


void Game::MainLoop(void)
{
    // Loop while the user did not close the window
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Update window events like input handling
        glfwPollEvents();

        // Handle user input
        HandleControls(delta_time);

        // Update all the game objects
        Update(delta_time);

        // Render all the game objects
        Render();

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);
    }
}


bool Game::GetMousePosition(glm::vec2 &mouse){

    // Get mouse pixel position in the window
    double x, y;
    glfwGetCursorPos(window_, &x, &y);

    // Get dimensions of the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);

    // If the mouse is outside the window, return false
    if (x < 0 || x > width || y < 0 || y > height) {
        return false;
    }

    // Get position in world coordinates corresponding to the mouse
    // pixel position on the window
    if (width > height){
        float aspect_ratio = ((float) width)/((float) height);
        mouse.x = ((2.0f*x - width)*aspect_ratio)/(width*camera_zoom_);
        mouse.y = (-2.0f*y + height)/(height*camera_zoom_);
    } else {
        float aspect_ratio = height/width;
        mouse.x = (2.0f*x - width)/(width*camera_zoom_);
        mouse.y = ((-2.0f*y + height)*aspect_ratio)/(height*camera_zoom_);
    }

    return true;
}


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set whether window can be resized
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Initialize sprite geometry
    sprite_ = new Sprite();
    sprite_->CreateGeometry();

    // Initialize particle geometry
    Particles* particles_temp = new Particles();
    particles_temp->CreateGeometry(4000); // Use 4000 particles
    particles_ = particles_temp;

    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());
    
    // Initialize background shader
    bg_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/bg_fragment_shader.glsl")).c_str());

    // Initialize particle shader
    particle_shader_.Init((resources_directory_g + std::string("/particle_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/particle_fragment_shader.glsl")).c_str());

    // Initialize time
    current_time_ = 0.0;

    // Set zoom factor
    camera_zoom_ = 0.25f;

    srand((unsigned int)time(nullptr));
}


Game::~Game()
{
    // Free memor y used by game world
    DestroyGameWorld();

    // Free rendering resources
    delete sprite_;
    delete particles_;

    // Close window
    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


void Game::SetTexture(GLuint w, const char *fname)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        std::cout << "Cannot load texture " << fname << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Game::LoadTextures(std::vector<std::string> &textures)
{
    // Allocate a buffer for all texture references
    int num_textures = textures.size();
    tex_ = new GLuint[num_textures];
    glGenTextures(num_textures, tex_);
    // Load each texture
    for (int i = 0; i < num_textures; i++){
        SetTexture(tex_[i], (resources_directory_g+textures[i]).c_str());
    }
    // Set first texture in the array as default
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}

} // namespace game
