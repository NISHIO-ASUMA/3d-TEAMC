//============================
//
// タイトル3d [title3d.cpp]
// Author:YOSHIDA YUUTO
//
//
//============================

//****************************
//インクルードファイル
//****************************
#include "title3d.h"
#include "camera.h"
#include "light.h"
#include "shadow.h"
#include "input.h"
#include "fade.h"

//****************************
//グローバル宣言
//****************************
bool bFlash;

//============================
//タイトル3dの初期化処理
//============================
void InitTitle3d(void)
{
	//カメラの初期化処理
	InitCamera();

	//ライトの初期化処理
	InitLight();

	//影の初期化処理
	InitShadow();
}
//============================
//タイトル3dの終了処理
//============================
void UninitTitle3d(void)
{
	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//影の終了処理
	UninitShadow();
}
//============================
//タイトル3dの更新処理
//============================
void UpdateTitle3d(void)
{
	UpdateCamera();

	UpdateLight();

	//影の更新処理
	UpdateShadow();

	if (KeyboardTrigger(DIK_RETURN))
	{
		SetFade(MODE_TUTORIAL);
	}
}
//============================
//タイトル3dの描画処理
//============================
void DrawTitle3d(void)
{
	//影の描画処理
	DrawShadow();
}