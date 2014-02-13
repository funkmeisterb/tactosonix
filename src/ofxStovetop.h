#ifndef _OFX_STOVETOP
#define _OFX_STOVETOP

/**
 * \class ofxStovetop
 *
 * \brief This is an implementation of a stove top in the cooking metaphor.
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
#include "ofxPot.h"

#define STOVE_COLOR 0x323232

/// A class that displays stove top information.
class stoveInformation
{
public:
	/// The location of the stove top.
	enum tactoStove {
		INVALID = -1, ///< Not a valid stove top.
		FRONT_RIGHT = 0, ///< The front right stove top.
		FRONT_LEFT = 1, ///< The front left stove top.
		REAR_LEFT = 2, ///< The rear left stove top.
		REAR_RIGHT = 3, ///< The rear right stove top.
	};

	/** \brief Constructor
	* \param stoveId The ID of the stove.
	* \param origin The point of origin of the stove top information.
	*/
	stoveInformation(tactoStove stoveId, ofPoint origin) :
	  m_ptOrigin(origin),
	  m_ptOriginalPosition(origin),
	  m_stoveId(stoveId)
		{};
	
	/// Default constructor
	stoveInformation() :
	  m_ptOrigin(),
	  m_ptOriginalPosition(),
	  m_stoveId(INVALID)
		{};
	void							draw(); ///< Regular OpenFrameworks function.
	void							drag(ofPoint deltaDrag); ///< Drags the stove top information according to the specified offset.
	void							setOrigin(ofPoint ptOrigin); ///< Sets the point of origin of the stove top information, without the dragged offset.
	ofPoint							getOrigin() { return m_ptOrigin; } ///< Returns the point of origin of the stove top information, without the dragged offset. \return The point of origin of the stove top information, including the dragged offset.
	void							setOriginInit(ofPoint ptOrigin); ///< Sets the point of origin of the stove top information, including the dragged offset.
	ofPoint							getOriginInit() { return m_ptOriginalPosition; } ///< Returns the point of origin of the stove top information, without the dragged offset. \return The point of origin of the stove top information, without the dragged offset.
private:
	ofPoint							m_ptOrigin; ///< The point of origin of the stove top information, including the dragged offset.
 	ofPoint							m_ptOriginalPosition; ///< The initial point of origin of the stove top information.
	tactoStove						m_stoveId; ///< The ID of the stove top.
};

/// A class that implements a stove top.
class ofxStovetop : public ofBaseApp
{
public:
	ofxStovetop(stoveInformation::tactoStove _id, ofPoint _origin, int _bpm); ///< Constructor
	ofxStovetop() :
		m_ptOrigin(0,0), m_ptOriginalPosition(0,0), m_stoveId(stoveInformation::INVALID) {}; ///< Default constructor

	void									update(int nCurrentBeat); ///< Regular OpenFrameworks function.
	void									exit(); ///< Regular OpenFrameworks function.
	void									draw(); ///< Regular OpenFrameworks function.
	void									windowResized(int w, int h); ///< Regular OpenFrameworks function.

	ofPoint									getOrigin(); ///< Returns the point of origin of the stove top.
	void									setOrigin(ofPoint _origin); ///< Sets the point of origin of the stove top.
	ofxPot*									getPot() { return &m_pot; } ///< Returns the pot (or burner) of the stove top. \return The pot (or burner) of the stove top.
	void									drag(ofPoint deltaDrag); ///< Drags the stove top by the specified offset.
	ofPoint									getTotalDrag() { return m_ptTotalDrag; } ///< Returns the total offset caused by dragging.
 	void									moveOffset(ofPoint offset); ///< Move all elements by the specified offset.
	bool									isPointInPot(float x, float y, bool fullRange); ///< Returns true if and only if the queried coordinates are inside the stove top.
	void									addCookElement(ofxCookableNode _nodeToCook); ///< Adds a cook element to the pot (or burner).
	stoveInformation::tactoStove			getID() { return m_stoveId; } ///< Returns the ID of the stove. \retun The ID of the stove top.
	float									getCrossfadeVolume(ofPoint pt); ///< Returns the crossfade volume of the stove top at the queried position.

	void									mouseDragged(int x, int y, int button); ///< Regular OpenFrameworks function.
	void									touchDown(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	void									touchMoved(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	void									touchUp(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	bool									mouseTouchDown(float x, float y, bool fullRange, int button = 0, int touchId = 0);
	bool									mouseTouchMoved(float x, float y, bool fullRange, int button = 0, int touchId = 0);
	void									mouseTouchUp(float x, float y, bool fullRange, int button = 0, int touchId = 0);

private:
	ofPoint									m_ptOrigin; ///< The point of origin of the stove top information, including the dragged offset.
	ofPoint									m_ptOriginalPosition; ///< The initial point of origin of the stove top information.
	ofxPot									m_pot; ///< The pot (or burner) of the stove top.
	stoveInformation						m_stoveInfo; ///< The stove top information.
	stoveInformation::tactoStove			m_stoveId; ///< The ID of the stove top.
	ofPoint									m_ptTotalDrag; ///< The normalized ([0;1] range) amount of drag motion that is applied to the stove top.
};

#endif
