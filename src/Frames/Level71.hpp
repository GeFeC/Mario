#pragma once

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level71(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level71;
  level.type = LevelState::Type::Horizontal;
  level.number = { 7, 1 };
  level.extra_textures = {
    &textures::castle_bg,
    texture_groups::flame_goomba,
		&textures::lava_bottom,
		&textures::lava_top,
		&textures::platform
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::castle_bg;

		level.game_objects.push(LavaState{ glm::vec2(61.f, 10.f), glm::vec2(45.f, 2.f) });
		level.game_objects.push(LavaState{ glm::vec2(24.f, 10.f), glm::vec2(3.f, 2.f) });
		level.game_objects.push(LavaState{ glm::vec2(45.f, 10.f), glm::vec2(13.f, 2.f) });

		level.game_objects.push(FireBarState(glm::vec2(27.f, 10.f), 6.f)); 
		level.game_objects.push(FireBarState(glm::vec2(110.f, 7.f), 5.f)); 
		level.game_objects.push(FireBarState(glm::vec2(116.f, 7.f), 5.f)); 
		level.game_objects.push(FireBarState(glm::vec2(113.f, 3.f), 5.f)); 
		level.game_objects.push(FireBarState(glm::vec2(119.f, 3.f), 5.f)); 

		level.game_objects.push(PlatformState(glm::vec2(64.f, 9.f), glm::vec2(0.f, -5.f), 10))
		.timer = 1.f;
		level.game_objects.push(PlatformState(glm::vec2(72.f, 9.f), glm::vec2(0.f, -5.f), 10))
		.timer = 3.f;
		level.game_objects.push(PlatformState(glm::vec2(80.f, 9.f), glm::vec2(0.f, -5.f), 10))
		.timer = 5.f;
		level.game_objects.push(PlatformState(glm::vec2(88.f, 9.f), glm::vec2(0.f, -5.f), 10))
		.timer = 7.f;

		level.game_objects.for_each_type<PlatformState>([](auto& platform){
			platform.is_active = true;
		});

		level.game_objects.push(FireBarState(glm::vec2(66.f, 6.f), 5.f));
		level.game_objects.push(FireBarState(glm::vec2(82.f, 6.f), 5.f));

		level.game_objects.push(FlameGoombaState::make(glm::vec2(76.f, 4.f)))
		.is_active = true;
		level.game_objects.push(FlameKoopaState::make(glm::vec2(92.f, 4.f)))
		.is_active = true;

		level.game_objects.push(FlameGoombaState::make(glm::vec2(153.f, 2.f))).flip_gravity();
		level.game_objects.push(FlameGoombaState::make(glm::vec2(163.f, 2.f))).flip_gravity();

		level_generator::put_q_block_with_entity(
			level, 
			HammerBroState::make_red(glm::vec2(102.f, 4.f))
		);

    level_generator::generate_level(level, "level71_1.csv");
    level_generator::generate_level(level, "level71_2.csv");
  });
}

} //namespace mario::frames
