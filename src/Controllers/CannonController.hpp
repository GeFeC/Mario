#pragma once

#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/MonsterController.hpp"

namespace mario{

static auto run_controller(CannonState& cannon, LevelState& level){
	const auto& player = level.player;

	//Shooting:
	const auto distance_to_player = glm::abs(player.position.x - cannon.position.x);
	if (distance_to_player < LevelState::BlocksInRow * 2.f * BlockBase::Size){
		cannon.shot_counter.run(window::delta_time);
	}

	if (cannon.shot_counter.stopped_counting()){
		cannon.shot_counter.reset();

		cannon.bullet_generator.make_item_if_needed(cannon.position / BlockBase::Size);
		auto& new_bullet = cannon.bullet_generator.item();
		new_bullet.is_active = true;
		
		auto fly_direction = player.position.x < cannon.position.x 
			? util::Direction::left()
			: util::Direction::right();

		new_bullet.set_direction(fly_direction);
		sounds::sounds[sounds::BulletBill].play();
	}

	//Bullets controller:
	for (auto& bullet : cannon.bullet_generator.items){
		bullet.position.x 
			+= bullet.walk_speed 
			* bullet.direction.as_int()
			* window::delta_time 
			* EntityState::MovementSpeedMultiplier;

		entity_controller::kill_player_on_touch(bullet, level);
		monster_controller::handle_points_particles(bullet);

		monster_controller::die_when_stomped(bullet, level, [&]{
			bullet.was_hit = true;
		});

		if (bullet.was_hit){
			entity_controller::handle_gravity(bullet, level);
		}

		const auto bullet_relative_pos = bullet.position - level.camera_offset;
		if (bullet.direction.is_left() && bullet_relative_pos.x < -bullet.size.x * 2.f){
			bullet.is_active = false;
		}

		if (bullet.direction.is_right() && bullet_relative_pos.x > config::FrameBufferSize.x){
			bullet.is_active = false;
		}
	}
}

} //namespace mario
