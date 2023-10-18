#pragma once

#include "Renderer/Texture.hpp"

#include <array>

namespace mario::textures{

using renderer::Texture;

static auto darkness_view = Texture("darkness_view.png");

static auto black = Texture("black.png");

static auto bg = Texture("normal_background.png");
static auto mushroom_bg = Texture("mushroom_background.png");
static auto snow_bg = Texture("snow_background.png");
static auto underground_bg = Texture("underground_background.png");
static auto sky_bg = Texture("sky_background.png");

static auto platform = Texture("platform.png");

//Blocks and Background objects
static auto bricks_particles_normal = Texture("bricks_particle/normal.png");
static auto bricks_particles_blue = Texture("bricks_particle/blue.png");

static auto bricks = Texture("tile/bricks.png");  
static auto blue_bricks = Texture("tile/bluebricks.png");  
static auto dirt = Texture("tile/dirt.png");
static auto blue_dirt = Texture("tile/bluedirt.png");

static auto stone = Texture("tile/stone.png");
static auto blue_stone = Texture("tile/bluestone.png");
static auto green_ground = Texture("tile/greenground.png");
static auto water_ground = Texture("tile/waterground.png");
static auto coral = Texture("tile/coral.png");
static auto bridge = Texture("tile/bridge.png");
static auto cloud_block = Texture("tile/cloud.png");
static auto cloud_unstable = Texture("tile/cloud_unstable.png");

static auto water_top = Texture("tile/water1.png");
static auto water_bottom = Texture("tile/water2.png");

static auto mushroom_bot1 = Texture("tile/mushroom_bot1.png");
static auto mushroom_bot2 = Texture("tile/mushroom_bot2.png");
static auto mushroom_center = Texture("tile/mushroom_center.png");
static auto mushroom_left = Texture("tile/mushroom_left.png");
static auto mushroom_right = Texture("tile/mushroom_right.png");

static auto grass_bot = Texture("tile/grass_bot.png");
static auto grass_center = Texture("tile/grass_center.png");
static auto grass_left = Texture("tile/grass_left.png");
static auto grass_right = Texture("tile/grass_right.png");

static auto snow_bot = Texture("tile/snow_bot.png");
static auto snow_center = Texture("tile/snow_center.png");
static auto snow_left = Texture("tile/snow_left.png");
static auto snow_right = Texture("tile/snow_right.png");

static auto red_cloud_top_left = Texture("cloud/left_top.png");
static auto red_cloud_top_center = Texture("cloud/center_top.png");
static auto red_cloud_top_right = Texture("cloud/right_top.png");
static auto red_cloud_bottom_left = Texture("cloud/left_bottom.png");
static auto red_cloud_bottom_center = Texture("cloud/center_bottom.png");
static auto red_cloud_bottom_right = Texture("cloud/right_bottom.png");

static auto blue_cloud_top_left = Texture("cloud/blue_left_top.png");
static auto blue_cloud_top_center = Texture("cloud/blue_center_top.png");
static auto blue_cloud_top_right = Texture("cloud/blue_right_top.png");
static auto blue_cloud_bottom_left = Texture("cloud/blue_left_bottom.png");
static auto blue_cloud_bottom_center = Texture("cloud/blue_center_bottom.png");
static auto blue_cloud_bottom_right = Texture("cloud/blue_right_bottom.png");

static auto small_stone = Texture("small_stone.png");

static auto white_bush_center = Texture("bush/snow_center.png");
static auto white_bush_left = Texture("bush/snow_left.png");
static auto white_bush_right = Texture("bush/snow_right.png");

static auto green_bush_center = Texture("bush/center.png");
static auto green_bush_left = Texture("bush/left.png");
static auto green_bush_right = Texture("bush/right.png");

static auto red_bush_center = Texture("bush/mush_center.png");
static auto red_bush_left = Texture("bush/mush_left.png");
static auto red_bush_right = Texture("bush/mush_right.png");

static auto white_hill_center = Texture("hill/snow_center.png");
static auto white_hill_center_dot = Texture("hill/snow_center_dot.png");
static auto white_hill_left = Texture("hill/snow_left.png");
static auto white_hill_right = Texture("hill/snow_right.png");
static auto white_hill_top = Texture("hill/snow_top.png");

static auto red_hill_center = Texture("hill/mush_center.png");
static auto red_hill_center_dot = Texture("hill/mush_center_dot.png");
static auto red_hill_left = Texture("hill/mush_left.png");
static auto red_hill_right = Texture("hill/mush_right.png");
static auto red_hill_top = Texture("hill/mush_top.png");

static auto green_hill_center = Texture("hill/center.png");
static auto green_hill_center_dot = Texture("hill/center_dot.png");
static auto green_hill_left = Texture("hill/left.png");
static auto green_hill_right = Texture("hill/right.png");
static auto green_hill_top = Texture("hill/top.png");

static auto red_pipe_top_left = Texture("pipe/red_left_top.png");
static auto red_pipe_top_right = Texture("pipe/red_right_top.png");
static auto red_pipe_bottom_left = Texture("pipe/red_left_bot.png");
static auto red_pipe_bottom_right = Texture("pipe/red_right_bot.png");

static auto green_pipe_top_left = Texture("pipe/green_left_top.png");
static auto green_pipe_top_right = Texture("pipe/green_right_top.png");
static auto green_pipe_bottom_left = Texture("pipe/green_left_bottom.png");
static auto green_pipe_bottom_right = Texture("pipe/green_right_bottom.png");
static auto green_pipe_bottom_horizontal = Texture("pipe/green_center_bottom_horizontal.png");
static auto green_pipe_top_horizontal = Texture("pipe/green_center_top_horizontal.png");
static auto green_pipe_bottom_horizontal_output = Texture("pipe/green_left_bottom_horizontal.png");
static auto green_pipe_top_horizontal_output = Texture("pipe/green_left_top_horizontal.png");

static auto white_pipe_top_left = Texture("pipe/white_left_top.png");
static auto white_pipe_top_right = Texture("pipe/white_right_top.png");
static auto white_pipe_bottom_left = Texture("pipe/white_left_bottom.png");
static auto white_pipe_bottom_right = Texture("pipe/white_right_bottom.png");

static auto coin = std::array<Texture, 3>{
  Texture("coin/0.png"),
  Texture("coin/1.png"),
  Texture("coin/2.png")
};

static auto purple_coin = std::array<Texture, 3>{
  Texture("coin/purple0.png"),
  Texture("coin/purple1.png"),
  Texture("coin/purple2.png")
};

static auto q_block = std::array<Texture, 3>{
  Texture("q_block/0.png"),
  Texture("q_block/1.png"),
  Texture("q_block/2.png"),
};

static auto spinning_coin = std::array<Texture, 4>{
  Texture("spinning_coin/0.png"),
  Texture("spinning_coin/1.png"),
  Texture("spinning_coin/2.png"),
  Texture("spinning_coin/3.png"),
};

static auto null_block = Texture("q_block/used.png");

//Player
static auto small_mario = Texture("player/mario/small/normal/mario.png");
static auto small_mario_dead = Texture("player/mario/small/normal/dead.png");
static auto small_mario_jumping = Texture("player/mario/small/normal/jumping.png");
static auto small_mario_turning = Texture("player/mario/small/normal/turning.png");

static auto small_mario_walk = std::array<Texture, 3>{
  Texture("player/mario/small/normal/walk1.png"),
  Texture("player/mario/small/normal/walk2.png"),
  Texture("player/mario/small/normal/walk3.png")
};

static auto small_mario_swim = std::array<Texture, 2>{
  Texture("player/mario/small/normal/swim1.png"),
  Texture("player/mario/small/normal/swim2.png")
};

static auto big_mario = Texture("player/mario/big/normal/mario.png");
static auto big_mario_jumping = Texture("player/mario/big/normal/jumping.png");
static auto big_mario_squating = Texture("player/mario/big/normal/squating.png");
static auto big_mario_turning = Texture("player/mario/big/normal/turning.png");

static auto big_mario_walk = std::array<Texture, 3>{
  Texture("player/mario/big/normal/walk1.png"),
  Texture("player/mario/big/normal/walk2.png"),
  Texture("player/mario/big/normal/walk3.png")
};

static auto big_mario_swim = std::array<Texture, 2>{
  Texture("player/mario/big/normal/swim1.png"),
  Texture("player/mario/big/normal/swim2.png")
};

static auto fire_big_mario = Texture("player/mario/big/fire/mario.png");
static auto fire_big_mario_jumping = Texture("player/mario/big/fire/jumping.png");
static auto fire_big_mario_squating = Texture("player/mario/big/fire/squating.png");
static auto fire_big_mario_turning = Texture("player/mario/big/fire/turning.png");

static auto fire_big_mario_walk = std::array<Texture, 3>{
  Texture("player/mario/big/fire/walk1.png"),
  Texture("player/mario/big/fire/walk2.png"),
  Texture("player/mario/big/fire/walk3.png")
};

static auto fire_big_mario_swim = std::array<Texture, 2>{
  Texture("player/mario/big/fire/swim1.png"),
  Texture("player/mario/big/fire/swim2.png")
};

static auto black_big_mario = Texture("player/mario/big/black/mario.png");
static auto black_big_mario_jumping = Texture("player/mario/big/black/jumping.png");
static auto black_big_mario_squating = Texture("player/mario/big/black/squating.png");
static auto black_big_mario_turning = Texture("player/mario/big/black/turning.png");

static auto black_big_mario_walk = std::array<Texture, 3>{
  Texture("player/mario/big/black/walk1.png"),
  Texture("player/mario/big/black/walk2.png"),
  Texture("player/mario/big/black/walk3.png")
};

static auto black_big_mario_swim = std::array<Texture, 2>{
  Texture("player/mario/big/black/swim1.png"),
  Texture("player/mario/big/black/swim2.png")
};

static auto medium_mario = Texture("player/mario/growing.png");

//Entities:
static auto hammer = std::array<Texture, 4>{
  Texture("entity/hammerbro/hammer1.png"),
  Texture("entity/hammerbro/hammer2.png"),
  Texture("entity/hammerbro/hammer3.png"),
  Texture("entity/hammerbro/hammer4.png"),
};

static auto fireball = std::array<Texture, 4>{
  Texture("fireball/1.png"),
  Texture("fireball/2.png"),
  Texture("fireball/3.png"),
  Texture("fireball/4.png")
};

static auto explosion = std::array<Texture, 3>{
  Texture("fireball/explosion1.png"),
  Texture("fireball/explosion2.png"),
  Texture("fireball/explosion3.png")
};

static auto fire_flower = std::array<Texture, 4>{
  Texture("flower/0.png"),
  Texture("flower/1.png"),
  Texture("flower/2.png"),
  Texture("flower/3.png")
};

static auto king_goomba_walk = std::array<Texture, 2>{
  Texture("entity/goomba/king0.png"),
  Texture("entity/goomba/king1.png")
};

static auto squid1 = Texture("entity/squid/1.png");
static auto squid2 = Texture("entity/squid/2.png");

static auto grey_fish_swim = std::array<Texture, 2>{
  Texture("entity/fish/grey1.png"),
  Texture("entity/fish/grey2.png")
};

static auto red_fish_swim = std::array<Texture, 2>{
  Texture("entity/fish/red1.png"),
  Texture("entity/fish/red2.png")
};

static auto goomba_walk = std::array<Texture, 2>{
  Texture("entity/goomba/0.png"),
  Texture("entity/goomba/1.png")
};
static auto goomba_dead = Texture("entity/goomba/dead.png");

static auto red_goomba_walk = std::array<Texture, 2>{
  Texture("entity/goomba/red0.png"),
  Texture("entity/goomba/red1.png")
};
static auto red_goomba_dead = Texture("entity/goomba/reddead.png");

static auto yellow_goomba_walk = std::array<Texture, 2>{
  Texture("entity/goomba/yellow0.png"),
  Texture("entity/goomba/yellow1.png")
};
static auto yellow_goomba_dead = Texture("entity/goomba/yellowdead.png");

static auto purple_koopa_walk = std::array<Texture, 2>{
  Texture("entity/koopa/purple1.png"),
  Texture("entity/koopa/purple2.png")
};
static auto purple_koopa_dead = Texture("entity/koopa/purpledead.png");

static auto purple_flying_koopa_walk = std::array<Texture, 2>{
  Texture("entity/koopa/purple_flying1.png"),
  Texture("entity/koopa/purple_flying2.png")
};

static auto green_koopa_walk = std::array<Texture, 2>{
  Texture("entity/koopa/green1.png"),
  Texture("entity/koopa/green2.png")
};
static auto green_koopa_dead = Texture("entity/koopa/greendead.png");

static auto green_flying_koopa_walk = std::array<Texture, 2>{
  Texture("entity/koopa/green_flying1.png"),
  Texture("entity/koopa/green_flying2.png")
};

static auto red_koopa_walk = std::array<Texture, 2>{
  Texture("entity/koopa/red1.png"),
  Texture("entity/koopa/red2.png")
};
static auto red_koopa_dead = Texture("entity/koopa/reddead.png");

static auto red_flying_koopa_walk = std::array<Texture, 2>{
  Texture("entity/koopa/red_flying1.png"),
  Texture("entity/koopa/red_flying2.png")
};

static auto beetle_walk = std::array<Texture, 2>{
  Texture("entity/beetle/0.png"),
  Texture("entity/beetle/1.png")
};
static auto beetle_dead = Texture("entity/beetle/dead.png");

static auto spike_walk = std::array<Texture, 2>{
  Texture("entity/spike/0.png"),
  Texture("entity/spike/1.png")
};

static auto hammerbro_walk = std::array<Texture, 2>{
  Texture("entity/hammerbro/1.png"),
  Texture("entity/hammerbro/2.png")
};

static auto hammerbro_walk_with_hammer = std::array<Texture, 2>{
  Texture("entity/hammerbro/3.png"),
  Texture("entity/hammerbro/4.png")
};

static auto red_hammerbro_walk = std::array<Texture, 2>{
  Texture("entity/hammerbro/red1.png"),
  Texture("entity/hammerbro/red2.png")
};

static auto red_hammerbro_walk_with_hammer = std::array<Texture, 2>{
  Texture("entity/hammerbro/red3.png"),
  Texture("entity/hammerbro/red4.png")
};

static auto lakito_hidden = Texture("entity/lakito/0.png");
static auto lakito_shown = Texture("entity/lakito/1.png");
static auto lakito_throw = std::array<Texture, 2>{
  Texture("entity/lakito/throw0.png"),
  Texture("entity/lakito/throw1.png")
};

static auto plant = std::array<Texture, 2>{
  Texture("entity/plant/0.png"),
  Texture("entity/plant/1.png")
};

static auto red_plant = std::array<Texture, 2>{
  Texture("entity/plant/red0.png"),
  Texture("entity/plant/red1.png")
};

static auto black_plant = std::array<Texture, 2>{
  Texture("entity/plant/black0.png"),
  Texture("entity/plant/black1.png")
};

static auto mushroom = Texture("entity/mushroom/mushroom.png");
static auto green_mushroom = Texture("entity/mushroom/green.png");

static auto mini_coin = Texture("mini.png");

} //namespace mario::textures
