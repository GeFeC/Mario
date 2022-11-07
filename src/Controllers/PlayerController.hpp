#pragma once

#include "States/EntityState.hpp"
#include "States/LevelState.hpp"
#include "States/BlockState.hpp"

auto player_controller(EntityState& player, LevelState& level) -> void;

auto player_hit_block_above(const EntityState& player, const BlockState& block) -> bool;
auto player_can_hit_block_above(const EntityState& player, const BlockState& block) -> bool;

auto player_stomp_on_entity(const EntityState& player, const EntityState& entity) -> bool;
