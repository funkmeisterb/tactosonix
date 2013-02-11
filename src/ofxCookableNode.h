#ifndef _OF_COOKABLENODE
#define _OF_COOKABLENODE

/**
 * \class ofxCookableNode
 *
 * \brief This is an implementation of a touch-enabled musical node within a Stacked Half-Pie Menu (\link ofxTactoSHPM), for the purpose of being cooked in the cooking metaphor of TactoSonix.
 *
 * \author Bruno Angeles (bruno.angeles@mail.mcgill.ca)
 *
 * \version 1.0
 *
 * \date 2012/08/14
 *
 */

#include "UI/ofxTactoSHPMNode.h"
#include "UI/ofxTactoBeatNode.h"

/// A class that implements a musical node that can be cooked in a pot.
class ofxCookableNode : public ofxTactoSHPMNode, public ofSoundPlayer
{
public:
	ofxCookableNode(); ///< Default constructor
	ofxCookableNode(ofColor _color, int _nRadius, int _nLifeTimeMs, string _sFileName, bool _bLoop, TACTO_LOOPTYPE _soundType, int _nBeatLength); ///< Constructor
	void							update(float _fPotVolume); ///< Regular OpenFrameworks function.
	void							exit(); ///< Regular OpenFrameworks function.
	void							play(); ///< Override of the ofSoundPlayer function.

	bool							isDead(); ///< Returns true if and only if the node's lifetime is over.
	int								getAge(); ///< Returns the age in milliseconds of the node.
	float							getStandardizedAge(); ///< Returns the standardized age in the [0;1] range of the node.
	void							setLifeTimeMs(int _nLifeTimeMs) { m_nLifeTimeMs = _nLifeTimeMs; } ///< Sets the lifetime in milliseconds of the node. \param _nLifeTimeMs The lifetime in milliseconds of the node.
	void							setLoopLength(int _nLoopLength) { m_nLoopLength = _nLoopLength; } ///< Sets the length in beats of the node. \param _nLoopLength The length in beats of the node.
	int								getSoundType() { return m_nSoundType; } ///< Returns the type of loop. \return The type of loop.
	bool							getLoopable() { return m_bLoopable; } ///< Returns true if and only if the node can be looped.
	int								getLoopLength() { return m_nLoopLength; } ///< Returns the length in beats of the loop.
	void							drag(ofPoint deltaDrag); ///< Drags the node by the specified offset.
	std::string						getFileName() { return m_sFileName; } ///< Returns the name of the file played by the node.
	void							setRelativeVolume(float _fVolume) { m_fRelativeVolume = _fVolume; } ///< Sets the volume of the cookable node within the pot. \param _fVolume The relative volume of the node within the pot.
	float							getRelativeVolume() { return m_fRelativeVolume; } ///< Returns the volume of the cookable node within the pot. \return The relative volume of the node within the pot.
	void							setLowpass(float pct); ///< Override of a new ofSoundPlayer function, to map to different frequency range.
	float							getLowpass() { return m_fLowpass; } ///< Returns the standardized ([0;1] range) cutoff frequency of the lowpass filter. \return The standardized ([0;1] range) cutoff frequency of the lowpass filter.
	void							drawLowpass(); ///< Draws the visual feedback for the lowpass filter.
	void							setLoopBeats(unsigned int loops); ///< Sets the number of beats of the loop.
	unsigned int					getLoopBeats() { return m_nLoopBeats; } ///< Returns the number of beats of the loop.
	void							setCurrLoopBeat(unsigned int loopLength) { m_nCurrLoopBeat = loopLength;} ///< Sets the current beat if the node is being looped via a stutter-like feature.
	unsigned int					getCurrLoopBeat() { return m_nCurrLoopBeat; } ///< Returns the current beat if the node is being looped via a stutter-like feature.
	void							incrementCurrLoopBeat(); ///< Increments the current beat of the looped the node via a stutter-like feature.

private:
	int								m_nTimeCreatedMs; ///< The time in milliseconds of the creation of the node.
	int								m_nLifeTimeMs; ///< The lifetime in milliseconds of the node.
	string							m_sFileName; ///< The name of the file being played back.
	int								m_nSoundType; ///< The type of sound.
	bool							m_bLoopable; ///< Whether or not the node is loopable after it has played.
	int								m_nLoopLength; ///< The length in beats of the node's loop.
	float							m_fRelativeVolume; ///< The relative volume of the node.
	float							m_fLowpass; ///< The standardized ([0;1] range) cutoff frequency of the lowpass filter.
	
	// Useful to handle loops
	unsigned int					m_nLoopBeats; ///< The loop length when looping this beat via a stutter-like feature.
	unsigned int					m_nCurrLoopBeat; ///< The current beat when looping this beat via a stutter-like feature.
	unsigned int					m_nLoopStartBeat; ///< The starting beat when looping this beat via a stutter-like feature.
	float							m_fLoopStartPosition; ///< The starting beat's time in milliseconds when looping via a stutter-like feature.
};

#endif
