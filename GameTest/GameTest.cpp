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

float total_time = 0.0f;

//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------
// CSimpleSprite *testSprite;

std::shared_ptr<GameObject> root;
std::shared_ptr<LevelObject> lvl;
int current_level;

// enum
// {
// 	ANIM_FORWARDS,
// 	ANIM_BACKWARDS,
// 	ANIM_LEFT,
// 	ANIM_RIGHT,
// };
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	// testSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
	// testSprite->SetPosition(400.0f, 400.0f);
	// float speed = 1.0f / 15.0f;
	// testSprite->CreateAnimation(ANIM_BACKWARDS, speed, {0, 1, 2, 3, 4, 5, 6, 7});
	// testSprite->CreateAnimation(ANIM_LEFT, speed, {8, 9, 10, 11, 12, 13, 14, 15});
	// testSprite->CreateAnimation(ANIM_RIGHT, speed, {16, 17, 18, 19, 20, 21, 22, 23});
	// testSprite->CreateAnimation(ANIM_FORWARDS, speed, {24, 25, 26, 27, 28, 29, 30, 31});
	// testSprite->SetScale(1.0f);
	//------------------------------------------------------------------------
	current_level = 0;

	root = std::shared_ptr<GameObject>(new GameObject());
	root->SetLocalPosition(400.0f, 400.0f);
	root->SetScale(1.0f);

	lvl = std::shared_ptr<LevelObject>(new LevelObject(1.0f, 0.0f, 0.0f, 0, root));
	lvl->LoadLevel(Level::levels[current_level]);
	root->AddChild(lvl);
}

void GetMouseDirection(int &x, int &z)
{

	float man_screen_x, man_screen_y;
	float mouse_x, mouse_y;

	lvl->GetManScreenPos(man_screen_x, man_screen_y);
	App::GetMousePos(mouse_x, mouse_y);

	float dy = (mouse_y - man_screen_y) * sqrt(2);
	float dx = (mouse_x - man_screen_x);
	if (dy * dy + dx * dx < 100 * 100)
	{
		x = 0;
		z = 0;
		return;
	}

	bool is_up = mouse_y > man_screen_y;
	bool is_right = mouse_x > man_screen_x;
	int quadrant = 2 * !is_up + (is_up ? is_right : !is_right) + lvl->GetGoalAngle();

	switch (quadrant % 4)
	{
	case 0:
		x = 0;
		z = -1;
		break;
	case 1:
		x = 1;
		z = 0;
		break;
	case 2:
		x = 0;
		z = 1;
		break;
	case 3:
		x = -1;
		z = 0;
		break;
	}
}

bool GetValidBombHeight(int bomb_x, int bomb_z, int &y) {
	int man_x, man_y, man_z;
	lvl->GetManPos(man_x, man_y, man_z);

	std::shared_ptr<Item3D> block;
	bool in_range = lvl->InRangeAt(bomb_x, man_y, bomb_z);
	if (!in_range) return false;

	bool is_valid_block = lvl->GetBlockAt(bomb_x, man_y, bomb_z, block);
	if (is_valid_block)
	{
		//block exists, proceed to look up
		do {
			man_y += 1;
			in_range = lvl->InRangeAt(bomb_x, man_y, bomb_z);
			is_valid_block = lvl->GetBlockAt(bomb_x, man_y, bomb_z, block);
		} while (in_range && is_valid_block);
		y = man_y;
		return true;
	}

	// otherwise, block doesn't exist. proceed to look down
	do {
		man_y -= 1;
		in_range = lvl->InRangeAt(bomb_x, man_y, bomb_z);
		is_valid_block = lvl->GetBlockAt(bomb_x, man_y, bomb_z, block);
	} while (!is_valid_block && in_range);
	y = man_y + 1;
	return in_range;
}

void MoveToHighestValidManPos(int x, int y, int z) {
	std::shared_ptr<Item3D> tmp;
	while(!lvl->GetBlockAt(x, y-1, z, tmp)){
		if(y <= 1) {
			lvl->UpdateManPos(x, -1, z);
			return;
		}
		y--;
	}
	lvl->UpdateManPos(x,y,z);

	if (lvl->GetBlockTypeAt(x,y-1,z) == Level::BlockType::door){
		current_level += 1;
		current_level %= Level::levels_size;

		lvl->LoadLevel(Level::levels[current_level]);
	}
}

