#pragma once

#include "Renderer/Renderer.hpp"

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"

#include "config.hpp"
#include "res/textures.hpp"

static auto render_entity(const EntityState& entity, float offset_x = 0.f){
  renderer::draw(Drawable{
    entity.position - glm::vec2(offset_x, 0),
    entity.size,
    entity.current_texture,
    entity.direction,
    entity.is_visible
  });
}

static auto render_block(const BlockState& block, float offset_x = 0.f){
  renderer::draw(Drawable{
    block.position - glm::vec2(offset_x, 0),
    glm::vec2(BlockState::Size),
    block.texture,
    1,
    block.is_visible
  });
}
