#pragma once

#include "Controllers/EntityController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "Controllers/FireballController.hpp"

namespace mario::monster_controller{

template<typename GenericMonster>
static auto throw_fireball(GenericMonster& monster, LevelState& level){
	fireball_controller::run(monster.fireball, level);
	if (entity_controller::kill_player_on_touch(monster.fireball, level)){
		fireball_controller::reset(monster.fireball);
	}

	if (monster.in_shell) return;
	if (!monster.is_active) return;

	const auto distance_to_player = std::abs(monster.position.x - level.player.position.x);

	if (!monster.fireball.is_active && distance_to_player < 8.f * BlockBase::Size){
		const auto fireball_y = monster.position.y + BlockBase::Size / 2.f;
		const auto fireball_x = monster.position.x + (monster.direction.is_left()
			? 0.f
			: monster.size.x - monster.fireball.size.x);

		monster.fireball.shoot(
			glm::vec2(fireball_x, fireball_y), 
			monster.direction, 
			PlayerState::FireballSpeed
		);
	}
}

static auto follow_player(MonsterState& monster, const LevelState& level){
	if (!level.player.is_dead){
		if (monster.position.x > level.player.position.x + BlockBase::Size + monster.size.x) {
			monster.set_direction(util::Direction::left());
		}
		if (monster.position.x + BlockBase::Size < level.player.position.x) {
			monster.set_direction(util::Direction::right());
		}
	}

	if (level.player.is_dead) monster.follows_player = false;
}

static auto handle_points_particles(MonsterState& monster){
  for (auto& p : monster.points_generator.items){
    points_particles_controller::run(p);
  }
}

static auto kill_player_on_touch(const MonsterState& monster, LevelState& level){
  if (monster.was_hit) return;
  entity_controller::kill_player_on_touch(monster | util::as<EntityState>, level);
};

template<typename Function>
static auto die_when_stomped(
    MonsterState& entity, 
    LevelState& level,
    Function set_entity_dead
){
  if (!entity.is_active) return false;

  auto& player = level.player;
  auto& stats = level.stats;

  if (entity_controller::was_stomped(entity, player)){
    set_entity_dead();

		player.is_on_ground = false;
    player.gravity = PlayerState::BouncePower;
    player.position.y = entity.gravity_flip.is_flipped()
      ? entity.position.y + entity.size.y
      : entity.position.y - player.size.y;

    stats.score += entity.reward_for_killing * player.mobs_killed_in_row;
    entity.spawn_points(player.mobs_killed_in_row);
    ++player.mobs_killed_in_row;

    return true;
  }

  return false;
};

static auto become_active_when_seen(MonsterState& entity, const LevelState& level){
  if (entity.is_in_q_block) return;

  const auto& player = level.player;

  const auto player_field_of_view_x = std::max(
    config::FrameBufferSize.x - LevelState::PlayerPositionToStartLevelScrolling.x,
    config::FrameBufferSize.x - player.position.x
  );

  const auto player_field_of_view_y = player.position.y - level.camera_offset.y;

  if (entity.position.x - player.position.x > player_field_of_view_x) return;

  const auto entity_is_far_behind_player 
    = player.position.x > entity.position.x + LevelState::BlocksInRow * BlockBase::Size;

  if (entity_is_far_behind_player && level.type == LevelState::Type::Horizontal) return;

  if (level.type == LevelState::Type::Vertical){
    const auto player_can_see_entity 
      = std::abs(player.position.y - entity.position.y - entity.size.y) > player_field_of_view_y;

    if (player_can_see_entity) return;
  }

  entity.is_active = true;
};

static auto bounce_out(MonsterState& entity){
  entity.gravity = MonsterState::BounceDiePower;
  entity.should_collide = false;
  entity.vertical_flip = util::Flip::flip();
}

static auto bounce_die(MonsterState& entity, StatsState& stats){
  entity.was_hit = true;
  bounce_out(entity);

  stats.score += entity.reward_for_killing;
  entity.spawn_points();
}

static auto is_hit_by_fireball(const MonsterState& entity, const FireballState& fireball){
  return collision_controller::intersects(fireball, entity) && fireball.is_active && entity.is_active && !entity.was_hit;
}

template<typename Callable>
static auto react_when_hit_by_fireball(MonsterState& entity, LevelState& level, const Callable& callback){
  for (auto& fireball : level.player.fireballs){
    if (is_hit_by_fireball(entity, fireball)){
      callback(fireball);
    }
  } 
}

static auto die_when_hit_by_fireball(MonsterState& entity, LevelState& level){
  react_when_hit_by_fireball(entity, level, [&](FireballState& fireball){
    bounce_die(entity, level.stats);

    fireball.acceleration.left = fireball.acceleration.right = 0.f;
  });
};  

static auto endure_fireball(MonsterState& entity, PlayerState& player){
  for (auto& fireball : player.fireballs){
    if (is_hit_by_fireball(entity, fireball)){
      fireball.acceleration.left = fireball.acceleration.right = 0.f;
    }
  } 
};  

template<typename Reaction>
static auto react_when_on_bouncing_block(
    MonsterState& entity, 
    const LevelState& level,
    Reaction reaction 
){
  const auto& objects = level.game_objects;

  const auto detect_bounce_and_react = [&](const auto& block){
    if (block.bounce_state.is_bouncing){
      if (entity.is_dead || !entity.should_collide) return;

      const auto collision_state = collision_controller::run(
        collision_controller::Rect(entity), 
        collision_controller::Rect(block)
      );
      
      if (collision_state.distance_below == util::in_range(-15.f, 0.f)){
        reaction();
      }
    }
  };

  for (auto& block : objects.get_vec<BricksBlockState>()){
    detect_bounce_and_react(block);
  }

  objects.for_each_template<QBlockState>(detect_bounce_and_react);
}

static auto die_when_on_bouncing_block(MonsterState& entity, LevelState& level){
  monster_controller::react_when_on_bouncing_block(entity, level, [&]{
    bounce_die(entity, level.stats);
  });
}

//For mushrooms:
static auto bounce_when_on_bouncing_block(MonsterState& entity, LevelState& level){
  monster_controller::react_when_on_bouncing_block(entity, level, [&]{
    entity.gravity = MonsterState::BouncePower;
    entity.is_on_ground = false;
  });
}

static auto handle_turning_around(MonsterState& entity){
  if (entity.acceleration.left == 0 && entity.acceleration.right == 0){
    entity.direction = -entity.direction;

    if (entity.direction.is_left()){
      entity.acceleration.left = entity.walk_speed;
      return;
    }

    entity.acceleration.right = entity.walk_speed;
  }
}

} //namespace mario::controllers
