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
	// Metaphor environments
	// ********************************************************************************
	m_metaKitchen.setup();
	
	// ********************************************************************************
	// Multi-touch support
	// ********************************************************************************
	m_touchEngine.setup();
	ofAddListener(ofEvents().touchDown, this, &testApp::touchDown);
	ofAddListener(ofEvents().touchUp, this, &testApp::touchUp);
	
	// ********************************************************************************
	// Particle system
	// ********************************************************************************
	m_dragDelta = ofPoint(0.0f, 0.0f);
	// 1 - ofxParticleEmitter particles
	if ( !m_emitter.loadFromXml( "snowflakes.pex" ) )
	{
		ofLog( OF_LOG_ERROR, "testApp::setup() - failed to load emitter config" );
	}
	
	// 2 - particles from OF math example
	int numParticles = 500;
	p.assign(numParticles, demoParticle());
	currentMode = PARTICLE_MODE_OFXPARTICLEEMITTER;
	currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
	resetParticles();
}


//--------------------------------------------------------------
// initialize OF math example particles
void testApp::resetParticles()
{
	//these are the attraction points used in the forth demo
	attractPoints.clear();
	for(int i = 0; i < 4; i++){
		attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) ) );
	}
	
	attractPointsWithMovement = attractPoints;
	
	for(int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);		
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
	}	
}

void testApp::update()
{
	// ************************************************************
	// Blob handling
	// ************************************************************
	switch(m_nState) {
	case STATE_KITCHEN:
		m_metaKitchen.update();
		break;
	}
	
	// ************************************************************
	// Multi-touch
	// ************************************************************
	m_touchEngine.update();
	
	// ************************************************************
	// Particle system
	// ************************************************************
	// 1 - ofxParticleEmitter particles
	m_emitter.update();
	
	// 2 - particles from OF math example
	for(int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);
		p[i].update();
	}	
	// add a bit of movement to the attract points
	for(int i = 0; i < attractPointsWithMovement.size(); i++){
		attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
		attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
	}
}

void testApp::draw()
{
	switch(m_nState)
	{
	case STATE_INTRO:
		// ************************************************************
		// Draw IDMIL logo
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

	// Draw the particle system
	if(currentMode == PARTICLE_MODE_OFXPARTICLEEMITTER){
		m_emitter.draw( 0, 0 );
	} else {
		for(int i = 0; i < p.size(); i++){
			p[i].draw();
		}
		
		ofSetColor(190);
		if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
			for(int i = 0; i < attractPoints.size(); i++){
				ofNoFill();
				ofCircle(attractPointsWithMovement[i], 10);
				ofFill();
				ofCircle(attractPointsWithMovement[i], 4);
			}
		}
	}
	
	// Display drag vector
	ofSetHexColor(0xFF0000);
	ofLine(100, 100, 100 + m_dragDelta.x, 100 + m_dragDelta.y);
	
	// Display text
	ofSetColor(230);
	ofPoint zeroAngle(1.0f, 0.0f);
	float angle = zeroAngle.angle(m_dragDelta);
	currentModeStr = "Current drag: " + ofToString(m_dragDelta.x) + " " + ofToString(m_dragDelta.y) + " / angle: " + ofToString(angle);
	ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-5 to change mode.", 10, 20);
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
	
	if( key == '1'){
		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse"; 		
	}
	if( key == '2'){
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse"; 				
	}
	if( key == '3'){
		currentMode = PARTICLE_MODE_NEAREST_POINTS;
		currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force"; 						
	}
	if( key == '4'){
		currentMode = PARTICLE_MODE_NOISE;
		currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation"; 						
		resetParticles();
	}
	if( key == '5'){
		currentMode = PARTICLE_MODE_EMITTER;
		currentModeStr = "5 - PARTICLE_MODE_EMITTER";
		resetParticles();
	}
	if( key == '9'){
		currentMode = PARTICLE_MODE_OFXPARTICLEEMITTER;
		currentModeStr = "9 - Using ofxParticleEmitter add-on";
	}
	if( key == 'x'){
		m_emitter.stopEmitting();
	}
	if( key == 'z'){
		m_emitter.startEmitting();
	}
		
	if( key == ' ' ){
		resetParticles();
	}
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
void testApp::mouseMoved(int x, int y)
{
	mouseTouchMoved(x,  y, true, -1);
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
	m_emitter.startEmitting();
	m_metaKitchen.mouseTouchDown(x, y, fullRange, button, touchId);
	switch(m_nState) {
	case STATE_INTRO:
	case STATE_ENVIRONMENTS:
		//m_emit
	case STATE_KITCHEN:
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
	// compute drag delta
	m_dragDelta = ofPoint(x, y) - m_oldCursorPosition;
	m_oldCursorPosition = ofPoint(x, y);
	// Compute angle
	ofPoint zeroAngle(1.0f, 0.0f);
	float angle = zeroAngle.angle(m_dragDelta);
	
	switch(m_nState) {
		case STATE_INTRO:
		case STATE_ENVIRONMENTS:
		case STATE_KITCHEN:
			m_metaKitchen.mouseTouchMoved(x, y, fullRange, button, touchId);
			m_emitter.duration = -1.0f;
			m_emitter.radialAcceleration = 32.0f;
			m_emitter.radialAccelVariance = 10.0f;
			m_emitter.startColor.red = 1.0f;
			m_emitter.startColor.green = 1.0f;
			m_emitter.startColor.blue = 0.0f;
			m_emitter.startColor.alpha = 0.7f;
			m_emitter.startParticleSize = 24.0f;
			m_emitter.finishColor.red = 0.4f;
			m_emitter.finishColor.green = 0.4f;
			m_emitter.finishColor.blue = 0.0f;
			m_emitter.finishColor.alpha = 0.0f;
			m_emitter.finishParticleSize = 10.0f;
			m_emitter.maxParticles = 20;
			m_emitter.speed = 60.0f;
			m_emitter.speedVariance = 40.0f;
			m_emitter.angle = angle;
			m_emitter.angleVariance = 0.0f; //360.0f;
			
			if(currentMode == PARTICLE_MODE_OFXPARTICLEEMITTER){
				m_emitter.sourcePosition.x = x;
				m_emitter.sourcePosition.y = y;
			}
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
	
	m_emitter.stopEmitting();
}