#include "Com_Audio.h"
#include "../main.h"
#include "../GameObject/GameObject.h"

using namespace DirectX::SimpleMath;

IXAudio2* Com_Audio::s_p_mXAudio = NULL;
IXAudio2MasteringVoice* Com_Audio::s_p_mMasteringVoice = NULL;
GameObject* Com_Audio::s_p_mTarget = nullptr;
float Com_Audio::s_fMasterVolume = 1.0f;

void Com_Audio::InitMaster()
{
	// COMの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2の生成
	XAudio2Create(&s_p_mXAudio, 0);

	// マスタリングボイス設定
	s_p_mXAudio->CreateMasteringVoice(&s_p_mMasteringVoice);
}

void Com_Audio::UninitMaster()
{
	s_p_mMasteringVoice->DestroyVoice();
	s_p_mXAudio->Release();
	CoUninitialize();
}

void Com_Audio::SetTarget(GameObject* target)
{
	s_p_mTarget = target;
}

void Com_Audio::Init()
{
	
}

void Com_Audio::Uninit()
{
	p_mSourceVoice->Stop();
	p_mSourceVoice->DestroyVoice();

	delete[] p_mSoundData;
}

void Com_Audio::Update()
{
	if (bBGM)
	{

	}
	else
	{
		if (bUseTarget)
		{
			float length = Vector3::Distance(s_p_mTarget->p_mTransform->mPosition, p_mObject->p_mTransform->mPosition);

			float tmpVolume = length / fSoundScaleRatio;

			if (tmpVolume > 1.0f)
			{
				tmpVolume = 1.0f;
			}

			tmpVolume = 1.0f - tmpVolume;

			SetVolume(tmpVolume);
		}
	}
}

void Com_Audio::Load(const char* _FileName)
{
	// サウンドデータの読み込み処理
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)_FileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		p_mSoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)p_mSoundData, buflen);


		iLength = readlen;
		iPlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// サウンドソース生成
	s_p_mXAudio->CreateSourceVoice(&p_mSourceVoice, &wfx);
	assert(p_mSourceVoice);
}

void Com_Audio::Play(bool Loop)
{
	p_mSourceVoice->Stop();
	p_mSourceVoice->FlushSourceBuffers();

	// バッファ設定
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));

	bufinfo.AudioBytes = iLength;
	bufinfo.pAudioData = p_mSoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = iPlayLength;

	if (Loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = iPlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	p_mSourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

	p_mSourceVoice->Start();
}

void Com_Audio::Stop()
{
	p_mSourceVoice->Stop();
}

void Com_Audio::SetVolume(float _volume)
{
	p_mSourceVoice->SetVolume(_volume);
}

void Com_Audio::SetPlayAtLocation(bool _val)
{
	bPlayAtLocation = _val;
}

void Com_Audio::SetMasterVolume(float _volume)
{
	s_p_mMasteringVoice->SetVolume(_volume);
}
