#pragma once

#include "States/AppState.hpp"
#include "Util/Util.hpp"
#include "config.hpp"
#include <iostream>

#include <thread>

namespace mario::terminal{

static bool god_mode = false;

static auto run(AppState& app){
  std::thread([&]{
    while(true){
      auto cmd = std::string{};

      std::cin >> cmd;

      //Teleportation
      if (cmd == "tp"){
        auto x = 0, y = 0;
        std::cin >> x >> y;

        app.current_level.player.position = glm::vec2(x, y) * BlockBase::Size;
      }

      //Invincibility
      if (cmd == "god"){
        god_mode = !god_mode;
        std::cerr << "God mode: " << std::array{ "OFF", "ON" }[god_mode] << '\n';
      }

      //Level change
      if (cmd == "lvl"){
        auto major = 0, minor = 0;
        std::cin >> major >> minor;

        //We're counting from 0:
        --major;
        --minor;

        app.current_frame = (major * config::LevelsInWorld + minor) | util::as<AppState::Frame>;
        app.current_level.current_checkpoint = LevelState::CheckpointNotSet;
      }

      //Teleport to checkpoint
      if (cmd == "tpch"){
        auto& level = app.current_level;
        level.player.position = level.checkpoints[0];        
      }
    }
  }).detach();
} 

} //namespace mario::terminal
