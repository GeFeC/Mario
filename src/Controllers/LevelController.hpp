#pragma once

#include "States/LevelState.hpp"
#include "States/AppState.hpp"

#include "LevelGenerator/LevelGenerator.hpp"

#include "Controllers/PlayerController.hpp"
#include "Controllers/BlinkController.hpp"
#include "Controllers/CoinController.hpp"
#include "Controllers/QBlockController.hpp"
#include "Controllers/StatsController.hpp"
#include "Controllers/GoombaController.hpp"
#include "Controllers/BricksController.hpp"
#include "Controllers/SpikeController.hpp"
#include "Controllers/PlantController.hpp"
#include "Controllers/BlackPlantController.hpp"
#include "Controllers/JumpingKoopaController.hpp"
#include "Controllers/FlyingKoopaController.hpp"
#include "Controllers/BeetleController.hpp"
#include "Controllers/KoopaController.hpp"
#include "Controllers/FireBarController.hpp"
#include "Controllers/HammerBroController.hpp"
#include "Controllers/PlatformController.hpp"
#include "Controllers/FishController.hpp"
#include "Controllers/SquidController.hpp"
#include "Controllers/LakitoController.hpp"
#include "Controllers/UnstableCloudController.hpp"
#include "Controllers/FlameGoombaController.hpp"
#include "Controllers/FlameKoopaController.hpp"
#include "Controllers/FlameFlyingKoopaController.hpp"
#include "Controllers/FlameJumpingKoopaController.hpp"
#include "Controllers/UpfireController.hpp"
#include "Controllers/BowserController.hpp"
#include "Controllers/CannonController.hpp"

#include "Controllers/KingGoombaController.hpp"
#include "Controllers/KingKoopaController.hpp"
#include "Controllers/KingBeetleController.hpp"
#include "Controllers/KingCheepController.hpp"
#include "Controllers/KingPlantController.hpp"
#include "Controllers/KingLakitoController.hpp"
#include "Controllers/KingBowserController.hpp"

#include "Util/LoopedCounter.hpp"
#include "Util/Enum.hpp"
#include "Window.hpp"
#include "config.hpp"

namespace mario{

template<typename T>
static auto run_controller(T& t, LevelState& level){}

} //namespace mario

