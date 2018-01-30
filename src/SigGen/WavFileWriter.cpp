// WavFileWriter.cpp: implementation of the WavFileWriter class.
//
//////////////////////////////////////////////////////////////////////

#include "WavFileWriter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <memory.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WavFileWriter::WavFileWriter(  string path,
                               size_t channels,
                               size_t srate,
                               size_t quality )
:m_path( path )
,m_outFile(0)
{

	m_channelCount = channels;
	m_sampleRate = srate;
	m_quality = quality;
	memset (&m_sfinfo, 0, sizeof (m_sfinfo)) ;

	m_sfinfo.samplerate	= m_sampleRate ;
	m_sfinfo.channels	= m_channelCount ;
	m_sfinfo.format		= (SF_FORMAT_WAV | SF_FORMAT_FLOAT) ;

	if( m_outFile = sf_open ( path.c_str(), SFM_WRITE, &m_sfinfo) )
		m_isStreamOk = true;
	else
		m_isStreamOk = false;
}

WavFileWriter::~WavFileWriter()
{
	sf_close( m_outFile );
}

void WavFileWriter::writeAudioFrames( float* buffer, size_t frames  )
{
	sf_write_float( m_outFile, buffer, frames * m_channelCount );
}
