#include "ofxStovetop.h"
#include "UI/ofxTactoSHPM.h"

// ****************************************************************************************
/** \param deltaDrag The amount of drag of the motion.
*/
void stoveInformation::drag(ofPoint deltaDrag)
{
	m_ptOrigin = m_ptOriginalPosition + deltaDrag;
}

/** \param ptOrigin The point of origin of the stove top information, including the dragged offset.
*/
void stoveInformation::setOrigin(ofPoint ptOrigin)
{
	m_ptOrigin = ptOrigin;
}

/** \param _origin The point of origin of the stove top, without the dragged offset.
*/
void stoveInformation::setOriginInit(ofPoint _origin)
{
	m_ptOriginalPosition = _origin;
}


void stoveInformation::draw()
{
	ofSetColor(0xFF, 0xFF, 0xFF, 100);
	ofNoFill();
	float infoBoxWidth = 0.1f * ofGetWidth();
	float infoBoxHeight = 0.1f * ofGetHeight();
	ofSetLineWidth(4.0f);
	ofRect(m_ptOrigin, infoBoxWidth, infoBoxHeight);
	ofPoint ptInfoBoxTopCentre(m_ptOrigin.x + infoBoxWidth / 2, m_ptOrigin.y);
	ofPoint ptInfoBoxBottomCentre(m_ptOrigin.x + infoBoxWidth / 2, m_ptOrigin.y + infoBoxHeight);
	ofPoint ptInfoBoxLeftCentre(m_ptOrigin.x, m_ptOrigin.y + infoBoxHeight / 2);
	ofPoint ptInfoBoxRightCentre(m_ptOrigin.x + infoBoxWidth, m_ptOrigin.y + infoBoxHeight / 2);
	ofLine(ptInfoBoxTopCentre, ptInfoBoxBottomCentre);
	ofLine(ptInfoBoxLeftCentre, ptInfoBoxRightCentre);
	// Draw circle for the stove
	ofPoint ptStoveCentre;
	float miniStoveRadius = min(infoBoxWidth / 4, infoBoxHeight / 4);
	miniStoveRadius *= 0.8f; // make a bit smaller
	switch (m_stoveId)
	{
	case FRONT_RIGHT:
		ptStoveCentre.x = m_ptOrigin.x + 0.75f * infoBoxWidth;
		ptStoveCentre.y = m_ptOrigin.y + 0.75f * infoBoxHeight;
		ofFill();
		ofCircle(ptStoveCentre, miniStoveRadius);
		break;
	case FRONT_LEFT:
		ptStoveCentre.x = m_ptOrigin.x + 0.25f * infoBoxWidth;
		ptStoveCentre.y = m_ptOrigin.y + 0.75f * infoBoxHeight;
		ofFill();
		ofCircle(ptStoveCentre, miniStoveRadius);
		break;
	case REAR_LEFT:
		ptStoveCentre.x = m_ptOrigin.x + 0.25f * infoBoxWidth;
		ptStoveCentre.y = m_ptOrigin.y + 0.25f * infoBoxHeight;
		ofFill();
		ofCircle(ptStoveCentre, miniStoveRadius);
		break;
	case REAR_RIGHT:
		ptStoveCentre.x = m_ptOrigin.x + 0.75f * infoBoxWidth;
		ptStoveCentre.y = m_ptOrigin.y + 0.25f * infoBoxHeight;
		ofFill();
		ofCircle(ptStoveCentre, miniStoveRadius);
		break;
	}
	ofSetLineWidth(1.0f);
}

