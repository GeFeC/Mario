#pragma once

#include "Renderer/Drawable.hpp"
#include "States/FlameGoombaState.hpp"
#include "Views/Util.hpp"

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

static auto make_drawable_from_entity(const EntityState& entity, const LevelState& level) -> renderer::Drawable{
  auto drawable = renderer::Drawable{};
  drawable.position = entity.position - level.camera_offset;
  drawable.size = entity.size;
  drawable.texture = entity.current_texture;
  drawable.is_visible = entity.is_visible;
  drawable.flip = { 
    util::Flip(entity.direction.as_int() * entity.texture_flip.as_int()), 
    entity.vertical_flip 
  };

  return drawable;
}

static auto render_entity(const EntityState& entity, const LevelState& level) -> void{
  if (!views::is_component_on_screen(entity, level.camera_offset)) return;
  renderer::draw(make_drawable_from_entity(entity, level));
}

static auto render_entity(const FlameParticleState& flame, const LevelState& level){
  if (!flame.is_active) return;

  renderer::draw_plain(renderer::PlainDrawable{
    flame.position - level.camera_offset,
    flame.size,
    glm::vec4(1.f, flame.opacity, 0.f, flame.opacity)
  });
}

static auto render_block(const BlockBase& block, const LevelState& level) -> void;

static auto render_entity(const FlameKoopaState& koopa, const LevelState& level){
	render_entity(koopa.fireball, level);
	render_block(koopa.fireball.explosion, level);

	render_entity(koopa | util::as<EntityState>, level);
}

static auto render_entity(const FlameFlyingKoopaState& koopa, const LevelState& level){
	render_entity(koopa.fireball, level);
	render_block(koopa.fireball.explosion, level);

	render_entity(koopa | util::as<EntityState>, level);
}

static auto render_entity(const FlameJumpingKoopaState& koopa, const LevelState& level){
	render_entity(koopa.fireball, level);
	render_block(koopa.fireball.explosion, level);

	render_entity(koopa | util::as<EntityState>, level);
}

static auto render_entity(const FlameGoombaState& goomba, const LevelState& level){
  for (const auto& flame : goomba.flames_generator.items){
    for (const auto& particle : flame.particles()){
      render_entity(particle, level);
    }
  }

  for (const auto& particle : goomba.inner_flame.particles()){
    render_entity(particle, level);
  }

  render_entity(goomba | util::as<EntityState>, level);
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
