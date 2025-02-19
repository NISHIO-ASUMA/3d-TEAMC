//=====================================================================================================================
//
// チュートリアル3d [tutorial3e.cpp]
// Author: TEAM_C
//
//=====================================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
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
#include "explosion.h"
#include "mouse.h"
#include "HPGauge.h"
#include "gameui.h"
#include "edit.h"
#include "minimap.h"
#include "edit2d.h"
#include "meshsword.h"
#include "Effect.h"
#include "Particle.h"
#include "item.h"

//**************************************************************************************************************
//グローバル変数
//**************************************************************************************************************
bool g_bEditMode2;		// 編集モードかどうか

//==============================================================================================================
//チュートリアル3dの初期化処理
//==============================================================================================================
void InitTutorial3d(void)
{
	// カーソルを無効化
	SetCursorVisibility(false);

	// カメラの初期化処理
	InitCamera();
	
	// 影の初期化処理
	InitShadow();

	// 剣の軌跡の初期化処理
	InitMeshSword();

	// メッシュフィールドの初期化処理
	InitMeshField();

	// 壁の初期化処理
	InitWall();

	// ゲージの初期化
	InitGauge();

	// UIの初期化処理
	InitGameUI();

	// ミニマップの初期化処理
	InitMiniMap();

	// ポリゴンの初期化処理
	InitPolygon();

	// プレイヤーの初期化処理
	InitPlayer();

	// 影の初期化処理
	InitShadow();

	// ブロックの初期化処理
	InitBlock();

	// アイテムの初期化処理
	InitItem();

	// 爆発の初期化処理
	InitExplosion();

	// エフェクトの初期化
	InitEffect();

	// パーチくる
	InitParticle();

	// ステージの読み込み
	LoadEdit();
	LoadEdit2d();

	//// ステージを読み込む
	//tutoload();

	// ブロックをセット
	// TODO : ここの配置情報を修正---------------------
	SetBlock(D3DXVECTOR3(-160.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f,D3DX_PI * 0.5f,0.0f), 19, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	SetBlock(D3DXVECTOR3( -60.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 20, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	SetBlock(D3DXVECTOR3( 40.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 21, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	SetBlock(D3DXVECTOR3(140.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 47, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// アイテムをセット
	SetItem(D3DXVECTOR3(40.0f, 0.0f, -90.0f), 29, D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	//-------------------------------------------------
	
	// UIをセット
	SetGameUI(D3DXVECTOR3(640.0f, 40.0f, 0.0f), 4, 600.0f, 40.0f, 0);

	// グローバル変数の初期化
	g_bEditMode2 = false;

	// 音楽を再生
	PlaySound(SOUND_LABEL_TUTORIAL_BGM);
}
//==============================================================================================================
//チュートリアル3dの終了処理
//==============================================================================================================
void UninitTutorial3d(void)
{
	// 音楽を停止
	StopSound();

	// カメラの終了処理
	UninitCamera();

	// 影の終了処理
	UninitShadow();

	// ポリゴンの終了処理
	UninitPolygon();

	// 剣の軌跡の終了処理
	UninitMeshSword();

	// メッシュフィールドの終了処理
	UninitMeshField();

	// ゲージの終了
	UninitGauge();

	// UIの終了
	UninitGameUI();

	// 壁の終了処理
	UninitWall();

	// ミニマップの終了処理
	UninitMinMap();

	// プレイヤーの終了処理
	UninitPlayer();

	// ブロックの終了処理
	UninitBlock();

	// アイテムの終了処理
	UninitItem();

	// 爆発の終了処理
	UninitExplosion();

	// エフェクトの終了
	UninitEffect();

	//// エディター画面の終了処理
	//UninitEdit();
}
//==============================================================================================================
//チュートリアル3dの更新処理
//==============================================================================================================
void UpdateTutorial3d(void)
{
	// カメラの更新処理
	UpdateCamera();

	// 影の更新処理
	UpdateShadow();

	// 壁の更新処理
	UpdateWall();

	// UIの更新処理
	UpdateGameUI();

	// ブロックの更新処理
	UpdateBlock();

	// アイテムの更新処理
	UpdateItem();

	// 爆発の更新処理
	UpdateExplosion();

	// 剣の軌跡の更新処理
	UpdateMeshSword();

	// プレイヤーの更新処理
	UpdatePlayer();

	// エフェクトの更新処理
	UpdateEffect();

	if ((KeyboardTrigger(DIK_RETURN) == true||JoypadTrigger(JOYKEY_START)==true))
	{//Enterキー or Startボタンが押された
		//ゲーム画面へ
		SetFade(MODE_GAME);

		// 音楽再生
		PlaySound(SOUND_LABEL_ENTER_SE);

	}
}
//==============================================================================================================
//チュートリアル3dの描画処理
//==============================================================================================================
void DrawTutorial3d(void)
{
	//メッシュフィールドの描画処理
	DrawMeshField();

	// 壁の描画
	DrawWall();

	// プレイヤーの描画処理
	DrawPlayer();

	// ポリゴンの描画処理
	DrawPolygon();

	// 影の描画処理
	DrawShadow();

	// ブロックの描画処理
	DrawBlock();

	// アイテムの描画処理
	DrawItem();

	// 爆発の描画処理
	DrawExplosion();

	// 剣の軌跡の描画処理
	DrawMeshSword();

	// エフェクトの描画処理
	//DrawEffect();

	// UIの描画処理
	DrawGameUI();

	if (g_bEditMode2)
	{// g_bEditMode2がtrue
		//DrawEdit();
	}
}
//==============================================================================================================
//編集モードの取得処理
//==============================================================================================================
bool GetEditStatetuto(void)
{
	return g_bEditMode2;
}
