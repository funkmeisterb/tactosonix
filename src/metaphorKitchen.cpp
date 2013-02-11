#include "metaphorKitchen.h"
#include "TactosonixHelpers.h"
#include "assert.h"

// ****************************************************************************************
/** \param x The x coordinate of the point.
* \param y The y coordinate of the point.
*/
draggedBGPoint::draggedBGPoint(float x, float y)
	: ofPoint(x, y)
{
	m_startingPoint.x = x;
	m_startingPoint.y = y;
	m_positionLastFrame = m_startingPoint;

	m_radius = 20.0f;
	m_color = 0xFFFFFF;
}

void draggedBGPoint::draw()
{
	ofSetHexColor(m_color);
	ofCircle(x * ofGetWidth(), y * ofGetHeight(), m_radius);
}

/** \param pt The point of origin of the draggable point.
*/
void draggedBGPoint::setPosition(ofPoint pt)
{
	m_positionLastFrame = ofPoint(x, y);
	x = pt.x;
	y = pt.y;
}

/**
 * \param pt The coordinates of the queried point.
 * \return Whether or not the queried point is inside the point.
 */
bool draggedBGPoint::isPointInside(ofPoint pt)
{
	bool returnValue = false;
	float fDistance = sqrt(pow(pt.x - x, 2.0f) + pow(pt.y - y, 2.0f));
	if (fDistance <= m_radius)
	{
		returnValue = true;
	}
	return returnValue;
}

/** \return The dragged distance of the point.
*/
float draggedBGPoint::getDragDistance()
{
	return sqrt(pow(m_startingPoint.x - x, 2.0f) + pow(m_startingPoint.y - y, 2.0f));
}

/** \param incremental Whether the drag motion should be computed based on the starting position (false) or on the last frame position (true).
* \return The amount of dragged motion.
*/
ofPoint draggedBGPoint::getDrag(bool incremental)
{
	ofPoint returnValue;
	if (!incremental)
	{
		// We return the deltaX and deltaY values
		returnValue = ofPoint(x - m_startingPoint.x, y - m_startingPoint.y);
	}
	else
	{
		returnValue = ofPoint(x - m_positionLastFrame.x, y - m_positionLastFrame.y);
	}
	return returnValue;
}

