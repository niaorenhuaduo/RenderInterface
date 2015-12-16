#include "Audio.h"

IXACT3Engine	*mpEngine;
IXACT3WaveBank	*mpWaveBank;
IXACT3SoundBank *mpSoundBank;
XACTINDEX		 mCueIndex; 
void *mpMapWaveBank;
void *mpSoundBankData;
bool mbInitialized;



Audio::Audio() : 
mpEngine(NULL),
mpWaveBank(NULL),
mpSoundBank(NULL),
mCueIndex(0),
mpMapWaveBank(NULL),
mpSoundBankData(NULL),
mbCoInitialized(false)
{}

Audio::~Audio()
{
	if(mpEngine) {
		mpEngine->ShutDown();
		mpEngine->Release();
	}

	SAFE_DELETE_ARRAY(mpSoundBankData);
	if(mpMapWaveBank) {
		UnmapViewOfFile(mpMapWaveBank);
		mpMapWaveBank = NULL;
	}

	if(mbCoInitialized) CoUninitialize();
}

bool Audio::OnInitialize(const char waveBankFile[], const char soundBankFile[]) {
	HRESULT hr;
	HANDLE hFile;
	DWORD dwFileSize;
	DWORD dwBytesRead;
	HANDLE hMapFile;


	hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );  // COINIT_APARTMENTTHREADED will work too
	if( SUCCEEDED( hr ) ) hr = XACT3CreateEngine( 0, &mpEngine );
	if( FAILED( hr ) || mpEngine == NULL ) {
		MessageBox(NULL, "Failed to create the XACT3 Sound Engine!", "error", MB_OK);
		return false;
	}

	// Initialize & create the XACT runtime 
	XACT_RUNTIME_PARAMETERS xrParams = {0};
	xrParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	hr = mpEngine->Initialize( &xrParams );
	if( FAILED( hr ) ) {
		MessageBox(NULL, "Failed to initialize the XACT3 Sound Engine!", "error", MB_OK);
		return false;
	}

	hr = E_FAIL; // assume failure
	hFile = CreateFile( waveBankFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
	if( hFile != INVALID_HANDLE_VALUE )
	{
		dwFileSize = GetFileSize( hFile, NULL );
		if( dwFileSize != -1 )
		{
			hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL );
			if( hMapFile )
			{
				mpMapWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
				if( mpMapWaveBank )
				{
					hr = mpEngine->CreateInMemoryWaveBank( mpMapWaveBank, dwFileSize, 0,
						0, &mpWaveBank );
				}
				CloseHandle( hMapFile ); // pbWaveBank maintains a handle on the file so close this unneeded handle
			}
		}
		CloseHandle( hFile ); // pbWaveBank maintains a handle on the file so close this unneeded handle
	}
	if( FAILED( hr ) ) {
		std::string msg = "Failed to load the wave bank: " + std::string(waveBankFile);
		MessageBox(NULL, msg.c_str(), "error", MB_OK);
		return false; // CleanupXACT() will cleanup state before exiting
	}

	hr = E_FAIL; // assume failure
	hFile = CreateFile( soundBankFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
	if( hFile != INVALID_HANDLE_VALUE )
	{
		dwFileSize = GetFileSize( hFile, NULL );
		if( dwFileSize != -1 )
		{
			// Allocate the data here and free the data when recieving the sound bank destroyed notification
			mpSoundBankData = new BYTE[dwFileSize];
			if( mpSoundBankData )
			{
				if( 0 != ReadFile( hFile, mpSoundBankData, dwFileSize, &dwBytesRead, NULL ) )
				{
					hr = mpEngine->CreateSoundBank( mpSoundBankData, dwFileSize, 0,
						0, &mpSoundBank );
				}
			}
		}
		CloseHandle( hFile );
	}
	if( FAILED( hr ) ) {
		std::string msg = "Failed to load the sound bank: " + std::string(soundBankFile);
		MessageBox(NULL, msg.c_str(), "error", MB_OK);
		return false; // CleanupXACT() will cleanup state before exiting
	}

	return true;
}

void Audio::OnUpdate()
{
	if(mpEngine == NULL) return;
	mpEngine->DoWork();
}

void Audio::PlayCue( const char cue[] )
{
	if(mpSoundBank == NULL) return;
	mCueIndex = mpSoundBank->GetCueIndex(cue);
	mpSoundBank->Play(mCueIndex, 0, 0, NULL);
}

void Audio::StopCue( const char cue[] )
{
	if(mpSoundBank == NULL) return;
	mCueIndex = mpSoundBank->GetCueIndex(cue);
	mpSoundBank->Stop(mCueIndex, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}
