#include "hud_game_object.h"

namespace game {

    HUDGameObject::HUDGameObject(const glm::vec3& position,
        Geometry* geometry, Shader* shader, GLuint texture)
        : GameObject(position, geometry, shader, texture) {
        displayed_number = 0;
        pos_relative_player = position;
    }

    //Update takes player position so that the hud can move with the player so it stays in the same spot
    void HUDGameObject::Update(double delta_time,glm::vec3 player_pos) {
        position_ = glm::vec3(pos_relative_player.x + player_pos.x, pos_relative_player.y + player_pos.y, 0);
        GameObject::Update(delta_time);
    }

    //Make sure that the shader is getting the number properly
    void HUDGameObject::Render(glm::mat4 view_matrix, double delta_time) {
        shader_->Enable();
        if (displayed_number != 0) {
            shader_->SetUniform1i("number", displayed_number);
        }
        GameObject::Render(view_matrix, delta_time);
    }
    
    void HUDGameObject::SetNumber(int number) {
        if (number > 0 || number < 10) {
            if (number == 0) {
                displayed_number = 10;
            }
            else {
                displayed_number = number;
            }
            
        }
        else {
            displayed_number = 0;
        }
    }

}