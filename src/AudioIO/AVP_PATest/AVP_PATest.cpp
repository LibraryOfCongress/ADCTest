// AVP_PATest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "portaudio.h"

#include "pablio.h"
#include "pa_asio.h"

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define PA_USE_ASIO 1

#define NUM_SECONDS         (4)
#define SAMPLE_RATE         (48000)
#define TABLE_SIZE   (200)
#define FREQUENCY           (125.0f)
#define PHASE_INCREMENT     (2.0f * FREQUENCY / SAMPLE_RATE)
#define FRAMES_PER_BLOCK    (1024)
#define SAMPLES_PER_FRAME   (2)

float   outSamples[FRAMES_PER_BLOCK*SAMPLES_PER_FRAME];
float   inSamples[FRAMES_PER_BLOCK*SAMPLES_PER_FRAME];
float   phases[SAMPLES_PER_FRAME];
float   sine[TABLE_SIZE]; 

static void PrintSupportedStandardSampleRates(
	const PaStreamParameters *inputParameters,
	const PaStreamParameters *outputParameters)
{
	static double standardSampleRates[] = {
		8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
		44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
	};
	int     i, printCount;
	PaError err;

	printCount = 0;
	for (i = 0; standardSampleRates[i] > 0; i++)
	{
		err = Pa_IsFormatSupported(inputParameters, outputParameters, standardSampleRates[i]);
		if (err == paFormatIsSupported)
		{
			if (printCount == 0)
			{
				printf("\t%8.2f", standardSampleRates[i]);
				printCount = 1;
			}
			else if (printCount == 4)
			{
				printf(",\n\t%8.2f", standardSampleRates[i]);
				printCount = 1;
			}
			else
			{
				printf(", %8.2f", standardSampleRates[i]);
				++printCount;
			}
		}
	}
	if (!printCount)
		printf("None\n");
	else
		printf("\n");
}

