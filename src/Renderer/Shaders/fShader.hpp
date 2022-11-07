#pragma once

namespace shaders{
  inline auto f_shader = R"(
    #version 330 core

    in vec2 f_texture_position;

    uniform mat4 projection;

    uniform sampler2D pixel_map;
    uniform vec4 f_color;

    uniform int is_glyph = 0;
    uniform int is_shadow = 0;

    out vec4 color;

    void main(){
      vec4 texture_pixel = texture(pixel_map, f_texture_position);
      float alpha = texture_pixel.r;

      const float shadow = 0.1;
      vec4 face_color = 
        texture_pixel * (1 - is_glyph) * (1 - is_shadow)
        + vec4(shadow, shadow, shadow, texture_pixel.a * 0.5) * is_shadow * (1 - is_glyph);

      vec4 glyph_color = 
        is_glyph * vec4(1.0, 1.0, 1.0, alpha) * (1 - is_shadow)
        + vec4(shadow, shadow, shadow, alpha * 0.5) * is_glyph * is_shadow;

      color = face_color + glyph_color;
    }
  )";
};
