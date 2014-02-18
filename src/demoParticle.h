#pragma once
#include "ofMain.h"

enum particleMode{
	PARTICLE_MODE_ATTRACT = 0,			// attracted to mouse
	PARTICLE_MODE_REPEL,				// repel from mouse
	PARTICLE_MODE_NEAREST_POINTS,		// attracted to a few spots
	PARTICLE_MODE_NOISE,				// like snow
	PARTICLE_MODE_EMITTER,				// emit particles that have a lifetime
	PARTICLE_MODE_OFXPARTICLEEMITTER	// OF add-on
};

class demoParticle{

	public:
		demoParticle();
		
		void setMode(particleMode newMode);	
		void setAttractPoints( vector <ofPoint> * attract );

		void reset();
		void update();
		void draw();		
		
		ofPoint pos;
		ofPoint vel;
		ofPoint frc;
		
		float drag; 
		float uniqueVal;
		float scale;
		
		particleMode mode;
		
		vector <ofPoint> * attractPoints; 
};