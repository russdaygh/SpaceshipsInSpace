#include "StdAfx.h"

//Local includes
#include "RLAudioData.h"
#include "RlAudioUtilities.h"

//OpenAL includes
//#include <al\al.h>
#include "AL\al.h"
//#include <al\alut.h>
#include "AL\alut.h"

namespace RustyLib
{
	namespace Audio
	{
		//Constant definitions
		const std::string RLAudioData::PATH_PREFIX = "assets\\audio\\";

		RLAudioData::RLAudioData(const std::string& pathStr):
			path(PATH_PREFIX),
			name(pathStr)
		{
			//Add the filename to the PATH_PREFIX
			//to give it the proper file path
			path.append(name);
			//Get the string length
			size_t strSz = name.size();
			//Cut off the file extension (assuming .wav)
			name.resize(strSz - 4);
		}


		RLAudioData::~RLAudioData(void)
		{
			alDeleteBuffers(1, &alBufferID);
		}


		void RLAudioData::BufferData(void )
		{
			//Vars for buffering data
			ALenum format;
			ALsizei size;
			ALvoid* data;
			ALsizei freq;
			ALboolean loop;

			//Generate a buffer in OpenAL
			//and buffer the file into it
			alGenBuffers(1, &alBufferID);
			CheckALError();
	
			//Load into memory
			alutLoadWAVFile(
				(ALbyte*)path.c_str(),
				&format,
				&data,
				&size,
				&freq,
				&loop);

			//Register with allocated buffer
			alBufferData(
				alBufferID,
				format,
				data,
				size,
				freq);
			CheckALError();

			//Unload format information from our memory
			alutUnloadWAV(
				format,
				data,
				size,
				freq);

			/*********CALCULATE DURATION IN SECONDS*******/
			int bits;
			int channels;

			alGetBufferi(alBufferID,AL_BITS,&bits);
			alGetBufferi(alBufferID,AL_CHANNELS,&channels);
			float sampleSize = (bits / 8) * channels;
			float samples = size / sampleSize;

			duration = samples / freq;
		}


		const unsigned int RLAudioData::GetBufferID(void )const
		{
			return alBufferID;
		}


		const std::string& RLAudioData::Name(void )const
		{
			return name;
		}


		const float RLAudioData::GetBufferDuration(void )const
		{
			return duration;
		}
	}
}