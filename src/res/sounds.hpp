#pragma once

#include "Audio.hpp"
#include <array>

namespace mario::sounds{

using audio::Sound;

enum SoundName{
	Overworld,
	OverworldFast,
	Underwater,
	UnderwaterFast,
	Underworld,
	UnderworldFast,
	Castle,
	Blockbreak,
	Blockhit,
	Coin,
	Jump,
	Jumpbig,
	Stomp,
	MushroomAppear,
	MushroomEat,
	OneUp,
	Pipe,
	Death,
	LevelEnd,
	Shrink,
	LowTime,
	Shot,
	Boom,
	Fireball,
	BulletBill,
	Fire,
	Pause,
	Princess,

	SoundsCount	
};

inline auto sounds = std::array<Sound, SoundsCount>{};

static auto init(){
	sounds[Overworld] = Sound("overworld.wav");
	sounds[OverworldFast] = Sound("overworld-fast.wav");
	sounds[Underwater] = Sound("underwater.wav");
	sounds[UnderwaterFast] = Sound("underwater-fast.wav");
	sounds[Underworld] = Sound("underground.wav");
	sounds[UnderworldFast] = Sound("underground-fast.wav");
	sounds[Castle] = Sound("castle.wav");
	sounds[Blockbreak] = Sound("blockbreak.wav");
	sounds[Blockhit] = Sound("blockhit.wav");
	sounds[Coin] = Sound("coin.wav");
	sounds[Jump] = Sound("jump.wav");
	sounds[Jumpbig] = Sound("jumpbig.wav");
	sounds[Stomp] = Sound("stomp.wav");
	sounds[MushroomAppear] = Sound("mushroomappear.wav");
	sounds[MushroomEat] = Sound("mushroomeat.wav");
	sounds[OneUp] = Sound("oneup.wav");
	sounds[Pipe] = Sound("pipe.wav");
	sounds[Death] = Sound("death.wav");
	sounds[LevelEnd] = Sound("levelend.wav");
	sounds[Shrink] = Sound("shrink.wav");
	sounds[LowTime] = Sound("lowtime.wav");
	sounds[Shot] = Sound("shot.wav");
	sounds[Boom] = Sound("boom.wav");
	sounds[Fireball] = Sound("fireball.wav");
	sounds[BulletBill] = Sound("bulletbill.wav");
	sounds[Fire] = Sound("fire.wav");
	sounds[Pause] = Sound("pause.wav");
	sounds[Princess] = Sound("princessmusic.wav");

	for (auto& sound : sounds){
		sound.load();
	}

	sounds[Overworld].set_looping();
	sounds[OverworldFast].set_looping();
	sounds[Underwater].set_looping();
	sounds[UnderwaterFast].set_looping();
	sounds[Underworld].set_looping();
	sounds[UnderworldFast].set_looping();
	sounds[Princess].set_looping();
}

static auto free(){
	for (auto& sound : sounds){
		sound.free();
	}
}

} //namespace mario
