#pragma once

#include "States/UpfireState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/EntityController.hpp"

namespace mario{

static auto run_controller(UpfireState& upfire, LevelState& level){
	entity_controller::kill_player_on_touch(upfire, level);	

	entity_controller::handle_gravity(upfire, level);

	if (upfire.gravity > 0.f){
		upfire.vertical_flip = util::Flip::flip();
	}
	else upfire.vertical_flip = util::Flip::no_flip();

	//Jumping:
	upfire.counter.run(window::delta_time);
	if (upfire.counter.stopped_counting()){
		upfire.counter.reset();
		upfire.is_active = true;
		upfire.position.y = upfire.initial_y;
		upfire.gravity = -upfire.jump_power;
		upfire.is_on_ground = false;
	}

	//Reseting:
	if (upfire.gravity > 0.f && upfire.position.y > upfire.initial_y){
		upfire.is_active = false;
	}
}

} //namespace mario
