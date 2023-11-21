#pragma once

#include "Views/Util.hpp"
#include "Views/Blocks.hpp"

#include "Util/Util.hpp"
#include "States/EntityState.hpp"
#include "States/PlantState.hpp"
#include "States/BlackPlantState.hpp"
#include "States/LevelState.hpp"

namespace mario::views{

template<typename T, typename = util::not_derived<T, EntityState>>
static auto render_entity(const T&, const LevelState&) {}

static auto render_entity(const PlantState&, const LevelState&) {}
static auto render_entity(const BlackPlantState&, const LevelState&) {}

static auto render_entity(const EntityState& entity, const LevelState& level) -> void{
  const auto& offset = level.camera_offset;
  if (!views::is_component_on_screen(entity, offset)) return;

  auto drawable = renderer::Drawable{};
  drawable.position = entity.position - offset;
  drawable.size = entity.size;
  drawable.texture = entity.current_texture;
  drawable.is_visible = entity.is_visible;
  drawable.flip = { 
    util::Flip(entity.direction.as_int() * entity.texture_flip.as_int()), 
    entity.vertical_flip 
  };

  renderer::draw(drawable);
}

static auto render_entity(const LakitoState& lakito, const LevelState& level){
  render_entity(lakito.thrown_spike, level);
  render_entity(lakito | util::as<EntityState>, level);
}

static auto render_entity(const FireBarState& bar, const LevelState& level){
  for (const auto& fireball : bar.fireballs){
    render_entity(fireball, level);
  }
}

static auto render_entity(const HammerBroState& bro, const LevelState& level){
  for (auto& item : bro.hammer_generator.items){
    render_entity(item, level);
  }

  render_entity(bro | util::as<EntityState>, level);
}

static auto render_entity(const PlatformState& platform, const LevelState& level){
  for (int i = 0; i < platform.width; ++i){
    auto drawable = renderer::Drawable{};

    drawable.position = platform.position + glm::vec2(i * PlatformState::ElementSize, 0) - level.camera_offset;
    drawable.size = glm::vec2(PlatformState::ElementSize);
    drawable.texture = &textures::platform;

    renderer::draw(drawable);
  }
}

static auto render_entity(const LoopedPlatformState& platform, const LevelState& level){
  render_entity(platform | util::as<PlatformState>, level);
}

template<typename T>
static auto render_entity(const BossState<T>& boss, const LevelState& level){
  renderer::highlight_mode = boss.is_highlighted;
  render_entity(boss | util::as<EntityState>, level);
  renderer::highlight_mode = false;
}

static auto render_entity(const KingCheepState& boss, const LevelState& level){
  auto drawable = renderer::RotatableDrawable();

  drawable.position = boss.position;
  drawable.size = boss.size;
  drawable.rotation = boss.rotation;
  drawable.texture = boss.current_texture;
  drawable.flip.vertical = boss.vertical_flip;

  renderer::highlight_mode = boss.is_highlighted;
  renderer::draw(drawable);
  renderer::highlight_mode = false;
}

static auto render_entity(const KingBeetleState& boss, const LevelState& level){
  for (const auto& f : boss.fireballs){
    render_entity(f, level);
    render_block(f.explosion, level);
  }

  render_entity(boss | util::as<BossState<>>, level);
}

static auto render_entity(const KingPlantState& boss, const LevelState& level){}

static auto render_entity(const KingLakitoState& boss, const LevelState& level){
  renderer::highlight_mode = boss.is_highlighted;
  for (auto& spike : boss.extra_thrown_spikes){
    render_entity(spike, level);
  }
  render_entity(boss | util::as<LakitoState>, level);
  renderer::highlight_mode = false;
}



} //namespace mario::views
