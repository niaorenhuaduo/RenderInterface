#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <RI.h>
#include <string>
#include <XInput.h>

// for high-definition mouse
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC      ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE     ((USHORT) 0x02)
#endif

const DWORD GAMEPAD_THUMBSTICK_DEADZONE = (DWORD)(0.20f * 0X7FFF);    // default to 20% of range as deadzone
const DWORD GAMEPAD_TRIGGER_DEADZONE = 30;							  // trigger range 0-255
const DWORD MAX_CONTROLLERS = 4;									  // Maximum number of controllers supported by XInput

// Bit corresponding to gamepad button in state.Gamepad.wButtons
const DWORD GAMEPAD_DPAD_UP        = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN      = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT      = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT     = 0x0008;
const DWORD GAMEPAD_START_BUTTON   = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON    = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB     = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB    = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER  = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A              = 0x1000;
const DWORD GAMEPAD_B              = 0x2000;
const DWORD GAMEPAD_X              = 0x4000;
const DWORD GAMEPAD_Y              = 0x8000;

struct ControllerState
{
	XINPUT_STATE        state;
	XINPUT_VIBRATION    vibration;
	float               vibrateTimeLeft;    // mSec
	float               vibrateTimeRight;   // mSec
	bool                connected;
};

class Input {
public:
	enum {  // values for clear()
		KEYS_DOWN = 1,
		KEYS_PRESSED = 2,
		MOUSE = 4,
		TEXT = 8,
		KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT
	};

	Input();
	virtual ~Input();

	void OnInit(HWND hWnd, bool bCapture);

	// Keyboard
	void KeyDown(WPARAM wParam);
	void KeyUp(WPARAM wParam);
	void keyIn(WPARAM wParam); // save the entered char in mTextIn

	bool IsKeyDown(UCHAR vkey) const;
	bool WasKeyPressed(UCHAR vkey) const;  // true if the key was pressed in this frame
	bool AnyKeyPressed() const;

	// Mouse
	void MouseIn(LPARAM lParam);
	void MouseRawIn(LPARAM lParam);
	void SetMouseLButton(bool x) { mbMouseLButton = x; }
	void SetMouseMButton(bool x) { mbMouseMButton = x; }
	void SetMouseRButton(bool x) { mbMouseRButton = x; }
	void SetMouseXButton(WPARAM wParam) { mbMouseX1Button = (wParam & MK_XBUTTON1) ? true:false;
                                          mbMouseX2Button = (wParam & MK_XBUTTON2) ? true:false;}
	I32 GetMouseX() const { return mMouseX; }
	I32 GetMouseY() const { return mMouseY; }
	I32 GetMouseRawX() const { return mMouseRawX; }
	I32 GetMouseRawY() const { return mMouseRawY; }
	bool GetMouseLButton()  const { return mbMouseLButton; }
	bool GetMouseMButton()  const { return mbMouseMButton; }
	bool GetMouseRButton()  const { return mbMouseRButton; }
	bool GetMouseX1Button() const { return mbMouseX1Button; }
	bool GetMouseX2Button() const { return mbMouseX2Button; }

	// Clear
	void Clear(UCHAR value);
	void ClearAll() { Clear(KEYS_MOUSE_TEXT); }
	void ClearKeyPress(UCHAR vkey) { if(vkey < KEYS_ARRAY_LENGTH) mbKeysPressed[vkey] = false; }
	void ClearTextIn() { mTextIn.clear(); }

	std::string GetTextIn() { return mTextIn; }
	char		GetLastChar() { return mLastCharIn; }


	// Controller
	void CheckControllers();
	void ReadControllers();
	void VibrateControllers(float fElapsedTime);

