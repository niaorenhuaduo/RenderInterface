#include "Input.h"
#include "GameError.h"


Input::Input() {
	for(U32 i=0; i<KEYS_ARRAY_LENGTH; ++i) {
		mbKeysDown[i] = false;
		mbKeysPressed[i] = false;
	}

	mMouseX = mMouseY = 0;
	mMouseRawX = mMouseRawY = 0;
	mbMouseLButton = mbMouseMButton = mbMouseRButton = false;
	mbMouseX1Button = mbMouseX2Button = false;

	for(int i=0; i<MAX_CONTROLLERS; ++i) {
		mControllers[i].vibrateTimeLeft = 0;
		mControllers[i].vibrateTimeRight = 0;
	}

	mbNewLine = true;
	mTextIn = "";
	mLastCharIn = 0;
}

Input::~Input() {
	if(mbMouseCaptured) ReleaseCapture();
}

void Input::OnInit(HWND hWnd, bool bCapture) {

	try {
		mbMouseCaptured = bCapture;
		mRid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		mRid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		mRid[0].dwFlags = RIDEV_INPUTSINK;
		mRid[0].hwndTarget = hWnd;
		RegisterRawInputDevices(mRid, 1, sizeof(mRid[0]));
		if(mbMouseCaptured) SetCapture(hWnd);

		ZeroMemory(mControllers, sizeof(ControllerState) * MAX_CONTROLLERS);
		CheckControllers();
	} catch(...) { throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing input system.")); }
}

void Input::KeyDown(WPARAM wParam) {
	if(wParam < KEYS_ARRAY_LENGTH) {
		mbKeysDown[wParam] = true;
		mbKeysPressed[wParam] = true;
	}
}

void Input::KeyUp(WPARAM wParam) {
	if(wParam < KEYS_ARRAY_LENGTH) mbKeysDown[wParam] = false;
}

void Input::keyIn(WPARAM wParam) {
	if(mbNewLine) {
		mTextIn.clear();
		mbNewLine = false;
	}
	if(wParam == '\b') {
		if(mTextIn.length() > 0) mTextIn.erase(mTextIn.size()-1);
	} else {
		mTextIn += wParam;
		mLastCharIn = wParam;
	}
	if((char)wParam == '\r') mbNewLine = true;
}

bool Input::IsKeyDown(UCHAR vkey) const {
	if(vkey < KEYS_ARRAY_LENGTH) return mbKeysDown[vkey];
	else return false;
}

bool Input::WasKeyPressed(UCHAR vkey) const {
	if(vkey < KEYS_ARRAY_LENGTH) return mbKeysPressed[vkey];
	else return false;
}

bool Input::AnyKeyPressed() const {
	for(U32 i=0; i<KEYS_ARRAY_LENGTH; ++i) if(mbKeysPressed[i]) return true;
	return false;
}

void Input::MouseIn( LPARAM lParam ) {
	mMouseX = GET_X_LPARAM(lParam);
	mMouseY = GET_Y_LPARAM(lParam);
}

void Input::MouseRawIn( LPARAM lParam ) {
	UINT dwSize = 40;
	static BYTE lpb[40];
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

	RAWINPUT *raw = (RAWINPUT*)lpb;
	if(raw->header.dwType == RIM_TYPEMOUSE) {
		mMouseRawX = raw->data.mouse.lLastX;
		mMouseRawY = raw->data.mouse.lLastY;
	}
}

void Input::Clear( UCHAR value ) {
	if(value & KEYS_DOWN) {
		for(int i=0; i<KEYS_ARRAY_LENGTH; ++i) mbKeysDown[i] = false;
	}
	if(value & KEYS_PRESSED) {
		for(int i=0; i<KEYS_ARRAY_LENGTH; ++i) mbKeysPressed[i] = false;
	}
	if(value & MOUSE) {
		mMouseX = 0;
		mMouseY = 0;
		mMouseRawX = 0;
		mMouseRawY = 0;
	}
	if(value & TEXT) {
		ClearTextIn();
	}
}

void Input::CheckControllers()
{
	DWORD result;
	for( DWORD i = 0; i <MAX_CONTROLLERS; i++)
	{
		result = XInputGetState(i, &mControllers[i].state);
		if(result == ERROR_SUCCESS)
			mControllers[i].connected = true;
		else
			mControllers[i].connected = false;
	}
}

void Input::ReadControllers()
{
	DWORD result;
	for( DWORD i = 0; i <MAX_CONTROLLERS; i++)
	{
		if(mControllers[i].connected)
		{
			result = XInputGetState(i, &mControllers[i].state);
			if(result == ERROR_DEVICE_NOT_CONNECTED)    // if controller disconnected
				mControllers[i].connected = false;
		}
	}
}

void Input::VibrateControllers(float fElapsedTime)
{
	for(int i=0; i < MAX_CONTROLLERS; i++)
	{
		if(mControllers[i].connected)
		{
			mControllers[i].vibrateTimeLeft -= fElapsedTime;
			if(mControllers[i].vibrateTimeLeft < 0)
			{
				mControllers[i].vibrateTimeLeft = 0;
				mControllers[i].vibration.wLeftMotorSpeed = 0;
			}
			mControllers[i].vibrateTimeRight -= fElapsedTime;
			if(mControllers[i].vibrateTimeRight < 0)
			{
				mControllers[i].vibrateTimeRight = 0;
				mControllers[i].vibration.wRightMotorSpeed = 0;
			}
			XInputSetState(i, &mControllers[i].vibration);
		}
	}
}



