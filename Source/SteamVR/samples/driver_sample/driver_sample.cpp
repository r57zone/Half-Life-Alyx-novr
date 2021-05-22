//============ Copyright (c) Valve Corporation, All rights reserved. ============
//====================== True Open Virtual Reality bridge =======================

#include <openvr_driver.h>
//#include "driverlog.h"

#include <vector>
#include <thread>
#include <chrono>

#include <windows.h>
#include <atlbase.h>
#include "IniReader\IniReader.h"

using namespace vr;
using namespace std::chrono;

#if defined(_WIN32)
#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )
#define HMD_DLL_IMPORT extern "C" __declspec( dllimport )
#elif defined(__GNUC__) || defined(COMPILER_GCC) || defined(__APPLE__)
#define HMD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define HMD_DLL_IMPORT extern "C" 
#else
#error "Unsupported Platform."
#endif

inline HmdQuaternion_t HmdQuaternion_Init( double w, double x, double y, double z )
{
	HmdQuaternion_t quat;
	quat.w = w;
	quat.x = x;
	quat.y = y;
	quat.z = z;
	return quat;
}

inline void HmdMatrix_SetIdentity( HmdMatrix34_t *pMatrix )
{
	pMatrix->m[0][0] = 1.f;
	pMatrix->m[0][1] = 0.f;
	pMatrix->m[0][2] = 0.f;
	pMatrix->m[0][3] = 0.f;
	pMatrix->m[1][0] = 0.f;
	pMatrix->m[1][1] = 1.f;
	pMatrix->m[1][2] = 0.f;
	pMatrix->m[1][3] = 0.f;
	pMatrix->m[2][0] = 0.f;
	pMatrix->m[2][1] = 0.f;
	pMatrix->m[2][2] = 1.f;
	pMatrix->m[2][3] = 0.f;
}


// keys for use with the settings API
static const char * const k_pch_Sample_Section = "hlalyx";
static const char * const k_pch_Sample_SerialNumber_String = "serialNumber";
static const char * const k_pch_Sample_ModelNumber_String = "modelNumber";
static const char * const k_pch_Sample_WindowX_Int32 = "windowX";
static const char * const k_pch_Sample_WindowY_Int32 = "windowY";
static const char * const k_pch_Sample_WindowWidth_Int32 = "windowWidth";
static const char * const k_pch_Sample_WindowHeight_Int32 = "windowHeight";
static const char * const k_pch_Sample_RenderWidth_Int32 = "renderWidth";
static const char * const k_pch_Sample_RenderHeight_Int32 = "renderHeight";
static const char * const k_pch_Sample_SecondsFromVsyncToPhotons_Float = "secondsFromVsyncToPhotons";
static const char * const k_pch_Sample_DisplayFrequency_Float = "displayFrequency";
static const char * const k_pch_Sample_ZoomWidth_Float = "ZoomWidth";
static const char * const k_pch_Sample_ZoomHeight_Float = "ZoomHeight";
static const char * const k_pch_Sample_FOV_Float = "FOV";
static const char * const k_pch_Sample_Stereo_Bool = "Stereo";
static const char * const k_pch_Sample_DebugMode_Bool = "DebugMode";

//Velocity
double FirstCtrlLastPos[3] = { 0, 0, 0 }, SecondCtrlLastPos[3] = { 0, 0, 0 };
milliseconds deltaTime;

//Half Life Alyx
typedef struct _HMDData
{
	double	X;
	double	Y;
	double	Z;
	double	Yaw;
	double	Pitch;
	double	Roll;
} THMD, *PHMD;

typedef struct _Controller
{
	double	X;
	double	Y;
	double	Z;
	double	Yaw;
	double	Pitch;
	double	Roll;
	unsigned short	Buttons;
	float	Trigger;
	float	AxisX;
	float	AxisY;
} TController, *PController;

#define GRIP_BTN	0x0001
#define THUMB_BTN	0x0002
#define A_BTN		0x0004
#define B_BTN		0x0008
#define MENU_BTN	0x0010
#define SYS_BTN		0x0020

THMD MyHMD;
TController MyCtrl, MyCtrl2;

bool HMDConnected = false, ctrlsConnected = false;

double DegToRad(double f) {
	return f * (3.14159265358979323846 / 180);
}

//Key bindings
int KEY_ID_LEFT_TRIGGER;
int KEY_ID_LEFT_MENU;
int KEY_ID_LEFT_GRIP;
int KEY_ID_LEFT_SYSTEM;
int KEY_ID_RIGHT_TRIGGER;
int KEY_ID_RIGHT_TRIGGER2;
int KEY_ID_RIGHT_MENU;
int KEY_ID_RIGHT_GRIP;
int KEY_ID_CHANGE_WEAPON;
int KEY_ID_TELEPORT;
int KEY_ID_BACKPACK;
int KEY_ID_FIRST_AID_SYRINGE;
int KEY_ID_FIXING_LEFT_CONTROLLER;
int KEY_ID_THROW_ITEMS;
int KEY_ID_HEALTH_AND_AMMO_DISPLAY;
int KEY_ID_MOVE_CONTROLLERS_FORWARD;
int KEY_ID_MOVE_CONTROLLERS_BACK;
int KEY_ID_PLAYER_RISE_HIGHER;
int KEY_ID_PLAYER_RISE_LOWER;
int KEY_ID_PLAYER_RISE_RESET;
int KEY_ID_CROUCH;
int KEY_ID_COVER_MOUTH;
int KEY_ID_PUT_HAT;
int KEY_ID_LASER_MODE;
int KEY_ID_THROW_ENERGY_BALL;
int KEY_ID_AIMING_MODE;
int KEY_ID_AIMING;
int KEY_ID_LEFT_HAND_CELL;
int KEY_ID_RIGHT_HAND_CELL;
int KEY_ID_LEFT_CTRL_MOTION;

int KEY_ID_ROTATION_CONTROLLERS_UP;
int KEY_ID_ROTATION_CONTROLLERS_DOWN;
int KEY_ID_ROTATION_CONTROLLERS_LEFT;
int KEY_ID_ROTATION_CONTROLLERS_RIGHT;
int KEY_ID_ROTATION_CONTROLLERS_RESET;

int KEY_ID_MOTION_MODE_MOVE_UP;
int KEY_ID_MOTION_MODE_MOVE_DOWN;

int KEY_ID_TURN_LEFT;
int KEY_ID_TURN_RIGHT;

int KEY_ID_UP;
int KEY_ID_DOWN;
int KEY_ID_LEFT;
int KEY_ID_RIGHT;
//end key bindings


#define StepPos 0.01; //0.0033;
#define StepRot 0.4;
double HMDPosZ = 0;

double FirstCtrlPos[3] = { 0, 0, 0 }, SecondCtrlPos[3] = { 0, 0, 0 };
double CtrlsRoll = 0, CtrlsPitch = 0;
float CrouchOffsetZ = 0;

THMD mouseHMD;
bool firstCP = true;
int m_HalfWidth = 1920 / 2;
int m_HalfHeight = 1080 / 2;
float mouseSensetivePitch = 0.04;
float mouseSensetiveYaw = 0.05;
float mouseYaw = 0, mousePitch = 0;

HWND SteamVRHeadsetWindow = 0;
HWND HalfLifeAlyxWindow = 0;
bool HeadsetWindowFocused = false;
bool HalfLifeAlyxFocused = false;
bool VRMode = false;
float VRAimingAngleOffset;

float oldPitch = 0, deltaPitch = 0;
float oldRoll = 0, deltaRoll = 0;
double CtrlPosOffset[3] = { 0, 0, 0 };

//For fix left controller
double FirstCtrlLastXYZ[3] = { 0, 0, 0 };
double FirstCtrlLastYPR[3] = { 0, 0, 0 };

double SecondCtrlYPROffset[3] = { 0, 0, 0 };
double SecondCtrlLastYPR[3] = { 0, 0, 0 };


float LeftHandYaw = 0, LeftHandPitch = 0, LeftAngleDist = 0, LeftRadDist = 0, LeftYOffset = 0;
float RightAngleDist = 0; //Граната, немного раздвигаем руки
float RightHandYaw = 0, RightHandPitch = 0, RightRadDist = 0;

float HandRadDist = 0;

//Motion
float MotionLeftRadDist = 0, MotionLeftAngleDist = 0;


int GranadeTick = 0;
bool UseGranade = false;

bool Aiming = false;
bool CoverMouth = false;
bool PutHat = false;
bool LaserMode = false;
float LaserPos = 0, LaserYaw = 0;

bool UsedEnergyBall = false;

#define OffsetPitchByDefault -45
float ZPosSensetive = 0.009;

double OffsetYPR(float f, float f2)
{
	f -= f2;
	if (f < -180)
		f += 360;
	else if (f > 180)
		f -= 360;

	return f;
}

void MouseToYawPitch()
{
	POINT mousePos;

	if (firstCP) {
		SetCursorPos(m_HalfWidth, m_HalfHeight);
		firstCP = false;
	}

	GetCursorPos(&mousePos);

	mouseYaw += (mousePos.x - m_HalfWidth) * mouseSensetiveYaw * -1;
	if (mouseYaw < -180)
		mouseYaw += 360;
	else if (mouseYaw > 180)
		mouseYaw -= 360;

	mouseHMD.Yaw = mouseYaw;

	mousePitch += (mousePos.y - m_HalfHeight) * mouseSensetivePitch * -1;
	if (mousePitch > 90)
		mousePitch = 90;
	else if (mousePitch < -90)
		mousePitch = -90;
	mouseHMD.Pitch = mousePitch;

	SetCursorPos(m_HalfWidth, m_HalfHeight);
}

