#pragma once
#include "States/BlockState.hpp"
#include "Renderer/Text.hpp"
#include "res/fonts.hpp"

#include <algorithm>

struct PointsParticlesState{
  static constexpr auto MaxOffset = 180.f;

  Text text;

  int hits_required_to_activate = 1;
  float offset = 0.f;
  bool is_active = false;
  bool can_be_activated = true;

  PointsParticlesState(){
    text = Text(&fonts::small, "");
    text.is_visible = false;
    text.position.y = config::BigValue;
  }

  PointsParticlesState(const std::string& label, const glm::vec2& position){
    text = Text(&fonts::small, label);
    text.set_position(position);
    text.is_visible = false;
  }

  PointsParticlesState(int label, const glm::vec2& position)
  : PointsParticlesState(std::to_string(label), position) {}

  auto set_active(const std::string& reward, const glm::vec2& position){
    text.set_text(reward);
    text.position = position;
    text.is_visible = true;
    is_active = true;
  }

  auto set_active(int reward, const glm::vec2& position){
    set_active(std::to_string(reward), position);
  }

  auto as_block() const{
    return BouncingBlockState(text.get_position() / 60.f);
  }

  auto finished() const{
    return offset >= MaxOffset;
  }
};

struct PointsParticlesManager{
  std::vector<PointsParticlesState> points;
  int current_points_index = 0;

  PointsParticlesManager(){
    points.reserve(8);
    points.emplace_back();
    current_points_index = 0;
  };

  auto make_next_points_particles_if_needed(){
    const auto available_points = std::find_if(points.begin(), points.end(), [](const auto& p){
      return !p.is_active;
    });

    if (available_points == points.end()){
      points.emplace_back();
      current_points_index = points.size() - 1;
      return;
    }

    current_points_index = available_points - points.begin();
 }

  auto& get_points_particles(){
    return points[current_points_index];
  }

  const auto& get_points_particles() const{
    return points[current_points_index];
  }
};
