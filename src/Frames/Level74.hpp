#pragma once

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level74(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level74;
  level.type = LevelState::Type::Horizontal;
  level.number = { 7, 4 };
  level.extra_textures = {
		texture_groups::lava,
		texture_groups::flame_koopa,
    &textures::castle_bg,
		&textures::platform,
		&textures::upfire,
		&textures::flame_fish_swim[0],
		&textures::flame_fish_swim[1],
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::castle_bg;

		level.game_objects.push(LavaState{ glm::vec2(6.f, 11.f), glm::vec2(6.f, 1.f) });
		level.game_objects.push(LavaState{ glm::vec2(15.f, 10.f), glm::vec2(7.f, 2.f) });

		level.game_objects.push(FireBarState(glm::vec2(13.f, 9.f), 5.f)); 

		for (int i = 0; i < 2; ++i){
			level.game_objects.push(UpfireState(glm::vec2(7.f + i * 3.f, 11.5f), 30.f))
				.counter.value = i * -3.f;

			level.game_objects.push(UpfireState(glm::vec2(17.f + i * 2.f, 11.f), 30.f))
				.counter.value = i * -3.f;
		}

		level.game_objects.push(LavaState{ glm::vec2(25.f, 10.f), glm::vec2(6.f, 2.f) });
		level.game_objects.push(LavaState{ glm::vec2(34.f, 10.f), glm::vec2(4.f, 2.f) });

		for (int i = 0; i < 2; ++i){
			level.game_objects.push(UpfireState(glm::vec2(26.f + i * 3.f, 11.f), 30.f));
		}

		level.game_objects.push(FireBarState(glm::vec2(32.f, 1.f), 4.f));
		level.game_objects.push(LavaState{ glm::vec2(55.f, 10.f), glm::vec2(5.f, 2.f) });

		level.game_objects.push(LavaState{ glm::vec2(75.f, 11.f), glm::vec2(3.f, 1.f) });
		level.game_objects.push(LavaState{ glm::vec2(82.f, 10.f), glm::vec2(112.f, 2.f) });

		for (int i = 1; i <= 9; i += 4){
			level.game_objects.push(LoopedPlatformState(glm::vec2(84.f, i), glm::vec2(0, 1)));
			level.game_objects.push(LoopedPlatformState(glm::vec2(91.f, i), glm::vec2(0, -1)));
			level.game_objects.push(LoopedPlatformState(glm::vec2(98.f, i), glm::vec2(0, 1)));
		}

		for (int i = 0; i < 2; ++i){
			auto& bar = level.game_objects.push(FireBarState(glm::vec2(92.5f, 5.5f), 2.f));
			bar.spacing = 7;
			bar.initial_rotation = i * 180.f;
		}

    for (int i = 120; i < 172; i += FishState::random_value(5, 8)){
      level.game_objects.push(FishState::make_jumping({ i, 12.f }))
				.type = FishState::Type::Flame;
    }

		level.game_objects.push(FlameFlyingKoopaState::make(glm::vec2(188.f, 7.f), glm::vec2(0.f, 2.f)));

    level_generator::generate_level(level, "level74_1.csv");
    level_generator::generate_level(level, "level74_2.csv");
  });
}

} //namespace mario::frames
