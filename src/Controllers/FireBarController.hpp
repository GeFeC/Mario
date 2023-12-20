#pragma once

#include "States/FireBarState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"

namespace mario{

static auto run_controller(FireBarState& bar, LevelState& level){
  for (int i = 0; i < bar.fireballs.size(); ++i){
    auto& fireball = bar.fireballs[i];
    fireball.current_texture = &textures::fireball[level.fireball_counter.int_value()];

    const auto y = BlockBase::Size * i * bar.spacing;
    const auto initial_rotation = glm::radians(bar.initial_rotation);
    const auto rotation = glm::vec2(
      -y * glm::sin(initial_rotation + LevelState::timer * FireBarState::RotationSpeed),
      y * glm::cos(initial_rotation + LevelState::timer * FireBarState::RotationSpeed)
    );
    fireball.position = bar.position + rotation;

    if (!level.is_finished){
      entity_controller::kill_player_on_touch(fireball, level);
    }
  }
}

} //namespace mario
