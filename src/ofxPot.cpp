#include "ofxPot.h"
#include "UI/ofxTactoSHPM.h"
#include "ofApp.h"

#include "TactosonixHelpers.h"
using namespace TactoHelpers;

/** \param _color The colour in hexadecimal values of the pot.
* \param _radius The radius in pixels of the pot.
* \param _origin The point of origin of the pot.
* \param _bpm The tempo in beats per minute of the pot's loops.
*/
ofxPot::ofxPot(int _color, int _radius, ofPoint _origin, int _bpm) :
m_nColor(_color), m_ptOrigin(_origin), m_nRadius(_radius), m_ptOriginalPosition(_origin), m_nBPM(_bpm), m_bShowingLoopFeedback(false)
{
	m_fVolume = 1.0f;
	m_nPeriodSingleBeatMs = (int)(1000 * 60 / m_nBPM);
	m_nCurrentBeat = 0;
	m_nTimeOfCreationMs = ofGetElapsedTimeMillis();
	
#ifdef _DEBUG
	m_infoText.init("fonts/arial.ttf", 40);
	m_infoText.setText("");
	m_infoText.wrapTextX(ofGetWidth() * 3/4);
#endif
}

void ofxPot::exit()
{
	vector<ofxCookableNode>::iterator It;
	// LOOPS 1
	for (It = m_activeCookNodesLoop1.begin(); It != m_activeCookNodesLoop1.end(); ++It)
	{
		It->exit();
	}
	// LOOPS 2
	for (It = m_activeCookNodesLoop2.begin(); It != m_activeCookNodesLoop2.end(); ++It)
	{
		It->exit();
	}
	// LOOPS 3
	for (It = m_activeCookNodesLoop3.begin(); It != m_activeCookNodesLoop3.end(); ++It)
	{
		It->exit();
	}
}

/** \param nCurrentBeat The current beat of the pot's sequencer.
*/
void ofxPot::update(int nCurrentBeat)
{
	// Figure out the current beat
	bool bNewBeat = false;
	if (m_nCurrentBeat != OFX_POT_NUMSEQUENCERSTEPS && nCurrentBeat != m_nCurrentBeat)
	{
		m_nCurrentBeat = nCurrentBeat;
		bNewBeat = true;
	}

	if (bNewBeat)
	{
		// LOOPS 1
		int numSoundsInVector = m_activeCookNodesLoop1.size();
		vector<ofxCookableNode>::iterator It = m_activeCookNodesLoop1.begin();

		if (numSoundsInVector == 1)
		{
			if (!It->getIsPlaying())
			{
				It->play();
			}
			It->incrementCurrLoopBeat();
		}
		else if (numSoundsInVector > 1)
		{
			// There are more sounds than needed
			It->stop(); // stop current sound
			It = m_activeCookNodesLoop1.erase(m_activeCookNodesLoop1.begin());
			It->play(); // play next sound
		}

		// LOOPS 2
		numSoundsInVector = m_activeCookNodesLoop2.size();
		It = m_activeCookNodesLoop2.begin();
		if (numSoundsInVector == 1)
		{
			if (!It->getIsPlaying())
			{
				It->play();
			}
			It->incrementCurrLoopBeat();
		}
		else if (numSoundsInVector > 1)
		{
			// There are more sounds than needed
			It->stop(); // stop current sound
			It = m_activeCookNodesLoop2.erase(m_activeCookNodesLoop2.begin());
			It->play(); // play next sound
		}

		// LOOPS 3
		numSoundsInVector = m_activeCookNodesLoop3.size();
		It = m_activeCookNodesLoop3.begin();
		if (numSoundsInVector == 1)
		{
			if (!It->getIsPlaying())
			{
				It->play();
			}
			It->incrementCurrLoopBeat();
		}
		else if (numSoundsInVector > 1)
		{
			// There are more sounds than needed
			It->stop(); // stop current sound
			It = m_activeCookNodesLoop3.erase(m_activeCookNodesLoop3.begin());
			It->play(); // play next sound
		}
	}
	
	// Update the volume of all playing nodes
	vector<ofxCookableNode>::iterator It;
	for (It = m_activeCookNodesLoop1.begin(); It != m_activeCookNodesLoop1.end(); ++It)
	{
		It->update(m_fVolume);
	}
	for (It = m_activeCookNodesLoop2.begin(); It != m_activeCookNodesLoop2.end(); ++It)
	{
		It->update(m_fVolume);
	}
	for (It = m_activeCookNodesLoop3.begin(); It != m_activeCookNodesLoop3.end(); ++It)
	{
		It->update(m_fVolume);
	}
}

