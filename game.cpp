//==============================================================================================================
//
// ゲーム画面 [game.cpp]
// Author: TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
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
#include "spgauge.h"
#include "boss.h"
#include "icon.h"
#include "polygon.h"
#include "edit2d.h"
#include "meshfan.h"
#include "billboard.h"
#include "craftui.h"

#include "Bullet.h"
#include "minimap.h"
//**************************************************************************************************************
//グローバル変数
//**************************************************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;//ゲームの状態
int g_nCounterGameState = 0;//状態管理カウンター
bool g_bPause = false;//ポーズ中かどうか
bool g_bEditMode = false; // エディットモードかどうか
int g_EnemyWaveTime;
bool g_bCraft = false;

//=========================================================================================================
//ゲーム画面の初期化処理
//=========================================================================================================
void InitGame(void)
{
	// カーソルを無効化
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

	// メッシュドームの初期化処理
	InitmeshFan();

	// 煙の初期化処理
	InitExplosion();

	// ミニマップの初期化処理
	InitMiniMap();

	//敵の初期化処理
	InitEnemy();

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

	// ビルボードの初期化処理
	InitBillboard();

	//ブロックの初期化処理
	InitBlock();

	//アイテムの初期化処理
	InitItem();

	//壁の初期化処理
	InitWall();

	//HPゲージの初期化処理
	InitGauge();

	// タイマーの初期化
	InitTime();

	// 軌跡の初期化処理
	InitMeshSword();

	// SPゲージの初期化処理
	InitSPgauge();

	// ボスの初期化処理
	InitBoss();

	// アイコンの初期化処理
	InitIcon();

	// ポリゴンの初期化処理
	InitPolygon();

	//エディットの初期化処理
	InitEdit();

	//エディットのロード処理
	LoadEdit();
	LoadEdit2d();

	//弾の初期化処理
	InitBullet();

	//WaveEnemy(0); // 敵を出す処理
	//WaveEnemy(1); // 敵を出す処理

	// UIをセット
	SetGameUI(D3DXVECTOR3(80.0f,550.0f,0.0f),UITYPE_ICONFRAME,80.0f,80.0f,0);

	SetIcon(D3DXVECTOR3(80.0f, 550.0f, 0.0f), 80.0f, 80.0f, 0);

	// テスト用 : 　ビルボードのセット
	//SetBillboard(D3DXVECTOR3(200.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.f, 0.0f), 0, 200.0f, 100.0f);

	SetEnemy(D3DXVECTOR3(200.0f, 0.0f, 200.0f), 5, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));

	SetBoss(D3DXVECTOR3(761.0f, 0.0f, 675.0f), 3.0f, 10000); // ボスをセット

	//// 壁を設置する
	//SetWall(D3DXVECTOR3(1000.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f,D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));
	//SetWall(D3DXVECTOR3(-1000.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));
	//SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));
	//SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));

	g_gameState = GAMESTATE_NORMAL; // 通常状態に設定
	g_nCounterGameState = 0;		// 画面遷移の時間

	g_bPause = false; // ポーズ解除
	g_bEditMode = false;// エディットモード解除
	g_EnemyWaveTime = 0; // 敵が出てくる時間
	g_bCraft = false;

	// 音楽を再生
	PlaySound(SOUND_LABEL_GAME_BGM);
}
//=========================================================================================================
//ゲーム画面の終了処理
//=========================================================================================================
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

	// メッシュドームの終了処理
	UninitmeshFan();

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
	UninitTime();

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

	// SPゲージの終了処理
	UninitSPgauge();

	// ボスの終了処理
	UninitBoss();

	// アイコンの終了処理
	UninitIcon();

	// ポリゴンの終了処理
	UninitPolygon();

	// ミニマップの終了処理
	UninitMinMap();

	//エディットの終了処理
	UninitEdit();

	// ビルボードの終了処理
	UninitBillboard();

	// 弾の終了処理
	UninitBullet();

}
//=========================================================================================================
//ゲーム画面の更新処理
//=========================================================================================================
void UpdateGame(void)
{
	if (g_bCraft == false && g_bPause == false && g_bEditMode == false)
	{
		// 敵の出現時間を加算する
		g_EnemyWaveTime++;
	}

	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// 敵の取得
	int nNumEnemy = GetNumEnemy();

	//// タイマーの取得
	//int nTime = GetTimer();
	
	// タイマーの取得
	int TimeMinute = GetTimeMinute(); // 分
	int TimeSecond = GetTimeSecond(); // 秒

	if (g_EnemyWaveTime >= 900)
	{
		int Spawn_randvalue = rand() % 100; // 出るか出ないか

		if (Spawn_randvalue <= 10)
		{
			int nSpawner = rand() % 2; // どこから出すか

			switch (nSpawner)
			{
			case 0:
				SetBoss(D3DXVECTOR3(761.0f, 0.0f, 675.0f), 3.0f, 10000); // ボスをセット
				break;
			case 1:
				SetBoss(D3DXVECTOR3(-526.0f, 0.0f, -455.0f), 3.0f, 10000); // ボスをセット
				break;
			case 2:
				SetBoss(D3DXVECTOR3(-506.0f, 0.0f, 675.0f), 3.0f, 10000); // ボスをセット
				break;
			default:
				break;
			}
		}
	}
	// 敵が出てくるまでの時間
	if (g_EnemyWaveTime >= 900 || nNumEnemy <= 0)
	{// カウントが900 or 場に出ている敵が0体以下の時
		int nSpawner = rand() % 2;

		// 敵を出す処理
		WaveEnemy(nSpawner);

		// タイムを初期化する
		g_EnemyWaveTime = 0;
	}

	if (TimeMinute <= 0 && TimeSecond <= 0)
	{// 敵が全滅 or タイマーが0秒以下
		g_gameState = GAMESTATE_END;
	}

	if (!pPlayer->bDisp)
	{// プレイヤーが未使用判定
		g_gameState = GAMESTATE_END;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL: //通常状態
		break;

	case GAMESTATE_END: //終了状態
		g_nCounterGameState++;

		if (g_nCounterGameState >= 60)
		{
			g_nCounterGameState = 0;	 // カウントを初期化
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

	if ((KeyboardTrigger(DIK_P) == true || JoypadTrigger(JOYKEY_START) == true) && g_bCraft == false)
	{//ポーズキー(Pキー)が押された
		g_bPause = g_bPause ? false : true;
	}
	if (g_bPause == true)
	{//ポーズ中
		//ポーズ中の更新処理
		UpdatePause();

		// カーソルを出現
		SetCursorVisibility(true);
	}

	int nNumObj = GetNumobj(); // オブジェクトの数を取得

	// エディットモードだったら
	if (KeyboardTrigger(DIK_F2) && g_bEditMode)
	{
		g_bEditMode = false;
		InitBlock();   // 出ているオブジェクトの初期化
		InitItem();    // 出ているオブジェクトの初期化
		InitPolygon(); // ポリゴンの初期化
		LoadEdit();    // ロード
		LoadEdit2d();
	}
	// エディットモードじゃなかったら
	else if (KeyboardTrigger(DIK_F2) && !g_bEditMode)
	{
		g_bEditMode = true;
	}

	if (g_bPause == false)
	{// ポーズ中でなければ
		// カーソルを無効化
		SetCursorVisibility(false);

		if (!g_bEditMode)
		{// 編集モードじゃなかったら

			if (!g_bCraft)
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
				{// bDispがtrue
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

				//HPゲージの更新処理
				UpdateGauge();

				// タイマーの更新処理
				UpdateTime();

				//壁の更新処理
				UpdateWall();

				// 軌跡の更新処理
				UpdateMeshSword();

				// SPゲージの更新処理
				UpdateSPgauge();

				// ボスの更新処理
				UpdateBoss();

				// アイコンの更新処理
				UpdateIcon();

				// ポリゴンの更新処理
				UpdatePolygon();

				// メッシュドームの更新処理
				UpdatemeshFan();

				//弾の更新処理
				UpdateBullet();

				// ミニマップの更新処理
				UpdateMiniMap();

			}
			// ビルボードの更新処理
			UpdateBillboard();

			//ブロックの更新処理
			UpdateBlock();

			//アイテムの更新処理
			UpdateItem();
		}
		else if (g_bEditMode)
		{
			//エディットの更新処理
			UpdateEdit();

			// カメラの更新処理
			UpdateCamera();
		}
	}

#ifdef _DEBUG

	if (KeyboardTrigger(DIK_F10))
	{// F10を押した
		g_gameState = GAMESTATE_END;
	}

#endif // _DEBUG

}
//=========================================================================================================
//ゲーム画面の描画処理
//=========================================================================================================
void DrawGame(void)
{
	//メッシュフィールドの描画処理
	DrawMeshField();

	// メッシュドームの描画処理
	DrawmeshFan();

	//プレイヤーの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	if (!g_bEditMode)
	{
		// ビルボードの描画処理
		DrawBillboard();

		//ブロックの描画処理
		DrawBlock();

		// ポリゴンの描画処理
		DrawPolygon();

		//アイテムの描画処理
		DrawItem();
	}

	// ボスの描画処理
	DrawBoss();

	//壁の描画処理
	DrawWall();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	//HPゲージの描写処理
    DrawGauge();

	// タイマーの描画処理
	DrawTime();

	// 煙の描画処理
	DrawExplosion();

	// 軌跡の描画処理
	DrawMeshSword();

	// ダメージの描画処理
	DrawDamege();

	// スコアの描画処理
	DrawScore();

	// SPゲージの描画処理
	DrawSPgauge();

	// アイコンの描画処理
	DrawIcon();

	// 影の描画処理
	DrawShadow();

	// ゲームのUIの描画処理
	DrawGameUI();

	//弾の描画処理
	DrawBullet();

	// ミニマップの描画処理
	DarwMinimap();

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
//=============================================================================================================
//ゲーム画面の状態の設定処理
//=============================================================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;

	g_nCounterGameState = 0;
}
//=========================================================================================================
//ゲーム画面の取得処理
//=========================================================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
//=======================================================================================================
//ポーズの有効無効処理
//=======================================================================================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}
//==========================================================================================================
// エディットモードの取得
//=======================================================================================================
bool GetEditState(void)
{
	return g_bEditMode;
}
//==========================================================================================================
// クラフト状態の設定処理
//=======================================================================================================
bool EnableCraft(bool bCraft)
{
	g_bCraft = bCraft;
	return g_bCraft;
}
