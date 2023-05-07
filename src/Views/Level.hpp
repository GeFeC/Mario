#pragma once

#include "States/LevelState.hpp"
#include "Renderer/Renderer.hpp"
#include "Controllers/LevelController.hpp"

#include "Views/Components.hpp"

#include "Util/Util.hpp"
#include "config.hpp"
#include "res/fonts.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <sstream>
#include <iomanip>

static auto render_clouds(const LevelState& level){
  const auto& clouds = level.background.clouds;
  const auto& cloud_textures = *level.cloud_textures;

  const auto& top_left = cloud_textures[0];
  const auto& top_right = cloud_textures[1];
  const auto& top_center = cloud_textures[2];
  const auto& bottom_left = cloud_textures[3];
  const auto& bottom_right = cloud_textures[4];
  const auto& bottom_center = cloud_textures[5];

  static auto cloud_offset = 0.f;
  cloud_offset += window::delta_time;

  if (cloud_offset >= 18.f) cloud_offset -= 18.f;

  for (const auto [position, cloud_size] : clouds){
    const auto x = position.x - cloud_offset;
    const auto y = position.y;

    render_block(BlockState({ x, y  }, &top_left), level.camera_offset);
    render_block(BlockState({ x, (y + 1)  }, &bottom_left), level.camera_offset);

    for (int i = 0; i < cloud_size; ++i){
      render_block(BlockState({ (x + i + 1), y  }, &top_center), level.camera_offset);
      render_block(BlockState({ (x + i + 1), (y + 1)  }, &bottom_center), level.camera_offset);
    }

    render_block(BlockState({ (x + cloud_size + 1), y  }, &top_right), level.camera_offset);
    render_block(BlockState({ (x + cloud_size + 1), (y + 1)  }, &bottom_right), level.camera_offset);
  }
}

static auto render_stats(const LevelState& level){
  const auto& stats = level.stats;

  auto body = std::ostringstream(); 
  body 
    << std::setw(6) << std::setfill('0') << stats.score << "    x"
    << std::setw(2) << std::setfill('0') << stats.coins << "    "
    << stats.level_major << '-' << stats.level_minor << "     "
    << std::setw(3) << std::setfill('0') << stats.time;

  const auto font_size = fonts::normal.size * 3.5f;
  static constexpr auto WindowWidth = config::FrameBufferSize.x;

  auto text = Text(&fonts::normal, "MARIO           WORLD   TIME", font_size / fonts::normal.size);
  const auto center_x = WindowWidth / 2 - text.get_size().x / 2;
  const auto step_y = 2.f / 3.f * BlockBase::Size;

  text.set_position({ center_x, step_y });
  renderer::print(text, glm::vec2(0));

  text.set_text(body.str());
  text.set_position(text.get_position() + glm::vec2(0.f, step_y));
  renderer::print(text, glm::vec2(0));

  //Mini coin
  renderer::draw(Drawable{
    glm::vec2(center_x + 9 * font_size, step_y * 2),
    glm::vec2(font_size),
    &textures::mini_coin
  });

  if (level.type == LevelState::Type::Boss && stats.boss_hp != nullptr){
    text.set_text("BOSS");
    text.set_position(glm::vec2(WindowWidth / 2.f - text.get_size().x / 2.f, step_y * 4));

    renderer::print(text, glm::vec2(0));

    static constexpr auto MaxBossBarSize = glm::vec2(100.f * 6.f, 50.f);

    renderer::draw_plain(PlainDrawable{
      glm::vec2(WindowWidth / 2 - MaxBossBarSize.x / 2, step_y * 5),
      glm::vec2(*stats.boss_hp * MaxBossBarSize.x / stats.max_boss_hp, MaxBossBarSize.y),
      glm::vec4(1.f)
    });
  }
}

static auto render_plants(const LevelState& level){
  for (const auto& plant : level.entities.plants){
    render_entity(plant, level.camera_offset);
  }

  for (const auto& plant : level.entities.red_plants){
    render_entity(plant, level.camera_offset);
  }
}

static auto render_entities(const LevelState& level){
  level.entities.for_each([&](const auto& entity){
    if constexpr (std::is_convertible_v<decltype(entity), PlantState>) return;

    render_entity(entity, level.camera_offset);
  });
}

