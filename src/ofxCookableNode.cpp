#include "ofxCookableNode.h"
#include "TactosonixHelpers.h"

ofxCookableNode::ofxCookableNode()
{
	m_nTimeCreatedMs = ofGetElapsedTimeMillis();
}

/** \param _color The colour of the node.
* \param _nRadius The radius in pixels of the node.
* \param _nLifeTimeMs The lifetime in milliseconds of the node.
* \param _sFileName The file name of the audio loop.
* \param _bLoop Whether or not the node should loop after it has played.
* \param _soundType The type of loop.
* \param _nBeatLength The length in beats of the node.
*/
ofxCookableNode::ofxCookableNode(ofColor _color, int _nRadius, int _nLifeTimeMs, string _sFileName,
	bool _bLoop, TACTO_LOOPTYPE _soundType, int _nBeatLength) :
	ofxTactoSHPMNode(_color, _soundType)
{
	m_nLifeTimeMs = _nLifeTimeMs;
	m_nTimeCreatedMs = ofGetElapsedTimeMillis();
	setRadius(_nRadius);
	setMultiPlay(false);
	m_bLoopable = _bLoop;
	m_nSoundType = _soundType;
	m_nLoopLength = _nBeatLength;
	m_fLowpass = 0;
	m_nLoopBeats = 0;
	m_nCurrLoopBeat = 0;
	m_nLoopStartBeat = 0;
	m_fLoopStartPosition = 0.0f;

	m_sFileName = _sFileName;
	if (m_sFileName != "")
	{
		loadSound(m_sFileName);
	}
}

/** \param _fPotVolume The volume of the pot in which this node is cooking.
*/
void ofxCookableNode::update(float _fPotVolume)
{
	ofSoundUpdate();
	setVolume(m_fRelativeVolume * _fPotVolume);
}

void ofxCookableNode::exit()
{
	unloadSound();
}

/** \return The age in milliseconds of the node.
*/
int ofxCookableNode::getAge()
{
	if (m_nLifeTimeMs == -1)
	{
		return -1;
	}
	else
	{
		return ofGetElapsedTimeMillis() - m_nTimeCreatedMs;
	}
}

/** \return True if and only if the node is done playing.
*/
bool ofxCookableNode::isDead()
{
	if (m_nLifeTimeMs == -1)
	{
		return false;
	}
	else
	{
		return ofGetElapsedTimeMillis() - m_nTimeCreatedMs > m_nLifeTimeMs;
	}
}

/** \return The standardized (in the [0;1] range) age of the node.
*/
float ofxCookableNode::getStandardizedAge()
{
	return (float)(ofGetElapsedTimeMillis() - m_nTimeCreatedMs) / m_nLifeTimeMs;
}

void ofxCookableNode::play()
{
	if (m_sFileName == "")
	{
		return;
	}
	
	ofSoundPlayer::play();
}

/** \param deltaDrag The amount of drag of the motion.
*/
void ofxCookableNode::drag(ofPoint deltaDrag)
{
	setOrigin(getOriginInit() + deltaDrag, true);
}

/** \note This function requires a modified version of ofSoundPlayer.
* \param pct The standardized (in the [0-1] range) cutoff frequency of the lowpass filter.
*/
void ofxCookableNode::setLowpass(float pct)
{
	m_fLowpass = pct;
	// We map [0-1] to a more purposeful frequency range
	float mappedValue = 0.33f * pct; // y = 0.33*x
	ofSoundPlayer::setLowpass(mappedValue);
}

void ofxCookableNode::drawLowpass()
{
	// Draw visual feedback indicating lowpass value.
	int nTips = 8;
	int nStarPts = nTips * 2;
	float angleChangePerPt = TWO_PI / (float)nStarPts;
	float outerRadius = 0.6f*getRadius();
	float innerRadius = outerRadius * (1.0f - m_fLowpass); // lower cutoff -> larger inner radius
	float origx = getOrigin().x;
	float origy = getOrigin().y;
	float angle = 0;
	
	ofSetColor(getColor(), 255);
	ofSetLineWidth(4.0f);
	ofFill();
	ofBeginShape();
	for (int i = 0; i < nStarPts; i++)
	{
		if (i % 2 == 0) {
			// inside point:
			float x = origx + innerRadius * cos(angle);
			float y = origy + innerRadius * sin(angle);
			ofVertex(x,y);
		} else {
			// outside point
			float x = origx + outerRadius * cos(angle);
			float y = origy + outerRadius * sin(angle);
			ofVertex(x,y);
		}
		angle += angleChangePerPt;
	}
	ofEndShape();
}

void ofxCookableNode::incrementCurrLoopBeat()
{
	if (m_nLoopLength <= 0)
	{
		return;
	}
	m_nCurrLoopBeat = (m_nCurrLoopBeat++ % m_nLoopLength);
	ofLog(OF_LOG_NOTICE, "Loop beat is " + ofToString(m_nCurrLoopBeat));
	
	if (m_nLoopBeats > 0)
	{
		// Perform looping
		if (m_nCurrLoopBeat == (m_nLoopStartBeat + m_nLoopBeats) % m_nLoopLength)
		{
			// Start playback at the start position
			setPosition(m_fLoopStartPosition);
			m_nCurrLoopBeat = m_nLoopStartBeat;
			ofLog(OF_LOG_NOTICE, "Looping back over " + ofToString(m_nLoopBeats) + " at this beat #" + ofToString(m_nCurrLoopBeat));
		}
	}
}

/** \param loops The number of beats over which to loop, in the implementation of a stutter-like feature.
*/
void ofxCookableNode::setLoopBeats(unsigned int loops)
{
	if (m_nLoopBeats != loops)
	{
		m_nLoopBeats = loops;
		m_nLoopStartBeat = m_nCurrLoopBeat; // remember this one and loop
		m_fLoopStartPosition = getPosition();
	}
}