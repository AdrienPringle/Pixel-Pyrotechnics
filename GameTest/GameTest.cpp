//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h>
#include <math.h>
#include <memory>
//------------------------------------------------------------------------
#include "App\app.h"
#include "src\src.h"

//------------------------------------------------------------------------

unsigned int HEX_HEIGHT = 146;
unsigned int HEX_DEPTH = 84;
unsigned int HEX_SIDE = 59;
unsigned int HEX_FACE = 120;
// 238 for full length
// 168 for full height (almost exactly 1/sqrt(2) ratio)

float total_time = 0.0f;

//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------
CSimpleSprite *testSprite;

std::shared_ptr<GameObject> root;
std::shared_ptr<SpriteObject> hex1;
std::shared_ptr<SpriteObject> hex2;

enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	testSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
	testSprite->SetPosition(400.0f, 400.0f);
	float speed = 1.0f / 15.0f;
	testSprite->CreateAnimation(ANIM_BACKWARDS, speed, {0, 1, 2, 3, 4, 5, 6, 7});
	testSprite->CreateAnimation(ANIM_LEFT, speed, {8, 9, 10, 11, 12, 13, 14, 15});
	testSprite->CreateAnimation(ANIM_RIGHT, speed, {16, 17, 18, 19, 20, 21, 22, 23});
	testSprite->CreateAnimation(ANIM_FORWARDS, speed, {24, 25, 26, 27, 28, 29, 30, 31});
	testSprite->SetScale(1.0f);

	root = std::shared_ptr<GameObject>(new GameObject());
	root->SetLocalPosition(400.0f, 400.0f);

	CSimpleSprite *hexSprite = App::CreateSprite("..\\assets\\spritesheet.bmp", 31, 1);
	CSimpleSprite *hexSprite2 = App::CreateSprite("..\\assets\\spritesheet.bmp", 31, 1);

	hex1 = std::shared_ptr<SpriteObject>(new SpriteObject(1, 0.0f, 0.0f, 0, root, std::unique_ptr<CSimpleSprite>(hexSprite)));
	root->AddChild(hex1);
	hex2 = std::shared_ptr<SpriteObject>(new SpriteObject(1, 0.0f, 146.0f, 0, root, std::unique_ptr<CSimpleSprite>(hexSprite2)));
	root->AddChild(hex2);

	//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	total_time += deltaTime;
	//------------------------------------------------------------------------
	// Example Sprite Code....
	testSprite->Update(deltaTime);

	int max_frames = 30;
	int cycles_till_full = 6;
	float degrees = fmod(total_time / 5.0f, 360);
	float interp = degrees / 360.0f;
	int frame = (int)(interp * cycles_till_full * max_frames) % max_frames;

	float ychange = HEX_HEIGHT;
	float xchange = ychange * sqrt(2);

	float newx = xchange * sin(interp * 2 * 3.14159);
	float newy = -ychange * cos(interp * 2 * 3.14159);
	hex2->SetLocalPosition(newx, newy);
	root->SetScale(sin(interp * 2 * 3.14159) / 4 + 1);

	hex2->SetZindex((int) -newy);

	hex2->SetFrame(frame);
	hex1->SetFrame(frame);

	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		testSprite->SetAnimation(ANIM_RIGHT);
		float x, y;
		testSprite->GetPosition(x, y);
		x += 1.0f;
		testSprite->SetPosition(x, y);
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		testSprite->SetAnimation(ANIM_LEFT);
		float x, y;
		testSprite->GetPosition(x, y);
		x -= 1.0f;
		testSprite->SetPosition(x, y);
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		testSprite->SetAnimation(ANIM_FORWARDS);
		float x, y;
		testSprite->GetPosition(x, y);
		y += 1.0f;
		testSprite->SetPosition(x, y);
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		testSprite->SetAnimation(ANIM_BACKWARDS);
		float x, y;
		testSprite->GetPosition(x, y);
		y -= 1.0f;
		testSprite->SetPosition(x, y);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	{
		testSprite->SetScale(testSprite->GetScale() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	{
		testSprite->SetScale(testSprite->GetScale() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		testSprite->SetAngle(testSprite->GetAngle() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		testSprite->SetAngle(testSprite->GetAngle() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		testSprite->SetAnimation(-1);
	}
	//------------------------------------------------------------------------
	// Sample Sound.
	//------------------------------------------------------------------------
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		App::PlaySound(".\\TestData\\Test.wav");
	}

	root->Update(deltaTime);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.)
// See App.h
//------------------------------------------------------------------------
void Render()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	testSprite->Draw();
	root->Draw();

	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	App::Print(100, 100, "Sample Text");

	//------------------------------------------------------------------------
	// Example Line Drawing.
	//------------------------------------------------------------------------
	static float a = 0.0f;
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	a += 0.1f;
	for (int i = 0; i < 20; i++)
	{

		float sx = 200 + sinf(a + i * 0.1f) * 60.0f;
		float sy = 200 + cosf(a + i * 0.1f) * 60.0f;
		float ex = 700 - sinf(a + i * 0.1f) * 60.0f;
		float ey = 700 - cosf(a + i * 0.1f) * 60.0f;
		g = (float)i / 20.0f;
		b = (float)i / 20.0f;
		App::DrawLine(sx, sy, ex, ey, r, g, b);
	}
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	delete testSprite;
	//------------------------------------------------------------------------
}