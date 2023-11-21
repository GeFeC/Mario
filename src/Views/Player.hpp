#pragma once

#include "States/PlayerState.hpp"
#include "States/LevelState.hpp"

#include "Views/Entities.hpp"

namespace mario::views{

static auto render_player(const PlayerState& player, const LevelState& level){
  for (const auto& fireball : player.fireballs){
    render_entity(fireball, level);
    render_block(fireball.explosion, level);
  } 

  render_entity(player, level);
}

} //namespace mario::views
