#pragma once
#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "LevelGenerator/LevelGenerator.hpp"
#include "config.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

struct LevelFrameSharedData{
  LevelState::Type type;
  AppState::Frame frame;
  std::vector<TextureGroup> extra_textures;

  struct LevelNumber{
    int major = 0;
    int minor = 0;
  } number;
};

template<typename Function>
static auto run_frame_levelbase(
    AppState& app, 
    const LevelFrameSharedData& level_data,
    const Function& extra_setup
){
  auto textures = std::vector<TextureGroup>{
    texture_groups::mario,
    &textures::mini_coin
  };

  const auto setup = [&](auto& app){
    fonts::normal.allocate(16);
    LevelState::timer = 0.f;

    auto& level = app.current_level;

    level.type = level_data.type;

    level.stats.level_major = level_data.number.major;
    level.stats.level_minor = level_data.number.minor;

    level.stats.time = 400.f;

    level.camera_offset.y = 0.f;
    auto& player = level.player;

    //Checkpoints:
    if (level.current_checkpoint == LevelState::CheckpointNotSet){
      const auto level_height = level.max_size().y;
      level.current_checkpoint = { config::BlockSize, (level_height - 3.f) * config::BlockSize };
    }
    player.position = level.current_checkpoint;

    //Generating Level:
    level.generate_hitbox_grid();
    extra_setup(app);

    //Camera:
    if (level.type == LevelState::Type::Vertical){
      level.camera_offset.y = player.position.y - config::BlocksInColumn / 2.f * config::BlockSize;
    }

    //Hitbox Borders:
    if (level.type == LevelState::Type::Vertical){
      for (int i = 0; i < LevelState::MaxVerticalLevelSize.y; ++i){
        auto& normal_blocks = level.blocks.normal;

        normal_blocks.emplace_back(glm::vec2(-1, i), level_generator::no_texture);
        normal_blocks.back().is_visible = false; 

        normal_blocks.emplace_back(glm::vec2(LevelState::MaxVerticalLevelSize.x, i), level_generator::no_texture);
        normal_blocks.back().is_visible = false; 
      }
    }

    //Textures:
    for (const auto texture : level_generator::allocated_textures){
      textures.push_back(level_generator::id_to_texture.at(texture));
    }

    for (const auto& texture : level_data.extra_textures){
      textures.push_back(texture);
    }
  };

  const auto loop = [](auto& app){
    level_controller(app);
    render_level(app.current_level);
  };

  run_frame(app, level_data.frame, textures, setup, loop);
}
