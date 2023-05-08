#include "AudioWrapper.h"

AudioWrapper::AudioWrapper()
{
	int result = initAudio();

	if (result != 0)
	{

		//ERROR
	}
}
AudioWrapper::~AudioWrapper()
{
	for (int i = 0; i < NUM_VOICES; i++)
	{

		if (sourceVoices[i] != NULL)
		{
			sourceVoices[i]->Stop();
			sourceVoices[i]->DestroyVoice();
		}
	}


	audioEngine->Release();
	delete[] sourceVoices;
	CoUninitialize();
}

int AudioWrapper::initAudio()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);



	if (FAILED(XAudio2Create(&audioEngine)))
	{
		wprintf(L"INIT OF XAUDIO FAILED");
		CoUninitialize();
		return -1;
	}

	if (FAILED(audioEngine->CreateMasteringVoice(&masterVoice)))
	{
		audioEngine->Release();
		CoUninitialize();
		return -2;
	}

	for (int i = 0; i < NUM_VOICES; i++)
	{
		sourceVoices[i] = NULL;
	}


	return 0;
}


bool AudioWrapper::audioStillPlaying()
{

	for (int i = 0; i < NUM_VOICES; i++)
	{

		if (sourceVoices[i] != NULL)
		{
			XAUDIO2_VOICE_STATE state;
			sourceVoices[i]->GetState(&state);
			int count = state.BuffersQueued;

			if (count > 0)
				return true;
		}
	}
	return false;
}

void AudioWrapper::stopLoopingAudio()
{
	vector<int>::iterator it;
	it = loopingVoices.begin();

	while (it != loopingVoices.end())
	{


		if (sourceVoices[*it] != NULL)
		{
			

			sourceVoices[*it]->Stop();
			sourceVoices[*it]->DestroyVoice();
			sourceVoices[*it] = NULL;

			BYTE *del = memoryFix[*it];
			if (del != NULL)
			{
				delete[] del;
				del = NULL;
				memoryFix.erase(*it);
			}

			it++;
		}
	}

	loopingVoices.clear();
}

void AudioWrapper::stopAllAudio()
{
	for (int i = 0; i < NUM_VOICES; i++)
	{

		if (sourceVoices[i] != NULL)
		{
			sourceVoices[i]->Stop();
			sourceVoices[i]->DestroyVoice();
			sourceVoices[i] = NULL;


			BYTE *del = memoryFix[i];

			if (del != NULL)
			{
				delete[] del;
				del = NULL;
				memoryFix.erase(i);
			}
		}
	}
	loopingVoices.clear();
}

void AudioWrapper::playSound(wstring fileName, bool loop)
{
	//PlaySoundW(fileName.c_str(), NULL, SND_FILENAME | SND_ASYNC);
	//WCHAR wav1FilePath[MAX_PATH];
	WCHAR* path = const_cast<wchar_t*>(fileName.c_str());
	playPCM(audioEngine, path, loop);

}

void AudioWrapper::playSound(wstring fileName, bool loop, float volume)
{
	WCHAR* path = const_cast<wchar_t*>(fileName.c_str());
	playPCM(audioEngine, path, loop, volume);
}

