#ifndef _TEST_APP
#define _TEST_APP

/**
 * \class testApp
 *
 * \brief This is the main class of TactoSonix. TactoSonix is a multi-touch application for DJing using novel metaphors.
 *
 * \author Bruno Angeles (bruno.angeles@mail.mcgill.ca)
 *
 * \version 1.0
 *
 * \date 2012/08/14
 *
 */

#include "ofMain.h"
#include "ofxTextSuite.h"
#include "metaphorKitchen.h"
#include "ofxTactoHandler.h"

#ifdef _DEBUG
	#define VERBOSE // display information in the console about changing mapping values
#endif

// The states of the application
enum testAppStates
{
	STATE_INTRO, ///< The first menu
	STATE_ENVIRONMENTS, ///< The choice of metaphor environments (unused)
	STATE_KITCHEN, ///< The MAKING MUSIC IS COOKING metaphor
	STATE_CROUPIER ///< The MAKING MUSIC IS GAMBLING metaphor (not implemented)
};

class testApp : public ofBaseApp
{
public:
	void					setup(); ///< Regular OpenFrameworks function.
	void					update(); ///< Regular OpenFrameworks function.
	void					draw(); ///< Regular OpenFrameworks function.
	void					exit(); ///< Regular OpenFrameworks function.

	void					keyPressed  (int key); ///< Regular OpenFrameworks function.
	void					keyReleased(int key); ///< Regular OpenFrameworks function.
	void					mouseDragged(int x, int y, int button); ///< Regular OpenFrameworks function.
	void					mousePressed(int x, int y, int button); ///< Regular OpenFrameworks function.
	void					mouseReleased(int x, int y, int button); ///< Regular OpenFrameworks function.
	void					windowResized(int w, int h); ///< Regular OpenFrameworks function.
	void					touchDown(ofTouchEventArgs& args); ///< Regular OpenFrameworks function.
	void					touchUp(ofTouchEventArgs& args); ///< Regular OpenFrameworks function.
	void					touchMoved(ofTouchEventArgs& args); ///< Regular OpenFrameworks function.
	void					mouseTouchDown(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch down events.
	void					mouseTouchMoved(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch moved events.
	void					mouseTouchUp(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch up events.

	metaphorKitchen*		getKitchen() { return &m_metaKitchen; } ///< Returns the kitchen metaphor class. \return The MAKING MUSIC IS COOKING implementation.

	testApp()
		: m_nState (STATE_INTRO) {} ///< Constructor

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
};

#endif
