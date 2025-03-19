//==============================================================================================================
//
// タイトル3d処理 [ title3d.cpp ]
// Author: TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
//インクルードファイル宣言
//**************************************************************************************************************
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
#include "minimap.h"
#include "polygon.h"
#include "edit2d.h"
#include "player.h"
#include "wall.h"

//==============================================================================================================
// タイトル3dの初期化処理
//==============================================================================================================
void InitTitle3d(void)
{
	// カーソルを無効化
	SetCursorVisibility(true);

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// 影の初期化処理
	InitShadow();

	// メッシュフィールドの初期化処理
	InitMeshField();

	// タイトルの初期化処理
	InitTitle();

	// ゲージの初期化処理
	InitGauge();

	// ゲームUIの初期化処理
	InitGameUI();

	// 壁の初期化処理
	InitWall();

	// ブロックの初期化処理
	InitBlock();
	
	// ミニマップの初期化処理
	InitMiniMap();

	// プレイヤーの初期化処理
	InitPlayer();

	// スペシャルゲージの初期化処理
	InitSPgauge();

	// ポリゴンの初期化処理
	InitPolygon();

	// タイトルのカメラのうごきを設定する関数
	SetAnimation(0);

	// タイトル用のステージを読み込む処理
	LoadTitleState();
	LoadEdit2d();

	// 壁を設置する
	SetWall(D3DXVECTOR3(1500.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f), 0);
	SetWall(D3DXVECTOR3(-1550.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f), 0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(16.0f, 1.0f, 1.0f), 0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1850.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(15.0f, 1.0f, 1.0f), 0);

	// UIをセット
	SetGameUI(D3DXVECTOR3(1200.0f, 150.0f, 0.0f), UITYPE_KATANA, 450.0f, 50.0f, false,0); // タイトルの刀
	SetGameUI(D3DXVECTOR3(640.0f, 10.0f, 0.0f), UITYPE_TITLE, 450.0f, 100.0f,false, 0);   // タイトルロゴ
	SetGameUI(D3DXVECTOR3(330.0f, 450.0f, 0.0f), UITYPE_TITLE2, 50.0f, 30.0f,false, 0);   // 選択用の刀

	// サウンドを再生
	PlaySound(SOUND_LABEL_TITLE_BGM);
}
//==============================================================================================================
// タイトル3dの終了処理
//==============================================================================================================
void UninitTitle3d(void)
{
	// 音楽を停止
	StopSound();

	// カメラの終了処理
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

	// 壁の終了処理
	UninitWall();

	// 影の終了処理
	UninitShadow();

	// ブロックの終了処理
	UninitBlock();

	// スペシャルゲージの終了処理
	UninitSPgauge();

	// ミニマップの終了処理
	UninitMinMap();

	// ポリゴンの終了処理
	UninitPolygon();

	// プレイヤーの終了処理
	UninitPlayer();
}
//==============================================================================================================
// タイトル3dの更新処理
//==============================================================================================================
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

	// 影の更新処理
	UpdateShadow();

#ifdef _DEBUG
	if (KeyboardTrigger(DIK_F2))
	{// F2キーを押した
		// エフェクトエディター画面へ
		SetFade(MODE_EFFECT);
	}
#endif // _DEBUG

}
//==============================================================================================================
// タイトル3dの描画処理
//==============================================================================================================
void DrawTitle3d(void)
{
	// ブロックの描画処理
	DrawBlock();

	// メッシュフィールドの描画処理
	DrawMeshField();

	// 壁の描画処理
	DrawWall();

	// 影の描画処理
	DrawShadow();

	// ポリゴンの描画処理
	DrawPolygon();

	// タイトルの描画処理
	DrawTitle();

	// ゲームUIの描画処理
	DrawGameUI();
}