namespace mario::level_controller{

static auto handle_background(LevelState& level){
  level.cloud_offset += window::delta_time;

  static constexpr auto CloudMaxOffset = 18.f;
  if (level.cloud_offset > CloudMaxOffset) level.cloud_offset -= CloudMaxOffset;
}

static auto handle_final_boss_level(LevelState& level){
	if (level.stats.boss_hp.current == nullptr) return;

	if (*level.stats.boss_hp.current == 0){
		level.game_objects = {};
		level.stats.boss_hp.current = nullptr;
		level.type = LevelState::Type::Horizontal;
		level.hitbox_grid.clear();
		level.initialise_hitbox_grid();
		level.game_objects.push(LavaState{ glm::vec2(3.f, 11.f), glm::vec2(14.f, 1.f) });
		level_generator::generate_level(level, "level76after_boss.csv");
    level.background_texture = &textures::castle_bg;
	}
}

static auto handle_finishing(LevelState& level, AppState& app){
  const auto finish = level.finish_position; 
  auto& player = level.player;

  if (level.is_finished){
    level.score_adding_after_finish_delay -= window::delta_time;

    //Going through pipe:
    if (level.player.position.y / BlockBase::Size < finish.y + 1){
      level.player.position.y += window::delta_time * BlockBase::Size;
    }

    //Give points for the remaining time:
    if (level.stats.time > 0) {
      if (level.score_adding_after_finish_delay <= 0.f){
        const auto multiplier = std::min(2, level.stats.time);

        level.score_adding_after_finish_delay = 1.f / 60.f;
        level.stats.time -= multiplier;
        static constexpr auto PointsForEachTimeUnit = 10;
        level.stats.score += PointsForEachTimeUnit * multiplier;
      }
    }
    else{
      level.finish_delay -= window::delta_time;

      if (level.finish_delay <= 0.f){
				if (app.current_frame == AppState::Frame::Level76){
					handle_final_boss_level(level);
					level.is_finished = false;

					return;
				}

        app.current_frame = util::enum_add(app.current_frame, 1);
        app.current_level.current_checkpoint = LevelState::CheckpointNotSet;

      }
    }
  }

  if (player.position.y / BlockBase::Size - finish.y != util::in_range(-1, 0)) return;
  if (player.position.x / BlockBase::Size - finish.x != util::in_range(0.25, 0.75)) return;

  if (window::is_key_pressed(GLFW_KEY_DOWN)) level.is_finished = true;
}

static auto get_worlds_first_level(AppState::Frame level){
  const auto world_number = util::enum_multiply(level, 1.f / config::LevelsInWorld);

  return util::enum_multiply(world_number, config::LevelsInWorld);
}

static auto restart_when_player_fell_out(AppState& app){
  auto& level = app.current_level;
  auto& player = level.player;

  const auto started_falling = player.gravity_flip.is_flipped()
    ? player.position.y < -BlockBase::Size
    : player.position.y > (level.max_size().y + 1) * BlockBase::Size;

  if (started_falling) {
    player.can_move = false;
    //set speed to 0
    player.set_direction(util::Direction::left(), 0);
  }

  const auto position_required_to_restart_level 
    = LevelState::PlayerYToRestartLevel * player.gravity_flip.as_int() + level.camera_offset.y;

  const auto is_falling_delay_over = player.gravity_flip.is_flipped()
    ? player.position.y < position_required_to_restart_level
    : player.position.y > position_required_to_restart_level;

  if (is_falling_delay_over){
    app.should_restart_current_frame = true;
    level.stats.hp--;
    player.form = PlayerState::Form::Normal;
    player.growth = PlayerState::Growth::Small;
  }
}

static auto handle_camera(AppState& app){
	auto& level = app.current_level;
  auto& player = level.player;
  auto player_y = player.position.y - BlockBase::Size + player.size.y;
  
  //Vertical level scroll
  if (level.type == LevelState::Type::Vertical && !player.is_dead){
    //Scroll up
    if (!level.is_level_underground() && player_y - level.camera_offset.y < LevelState::MinPlayerRelativeY){
      level.camera_offset.y = player_y - LevelState::MinPlayerRelativeY;
    }

    //Scroll down
		const auto level_allows_scrolling_down = [&]{
			if (level.is_level_underground()) return true;
			if (player_controller::is_in_water(player, level)) return true;
			if (app.current_frame == AppState::Frame::Level35) return true;

			return false;
		};

    if (level_allows_scrolling_down() && player_y - level.camera_offset.y > LevelState::MaxPlayerRelativeY){
      level.camera_offset.y = player_y - LevelState::MaxPlayerRelativeY;
    }

    const auto max_scroll_y = level.max_size().y * BlockBase::Size - config::FrameBufferSize.y;
    const auto min_scroll_y = level.min_scroll_y * BlockBase::Size;
    level.camera_offset.y = std::clamp<float>(level.camera_offset.y, min_scroll_y, max_scroll_y);
  }

  //Horizontal level scroll
  else if (level.type == LevelState::Type::Horizontal){
    static constexpr auto HorizontalLevelWidth = LevelState::HorizontalLevelSize.x;
    static constexpr auto PlayerPositionToScroll = LevelState::PlayerPositionToStartLevelScrolling;

    const auto player_position_to_stop_scrolling 
      = HorizontalLevelWidth * BlockBase::Size 
      - (PlayerPositionToScroll.x + BlockBase::Size) * 2;

    if (player.position.x >= PlayerPositionToScroll.x && level.type == LevelState::Type::Horizontal){
      level.camera_offset.x = std::min(
        player.position.x - PlayerPositionToScroll.x,
        player_position_to_stop_scrolling
      );
    }
  }
}

static auto handle_checkpoints(LevelState& level){
  auto& player = level.player;

  if (player.is_dead) return;

  for (const auto& checkpoint : level.checkpoints){
    if (level.type == LevelState::Type::Horizontal && player.position.x >= checkpoint.x){
      level.current_checkpoint = checkpoint;
    }

    if (level.type == LevelState::Type::Vertical && player.position.y <= checkpoint.y){
      level.current_checkpoint = checkpoint;
    }
  }
}

static auto handle_level_background_pulsing(LevelState& level){
  auto& opacity = level.background_opacity;
  opacity = std::max(opacity - window::delta_time, 0.5f);

  auto& counter = level.background_pulse_counter;
  counter.run(window::delta_time);
  if (counter.loops >= 1){
    counter.loops = 0;

    opacity = 1.f;
    counter.limit = util::random_value(5, 15) / 10.f;
  }
}

static auto handle_lava(LevelState& level){
	level.lava_offset += window::delta_time;
	if (level.lava_offset > 1.f) level.lava_offset -= 1.f;
}

static auto handle_pause_menu(AppState& app){
	auto& level = app.current_level;

	if (input::key_escape.clicked() && app.current_frame != AppState::Frame::Menu) {
		level.is_paused = !level.is_paused;
	}

	if (!level.is_paused) return;

	if (input::key_up.clicked() || input::key_down.clicked()) {
		level.pause_menu_current_option = !level.pause_menu_current_option;
	}

	if (!input::key_enter.clicked()) return;

	level.is_paused = false;
	if (level.pause_menu_current_option == 1) {
		app.current_frame = AppState::Frame::Menu;
	}
}

static auto run(AppState& app){
  auto& level = app.current_level;

  if (level.is_level_in_castle()){
    handle_level_background_pulsing(level);
  }

	handle_pause_menu(app);

	if (level.is_paused) return;

  //Level loading
  if (level.load_delay > 0.f) {
    level.load_delay -= window::delta_time;
    return;
  }

	handle_lava(level);

  handle_checkpoints(level);
  restart_when_player_fell_out(app);

  //Blinking and counters
  level.blink_state = blink_controller::run();
  level.coin_spin_counter.run(window::delta_time);
  level.fire_flower_blink_counter.run(window::delta_time);

  level.purple_koopa_counter.run(window::delta_time);
  level.fireball_counter.run(window::delta_time);
  level.hammer_counter.run(window::delta_time);

  auto& player = level.player;

  if (!level.is_finished) {
    stats_controller::run(level.stats);

		if (!(app.current_frame == AppState::Frame::Level76 && player.position.x >= 39.f * BlockBase::Size)){
			player_controller::run(player, level);
		}
		else{
			level.player.current_texture = &textures::fire_big_mario;
			app.game_finish_timer += window::delta_time;
		}

    if (level.stats.time <= 0){
      player.is_dead = true;
    }

		const auto final_boss_defeated = 
			app.current_frame == AppState::Frame::Level76 && 
			level.stats.boss_hp.current == nullptr;

		if (final_boss_defeated){
			//Prevent from death:
			player.is_dead = false;
		}
  }

  if (player.is_growing_up || player.is_shrinking || player.is_changing_to_fire) return;
  //Game objects
  handle_background(level);
  
  level.game_objects.for_each([&](auto& object){
    run_controller(object, level);
  });

  handle_finishing(level, app);

  //Counting coins
  static constexpr auto CoinsToGetHP = 100;
  if (level.stats.coins >= CoinsToGetHP) {
    level.stats.coins -= CoinsToGetHP;
    level.stats.hp++;
  }

  //Camera
  handle_camera(app);

  //Timers
  LevelState::timer += window::delta_time;
  level.purple_flying_koopa_timer += window::delta_time;
}

} //namespace mario::level_controller