/** \return The colour of the pot.
*/
int ofxPot::getColor()
{
	return m_nColor;
}

/**
 * \param pt The coordinates of the queried point.
 * \return Whether or not the queried point is inside the pot.
 */
bool ofxPot::isPointInside(ofPoint pt)
{
	bool returnValue = false;

	// pt will always be [0;1], while the origin and most importantly radius will be in world coordinates
	pt.x *= ofGetWidth();
	pt.y *= ofGetHeight();

	float fDistance = sqrt(pow(pt.x - m_ptOrigin.x, 2.0f) + pow(pt.y - m_ptOrigin.y, 2.0f));
	if (fDistance <= m_nRadius)
	{
		returnValue = true;
	}
	return returnValue;
}

void ofxPot::draw()
{
	// Draw a pot that follows the beat
	ofFill();
	int r = (m_nColor & 0xFF0000) >> 16;
	int g = (m_nColor & 0x00FF00) >> 8;
	int b = m_nColor & 0x0000FF;
	float fMaxAlpha = 255.0f;
	float fMinAlpha = 128.0f;
	float fDampingConstant = - m_nPeriodSingleBeatMs / (log(fMinAlpha / fMaxAlpha) / log(exp(1.0f))); // ln(x) = log10(x)/log10(e)
	float fAlphaBPM = fMaxAlpha * exp(- (float)(ofGetElapsedTimeMillis() % m_nPeriodSingleBeatMs) / fDampingConstant);
	ofSetColor(r, g, b, (int)fAlphaBPM);
	ofCircle(m_ptOrigin.x, m_ptOrigin.y, m_nRadius);

	// Draw the current beat around the pot, or as pot handle?
	float fCurrBeatPct = (float)(m_nCurrentBeat % OFX_POT_NUMSEQUENCERSTEPS) / (float)OFX_POT_NUMSEQUENCERSTEPS; // [0;1]
	// Polar coordinates
	float fCurrBeatAngleRads = fCurrBeatPct * TWO_PI;
	// Add 90 degrees to start at x,y=[0,1]
	cartesianCoords currBeatOffset = polToCar(m_nRadius, PI/2 + fCurrBeatAngleRads);
	ofPoint ptBeatOrigin(m_ptOrigin.x - currBeatOffset.x, m_ptOrigin.y - currBeatOffset.y);
	ofSetColor(r, g, b, 255);
	ofCircle(ptBeatOrigin, m_nRadius * 0.05);

	// Potentially show a crosshair for looping sections
	if (m_bShowingLoopFeedback)
	{
		ofSetColor(185);
		ofSetLineWidth(2.0f);
		ofPoint pt1(m_ptOrigin + ofPoint(-m_nRadius/2, 0));
		ofPoint pt2(m_ptOrigin + ofPoint(m_nRadius/2, 0));
		ofPoint pt3(m_ptOrigin + ofPoint(0, -m_nRadius/2));
		ofPoint pt4(m_ptOrigin + ofPoint(0, m_nRadius/2));
		ofLine(pt1, pt2);
		ofLine(pt3, pt4);
	}

	// Draw the nodes
	vector<ofxCookableNode>::iterator It;
	// LOOPS 1
	for (It = m_activeCookNodesLoop1.begin(); It != m_activeCookNodesLoop1.end(); ++It)
	{
		It->draw();
	}
	// LOOPS 2
	for (It = m_activeCookNodesLoop2.begin(); It != m_activeCookNodesLoop2.end(); ++It)
	{
		It->draw();
	}
	// LOOPS 3
	for (It = m_activeCookNodesLoop3.begin(); It != m_activeCookNodesLoop3.end(); ++It)
	{
		It->draw();
	}

	vector<ofxCookableNode*>::iterator It2;
	for (It2 = m_draggedBeats.begin(); It2 != m_draggedBeats.end(); It2++)
	{
		float fRelVolume = (*It2)->getRelativeVolume();
		ofNoFill();
		// get distance from node to centre of pot
		ofPoint potOrig = getOrigin();
		ofPoint beatOrig = (*It2)->getOrigin();
		ofPoint scaledDown(beatOrig.x / ofGetWidth(), beatOrig.y / ofGetHeight());
		if (isPointInside(scaledDown))
		{
			// Draw volume feedback
			ofSetLineWidth(fRelVolume * 10.0f);
			ofColor nodeColour = (*It2)->getColor();
			ofSetColor((*It2)->getColor(), fRelVolume * 255);
			float fDistance = sqrt(pow(potOrig.x - beatOrig.x, 2) + pow(potOrig.y - beatOrig.y, 2));
			ofCircle(potOrig, fDistance);

			// Draw lowpass feedback
			(*It2)->drawLowpass();
		}
	}

#ifdef _DEBUG
	// Draw some beat info
	m_infoText.setText(ofToString(m_nCurrentBeat));
	m_infoText.setColor(255, 0, 0, 255);
	float debugWidth = m_infoText.getWidth();
	float debugHeight = m_infoText.getHeight();
	m_infoText.draw(m_ptOrigin.x - debugWidth/2.0f, m_ptOrigin.y - debugHeight/2.0f);
#endif
}

