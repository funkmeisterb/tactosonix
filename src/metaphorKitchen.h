#ifndef _METAPHOR_KITCHEN
#define _METAPHOR_KITCHEN

/**
 * \class metaphorKitchen
 *
 * \brief This class is a blend between the tasks of making music and cooking.
 *
 * See ISBN 0-465-08786-8, "The way we think" by Fauconnier and Turner, for information on conceptual blending.
 *
 * \author Bruno Angeles (bruno.angeles@mail.mcgill.ca)
 *
 * \version 1.0
 *
 * \date 2012/08/14
 *
 */

#include "ofMain.h"

// GUI
#include "UI/ofxTactoSHPM.h"
#include "UI/ofxTactoBeatNode.h"
#include "ofxStovetop.h"
#include "ofxCookableNode.h"
#include <vector>

/// A class that implements a dragged point.
class draggedBGPoint : public ofPoint
{
public:
	draggedBGPoint(float x, float y); ///< Constructor
	void					draw(); ///< Regular OpenFrameworks function.
	void					setPosition(ofPoint pt); ///< Sets the position of the point.
	bool					isPointInside(ofPoint pt); ///< Returns true if and only if the queried point is inside the dragged point.
	float					getDragDistance(); ///< Returns the distance dragged by the point.
	ofPoint					getDrag(bool incremental = false); /// Returns the drag offset of the point.
private:
	float					m_radius; ///< The radius of the point.
	int						m_color; ///< The colour of the point.
	ofPoint					m_startingPoint; ///< The origin of the drag motion.
	ofPoint					m_positionLastFrame; ///< A reference point to compute the drag motion.
};

/// A class that implements the MAKING MUSIC IS COOKING metaphor.
class metaphorKitchen : public ofBaseApp
{
public:	
	void					setup(); ///< Regular OpenFrameworks function.
	void					update(); ///< Regular OpenFrameworks function.
	void					draw(); ///< Regular OpenFrameworks function.
	void					exit(); ///< Regular OpenFrameworks function.
	
	void					windowResized(int w, int h); ///< Regular OpenFrameworks function.
	void					keyPressed  (int key); ///< Regular OpenFrameworks function.
	void					mouseDragged(int x, int y, int button); ///< Regular OpenFrameworks function.
	void					mousePressed(int x, int y, int button); ///< Regular OpenFrameworks function.
	void					mouseReleased(int x, int y, int button); ///< Regular OpenFrameworks function.
	void					touchDown(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	void					touchMoved(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	void					touchUp(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	void					mouseTouchDown(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch down events.
	void					mouseTouchMoved(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch moved events.
	void					mouseTouchUp(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch up events.

	// Getters & setters
	ofPoint					getDragOffset() { return m_dragOffset; } ///< Returns the drag offset of the motion. \return A point representing the offset of the drag.

private:
	ofPoint					m_dragOffset; ///< The offset caused by dragging the element around.
	ofxTactoSHPM			m_shpmMenu; ///< The Stacked Half-Pie Menu (SHPM) of musical elements.
	ofxStovetop				m_stovetops[4]; ///< The four stove tops in the metaphor.
	int						m_nBPM; ///< The tempo in beats per minute of the music.
	vector<draggedBGPoint>	m_draggedBackgroundPoints; ///< A vector of points being dragged in the background of the user interface.
	int						m_nTimeOfCreationMs; ///< The time in milliseconds of creation of the metaphor.
};

#endif
