//
//  lineDrawEffect.h
//  karmaMapper
//
//  Created by Daan de Lange on 10/10/15.
//
//
//

#pragma once

#include "ofMain.h"
#include "shapes.h"
#include "basicEffect.h"
#include "animationParams.h"
#include "mirReceiver.h"
#include "liveGrabberOSC.h"
#include "lineDrawEffectLine.h"

struct animationParams;

class lineDrawEffect : public basicEffect {
	
public:
	// constructors
	lineDrawEffect();
	~lineDrawEffect();
	
	// global effect functions
	bool initialise(const animationParams& params);
	bool render(karmaFboLayer& renderLayer, const animationParams& params);
	void update(karmaFboLayer& renderLayer, const animationParams& params);
	void reset();
	
	// #########
	// GUI STUFF
	virtual bool printCustomEffectGui();
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml, const shapesDB& _scene);
	
	// controller functions
	bool randomizePresets();
	
	virtual void tempoEventListener(mirTempoEventArgs &_args);
	virtual void liveGrabberNoteEventListener(liveGrabberNoteEventArgs &_args);
	
protected:
	//ofFbo fbo; // for compatibility issues, we need a specific fbo object
	//float linesColor[4];
	list<lineDrawEffectLine> lines;
	
	float fLineBeatDuration;
	int spawnAmount;
	
	bool bReactToMusic; // *threaded memory*
	
	bool bStressTestMode;
	float fStressTestMultiplier;
	float fStressTestAddTolerance;
	float fStressTestRemoveTolerance;
	int fStressTestTargetFPS;
	
private:
	
	
};

#define GUILineDrawEffectPanel "Line Drawer"