#pragma once
#include <iostream>

#include "States/BlockState.hpp"
#include "Renderer/Text.hpp"
#include "res/fonts.hpp"

struct PointsParticlesState{
  enum class Type{
    QBlock,
    Entity
  } type;

  Text text;

  int hits_required_to_activate = 1;
  float offset = 0.f;
  bool is_active = false;
  bool can_be_activated = true;

  PointsParticlesState(int label, const glm::vec2& position, Type type)
  : type(type) {
    text = Text(&fonts::small, std::to_string(label));
    text.set_position(position);
    text.is_visible = false;
  }

  auto set_active(const glm::vec2& position){
    text.position = position;
    text.is_visible = true;
    is_active = true;
  }

  auto as_block() const{
    return BouncingBlockState(text.get_position() / 60.f);
  }
};