void GetHMDData(__out THMD *HMD)
{
	if ((GetAsyncKeyState(KEY_ID_PLAYER_RISE_HIGHER) & 0x8000) != 0)
		if (HMDPosZ < 0.5)
			HMDPosZ += StepPos;
	if ((GetAsyncKeyState(KEY_ID_PLAYER_RISE_LOWER) & 0x8000) != 0)
		if (HMDPosZ - CrouchOffsetZ > -1.5)
			HMDPosZ -= StepPos;
	if ((GetAsyncKeyState(VK_SUBTRACT) & 0x8000) != 0 || (GetAsyncKeyState(KEY_ID_PLAYER_RISE_RESET) & 0x8000) != 0)
		HMDPosZ = 0; //Minus numpad or -

	HMD->X = 0;
	HMD->Y = 0;
	HMD->Z = HMDPosZ - CrouchOffsetZ;

	HMD->Yaw = mouseHMD.Yaw * -1;
	HMD->Pitch = mouseHMD.Pitch * -1;
	HMD->Roll = 0;
}

void GetControllersData(__out TController *FirstController, __out TController *SecondController)
{
	//Controllers default state 
	FirstController->AxisX = 0;
	FirstController->AxisY = 0;
	FirstController->Buttons = 0;
	FirstController->Trigger = 0;

	SecondController->AxisX = 0;
	SecondController->AxisY = 0;
	SecondController->Buttons = 0;
	SecondController->Trigger = 0;

	SecondCtrlYPROffset[0] = 0;
	SecondCtrlYPROffset[1] = 0;
	SecondCtrlYPROffset[2] = 0;

	if (HeadsetWindowFocused || HalfLifeAlyxFocused) //Don't press buttons when window is not focused / Не нажимать кнопки когда окно не в фокусе
	{
		if ((GetAsyncKeyState(KEY_ID_ROTATION_CONTROLLERS_UP) & 0x8000) != 0) CtrlsPitch -= StepRot; //U
		if ((GetAsyncKeyState(KEY_ID_ROTATION_CONTROLLERS_DOWN) & 0x8000) != 0) CtrlsPitch += StepRot; //J

		if (CtrlsPitch < -180)
			CtrlsPitch += 360;
		else if (CtrlsPitch > 180)
			CtrlsPitch -= 360;

		if ((GetAsyncKeyState(KEY_ID_ROTATION_CONTROLLERS_LEFT) & 0x8000) != 0) CtrlsRoll += StepRot; //H
		if ((GetAsyncKeyState(KEY_ID_ROTATION_CONTROLLERS_RIGHT) & 0x8000) != 0) CtrlsRoll -= StepRot; //K

		if (CtrlsRoll < -180)
			CtrlsRoll += 360;
		else if (CtrlsRoll > 180)
			CtrlsRoll -= 360;

		if ((GetAsyncKeyState(KEY_ID_ROTATION_CONTROLLERS_RESET) & 0x8000) != 0 || (GetAsyncKeyState(KEY_ID_CHANGE_WEAPON) & 0x8000) != 0) { //Y
			CtrlsRoll = 0;
			CtrlsPitch = 0;
		}
	}

	//Default
	LeftHandYaw = 0;
	LeftHandPitch = 0;
	LeftAngleDist = 0;
	LeftRadDist = 0;

	RightHandYaw = 0;
	RightHandPitch = 0;
	RightAngleDist = 0;
	RightRadDist = 0;

	//Gestures / Жесты
	//First aid / Шприц
	if ((GetAsyncKeyState(KEY_ID_FIRST_AID_SYRINGE) & 0x8000) != 0) //"\" 
	{
		LeftHandYaw = -85;
		LeftAngleDist = 13;
		LeftRadDist = 0.2;
		FirstController->Buttons |= MENU_BTN;
	}

	//Left hand HUD / Информация на левой руке
	if ((GetAsyncKeyState(KEY_ID_HEALTH_AND_AMMO_DISPLAY) & 0x8000) != 0)
	{
		LeftHandYaw = -120; //-120
		LeftHandPitch = -160; //предыдущее -150
		LeftAngleDist = 5; //предыдущее 4
		LeftRadDist = 0.1;
	}

	//Left cell for items / Левая ячейка для предметов
	if ((GetAsyncKeyState(KEY_ID_LEFT_HAND_CELL) & 0x8000) != 0)
	{
		RightHandYaw = -45;
		RightRadDist = 0.17; //0.15
		RightAngleDist = -39.5;
	}

	if ((GetAsyncKeyState(KEY_ID_RIGHT_HAND_CELL) & 0x8000) != 0)
	{
		LeftHandYaw = 45;
		LeftAngleDist = 39.5;
		LeftRadDist = 0.17;
	}

	//Crouch / Присесть
	if ((GetAsyncKeyState(KEY_ID_CROUCH) & 0x8000) != 0) {
		HMDPosZ = 0;
		CrouchOffsetZ = 0.8;
	}
	else
		CrouchOffsetZ = 0;

	//Backpack / Рюкзак (PART 1)
	if ((GetAsyncKeyState(KEY_ID_BACKPACK) & 0x8000) != 0) //"?" 
	{
		LeftHandYaw = -15;
		LeftAngleDist = -120;
		LeftRadDist = 0.22;
		LeftHandPitch = 50;
	}

	//Granades / Гранаты
	if (UseGranade) {
		if (GranadeTick < 15) {
			GranadeTick++;
			RightAngleDist = 40;
			LeftAngleDist = 17;
		}
		else {
			GranadeTick = 0;
			UseGranade = false;
		}
	}

	//Throw / Бросок
	if ((GetAsyncKeyState(KEY_ID_THROW_ITEMS) & 0x8000) != 0) //186 is ";"
	{
		LeftAngleDist = -120;
		RightAngleDist = 40;
		LeftRadDist = 0.25;
		LeftHandPitch = -75;
		FirstController->Buttons |= MENU_BTN;
		FirstController->Trigger = 1;
		UseGranade = true;
		GranadeTick = 0;
	}

	//Energy ball (Part 1)
	if (UsedEnergyBall)
	{
		LeftAngleDist = -40;
		LeftRadDist = 0.17;
		UsedEnergyBall = false;
	}
	if ((GetAsyncKeyState(KEY_ID_THROW_ENERGY_BALL) & 0x8000) != 0)
	{
		LeftAngleDist = -70;
		LeftRadDist = 0.17;
		UsedEnergyBall = true;
		FirstController->Trigger = 1;
	}

	//Cover mouth / Прикрыть рот
	if ((GetAsyncKeyState(KEY_ID_COVER_MOUTH) & 0x8000) != 0) CoverMouth = true;
	if ((GetAsyncKeyState(KEY_ID_LEFT_CTRL_MOTION) & 0x8000) != 0 || (GetAsyncKeyState(KEY_ID_CHANGE_WEAPON) & 0x8000) != 0 || (GetAsyncKeyState(KEY_ID_AIMING) & 0x8000) != 0 || (GetAsyncKeyState(KEY_ID_PUT_HAT) & 0x8000) != 0) CoverMouth = false;
	if (CoverMouth)
	{
		LeftHandYaw = 90;
		LeftAngleDist = 4;
		LeftRadDist = 0.34;
	}

	if ((GetAsyncKeyState(KEY_ID_PUT_HAT) & 0x8000) != 0) PutHat = true;
	if ((GetAsyncKeyState(KEY_ID_LEFT_CTRL_MOTION) & 0x8000) != 0 || (GetAsyncKeyState(KEY_ID_CHANGE_WEAPON) & 0x8000) != 0 || (GetAsyncKeyState(KEY_ID_AIMING) & 0x8000) != 0 || (GetAsyncKeyState(KEY_ID_COVER_MOUTH) & 0x8000) != 0) PutHat = false;
	if (PutHat)
	{
		LeftHandYaw = 0;
		LeftAngleDist = -120;
		LeftRadDist = 0.22;
	}

	//Motion left controller on right click mode
	deltaPitch = mouseHMD.Yaw - oldPitch;
	oldPitch = mouseHMD.Yaw;

	deltaRoll = mouseHMD.Pitch - oldRoll;
	oldRoll = mouseHMD.Pitch;
	if ((GetAsyncKeyState(KEY_ID_LEFT_CTRL_MOTION) & 0x8000) != 0)
	{
		MotionLeftRadDist -= deltaRoll * 0.01;
		MotionLeftAngleDist -= deltaPitch;
		if (LeftAngleDist < -180)
			LeftAngleDist = -180;
		if (LeftAngleDist > 180)
			LeftAngleDist = 180;
		if ((GetAsyncKeyState(KEY_ID_MOTION_MODE_MOVE_DOWN) & 0x8000) != 0)
			LeftYOffset -= 0.003;
		if ((GetAsyncKeyState(KEY_ID_MOTION_MODE_MOVE_UP) & 0x8000) != 0)
			LeftYOffset += 0.003;
		LeftHandYaw = 25;
		//if (KEY_ID_LEFT_CTRL_MOTION == VK_RBUTTON) //Нажимаем триггер на левую мышку только если motion режим на правой кнопке, как бы переключение контроллеров
		if ((GetAsyncKeyState(KEY_ID_RIGHT_TRIGGER) & 0x8000) != 0) FirstController->Trigger = 1;
		SecondCtrlYPROffset[1] = 20;
		LeftRadDist = MotionLeftRadDist;
		LeftAngleDist = MotionLeftAngleDist;
	}
	else
	{
		MotionLeftRadDist = 0;
		MotionLeftAngleDist = 0;
		LeftYOffset = 0;
	}

	//Movement controllers
	if ((GetAsyncKeyState(KEY_ID_MOVE_CONTROLLERS_FORWARD) & 0x8000) != 0)
		if (HandRadDist < 0.4)
			HandRadDist += 0.005;
	if ((GetAsyncKeyState(KEY_ID_MOVE_CONTROLLERS_BACK) & 0x8000) != 0)
		if (HandRadDist > -0.4)
			HandRadDist -= 0.005;
	if ((GetAsyncKeyState(KEY_ID_MOVE_CONTROLLERS_FORWARD) & 0x8000) == 0 && (GetAsyncKeyState(KEY_ID_MOVE_CONTROLLERS_BACK) & 0x8000) == 0)
		HandRadDist = 0;

	FirstCtrlPos[0] = (0.4 - LeftRadDist + HandRadDist) * sin(DegToRad(OffsetYPR(OffsetYPR(mouseHMD.Yaw + 180, -17), LeftAngleDist))); //Первый OffsetYPR это расстояние между контроллерами, а второй жесты
	FirstCtrlPos[1] = (0.4 - LeftRadDist + HandRadDist) * cos(DegToRad(OffsetYPR(OffsetYPR(mouseHMD.Yaw + 180, -17), LeftAngleDist)));
	if ((GetAsyncKeyState(KEY_ID_LEFT_CTRL_MOTION) & 0x8000) == 0)
		FirstCtrlPos[2] = mouseHMD.Pitch * ZPosSensetive - CrouchOffsetZ + HMDPosZ;

	//Height coordinates for gestures / Координаты высоты для жестов
	//Backpack / Рюкзак (PART 2)
	if ((GetAsyncKeyState(KEY_ID_BACKPACK) & 0x8000) != 0) //"?"  
	{
		FirstCtrlPos[2] = 0.15 - CrouchOffsetZ + HMDPosZ; //0.1 - just above head / чуть выше головы
	}

	//Granade first state / Первая позиция гранаты (PART 2)
	if ((GetAsyncKeyState(KEY_ID_THROW_ITEMS) & 0x8000) != 0)
	{
		FirstCtrlPos[2] = FirstCtrlPos[2] - 0.15;
	}

	if (CoverMouth) //PART 2
	{
		FirstCtrlPos[2] = 0.12 - CrouchOffsetZ + HMDPosZ;
	}

	if (PutHat) //PART 2
	{
		FirstCtrlPos[2] = 0.35 - CrouchOffsetZ + HMDPosZ; //0.4
	}

	FirstController->X = FirstCtrlPos[0];
	FirstController->Y = FirstCtrlPos[1];
	FirstController->Z = FirstCtrlPos[2] + LeftYOffset - 0.15;

	if (LaserMode)
		ZPosSensetive = 0.006;
	else
		ZPosSensetive = 0.009;

	if ((GetAsyncKeyState(KEY_ID_LEFT_CTRL_MOTION) & 0x8000) == 0)
	{
		SecondCtrlPos[0] = (0.4 - RightRadDist + HandRadDist) * sin(DegToRad(OffsetYPR(OffsetYPR(mouseHMD.Yaw + 180, 17), RightAngleDist)));
		SecondCtrlPos[1] = (0.4 - RightRadDist + HandRadDist) * cos(DegToRad(OffsetYPR(OffsetYPR(mouseHMD.Yaw + 180, 17), RightAngleDist)));
		SecondCtrlPos[2] = mouseHMD.Pitch * ZPosSensetive - CrouchOffsetZ + HMDPosZ;
	}

	SecondController->X = SecondCtrlPos[0];
	SecondController->Y = SecondCtrlPos[1];
	SecondController->Z = SecondCtrlPos[2] - 0.15;

	//Rotation controllers
	FirstController->Roll = CtrlsRoll;
	FirstController->Yaw = OffsetYPR(mouseHMD.Yaw, LeftHandYaw);
	FirstController->Pitch = OffsetYPR(OffsetYPR(OffsetYPR(mouseHMD.Pitch, OffsetPitchByDefault), LeftHandPitch), CtrlsPitch);

	//Backpack / Рюкзак (PART 3)
	if ((GetAsyncKeyState(191) & 0x8000) != 0) {
		FirstController->Roll = 0;
		FirstController->Pitch = -175;
	}

	if (CoverMouth) //Прикрытие рта (PART 3)
		FirstController->Pitch = 45;

	//Put Hat (PART 3)
	if (PutHat) {
		FirstController->Roll = 0; //0
		FirstController->Pitch = 90; //90
	}

	//Energy ball 3
	if ((GetAsyncKeyState(KEY_ID_THROW_ENERGY_BALL) & 0x8000) != 0)
		FirstController->Pitch = -175;
	if (UsedEnergyBall)
		FirstController->Pitch = 160;

	SecondController->Yaw = OffsetYPR(OffsetYPR(mouseHMD.Yaw, SecondCtrlYPROffset[0]), RightHandYaw);
	SecondController->Pitch = OffsetYPR(OffsetYPR(OffsetYPR(OffsetYPR(mouseHMD.Pitch, OffsetPitchByDefault), CtrlsPitch), SecondCtrlYPROffset[1]), RightHandPitch);
	SecondController->Roll = CtrlsRoll;

	//Right hand mode
	if ((GetAsyncKeyState(KEY_ID_LEFT_CTRL_MOTION) & 0x8000) == 0) {

		//SecondController->Roll = OffsetYPR(CtrlYaw, SecondCtrlYPROffset[2]);

		SecondCtrlLastYPR[0] = SecondController->Yaw;
		SecondCtrlLastYPR[1] = SecondController->Pitch;
		SecondCtrlLastYPR[2] = SecondController->Roll;
	}
	else {
		SecondController->Yaw = OffsetYPR(SecondCtrlLastYPR[0], SecondCtrlYPROffset[0]);
		SecondController->Pitch = OffsetYPR(SecondCtrlLastYPR[1], SecondCtrlYPROffset[1]);
		SecondController->Roll = OffsetYPR(SecondCtrlLastYPR[2], SecondCtrlYPROffset[2]);
	}

	//Fix left controller (chapter 3: shotgun, chapter 5: Northern Star)
	if ((GetAsyncKeyState(KEY_ID_FIXING_LEFT_CONTROLLER) & 0x8000) != 0) { //Key "
		FirstController->Yaw = FirstCtrlLastYPR[0];
		FirstController->Pitch = FirstCtrlLastYPR[1];
		FirstController->Roll = FirstCtrlLastYPR[2];

		FirstController->X = FirstCtrlLastXYZ[0];
		FirstController->Y = FirstCtrlLastXYZ[1];
		FirstController->Z = FirstCtrlLastXYZ[2];
	}
	else {
		//Save last rotation and position for fixing left controller
		FirstCtrlLastYPR[0] = FirstController->Yaw;
		FirstCtrlLastYPR[1] = FirstController->Pitch;
		FirstCtrlLastYPR[2] = FirstController->Roll;

		FirstCtrlLastXYZ[0] = FirstController->X;
		FirstCtrlLastXYZ[1] = FirstController->Y;
		FirstCtrlLastXYZ[2] = FirstController->Z;
	}

	//Aiming mode
	if ((GetAsyncKeyState(KEY_ID_AIMING_MODE) & 0x8000) != 0) Aiming = true;
	if ((GetAsyncKeyState(KEY_ID_LEFT_CTRL_MOTION) & 0x8000) != 0 || (GetAsyncKeyState(KEY_ID_CHANGE_WEAPON) & 0x8000) != 0 || (GetAsyncKeyState(KEY_ID_AIMING) & 0x8000) != 0) Aiming = false;
	if (Aiming || (GetAsyncKeyState(KEY_ID_AIMING) & 0x8000) != 0) {
		double a = DegToRad(OffsetYPR(mouseHMD.Yaw, -97) * -1); //-98
		if (VRMode)
			a = DegToRad( OffsetYPR( OffsetYPR(mouseHMD.Yaw, -91), VRAimingAngleOffset) * -1);

		double b = DegToRad(OffsetYPR(mouseHMD.Pitch, 90) * -1);

		SecondController->X = 0.30 * cos(a) * sin(b);
		SecondController->Y = 0.30 * sin(a) * sin(b);
		SecondController->Z = 0.30 * cos(b * -1) - 0.1 - CrouchOffsetZ + HMDPosZ;

		SecondController->Yaw = mouseHMD.Yaw;
		if (VRMode)
			SecondController->Yaw = OffsetYPR(SecondController->Yaw, -6);
		SecondController->Pitch = OffsetYPR(SecondCtrlLastYPR[1] * 1.1, 5);
		if (VRMode)
			SecondController->Pitch = OffsetYPR(SecondController->Pitch, 2);

		//First hand under weapon / Первая рука под оружием
		FirstController->X = 0.22 * cos(a) * sin(b); //0.18
		FirstController->Y = 0.22 * sin(a) * sin(b); //0.18
		FirstController->Z = SecondController->Z - 0.10;
	}

	//Lasers mode
	if ((GetAsyncKeyState(KEY_ID_LASER_MODE) & 0x8000) != 0) LaserMode = true;
	if ((GetAsyncKeyState(KEY_ID_LEFT_CTRL_MOTION) & 0x8000) != 0 || (GetAsyncKeyState(KEY_ID_CHANGE_WEAPON) & 0x8000) != 0 || (GetAsyncKeyState(KEY_ID_AIMING) & 0x8000) != 0) LaserMode = false;
	if (LaserMode) {
		double a = DegToRad(OffsetYPR(mouseHMD.Yaw, -97) * -1); //-98
		if (VRMode)
			a = DegToRad(OffsetYPR(mouseHMD.Yaw, -91) * -1);
		double b = DegToRad(OffsetYPR(mouseHMD.Pitch, 90) * -1);

		if ((GetAsyncKeyState(KEY_ID_UP) & 0x8000) != 0) LaserPos += 0.0013;
		if ((GetAsyncKeyState(KEY_ID_DOWN) & 0x8000) != 0) LaserPos -= 0.0013;
		if ((GetAsyncKeyState(KEY_ID_LEFT) & 0x8000) != 0) LaserYaw += 2;
		if ((GetAsyncKeyState(KEY_ID_RIGHT) & 0x8000) != 0) LaserYaw -= 2;
		if ((GetAsyncKeyState(KEY_ID_TURN_LEFT) & 0x8000) != 0) LaserYaw += 2;
		if ((GetAsyncKeyState(KEY_ID_TURN_RIGHT) & 0x8000) != 0) LaserYaw -= 2;
		if (LaserYaw < -180)
			LaserYaw += 360;
		else if (LaserYaw > 180)
			LaserYaw -= 360;

		SecondController->X = LaserPos * cos(a) * sin(b);
		SecondController->Y = LaserPos * sin(a) * sin(b);

		SecondController->Yaw = OffsetYPR(OffsetYPR(mouseHMD.Yaw, 0), LaserYaw);
		if (VRMode)
			SecondController->Yaw = OffsetYPR(SecondController->Yaw, -6);
		SecondController->Pitch = OffsetYPR(SecondCtrlLastYPR[1] * 1.1, 5);

		//First hand under weapon / Первая рука под оружием
		FirstController->X = 0.05 * cos(a) * sin(b);
		FirstController->Y = 0.05 * sin(a) * sin(b);
		FirstController->Z = 0.05 * cos(b * -1) - 0.3 - CrouchOffsetZ + HMDPosZ;
	}
	else {
		LaserPos = 0.3;
		LaserYaw = 0;
	}

	//Buttons
	if ((GetAsyncKeyState(KEY_ID_LEFT_GRIP) & 0x8000) != 0) FirstController->Buttons |= GRIP_BTN;
	if ((GetAsyncKeyState(KEY_ID_LEFT_MENU) & 0x8000) != 0) FirstController->Buttons |= MENU_BTN;
	if (UseGranade == false) //Гранаты
		if ((GetAsyncKeyState(KEY_ID_LEFT_TRIGGER) & 0x8000) != 0) FirstController->Trigger = 1;
	if ((GetAsyncKeyState(KEY_ID_LEFT_SYSTEM) & 0x8000) != 0) FirstController->Buttons |= SYS_BTN;

	//Movement
	if (LaserMode == false) //Don't move in laser mode / Не двигаемся в режиме лазера
	{
		if ((GetAsyncKeyState(KEY_ID_UP) & 0x8000) != 0) { FirstController->AxisY = 1; FirstController->Buttons |= THUMB_BTN; }
		if ((GetAsyncKeyState(KEY_ID_DOWN) & 0x8000) != 0) { FirstController->AxisY = -1; FirstController->Buttons |= THUMB_BTN; }
		if ((GetAsyncKeyState(KEY_ID_LEFT) & 0x8000) != 0) { FirstController->AxisX = -1; FirstController->Buttons |= THUMB_BTN; }
		if ((GetAsyncKeyState(KEY_ID_RIGHT) & 0x8000) != 0) { FirstController->AxisX = 1; FirstController->Buttons |= THUMB_BTN; }

		if ((GetAsyncKeyState(KEY_ID_TURN_LEFT) & 0x8000) != 0) { SecondController->AxisX = -1; SecondController->Buttons |= THUMB_BTN; }
		if ((GetAsyncKeyState(KEY_ID_TURN_RIGHT) & 0x8000) != 0) { SecondController->AxisX = 1; SecondController->Buttons |= THUMB_BTN; }
	}

	if ((GetAsyncKeyState(KEY_ID_RIGHT_MENU) & 0x8000) != 0) SecondController->Buttons |= MENU_BTN;
	if ((GetAsyncKeyState(KEY_ID_RIGHT_GRIP) & 0x8000) != 0) SecondController->Buttons |= GRIP_BTN;
	if ((GetAsyncKeyState(KEY_ID_RIGHT_TRIGGER2) & 0x8000) != 0) SecondController->Trigger = 1;

	//Teleport
	if ((GetAsyncKeyState(KEY_ID_TELEPORT) & 0x8000) != 0) {
		SecondController->AxisY = -1;
		SecondController->Buttons |= THUMB_BTN;
	}
	//Select weapon
	if ((GetAsyncKeyState(KEY_ID_LEFT_CTRL_MOTION) & 0x8000) == 0) { //Only in right controller mode
		if ((GetAsyncKeyState(KEY_ID_CHANGE_WEAPON) & 0x8000) != 0) {
			SecondController->AxisY = 1;
			SecondController->Buttons |= THUMB_BTN;
		}
		if ((GetAsyncKeyState(KEY_ID_RIGHT_TRIGGER) & 0x8000) != 0) SecondController->Trigger = 1;
	}

	if (HeadsetWindowFocused == false && HalfLifeAlyxFocused == false) //Don't press buttons when window is not focused / Не нажимать кнопки когда окно не в фокусе
	{
		FirstController->Buttons = 0;
		FirstController->Trigger = 0;
		FirstController->AxisX = 0;
		FirstController->AxisY = 0;
		SecondController->Buttons = 0;
		SecondController->Trigger = 0;
		SecondController->AxisX = 0;
		SecondController->AxisY = 0;
	}

	//New version of SteamVR driver with correct axes / Новая версия SteamVR драйвер, с правильными осями
	FirstController->Yaw = FirstController->Yaw * -1;
	FirstController->Pitch = FirstController->Pitch * -1;

	SecondController->Yaw = SecondController->Yaw * -1;
	SecondController->Pitch = SecondController->Pitch * -1;
}