void HandleBomb()
{
	if (lvl->GetLevelState() != LevelState::input)
		return;

	int dx, dz;
	int man_x, man_y, man_z;

	lvl->GetManPos(man_x, man_y, man_z);
	GetMouseDirection(dx, dz);

	int bomb_y;
	bool is_valid = GetValidBombHeight(man_x+dx, man_z+dz, bomb_y);
	lvl->UpdateBombPos(man_x + dx, bomb_y, man_z + dz, is_valid);

	if (is_valid && App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		// HandleBombPlace();
		Level::BlockType b = lvl->GetBlockTypeAt(man_x + dx, bomb_y - 1, man_z + dz);
		if(b == Level::BlockType::brick) {
			lvl->DeleteBlockAt(man_x + dx, bomb_y - 1, man_z + dz);
		} else if (b == Level::BlockType::barrel) {
			lvl->DeleteBlockAt(man_x + dx, bomb_y - 1, man_z + dz);
			lvl->ReduceBarrelCount();
		}

		// blocking (1) (edge case??? todo)
		//	 =
		// o =
		// = = = =

		// blocking (2)
		//     =
		// o x 
		// = = = =

		// not blocking
		//     x
		// o = =
		// = = = 
		std::shared_ptr<Item3D> tmp;
		if (bomb_y != man_y){
			return;
		}
		if (dx == 0 && dz == 0) {
			MoveToHighestValidManPos(man_x, man_y, man_z);
			return;
		}

		if (lvl->GetBlockAt(man_x - dx, bomb_y + 1, man_z - dz, tmp)){
			MoveToHighestValidManPos(man_x, man_y, man_z);
			return;
		}

		if (lvl->GetBlockAt(man_x - 2*dx, bomb_y + 1, man_z - 2*dz, tmp)){
			MoveToHighestValidManPos(man_x - dx, man_y + 1, man_z - dz);
			return;
		}
		MoveToHighestValidManPos(man_x - 2*dx, man_y + 1, man_z - 2*dz);

	}
}

void GameLogic()
{
	HandleBomb();
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, true))
	{
		lvl->RotateLeft();
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, true))
	{
		lvl->RotateRight();
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_START, true))
	{
		lvl->LoadLevel(Level::levels[current_level]);
	}
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	// testSprite->Update(deltaTime);

	// root->SetScale(sin(degrees * 2 * 3.14159 / 360) / 10 + 0.4f);

	// if (App::GetController().GetLeftThumbStickX() > 0.5f)
	// {
	// 	testSprite->SetAnimation(ANIM_RIGHT);
	// 	float x, y;
	// 	testSprite->GetPosition(x, y);
	// 	x += 1.0f;
	// 	testSprite->SetPosition(x, y);
	// }

	//------------------------------------------------------------------------
	// Sample Sound.
	//------------------------------------------------------------------------
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		App::PlaySound(".\\TestData\\Test.wav");
	}

	GameLogic();
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
	// testSprite->Draw();
	root->Draw();

	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	App::Print(100, 100, "Sample Text");

	float man_screen_x, man_screen_y;
	float mouse_x, mouse_y;
	lvl->GetManScreenPos(man_screen_x, man_screen_y);
	App::GetMousePos(mouse_x, mouse_y);

	char buffer[50];
	std::snprintf(buffer, 50, "mouse x:%d y:%d \n man: x:%d y:%d", (int)mouse_x, (int)mouse_y, (int)man_screen_x, (int)man_screen_y);
	App::Print(50, 50, buffer);
	
	App::GetMousePos(mouse_x, mouse_y);

	//------------------------------------------------------------------------
	// Example Line Drawing.
	// //------------------------------------------------------------------------
	// static float a = 0.0f;
	// float r = 1.0f;
	// float g = 1.0f;
	// float b = 1.0f;
	// a += 0.1f;
	// for (int i = 0; i < 20; i++)
	// {

	// 	float sx = 200 + sinf(a + i * 0.1f) * 60.0f;
	// 	float sy = 200 + cosf(a + i * 0.1f) * 60.0f;
	// 	float ex = 700 - sinf(a + i * 0.1f) * 60.0f;
	// 	float ey = 700 - cosf(a + i * 0.1f) * 60.0f;
	// 	g = (float)i / 20.0f;
	// 	b = (float)i / 20.0f;
	// 	App::DrawLine(sx, sy, ex, ey, r, g, b);
	// }
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	// delete testSprite;
	//------------------------------------------------------------------------

	// My understanding of smart pointers is that they don't need to be freed?
}