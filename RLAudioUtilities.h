#pragma once
/* Wrapper file for static methods to assist usage
of RustyAL */

//OpenAL includes
#include "AL\al.h"

namespace RustyLib
{
	namespace Audio
	{
		enum error {
			NO_ERRORS,
			INVALID_BUFFER,
			INVALID_NAME,
			INVALID_ENUM,
			INVALID_VALUE,
			INVALID_OPERATION,
			OUT_OF_MEMORY,
			UNKOWN_ERROR
		};

		static error CheckALError(void )
		{
			ALenum error;

			//If there is an error 
			if( (error = alGetError()) != AL_NO_ERROR)
			{		
				std::string errStr;
				switch(error)
				{
				case AL_INVALID_NAME:
					errStr = "AL_INVALID_NAME";
					return INVALID_NAME;
					break;
				case AL_INVALID_ENUM:
					errStr = "AL_INVALID_ENUM";
					return INVALID_ENUM;
					break;
				case AL_INVALID_VALUE:
					errStr = "AL_INVALID_VALUE";
					return INVALID_VALUE;
					break;
				case AL_INVALID_OPERATION:
					errStr = "AL_INVALID_OPERATION";
					return INVALID_OPERATION;
					break;
				case AL_OUT_OF_MEMORY:
					errStr = "AL_OUT_OF_MEMORY";
					return OUT_OF_MEMORY;
					break;
				default:
					return UNKOWN_ERROR;
					break;
				}
				std::cout << "OpenAL Error = " << errStr << std::endl;
			}
			else
			{
				return NO_ERRORS;
			}
		}
	}
}