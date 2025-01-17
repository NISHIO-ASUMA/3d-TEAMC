//============================
//
// チュートリアル3d [tutorial3e.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include "tutorial3d.h"
#include "shadow.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include"damagepop.h"

//****************************
//グローバル変数
//****************************
static int nPressTuto;
//============================
//チュートリアル3dの初期化処理
//============================
void InitTutorial3d(void)
{
	InitCamera();

	InitShadow();
}
//============================
//チュートリアル3dの終了処理
//============================
void UninitTutorial3d(void)
{
	UninitCamera();

	UninitShadow();
}
//============================
//チュートリアル3dの更新処理
//============================
void UpdateTutorial3d(void)
{
	UpdateCamera();

	UpdateShadow();

	if (KeyboardTrigger(DIK_RETURN) == true||JoypadTrigger(JOYKEY_START)==true)
	{
		SetFade(MODE_GAME);
	}
}
//============================
//チュートリアル3dの描画処理
//============================
void DrawTutorial3d(void)
{
	SetCamera();

	DrawShadow();
}
