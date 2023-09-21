#pragma once

#include "config.hpp"

#include <stb/stb_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <stdexcept>

namespace mario::renderer{

struct Texture{
private:
  std::string source_path;

  auto read_from_source_path_and_load_() const -> void;

public:
  bool allocated = false;
  GLuint id;

  Texture() = default;

  explicit Texture(const std::string& path){
    this->source_path = config::TexturesSourceDir + path;
  } 

  explicit Texture(GLuint id) noexcept : id(id) {}
  
  auto allocate(){
    if (allocated) return this;

    allocated = true;

    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);

    //Load texture from file
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

    return this;
  }

  auto set_min_filter(int min_filter) const{
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
  }

  auto set_mag_filter(int mag_filter) const{
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
  }

  auto bind() const{
    glBindTexture(GL_TEXTURE_2D, this->id);
  }

  auto free(){
    allocated = false;
    glDeleteTextures(1, &this->id);
  }
};

struct TextureGroup{
  using TexturePtr = Texture*;

private:
  std::vector<TexturePtr> textures;

public:
  TextureGroup() = default;

  TextureGroup(TexturePtr texture){
    textures.push_back(texture);
  }

  explicit TextureGroup(const std::vector<TexturePtr>& textures)
  : textures(textures){}

  auto with(const std::vector<TexturePtr>& textures) const{
    auto textures_copy = this->textures;

    for (const auto& texture : textures){
      textures_copy.push_back(texture);
    }

    return TextureGroup(textures_copy);
  }

  auto allocate(){
    for (auto t : textures){
      t->allocate();
    }
  }

  auto free(){
    for (auto t : textures){
      t->free();
    }
  }

  const auto& front() const{
    return *textures[0];
  }

  const auto& operator[](int index) const{
    return *textures[index];
  }
};

} //namespace mario::renderer
