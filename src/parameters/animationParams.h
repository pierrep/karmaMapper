//
//  animationParams.h
//  karmaMapper
//
//  Created by Daan on 07/10/15.
//
//

#pragma once

#include "ofMain.h"

struct seasonsYear{
	seasonsYear(): winter(0), spring(0), summer(0), autumn(0){};
	
	float winter, spring, summer, autumn;
	
	inline float& operator[](const int i) {
		switch (i){
			case 0 :
				return winter;
				break;
			case 1 :
				return spring;
				break;
			case 2 :
				return summer;
			default:
				return autumn;
				break;
		}
	}
	inline float operator[](const int i) const {
		switch (i){
			case 0 :
				return winter;
				break;
			case 1 :
				return spring;
				break;
			case 2 :
				return summer;
			default:
				return autumn;
				break;
		}
	}
};

struct colorSet {
	colorSet() :
	main(ofFloatColor(0,0,0,255)),
	secondary(ofFloatColor(0,0,0,255))
	{};
	
	ofFloatColor main, secondary;
};

struct animationParams {
	
	animationParams():
	fps(0),
	elapsedUpdates(0),
	elapsedFrames(0),
	elapsedTime(0.f),
	idleTimeMillis(0),
	uniqueID(""),
	uniqueIDAlt(""),
	userChain("")
	{};
	
	float fps;
	unsigned int elapsedUpdates;
	unsigned int elapsedFrames;
	float elapsedTime;
	uint32_t idleTimeMillis;
	
	// used for customising effects and synchronising their
	string uniqueID; // hex string
	string uniqueIDAlt;
	
	// chain typed by user // keylogger
	string userChain;
	
	seasonsYear seasons;
	colorSet staticColors;
	colorSet staticColorsAlt;
	colorSet varyingColors;
	colorSet userColors;
	
};