inline vr::HmdQuaternion_t EulerAngleToQuaternion(double Yaw, double Pitch, double Roll)
{
	vr::HmdQuaternion_t q;
	// Abbreviations for the various angular functions
	double cy = cos(Yaw * 0.5);
	double sy = sin(Yaw * 0.5);
	double cp = cos(Pitch * 0.5);
	double sp = sin(Pitch * 0.5);
	double cr = cos(Roll * 0.5);
	double sr = sin(Roll * 0.5);

	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CSampleDeviceDriver : public vr::ITrackedDeviceServerDriver, public vr::IVRDisplayComponent
{
public:
	CSampleDeviceDriver(  )
	{
		HMDIndex_t = vr::k_unTrackedDeviceIndexInvalid;
		m_ulPropertyContainer = vr::k_ulInvalidPropertyContainer;

		//DriverLog( "Using settings values\n" );
		m_flIPD = vr::VRSettings()->GetFloat( k_pch_SteamVR_Section, k_pch_SteamVR_IPD_Float );

		char buf[1024];
		vr::VRSettings()->GetString( k_pch_Sample_Section, k_pch_Sample_SerialNumber_String, buf, sizeof( buf ) );
		m_sSerialNumber = buf;

		vr::VRSettings()->GetString( k_pch_Sample_Section, k_pch_Sample_ModelNumber_String, buf, sizeof( buf ) );
		m_sModelNumber = buf;

		m_nWindowX = vr::VRSettings()->GetInt32( k_pch_Sample_Section, k_pch_Sample_WindowX_Int32 );
		m_nWindowY = vr::VRSettings()->GetInt32( k_pch_Sample_Section, k_pch_Sample_WindowY_Int32 );
		m_nWindowWidth = vr::VRSettings()->GetInt32( k_pch_Sample_Section, k_pch_Sample_WindowWidth_Int32 );
		m_nWindowHeight = vr::VRSettings()->GetInt32( k_pch_Sample_Section, k_pch_Sample_WindowHeight_Int32 );

		if (m_nWindowWidth == 0)
			m_nWindowWidth = GetSystemMetrics(SM_CXSCREEN);
		if (m_nWindowHeight == 0)
			m_nWindowHeight = GetSystemMetrics(SM_CYSCREEN);

		m_nRenderWidth = vr::VRSettings()->GetInt32( k_pch_Sample_Section, k_pch_Sample_RenderWidth_Int32 );
		m_nRenderHeight = vr::VRSettings()->GetInt32( k_pch_Sample_Section, k_pch_Sample_RenderHeight_Int32 );

		m_flSecondsFromVsyncToPhotons = vr::VRSettings()->GetFloat( k_pch_Sample_Section, k_pch_Sample_SecondsFromVsyncToPhotons_Float );
		m_flDisplayFrequency = vr::VRSettings()->GetFloat( k_pch_Sample_Section, k_pch_Sample_DisplayFrequency_Float );

		m_fZoomWidth = vr::VRSettings()->GetFloat(k_pch_Sample_Section, k_pch_Sample_ZoomWidth_Float);
		m_fZoomHeight = vr::VRSettings()->GetFloat(k_pch_Sample_Section, k_pch_Sample_ZoomHeight_Float);
		m_fFOV = (vr::VRSettings()->GetFloat(k_pch_Sample_Section, k_pch_Sample_FOV_Float) * 3.14159265358979323846 / 180); //radians
		m_bStereoMode = vr::VRSettings()->GetBool(k_pch_Sample_Section, k_pch_Sample_Stereo_Bool);
		m_bDebugMode = vr::VRSettings()->GetBool(k_pch_Sample_Section, k_pch_Sample_DebugMode_Bool);

		/*DriverLog( "driver_null: Serial Number: %s\n", m_sSerialNumber.c_str() );
		DriverLog( "driver_null: Model Number: %s\n", m_sModelNumber.c_str() );
		DriverLog( "driver_null: Window: %d %d %d %d\n", m_nWindowX, m_nWindowY, m_nWindowWidth, m_nWindowHeight );
		DriverLog( "driver_null: Render Target: %d %d\n", m_nRenderWidth, m_nRenderHeight );
		DriverLog( "driver_null: Seconds from Vsync to Photons: %f\n", m_flSecondsFromVsyncToPhotons );
		DriverLog( "driver_null: Display Frequency: %f\n", m_flDisplayFrequency );
		DriverLog( "driver_null: IPD: %f\n", m_flIPD );*/

	}

	virtual ~CSampleDeviceDriver()
	{
	}


	virtual EVRInitError Activate( vr::TrackedDeviceIndex_t unObjectId ) 
	{
		HMDIndex_t = unObjectId;
		m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer( HMDIndex_t );

		vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, Prop_ModelNumber_String, m_sModelNumber.c_str() );
		vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, Prop_RenderModelName_String, m_sModelNumber.c_str() );
		vr::VRProperties()->SetFloatProperty( m_ulPropertyContainer, Prop_UserIpdMeters_Float, m_flIPD );
		vr::VRProperties()->SetFloatProperty( m_ulPropertyContainer, Prop_UserHeadToEyeDepthMeters_Float, 0.f );
		vr::VRProperties()->SetFloatProperty( m_ulPropertyContainer, Prop_DisplayFrequency_Float, m_flDisplayFrequency );
		vr::VRProperties()->SetFloatProperty( m_ulPropertyContainer, Prop_SecondsFromVsyncToPhotons_Float, m_flSecondsFromVsyncToPhotons );

		// return a constant that's not 0 (invalid) or 1 (reserved for Oculus)
		vr::VRProperties()->SetUint64Property( m_ulPropertyContainer, Prop_CurrentUniverseId_Uint64, 2 );

		// avoid "not fullscreen" warnings from vrmonitor
		vr::VRProperties()->SetBoolProperty( m_ulPropertyContainer, Prop_IsOnDesktop_Bool, false );

		//Debug mode activate Windowed Mode (borderless fullscreen), lock to 30 FPS 
		vr::VRProperties()->SetBoolProperty(m_ulPropertyContainer, Prop_DisplayDebugMode_Bool, m_bDebugMode);

		// Icons can be configured in code or automatically configured by an external file "drivername\resources\driver.vrresources".
		// Icon properties NOT configured in code (post Activate) are then auto-configured by the optional presence of a driver's "drivername\resources\driver.vrresources".
		// In this manner a driver can configure their icons in a flexible data driven fashion by using an external file.
		//
		// The structure of the driver.vrresources file allows a driver to specialize their icons based on their HW.
		// Keys matching the value in "Prop_ModelNumber_String" are considered first, since the driver may have model specific icons.
		// An absence of a matching "Prop_ModelNumber_String" then considers the ETrackedDeviceClass ("HMD", "Controller", "GenericTracker", "TrackingReference")
		// since the driver may have specialized icons based on those device class names.
		//
		// An absence of either then falls back to the "system.vrresources" where generic device class icons are then supplied.
		//
		// Please refer to "bin\drivers\sample\resources\driver.vrresources" which contains this sample configuration.
		//
		// "Alias" is a reserved key and specifies chaining to another json block.
		//
		// In this sample configuration file (overly complex FOR EXAMPLE PURPOSES ONLY)....
		//
		// "Model-v2.0" chains through the alias to "Model-v1.0" which chains through the alias to "Model-v Defaults".
		//
		// Keys NOT found in "Model-v2.0" would then chase through the "Alias" to be resolved in "Model-v1.0" and either resolve their or continue through the alias.
		// Thus "Prop_NamedIconPathDeviceAlertLow_String" in each model's block represent a specialization specific for that "model".
		// Keys in "Model-v Defaults" are an example of mapping to the same states, and here all map to "Prop_NamedIconPathDeviceOff_String".
		//
		/*bool bSetupIconUsingExternalResourceFile = true;
		if ( !bSetupIconUsingExternalResourceFile )
		{
			// Setup properties directly in code.
			// Path values are of the form {drivername}\icons\some_icon_filename.png
			vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceOff_String, "{null}/icons/headset_sample_status_off.png" );
			vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceSearching_String, "{null}/icons/headset_sample_status_searching.gif" );
			vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceSearchingAlert_String, "{null}/icons/headset_sample_status_searching_alert.gif" );
			vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceReady_String, "{null}/icons/headset_sample_status_ready.png" );
			vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceReadyAlert_String, "{null}/icons/headset_sample_status_ready_alert.png" );
			vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceNotReady_String, "{null}/icons/headset_sample_status_error.png" );
			vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceStandby_String, "{null}/icons/headset_sample_status_standby.png" );
			vr::VRProperties()->SetStringProperty( m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceAlertLow_String, "{null}/icons/headset_sample_status_ready_low.png" );
		}*/

		return VRInitError_None;
	}

	virtual void Deactivate() 
	{
		HMDIndex_t = vr::k_unTrackedDeviceIndexInvalid;
	}

	virtual void EnterStandby()
	{
	}

	void *GetComponent( const char *pchComponentNameAndVersion )
	{
		if ( !_stricmp( pchComponentNameAndVersion, vr::IVRDisplayComponent_Version ) )
		{
			return (vr::IVRDisplayComponent*)this;
		}

		// override this to add a component to a driver
		return NULL;
	}

	virtual void PowerOff() 
	{
	}

	/** debug request from a client */
	virtual void DebugRequest( const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize ) 
	{
		if( unResponseBufferSize >= 1 )
			pchResponseBuffer[0] = 0;
	}

	virtual void GetWindowBounds( int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) 
	{
		*pnX = m_nWindowX;
		*pnY = m_nWindowY;
		*pnWidth = m_nWindowWidth;
		*pnHeight = m_nWindowHeight;
	}

	virtual bool IsDisplayOnDesktop() 
	{
		return true;
	}

	virtual bool IsDisplayRealDisplay() 
	{
		return true;
	}

	virtual void GetRecommendedRenderTargetSize( uint32_t *pnWidth, uint32_t *pnHeight ) 
	{
		*pnWidth = m_nRenderWidth;
		*pnHeight = m_nRenderHeight;
	}

	virtual void GetEyeOutputViewport( EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) 
	{
		/**pnY = 0;
				*pnWidth = m_nWindowWidth / 2;
				*pnHeight = m_nWindowHeight;

				if ( eEye == Eye_Left )
				{
					*pnX = 0;
				}
				else
				{
					*pnX = m_nWindowWidth / 2;
				}*/

		if (m_bStereoMode) {

			*pnY = 0;
			*pnWidth = m_nWindowWidth / 2;
			*pnHeight = m_nWindowHeight;

			if (eEye == Eye_Left)
			{
				*pnX = 0;
			}
			else
			{
				*pnX = (m_nWindowWidth / 2);
			}
		}
		else { //Mono mode
			pnY = 0;
			*pnWidth = m_nRenderWidth;
			*pnHeight = m_nRenderHeight;

			if (eEye == Eye_Left)
			{
				*pnX = (m_nWindowWidth - m_nRenderWidth) / 2;
			}
			else
			{
				*pnX = m_nWindowWidth;
			}


		}
	}

	virtual void GetProjectionRaw( EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom ) 
	{
		if (m_bStereoMode) {
			
			*pfLeft = -m_fFOV;
			*pfRight = m_fFOV;
			*pfTop = -m_fFOV;
			*pfBottom = m_fFOV;
		}
		else { //Mono
			*pfLeft = (float)m_nRenderWidth / m_nRenderHeight * -1;
			*pfRight = (float)m_nRenderWidth / m_nRenderHeight;
			*pfTop = -1.0;
			*pfBottom = 1.0;
		}
	}

	virtual DistortionCoordinates_t ComputeDistortion( EVREye eEye, float fU, float fV ) 
	{
		DistortionCoordinates_t coordinates;

		//distortion for lens from https://github.com/HelenXR/openvr_survivor/blob/master/src/head_mount_display_device.cc
		float hX;
		float hY;
		double rr;
		double r2;
		double theta;

		rr = sqrt((fU - 0.5f)*(fU - 0.5f) + (fV - 0.5f)*(fV - 0.5f));
		r2 = rr * (1 + 0 * (rr*rr) + 0 * (rr*rr*rr*rr));
		theta = atan2(fU - 0.5f, fV - 0.5f);
		hX = sin(theta)*r2*m_fZoomWidth;
		hY = cos(theta)*r2*m_fZoomHeight;

		coordinates.rfBlue[0] = hX + 0.5f;
		coordinates.rfBlue[1] = hY + 0.5f;
		coordinates.rfGreen[0] = hX + 0.5f;
		coordinates.rfGreen[1] = hY + 0.5f;
		coordinates.rfRed[0] = hX + 0.5f;
		coordinates.rfRed[1] = hY + 0.5f;

		return coordinates;
	}

	virtual DriverPose_t GetPose() 
	{
		DriverPose_t pose = { 0 };

		if (HMDConnected) {
			pose.poseIsValid = true;
			pose.result = TrackingResult_Running_OK;
			pose.deviceIsConnected = true;
		}
		else
		{
			pose.poseIsValid = false;
			pose.result = TrackingResult_Uninitialized;
			pose.deviceIsConnected = false;
		}

		pose.qWorldFromDriverRotation = HmdQuaternion_Init(1, 0, 0, 0);
		pose.qDriverFromHeadRotation = HmdQuaternion_Init(1, 0, 0, 0);

		if (HMDConnected) {

			//TrueOpenVR data
			GetHMDData(&MyHMD);

			//Set head tracking rotation
			pose.qRotation = EulerAngleToQuaternion(DegToRad(MyHMD.Roll), DegToRad(-MyHMD.Yaw), DegToRad(-MyHMD.Pitch));

			//Set head position tracking
			pose.vecPosition[0] = MyHMD.X;
			pose.vecPosition[1] = MyHMD.Z;
			pose.vecPosition[2] = MyHMD.Y;
		}

		return pose;
	}
	

	void RunFrame()
	{
		// In a real driver, this should happen from some pose tracking thread.
		// The RunFrame interval is unspecified and can be very irregular if some other
		// driver blocks it for some periodic task.
		if ( HMDIndex_t != vr::k_unTrackedDeviceIndexInvalid )
		{
			vr::VRServerDriverHost()->TrackedDevicePoseUpdated( HMDIndex_t , GetPose(), sizeof( DriverPose_t ) );
		}
	}

	std::string GetSerialNumber() const { return m_sSerialNumber; }