static auto render_all_points_particles(const LevelState& level){
  level.entities.for_each([&](const auto& entity){
    render_points_particles(entity.points_generator.items, level.camera_offset);
  });

  for (const auto& block : level.blocks.q_blocks_with_coins){
    render_points_particles(block.pusher.points_generator.items, level.camera_offset);
  }

  for (const auto& block : level.blocks.q_blocks_with_mushroom){
    render_points_particles(block.pusher.entity.points_generator.items, level.camera_offset);
  }

  for (const auto& block : level.blocks.q_blocks_with_goomba){
    render_points_particles(block.pusher.entity.points_generator.items, level.camera_offset);
  }

  for (const auto& block : level.blocks.q_blocks_with_flower){
    render_points_particles(block.pusher.fire_flower.points_generator.items, level.camera_offset);
  }
}

static auto render_loading_screen(const LevelState& level){
  const auto font_size = fonts::normal.size * 3.5f;

  //Background;
  renderer::draw(Drawable{
    glm::vec2(0, 0),
    config::FrameBufferSize,
    &textures::black
  });

  //Header:
  const auto& stats = level.stats;

  auto text = Text(
    &fonts::normal, 
    "WORLD " + std::to_string(stats.level_major) + "-" + std::to_string(stats.level_minor),
    font_size / fonts::normal.size
  );

  const auto center_pos = config::FrameBufferSize / 2.f - text.get_size() * 0.5f;

  text.set_position(center_pos - glm::vec2(0, font_size * 2.5f));

  renderer::print(text, glm::vec2(0));

  //Mario:
  renderer::draw(Drawable{
    glm::vec2(
      config::FrameBufferSize.x / 2 - BlockBase::Size * 1.5f, 
      center_pos.y - (BlockBase::Size - font_size * 1.f) 
    ),
    glm::vec2(BlockBase::Size),
    &textures::small_mario
  });

  //HP:
  text.set_text("X " + std::to_string(stats.hp));
  text.set_position(glm::vec2(config::FrameBufferSize.x / 2, center_pos.y));

  renderer::print(text, glm::vec2(0));
}

static auto render_blocks(const LevelState& level){
  for (const auto& block : level.blocks.normal){
    render_block(block, level.camera_offset);
  }

  for (const auto& block : level.blocks.coins){
    render_block(block, level.camera_offset);
  }
}

static auto render_level(const LevelState& level){
  renderer::draw(Drawable{
    glm::vec2(0, 0),
    config::FrameBufferSize,
    level.background_texture
  });

  renderer::draw_with_shadow([&]{
    render_clouds(level);
  });

  renderer::draw_with_shadow([&]{
    if (level.is_finished) render_player(level.player, level.camera_offset);

    for (const auto& hill : level.background.hills){
      render_block(hill, level.camera_offset);
    }
  });

  renderer::draw_with_shadow([&]{
    for (const auto& bush : level.background.bushes){
      render_block(bush, level.camera_offset);
    }
  });

  renderer::draw_with_shadow([&]{
    render_plants(level);
  });

  renderer::draw_with_shadow([&]{
    render_blocks(level);
  });

  renderer::draw_with_shadow([&]{
    for (const auto& platform : level.platforms){
      render_platform(platform, level.camera_offset);
    }

    for (const auto& platform : level.looped_platforms){
      render_platform(platform, level.camera_offset);
    }
  });

  renderer::draw_with_shadow([&]{
    render_entities(level);

    for (const auto& block : level.blocks.q_blocks_with_coins){
      render_q_block_with_coins(block, level.camera_offset);
    }

    level.blocks.for_each_q_block([&](const auto& block){
      using pusher_t = std::decay_t<decltype(block.pusher)>;
      if constexpr (pusher_t::ContainsEntity){
        render_q_block_with_entity(block, level.camera_offset);
      }
    });

    for (const auto& block : level.blocks.q_blocks_with_flower){
      render_q_block_with_flower(block, level.camera_offset);
    }

    for (const auto& block : level.blocks.bricks){
      render_bricks(block, level.camera_offset);
    }

    for (const auto& bar : level.fire_bars){
      render_fire_bar(bar, level.camera_offset);
    }

  });

  renderer::draw_with_shadow([&]{
    if (level.type == LevelState::Type::Boss){
      render_boss(*level.bosses.current_boss.lock(), level.camera_offset);
    }
  });

  renderer::draw_with_shadow([&]{
    if (!level.is_finished) render_player(level.player, level.camera_offset);

    if (level.load_delay > 0.f){
      render_loading_screen(level);
    }
    render_stats(level);
  });

  //Points particles
  renderer::draw_with_shadow([&]{
    render_all_points_particles(level);  
  });
}
