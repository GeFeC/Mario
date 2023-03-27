#pragma once

#include "States/LevelState.hpp"
#include "Renderer/Renderer.hpp"

#include "Views/Components.hpp"

#include "Util/Util.hpp"
#include "config.hpp"
#include "res/fonts.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <sstream>
#include <iomanip>

static auto render_clouds(const LevelState& level, const glm::vec2& screen_scroll){
  auto& clouds = level.background.clouds;

  static auto cloud_offset = 0.f;
  cloud_offset += window::delta_time;

  if (cloud_offset >= 18.f) cloud_offset -= 18.f;

  for (const auto [position, cloud_size] : clouds){
    const auto x = position.x - cloud_offset;
    const auto y = position.y;

    render_block(BlockState({ x , y  }, &textures::red_cloud_top_left), screen_scroll);
    render_block(BlockState({ x , (y + 1)  }, &textures::red_cloud_bottom_left), screen_scroll);

    for (int i = 0; i < cloud_size; ++i){
      render_block(BlockState({ (x + i + 1) , y  }, &textures::red_cloud_top_center), screen_scroll);
      render_block(BlockState({ (x + i + 1) , (y + 1)  }, &textures::red_cloud_bottom_center), screen_scroll);
    }

    render_block(BlockState({ (x + cloud_size + 1) , y  }, &textures::red_cloud_top_right), screen_scroll);
    render_block(BlockState({ (x + cloud_size + 1) , (y + 1)  }, &textures::red_cloud_bottom_right), screen_scroll);
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
  static constexpr auto WindowWidth = config::InitialWindowWidth;

  auto text = Text(&fonts::normal, "MARIO           WORLD   TIME", font_size / fonts::normal.size);
  const auto center_x = WindowWidth / 2 - text.get_size().x / 2;
  const auto step_y = 2.f / 3.f * config::BlockSize;

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

static auto render_plants(const LevelState& level, const glm::vec2& screen_scroll){
  for (const auto& plant : level.entities.plants){
    render_entity(plant, screen_scroll);
  }

  for (const auto& plant : level.entities.red_plants){
    render_entity(plant, screen_scroll);
  }
}

static auto render_entities(const LevelState& level, const glm::vec2& screen_scroll){
  for (const auto& goomba : level.entities.goombas){
    render_entity(goomba, screen_scroll);
  }

  for (const auto& goomba : level.entities.red_goombas){
    render_entity(goomba, screen_scroll);
  }

  for (const auto& goomba : level.entities.yellow_goombas){
    render_entity(goomba, screen_scroll);
  }

  for (const auto& koopa : level.entities.green_koopas){
    render_entity(koopa, screen_scroll);
  }

  for (const auto& koopa : level.entities.green_jumping_koopas){
    render_entity(koopa, screen_scroll);
  }

  for (const auto& koopa : level.entities.green_flying_koopas){
    render_entity(koopa, screen_scroll);
  }

  for (const auto& koopa : level.entities.red_koopas){
    render_entity(koopa, screen_scroll);
  }

  for (const auto& koopa : level.entities.red_jumping_koopas){
    render_entity(koopa, screen_scroll);
  }

  for (const auto& koopa : level.entities.red_flying_koopas){
    render_entity(koopa, screen_scroll);
  }

  for (const auto& mushroom : level.entities.mushrooms){
    render_entity(mushroom, screen_scroll);
  }

  for (const auto& mushroom : level.entities.green_mushrooms){
    render_entity(mushroom, screen_scroll);
  }

  for (const auto& beetle : level.entities.beetles){
    render_entity(beetle, screen_scroll);
  }

  for (const auto& spike : level.entities.spikes){
    render_entity(spike, screen_scroll);
  }

  for (const auto& bro : level.entities.hammerbros){
    render_hammerbro(bro, screen_scroll);
  }
}

static auto render_all_points_particles(const LevelState& level, const glm::vec2& screen_scroll){
  for (const auto& plant : level.entities.plants){
    render_points_particles(plant.points_generator.items, screen_scroll);
  }

  for (const auto& plant : level.entities.red_plants){
    render_points_particles(plant.points_generator.items, screen_scroll);
  }

  for (const auto& koopa : level.entities.green_koopas){
    render_points_particles(koopa.points_generator.items, screen_scroll);
  }

  for (const auto& koopa : level.entities.green_jumping_koopas){
    render_points_particles(koopa.points_generator.items, screen_scroll);
  }

  for (const auto& koopa : level.entities.green_flying_koopas){
    render_points_particles(koopa.points_generator.items, screen_scroll);
  }

  for (const auto& koopa : level.entities.red_koopas){
    render_points_particles(koopa.points_generator.items, screen_scroll);
  }

  for (const auto& koopa : level.entities.red_jumping_koopas){
    render_points_particles(koopa.points_generator.items, screen_scroll);
  }

  for (const auto& koopa : level.entities.red_flying_koopas){
    render_points_particles(koopa.points_generator.items, screen_scroll);
  }

  for (const auto& goomba : level.entities.goombas){
    render_points_particles(goomba.points_generator.items, screen_scroll);
  }

  for (const auto& goomba : level.entities.red_goombas){
    render_points_particles(goomba.points_generator.items, screen_scroll);
  }

  for (const auto& goomba : level.entities.yellow_goombas){
    render_points_particles(goomba.points_generator.items, screen_scroll);
  }

  for (const auto& beetle : level.entities.beetles){
    render_points_particles(beetle.points_generator.items, screen_scroll);
  }

  for (const auto& spike : level.entities.spikes){
    render_points_particles(spike.points_generator.items, screen_scroll);
  }

  for (const auto& mushroom : level.entities.mushrooms){
    render_points_particles(mushroom.points_generator.items, screen_scroll);
  }

  for (const auto& mushroom : level.entities.green_mushrooms){
    render_points_particles(mushroom.points_generator.items, screen_scroll);
  }

  for (const auto& block : level.blocks.q_blocks){
    render_points_particles(block.points_generator.items, screen_scroll);
  }

  for (const auto& flower : level.blocks.fire_flowers){
    render_points_particles(flower.points_generator.items, screen_scroll);
  }

  for (const auto& bro : level.entities.hammerbros){
    render_points_particles(bro.points_generator.items, screen_scroll);
  }
}

static auto render_loading_screen(const LevelState& level){
  const auto font_size = fonts::normal.size * 3.5f;

  //Background;
  renderer::draw(Drawable{
    glm::vec2(0, 0),
    glm::vec2(config::InitialWindowWidth, config::InitialWindowHeight),
    &textures::black
  });

  //Header:
  static constexpr auto WindowWidth = config::InitialWindowWidth;
  static constexpr auto WindowHeight = config::InitialWindowHeight;

  const auto& stats = level.stats;

  auto text = Text(
    &fonts::normal, 
    "WORLD " + std::to_string(stats.level_major) + "-" + std::to_string(stats.level_minor),
    font_size / fonts::normal.size
  );

  const auto center_pos = glm::vec2(WindowWidth, WindowHeight) / 2.f - text.get_size() * 0.5f;

  text.set_position(center_pos - glm::vec2(0, font_size * 2.5f));

  renderer::print(text, glm::vec2(0));

  //Mario:
  renderer::draw(Drawable{
    glm::vec2(
      WindowWidth / 2 - config::BlockSize * 1.5f, 
      center_pos.y - (config::BlockSize - font_size * 1.f) 
    ),
    glm::vec2(config::BlockSize),
    &textures::small_mario
  });

  //HP:
  text.set_text("X " + std::to_string(stats.hp));
  text.set_position(glm::vec2(WindowWidth / 2, center_pos.y));

  renderer::print(text, glm::vec2(0));
}

static auto render_blocks(const LevelState& level, const glm::vec2& screen_scroll){
  for (const auto& block : level.blocks.normal){
    render_block(block, screen_scroll);
  }

  for (const auto& block : level.blocks.coins){
    render_block(block, screen_scroll);
  }
  
  for (const auto& block : level.blocks.spinning_coins){
    render_block(block, screen_scroll);
  }

  for (const auto& block : level.blocks.fire_flowers){
    render_block(block, screen_scroll);
  }
}

static auto get_screen_scroll(const LevelState& level){
  static constexpr auto CameraOffsetFromPlayer = config::BlockSize * 6.f;

  auto screen_scroll = glm::vec2(0.f);
  auto& player = level.player;

  if (player.position.x >= config::PlayerPositionToScroll.x && level.type == LevelState::Type::Horizontal){
    screen_scroll.x = std::min(
      player.position.x - config::PlayerPositionToScroll.x,
      config::HorizontalLevelWidth * config::BlockSize - (config::PlayerPositionToScroll.x + config::BlockSize) * 2
    );
  }

  if (level.type == LevelState::Type::Vertical){
    screen_scroll.y = std::clamp(level.camera_offset_y, 50.f * config::BlockSize, LevelState::MaxLevelScrollY + 50.f * config::BlockSize);
  }

  return screen_scroll;
}

static auto render_level(const LevelState& level){
  const auto screen_scroll = get_screen_scroll(level);

  renderer::draw(Drawable{
    glm::vec2(0, 0),
    glm::vec2(config::InitialWindowWidth, config::InitialWindowHeight),
    &textures::mushroom_bg
  });

  renderer::draw_with_shadow([&]{
    render_clouds(level, screen_scroll);
  });

  renderer::draw_with_shadow([&]{
    if (level.is_finished) render_player(level.player, screen_scroll);

    for (const auto& hill : level.background.hills){
      render_block(hill, screen_scroll);
    }
  });

  renderer::draw_with_shadow([&]{
    for (const auto& bush : level.background.bushes){
      render_block(bush, screen_scroll);
    }
  });

  renderer::draw_with_shadow([&]{
    render_plants(level, screen_scroll);
  });

  renderer::draw_with_shadow([&]{
    render_blocks(level, screen_scroll);
  });

  renderer::draw_with_shadow([&]{
    render_entities(level, screen_scroll);

    for (const auto& block : level.blocks.q_blocks){
      render_block(block, screen_scroll);
    }

    for (const auto& block : level.blocks.bricks){
      render_bricks(block, screen_scroll);
    }

    for (const auto& bar : level.fire_bars){
      render_fire_bar(bar, screen_scroll);
    }

  });

  renderer::draw_with_shadow([&]{
    if (level.type == LevelState::Type::Boss){
      render_boss(*level.bosses.king_goomba, screen_scroll);
    }
  });

  renderer::draw_with_shadow([&]{
    if (!level.is_finished) render_player(level.player, screen_scroll);

    if (level.load_delay > 0.f){
      render_loading_screen(level);
    }
    render_stats(level);
  });

  //Points particles
  renderer::draw_with_shadow([&]{
    render_all_points_particles(level, screen_scroll);  
  });
}
