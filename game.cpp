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
#include "effect2.h"
#include "particle2.h"
#include "meshimpact.h"
#include "itemgage.h"
#include "meshcylinder.h"
#include "mark.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define SPAWN_ENEMY (15) // 敵のスポーン数

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

	//メッシュインパクトの初期化処理
	InitMeshImpact();

	//メッシュシリンダーの初期化処理
	InitMeshCylinder();

	// メッシュドームの初期化処理
	InitmeshFan();

	// 煙の初期化処理
	InitExplosion();

	// ミニマップの初期化処理
	InitMiniMap();

	//敵の初期化処理
	InitEnemy();

	// 矢印の初期化処理
	InitMark();

	//プレイヤーの初期化処理
	InitPlayer();

	// エフェクトの初期化処理
	InitEffect();

	// エフェクトの初期化処理
	InitEffectX();

	// パーティクルの初期化処理
	InitParticle();

	// パーティクルの初期化処理
	InitParticleX();

	// ダメージの初期化処理
	InitDamege();

	// スコアの初期化処理
	InitScore();

	// クラフト画面の初期化処理
	InitCraftUI();

	// ビルボードの初期化処理
	InitBillboard();

	//ブロックの初期化処理
	InitBlock();

	//アイテムの初期化処理
	InitItem();

	//壁の初期化処理
	InitWall();
	
	// アイテムゲージの初期化
	InitItemGage();

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

	//弾の初期化処理
	InitBullet();

#ifdef _DEBUG

	//エディットの初期化処理
	InitEdit();

#endif // DEBUG

	//エディットのロード処理
	LoadEdit();
	LoadEdit2d();


	for (int nCntEnemy = 0; nCntEnemy < SPAWN_ENEMY; nCntEnemy++)
	{
		SpawnEnemy(2); // 敵を出す処理
		SpawnEnemy(1); // 敵を出す処理
	}

	// UIをセット
	SetGameUI(D3DXVECTOR3(70.0f,610.0f,0.0f),UITYPE_ICONFRAME,70.0f, 80.0f,0);
	SetGameUI(D3DXVECTOR3(200.0f, 630.0f, 0.0f), UITYPE_ICONFRAMESTOCK, 60.0f, 60.0f, 0);
	SetGameUI(D3DXVECTOR3(95.0f, 115.0f, 0.0f), UITYPE_KEY, 90.0f, 50.0f, 0);

	// ストックアイテムのアイコン
	SetIcon(D3DXVECTOR3(70.0f, 610.0f, 0.0f), 60.0f, 60.0f, 0,ICONTYPE_HOLDITEM);

	// テスト用 : 　ビルボードのセット
	//SetBillboard(D3DXVECTOR3(200.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.f, 0.0f), 0, 200.0f, 100.0f);

#ifdef _DEBUG

	SetEnemy(D3DXVECTOR3(200.0f, 0.0f, 200.0f), 6, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));

	SetBoss(D3DXVECTOR3(761.0f, 0.0f, 675.0f), 3.0f, 10000); // ボスをセット
	SetBoss(D3DXVECTOR3(761.0f, 0.0f, 675.0f), 3.0f, 10000); // ボスをセット

#endif // _DEBUG

	// 壁を設置する
	SetWall(D3DXVECTOR3(1500.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f,D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f),0);
	SetWall(D3DXVECTOR3(-1550.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f),0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(16.0f, 1.0f, 1.0f),0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1850.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(15.0f, 1.0f, 1.0f),0);

	//// 壁を設置する
	//SetWall(D3DXVECTOR3(1600.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 4.0f, 1.0f), 1);
	//SetWall(D3DXVECTOR3(-1650.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 4.0f, 1.0f), 1);
	//SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(15.0f, 4.0f, 1.0f), 1);
	//SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1950.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(15.0f, 4.0f, 1.0f), 1);

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

	// アイテムゲージの終了処理
	UninitItemGage();

	//影の終了処理
	UninitShadow();

	//ポーズの終了処理
	UninitPause();

	//メッシュフィールドの終了処理
	UninitMeshField();

	//メッシュインパクトの終了処理
	UninitMeshImpact();

	//メッシュシリンダーの終了処理
	UninitMeshCylinder();

	// メッシュドームの終了処理
	UninitmeshFan();

	// 軌跡の終了処理
	UninitMeshSword();

	// クラフト画面の終了処理
	UninitCraftUI();

	// 煙の終了処理
	UninitExplosion();

	//プレイヤーの終了処理
	UninitPlayer();

	// エフェクトの終了処理
	UninitEffect();

	// パーティクルの終了処理
	UninitParticle();

	// エフェクトの終了処理
	UninitEffectX();

	// パーティクルの終了処理
	UninitParticleX();

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

	// 矢印の終了処理
	UninitMark();

	// ミニマップの終了処理
	UninitMinMap();

