//===================================
//
// チュートリアル3d [tutorial3e.cpp]
// Author: TEAM_C
//
//===================================

//****************************
//インクルードファイル
//****************************
#include "tutorial3d.h"
#include "shadow.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include"damagepop.h"
#include "meshfield.h"

//****************************
//グローバル変数
//****************************
static int nPressTuto;

//============================
//チュートリアル3dの初期化処理
//============================
void InitTutorial3d(void)
{
	//カメラの初期化処理
	InitCamera();
	
	//影の初期化処理
	InitShadow();

	//メッシュフィールドの初期化処理
	InitMeshField();
}
//============================
//チュートリアル3dの終了処理
//============================
void UninitTutorial3d(void)
{
	//カメラの終了処理
	UninitCamera();

	//影の終了処理
	UninitShadow();

	//メッシュフィールドの終了処理
	UninitMeshField();
}
//============================
//チュートリアル3dの更新処理
//============================
void UpdateTutorial3d(void)
{
	//カメラの更新処理
	UpdateCamera();

	//影の更新処理
	UpdateShadow();

	if (KeyboardTrigger(DIK_RETURN) == true||JoypadTrigger(JOYKEY_START)==true)
	{//Enterキー or Startボタンが押された
		//ゲーム画面へ
		SetFade(MODE_GAME);
	}
}
//============================
//チュートリアル3dの描画処理
//============================
void DrawTutorial3d(void)
{
	//影の描画処理
	DrawShadow();

	//メッシュフィールドの描画処理
	DrawMeshField();
}
