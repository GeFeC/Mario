#pragma once

namespace mario::renderer{

static auto fragment_shader_script = R"(
  #version 330 core

  in vec2 f_texture_position;

  uniform mat4 projection;

  uniform sampler2D pixel_map;
  uniform vec4 f_color;

  uniform float texture_alpha = 1;
  uniform int is_texture = 1;
  uniform int is_glyph = 0;
  uniform int is_shadow = 0;
  uniform int is_highlighted = 0;

  out vec4 color;

  void main(){
    vec4 texture_pixel = texture(pixel_map, f_texture_position);
    texture_pixel.a *= texture_alpha;
    float alpha = texture_pixel.r;

    const float shadow = 0.1;
    vec4 face_color = 
      texture_pixel * (1 - is_glyph) * (1 - is_shadow) * (is_texture)
      + f_color * (1 - is_texture) * (1 - is_shadow)
      + vec4(shadow, shadow, shadow, texture_pixel.a * 0.5) * is_shadow * (1 - is_glyph)
      + vec4(1, 1, 1, texture_pixel.a) * is_highlighted * (1 - is_shadow);

    vec4 glyph_color = 
      is_glyph * vec4(f_color.r, f_color.g, f_color.b, alpha * f_color.a) * (1 - is_shadow)
      + vec4(shadow, shadow, shadow, alpha * 0.5) * is_glyph * is_shadow;

    color = face_color + glyph_color;
  }
)";

}