// ****************************************************************************************
/** \param _id The ID of the stove top.
* \param _origin The point of origin of the stove top.
* \param _bpm The tempo in beats per minute of the stove top's music.
*/
ofxStovetop::ofxStovetop(stoveInformation::tactoStove _id, ofPoint _origin, int _bpm) :
	m_ptOrigin(_origin), m_ptOriginalPosition(_origin), m_stoveId(_id), m_ptTotalDrag(0, 0)
{
	int nRadius = ofGetHeight()/3;
	m_stoveInfo = stoveInformation();
	
	ofPoint ptPotOrigin = ofPoint(0, 0);
	
	switch (m_stoveId)
	{
	case stoveInformation::FRONT_RIGHT:
		m_pot = ofxPot(STOVE_COLOR, nRadius, _origin, _bpm);
		ptPotOrigin += 2;
		m_stoveInfo = stoveInformation(_id, ptPotOrigin);
		break;
	case stoveInformation::FRONT_LEFT:
		m_pot = ofxPot(STOVE_COLOR, nRadius, _origin, _bpm);
		ptPotOrigin = ofPoint(- ofGetWidth(), 0);
		ptPotOrigin += 2;
		m_stoveInfo = stoveInformation(_id, ptPotOrigin);
		break;
	case stoveInformation::REAR_LEFT:
		m_pot = ofxPot(STOVE_COLOR, nRadius, _origin, _bpm);
		ptPotOrigin = ofPoint(- ofGetWidth(), -ofGetHeight());
		ptPotOrigin += 2;
		m_stoveInfo = stoveInformation(_id, ptPotOrigin);
		break;
	case stoveInformation::REAR_RIGHT:
		m_pot = ofxPot(STOVE_COLOR, nRadius, _origin, _bpm);
		ptPotOrigin = ofPoint(0, - ofGetHeight());
		ptPotOrigin += 2;
		m_stoveInfo = stoveInformation(_id, ptPotOrigin);
		break;
	}
}

/** \param nCurrentBeat The current beat of the stove top's sequencer.
*/
void ofxStovetop::update(int nCurrentBeat)
{
	m_pot.update(nCurrentBeat);
}

void ofxStovetop::exit()
{
	m_pot.exit();
}

void ofxStovetop::draw()
{
	m_pot.draw();
	m_stoveInfo.draw();
}

void ofxStovetop::windowResized(int w, int h)
{
	ofPoint ptPotOrigin(0, 0);
	ofPoint ptPotOffset(ofGetWidth()/2, ofGetHeight()/3);
	ofPoint ptPotOriginInfo(0, 0);
	
	switch (m_stoveId)
	{
	case stoveInformation::FRONT_RIGHT:
		// Reset the coordinates of the pot.
		ptPotOrigin = ptPotOffset;
		m_pot.setOriginInit(ptPotOrigin);
		ptPotOrigin.x += m_ptTotalDrag.x * ofGetWidth();
		ptPotOrigin.y += m_ptTotalDrag.y * ofGetHeight();
		m_pot.setOrigin(ptPotOrigin);
		// Reset the coordinates of the stove top information.
		ptPotOriginInfo = ofPoint(0, 0);
		ptPotOriginInfo += 2;
		m_stoveInfo.setOrigin(ptPotOriginInfo);
		m_stoveInfo.setOriginInit(ptPotOriginInfo);
		// Apply the offset
		drag(m_ptTotalDrag * ofGetWidth());
		break;
	case stoveInformation::FRONT_LEFT:
		// Reset the coordinates of the pot.
		ptPotOrigin = ofPoint(-ofGetWidth(), 0);
		ptPotOrigin += ptPotOffset;
		m_pot.setOriginInit(ptPotOrigin);
		ptPotOrigin.x += m_ptTotalDrag.x * ofGetWidth();
		ptPotOrigin.y += m_ptTotalDrag.y * ofGetHeight();
		m_pot.setOrigin(ptPotOrigin);
		// Reset the coordinates of the stove top information.
		ptPotOriginInfo = ofPoint(-ofGetWidth(), 0);
		ptPotOriginInfo += 2;
		m_stoveInfo.setOrigin(ptPotOriginInfo);
		m_stoveInfo.setOriginInit(ptPotOriginInfo);
		// Apply the offset
		drag(m_ptTotalDrag * ofGetWidth());
		break;
	case stoveInformation::REAR_LEFT:
		// Reset the coordinates of the pot.
		ptPotOrigin = ofPoint(-ofGetWidth(), -ofGetHeight());
		ptPotOrigin += ptPotOffset;
		m_pot.setOriginInit(ptPotOrigin);
		ptPotOrigin.x += m_ptTotalDrag.x * ofGetWidth();
		ptPotOrigin.y += m_ptTotalDrag.y * ofGetHeight();
		m_pot.setOrigin(ptPotOrigin);
		// Reset the coordinates of the stove top information.
		ptPotOriginInfo = ofPoint(-ofGetWidth(), -ofGetHeight());
		ptPotOriginInfo += 2;
		m_stoveInfo.setOrigin(ptPotOriginInfo);
		m_stoveInfo.setOriginInit(ptPotOriginInfo);
		// Apply the offset
		drag(m_ptTotalDrag * ofGetWidth());
		break;
	case stoveInformation::REAR_RIGHT:
		// Reset the coordinates of the pot.
		ptPotOrigin = ofPoint(0, -ofGetHeight());
		ptPotOrigin += ptPotOffset;
		m_pot.setOriginInit(ptPotOrigin);
		ptPotOrigin.x += m_ptTotalDrag.x * ofGetWidth();
		ptPotOrigin.y += m_ptTotalDrag.y * ofGetHeight();
		m_pot.setOrigin(ptPotOrigin);
		// Reset the coordinates of the stove top information.
		ptPotOriginInfo = ofPoint(0, -ofGetHeight());
		ptPotOriginInfo += 2;
		m_stoveInfo.setOrigin(ptPotOriginInfo);
		m_stoveInfo.setOriginInit(ptPotOriginInfo);
		// Apply the offset
		drag(m_ptTotalDrag * ofGetWidth());
		break;
	}
}