	// Return state of specified game controller.
	const ControllerState* getControllerState(UINT n)
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return &mControllers[n];
	}

	// Return state of controller n buttons.
	const WORD getGamepadButtons(UINT n)
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return mControllers[n].state.Gamepad.wButtons;
	}

	// Return state of controller n D-pad Up
	bool getGamepadDPadUp(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return ((mControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_UP) != 0);
	}

	// Return state of controller n D-pad Down.
	bool getGamepadDPadDown(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return ((mControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_DOWN) != 0);
	}

	// Return state of controller n D-pad Left.
	bool getGamepadDPadLeft(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return ((mControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_LEFT) != 0);
	}

	// Return state of controller n D-pad Right.
	bool getGamepadDPadRight(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_RIGHT) != 0);
	}

	// Return state of controller n Start button.
	bool getGamepadStart(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_START_BUTTON) != 0);
	}

	// Return state of controller n Back button.
	bool getGamepadBack(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_BACK_BUTTON) != 0);
	}

	// Return state of controller n Left Thumb button.
	bool getGamepadLeftThumb(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_THUMB) != 0);
	}

	// Return state of controller n Right Thumb button.
	bool getGamepadRightThumb(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_THUMB) != 0);
	}

	// Return state of controller n Left Shoulder button.
	bool getGamepadLeftShoulder(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_SHOULDER) != 0);
	}

	// Return state of controller n Right Shoulder button.
	bool getGamepadRightShoulder(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_SHOULDER) != 0);
	}

	// Return state of controller n A button.
	bool getGamepadA(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_A) != 0);
	}

	// Return state of controller n B button.
	bool getGamepadB(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_B) != 0);
	}

	// Return state of controller n X button.
	bool getGamepadX(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_X) != 0);
	}

	// Return state of controller n Y button.
	bool getGamepadY(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_Y) != 0);
	}

	// Return value of controller n Left Trigger.
	BYTE getGamepadLeftTrigger(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return (mControllers[n].state.Gamepad.bLeftTrigger);
	}

	// Return value of controller n Right Trigger.
	BYTE getGamepadRightTrigger(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return (mControllers[n].state.Gamepad.bRightTrigger);
	}

	// Return value of controller n Left Thumbstick X.
	SHORT getGamepadThumbLX(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return (mControllers[n].state.Gamepad.sThumbLX);
	}

	// Return value of controller n Left Thumbstick Y.
	SHORT getGamepadThumbLY(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return (mControllers[n].state.Gamepad.sThumbLY);
	}

	// Return value of controller n Right Thumbstick X.
	SHORT getGamepadThumbRX(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return (mControllers[n].state.Gamepad.sThumbRX);
	}

	// Return value of controller n Right Thumbstick Y.
	SHORT getGamepadThumbRY(UINT n) 
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		return (mControllers[n].state.Gamepad.sThumbRY);
	}

	// Vibrate controller n left motor.
	// Left is low frequency vibration.
	// speed 0=off, 65536=100 percent
	// sec is time to vibrate in seconds
	void gamePadVibrateLeft(UINT n, WORD speed, float sec)
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		mControllers[n].vibration.wLeftMotorSpeed = speed;
		mControllers[n].vibrateTimeLeft = sec;
	}

	// Vibrate controller n right motor.
	// Right is high frequency vibration.
	// speed 0=off, 65536=100 percent
	// sec is time to vibrate in seconds
	void gamePadVibrateRight(UINT n, WORD speed, float sec)
	{
		if(n > MAX_CONTROLLERS-1)
			n=MAX_CONTROLLERS-1;
		mControllers[n].vibration.wRightMotorSpeed = speed;
		mControllers[n].vibrateTimeRight = sec;
	}


private:
	static const U32 KEYS_ARRAY_LENGTH = 256;
	bool mbKeysDown[KEYS_ARRAY_LENGTH];
	bool mbKeysPressed[KEYS_ARRAY_LENGTH];

	std::string mTextIn;
	char		mLastCharIn;
	bool		mbNewLine;

	I32		mMouseX, mMouseY;
	I32		mMouseRawX, mMouseRawY;
	RAWINPUTDEVICE	mRid[1];
	bool	mbMouseCaptured;
	bool	mbMouseLButton;
	bool	mbMouseMButton;
	bool	mbMouseRButton;
	bool	mbMouseX1Button;
	bool	mbMouseX2Button;
	ControllerState mControllers[MAX_CONTROLLERS];
};