// ****************************************************************************************
void metaphorKitchen::setup()
{
	ofBackground(0,0,0);
	m_dragOffset.x = 0;
	m_dragOffset.y = 0;
	
	m_nBPM = 120;

	// Set up the four stove tops
	int nRadius = ofGetHeight()/3;
	ofPoint ptPotOrigin = ofPoint(ofGetWidth()/2, ofGetHeight()/3);
	m_stovetops[stoveInformation::FRONT_RIGHT] = ofxStovetop(stoveInformation::FRONT_RIGHT, ptPotOrigin, m_nBPM);
	ptPotOrigin = ofPoint(- ofGetWidth()/2, ofGetHeight()/3);
	m_stovetops[stoveInformation::FRONT_LEFT] = ofxStovetop(stoveInformation::FRONT_LEFT, ptPotOrigin, m_nBPM);
	ptPotOrigin = ofPoint(- ofGetWidth()/2, - ofGetHeight() * 2/3);
	m_stovetops[stoveInformation::REAR_LEFT] = ofxStovetop(stoveInformation::REAR_LEFT, ptPotOrigin, m_nBPM);
	ptPotOrigin = ofPoint(ofGetWidth()/2, - ofGetHeight() * 2/3);
	m_stovetops[stoveInformation::REAR_RIGHT] = ofxStovetop(stoveInformation::REAR_RIGHT, ptPotOrigin, m_nBPM);

	// Create menu structure and load loops
	ofColor coulourMenuNodes(0x5D, 0xB1, 0xDB);
	ofColor colourCountry(0xFF, 0x00, 0x00);
	ofColor colourJazz(0x00, 0xFF, 0x00);
	ofColor colourHipHop(0x00, 0x00, 0xFF);
	ofColor colourFunk(0xAB, 0x00, 0xAB);
	ofPoint ptCentre = ofPoint(ofGetWidth()/2, ofGetHeight());
	ofxTactoSHPMNode* root = new ofxTactoSHPMNode(ofColor(0x25, 0x85, 0xD9));
	// Add the children root
	// 1) Loops 1 - Drums
	ofxTactoSHPMNode* loops1 = new ofxTactoSHPMNode(coulourMenuNodes, TACTO_LOOPTYPE_DRUMS);
	ofxTactoBeatNode* drumLoop1 = new ofxTactoBeatNode(colourCountry, "sound/Country_Drums.aif", -1, TACTO_LOOPTYPE_DRUMS, 32);
	ofxTactoBeatNode* drumLoop2 = new ofxTactoBeatNode(colourFunk, "sound/Funk_Drums.aif", -1, TACTO_LOOPTYPE_DRUMS, 32);
	ofxTactoBeatNode* drumLoop3 = new ofxTactoBeatNode(colourHipHop, "sound/HipHop_Drums.aif", -1, TACTO_LOOPTYPE_DRUMS, 32);
	ofxTactoBeatNode* drumLoop4 = new ofxTactoBeatNode(colourJazz, "sound/Jazz_Drums.aif", -1, TACTO_LOOPTYPE_DRUMS, 32);
	loops1->addChild(drumLoop1);
	loops1->addChild(drumLoop2);
	loops1->addChild(drumLoop3);
	loops1->addChild(drumLoop4);	
	root->addChild(loops1);
	// 2) Loops 2 - Bass
	ofxTactoSHPMNode* loops2 = new ofxTactoSHPMNode(coulourMenuNodes, TACTO_LOOPTYPE_BASS);
	ofxTactoBeatNode* bassLoop1 = new ofxTactoBeatNode(colourCountry, "sound/Country_Bass.aif", -1, TACTO_LOOPTYPE_BASS, 32);
	ofxTactoBeatNode* bassLoop2 = new ofxTactoBeatNode(colourFunk, "sound/Funk_Bass.aif", -1, TACTO_LOOPTYPE_BASS, 32);
	ofxTactoBeatNode* bassLoop3 = new ofxTactoBeatNode(colourHipHop, "sound/HipHop_Bass.aif", -1, TACTO_LOOPTYPE_BASS, 32);
	ofxTactoBeatNode* bassLoop4 = new ofxTactoBeatNode(colourJazz, "sound/Jazz_Bass.aif", -1, TACTO_LOOPTYPE_BASS, 32);
	loops2->addChild(bassLoop1);
	loops2->addChild(bassLoop2);
	loops2->addChild(bassLoop3);
	loops2->addChild(bassLoop4);
	root->addChild(loops2);
	// 3) Loops 3 - Horns, Guitar, Banjo, Synth
	ofxTactoSHPMNode* loops3 = new ofxTactoSHPMNode(coulourMenuNodes, TACTO_LOOPTYPE_LEAD);
	ofxTactoBeatNode* spiceLoop1 = new ofxTactoBeatNode(colourFunk, "sound/Funk_Guitar.aif", -1, TACTO_LOOPTYPE_LEAD, 32);
	ofxTactoBeatNode* spiceLoop2 = new ofxTactoBeatNode(colourFunk, "sound/Funk_Saxophone.aif", -1, TACTO_LOOPTYPE_LEAD, 32);
	ofxTactoBeatNode* spiceLoop3 = new ofxTactoBeatNode(colourHipHop, "sound/HipHop_Synth.aif", -1, TACTO_LOOPTYPE_LEAD, 32);
	ofxTactoBeatNode* spiceLoop4 = new ofxTactoBeatNode(colourJazz, "sound/Jazz_Horn.aif", -1, TACTO_LOOPTYPE_LEAD, 32);
	ofxTactoBeatNode* spiceLoop5 = new ofxTactoBeatNode(colourCountry, "sound/Country_Banjo.aif", -1, TACTO_LOOPTYPE_LEAD, 32);
	loops3->addChild(spiceLoop1);
	loops3->addChild(spiceLoop2);
	loops3->addChild(spiceLoop3);
	loops3->addChild(spiceLoop4);
	loops3->addChild(spiceLoop5);
	root->addChild(loops3);
	m_shpmMenu.setup(root, ptCentre, 150);
	m_nTimeOfCreationMs = ofGetElapsedTimeMillis();
}

void metaphorKitchen::update()
{	
	// Figure out the current beat
	int nPeriodSingleBeatMs = (int)(1000 * 60 / m_nBPM);

	float fNumBeatsElapsed = ((float)ofGetElapsedTimeMillis() - (float)m_nTimeOfCreationMs) / (float)nPeriodSingleBeatMs;
	int nNumBeatsElapsed = (int)floor(fNumBeatsElapsed);
	int nNumBeatsSequencer = nNumBeatsElapsed % OFX_POT_NUMSEQUENCERSTEPS;
	
	for (int i=0; i<4; i++)
	{
		m_stovetops[i].update(nNumBeatsSequencer);
	}
}

void metaphorKitchen::draw()
{
	// Draw the stove position
	for (int i=0; i<4; i++)
	{
		m_stovetops[i].draw();
	}
	m_shpmMenu.draw(); // menu

    // Draw the points being dragged, as well as their parameters
    for (vector<draggedBGPoint>::iterator It = m_draggedBackgroundPoints.begin(); It != m_draggedBackgroundPoints.end(); It++)
    {
		It->draw();
    }
}

