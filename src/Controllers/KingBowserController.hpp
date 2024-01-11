#pragma once

#include "States/BossState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/EntityController.hpp"
#include "Controllers/BossController.hpp"
#include "Controllers/FlameController.hpp"

namespace mario{

static auto run_controller(KingBowserState& boss, LevelState& level){
	if (boss.hp > 0){
    entity_controller::run_movement_animation(boss, textures::king_bowser_walk_attack);
	}

	boss_controller::run(boss, level);
  boss_controller::react_when_hit_by_fireball(boss, level);

	if (boss.hp == 0){
		entity_controller::handle_gravity(boss, level);
		return;
	}

	//Attack indicator:
	if (boss.attack_indicator_opacity > 0.f){
		boss.attack_indicator_opacity -= window::delta_time * 1.5f;
	}

	//Switching between attacks:
	if (boss.attacks_count == 0 && boss.attack_state == KingBowserState::AttackState::Charge){
		boss.rotation = 0;
		boss.attack_state = KingBowserState::AttackState::Flame;
		boss.direction = util::Direction::right();
	}

	if (boss.rotation >= 360.f && boss.attack_state == KingBowserState::AttackState::Flame){
		boss.attack_state = KingBowserState::AttackState::Charge;
		boss.attacks_count = 6;
	}
	else if (boss.rotation < 360.f){
		boss.rotation += window::delta_time * 100.f;
	}

	//Charge attack:
	auto& player = level.player;
	if (
		boss.attack_state == KingBowserState::AttackState::Charge && 
		!player.is_dead && boss.attacks_count > 0 &&
		player.position.y < (LevelState::BlocksInColumn + 1) * BlockBase::Size
	){
		boss.charge_counter.run(window::delta_time);
	}

	//Get target:
	const auto get_target = [&]{
		return boss.attacks_count == 1
			? KingBowserState::InitialPosition
			: boss.charge_target_position;
	};

	if (boss.charge_counter.value > 8.f && boss.charge_target_position == KingBowserState::NoTarget){
		boss.charge_target_position = player.position;

		boss.charge_target_position.x = std::clamp(
			boss.charge_target_position.x, 
			BlockBase::Size,
			(LevelState::BlocksInRow- 1) * BlockBase::Size - boss.size.x
		);

		boss.charge_target_position.y = std::clamp(
			boss.charge_target_position.y, 
			BlockBase::Size,
			(LevelState::BlocksInColumn - 1) * BlockBase::Size - boss.size.y
		);

		boss.attack_indicator_opacity = 1.f;
		boss.attack_indicator_pos = 
			get_target()
			- KingBowserState::AttackIndicatorSize / 2.f
			+ boss.size / 2.f;

		const auto distance_between_boss_and_player = player.position.x - boss.position.x;
		boss.direction = util::Direction::right();
		if (distance_between_boss_and_player <= 0) boss.direction = util::Direction::left();
	}

	//Attack:
	entity_controller::fade_copies_out(boss);
	
	if (boss.charge_counter.stopped_counting()){
		const auto target = get_target();

		const auto direction_to_target = target - boss.charge_start_position;
		boss.position += glm::normalize(direction_to_target) * 8'000.f * window::delta_time;
		
		entity_controller::create_copy(boss);

		if (direction_to_target.x * boss.position.x > direction_to_target.x * target.x){
			boss.position = target;
		}

		if (boss.position == target){
			boss.attacks_count--;
			boss.charge_counter.reset();
			boss.charge_target_position = KingBowserState::NoTarget;
			boss.charge_start_position = boss.position;
			boss.temp_direction = glm::normalize(direction_to_target);
			if (boss.attacks_count >= 1)
				boss.velocity = 100.f;
		}
	}

	if (boss.velocity > 0.f){
		boss.velocity = std::max(0.f, boss.velocity - window::delta_time * 120.f);
		boss.position += boss.temp_direction * boss.velocity * window::delta_time;
	}

	//Flames:
	if (boss.attack_state == KingBowserState::AttackState::Flame){
		boss.breathe_counter.run(window::delta_time);
	}

	for (auto& flame : boss.flames_generator.items){
		flame_controller(flame, level);
	}

	if (boss.breathe_counter.stopped_counting()){
		boss.breathe_counter.reset();

		boss.flames_generator.make_item_if_needed();
		auto& new_flame = boss.flames_generator.item();
		new_flame = FlameState();
		new_flame.is_active = true;
		new_flame.particle_counter.max_loops = 5;
		new_flame.rotation = boss.rotation + 90.f;
		new_flame.speed = 1'500.f;
		new_flame.min_angle = -30.f;
		new_flame.min_angle = 30.f;
		new_flame.particle_size = BlockBase::Size;
		new_flame.burn_speed = 0.7f;
		new_flame.opacity_required_to_kill_player = 0.1f;

		const auto bowser_center_pos = boss.position + boss.size / 2.f - new_flame.particle_size / 2.f;
		const auto flame_offset_from_center = glm::vec2(boss.size.x / 2.f, 0.f);

		const auto sin_rotation = glm::sin(glm::radians(boss.rotation));
		const auto cos_rotation = glm::cos(glm::radians(boss.rotation));

		new_flame.position = bowser_center_pos + glm::vec2(
			flame_offset_from_center.x * cos_rotation - flame_offset_from_center.y * sin_rotation,
			flame_offset_from_center.x * sin_rotation + flame_offset_from_center.y * cos_rotation
		);
	}

}

} //namespace mario
