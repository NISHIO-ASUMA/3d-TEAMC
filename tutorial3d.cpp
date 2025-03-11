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
#include "icon.h"
#include "itemgage.h"
#include "billboard.h"
#include "craftui.h"
#include "TutorialSupport.h"
#include"spgauge.h"

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

	// クラフト画面の初期化処理
	InitCraftUI();

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

	// アイコンの初期化
	InitIcon();

	// アイテムゲージ
	InitItemGage();

	// ビルボードの初期化
	InitBillboard();

	// スペシャルゲージの初期化処理
	InitSPgauge();

	// ステージの読み込み
	LoadEdit();
	LoadEdit2d();

	// アイテムをセット
	SetItem(D3DXVECTOR3(70.0f, 0.0f, 120.0f), 0); // バット
	SetItem(D3DXVECTOR3(-10.0f, 0.0f, 120.0f), 3); // 石

	// UIをセット
	SetGameUI(D3DXVECTOR3(640.0f, 500.0f, 0.0f), UITYPE_TUTORIAL, 600.0f, 40.0f,false, 0);
	SetGameUI(D3DXVECTOR3(70.0f, 640.0f, 0.0f), UITYPE_ICONFRAME, 70.0f, 80.0f, false, 0);
	SetGameUI(D3DXVECTOR3(200.0f, 660.0f, 0.0f), UITYPE_ICONFRAMESTOCK, 60.0f, 60.0f, false, 0);

	// 壁を設置する
	SetWall(D3DXVECTOR3(1500.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f), 0);
	SetWall(D3DXVECTOR3(-1550.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f), 0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(16.0f, 1.0f, 1.0f), 0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1850.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(15.0f, 1.0f, 1.0f), 0);

	// ストックアイテムのアイコン
	SetIcon(D3DXVECTOR3(70.0f, 640.0f, 0.0f), 60.0f, 60.0f, 0, ICONTYPE_HOLDITEM);

	// チュートリアル補助
	InitManager();

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

	// クラフト画面の終了処理
	UninitCraftUI();

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

	// アイコンの終了
	UninitIcon();

	// アイテムゲージの終了
	UninitItemGage();

	// ビルボードの終了
	UninitBillboard();

	// チュートリアル補助の終了
	UninitManager();

	// スペシャルゲージの終了処理
	UninitSPgauge();

	//// エディター画面の終了処理
	//UninitEdit();
}
//==============================================================================================================
//チュートリアル3dの更新処理
//==============================================================================================================
void UpdateTutorial3d(void)
{
	Player* pPlayer = GetPlayer();

	if (pPlayer->bCraft == false)
	{
		// カメラの更新処理
		UpdateCamera();

		// 影の更新処理
		UpdateShadow();

		// 壁の更新処理
		UpdateWall();

		// UIの更新処理
		UpdateGameUI();

		// 爆発の更新処理
		UpdateExplosion();

		// 剣の軌跡の更新処理
		UpdateMeshSword();

		// エフェクトの更新処理
		UpdateEffect();

		// アイテムゲージの更新
		UpdateItemGage();

		// アイコンの更新
		UpdateIcon();

		// チュートリアル補助の更新
		UpdateManager();

		// スペシャルゲージの更新処理
		UpdateSPgauge();
	}
	// プレイヤーの更新処理
	UpdatePlayer();

	// クラフト画面の更新処理
	UpdateCraftUI();

	// ブロックの更新処理
	UpdateBlock();

	// アイテムの更新処理
	UpdateItem();

	// ビルボードの更新
	UpdateBillboard();

	// HPゲージの更新
	UpdateGauge();

	if ((KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_START)==true))
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

	// ビルボードの描画
	DrawBillboard();

	// アイテムの描画処理
	DrawItem();

	// 爆発の描画処理
	DrawExplosion();

	// 剣の軌跡の描画処理
	DrawMeshSword();

	// エフェクトの描画処理
	//DrawEffect();

	// スペシャルゲージの描画処理
	DrawSPgauge();

	// UIの描画処理
	DrawGameUI();

	// アイコンの描画処理
	DrawIcon();

	// アイテムゲージ
	DrawItemGage();

	// チュートリアル補助の描画
	DrawManager();

	// HPゲージの描画
	DrawGauge();

	Player* pPlayer = GetPlayer();

	if (pPlayer->bCraft == true)
	{
		// クラフト画面の描画処理
		DrawCraftUI();
	}

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
