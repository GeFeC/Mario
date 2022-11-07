#include "Texture.hpp"

#include "config.hpp"

#include <fstream>
#include <vector>
#include <stb/stb_image.h>

Texture::Texture(const std::string& path) noexcept{
  this->source_path = config::TexturesSourceDir + path;
}

auto Texture::allocate() -> void{
  glGenTextures(1, &id);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, id);

  //Load texture from file
  {
    int width, height, channels;
    auto image_buffer = stbi_load(source_path.c_str(), &width, &height, &channels, 0);

    if (image_buffer == nullptr) {
      throw std::runtime_error("Failed to load image from: " + source_path);
    }
      
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(
      GL_TEXTURE_2D, 
      0, 
      GL_RGBA, 
      width, 
      height, 
      0, 
      GL_RGBA, 
      GL_UNSIGNED_BYTE, 
      image_buffer
    );
    
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image_buffer);
  }
}

auto Texture::set_min_filter(int min_filter) const noexcept -> void{
  glBindTexture(GL_TEXTURE_2D, this->id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
}

auto Texture::set_mag_filter(int mag_filter) const noexcept -> void{
  glBindTexture(GL_TEXTURE_2D, this->id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
}

auto Texture::bind() const noexcept -> void{
  glBindTexture(GL_TEXTURE_2D, this->id);
}

auto Texture::free() noexcept -> void{
  glDeleteTextures(1, &this->id);
}
