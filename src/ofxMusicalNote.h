#pragma once

/**
 * \class ofxMusicalNote
 *
 * \brief This is a class that represents musical notes, to be used 
 * in a sequencer.
 *
 * \author Bruno Angeles (bruno.angeles@mail.mcgill.ca)
 *
 * \version 0.1
 *
 * \date 2011/05/26 14:16:20
 *
 */

// These are MIDI values converted to Hz.
// See https://ccrma.stanford.edu/software/stk/classstk_1_1Drummer.html for details.
#define STK_DRUM_SNARE 73.4
#define STK_DRUM_BASS 65.4
#define STK_DRUM_TOM_LOW 87.3
#define STK_DRUM_TOM_MID 110
#define STK_DRUM_TOM_HIGH 130.8
#define STK_DRUM_HIGH_HAT 92.5
#define STK_DRUM_RIDE 116.5
#define STK_DRUM_CRASH 138.6
#define STK_DRUM_COWBELL 207.7
#define STK_DRUM_TAMBOURINE 185

class ofxMusicalNote
{
public:
	ofxMusicalNote(float _fPitch = 0, float _fVelocity = 1.0f, int _nDuration = 1);
	~ofxMusicalNote(void);
	float getPitch();
	void setPitch(float _fPitch);
	float getVelocity();
	void setVelocity(float _fVelocity);
	int getDuration();
	void setDuration(int _nDuration);

private:
	float				m_fPitch; ///< The pitch (in Hz) of the note
	float				m_fVelocity; ///< The velocity [0;1] of the note
	int					m_nDuration;  ///< The length of the note in the sequencer (1=1/16, 2=1/8, etc.)
};

