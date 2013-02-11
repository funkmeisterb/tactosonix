#include "ofxMusicalNote.h"

/** \brief The constructor method.
* \param _fPitch The pitch of the note
* \param _fVelocity The velocity of the note
* \param _nDuration The duration of the note
*/
ofxMusicalNote::ofxMusicalNote(float _fPitch, float _fVelocity, int _nDuration)
	: m_fPitch (_fPitch), m_fVelocity (_fVelocity), m_nDuration (_nDuration)
{
}

/** \brief The destructor method.
*/
ofxMusicalNote::~ofxMusicalNote(void)
{
}

/** \brief The getter method for the note's pitch.
* \return The value of the note's pitch
*/
float ofxMusicalNote::getPitch()
{
	return m_fPitch;
}

/** \brief The setter method for the note's pitch.
* \param _fPitch The value of the note's pitch
*/
void ofxMusicalNote::setPitch(float _fPitch)
{
	m_fPitch = _fPitch;
}

/** \brief The getter method for the note's velocity.
* \return The value of the note's velocity
*/
float ofxMusicalNote::getVelocity()
{
	return m_fVelocity;
}

/** \brief The setter method for the note's velocity.
* \param _fVelocity The value of the note's velocity
*/
void ofxMusicalNote::setVelocity(float _fVelocity)
{
	m_fVelocity = _fVelocity;
}

/** \brief The getter method for the note's duration.
* \return The value of the note's duration
*/
int ofxMusicalNote::getDuration()
{
	return m_nDuration;
}

/** \brief The setter method for the note's duration.
* \param _nDuration The value of the note's duration
*/
void ofxMusicalNote::setDuration(int _nDuration)
{
	m_nDuration = _nDuration;
}