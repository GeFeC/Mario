#pragma once

#include "res/textures.hpp"

namespace texture_allocators{
  inline auto coin = TextureAllocator({
    &textures::coin[0],
    &textures::coin[1],
    &textures::coin[2]
  });

  inline auto spinning_coin = TextureAllocator({
    &textures::spinning_coin[0],
    &textures::spinning_coin[1],
    &textures::spinning_coin[2],
    &textures::spinning_coin[3]
  });

  inline auto goomba = TextureAllocator({
    &textures::goomba_dead,
    &textures::goomba_walk[0],
    &textures::goomba_walk[1]
  });

  inline auto red_goomba = TextureAllocator({
    &textures::red_goomba_dead,
    &textures::red_goomba_walk[0],
    &textures::red_goomba_walk[1]
  });

  inline auto yellow_goomba = TextureAllocator({
    &textures::yellow_goomba_dead,
    &textures::yellow_goomba_walk[0],
    &textures::yellow_goomba_walk[1]
  });

  inline auto green_koopa = TextureAllocator({
    &textures::green_koopa_dead,
    &textures::green_koopa_walk[0],
    &textures::green_koopa_walk[1]
  });

  inline auto red_koopa = TextureAllocator({
    &textures::red_koopa_dead,
    &textures::red_koopa_walk[0],
    &textures::red_koopa_walk[1]
  });

  inline auto beetle = TextureAllocator({
    &textures::beetle_dead,
    &textures::beetle_walk[0],
    &textures::beetle_walk[1]
  });

  inline auto spike = TextureAllocator({
    &textures::spike_walk[0],
    &textures::spike_walk[1]
  });

  inline auto q_block = TextureAllocator({
    &textures::q_block[0],
    &textures::q_block[1],
    &textures::q_block[2],
    &textures::null_block
  });

  inline auto fire_flower = TextureAllocator({
    &textures::fire_flower[0],
    &textures::fire_flower[1],
    &textures::fire_flower[2],
    &textures::fire_flower[3]
  });

  inline auto bricks = TextureAllocator({
    &textures::bricks,
    &textures::bricks_particles_normal
  });

  inline auto mario = TextureAllocator({
    &textures::small_mario_walk[0],
    &textures::small_mario_walk[1],
    &textures::small_mario_walk[2],
    &textures::small_mario,
    &textures::small_mario_dead,
    &textures::small_mario_jumping,
    &textures::small_mario_turning,
    &textures::big_mario_walk[0],
    &textures::big_mario_walk[1],
    &textures::big_mario_walk[2],
    &textures::big_mario,
    &textures::big_mario_jumping,
    &textures::big_mario_turning,
    &textures::big_mario_squating,
    &textures::fire_big_mario_walk[0],
    &textures::fire_big_mario_walk[1],
    &textures::fire_big_mario_walk[2],
    &textures::fire_big_mario,
    &textures::fire_big_mario_jumping,
    &textures::fire_big_mario_turning,
    &textures::fire_big_mario_squating,
    &textures::black_big_mario_walk[0],
    &textures::black_big_mario_walk[1],
    &textures::black_big_mario_walk[2],
    &textures::black_big_mario,
    &textures::black_big_mario_jumping,
    &textures::black_big_mario_turning,
    &textures::black_big_mario_squating,
    &textures::medium_mario,
    &textures::fireball[0],
    &textures::fireball[1],
    &textures::fireball[2],
    &textures::fireball[3],
    &textures::explosion[0],
    &textures::explosion[1],
    &textures::explosion[2]
  });

  inline auto mushroom = TextureAllocator({
    &textures::mushroom_center,
    &textures::mushroom_bot,
    &textures::mushroom_left,
    &textures::mushroom_right
  });

  inline auto red_cloud = TextureAllocator({
    &textures::red_cloud_top_left,
    &textures::red_cloud_top_right,
    &textures::red_cloud_top_center,
    &textures::red_cloud_bottom_left,
    &textures::red_cloud_bottom_right,
    &textures::red_cloud_bottom_center
  });

  inline auto red_bush = TextureAllocator({
    &textures::red_bush_left,
    &textures::red_bush_right,
    &textures::red_bush_center
  });

  inline auto red_hill = TextureAllocator({
    &textures::red_hill_top,
    &textures::red_hill_left,
    &textures::red_hill_right,
    &textures::red_hill_center,
    &textures::red_hill_center_dot
  });

  inline auto red_pipe = TextureAllocator({
    &textures::red_pipe_top_left,
    &textures::red_pipe_top_right,
    &textures::red_pipe_bottom_left,
    &textures::red_pipe_bottom_right
  });
}
