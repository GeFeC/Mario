#pragma once

#include "States/EntityState.hpp"
#include "States/PlayerState.hpp"
#include "States/LevelState.hpp"
#include "States/BlockState.hpp"

#include "Controllers/CollisionController.hpp"

auto player_controller(PlayerState& player, LevelState& level) -> void;

auto player_can_hit_block_above(const PlayerState& player, const BouncingBlockState& block) -> bool;
auto player_hit_block_above(const PlayerState& player, const BouncingBlockState& block) -> bool;

auto player_is_on_entity(const PlayerState& player, const EntityState& entity) -> bool;
auto player_stomp_on_entity(const PlayerState& player, const EntityState& entity) -> bool;