private:
	vr::TrackedDeviceIndex_t HMDIndex_t;
	vr::PropertyContainerHandle_t m_ulPropertyContainer;

	std::string m_sSerialNumber;
	std::string m_sModelNumber;

	int32_t m_nWindowX;
	int32_t m_nWindowY;
	int32_t m_nWindowWidth;
	int32_t m_nWindowHeight;
	int32_t m_nRenderWidth;
	int32_t m_nRenderHeight;
	float m_flSecondsFromVsyncToPhotons;
	float m_flDisplayFrequency;
	float m_flIPD;
	float m_fZoomWidth;
	float m_fZoomHeight;
	float m_fFOV;
	bool m_bStereoMode;
	bool m_bDebugMode;
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------

class CSampleControllerDriver : public vr::ITrackedDeviceServerDriver
{
	int32_t ControllerIndex;
public:
	CSampleControllerDriver()
	{
		Ctrl1Index_t = vr::k_unTrackedDeviceIndexInvalid;
		Ctrl2Index_t = vr::k_unTrackedDeviceIndexInvalid;
		m_ulPropertyContainer = vr::k_ulInvalidPropertyContainer;
	}

	virtual void SetControllerIndex(int32_t CtrlIndex)
	{
		ControllerIndex = CtrlIndex;
	}

