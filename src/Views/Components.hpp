#pragma once

#include "Renderer/Renderer.hpp"

#include "States/EntityState.hpp"
#include "States/PlayerState.hpp"
#include "States/BlockState.hpp"
#include "States/PointsParticlesState.hpp"

#include "config.hpp"
#include "res/textures.hpp"

static auto render_entity(const EntityState& entity, float offset_x = 0.f){
  renderer::draw(Drawable{
    entity.position - glm::vec2(offset_x, 0),
    entity.size,
    entity.current_texture,
    { entity.direction * entity.texture_flip, entity.vertical_flip },
    entity.is_visible
  });
}

static auto render_points_particles(const std::vector<PointsParticlesState>& points, float offset_x = 0.f){
  for (const auto& point : points){
    renderer::print(point.text, offset_x);
  }
}

static auto render_block(const BlockBase& block, float offset_x = 0.f){
  renderer::draw(Drawable{
    block.position - glm::vec2(offset_x, 0),
    glm::vec2(BlockState::Size),
    block.texture,
    { Drawable::Flip::NoFlip, Drawable::Flip::NoFlip },
    block.is_visible
  });
}

static auto render_bricks(const BricksBlockState& block, float offset_x = 0.f){
  for (const auto& particle : block.particles){
    render_entity(particle, offset_x);
  }

  render_block(block, offset_x);
}

static auto render_player(const PlayerState& player, float offset_x = 0.f){
  for (const auto& fireball : player.fireballs){
    render_entity(fireball, offset_x);
    render_block(fireball.explosion, offset_x);
  } 

  render_entity(player, offset_x);
}
