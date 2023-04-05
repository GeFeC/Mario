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
  TextureGroup() = default;

  TextureGroup(Texture* texture){
    textures.push_back(texture);
  }

  explicit TextureGroup(const std::vector<Texture*>& textures)
  : textures(textures){}

  auto with(const std::vector<Texture*>& textures) const{
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
};