	virtual ~CSampleControllerDriver()
	{
	}

	virtual EVRInitError Activate( vr::TrackedDeviceIndex_t unObjectId )
	{

		switch (ControllerIndex)
		{
		case 1:
			Ctrl1Index_t = unObjectId;
			m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(Ctrl1Index_t);
			vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_SerialNumber_String, "CTRL1Serial");
			vr::VRProperties()->SetInt32Property(m_ulPropertyContainer, Prop_ControllerRoleHint_Int32, TrackedControllerRole_LeftHand);
			break;
		case 2:
			Ctrl2Index_t = unObjectId;
			m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(Ctrl2Index_t);
			vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_SerialNumber_String, "CTRL2Serial");
			vr::VRProperties()->SetInt32Property(m_ulPropertyContainer, Prop_ControllerRoleHint_Int32, TrackedControllerRole_RightHand);
			break;
		}

		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_ControllerType_String, "vive_controller");

		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_ModelNumber_String, "ViveMV");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_ManufacturerName_String, "HTC");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_RenderModelName_String, "vr_controller_vive_1_5");

		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_TrackingSystemName_String, "VR Controller");

		uint64_t supportedButtons = 0xA4;
		vr::VRProperties()->SetUint64Property(m_ulPropertyContainer, vr::Prop_SupportedButtons_Uint64, supportedButtons);

		// avoid "not fullscreen" warnings from vrmonitor
		vr::VRProperties()->SetBoolProperty(m_ulPropertyContainer, Prop_IsOnDesktop_Bool, false);

		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_InputProfilePath_String, "{hlalyx}/input/controller_profile.json");

		/*bool bSetupIconUsingExternalResourceFile = false;
		if (!bSetupIconUsingExternalResourceFile)
		{
			// Setup properties directly in code.
			// Path values are of the form {drivername}\icons\some_icon_filename.png
			vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceOff_String, "{hlalyx}/icons/barebones_status_off.png");
			vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceReady_String, "{hlalyx}/icons/barebones_status_ready.png");
			vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceReadyAlert_String, "{hlalyx}/icons/barebones_status_ready_alert.png");
			vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceNotReady_String, "{hlalyx}/icons/barebones_status_error.png");
			vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceStandby_String, "{hlalyx}/icons/barebones_status_standby.png");
			vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceAlertLow_String, "{hlalyx}/icons/barebones_status_ready_low.png");
		}*/

		vr::VRDriverInput()->CreateScalarComponent( m_ulPropertyContainer, "/input/trackpad/x", &HAnalog[0], vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedTwoSided);
		vr::VRDriverInput()->CreateScalarComponent(m_ulPropertyContainer, "/input/trackpad/y", &HAnalog[1], vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedTwoSided);
		vr::VRDriverInput()->CreateScalarComponent(m_ulPropertyContainer, "/input/trigger/value", &HAnalog[2], vr::EVRScalarType::VRScalarType_Absolute, vr::EVRScalarUnits::VRScalarUnits_NormalizedOneSided);

		//  Buttons handles
		vr::VRDriverInput()->CreateBooleanComponent(m_ulPropertyContainer, "/input/application_menu/click", &HButtons[0]);
		vr::VRDriverInput()->CreateBooleanComponent(m_ulPropertyContainer, "/input/system/click", &HButtons[2]);

		vr::VRDriverInput()->CreateBooleanComponent(m_ulPropertyContainer, "/input/grip/click", &HButtons[1]);
		vr::VRDriverInput()->CreateBooleanComponent(m_ulPropertyContainer, "/input/trigger/click", &HButtons[5]);

		vr::VRDriverInput()->CreateBooleanComponent(m_ulPropertyContainer, "/input/trackpad/click", &HButtons[3]);
		vr::VRDriverInput()->CreateBooleanComponent(m_ulPropertyContainer, "/input/trackpad/touch", &HButtons[4]);

		// create our haptic component
		vr::VRDriverInput()->CreateHapticComponent(m_ulPropertyContainer, "/output/haptic", &m_compHaptic);

		return VRInitError_None;
	}

	virtual void Deactivate()
	{
		switch (ControllerIndex)
		{
		case 1:
			Ctrl1Index_t = vr::k_unTrackedDeviceIndexInvalid;
			break;
		case 2:
			Ctrl2Index_t = vr::k_unTrackedDeviceIndexInvalid;
			break;
		}
	}


	virtual void EnterStandby()
	{
	}

	void *GetComponent( const char *pchComponentNameAndVersion )
	{
		// override this to add a component to a driver
		return NULL;
	}

	virtual void PowerOff()
	{
	}

	/** debug request from a client */
	virtual void DebugRequest( const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize )
	{
		if ( unResponseBufferSize >= 1 )
			pchResponseBuffer[0] = 0;
	}

	virtual DriverPose_t GetPose()
	{
		DriverPose_t pose = { 0 };
		//pose.poseIsValid = false;
		pose.poseIsValid = true;
		//pose.result = TrackingResult_Calibrating_OutOfRange;
		pose.result = TrackingResult_Running_OK;
		pose.deviceIsConnected = true;

		pose.qWorldFromDriverRotation = HmdQuaternion_Init( 1, 0, 0, 0 );
		pose.qDriverFromHeadRotation = HmdQuaternion_Init( 1, 0, 0, 0 );

		//Controllers positions and rotations
		if (ControllerIndex == 1) {

			pose.vecPosition[0] = MyCtrl.X;
			pose.vecPosition[1] = MyCtrl.Z;
			pose.vecPosition[2] = MyCtrl.Y;

			//Velocity, right?
			pose.vecVelocity[0] = (pose.vecPosition[0] - FirstCtrlLastPos[0]) * 1000 / max((int)deltaTime.count(), 1) / 3; // div 3 - ghosting fix, there are right ways to remove ghosting?
			pose.vecVelocity[1] = (pose.vecPosition[1] - FirstCtrlLastPos[1]) * 1000 / max((int)deltaTime.count(), 1) / 3;
			pose.vecVelocity[2] = (pose.vecPosition[2] - FirstCtrlLastPos[2]) * 1000 / max((int)deltaTime.count(), 1) / 3;
			FirstCtrlLastPos[0] = pose.vecPosition[0];
			FirstCtrlLastPos[1] = pose.vecPosition[1];
			FirstCtrlLastPos[2] = pose.vecPosition[2];

			//Rotation first controller
			pose.qRotation = EulerAngleToQuaternion(DegToRad(MyCtrl.Roll), DegToRad(-MyCtrl.Yaw), DegToRad(-MyCtrl.Pitch));

		} else { 
			//Controller2
			pose.vecPosition[0] = MyCtrl2.X;
			pose.vecPosition[1] = MyCtrl2.Z;
			pose.vecPosition[2] = MyCtrl2.Y;

			//Velocity
			pose.vecVelocity[0] = (pose.vecPosition[0] - SecondCtrlLastPos[0]) * 1000 / max((int)deltaTime.count(), 1) / 3; 
			pose.vecVelocity[1] = (pose.vecPosition[1] - SecondCtrlLastPos[1]) * 1000 / max((int)deltaTime.count(), 1) / 3;
			pose.vecVelocity[2] = (pose.vecPosition[2] - SecondCtrlLastPos[2]) * 1000 / max((int)deltaTime.count(), 1) / 3;
			SecondCtrlLastPos[0] = pose.vecPosition[0];
			SecondCtrlLastPos[1] = pose.vecPosition[1];
			SecondCtrlLastPos[2] = pose.vecPosition[2];

			pose.qRotation = EulerAngleToQuaternion(DegToRad(MyCtrl2.Roll), DegToRad(-MyCtrl2.Yaw), DegToRad(-MyCtrl2.Pitch));
		}

		return pose;
	}

	void RunFrame()
	{
		switch (ControllerIndex)
		{
		case 1:
			if (Ctrl1Index_t != vr::k_unTrackedDeviceIndexInvalid)
			{
				vr::VRServerDriverHost()->TrackedDevicePoseUpdated(Ctrl1Index_t, GetPose(), sizeof(DriverPose_t));
			}
			break;
		case 2:
			if (Ctrl2Index_t != vr::k_unTrackedDeviceIndexInvalid)
			{
				vr::VRServerDriverHost()->TrackedDevicePoseUpdated(Ctrl2Index_t, GetPose(), sizeof(DriverPose_t));
			}
			break;
		}

		// Your driver would read whatever hardware state is associated with its input components and pass that
		// in to UpdateBooleanComponent. This could happen in RunFrame or on a thread of your own that's reading USB
		// state. There's no need to update input state unless it changes, but it doesn't do any harm to do so.
		
		if (ControllerIndex == 1) {

			vr::VRDriverInput()->UpdateBooleanComponent(HButtons[0], (MyCtrl.Buttons & MENU_BTN) != 0, 0); //Application Menu
			vr::VRDriverInput()->UpdateBooleanComponent(HButtons[1], (MyCtrl.Buttons & GRIP_BTN) != 0, 0); //Grip
			vr::VRDriverInput()->UpdateBooleanComponent(HButtons[2], (MyCtrl.Buttons & SYS_BTN) != 0, 0); //System
			vr::VRDriverInput()->UpdateBooleanComponent(HButtons[3], (MyCtrl.Buttons & THUMB_BTN) != 0, 0); //Trackpad

			vr::VRDriverInput()->UpdateScalarComponent(HAnalog[0], MyCtrl.AxisX, 0); //Trackpad x
			vr::VRDriverInput()->UpdateScalarComponent(HAnalog[1], MyCtrl.AxisY, 0); //Trackpad y

			if (MyCtrl.AxisX != 0 || MyCtrl.AxisY != 0) {
				vr::VRDriverInput()->UpdateBooleanComponent(HButtons[4], 1, 0); //Trackpad touch
			}
			else {
				vr::VRDriverInput()->UpdateBooleanComponent(HButtons[4], 0, 0); //Trackpad touch
			}

			vr::VRDriverInput()->UpdateScalarComponent(HAnalog[2], MyCtrl.Trigger, 0); //Trigger
		} else {
			//Controller2
			vr::VRDriverInput()->UpdateBooleanComponent(HButtons[0], (MyCtrl2.Buttons & MENU_BTN) != 0, 0); //Application Menu
			vr::VRDriverInput()->UpdateBooleanComponent(HButtons[1], (MyCtrl2.Buttons & GRIP_BTN) != 0, 0); //Grip
			vr::VRDriverInput()->UpdateBooleanComponent(HButtons[2], (MyCtrl2.Buttons & SYS_BTN) != 0, 0); //System
			vr::VRDriverInput()->UpdateBooleanComponent(HButtons[3], (MyCtrl2.Buttons & THUMB_BTN) != 0, 0); //Trackpad

			vr::VRDriverInput()->UpdateScalarComponent(HAnalog[0], MyCtrl2.AxisX, 0); //Trackpad x
			vr::VRDriverInput()->UpdateScalarComponent(HAnalog[1], MyCtrl2.AxisY, 0); //Trackpad y

			if (MyCtrl2.AxisX != 0 || MyCtrl2.AxisY != 0) {
				vr::VRDriverInput()->UpdateBooleanComponent(HButtons[4], 1, 0); //Trackpad touch
			}
			else {
				vr::VRDriverInput()->UpdateBooleanComponent(HButtons[4], 0, 0); //Trackpad touch
			}

			vr::VRDriverInput()->UpdateScalarComponent(HAnalog[2], MyCtrl2.Trigger, 0); //Trigger
		}

	}

	void ProcessEvent( const vr::VREvent_t & vrEvent )
	{
		switch ( vrEvent.eventType )
		{
		case vr::VREvent_Input_HapticVibration:
		{
			if ( vrEvent.data.hapticVibration.componentHandle == m_compHaptic )
			{
				//SetControllerData(ControllerIndex, 100); 
				// This is where you would send a signal to your hardware to trigger actual haptic feedback
			}
		}
		break;
		}
	}

	std::string GetSerialNumber() const { 
		
		switch (ControllerIndex)
		{
		case 1:
			return "CTRL1Serial";
			break;
		case 2:
			return "CTRL2Serial";
			break;
		}
	}

