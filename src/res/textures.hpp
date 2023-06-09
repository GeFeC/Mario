#pragma once

#include "Renderer/Texture.hpp"

#include <array>

namespace mario::textures{

using renderer::Texture;

inline auto black = Texture("black.png");

inline auto bg = Texture("normal_background.png");
inline auto mushroom_bg = Texture("mushroom_background.png");
inline auto snow_bg = Texture("snow_background.png");

inline auto platform = Texture("platform.png");

//Blocks and Background objects
inline auto bricks_particles_normal = Texture("bricks_particle/normal.png");

inline auto bricks = Texture("tile/bricks.png");  
inline auto dirt = Texture("tile/dirt.png");
inline auto stone = Texture("tile/stone.png");
inline auto green_ground = Texture("tile/greenground.png");
inline auto water_ground = Texture("tile/waterground.png");
inline auto coral = Texture("tile/coral.png");
inline auto bridge = Texture("tile/bridge.png");

inline auto water_top = Texture("tile/water1.png");
inline auto water_bottom = Texture("tile/water2.png");

inline auto mushroom_bot1 = Texture("tile/mushroom_bot1.png");
inline auto mushroom_bot2 = Texture("tile/mushroom_bot2.png");
inline auto mushroom_center = Texture("tile/mushroom_center.png");
inline auto mushroom_left = Texture("tile/mushroom_left.png");
inline auto mushroom_right = Texture("tile/mushroom_right.png");

inline auto grass_bot = Texture("tile/grass_bot.png");
inline auto grass_center = Texture("tile/grass_center.png");
inline auto grass_left = Texture("tile/grass_left.png");
inline auto grass_right = Texture("tile/grass_right.png");

inline auto snow_bot = Texture("tile/snow_bot.png");
inline auto snow_center = Texture("tile/snow_center.png");
inline auto snow_left = Texture("tile/snow_left.png");
inline auto snow_right = Texture("tile/snow_right.png");

inline auto red_cloud_top_left = Texture("cloud/left_top.png");
inline auto red_cloud_top_center = Texture("cloud/center_top.png");
inline auto red_cloud_top_right = Texture("cloud/right_top.png");
inline auto red_cloud_bottom_left = Texture("cloud/left_bottom.png");
inline auto red_cloud_bottom_center = Texture("cloud/center_bottom.png");
inline auto red_cloud_bottom_right = Texture("cloud/right_bottom.png");

inline auto blue_cloud_top_left = Texture("cloud/blue_left_top.png");
inline auto blue_cloud_top_center = Texture("cloud/blue_center_top.png");
inline auto blue_cloud_top_right = Texture("cloud/blue_right_top.png");
inline auto blue_cloud_bottom_left = Texture("cloud/blue_left_bottom.png");
inline auto blue_cloud_bottom_center = Texture("cloud/blue_center_bottom.png");
inline auto blue_cloud_bottom_right = Texture("cloud/blue_right_bottom.png");

inline auto white_bush_center = Texture("bush/snow_center.png");
inline auto white_bush_left = Texture("bush/snow_left.png");
inline auto white_bush_right = Texture("bush/snow_right.png");

inline auto green_bush_center = Texture("bush/center.png");
inline auto green_bush_left = Texture("bush/left.png");
inline auto green_bush_right = Texture("bush/right.png");

inline auto red_bush_center = Texture("bush/mush_center.png");
inline auto red_bush_left = Texture("bush/mush_left.png");
inline auto red_bush_right = Texture("bush/mush_right.png");

inline auto white_hill_center = Texture("hill/snow_center.png");
inline auto white_hill_center_dot = Texture("hill/snow_center_dot.png");
inline auto white_hill_left = Texture("hill/snow_left.png");
inline auto white_hill_right = Texture("hill/snow_right.png");
inline auto white_hill_top = Texture("hill/snow_top.png");

inline auto red_hill_center = Texture("hill/mush_center.png");
inline auto red_hill_center_dot = Texture("hill/mush_center_dot.png");
inline auto red_hill_left = Texture("hill/mush_left.png");
inline auto red_hill_right = Texture("hill/mush_right.png");
inline auto red_hill_top = Texture("hill/mush_top.png");

inline auto green_hill_center = Texture("hill/center.png");
inline auto green_hill_center_dot = Texture("hill/center_dot.png");
inline auto green_hill_left = Texture("hill/left.png");
inline auto green_hill_right = Texture("hill/right.png");
inline auto green_hill_top = Texture("hill/top.png");

inline auto red_pipe_top_left = Texture("pipe/red_left_top.png");
inline auto red_pipe_top_right = Texture("pipe/red_right_top.png");
inline auto red_pipe_bottom_left = Texture("pipe/red_left_bot.png");
inline auto red_pipe_bottom_right = Texture("pipe/red_right_bot.png");

inline auto green_pipe_top_left = Texture("pipe/green_left_top.png");
inline auto green_pipe_top_right = Texture("pipe/green_right_top.png");
inline auto green_pipe_bottom_left = Texture("pipe/green_left_bottom.png");
inline auto green_pipe_bottom_right = Texture("pipe/green_right_bottom.png");

inline auto white_pipe_top_left = Texture("pipe/white_left_top.png");
inline auto white_pipe_top_right = Texture("pipe/white_right_top.png");
inline auto white_pipe_bottom_left = Texture("pipe/white_left_bottom.png");
inline auto white_pipe_bottom_right = Texture("pipe/white_right_bottom.png");

inline auto coin = std::array<Texture, 3>{
  Texture("coin/0.png"),
  Texture("coin/1.png"),
  Texture("coin/2.png")
};

inline auto q_block = std::array<Texture, 3>{
  Texture("q_block/0.png"),
  Texture("q_block/1.png"),
  Texture("q_block/2.png"),
};

inline auto spinning_coin = std::array<Texture, 4>{
  Texture("spinning_coin/0.png"),
  Texture("spinning_coin/1.png"),
  Texture("spinning_coin/2.png"),
  Texture("spinning_coin/3.png"),
};

inline auto null_block = Texture("q_block/used.png");

//Player
inline auto small_mario = Texture("player/mario/small/normal/mario.png");
inline auto small_mario_dead = Texture("player/mario/small/normal/dead.png");
inline auto small_mario_jumping = Texture("player/mario/small/normal/jumping.png");
inline auto small_mario_turning = Texture("player/mario/small/normal/turning.png");

inline auto small_mario_walk = std::array<Texture, 3>{
  Texture("player/mario/small/normal/walk1.png"),
  Texture("player/mario/small/normal/walk2.png"),
  Texture("player/mario/small/normal/walk3.png")
};

inline auto small_mario_swim = std::array<Texture, 2>{
  Texture("player/mario/small/normal/swim1.png"),
  Texture("player/mario/small/normal/swim2.png")
};

inline auto big_mario = Texture("player/mario/big/normal/mario.png");
inline auto big_mario_jumping = Texture("player/mario/big/normal/jumping.png");
inline auto big_mario_squating = Texture("player/mario/big/normal/squating.png");
inline auto big_mario_turning = Texture("player/mario/big/normal/turning.png");

inline auto big_mario_walk = std::array<Texture, 3>{
  Texture("player/mario/big/normal/walk1.png"),
  Texture("player/mario/big/normal/walk2.png"),
  Texture("player/mario/big/normal/walk3.png")
};

inline auto big_mario_swim = std::array<Texture, 2>{
  Texture("player/mario/big/normal/swim1.png"),
  Texture("player/mario/big/normal/swim2.png")
};

inline auto fire_big_mario = Texture("player/mario/big/fire/mario.png");
inline auto fire_big_mario_jumping = Texture("player/mario/big/fire/jumping.png");
inline auto fire_big_mario_squating = Texture("player/mario/big/fire/squating.png");
inline auto fire_big_mario_turning = Texture("player/mario/big/fire/turning.png");

inline auto fire_big_mario_walk = std::array<Texture, 3>{
  Texture("player/mario/big/fire/walk1.png"),
  Texture("player/mario/big/fire/walk2.png"),
  Texture("player/mario/big/fire/walk3.png")
};

inline auto fire_big_mario_swim = std::array<Texture, 2>{
  Texture("player/mario/big/fire/swim1.png"),
  Texture("player/mario/big/fire/swim2.png")
};

inline auto black_big_mario = Texture("player/mario/big/black/mario.png");
inline auto black_big_mario_jumping = Texture("player/mario/big/black/jumping.png");
inline auto black_big_mario_squating = Texture("player/mario/big/black/squating.png");
inline auto black_big_mario_turning = Texture("player/mario/big/black/turning.png");

inline auto black_big_mario_walk = std::array<Texture, 3>{
  Texture("player/mario/big/black/walk1.png"),
  Texture("player/mario/big/black/walk2.png"),
  Texture("player/mario/big/black/walk3.png")
};

inline auto black_big_mario_swim = std::array<Texture, 2>{
  Texture("player/mario/big/black/swim1.png"),
  Texture("player/mario/big/black/swim2.png")
};

inline auto medium_mario = Texture("player/mario/growing.png");

//Entities:
inline auto hammer = std::array<Texture, 4>{
  Texture("entity/hammerbro/hammer1.png"),
  Texture("entity/hammerbro/hammer2.png"),
  Texture("entity/hammerbro/hammer3.png"),
  Texture("entity/hammerbro/hammer4.png"),
};

inline auto fireball = std::array<Texture, 4>{
  Texture("fireball/1.png"),
  Texture("fireball/2.png"),
  Texture("fireball/3.png"),
  Texture("fireball/4.png")
};

inline auto explosion = std::array<Texture, 3>{
  Texture("fireball/explosion1.png"),
  Texture("fireball/explosion2.png"),
  Texture("fireball/explosion3.png")
};

inline auto fire_flower = std::array<Texture, 4>{
  Texture("flower/0.png"),
  Texture("flower/1.png"),
  Texture("flower/2.png"),
  Texture("flower/3.png")
};

inline auto king_goomba_walk = std::array<Texture, 2>{
  Texture("entity/goomba/king0.png"),
  Texture("entity/goomba/king1.png")
};

inline auto squid1 = Texture("entity/squid/1.png");
inline auto squid2 = Texture("entity/squid/2.png");

inline auto grey_fish_swim = std::array<Texture, 2>{
  Texture("entity/fish/grey1.png"),
  Texture("entity/fish/grey2.png")
};

inline auto red_fish_swim = std::array<Texture, 2>{
  Texture("entity/fish/red1.png"),
  Texture("entity/fish/red2.png")
};

inline auto goomba_walk = std::array<Texture, 2>{
  Texture("entity/goomba/0.png"),
  Texture("entity/goomba/1.png")
};
inline auto goomba_dead = Texture("entity/goomba/dead.png");

inline auto red_goomba_walk = std::array<Texture, 2>{
  Texture("entity/goomba/red0.png"),
  Texture("entity/goomba/red1.png")
};
inline auto red_goomba_dead = Texture("entity/goomba/reddead.png");

inline auto yellow_goomba_walk = std::array<Texture, 2>{
  Texture("entity/goomba/yellow0.png"),
  Texture("entity/goomba/yellow1.png")
};
inline auto yellow_goomba_dead = Texture("entity/goomba/yellowdead.png");

inline auto purple_koopa_walk = std::array<Texture, 2>{
  Texture("entity/koopa/purple1.png"),
  Texture("entity/koopa/purple2.png")
};
inline auto purple_koopa_dead = Texture("entity/koopa/purpledead.png");

inline auto purple_flying_koopa_walk = std::array<Texture, 2>{
  Texture("entity/koopa/purple_flying1.png"),
  Texture("entity/koopa/purple_flying2.png")
};

inline auto green_koopa_walk = std::array<Texture, 2>{
  Texture("entity/koopa/green1.png"),
  Texture("entity/koopa/green2.png")
};
inline auto green_koopa_dead = Texture("entity/koopa/greendead.png");

inline auto green_flying_koopa_walk = std::array<Texture, 2>{
  Texture("entity/koopa/green_flying1.png"),
  Texture("entity/koopa/green_flying2.png")
};

inline auto red_koopa_walk = std::array<Texture, 2>{
  Texture("entity/koopa/red1.png"),
  Texture("entity/koopa/red2.png")
};
inline auto red_koopa_dead = Texture("entity/koopa/reddead.png");

inline auto red_flying_koopa_walk = std::array<Texture, 2>{
  Texture("entity/koopa/red_flying1.png"),
  Texture("entity/koopa/red_flying2.png")
};

inline auto beetle_walk = std::array<Texture, 2>{
  Texture("entity/beetle/0.png"),
  Texture("entity/beetle/1.png")
};
inline auto beetle_dead = Texture("entity/beetle/dead.png");

inline auto spike_walk = std::array<Texture, 2>{
  Texture("entity/spike/0.png"),
  Texture("entity/spike/1.png")
};

inline auto hammerbro_walk = std::array<Texture, 2>{
  Texture("entity/hammerbro/1.png"),
  Texture("entity/hammerbro/2.png")
};

inline auto hammerbro_walk_with_hammer = std::array<Texture, 2>{
  Texture("entity/hammerbro/3.png"),
  Texture("entity/hammerbro/4.png")
};

inline auto red_hammerbro_walk = std::array<Texture, 2>{
  Texture("entity/hammerbro/red1.png"),
  Texture("entity/hammerbro/red2.png")
};

inline auto red_hammerbro_walk_with_hammer = std::array<Texture, 2>{
  Texture("entity/hammerbro/red3.png"),
  Texture("entity/hammerbro/red4.png")
};

inline auto plant = std::array<Texture, 2>{
  Texture("entity/plant/0.png"),
  Texture("entity/plant/1.png")
};

inline auto red_plant = std::array<Texture, 2>{
  Texture("entity/plant/red0.png"),
  Texture("entity/plant/red1.png")
};

inline auto mushroom = Texture("entity/mushroom/mushroom.png");
inline auto green_mushroom = Texture("entity/mushroom/green.png");

inline auto mini_coin = Texture("mini.png");

} //namespace mario::textures
