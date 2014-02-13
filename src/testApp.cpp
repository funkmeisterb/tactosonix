#include "testApp.h"
#include "assert.h"
#include "ofLog.h"

void testApp::setup()
{
#ifdef _DEBUG
	ofSetLogLevel(OF_LOG_NOTICE);
#endif

	ofBackground(0,0,0);
	ofSetFrameRate(60);
	ofSetCircleResolution(40);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofDisableArbTex(); // backward compatibility with old graphics cards

	m_profileInfo.init("fonts/arial.ttf", 20);
	m_profileInfo.setText("");
	m_profileInfo.wrapTextX(ofGetWidth() * 3/4);

	// ********************************************************************************
	// GUI
	// ********************************************************************************
	m_IDMILlogo.loadImage("images/idmil_logo.png");
	ofImage logo = ofImage("images/Kitchen.png");

	// ********************************************************************************
	// METAPHOR ENVIRONMENTS
	// ********************************************************************************
	m_metaKitchen.setup();
	
	// ********************************************************************************
	// MULTI-TOUCH SUPPORT
	// ********************************************************************************
	m_touchEngine.setup();
	ofAddListener(ofEvents().touchDown, this, &testApp::touchDown);
	ofAddListener(ofEvents().touchUp, this, &testApp::touchUp);
}

void testApp::update()
{
	// ************************************************************
	// BLOB HANDLING
	// ************************************************************
	switch(m_nState) {
	case STATE_KITCHEN:
		m_metaKitchen.update();
		break;
	}
	
	// ************************************************************
	// MULTI-TOUCH
	// ************************************************************
	m_touchEngine.update();
}

void testApp::draw()
{
	switch(m_nState)
	{
	case STATE_INTRO:
		// ************************************************************
		// DRAW IDMIL LOGO
		// ************************************************************
		ofSetHexColor(0xFFFFFF);
		m_IDMILlogo.draw(ofGetWidth()/2 - m_IDMILlogo.getWidth()/2,
			ofGetHeight()/2 - m_IDMILlogo.getHeight()/2);
		break;

	case STATE_ENVIRONMENTS:
		// not implemented
	case STATE_KITCHEN:
		m_metaKitchen.draw();
		break;
	case STATE_CROUPIER:
		break;
	default:
		break;
	}

	// draw the frame rate
#ifdef _DEBUG
	ofSetHexColor(0xFF0000); // red
	m_profileInfo.setText(ofToString((int)ofGetFrameRate()));
	m_profileInfo.drawCenter(0.1f*ofGetWidth(), 0.9*ofGetHeight());
#endif
	
	m_touchEngine.drawBlobs();
}

void testApp::exit()
{
	m_metaKitchen.exit();
}

/** \param key The key pressed.
*/
void testApp::keyPressed  (int key)
{
	m_metaKitchen.keyPressed(key);
}

/** \param key The key released.
*/
void testApp::keyReleased  (int key)
{
	if (key == 'r') {
		// reset
		m_metaKitchen.exit();
		m_metaKitchen.setup();
		m_nState = STATE_INTRO;
	}
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void testApp::mouseDragged(int x, int y, int button)
{
	mouseTouchMoved(x,  y, true, button);
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void testApp::mousePressed(int x, int y, int button)
{
	mouseTouchDown(x,  y, true, button);
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void testApp::mouseReleased(int x, int y, int button)
{
	mouseTouchUp(x, y, true, button);
}

/**
* \param w The new width in pixels.
* \param h The new height in pixels.
*/
void testApp::windowResized(int w, int h)
{
	m_metaKitchen.windowResized(w, h);
}

/**
* \param args The touch event.
*/
void testApp::touchDown(ofTouchEventArgs& args)
{
	mouseTouchDown(args.x, args.y, false, 0, args.id);
}

/**
* \param args The touch event.
*/
void testApp::touchMoved(ofTouchEventArgs& args)
{
	mouseTouchMoved(args.x, args.y, false, 0, args.id);
}

/**
* \param args The touch event.
*/
void testApp::touchUp(ofTouchEventArgs& args)
{
	mouseTouchUp(args.x, args.y, false, 0, args.id);
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \param button The ID of the mouse button, in the case of mouse input.
* \param touchId The ID of the touch event, in the case of touch input.
*/
void testApp::mouseTouchDown(float x, float y, bool fullRange, int button, int touchId)
{
	switch(m_nState) {
	case STATE_INTRO:
		break;
	case STATE_ENVIRONMENTS:
		break;
	case STATE_KITCHEN:
		m_metaKitchen.mouseTouchDown(x, y, fullRange, button, touchId);
		break;
	default:
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
void testApp::mouseTouchMoved(float x, float y, bool fullRange, int button, int touchId)
{
	switch(m_nState) {
	case STATE_INTRO:
		break;
	case STATE_ENVIRONMENTS:
		break;
	case STATE_KITCHEN:
		m_metaKitchen.mouseTouchMoved(x, y, fullRange, button, touchId);
		break;
	default:
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
void testApp::mouseTouchUp(float x, float y, bool fullRange, int button, int touchId)
{
	switch(m_nState)
	{
	case STATE_INTRO:
		// Bypass the selection of environmens
		m_nState = STATE_KITCHEN;
		break;
	case STATE_ENVIRONMENTS:
		break;
	case STATE_KITCHEN:
		m_metaKitchen.mouseTouchUp(x, y, fullRange, button, touchId);
		break;
	default:
		break;
	}
}