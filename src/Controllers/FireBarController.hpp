#pragma once

#include "PolyControllers.hpp"

#include "States/FireBarState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"

namespace mario{

template<>
struct Controller<FireBarState>{
  static auto run(FireBarState& bar, LevelState& level){
    for (int i = 0; i < bar.fireballs.size(); ++i){
      auto& fireball = bar.fireballs[i];
      fireball.current_texture = &textures::fireball[level.fireball_counter.int_value()];

      const auto y = BlockBase::Size * i * 0.8f;
      const auto rotation = glm::vec2(
        -y * glm::sin(LevelState::timer * FireBarState::RotationSpeed),
        y * glm::cos(LevelState::timer * FireBarState::RotationSpeed)
      );
      fireball.position = bar.position + rotation;

      if (!level.is_finished){
        entity_controller::kill_player_on_touch(fireball, level.player);
      }
    }
  }
};

} //namespace mario
