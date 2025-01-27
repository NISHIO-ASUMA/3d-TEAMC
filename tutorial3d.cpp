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
#include "damagepop.h"
#include "meshfield.h"
#include "block.h"
#include "wall.h"
#include "sound.h"

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

	// 壁の初期化処理
	InitWall();

	// プレイヤーの初期化処理
	InitPlayer();

	// ブロックの初期化処理
	InitBlock();

	// アイテムの初期化処理
	InitItem();

	// ブロックをセット
	SetBlock(D3DXVECTOR3(-160.0f, 20.0f, 0.0f), 19, D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	SetBlock(D3DXVECTOR3( -60.0f, 20.0f, 0.0f), 20, D3DXVECTOR3(1.5f, 1.5f, 1.5f)); 
	SetBlock(D3DXVECTOR3( 40.0f, 20.0f, 0.0f), 21, D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	// アイテムをセット
	SetItem(D3DXVECTOR3(140.0f, -10.0f, 0.0f), 29, D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	// 音楽を再生
	PlaySound(SOUND_LABEL_TUTORIAL_BGM);
}
//============================
//チュートリアル3dの終了処理
//============================
void UninitTutorial3d(void)
{
	// 音楽を停止
	StopSound();

	//カメラの終了処理
	UninitCamera();

	//影の終了処理
	UninitShadow();

	//メッシュフィールドの終了処理
	UninitMeshField();

	// 壁の終了処理
	UninitWall();

	// プレイヤーの終了処理
	UninitPlayer();

	// ブロックの終了処理
	UninitBlock();

	// アイテムの終了処理
	UninitItem();
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

	// 壁の更新処理
	UpdateWall();

	// プレイヤーの更新処理
	UpdatePlayer();

	// ブロックの更新処理
	UpdateBlock();

	// アイテムの更新処理
	UpdateItem();

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
	//メッシュフィールドの描画処理
	DrawMeshField();

	// 壁の描画
	DrawWall();

	// プレイヤーの描画処理
	DrawPlayer();

	//影の描画処理
	DrawShadow();

	// ブロックの描画処理
	DrawBlock();

	// アイテムの描画処理
	DrawItem();
}