void metaphorKitchen::exit()
{
	for (int i=0; i<4; i++)
	{
		m_stovetops[i].exit();
	}
}

/** \param key The key pressed.
*/
void metaphorKitchen::keyPressed  (int key)
{
	if (key == 's')
	{
		// Take screenshot
		ofImage screenImg;  
		screenImg.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);  
		screenImg.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		screenImg.resize(400, 300);
		std::string strMonth = ofGetMonth() < 10 ? "0" + ofToString(ofGetMonth()) : ofToString(ofGetMonth());
		std::string strDay = ofGetDay() < 10 ? "0" + ofToString(ofGetDay()) : ofToString(ofGetDay());
		std::string strHours = ofGetHours() < 10 ? "0" + ofToString(ofGetHours()) : ofToString(ofGetHours());
		std::string strMins = ofGetMinutes() < 10 ? "0" + ofToString(ofGetMinutes()) : ofToString(ofGetMinutes());
		std::string strSecs = ofGetSeconds() < 10 ? "0" + ofToString(ofGetSeconds()) : ofToString(ofGetSeconds());
		std::string strSuffix = ofToString(ofGetYear()) + strMonth + strDay + "_" + strHours + "h" + strMins + "m" + strSecs + "s" ;
		
#ifdef _DEBUG
		strSuffix += "_debug";
#endif
		screenImg.saveImage("screenshot_" + strSuffix + ".png"); 
	}
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void metaphorKitchen::mouseDragged(int x, int y, int button)
{
	mouseTouchMoved(x, y, true, button);
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void metaphorKitchen::mousePressed(int x, int y, int button)
{
	mouseTouchDown(x, y, true, button);
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void metaphorKitchen::mouseReleased(int x, int y, int button)
{
	mouseTouchUp(x, y, true, button);
}

/**
* \param w The new width in pixels.
* \param h The new height in pixels.
*/
void metaphorKitchen::windowResized(int w, int h)
{
	m_shpmMenu.windowResized(w, h);
	ofPoint ptPotOrigin = ofPoint(ofGetWidth()/2, ofGetHeight()/3);
	m_stovetops[stoveInformation::FRONT_RIGHT].windowResized(w, h);
	m_stovetops[stoveInformation::FRONT_LEFT].windowResized(w, h);
	m_stovetops[stoveInformation::REAR_LEFT].windowResized(w, h);
	m_stovetops[stoveInformation::REAR_RIGHT].windowResized(w, h);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void metaphorKitchen::touchDown(float x, float y, int touchId)
{
	mouseTouchDown(x, y, false, 0, touchId);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void metaphorKitchen::touchMoved(float x, float y, int touchId)
{
	mouseTouchMoved(x, y, false, 0, touchId);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void metaphorKitchen::touchUp(float x, float y, int touchId)
{
	mouseTouchUp(x, y, false, 0, touchId);
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \param button The ID of the mouse button, in the case of mouse input.
* \param touchId The ID of the touch event, in the case of touch input.
*/
void metaphorKitchen::mouseTouchDown(float x, float y, bool fullRange, int button, int touchId)
{
    bool touchedMenuPot = false;
    touchedMenuPot |= m_shpmMenu.mouseTouchDown(x, y, fullRange, button, touchId); // grabs items within the menu
	for (int i=0; i<4; i++)
	{
		touchedMenuPot |= m_stovetops[i].mouseTouchDown(x, y, fullRange, button, touchId);
	}

	if (!touchedMenuPot)
	{
	    // Add to the list of contact points (touch or mouse) currently in motion
		ofPoint ptCompare(x, y);
		if (fullRange)
		{
			ptCompare.x /= ofGetWidth();
			ptCompare.y /= ofGetHeight();
		}

	    draggedBGPoint ptTmp = draggedBGPoint(ptCompare.x, ptCompare.y);
	    m_draggedBackgroundPoints.push_back(ptTmp);
	}
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \param button The ID of the mouse button, in the case of mouse input.
* \param touchId The ID of the touch event, in the case of touch input.
*/
void metaphorKitchen::mouseTouchMoved(float x, float y, bool fullRange, int button, int touchId)
{
    bool touchedMenuPot = false;
    touchedMenuPot |= m_shpmMenu.mouseTouchMoved(x, y, fullRange, button, touchId);
	for (int i=0; i<4; i++)
	{
		touchedMenuPot |= m_stovetops[i].mouseTouchMoved(x, y, fullRange, button, touchId);
	}

    ofPoint ptCompare(x, y);
    if (fullRange)
    {
        ptCompare.x /= ofGetWidth();
        ptCompare.y /= ofGetHeight();
    }

	// We didn't touch the menu or pot, let's move the background points
	if (!touchedMenuPot)
	{
	    // Move the points
	    vector<draggedBGPoint>::iterator It;
	    for (It = m_draggedBackgroundPoints.begin(); It != m_draggedBackgroundPoints.end(); It++)
	    {
			if (It->isPointInside(ptCompare))
	        {
				It->setPosition(ofPoint(ptCompare.x, ptCompare.y));
	        }
	    }
	}
	
	// Drag only with 1 finger
	if (m_draggedBackgroundPoints.size() == 1)
	{
		ofPoint ptDrag = m_draggedBackgroundPoints[0].getDrag(true);
		m_dragOffset += ptDrag;

		// Put limits on drag offset (remember we start on the front right stove)
		m_dragOffset.x = min(m_dragOffset.x, 1.0f);
		m_dragOffset.x = max(m_dragOffset.x, 0.0f);
		m_dragOffset.y = min(m_dragOffset.y, 1.0f);
		m_dragOffset.y = max(m_dragOffset.y, 0.0f);

		for (int i=0; i<4; i++)
		{		
			if (m_dragOffset.x != 0.0f || m_dragOffset.y != 0.0f)
			{
				m_stovetops[i].drag(ofPoint(m_dragOffset.x*ofGetWidth(), m_dragOffset.y*ofGetHeight()));
				float newVolume = m_stovetops[i].getCrossfadeVolume(ofPoint(m_dragOffset));
				m_stovetops[i].getPot()->setFocusVolume(newVolume);
			}
		}
	}
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \param button The ID of the mouse button, in the case of mouse input.
* \param touchId The ID of the touch event, in the case of touch input.
*/
void metaphorKitchen::mouseTouchUp(float x, float y, bool fullRange, int button, int touchId)
{
	// Call method on stovetops
	for (int i=0; i<4; i++)
	{
		m_stovetops[i].mouseTouchUp(x, y, fullRange, button, touchId);
	}

    ofPoint ptCompare(x, y);
    if (fullRange)
    {
        ptCompare.x /= ofGetWidth();
        ptCompare.y /= ofGetHeight();
    }

	vector<ofxTactoBeatNode*>::iterator It = m_shpmMenu.getDraggedNodes()->begin();
	while (It != m_shpmMenu.getDraggedNodes()->end())
	{
		ofxTactoBeatNode* currentBeatNode = *It;
		if (currentBeatNode->isPointInside(ptCompare))
		{
			// Add the node to the nodes that are being cooked
			// but only if it is dragged onto the pot
			for (int i=0; i<4; i++)
			{
				if (m_stovetops[i].isPointInPot(x, y, fullRange))
				{
					// Drop the current node here
					// create the node here
					ofxCookableNode cookNode(currentBeatNode->getColor(), 
						currentBeatNode->getRadius(), currentBeatNode->getLifeTime(), 
						currentBeatNode->getFullFilePath(), true, currentBeatNode->getLoopType(),
						currentBeatNode->getLifeTime());
					cookNode.setOrigin(currentBeatNode->getOrigin(), true);
					ofPoint fullRangeOffset = ofPoint(m_dragOffset.x * ofGetWidth(), m_dragOffset.y * ofGetHeight());
					cookNode.setOriginInit(currentBeatNode->getOrigin() - fullRangeOffset, true);
					cookNode.setLoopLength(currentBeatNode->getLoopLength());
					ofxPot* currPot = m_stovetops[i].getPot();
					
					// Set volume
					float fDistFromCenter = sqrtf(pow(currPot->getOrigin().x - ptCompare.x*ofGetWidth() , 2.0f) + pow(currPot->getOrigin().y - ptCompare.y*ofGetHeight(), 2.0f));
					float fVolume = 1.0f - fDistFromCenter / currPot->getRadius();
					cookNode.setRelativeVolume(fVolume*1.5f);
					
					// Set lowpass
					float yStart = currPot->getOrigin().y + currPot->getRadius(); // lowest point in the pot
					float tmp1 = ptCompare.y*ofGetHeight();
					float fRelHeight = (yStart - tmp1)/(2*currPot->getRadius());
					cookNode.setLowpass(fRelHeight);

					// Do not set loop length until the node is redropped on the pot.

					m_stovetops[i].addCookElement(cookNode);
				}
			}
			// We are releasing an item that was was dragged
			It = m_shpmMenu.getDraggedNodes()->erase(It);
		}
		else
		{
			++It;
		}
	}

	vector<draggedBGPoint>::iterator It2 = m_draggedBackgroundPoints.begin();
	while (It2 != m_draggedBackgroundPoints.end())
	{
		if (It2->isPointInside(ptCompare))
        {
            It2 = m_draggedBackgroundPoints.erase(It2);
        }
        else
        {
            ++It2;
        }
	}
}
