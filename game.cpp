//============================
//
// ゲーム画面 [game.cpp]
// Author: TEAM_C
//
//============================

//****************************
//インクルードファイル
//****************************
#include "main.h"
#include "game.h"
#include "fade.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "shadow.h"
#include "RankingScore.h"
#include "damagepop.h"
#include "time.h"
#include"pause.h"
#include "meshfield.h"
#include "player.h"
#include "enemy.h"
#include "block.h"

//****************************
//マクロ定義
//****************************

//****************************
//グローバル変数
//****************************
GAMESTATE g_gameState = GAMESTATE_NONE;//ゲームの状態
int g_nCounterGameState = 0;//状態管理カウンター
bool g_bPause = false;//ポーズ中かどうか

//=======================
//ゲーム画面の初期化処理
//=======================
void InitGame(void)
{
	//カメラの初期化処理
	InitCamera();

	//ライトの初期化処理
	InitLight();

	//影の初期化処理
	InitShadow();

	//ポーズの初期化処理
	InitPause();

	//メッシュフィールドの初期化処理
	InitMeshField();

	//プレイヤーの初期化処理
	InitPlayer();

	//敵の初期化処理
	InitEnemy();

	//ブロックの初期化処理
	InitBlock();

	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 20.0f), ENEMYTYPE_ONE, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(20.0f, 0.0f, 60.0f), ENEMYTYPE_ONE, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(50.0f, 0.0f, 80.0f), ENEMYTYPE_ONE, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//仮置き
	SetBlock(D3DXVECTOR3(50.0f, 0.0f, 0.0f), 0, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
	SetBlock(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), 0, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
	SetBlock(D3DXVECTOR3(100.0f, 0.0f, 0.0f), 0, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
	SetBlock(D3DXVECTOR3(110.0f, 0.0f, 60.0f), 0, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
	SetBlock(D3DXVECTOR3(120.0f, 0.0f, 20.0f), 0, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
	SetBlock(D3DXVECTOR3(130.0f, 0.0f, 40.0f), 0, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
	SetBlock(D3DXVECTOR3(140.0f, 0.0f, 90.0f), 0, D3DXVECTOR3(2.0f, 2.0f, 2.0f));

	g_gameState = GAMESTATE_NORMAL;//通常状態に設定
	g_nCounterGameState = 0;

	g_bPause = false;//ポーズ解除

}
//=======================
//ゲーム画面の終了処理
//=======================
void UninitGame(void)
{
	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//影の終了処理
	UninitShadow();

	//ポーズの終了処理
	UninitPause();

	//メッシュフィールドの終了処理
	UninitMeshField();

	//プレイヤーの終了処理
	UninitPlayer();

	//敵の終了処理
	UninitEnemy();

	//ブロックの終了処理
	UninitBlock();

}
//=======================
//ゲーム画面の更新処理
//=======================
void UpdateGame(void)
{
	int nNumEnemy = GetNumEnemy();

	//敵が全滅したらゲーム終了

	if (nNumEnemy <= 0)
	{
		g_gameState = GAMESTATE_END;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL://通常状態
		break;

	case GAMESTATE_END://終了状態
		g_nCounterGameState++;

		if (g_nCounterGameState >= 60)
		{
			g_nCounterGameState = 0;
			g_gameState = GAMESTATE_NONE;//何もしていない状態

			//画面(モード)の設定
			SetFade(MODE_RESULT);

			//ランキングのリセット
			ResetRanking();

			//ランキングのセット
			SetRanking(0);
		}
		break;
	}

	if (KeyboardTrigger(DIK_P) == true || JoypadTrigger(JOYKEY_START) == true)
	{//ポーズキー(Pキー)が押された
		g_bPause = g_bPause ? false : true;
	}
	if (g_bPause == true)
	{//ポーズ中
		//ポーズ中の更新処理
		UpdatePause();
	}
	if (g_bPause == false)
	{//ポーズ中でなければ

		//カメラの更新処理
		UpdateCamera();

		//ライトの更新処理
		UpdateLight();

		//影の更新処理
		UpdateShadow();

		//プレイヤーの更新処理
		UpdatePlayer();

		//敵の更新処理
		UpdateEnemy();

		//ブロックの更新処理
		UpdateBlock();

		if (KeyboardTrigger(DIK_F8))
		{
			g_gameState = GAMESTATE_END;
		}
	}
}
//=======================
//ゲーム画面の描画処理
//=======================
void DrawGame(void)
{
	//カメラの設定処理
	SetCamera();

	//プレイヤーの影の描画処理
	DrawShadow();

	//メッシュフィールドの描画処理
	DrawMeshField();

	//プレイヤーの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	//ブロックの描画処理
	DrawBlock();

	if (g_bPause == true)
	{//ポーズ中
		//ポーズ中の描画処理
		DrawPause();
	}

}
//===========================
//ゲーム画面の状態の設定処理
//===========================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;

	g_nCounterGameState = 0;
}
//=======================
//ゲーム画面の取得処理
//=======================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
//=====================
//ポーズの有効無効処理
//=====================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}