#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

struct Texture{
private:
  std::string source_path;

  auto read_from_source_path_and_load_() const -> void;

public:
  bool allocated = false;
  GLuint id;

  Texture() noexcept = default;

  explicit Texture(const std::string& path) noexcept;
  explicit Texture(GLuint id) noexcept : id(id) {}
  
  auto allocate() -> void;
  auto free() noexcept -> void;

  auto set_min_filter(int min_filter) const noexcept -> void;
  auto set_mag_filter(int mag_filter) const noexcept -> void;

  auto bind() const noexcept -> void;
};

struct TextureGroup{
private:
  std::vector<Texture*> textures;
public:
  TextureGroup(Texture* texture){
    textures.push_back(texture);
  }

  TextureGroup(const std::vector<Texture*>& textures)
  : textures(textures){}

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
};