#ifdef _DEBUG

	//エディットの終了処理
	UninitEdit();

#endif // DEBUG

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

	int nNumKill = GetNumKill(); // キル数

	//// 百の位、十の位、一の位
	//int HandredPlace = nNumKill / 100 % 10;
	//int tenPlace = nNumKill / 10 % 10;
	//int onePlace = nNumKill % 10;

	//static bool bSet = false;

	//int nSetSpawn = (HandredPlace * 100) + (tenPlace * 10) + onePlace;

	if (g_EnemyWaveTime >= 1800)
	{
		int Spawn_randvalue = rand() % 100; // 出るか出ないか
		int nSpawner = rand() % 3; // どこから出すか

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

		g_EnemyWaveTime = 0;
	}
	//else if ((nSetSpawn % 40 != 0))
	//{
	//	bSet = false;
	//}

	//// 敵が出てくるまでの時間
	//if (g_EnemyWaveTime >= 900)
	//{// カウントが900 or 場に出ている敵が0体以下の時

	//	int nSpawner = rand() % 4;

	//	for (int nCntEnemy = 0; nCntEnemy < SPAWN_ENEMY; nCntEnemy++)
	//	{
	//		// 敵を出す処理
	//		Enemy(nSpawner);
	//	}

	//	// タイムを初期化する
	//}

	if (TimeMinute <= 0 && TimeSecond <= 0)
	{// 敵が全滅 or タイマーが0秒以下
		g_gameState = GAMESTATE_END;
	}

	if (pPlayer->nLife <= 0)
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

	Camera* pCamera = GetCamera();

	if (g_bPause == false)
	{// ポーズ中でなければ
		// カーソルを無効化
		SetCursorVisibility(false);

		if (!g_bEditMode && pCamera->bEditMode == false)
		{// 編集モードじゃなかったら

			if (g_bCraft == false)
			{
				//カメラの更新処理
				UpdateCamera();

				//ライトの更新処理
				UpdateLight();

				//影の更新処理
				UpdateShadow();

				// エフェクトの更新処理
				UpdateEffectX();

				// パーティクルの更新処理
				UpdateParticleX();

				// 煙の更新処理
				UpdateExplosion();
				
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

				//更新処理
				UpdateBullet();

				// ミニマップの更新処理
				UpdateMiniMap();

				// メッシュインパクトの更新処理
				UpdateMeshImpact();

				// アイテムゲージの更新処理
				UpdateItemGage();

				//メッシュシリンダーの更新処理
				UpdateMeshCylinder();

				// 矢印の更新処理
				UpdateMark();

				// ブロックの更新処理
				UpdateBlock();
			}

			//プレイヤーの更新処理
			UpdatePlayer();

			// クラフト画面の更新処理
			UpdateCraftUI();

			// ビルボードの更新処理
			UpdateBillboard();

			//アイテムの更新処理
			UpdateItem();

		}
		else if (g_bEditMode == true || pCamera->bEditMode == true)
		{
#ifdef _DEBUG

			//エディットの終了処理
			UpdateEdit();

#endif // DEBUG

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

	// 矢印の描画処理
	DrawMark();

#ifdef _DEBUG

	if (g_bEditMode == true)
	{
		//エディットの描画処理
		DrawEdit();
	}

#endif // DEBUG

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

	//メッシュインパクトの描画処理
	DrawMeshImpact();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	// 煙の描画処理
	DrawExplosion();

	// エフェクトの描画処理
	DrawEffectX();

	// パーティクルの描画処理
	DrawParticleX();

	// ダメージの描画処理
	DrawDamege();

	// 影の描画処理
	DrawShadow();

	// アイコンの描画処理
	DrawIcon();

	//壁の描画処理
	DrawWall();

	// ボスの描画処理
	DrawBoss();

	// 軌跡の描画処理
	DrawMeshSword();

	//メッシュシリンダーの描画処理
	DrawMeshCylinder();

	//HPゲージの描写処理
	DrawGauge();

	// SPゲージの描画処理
	DrawSPgauge();

	// ゲームのUIの描画処理
	DrawGameUI();

	// アイテムゲージの描画処理
	DrawItemGage();

	// タイマーの描画処理
	DrawTime();

	//弾の描画処理
	DrawBullet();

	// ミニマップの描画処理
	DarwMinimap();

	if (g_bCraft == true)
	{
		// クラフト画面の描画処理
		DrawCraftUI();
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
