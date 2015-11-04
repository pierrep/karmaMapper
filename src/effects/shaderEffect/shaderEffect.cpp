//
//  shaderEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 10/10/15.
//
//  - - - -
//
//  Applies on vertexShape
//	Distorts it's vertexes

#include "shaderEffect.h"

// - - - - - - -
// CONSTRUCTORS
// - - - - - - -

shaderEffect::shaderEffect(){
	
	shaderEffect::reset();
	
	// effect type must match with class
	effectType = "shaderEffect";
}

shaderEffect::~shaderEffect(){
	ofRemoveListener(mirReceiver::mirOnSetEvent, this, &shaderEffect::onSetEventListener);
}

// - - - - - - -
// BASIC EFFECT FUNCTIONS
// - - - - - - -

// initialises the effect
bool shaderEffect::initialise(const animationParams& params){
	// init values
	basicEffect::initialise(params);
	
	isLoading = true;
	isInitialised = false;
	onSetCalls = 0;
	
	// do stuff
	ofEnableAlphaBlending();
	
	// set this when done
	isInitialised = true;
	isLoading = false;
	
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 8);
	fbo.begin();
	ofClear(0,0,0,0); // clear all, including alpha
	fbo.end();
	
	ofAddListener(mirReceiver::mirOnSetEvent, this, &shaderEffect::onSetEventListener);
	
	return isInitialised;
}

bool shaderEffect::render(const animationParams &params){
	if(!isReady() || !shader.isLoaded()) return false;
	
	fbo.begin();
	// fade out
	//ofClear(ofFloatColor(0,0,0, 30);
	
//	glEnable(GL_BLEND);
//	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
//	glBlendEquation(GL_FUNC_ADD);

	//ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
//	glEnable(GL_BLEND);
//	glBlendEquation(GL_FUNC_ADD);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	glBlendFunc(GL_ONE, GL_ONE);
	
	
//	glEnable(GL_BLEND);
//	glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_DST_ALPHA);
//	glBlendEquation(GL_FUNC_SUBTRACT);
	
	
	//glBlendColor(0,0,0,30);

	ofSetColor(0.0f, 5.0f*params.seasons.spring + 5.0f*params.seasons.autumn);
	ofFill();
	ofDrawRectangle(0,0, fbo.getWidth(), fbo.getHeight());
	
	glDisable(GL_BLEND);
	
	shader.begin();
	registerShaderVariables(params);
	
	// set drawing environment
	effectMutex.lock();
	ofSetColor( params.varyingColors.main.getLerped(ofColor(0), onSetCalls/10.0f) );
	onSetCalls = 0;
	effectMutex.unlock();
	ofFill();
	
	// tmp
	//ofDrawRectangle(50,50, 200,200);
	
	// draw shape so GPU gets their vertex data
	for(auto it=shapes.begin(); it!=shapes.end(); ++it){
		if( (*it)->isType("vertexShape") ){
			
			(*it)->sendToGPU();
		}
	}
	
	// flush the pipeline! :D
	shader.end();
	fbo.end();
	
	// draw fbo
	fbo.draw(0,0);
	
	return true;
}

// updates shape data
void shaderEffect::update(const animationParams& params){
	
	// do basic Effect function
	basicEffect::update( params );
//	
//	ofScopedLock lock(effectMutex);
//	
//	if(shapes.size()<1) return;
//	
//	for(auto it=shapes.begin(); it!=shapes.end(); it++){
//		// todo: basicEffect should have a nice downcast getter function like .asType<vertexShape>()
//		if( (*it)->isType("vertexShape") ){
//			list<basicPoint>& pts = ((vertexShape*)(*it))->getPoints(POINT_POSITION_RELATIVE);
////			int offset = 0;
////			for(auto itb=pts.begin(); itb!=pts.end(); ++itb){
////				(*itb) += basicPoint(
////					sin(params.seasons.summer*TWO_PI+offset*params.seasons.summer)*50,
////					cos(params.seasons.summer*TWO_PI+offset*params.seasons.summer)*50
////				);
////				offset++;
////			}
////			(*it)->onShapeModified();
//		}
//	}
	// start shader
	
}

// resets all values
void shaderEffect::reset(){
	basicEffect::reset();
}

// - - - - - - -
// CONTROLLER FUNCTIONS
// - - - - - - -

bool shaderEffect::randomizePresets(){
	if(!basicEffect::randomizePresets() ) return false;
	
	// do your stuff here
	
	return true;
}

// - - - - -
// shaderEffect Methods
// - - - - -
void shaderEffect::registerShaderVariables(const animationParams& params){
	
	if( !shader.isLoaded() ){
		ofLogWarning("shaderEffect::registerShaderVariables() --> shader not loaded or linked!");
		return;
	}
	
	//if(shader==NULL) return;
	
	shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );
	shader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );
	
	effectMutex.lock();
	shader.setUniform1f("mirZeroCrossings", mirReceiver::mirCache.zcr );
	effectMutex.unlock();
	shader.setUniform1f("mirOnSetCalls", onSetCalls );
	
	shader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(), 0);
	shader.setUniform4f("iMouse", ofGetMouseX(), ofGetMouseY(), 0, 0 );
	shader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
	shader.setUniform1i("tex", 0);
}

bool shaderEffect::loadShader(string _vert, string _frag){
	;
	//shader.setupShaderFromFile(GL_FRAGMENT_SHADER, _frag);
	
	if( shader.load(_vert, _frag) && !shader.isLoaded() ){
		ofLogNotice("shaderEffect::registerShaderVariables() --> shader not loaded");
		hasError = true;
		return true;
	}
	else{
		hasError = false;
		return false;
	}
	
	//cout << "OK:" << shader.linkProgram() << endl;
	//cout << shader.isLoaded() << shader.linkProgram() << endl;
	//shader.checkProgramInfoLog( shader.getProgram() );
}

void shaderEffect::onSetEventListener(mirOnSetEventArgs &_args){
	ofScopedLock lock(effectMutex);
	
	if(shapes.size()<=0) return;
	
	if(_args.source.compare("aubioOnSet")==0){
		onSetCalls++;
	}
	
	//else if(_args.source.compare("")==0){}
}