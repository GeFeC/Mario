#pragma once

#include "config.hpp"

#include <miniaudio.h>
#include <stdexcept>

namespace mario::audio{

inline ma_engine engine;
inline bool engine_initialised = false;

static auto init(){
	if (!engine_initialised){
		engine_initialised = true;

		const auto result = ma_engine_init(NULL, &engine);
		if (result != MA_SUCCESS) {
			throw std::runtime_error("Failed to initialize audio engine!\n");
		}
	}
}

static auto free(){
	ma_engine_uninit(&engine);
}

struct Sound{
	std::string path;
	ma_sound sound;

	Sound() = default;

	Sound(const std::string& path){
		this->path = config::AudioSourceDir + path;
	}

	auto load(){
		const auto result = ma_sound_init_from_file(&engine, this->path.c_str(), 0, NULL, NULL, &sound);

		if (result != MA_SUCCESS) {
			throw std::runtime_error("Unable to load sound: " + this->path);
		}
	}

	auto pause(){
		ma_sound_stop(&sound);
	}

	auto resume(){
		ma_sound_start(&sound);
	}

	auto stop(){
		ma_sound_stop(&sound);
		ma_sound_seek_to_pcm_frame(&sound, 0);
	}

	auto play(){
		stop();

		ma_sound_start(&sound);
	}

	auto set_pitch(float pitch){
		ma_sound_set_pitch(&sound, pitch);
	}

	auto set_looping(){
		ma_sound_set_looping(&sound, true);
	}

	auto is_playing() const{
		return ma_sound_is_playing(&sound);
	}

	auto free(){
		ma_sound_uninit(&sound);
	}

};

} //namespace mario::audio
