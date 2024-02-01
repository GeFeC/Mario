#pragma once

#include "Window.hpp"
#include "States/StatsState.hpp"
#include "States/LevelState.hpp"

#include "res/sounds.hpp"

namespace mario::stats_controller{

static auto run(StatsState& stats, LevelState& level){
  //Time:
  static auto counter = 0.f;
  counter += window::delta_time * 16.f;

	if (level.player.is_dead) return;

  if (stats.time == 0) return;
	if (stats.time == 100 && level.background_music->is_playing()){
		level.background_music->stop();		
		level.background_music = level.background_low_time_music;
		sounds::sounds[sounds::LowTime].play();
	}

	if (
		stats.time < 100 && 
		!sounds::sounds[sounds::LowTime].is_playing() && 
		!level.background_music->is_playing()
	){
		level.background_music->play();
	}

  if (counter >= 5.f){
    --stats.time;
    counter = 0.f;
  }

  //View movement:

  stats.position_y += window::delta_time * 2000.f * stats.move_direction.as_int();

  stats.position_y = std::clamp(stats.position_y, StatsState::MinPositionY, StatsState::MaxPositionY);
}

} //namespace mario
