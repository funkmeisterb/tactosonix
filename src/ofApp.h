#pragma once

#include "ofMain.h"
#include "ofxTextSuite.h"
#include "metaphorKitchen.h"
#include "ofxTactoHandler.h"

#include "ofxParticleEmitter.h"
#include "demoParticle.h"

// The states of the application
enum testAppStates
{
	STATE_INTRO, ///< The first menu
	STATE_ENVIRONMENTS, ///< The choice of metaphor environments (unused)
	STATE_KITCHEN, ///< The MAKING MUSIC IS COOKING metaphor
	STATE_CROUPIER ///< The MAKING MUSIC IS GAMBLING metaphor (not implemented)
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void exit();
		void					touchDown(ofTouchEventArgs& args); ///< Regular OpenFrameworks function.
		void					touchUp(ofTouchEventArgs& args); ///< Regular OpenFrameworks function.
		void					touchMoved(ofTouchEventArgs& args); ///< Regular OpenFrameworks function.
		void					mouseTouchDown(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch down events.
		void					mouseTouchMoved(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch moved events.
		void					mouseTouchUp(float x, float y, bool fullRange, int button = 0, int touchId = 0);

		metaphorKitchen*		getKitchen() { return &m_metaKitchen; } ///< Returns the kitchen metaphor class.

		ofApp()
		: m_nState(STATE_INTRO) {} ///< Constructor

private:
		unsigned long int		m_nCounter; ///< A counter for dynamic features.
		int						m_nMusicStartTime; ///< The time (in ms) at which the music started.
		int						m_nState; ///< The current state of the application.
		int						m_nCurrentStep; ///< The current step in the sequencer.
		ofImage					m_IDMILlogo; ///< The IDMIL logo.

		// Display
		ofxTextBlock			m_profileInfo; ///< A text block to display profiling information for the application.

											   // Metaphor environments
		metaphorKitchen			m_metaKitchen; ///< The implementation of the MAKING MUSIC IS COOKING metaphor.

											   // Touch handler
		ofxTactoHandler			m_touchEngine; ///< The handler of TUIO events.
		ofPoint					m_dragDelta; ///< The drag motion difference between frames.
		ofPoint					m_oldCursorPosition;

		// Particle system
		ofxParticleEmitter		m_emitter;
		void					resetParticles();
		particleMode			currentMode;
		string					currentModeStr;

		vector<demoParticle>	p;
		vector<ofPoint>			attractPoints;
		vector<ofPoint>			attractPointsWithMovement;
};
