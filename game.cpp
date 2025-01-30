//============================
//
// ゲーム画面 [game.cpp]
// Author: TEAM_C
//
//============================

//****************************
// インクルードファイル
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
#include "pause.h"
#include "meshfield.h"
#include "player.h"
#include "enemy.h"
#include "block.h"
#include "item.h"
#include "edit.h"
#include "wall.h"
#include "HPGauge.h"
#include "Effect.h"
#include "Particle.h"
#include "sound.h"
#include "explosion.h"
#include "Timer.h"
#include "Score.h"
#include "gameui.h"
#include "mouse.h"
#include "meshsword.h"

//****************************
//グローバル変数
//****************************
GAMESTATE g_gameState = GAMESTATE_NONE;//ゲームの状態
int g_nCounterGameState = 0;//状態管理カウンター
bool g_bPause = false;//ポーズ中かどうか
bool g_bEditMode = false; // エディットモードかどうか
int g_EnemyWaveTime;

//=======================
//ゲーム画面の初期化処理
//=======================
void InitGame(void)
{
	SetCursorVisibility(false);
	//カメラの初期化処理
	InitCamera();

	//ライトの初期化処理
	InitLight();

	// ゲームのUIの初期化
	InitGameUI();

	//影の初期化処理
	InitShadow();

	//ポーズの初期化処理
	InitPause();

	//メッシュフィールドの初期化処理
	InitMeshField();

	// 煙の初期化処理
	InitExplosion();

	//プレイヤーの初期化処理
	InitPlayer();

	// エフェクトの初期化処理
	InitEffect();

	// パーティクルの初期化処理
	InitParticle();

	// ダメージの初期化処理
	InitDamege();

	// スコアの初期化処理
	InitScore();

	//敵の初期化処理
	InitEnemy();

	//ブロックの初期化処理
	InitBlock();

	//アイテムの初期化処理
	InitItem();

	//壁の初期化処理
	InitWall();

	//HPゲージの初期化処理
	InitGauge();

	// タイマーの初期化
	InitTimer();

	// 軌跡の初期化処理
	InitMeshSword();

	//エディットの初期化処理
	InitEdit();

	//エディットのロード処理
	LoadEdit();

	WaveEnemy(0); // 敵を出す処理
	WaveEnemy(1); // 敵を出す処理

	SetWall(D3DXVECTOR3(1000.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f,D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(-1000.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));

	g_gameState = GAMESTATE_NORMAL;//通常状態に設定
	g_nCounterGameState = 0;

	g_bPause = false;//ポーズ解除
	g_bEditMode = false;//エディットモード解除
	g_EnemyWaveTime = 1790; // 敵が出てくる時間

	// 音楽を再生
	PlaySound(SOUND_LABEL_GAME_BGM);
}
//=======================
//ゲーム画面の終了処理
//=======================
void UninitGame(void)
{
	// 音楽を停止
	StopSound();

	// ゲームのスコアを保存
	SaveScore();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	// ゲームのUIの終了処理
	UninitGameUI();

	//影の終了処理
	UninitShadow();

	//ポーズの終了処理
	UninitPause();

	//メッシュフィールドの終了処理
	UninitMeshField();

	// 軌跡の終了処理
	UninitMeshSword();

	// 煙の終了処理
	UninitExplosion();

	//プレイヤーの終了処理
	UninitPlayer();

	// エフェクトの終了処理
	UninitEffect();

	// パーティクルの終了処理
	UninitParticle();

	// ダメージの終了処理
	UninitDamege();
	
	// タイマーの終了
	UninitTimer();

	// スコアの終了処理
	UninitScore();

	//敵の終了処理
	UninitEnemy();

	//ブロックの終了処理
	UninitBlock();

	//アイテムの終了処理
	UninitItem();

	//壁の終了処理
	UninitWall();

	//HPゲージの終了処理
	UninitGauge();

	//エディットの終了処理
	UninitEdit();
}
//=======================
//ゲーム画面の更新処理
//=======================
void UpdateGame(void)
{
	g_EnemyWaveTime++;

	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// 敵の取得
	int nNumEnemy = GetNumEnemy();

	// タイマーの取得
	int nTime = GetTimer();

	// TODO : 敵が全滅したらゲーム終了
	
	// 敵が出てくるまでの時間
	if (g_EnemyWaveTime >= 900 || nNumEnemy <= 0)
	{
		int nSpawner = rand() % 2;

		// 敵を出す処理
		WaveEnemy(nSpawner);

		// タイムを初期化する
		g_EnemyWaveTime = 0;
	}

	if (nTime <= 0)
	{// 敵が全滅 or タイマーが0秒以下
		g_gameState = GAMESTATE_END;
	}

	if (!pPlayer->bDisp)
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
			SetRanking(GetScore());
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
		SetCursorVisibility(true);
	}

	int nNumObj = GetNumobj(); // オブジェクトの数を取得

	//エディットモードだったら
	if (KeyboardTrigger(DIK_F2) && g_bEditMode)
	{
		g_bEditMode = false;
		InitBlock(); // 出ているオブジェクトの初期化
		InitItem();  // 出ているオブジェクトの初期化
		LoadEdit();  // ロード
	}
	//エディットモードじゃなかったら
	else if (KeyboardTrigger(DIK_F2) && !g_bEditMode)
	{
		g_bEditMode = true;
	}

	if (g_bPause == false)
	{//ポーズ中でなければ
	//ポーズ中の更新処理
		SetCursorVisibility(false);

		if (!g_bEditMode)
		{
			//カメラの更新処理
			UpdateCamera();

			//ライトの更新処理
			UpdateLight();

			//影の更新処理
			UpdateShadow();

			// 煙の更新処理
			UpdateExplosion();

			if (pPlayer->bDisp)
			{
				//プレイヤーの更新処理
				UpdatePlayer();
			}

			// ダメージの更新処理
			UpdateDamege();

			// エフェクトの更新処理
			UpdateEffect();

			// パーティクルの更新処理
			UpdateParticle();

			// ゲームのUIの更新処理
			UpdateGameUI();

			// スコアの更新処理
			UpdateScore();

			//敵の更新処理
			UpdateEnemy();

			//ブロックの更新処理
			UpdateBlock();

			//アイテムの更新処理
			UpdateItem();

			//HPゲージの更新処理
			UpdateGauge();

			// タイマーの更新処理
			UpdateTimer();

			//壁の更新処理
			UpdateWall();

			// 軌跡の更新処理
			UpdateMeshSword();
		}
		else if (g_bEditMode)
		{
			//エディットの更新処理
			UpdateEdit();

			UpdateCamera();
		}	
	}

#ifdef _DEBUG

	if (KeyboardTrigger(DIK_F10))
	{
		g_gameState = GAMESTATE_END;
	}

#endif // _DEBUG

}
//=======================
//ゲーム画面の描画処理
//=======================
void DrawGame(void)
{
	//メッシュフィールドの描画処理
	DrawMeshField();

	//プレイヤーの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	//ブロックの描画処理
	DrawBlock();

	//アイテムの描画処理
	DrawItem();

	//壁の描画処理
	DrawWall();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	//HPゲージの描写処理
    DrawGauge();

	// タイマーの描画処理
	DrawTimer();

	// 煙の描画処理
	DrawExplosion();

	// 軌跡の描画処理
	DrawMeshSword();

	// ダメージの描画処理
	DrawDamege();

	// スコアの描画処理
	DrawScore();

    //プレイヤーの影の描画処理
    DrawShadow();

	// ゲームのUIの描画処理
	DrawGameUI();

	if (g_bEditMode)
	{
		//エディットの描画処理
		DrawEdit();
	}

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
//========================
// エディットモードの取得
//=====================
bool GetEditState(void)
{
	return g_bEditMode;
}
