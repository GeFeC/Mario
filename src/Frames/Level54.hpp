#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level54(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level54;
  level.type = LevelState::Type::Vertical;
  level.number = { 5, 4 };
  level.extra_textures = {
    &textures::underground_bg,
    &textures::darkness_view,
    texture_groups::green_flying_koopa
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::underground_bg;

    level.is_dark = true;
    level.should_handle_hitbox_on_sides = false;

    level.player.position = glm::vec2(2, 5) * BlockBase::Size;

    level_generator::generate_level(level, "level54_1.csv");
    level_generator::generate_level(level, "level54_2.csv");

    level.game_objects.push(FlyingKoopaState::make_green({ 9.5, 39.5 }, { 6, 0 }));
    level.game_objects.push(FlyingKoopaState::make_green({ 9.5, 50.5 }, { 6, 0 }));
    level.game_objects.push(BlockState({ 20, 108 }, &textures::blue_dirt));

    level.game_objects.for_each_type<SpikeState>([](auto& spike){
      spike.can_move = false; //Spikes don't need to walk in this level
    });
  });
}

} //namespace mario::frames
