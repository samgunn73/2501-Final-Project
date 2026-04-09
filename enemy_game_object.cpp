#include "enemy_game_object.h"
#include <glm/gtc/constants.hpp>.
#include <iostream>

namespace game {

    // Constructor
    EnemyGameObject::EnemyGameObject(const glm::vec3& position,
        Geometry* geometry, Shader* shader, GLuint texture,GameObject* target,EnemyState state) : GameObject(position, geometry, shader, texture), 
        state_(state),
        ellipse_center_(position),
        ellipse_width_(1.0f),
        ellipse_height_(0.5f),
        theta_(0.0f),
        target_(target),
        speed_(1.0f),
        accel_(glm::vec3(0.0f,0.0f,0.0f))
    {
        reaction_timer_.Start(2.0);
    }

    void EnemyGameObject::Update(double delta_time)
    {
        if (state_ == PATROLLING) {

            // Advance angle
            theta_ += delta_time;

            // Elliptical motion
            glm::vec3 pos;
            pos.x = ellipse_center_.x + ellipse_width_ * cos(theta_);
            pos.y = ellipse_center_.y + ellipse_height_ * sin(theta_);
            pos.z = ellipse_center_.z;

            SetPosition(pos);

            // Check distance to player
            if (target_) {
                float dist = glm::length(target_->GetPosition() - GetPosition());
                if (dist < 2.5f) {
                    state_ = INTERCEPTING;
                    glm::vec3 direction = target_->GetPosition() - GetPosition();
                    direction = glm::normalize(direction);
                    velocity_ = direction * speed_;

                    reaction_timer_.Start(2.0);
                }
            }
        }
        else if (state_ == INTERCEPTING) {

            // Recalculate direction every 2 seconds
            if (reaction_timer_.Finished()) {

                glm::vec3 direction = target_->GetPosition() - GetPosition();

                direction = glm::normalize(direction);
                velocity_ = direction * speed_;

                reaction_timer_.Start(2.0);
            }
        }
        else if (state_ == HOOK) {
            //If the hook is just spawned in and has no velocity of any kind
            if (velocity_.y == 0) {
                velocity_ = glm::vec3(0.0f, -speed_, 0.0f);

            }
            //If the hook is traveling down, and is far away vertically from its spawn location
            //It should switch velocity upwards
            else if (velocity_.y < 0 && spawn_position_.y -5.0 > GetPosition().y) {
                velocity_ = glm::vec3(0.0f, speed_, 0.0f);
            }
            //If the hook is traveling up and has surpassed its spawn location, make it move downwards
            else if (velocity_.y > 0 && spawn_position_.y < GetPosition().y) {
                velocity_ = glm::vec3(0.0f, -speed_, 0.0f);
            }
            //If the hook has come into a certain distance from the player, turn to intercepting
            if (target_) {
                float dist = glm::length(target_->GetPosition() - GetPosition());
                if (dist < 2.5f) {
                    state_ = INTERCEPTING;
                    glm::vec3 direction = target_->GetPosition() - GetPosition();
                    direction = glm::normalize(direction);
                    velocity_ = direction * speed_;

                    reaction_timer_.Start(2.0);
                }
            }
        }
        //Net performs a wandering AI as its movement, with weighted priority on moving downwards
        if (state_ == NET) {
            //If stationary give a random target downwards
            float pi = glm::pi<float>();
            float randnumx = (float)rand() / (float)RAND_MAX;
            float randnumy = (float)rand() / (float)RAND_MAX;
            float randnumangle = ((float)rand() / (float)RAND_MAX) * pi - pi;
            if (velocity_.x == 0 && velocity_.y == 0) {
                glm::vec3 currpos = GetPosition();
                //Creating a new game object for the target, only really care about position, so making nullptrs for the other data, will never be rendered
                GameObject* new_target = new GameObject(glm::vec3(currpos.x + 4 * randnumx - 2, currpos.y + 4*randnumy -4,0.0f),nullptr,nullptr,GL_TEXTURE0 );
                target_ = new_target;
                //Since velocity is zero, can make accelleration 
                velocity_ = glm::normalize(target_->GetPosition() - currpos) * speed_;
                // start the timer to generate a new target
                reaction_timer_.Start(1.0);
            }
            if (reaction_timer_.Finished()) {
                //The new target will be on a circle in the dirrection of travel
                //If the target is above the current gameobject, recalculate 

                glm::vec3 circlepos = glm::normalize(velocity_) * WANDER_RING_DISTANCE;
                glm::vec3 targetpos = circlepos + glm::vec3(cos(randnumangle), sin(randnumangle), 0.0f) * WANDER_RING_RADIUS;
                glm::vec3 direction = targetpos - GetPosition();
                direction = glm::normalize(direction);
                glm::vec3 target_velocity = direction * speed_;
                accel_ = target_velocity - velocity_;
                
                
                reaction_timer_.Start(2.0);
            }
            velocity_ = velocity_ + accel_ * (float)delta_time;
            //If Veloc
            

            
                    
                    
                

               
                

                
            
            
        }
        //If it needs to perform euler integration
        if (state_ == HOOK || state_ == NET || state_ == INTERCEPTING) {
           
            glm::vec3 new_pos = GetPosition() + velocity_ * (float)delta_time;
            SetPosition(new_pos);
        }

        GameObject::Update(delta_time);
    }

    void EnemyGameObject::SetEllipse(const glm::vec3& center,
        float width,
        float height)
    {
        ellipse_center_ = center;
        ellipse_width_ = width;
        ellipse_height_ = height;
    }

    void EnemyGameObject::SetTarget(GameObject* target)
    {
        target_ = target;
    }
}