private:
	vr::TrackedDeviceIndex_t Ctrl1Index_t;
	vr::TrackedDeviceIndex_t Ctrl2Index_t;
	vr::PropertyContainerHandle_t m_ulPropertyContainer;

	//vr::VRInputComponentHandle_t m_compA;
	//vr::VRInputComponentHandle_t m_compB;
	//vr::VRInputComponentHandle_t m_compC;
	vr::VRInputComponentHandle_t m_compHaptic;

	vr::VRInputComponentHandle_t HButtons[6], HAnalog[3];
	//std::string m_sSerialNumber;
	//std::string m_sModelNumber;
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CServerDriver_Sample: public IServerTrackedDeviceProvider
{
public:
	virtual EVRInitError Init( vr::IVRDriverContext *pDriverContext ) ;
	virtual void Cleanup() ;
	virtual const char * const *GetInterfaceVersions() { return vr::k_InterfaceVersions; }
	virtual void RunFrame() ;
	virtual bool ShouldBlockStandbyMode()  { return false; }
	virtual void EnterStandby()  {}
	virtual void LeaveStandby()  {}

private:
	CSampleDeviceDriver *m_pNullHmdLatest = nullptr;
	CSampleControllerDriver *m_pController = nullptr;
	CSampleControllerDriver *m_pController2 = nullptr;
};

