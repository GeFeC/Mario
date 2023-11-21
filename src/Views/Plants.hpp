#pragma once

#include "States/BossState.hpp"
#include "States/PlantState.hpp"
#include "States/LevelState.hpp"

#include "Renderer/Renderer.hpp"

#include "Views/Entities.hpp"
#include "Views/Blocks.hpp"

namespace mario::views{

template<typename T>
static auto render_plant(const T&, const LevelState&) {}

static auto render_plant(const BlackPlantState& plant, const LevelState& level){
  for (const auto& f : plant.fireball_generator.items){
    render_entity(f, level);
    render_block(f.explosion, level);
  }

  render_entity(plant | util::as<EntityState>, level);
}

static auto render_plant(const KingPlantState& boss, const LevelState& level){
  renderer::highlight_mode = boss.is_highlighted;
  render_plant(boss | util::as<BlackPlantState>, level);
  renderer::highlight_mode = false;
}

static auto render_plant(const PlantState& plant, const LevelState& level){
  render_entity(plant | util::as<EntityState>, level);
}

} //namespace mario::views
