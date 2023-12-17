#pragma once

#include "States/FlameKoopaState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/MonsterController.hpp"
#include "Controllers/ShellMonsterController.hpp"
#include "Controllers/FireballController.hpp"

namespace mario{

static auto run_controller(FlameKoopaState& koopa, LevelState& level){
	//Base:
	monster_controller::endure_fireball(koopa, level.player);
	monster_controller::become_active_when_seen(koopa, level);
  monster_controller::die_when_on_bouncing_block(koopa, level);

	shell_monster_controller::run(koopa, level, textures::flame_koopa_walk);
	shell_monster_controller::handle_shell(koopa, level, textures::flame_koopa_dead);

	//Follow player:
	if (!koopa.in_shell && !level.player.is_dead){
		if (koopa.position.x > level.player.position.x) koopa.set_direction(util::Direction::left());
		else koopa.set_direction(util::Direction::right());
	}

	if (level.player.is_dead) koopa.follows_player = false;

	//Fireball controller:
	fireball_controller::run(koopa.fireball, level);
	if (entity_controller::kill_player_on_touch(koopa.fireball, level)){
		fireball_controller::reset(koopa.fireball);
	}

	if (koopa.in_shell) return;
	if (!koopa.is_active) return;

	const auto distance_to_player = std::abs(koopa.position.x - level.player.position.x);

	if (!koopa.fireball.is_active && distance_to_player < 8.f * BlockBase::Size){
		const auto fireball_y = koopa.position.y + BlockBase::Size / 2.f;
		const auto fireball_x = koopa.position.x + (koopa.direction.is_left()
			? 0.f
			: koopa.size.x - koopa.fireball.size.x);

		koopa.fireball.shoot(
			glm::vec2(fireball_x, fireball_y), 
			koopa.direction, 
			PlayerState::FireballSpeed
		);
	}
}

} //namespace mario
