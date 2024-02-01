#pragma once

#include "States/LevelState.hpp"
#include "Controllers/MonsterController.hpp"
#include "res/textures.hpp"

namespace mario{

static auto run_controller(BowserState& bowser, LevelState& level){
	//Base:
	monster_controller::become_active_when_seen(bowser, level);
	monster_controller::handle_points_particles(bowser);
	entity_controller::handle_movement(bowser, level);
	entity_controller::handle_gravity(bowser, level);
	entity_controller::kill_player_on_touch(bowser, level);

	bowser.is_highlighted = false;
	for (auto& fireball : level.player.fireballs){
		if (monster_controller::is_hit_by_fireball(bowser, fireball)){
			bowser.hp--;
			
			if (bowser.hp == 0 && !bowser.was_hit){
				monster_controller::bounce_die(bowser, level.stats);
			}

			bowser.is_highlighted = true;
			fireball_controller::reset(fireball);

		}
	}

	//Textures:
	if (bowser.is_attacking){
		entity_controller::run_movement_animation(bowser, textures::bowser_walk_attack);
	}
	else{
		entity_controller::run_movement_animation(bowser, textures::bowser_walk);
	}

	//Movement:
	static constexpr auto MaxOffset = 3.f * BlockBase::Size;
	if (bowser.position.x > bowser.initial_x + MaxOffset){
		bowser.acceleration.right = 0.f;
		bowser.acceleration.left = BowserState::WalkSpeed;
	}

	if (bowser.position.x < bowser.initial_x - MaxOffset){
		bowser.acceleration.right = BowserState::WalkSpeed;
		bowser.acceleration.left = 0.f;
	}

	if (bowser.acceleration.left == bowser.acceleration.right){
		bowser.acceleration.right = BowserState::WalkSpeed;
		bowser.acceleration.left = 0.f;
	}

	//Jumping:
	auto& jump_counter = bowser.jump_counter;
	jump_counter.run(window::delta_time);
	if (jump_counter.stopped_counting() && !bowser.was_hit){
		jump_counter.reset();

		jump_counter.limit = util::random_value(4, 5);
		bowser.gravity = -20.f;
		bowser.is_on_ground = false;
	}

	//Following player:
	auto& player = level.player;
	
	if (!player.is_dead && !bowser.was_hit){
		if (bowser.position.x > player.position.x + bowser.size.x + BlockBase::Size) {
			bowser.direction = util::Direction::left();
		}
		if (bowser.position.x + BlockBase::Size < player.position.x) {
			bowser.direction = util::Direction::right();
		}
	}

	if (!bowser.is_active) return;

	//Attacking:
	auto& attack_counter = bowser.attack_counter;
	attack_counter.run(window::delta_time);

	if (
			attack_counter.stopped_counting() && 
			player.position.y < bowser.position.y + 2 * BlockBase::Size &&
			!bowser.was_hit
	){
		attack_counter.reset();

		bowser.is_attacking = true;
		bowser.fire_generator.make_item_if_needed();
		auto& fire = bowser.fire_generator.item();
		fire.is_active = true;
		fire.set_direction(bowser.direction, 6.f);
		fire.position.x = bowser.position.x 
			- fire.size.x * (1 - fire.direction.as_binary())
			+ bowser.size.x * fire.direction.as_binary();
		fire.position.y = bowser.position.y + fire.size.y / 2;

		if (entity_controller::is_player_nearby(bowser, player)){
			sounds::sounds[sounds::Fire].play();
		}

		//Calculation of direction vector:
		const auto player_center = player.position + player.size / 2.f;
		const auto fire_center = fire.position + fire.size / 2.f;

		auto distance = player_center - fire_center;

		//Max angle: 30 degrees
		if (glm::abs(distance.y) > glm::abs(distance.x) / 2.f){
			const auto sign = distance.y / glm::abs(distance.y);
			distance.y = glm::abs(distance.x) * sign / 2.f;
		}
		
		fire.direction_vec = glm::normalize(distance);
	}

	if (attack_counter.value > 0.5f) bowser.is_attacking = false;

	//Fire controller:
	for (auto& fire : bowser.fire_generator.items){
		static constexpr auto FireSpeed = 600.f;
		fire.position += fire.direction_vec * window::delta_time * FireSpeed;

		entity_controller::run_movement_animation(fire, textures::bowser_fire, 16.f);
		entity_controller::kill_player_on_touch(fire, level);

		const auto fire_relative_pos = fire.position - level.camera_offset;
		if (fire_relative_pos.x != util::in_range(-fire.size.x * 2.f, config::FrameBufferSize.x)){
			fire.is_active = false;
		}

		if (fire_relative_pos.y != util::in_range(-fire.size.y * 2.f, config::FrameBufferSize.y)){
			fire.is_active = false;
		}
	}
}

} //namespace mario