CServerDriver_Sample g_serverDriverNull;


EVRInitError CServerDriver_Sample::Init( vr::IVRDriverContext *pDriverContext )
{
	VR_INIT_SERVER_DRIVER_CONTEXT( pDriverContext );

	CRegKey key;
	TCHAR steamPath[MAX_PATH];
	LONG status = key.Open(HKEY_CURRENT_USER, _T("Software\\Valve\\Steam"));
	if (status == ERROR_SUCCESS)
	{
		ULONG regSize = sizeof(steamPath);
		status = key.QueryStringValue(_T("SteamPath"), steamPath, &regSize);
	}
	key.Close();

	TCHAR configPath[MAX_PATH] = { 0 };
	_tcscat_s(configPath, sizeof(configPath), steamPath);
	_tcscat_s(configPath, sizeof(configPath), _T("/steamapps/common/SteamVR/drivers/hlalyx/HalfLifeAlyx.ini"));

	ctrlsConnected = true;

	CIniReader IniFile((char *)configPath);

	KEY_ID_LEFT_TRIGGER = IniFile.ReadInteger("Keys", "LEFT_TRIGGER", VK_RSHIFT);
	KEY_ID_LEFT_MENU = IniFile.ReadInteger("Keys", "LEFT_MENU", VK_RETURN);
	KEY_ID_LEFT_GRIP = IniFile.ReadInteger("Keys", "LEFT_GRIP", VK_ESCAPE);
	KEY_ID_LEFT_SYSTEM = IniFile.ReadInteger("Keys", "LEFT_SYSTEM", VK_F9);
	KEY_ID_RIGHT_TRIGGER = IniFile.ReadInteger("Keys", "RIGHT_TRIGGER", VK_LBUTTON);
	KEY_ID_RIGHT_TRIGGER2 = IniFile.ReadInteger("Keys", "RIGHT_TRIGGER2", VK_NUMPAD1);
	KEY_ID_RIGHT_MENU = IniFile.ReadInteger("Keys", "RIGHT_MENU", VK_DECIMAL);
	KEY_ID_RIGHT_GRIP = IniFile.ReadInteger("Keys", "RIGHT_GRIP", VK_NUMPAD0);
	KEY_ID_CHANGE_WEAPON = IniFile.ReadInteger("Keys", "CHANGE_WEAPON", VK_MBUTTON);
	KEY_ID_TELEPORT = IniFile.ReadInteger("Keys", "TELEPORT", VK_SPACE);
	KEY_ID_BACKPACK = IniFile.ReadInteger("Keys", "BACKPACK", 191); //?
	KEY_ID_FIRST_AID_SYRINGE = IniFile.ReadInteger("Keys", "FIRST_AID_SYRINGE", 220);
	KEY_ID_FIXING_LEFT_CONTROLLER = IniFile.ReadInteger("Keys", "FIXING_LEFT_CONTROLLER", 222);
	KEY_ID_THROW_ITEMS = IniFile.ReadInteger("Keys", "THROW_ITEMS", 186); //";"
	KEY_ID_HEALTH_AND_AMMO_DISPLAY = IniFile.ReadInteger("Keys", "HEALTH_AND_AMMO_DISPLAY", VK_BACK);
	KEY_ID_MOVE_CONTROLLERS_FORWARD = IniFile.ReadInteger("Keys", "MOVE_CONTROLLERS_FORWARD", VK_INSERT);
	KEY_ID_MOVE_CONTROLLERS_BACK = IniFile.ReadInteger("Keys", "MOVE_CONTROLLERS_BACK", VK_HOME);
	KEY_ID_PLAYER_RISE_HIGHER = IniFile.ReadInteger("Keys", "PLAYER_RISE_HIGHER", VK_PRIOR);
	KEY_ID_PLAYER_RISE_LOWER = IniFile.ReadInteger("Keys", "PLAYER_RISE_LOWER", VK_NEXT);
	KEY_ID_PLAYER_RISE_RESET = IniFile.ReadInteger("Keys", "PLAYER_RISE_RESET", 189); //-
	KEY_ID_CROUCH = IniFile.ReadInteger("Keys", "CROUCH", VK_RCONTROL);

	KEY_ID_COVER_MOUTH = IniFile.ReadInteger("Keys", "COVER_MOUTH", 'I');
	KEY_ID_PUT_HAT = IniFile.ReadInteger("Keys", "PUT_HAT", 'T');
	KEY_ID_LASER_MODE = IniFile.ReadInteger("Keys", "LASER_MODE", 'L');
	KEY_ID_THROW_ENERGY_BALL = IniFile.ReadInteger("Keys", "THROW_ENERGY_BALL", VK_END);
	KEY_ID_AIMING_MODE = IniFile.ReadInteger("Keys", "AIMING_MODE", VK_DELETE);
	KEY_ID_AIMING = IniFile.ReadInteger("Keys", "AIMING", VK_XBUTTON1);
	KEY_ID_LEFT_HAND_CELL = IniFile.ReadInteger("Keys", "LEFT_HAND_CELL", 'O');
	KEY_ID_RIGHT_HAND_CELL = IniFile.ReadInteger("Keys", "RIGHT_HAND_CELL", 'P');
	KEY_ID_LEFT_CTRL_MOTION = IniFile.ReadInteger("Keys", "LEFT_CTRL_MOTION", VK_RBUTTON);

	KEY_ID_ROTATION_CONTROLLERS_UP = IniFile.ReadInteger("Keys", "ROTATION_CONTROLLERS_UP", 'U');
	KEY_ID_ROTATION_CONTROLLERS_DOWN = IniFile.ReadInteger("Keys", "ROTATION_CONTROLLERS_DOWN", 'J');
	KEY_ID_ROTATION_CONTROLLERS_LEFT = IniFile.ReadInteger("Keys", "ROTATION_CONTROLLERS_LEFT", 'H');
	KEY_ID_ROTATION_CONTROLLERS_RIGHT = IniFile.ReadInteger("Keys", "ROTATION_CONTROLLERS_RIGHT", 'K');
	KEY_ID_ROTATION_CONTROLLERS_RESET = IniFile.ReadInteger("Keys", "ROTATION_CONTROLLERS_RESET", 'Y');

	KEY_ID_MOTION_MODE_MOVE_UP = IniFile.ReadInteger("Keys", "MOTION_MODE_MOVE_UP", 219);
	KEY_ID_MOTION_MODE_MOVE_DOWN = IniFile.ReadInteger("Keys", "MOTION_MODE_MOVE_DOWN", 221);

	KEY_ID_TURN_LEFT = IniFile.ReadInteger("Keys", "TURN_LEFT", 'N');
	KEY_ID_TURN_RIGHT = IniFile.ReadInteger("Keys", "TURN_RIGHT", 'M');

	KEY_ID_UP = IniFile.ReadInteger("Keys", "UP", VK_UP);
	KEY_ID_DOWN = IniFile.ReadInteger("Keys", "DOWN", VK_DOWN);
	KEY_ID_LEFT = IniFile.ReadInteger("Keys", "LEFT", VK_LEFT);
	KEY_ID_RIGHT = IniFile.ReadInteger("Keys", "RIGHT", VK_RIGHT);

	mouseSensetiveYaw = IniFile.ReadFloat("Main", "AxisX", 0.05);
	mouseSensetivePitch = IniFile.ReadFloat("Main", "AxisY", 0.04);

	ZPosSensetive = IniFile.ReadFloat("Main", "AxisZ", 0.009);

	VRMode = IniFile.ReadBoolean("Main", "VRMode", false);
	VRAimingAngleOffset = IniFile.ReadFloat("Main", "VRAimingAngleOffset", 0);

	m_HalfWidth = GetSystemMetrics(SM_CXSCREEN) / 2;
	m_HalfHeight = GetSystemMetrics(SM_CYSCREEN) / 2;

	HMDConnected = IniFile.ReadBoolean("Main", "HMD", true);

	if (HMDConnected) 
	{
		m_pNullHmdLatest = new CSampleDeviceDriver();
		vr::VRServerDriverHost()->TrackedDeviceAdded(m_pNullHmdLatest->GetSerialNumber().c_str(), vr::TrackedDeviceClass_HMD, m_pNullHmdLatest);
	}

	if (ctrlsConnected) {
		m_pController = new CSampleControllerDriver();
		m_pController->SetControllerIndex(1);
		vr::VRServerDriverHost()->TrackedDeviceAdded(m_pController->GetSerialNumber().c_str(), vr::TrackedDeviceClass_Controller, m_pController);


		m_pController2 = new CSampleControllerDriver();
		m_pController2->SetControllerIndex(2);
		vr::VRServerDriverHost()->TrackedDeviceAdded(m_pController2->GetSerialNumber().c_str(), vr::TrackedDeviceClass_Controller, m_pController2);
	}

	return VRInitError_None;
}

