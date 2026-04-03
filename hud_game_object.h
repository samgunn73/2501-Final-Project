#ifndef HUD_H_
#define HUD_H_



#include "game_object.h"

namespace game {

    class HUDGameObject : public GameObject {
    public:
        // Constructor
        HUDGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
        void Update(double delta_time, glm::vec3 player_pos);
        void Render(glm::mat4 view_matrix,double delta_time);
        void SetNumber(int number);
        
    private:
        glm::vec3 pos_relative_player;
        //Only for numbers
        //If = 0, the HUD Object is not a number
        //If 1-9 it displays the number
        //If 10 it displays 0
        int displayed_number;
    };
}
#endif