int AudioWrapper::playPCM(IXAudio2* audioEngine, WCHAR *path, bool infiniteLoop)
{

	CWaveFile wav;
	//WCHAR wavFilePath[MAX_PATH];

	if (FAILED(wav.Open(path, NULL, WAVEFILE_READ)))
	{
		wprintf(L"Failed To Open WaveFile 1");
		audioEngine->Release();
		CoUninitialize();
		return -1;
	}

	WAVEFORMATEX* pwfx = wav.GetFormat();
	DWORD cbWaveSize = wav.GetSize();
	BYTE* pbWaveData = new BYTE[cbWaveSize];

	if (FAILED(wav.Read(pbWaveData, cbWaveSize, &cbWaveSize)))
	{
		wprintf(L"Failed to read WAV data");
		delete[] pbWaveData;
		pbWaveData = NULL;
		return -1;
	}

	// LOOK FOR AN AVALIABLE VOICE SLOT
	for (int i = 0; i < NUM_VOICES; i++)
	{
		XAUDIO2_VOICE_STATE state;
		// IF ITS NULL WE KNOW IT IS NOT CURRENTLY BEING USED
		if (sourceVoices[i] == NULL)
		{

			memoryFix.insert({ i, pbWaveData });

			audioEngine->CreateSourceVoice(&sourceVoices[i], pwfx);
			XAUDIO2_BUFFER buffer = { 0 };
			buffer.pAudioData = pbWaveData;
			buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
			buffer.AudioBytes = cbWaveSize;
			
			if (infiniteLoop)
			{
				buffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;
				loopingVoices.push_back(i);
				sourceVoices[i]->SetVolume(0.75f);//half volume for music
			}
			else
				sourceVoices[i]->SetVolume(0.15f);

			sourceVoices[i]->SubmitSourceBuffer(&buffer);
			sourceVoices[i]->Start(0);


			return 0;
		}
		else
		{
			sourceVoices[i]->GetState(&state);
			bool isAvaliable = state.BuffersQueued;


			if (!isAvaliable)
			{
				BYTE *del = memoryFix[i];
				delete[] del;
				del = NULL;
				memoryFix.erase(i);

				XAUDIO2_BUFFER buffer = { 0 };
				buffer.pAudioData = pbWaveData;
				buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
				buffer.AudioBytes = cbWaveSize;

				if (infiniteLoop)
				{
					buffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;
					loopingVoices.push_back(i);

				}

				memoryFix.insert({ i, pbWaveData });

				sourceVoices[i]->SubmitSourceBuffer(&buffer);
				sourceVoices[i]->Start(0);
				return 0;
			}
		}
	}

	// WE COULDNT FIND SPOT
	delete[] pbWaveData;
	pbWaveData = NULL;

	return -1;
}









int AudioWrapper::playPCM(IXAudio2* audioEngine, WCHAR *path, bool infiniteLoop, float volume)
{

	CWaveFile wav;
	//WCHAR wavFilePath[MAX_PATH];

	if (FAILED(wav.Open(path, NULL, WAVEFILE_READ)))
	{
		wprintf(L"Failed To Open WaveFile 1");
		audioEngine->Release();
		CoUninitialize();
		return -1;
	}

	WAVEFORMATEX* pwfx = wav.GetFormat();
	DWORD cbWaveSize = wav.GetSize();
	BYTE* pbWaveData = new BYTE[cbWaveSize];

	if (FAILED(wav.Read(pbWaveData, cbWaveSize, &cbWaveSize)))
	{
		wprintf(L"Failed to read WAV data");
		delete[] pbWaveData;
		pbWaveData = NULL;
		return -1;
	}

	// LOOK FOR AN AVALIABLE VOICE SLOT
	for (int i = 0; i < NUM_VOICES; i++)
	{
		XAUDIO2_VOICE_STATE state;
		// IF ITS NULL WE KNOW IT IS NOT CURRENTLY BEING USED
		if (sourceVoices[i] == NULL)
		{

			memoryFix.insert({ i, pbWaveData });

			audioEngine->CreateSourceVoice(&sourceVoices[i], pwfx);
			XAUDIO2_BUFFER buffer = { 0 };
			buffer.pAudioData = pbWaveData;
			buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
			buffer.AudioBytes = cbWaveSize;

			if (infiniteLoop)
			{
				buffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;
				loopingVoices.push_back(i);
				sourceVoices[i]->SetVolume(volume);//half volume for music
			}
			else
				sourceVoices[i]->SetVolume(volume);

			sourceVoices[i]->SubmitSourceBuffer(&buffer);
			sourceVoices[i]->Start(0);


			return 0;
		}
		else
		{
			sourceVoices[i]->GetState(&state);
			bool isAvaliable = state.BuffersQueued;


			if (!isAvaliable)
			{
				BYTE *del = memoryFix[i];
				delete[] del;
				del = NULL;
				memoryFix.erase(i);

				XAUDIO2_BUFFER buffer = { 0 };
				buffer.pAudioData = pbWaveData;
				buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
				buffer.AudioBytes = cbWaveSize;

				if (infiniteLoop)
				{
					buffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;
					loopingVoices.push_back(i);

				}

				memoryFix.insert({ i, pbWaveData });

				sourceVoices[i]->SubmitSourceBuffer(&buffer);
				sourceVoices[i]->Start(0);
				return 0;
			}
		}
	}

	// WE COULDNT FIND SPOT
	delete[] pbWaveData;
	pbWaveData = NULL;

	return -1;
}

