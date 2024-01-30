#pragma once

#include "States/AppState.hpp"
#include "Input.hpp"
#include <fstream>

namespace mario::saves{

static constexpr auto FileName = "savefile";

struct SaveState{
	int world = 0;
	int level = 0;
	AppState::Difficulty difficulty = AppState::Difficulty::Easy;
	std::array<int, input::ControlName::ItemsCount> controls;
};

static auto file_exists(){
	return !!std::ifstream(FileName, std::ios::binary);
}

static auto save(SaveState state){
	auto file = std::ofstream(FileName, std::ios::binary);

	file.write(reinterpret_cast<char*>(&state), sizeof(state));
}

static auto load(){
	auto file = std::ifstream(FileName, std::ios::binary);

	if (!file){
		save(SaveState{});
		return SaveState{};
	}

	auto state = SaveState{};
	file.read(reinterpret_cast<char*>(&state), sizeof(state));

	return state;
}

} //namespace mario::saves
