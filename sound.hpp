// Term--Fall 2020
// Chenyang Zhang

#pragma once

#include <stdio.h>
#include <iostream>
#include <irrKlang.h>
#include <vector>

using namespace irrklang;
using namespace std;

#pragma comment(lib, "irrKlang.lib")

class Sound {
	ISoundEngine* engine;
	ISound * bgm;
	vector<ISound *> exp;
	float volume = 1.0f;
public:
	Sound() {
		engine = nullptr;
	}
	~Sound() {
		if (engine) {
			engine->drop();
		}
		for (auto s: exp) {
			if (s) {
				s->drop();
			}
		}
		if (bgm) {
			bgm->drop();
		}
	}


	// initialize background music
	void init() {
		engine = createIrrKlangDevice();
		if (!engine) {
			std::cout << "no engine" << std::endl;
		}
		bgm = engine->play2D("Assets/bgm.ogg", true, true);
	}
	// play the background music
	void play() {
		bgm->setIsPaused(false);
	}

	// play a explosion sound effect
	void playExplosion() {
		for (auto it = exp.begin(); it != exp.end();) {
			if ((*it)->isFinished()) {
				exp.erase(it);
				continue;
			}
			it++;
		}
		ISound * new_exp = engine->play2D("Assets/explosion.wav", false, true);
		new_exp->setIsPaused(false);
		new_exp->setVolume(volume);
		exp.push_back(new_exp);
	}

	// set the sound volume
	void setVolume(float v) {
		volume = v;
		if (bgm) {
			bgm->setVolume(v);
		}
		for (auto s: exp) {
			if (s && !s->isFinished()) {
				s->setVolume(v);
			}
		}
	}
};

