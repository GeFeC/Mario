#pragma once

#include <string>

namespace shaders{
  inline auto v_shader = R"(
    #version 330 core

    layout (location = 0) in vec2 position;
    layout (location = 1) in vec2 texture_position;

    uniform mat4 projection;
    uniform vec4 rect;

    out vec2 f_texture_position;

    mat4 translate(mat4 model, vec2 vector){
      model[3][0] = vector.x;
      model[3][1] = vector.y;
    
      return model;
    }

    mat4 scale(mat4 model, vec2 vector){
      model[0][0] = vector.x;
      model[1][1] = vector.y;
    
      return model;
    }

    void main(){
      f_texture_position = texture_position;

      mat4 model = mat4(1.0);
      model = translate(model, vec2(rect.x, rect.y));
      model = scale(model, vec2(rect.z, rect.w));
      gl_Position = projection * model * vec4(position, 0.0, 1.0);
    }
  )";
};