/** \return The point of origin of the stove top.
*/
ofPoint ofxStovetop::getOrigin()
{
	return m_ptOrigin;
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void ofxStovetop::mouseDragged(int x, int y, int button)
{
	mouseTouchMoved(x, y, true, button);
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \param button The ID of the mouse button, in the case of mouse input.
* \param touchId The ID of the touch event, in the case of touch input.
*/
bool ofxStovetop::mouseTouchDown(float x, float y, bool fullRange, int button, int touchId)
{
	return m_pot.mouseTouchDown(x, y, fullRange, button, touchId);
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \param button The ID of the mouse button, in the case of mouse input.
* \param touchId The ID of the touch event, in the case of touch input.
*/
bool ofxStovetop::mouseTouchMoved(float x, float y, bool fullRange, int button, int touchId)
{
	return m_pot.mouseTouchMoved(x, y, fullRange, button, touchId);
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \param button The ID of the mouse button, in the case of mouse input.
* \param touchId The ID of the touch event, in the case of touch input.
*/
void ofxStovetop::mouseTouchUp(float x, float y, bool fullRange, int button, int touchId)
{
	m_pot.mouseTouchUp(x, y, fullRange, button, touchId);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxStovetop::touchDown(float x, float y, int touchId)
{
	mouseTouchDown(x, y, false, 0, touchId);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxStovetop::touchMoved(float x, float y, int touchId)
{
	mouseTouchMoved(x, y, false, 0, touchId);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxStovetop::touchUp(float x, float y, int touchId)
{
	mouseTouchUp(x, y, false, 0, touchId);
}

/** \param deltaDrag The amount of drag of the motion.
*/
void ofxStovetop::drag(ofPoint deltaDrag)
{
	m_ptTotalDrag.x = deltaDrag.x / ofGetWidth();
	m_ptTotalDrag.y = deltaDrag.y / ofGetWidth();
	m_ptOrigin = m_ptOriginalPosition + deltaDrag;

	m_pot.drag(deltaDrag);
	m_stoveInfo.drag(deltaDrag);
}

/** \param offset The offset to apply to the stove top's elements.
*/
void ofxStovetop::moveOffset(ofPoint offset)
{
	// Change the position of all elements
	ofPoint ptOrigin = m_pot.getOriginInit();
	m_pot.setOrigin(ptOrigin + offset);
	m_pot.setOriginInit(ptOrigin + offset);
	ptOrigin = m_stoveInfo.getOrigin();
	m_stoveInfo.setOrigin(ptOrigin + offset);
}

/**
* \param x The x coordinate of the queried point.
* \param y The y coordinate of the queried point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \return Whether or not the queried point is inside the pot.
*/
bool ofxStovetop::isPointInPot(float x, float y, bool fullRange)
{
    ofPoint ptCompare(x, y);
    if (fullRange)
    {
        ptCompare.x /= ofGetWidth();
        ptCompare.y /= ofGetHeight();
    }

	return m_pot.isPointInside(ptCompare);
}

/** \param _nodeToCook The node to add to the stove top's pot (or burner).
*/
void ofxStovetop::addCookElement(ofxCookableNode _nodeToCook)
{
	m_pot.addCookElement(_nodeToCook);
}

/** \param pt The queried point.
* \return The crossfade volume to apply to the stove top.
*/
float ofxStovetop::getCrossfadeVolume(ofPoint pt)
{
	float currVolume = 0.0f;

	// Crossfading volume determined by path on 3D surface
	switch (m_stoveId)
	{
	case stoveInformation::FRONT_RIGHT:
		if (pt.x == 0.0f)
		{
			// Simple 2D problem
			currVolume = 1.0f - pt.y;
		}
		else if (pt.y == 0.0f)
		{
			// Simple 2D problem
			currVolume = 1.0f - pt.x;
		}
		else if (pt.x <= pt.y)
		{
			// Intersection of plane and line
			currVolume = 1.0f - pt.y;
		}
		else if (pt.x > pt.y)
		{
			// Intersection of plane and line
			currVolume = 1.0f - pt.x;
		}
		else
		{
			currVolume = 0.0f;
		}
		break;
	case stoveInformation::FRONT_LEFT:
		if (pt.x == 1.0f)
		{
			// Simple 2D problem
			currVolume = 1.0f - pt.y;
		}
		else if (pt.y == 0.0f)
		{
			// Simple 2D problem
			currVolume = pt.x;
		}
		else if (1.0f - pt.x <= pt.y)
		{
			// Intersection of plane and line
			currVolume = 1.0f - pt.y;
		}
		else if (1.0f - pt.x > pt.y)
		{
			// Intersection of plane and line
			currVolume = pt.x;
		}
		else
		{
			currVolume = 0.0f;
		}
		break;
	case stoveInformation::REAR_LEFT:
		if (pt.x == 1.0f)
		{
			// Simple 2D problem
			currVolume = pt.y;
		}
		else if (pt.y == 1.0f)
		{
			// Simple 2D problem
			currVolume = pt.x;
		}
		else if (pt.x <= pt.y)
		{
			// Intersection of plane and line
			currVolume = pt.x;
		}
		else if (pt.x > pt.y)
		{
			// Intersection of plane and line
			currVolume = pt.y;
		}
		else
		{
			currVolume = 0.0f;
		}
		break;
	case stoveInformation::REAR_RIGHT:
		if (pt.x == 0.0f)
		{
			// Simple 2D problem
			currVolume = pt.y;
		}
		else if (pt.y == 1.0f)
		{
			// Simple 2D problem
			currVolume = 1.0f - pt.x;
		}
		else if (1.0f - pt.x <= pt.y)
		{
			// Intersection of plane and line
			currVolume = 1.0f - pt.x;
		}
		else if (1.0f - pt.x > pt.y)
		{
			// Intersection of plane and line
			currVolume = pt.y;
		}
		else
		{
			currVolume = 0.0f;
		}
		break;
	}
	return currVolume;
}
