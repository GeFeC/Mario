#pragma once
#include "States/BlockState.hpp"
#include "Renderer/Text.hpp"
#include "res/fonts.hpp"
#include "Util/Util.hpp"

#include <algorithm>

namespace mario{

struct PointsParticlesState{
  static constexpr auto MaxOffset = 180.f;
  static constexpr auto TextSize = 2.f;

  renderer::Text text;

  int hits_required_to_activate = 1;
  float offset = 0.f;
  bool is_active = false;
  bool can_be_activated = true;

  PointsParticlesState(){
    text = renderer::Text(&fonts::normal, "", TextSize);
    text.is_visible = false;
    text.position.y = util::BigValue;
  }

  PointsParticlesState(const std::string& label, const glm::vec2& position){
    text = renderer::Text(&fonts::normal, label, TextSize);
    text.position = position;
    text.is_visible = false;
    text.update();
  }

  PointsParticlesState(int label, const glm::vec2& position)
  : PointsParticlesState(std::to_string(label), position) {}

  auto set_active(const std::string& reward, const glm::vec2& position){
    text.text = reward;
    text.position = position;
    text.is_visible = true;
    text.update();
    is_active = true;
  }

  auto set_active(int reward, const glm::vec2& position){
    set_active(std::to_string(reward), position);
  }

  auto as_block() const{
    return BouncingBlockState(text.position / BlockBase::Size);
  }

  auto finished() const{
    return offset >= MaxOffset;
  }
};

} //namespace mario
