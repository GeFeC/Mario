#pragma once

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level73(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level73;
  level.type = LevelState::Type::Vertical;
  level.number = { 7, 3 };
  level.extra_textures = {
    &textures::castle_bg,
		&textures::upfire,
		texture_groups::lava,
		texture_groups::flame_koopa
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::castle_bg;

		level.game_objects.push(LavaState{ glm::vec2(3.f, 149.f), glm::vec2(14.f, 1.f) });

		for (int i = 0; i < 7; ++i){
			auto& upfire = level.game_objects.push(UpfireState(glm::vec2(3.f + i * 2.f, 149.f), 22.f));
			static constexpr auto CounterLimit = 6.f;
			upfire.counter.limit = CounterLimit;
			upfire.counter.value = CounterLimit - CounterLimit * i / 7.f;
		}

		level.game_objects.push(FlameGoombaState::make(glm::vec2(15.f, 128.f)))
			.flip_gravity();

		level.game_objects.push(FlameGoombaState::make(glm::vec2(5.f, 129.f)))
			.flip_gravity();

		level.game_objects.push(FireBarState(glm::vec2(4.f, 118.f), 4.f));
		level.game_objects.push(FireBarState(glm::vec2(4.f, 118.f), 4.f))
			.initial_rotation = 180.f;

		level.game_objects.push(FireBarState(glm::vec2(9.f, 85.f), 3.f));

		level.game_objects.push(LavaState{ glm::vec2(1.f, 65.f), glm::vec2(15.f, 2.f) });

		for (int i = 0; i < 3; ++i){
			auto& upfire = level.game_objects.push(UpfireState(glm::vec2(4.f + i * 4.f, 66.f), 30.f));
			upfire.counter.value = i * 3.f;
		}

    level_generator::generate_level(level, "level73_1.csv");
    level_generator::generate_level(level, "level73_2.csv");
  });
}

} //namespace mario::frames