////////////////////////////////////////////////////////////////////////
int main_padevs(void)
{
	int     i, numDevices, defaultDisplayed;
	const   PaDeviceInfo *deviceInfo;
	PaStreamParameters inputParameters, outputParameters;
	PaError err;


	err = Pa_Initialize();
	if (err != paNoError)
	{
		printf("ERROR: Pa_Initialize returned 0x%x\n", err);
		goto error;
	}

	printf("PortAudio version: 0x%08X\n", Pa_GetVersion());
	printf("Version text: '%s'\n", Pa_GetVersionInfo()->versionText);

	numDevices = Pa_GetDeviceCount();
	if (numDevices < 0)
	{
		printf("ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices);
		err = numDevices;
		goto error;
	}

	printf("Number of devices = %d\n", numDevices);
	for (i = 0; i<numDevices; i++)
	{
		deviceInfo = Pa_GetDeviceInfo(i);
		printf("--------------------------------------- device #%d\n", i);

		/* Mark global and API specific default devices */
		defaultDisplayed = 0;
		if (i == Pa_GetDefaultInputDevice())
		{
			printf("[ Default Input");
			defaultDisplayed = 1;
		}
		else if (i == Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultInputDevice)
		{
			const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
			printf("[ Default %s Input", hostInfo->name);
			defaultDisplayed = 1;
		}

		if (i == Pa_GetDefaultOutputDevice())
		{
			printf((defaultDisplayed ? "," : "["));
			printf(" Default Output");
			defaultDisplayed = 1;
		}
		else if (i == Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultOutputDevice)
		{
			const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
			printf((defaultDisplayed ? "," : "["));
			printf(" Default %s Output", hostInfo->name);
			defaultDisplayed = 1;
		}

		if (defaultDisplayed)
			printf(" ]\n");

		/* print device info fields */
#ifdef WIN32
		{   /* Use wide char on windows, so we can show UTF-8 encoded device names */
			wchar_t wideName[MAX_PATH];
			MultiByteToWideChar(CP_UTF8, 0, deviceInfo->name, -1, wideName, MAX_PATH - 1);
			wprintf(L"Name                        = %s\n", wideName);
		}
#else
		printf("Name                        = %s\n", deviceInfo->name);
#endif
		printf("Host API                    = %s\n", Pa_GetHostApiInfo(deviceInfo->hostApi)->name);
		printf("Max inputs = %d", deviceInfo->maxInputChannels);
		printf(", Max outputs = %d\n", deviceInfo->maxOutputChannels);

		printf("Default low input latency   = %8.4f\n", deviceInfo->defaultLowInputLatency);
		printf("Default low output latency  = %8.4f\n", deviceInfo->defaultLowOutputLatency);
		printf("Default high input latency  = %8.4f\n", deviceInfo->defaultHighInputLatency);
		printf("Default high output latency = %8.4f\n", deviceInfo->defaultHighOutputLatency);

#ifdef WIN32
#if PA_USE_ASIO
		/* ASIO specific latency information */
		if (Pa_GetHostApiInfo(deviceInfo->hostApi)->type == paASIO) {
			long minLatency, maxLatency, preferredLatency, granularity;

			err = PaAsio_GetAvailableLatencyValues(i,&minLatency, &maxLatency, &preferredLatency, &granularity);

			printf("ASIO minimum buffer size    = %ld\n", minLatency);
			printf("ASIO maximum buffer size    = %ld\n", maxLatency);
			printf("ASIO preferred buffer size  = %ld\n", preferredLatency);

			if (granularity == -1)
				printf("ASIO buffer granularity     = power of 2\n");
			else
				printf("ASIO buffer granularity     = %ld\n", granularity);
		}
#endif /* PA_USE_ASIO */
#endif /* WIN32 */

		printf("Default sample rate         = %8.2f\n", deviceInfo->defaultSampleRate);

		/* poll for standard sample rates */
		inputParameters.device = i;
		inputParameters.channelCount = deviceInfo->maxInputChannels;
		inputParameters.sampleFormat = paFloat32;// paInt16;
		inputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
		inputParameters.hostApiSpecificStreamInfo = NULL;

		outputParameters.device = i;
		outputParameters.channelCount = deviceInfo->maxOutputChannels;
		outputParameters.sampleFormat = paFloat32;// paInt16;
		outputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
		outputParameters.hostApiSpecificStreamInfo = NULL;

		if (inputParameters.channelCount > 0)
		{
			printf("Supported standard sample rates\n for half-duplex 16 bit %d channel input = \n",
				inputParameters.channelCount);
			PrintSupportedStandardSampleRates(&inputParameters, NULL);
		}

		if (outputParameters.channelCount > 0)
		{
			printf("Supported standard sample rates\n for half-duplex 16 bit %d channel output = \n",
				outputParameters.channelCount);
			PrintSupportedStandardSampleRates(NULL, &outputParameters);
		}

		if (inputParameters.channelCount > 0 && outputParameters.channelCount > 0)
		{
			printf("Supported standard sample rates\n for full-duplex 16 bit %d channel input, %d channel output = \n",
				inputParameters.channelCount, outputParameters.channelCount);
			PrintSupportedStandardSampleRates(&inputParameters, &outputParameters);
		}
	}

	Pa_Terminate();

	printf("----------------------------------------------\n");
	return 0;

error:
	Pa_Terminate();
	fprintf(stderr, "Error number: %d\n", err);
	fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
	return err;
}
/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
int main_pablio(void)
{
	int             i, j;
	int left_phase = 0;
	int right_phase = 0;
	int left_inc = 1;
	int right_inc = 3; /* higher pitch so we can distinguish left and right. */

	PaError         err;
	PABLIO_Stream  *aOutStream;

	/* Open simplified blocking I/O layer on top of PortAudio. */
	err = OpenAudioStream(&aOutStream, 1, 3, SAMPLE_RATE, paFloat32, (PABLIO_READ_WRITE | PABLIO_STEREO));
	if (err != paNoError) goto error;


	/* Initialize oscillator phases. */
	/* initialise sinusoidal wavetable */
	for (i = 0; i<TABLE_SIZE; i++)
	{
		sine[i] = (float)sin(((double)i / (double)TABLE_SIZE) * M_PI * 2.);
	}

	phases[0] = 0.0;
	phases[1] = 0.0;

	for (i = 0; i<(NUM_SECONDS * SAMPLE_RATE); i += FRAMES_PER_BLOCK)
	{
		// Generate sawtooth waveforms in a block for efficiency. 
		for (j = 0; j<FRAMES_PER_BLOCK; j++)
		{
			// Generate a sawtooth wave by incrementing a variable. 
			phases[0] += PHASE_INCREMENT;
			// The signal range is -1.0 to +1.0 so wrap around if we go over. 
			if (phases[0] > 1.0f) phases[0] -= 2.0f;
			outSamples[2*j] = phases[0];

			// On the second channel, generate a sawtooth wave a fifth higher. 
			phases[1] += PHASE_INCREMENT * (3.0f / 2.0f);
			if (phases[1] > 1.0f) phases[1] -= 2.0f;
			outSamples[2*j+1] = phases[1];
		}
		
		WriteAudioStream(aOutStream, outSamples, FRAMES_PER_BLOCK);

		Pa_Sleep(10);

		ReadAudioStream(aOutStream, inSamples, FRAMES_PER_BLOCK);
	}

	CloseAudioStream(aOutStream);

	printf("Sawtooth sound test complete.\n");
	fflush(stdout);
	return 0;

error:
	fprintf(stderr, "An error occured while using PABLIO\n");
	fprintf(stderr, "Error number: %d\n", err);
	fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
	return -1;
}

int main()
{
	main_padevs();
	system("pause");

	//main_pablio();
    return 0;
}

