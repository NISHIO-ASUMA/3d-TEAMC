//============================
//
// タイトル3d [title3d.cpp]
// Author: TEAM_C
//
//============================

//****************************
//インクルードファイル
//****************************
#include "title3d.h"
#include "title.h"
#include "camera.h"
#include "light.h"
#include "shadow.h"
#include "input.h"
#include "fade.h"
#include "meshfield.h"
#include "gameui.h"
#include "sound.h"
#include "mouse.h"
#include "block.h"
#include "HPGauge.h"
#include "spgauge.h"

//============================
//タイトル3dの初期化処理
//============================
void InitTitle3d(void)
{
	// カーソルを無効化
	SetCursorVisibility(false);

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	//影の初期化処理
	InitShadow();

	// メッシュフィールドの初期化処理
	InitMeshField();

	// タイトルの初期化処理
	InitTitle();

	// ゲージの初期化処理
	InitGauge();

	// ゲームUIの初期化処理
	InitGameUI();

	// ブロックの初期化処理
	InitBlock();
	
	// プレイヤーの初期化処理
	InitPlayer();

	// スペシャルゲージの初期化処理
	InitSPgauge();

	// タイトル用のステージを読み込む処理
	LoadTitleState();

	// UIをセット
	SetGameUI(D3DXVECTOR3(640.0f, -200.0f, 0.0f), UITYPE_TITLE, 450.0f, 150.0f, 0);
	SetGameUI(D3DXVECTOR3(380.0f, 450.0f, 0.0f), UITYPE_TITLE2, 50.0f, 30.0f, 0);

	// サウンドを再生
	PlaySound(SOUND_LABEL_TITLE_BGM);
}
//============================
//タイトル3dの終了処理
//============================
void UninitTitle3d(void)
{
	// 音楽を停止
	StopSound();

	//カメラの終了処理
	UninitCamera();

	// ゲージの終了処理
	UninitGauge();

	// ライトの終了処理
	UninitLight();

	// 影の終了処理
	UninitShadow();

	// メッシュフィールドの終了処理
	UninitMeshField();

	// タイトルの終了処理
	UninitTitle();

	// ゲームUIの終了処理
	UninitGameUI();

	// ブロックの終了処理
	UninitBlock();

	// スペシャルゲージの終了処理
	UninitSPgauge();

	// プレイヤーの終了処理
	UninitPlayer();
}
//============================
//タイトル3dの更新処理
//============================
void UpdateTitle3d(void)
{
	// カメラの更新処理
	UpdateCamera();

	// ライトの更新処理
	UpdateLight();

	// 影の更新処理
	UpdateShadow();

	// タイトルの更新処理
	UpdateTitle();

	// ゲームUIの更新処理
	UpdateGameUI();
}
//============================
//タイトル3dの描画処理
//============================
void DrawTitle3d(void)
{
	// ブロックの描画処理
	DrawBlock();

	// 影の描画処理
	DrawShadow();

	// メッシュフィールドの描画処理
	DrawMeshField();

	// タイトルの描画処理
	DrawTitle();

	// ゲームUIの描画処理
	DrawGameUI();
}