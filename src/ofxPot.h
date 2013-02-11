#ifndef _OFX_POT
#define _OFX_POT

/**
 * \class ofxPot
 *
 * \brief This is an implementation of a pot, or burner, in the cooking metaphor of TactoSonix.
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
#include "UI/ofxTactoSHPMNode.h"
#include "ofxCookableNode.h"

/// A class that implements a pot (or burner) in a stove top.
class ofxPot : public ofBaseApp
{
public:
	ofxPot(int _color, int _radius, ofPoint _origin, int _bpm); ///< Constructor
	ofxPot() :
		m_nColor(0), m_nRadius(1), m_ptOrigin(0,0), m_ptOriginalPosition(0,0), m_nBPM(120) {}; ///< Default constructor

	void									update(int nCurrentBeat); ///< Regular OpenFrameworks function.
	void									exit(); ///< Regular OpenFrameworks function.
	void									draw(); ///< Regular OpenFrameworks function.

	int										getColor(); ///< Returns the colour of the pot.
	bool									isPointInside(ofPoint pt); ///< Returns true if and only if the queried point is inside the pot.
	ofPoint									getOrigin(); ///< Returns the point of origin of the pot, including the dragged offset.
	void									setOrigin(ofPoint _origin); ///< Sets the point of origin of the pot, including the dragged offset.
	ofPoint									getOriginInit(); ///< Returns the point of origin of the pot, without the dragged offset.
	void									setOriginInit(ofPoint _origin); ///< Sets the point of origin of the pot, without the dragged offset.
	int										getRadius(); ///< Returns the radius in pixels of the pot.
	void									setRadius(int _nRadius); ///< Sets the radius in pixels of the pot.
	void									addCookElement(ofxCookableNode _nodeToCook); ///< Adds an element to cook in the pot.
	void									setFocusVolume(float _fVolume) { m_fVolume = _fVolume; } ///< Sets the amount of focus of the pot in the current view. \param _fVolume The volume of the pot
	
	void									mouseDragged(int x, int y, int button); ///< Regular OpenFrameworks function.
	void									mousePressed(int x, int y, int button); ///< Regular OpenFrameworks function.
	void									mouseReleased(int x, int y, int button); ///< Regular OpenFrameworks function.
	void									touchDown(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	void									touchMoved(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	void									touchUp(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	bool									mouseTouchDown(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch down events.
	bool									mouseTouchMoved(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch moved events.
	void									mouseTouchUp(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch up events.

	void									drag(ofPoint deltaDrag); ///< Drags the pot by the specified offset.

private:
	int										m_nColor; ///< The color of the menu option.
	ofPoint									m_ptOrigin; ///< The point of origin of the stove top information, including the dragged offset.
	ofPoint									m_ptOriginalPosition; ///< The initial point of origin of the stove top information.
	int										m_nRadius; ///< The radius of the node.
	vector<ofxCookableNode>					m_activeCookNodesLoop1; ///< The first audio channel of nodes being cooked in this pot.
	vector<ofxCookableNode>					m_activeCookNodesLoop2; ///< The second audio channel of nodes being cooked in this pot.
	vector<ofxCookableNode>					m_activeCookNodesLoop3; ///< The third audio channel of nodes being cooked in this pot.

	// Necessary for beat tracking
	int										m_nTimeOfCreationMs; ///< The time at which this pot was created.
	int										m_nBPM; ///< The tempo in beats per minute of the sequencer.
	int										m_nPeriodSingleBeatMs; ///< The period of each beat in milliseconds.
	int										m_nCurrentBeat; ///< The current beat in the sequencer.
	float									m_fVolume; ///< The volume of the pot (it affects individual the volume of individual sounds).
#ifdef _DEBUG
	ofxTextBlock							m_infoText; ///< A text block of information about the pot.
#endif
	vector<ofxCookableNode*>				m_draggedBeats; ///< A vector of musical loops that are being dragged.
	bool									m_bShowingLoopFeedback; ///< Whether or not we are showing feedback about looping nodes.
};

#endif
