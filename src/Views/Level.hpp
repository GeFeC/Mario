#pragma once

#include "States/LevelState.hpp"
#include "Renderer/Renderer.hpp"

#include "Views/Components.hpp"

#include "Util.hpp"
#include "config.hpp"
#include "res/fonts.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <sstream>
#include <iomanip>

static auto render_clouds(float screen_scroll){
  auto clouds = std::vector<std::pair<glm::vec2, int>>();
  clouds.reserve(64);

  for (int i = 0; i < 16; ++i){
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 1, 2), 3));
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 7, 1), 2));
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 12, 2), 1));
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 15, 1), 1));
  }

  static auto cloud_offset = 0.f;
  cloud_offset += window::delta_time;
  if (cloud_offset > 18.f) cloud_offset -= 18.f;

  for (const auto [position, cloud_size] : clouds){
    const auto x = position.x - cloud_offset;
    const auto y = position.y;

    render_block(BlockState({ x , y  }, &textures::red_cloud_top_left), screen_scroll);
    render_block(BlockState({ x , (y + 1)  }, &textures::red_cloud_bottom_left), screen_scroll);

    for (int i = 0; i < cloud_size; ++i){
      render_block(BlockState({ (x + i + 1) , y  }, &textures::red_cloud_top_center), screen_scroll);
      render_block(BlockState({ (x + i + 1) , (y + 1)  }, &textures::red_cloud_bottom_center), screen_scroll);
    }

    render_block(BlockState( { (x + cloud_size + 1) , y  }, &textures::red_cloud_top_right ), screen_scroll);
    render_block(BlockState( { (x + cloud_size + 1) , (y + 1)  }, &textures::red_cloud_bottom_right ), screen_scroll);
  }
}

static auto render_bushes(const std::vector<BackgroundObjectState>& bushes, float screen_scroll){
  for (const auto [position, bush_size] : bushes){
    const auto x = position.x;
    const auto y = position.y;

    render_block(BlockState({ x , y  }, &textures::red_bush_left), screen_scroll);

    for (int i = 0; i < bush_size; ++i){
      render_block(BlockState({ (x + i + 1) , y  }, &textures::red_bush_center), screen_scroll);
    }

    render_block(BlockState({ (x + bush_size + 1) , y  }, &textures::red_bush_right), screen_scroll);
  }
}

static auto render_hills(const std::vector<BackgroundObjectState>& hills, float screen_scroll){
  for (auto [position, hill_size] : hills){
    auto x = position.x;
    auto y = position.y;

    while(hill_size > 0){
      render_block(BlockState({ x , y  }, &textures::red_hill_left), screen_scroll);

      for (int i = 0; i < hill_size * 2 - 1; ++i){
        const auto texture = i % 2 ? &textures::red_hill_center_dot : &textures::red_hill_center;
        render_block(BlockState({ (x + i + 1) , y  }, texture), screen_scroll);
      }

      render_block(BlockState({ (x + hill_size * 2) , y  }, &textures::red_hill_right), screen_scroll);

      --hill_size;
      ++x;
      --y;
    }

    render_block(BlockState({ x , y  }, &textures::red_hill_top), screen_scroll);
  }
}

static auto render_stats(const StatsState& stats){
  auto body = std::ostringstream(); 
  body 
    << std::setw(6) << std::setfill('0') << stats.score << "    x"
    << std::setw(2) << std::setfill('0') << stats.coins << "    "
    << stats.level_major << '-' << stats.level_major << "     "
    << std::setw(3) << std::setfill('0') << stats.time;

  auto text = Text(&fonts::medium, "MARIO           WORLD   TIME");
  text.set_position(glm::vec2(154, 40));
  renderer::print(text);

  text.set_text(body.str());
  text.set_position(text.get_position() + glm::vec2(0.f, 40));
  renderer::print(text);

  renderer::draw(Drawable{
    glm::vec2(440, 80),
    glm::vec2(32),
    &textures::mini_coin
  });
}

static auto render_entities(const LevelState& level, float screen_scroll){
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

  for (const auto& koopa : level.entities.red_koopas){
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
}

static auto render_all_points_particles(const LevelState& level, float screen_scroll){
  for (const auto& koopa : level.entities.green_koopas){
    render_points_particles(koopa.points_manager.points, screen_scroll);
  }

  for (const auto& koopa : level.entities.red_koopas){
    render_points_particles(koopa.points_manager.points, screen_scroll);
  }

  for (const auto& goomba : level.entities.goombas){
    render_points_particles(goomba.points_manager.points, screen_scroll);
  }

  for (const auto& goomba : level.entities.red_goombas){
    render_points_particles(goomba.points_manager.points, screen_scroll);
  }

  for (const auto& goomba : level.entities.yellow_goombas){
    render_points_particles(goomba.points_manager.points, screen_scroll);
  }

  for (const auto& beetle : level.entities.beetles){
    render_points_particles(beetle.points_manager.points, screen_scroll);
  }

  for (const auto& spike : level.entities.spikes){
    render_points_particles(spike.points_manager.points, screen_scroll);
  }

  for (const auto& mushroom : level.entities.mushrooms){
    render_points_particles(mushroom.points_manager.points, screen_scroll);
  }

  for (const auto& mushroom : level.entities.green_mushrooms){
    render_points_particles(mushroom.points_manager.points, screen_scroll);
  }

  for (const auto& block : level.blocks.q_blocks){
    render_points_particles(block.points_manager.points, screen_scroll);
  }

  for (const auto& flower : level.blocks.fire_flowers){
    render_points_particles(flower.points_manager.points, screen_scroll);
  }
}

static auto render_loading_screen(const LevelState& level){
  //Background;
  renderer::draw(Drawable{
    glm::vec2(0, 0),
    glm::vec2(config::InitialWindowWidth, config::InitialWindowHeight),
    &textures::black
  });

  //Header:

  const auto window_width = config::InitialWindowWidth;

  const auto& stats = level.stats;
  auto text = Text(&fonts::medium, "WORLD " + std::to_string(stats.level_major) + "-" + std::to_string(stats.level_minor));
  text.set_position(glm::vec2(460, 250));

  renderer::print(text);

  //Mario:
  renderer::draw(Drawable{
    glm::vec2(window_width / 2 - 90, 320),
    glm::vec2(60),
    &textures::small_mario
  });

  //HP:
  text.set_text("X " + std::to_string(stats.hp));
  text.set_position(text.get_position() + glm::vec2(140, 100));

  renderer::print(text);
}

static auto render_blocks(const LevelState& level, float screen_scroll){
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

static auto render_level(const LevelState& level){
  auto screen_scroll = 0.f;
  if (level.should_screen_scroll){
    screen_scroll = level.player.position.x - config::PlayerPositionToScroll;
  }

  renderer::draw(Drawable{
    glm::vec2(0, 0),
    glm::vec2(config::InitialWindowWidth, config::InitialWindowHeight),
    &textures::mushroom_bg
  });

  renderer::draw_with_shadow([&]{
    render_hills(level.background.hills, screen_scroll);
  });

  renderer::draw_with_shadow([&]{
    render_clouds(screen_scroll);
    render_bushes(level.background.bushes, screen_scroll);
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
  });

  renderer::draw_with_shadow([&]{
    render_player(level.player, screen_scroll);

    if (level.load_delay > 0.f){
      render_loading_screen(level);
    }
    render_stats(level.stats);
  });

  //Points particles
  renderer::draw_with_shadow([&]{
    render_all_points_particles(level, screen_scroll);  
  });
}