void CServerDriver_Sample::Cleanup() 
{

}


void CServerDriver_Sample::RunFrame()
{
	//Velocity
	static milliseconds lastMillis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	deltaTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - lastMillis;
	lastMillis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	//HL Alyx
	SteamVRHeadsetWindow = FindWindow(NULL, "Headset Window");
	HalfLifeAlyxWindow = FindWindow(NULL, "Half-Life: Alyx");

	if ((GetAsyncKeyState(VK_F1) & 0x8000) != 0 || (GetAsyncKeyState(VK_F7) & 0x8000) != 0)
		SetForegroundWindow(SteamVRHeadsetWindow);

	//if ((GetAsyncKeyState(VK_F7) & 0x8000) != 0)
	HeadsetWindowFocused = SteamVRHeadsetWindow != 0 && SteamVRHeadsetWindow == GetForegroundWindow() && IsWindowVisible(SteamVRHeadsetWindow);
	HalfLifeAlyxFocused = HalfLifeAlyxWindow != 0 && HalfLifeAlyxWindow == GetForegroundWindow() && IsWindowVisible(HalfLifeAlyxWindow);

	if (HeadsetWindowFocused || HalfLifeAlyxFocused)
		MouseToYawPitch();
	////////

	if ( m_pNullHmdLatest )
	{
		m_pNullHmdLatest->RunFrame();
	}

	if (ctrlsConnected) {
		
		GetControllersData(&MyCtrl, &MyCtrl2);

		if (m_pController)
		{
			m_pController->RunFrame();
		}
		if (m_pController2)
		{
			m_pController2->RunFrame();
		}

		vr::VREvent_t vrEvent;
		while ( vr::VRServerDriverHost()->PollNextEvent( &vrEvent, sizeof( vrEvent ) ) )
		{
			if ( m_pController )
			{
				m_pController->ProcessEvent(vrEvent);
			}
			if (m_pController2)
			{
				m_pController2->ProcessEvent(vrEvent);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
HMD_DLL_EXPORT void *HmdDriverFactory( const char *pInterfaceName, int *pReturnCode )
{
	if( 0 == strcmp( IServerTrackedDeviceProvider_Version, pInterfaceName ) )
	{
		return &g_serverDriverNull;
	}

	if( pReturnCode )
		*pReturnCode = VRInitError_Init_InterfaceNotFound;

	return NULL;
}