/** \return The point of origin of the pot, including the dragged offset..
*/
ofPoint ofxPot::getOrigin()
{
	return m_ptOrigin;
}

/** \param _origin The point of origin of the pot, including the dragged offset.
*/
void ofxPot::setOrigin(ofPoint _origin)
{
	m_ptOrigin.x = _origin.x;
	m_ptOrigin.y = _origin.y;
}

/** \return The point of origin of the pot, without the dragged offset.
*/
ofPoint ofxPot::getOriginInit()
{
	return m_ptOriginalPosition;
}

/** \param The point of origin of the pot, without the dragged offset.
*/
void ofxPot::setOriginInit(ofPoint _origin)
{
	m_ptOriginalPosition = _origin;
}

/** \return The radius in pixels of the pot.
*/
int ofxPot::getRadius()
{
	return m_nRadius;
}

/** \param _nRadius The radius in pixels of the pot.
*/
void ofxPot::setRadius(int _nRadius)
{
	m_nRadius = _nRadius;
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void ofxPot::mouseDragged(int x, int y, int button)
{
	mouseTouchMoved(x, y, true, button);
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void ofxPot::mousePressed(int x, int y, int button)
{
	mouseTouchDown(x, y, true, button);
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void ofxPot::mouseReleased(int x, int y, int button)
{
}

/** \param _nodeToCook A new cookable element of music to add to the pot.
*/
void ofxPot::addCookElement(ofxCookableNode _nodeToCook)
{
	switch (_nodeToCook.getSoundType())
	{
	case TACTO_LOOPTYPE_DRUMS:
		m_activeCookNodesLoop1.push_back(_nodeToCook);
		break;
	case TACTO_LOOPTYPE_BASS:
		m_activeCookNodesLoop2.push_back(_nodeToCook);
		break;
	case TACTO_LOOPTYPE_LEAD:
		m_activeCookNodesLoop3.push_back(_nodeToCook);
		break;
	}
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \param button The ID of the mouse button, in the case of mouse input.
* \param touchId The ID of the touch event, in the case of touch input.
*/
bool ofxPot::mouseTouchDown(float x, float y, bool fullRange, int button, int touchId)
{
    ofPoint ptCompare(x, y);
    if (fullRange)
    {
        ptCompare.x /= ofGetWidth();
        ptCompare.y /= ofGetHeight();
    }

    bool bTouchIsInsidePot = isPointInside(ptCompare);

	bool bFoundANodeToMove = false;
	vector<ofxCookableNode>::iterator It;
	// LOOPS 1
	for (It = m_activeCookNodesLoop1.begin(); It != m_activeCookNodesLoop1.end(); ++It)
	{
		if(It->isPointInside(ptCompare))
		{
			m_draggedBeats.push_back(&*It);
			bFoundANodeToMove = true;
			break;
		}
	}
	// LOOPS 2
	if (!bFoundANodeToMove)
	{
		for (It = m_activeCookNodesLoop2.begin(); It != m_activeCookNodesLoop2.end(); ++It)
		{
			if(It->isPointInside(ptCompare))
			{
				m_draggedBeats.push_back(&*It);
				bFoundANodeToMove = true;
				break;
			}
		}
	}

	// LOOPS 3
	if (!bFoundANodeToMove)
	{
		for (It = m_activeCookNodesLoop3.begin(); It != m_activeCookNodesLoop3.end(); ++It)
		{
			if(It->isPointInside(ptCompare))
			{
				m_draggedBeats.push_back(&*It);
				bFoundANodeToMove = true;
				break;
			}
		}
	}

	return bFoundANodeToMove | bTouchIsInsidePot;
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \param button The ID of the mouse button, in the case of mouse input.
* \param touchId The ID of the touch event, in the case of touch input.
*/
bool ofxPot::mouseTouchMoved(float x, float y, bool fullRange, int button, int touchId)
{
	bool bFoundANodeToMove = false;

	// Move the points
    ofPoint ptCompare(x, y);
    if (fullRange)
    {
        ptCompare.x /= ofGetWidth();
        ptCompare.y /= ofGetHeight();
    }
	
	// Figure out if we have to show the crosshair
	bool bShowLoopFeedback = false;
	for (vector<ofxCookableNode>::iterator It = m_activeCookNodesLoop1.begin(); It != m_activeCookNodesLoop1.end(); ++It)
	{
		float fDistFromCenter = sqrtf(pow(m_ptOrigin.x - It->getOrigin().x, 2.0f) + pow(m_ptOrigin.y - It->getOrigin().y, 2.0f));
		if (fDistFromCenter / m_nRadius < RADIUS_LOOP_PCT)
		{
			bShowLoopFeedback = true;
			break;
		}
	}
	if (!bShowLoopFeedback)
	{
		for (vector<ofxCookableNode>::iterator It = m_activeCookNodesLoop2.begin(); It != m_activeCookNodesLoop2.end(); ++It)
		{
			float fDistFromCenter = sqrtf(pow(m_ptOrigin.x - It->getOrigin().x, 2.0f) + pow(m_ptOrigin.y - It->getOrigin().y, 2.0f));
			if (fDistFromCenter / m_nRadius < RADIUS_LOOP_PCT)
			{
				bShowLoopFeedback = true;
				break;
			}
		}
	}
	if (!bShowLoopFeedback)
	{
		for (vector<ofxCookableNode>::iterator It = m_activeCookNodesLoop3.begin(); It != m_activeCookNodesLoop3.end(); ++It)
		{
			float fDistFromCenter = sqrtf(pow(m_ptOrigin.x - It->getOrigin().x, 2.0f) + pow(m_ptOrigin.y - It->getOrigin().y, 2.0f));
			if (fDistFromCenter / m_nRadius < RADIUS_LOOP_PCT)
			{
				bShowLoopFeedback = true;
				break;
			}
		}
	}
	m_bShowingLoopFeedback = bShowLoopFeedback;

	vector<ofxCookableNode*>::iterator It;
	for (It = m_draggedBeats.begin(); It != m_draggedBeats.end(); It++)
	{
		if ((*It)->isPointInside(ptCompare))
	    {
			// Move the node, don't forget the offset
			ofPoint ptWithOffset(ptCompare.x, ptCompare.y);
			ofPoint offset = ((ofApp*)ofGetAppPtr())->getKitchen()->getDragOffset();
			ptWithOffset -= offset;
			(*It)->setOriginInit(ptWithOffset, false);
			(*It)->setOrigin(ofPoint(ptCompare.x, ptCompare.y), false);
			
			// Change volume
			float fDistFromCenter = sqrtf(pow(m_ptOrigin.x - ptCompare.x*ofGetWidth(), 2.0f) + pow(m_ptOrigin.y - ptCompare.y*ofGetHeight(), 2.0f));
			float fVolume = 1.0f - fDistFromCenter / m_nRadius;
			(*It)->setRelativeVolume(fVolume*1.5f);

			// Change lowpass
			float yStart = m_ptOrigin.y + getRadius(); // lowest point in the pot
			float tmp1 = ptCompare.y*ofGetHeight();
			float fRelHeight = (yStart - tmp1)/(2*getRadius());
			(*It)->setLowpass(fRelHeight);

			// Change loop
			int nLoopLength = 0;
			if (fDistFromCenter / m_nRadius < RADIUS_LOOP_PCT)
			{
				// Node is close enough to center, figure out in which quadrant it is
				ofVec2f v1(ptCompare.x - m_ptOrigin.x/ofGetWidth(), ptCompare.y - m_ptOrigin.y/ofGetHeight());
				if (v1.x > 0.0f && v1.y > 0.0f)
				{
					// Lower right
					nLoopLength = 2;
				}
				else if (v1.x > 0.0f && v1.y < 0.0f)
				{
					// Top right
					nLoopLength = 1;
				}
				else if (v1.x < 0.0f && v1.y > 0.0f)
				{
					// Lower left
					nLoopLength = 4;
				}
				else if (v1.x < 0.0f && v1.y < 0.0f)
				{
					// Top left
					nLoopLength = 8;
				}
			}
			(*It)->setLoopBeats(nLoopLength);
		}
	}
	return bFoundANodeToMove;
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \param button The ID of the mouse button, in the case of mouse input.
* \param touchId The ID of the touch event, in the case of touch input.
*/
void ofxPot::mouseTouchUp(float x, float y, bool fullRange, int button, int touchId)
{
    ofPoint ptCompare(x, y);
    if (fullRange)
    {
        ptCompare.x /= ofGetWidth();
        ptCompare.y /= ofGetHeight();
    }

	// Remove the dragged beats
	vector<ofxCookableNode*>::iterator It1 = m_draggedBeats.begin();
	while (It1 != m_draggedBeats.end())
	{
		if ((*It1)->isPointInside(ptCompare))
	    {
			// Remove this dragged beat
			It1 = m_draggedBeats.erase(It1);
	    }
		else
		{
			++It1;
		}
	}
	
	// Remove the loops when dragged outside pot
	// LOOPS 1
	vector<ofxCookableNode>::iterator It = m_activeCookNodesLoop1.begin();
	while (It != m_activeCookNodesLoop1.end())
	{
		if ((*It).isPointInside(ptCompare))
	    {
			// We are dragging this node
			if (!isPointInside(ptCompare))
			{
				// Point is not on this pot anymore!
				It = m_activeCookNodesLoop1.erase(It);
			}
			else
			{
				++It;
			}
	    }
		else
		{
			++It;
		}
	}
	
	// LOOPS 2
	It = m_activeCookNodesLoop2.begin();
	while (It != m_activeCookNodesLoop2.end())
	{
		if ((*It).isPointInside(ptCompare))
	    {
			// We are dragging this node
			if (!isPointInside(ptCompare))
			{
				// Point is not on this pot anymore!
				It = m_activeCookNodesLoop2.erase(It);
			}
			else
			{
				++It;
			}
	    }
		else
		{
			++It;
		}
	}
	
	// LOOPS 3
	It = m_activeCookNodesLoop3.begin();
	while (It != m_activeCookNodesLoop3.end())
	{
		if ((*It).isPointInside(ptCompare))
	    {
			// We are dragging this node
			if (!isPointInside(ptCompare))
			{
				// Point is not on this pot anymore!
				It = m_activeCookNodesLoop3.erase(It);
			}
			else
			{
				++It;
			}
	    }
		else
		{
			++It;
		}
	}
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxPot::touchDown(float x, float y, int touchId)
{
	mouseTouchDown(x, y, false, 0, touchId);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxPot::touchMoved(float x, float y, int touchId)
{
	mouseTouchMoved(x, y, false, 0, touchId);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxPot::touchUp(float x, float y, int touchId)
{
	mouseTouchUp(x, y, false, 0, touchId);
}

/** \param deltaDrag The amount of drag of the motion.
*/
void ofxPot::drag(ofPoint deltaDrag)
{
	m_ptOrigin = m_ptOriginalPosition + deltaDrag;

	// Drag pot elements
	vector<ofxCookableNode>::iterator It;
	// LOOPS 1
	for (It = m_activeCookNodesLoop1.begin(); It != m_activeCookNodesLoop1.end(); ++It)
	{
		It->drag(deltaDrag);
	}
	// LOOPS 2
	for (It = m_activeCookNodesLoop2.begin(); It != m_activeCookNodesLoop2.end(); ++It)
	{
		It->drag(deltaDrag);
	}
	// LOOPS 3
	for (It = m_activeCookNodesLoop3.begin(); It != m_activeCookNodesLoop3.end(); ++It)
	{
		It->drag(deltaDrag);
	}
}