//===============================================================================================================
//
// プレイヤー[player.cpp]
// Author:YOSHIDA YUUTO
//
//===============================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "player.h"
#include "input.h"
#include "camera.h"
#include <stdio.h>
#include <string.h>
#include "motion.h"
#include "meshfield.h"
#include "block.h"
#include "enemy.h"
#include "wall.h"
#include "mouse.h"
#include "Shadow.h"
#include "Effect.h"
#include "Particle.h"
#include "HPGauge.h"
#include "explosion.h"
#include "gameui.h"
#include "sound.h"
#include "meshsword.h"
#include "spgauge.h"
#include "boss.h"
#include "Bullet.h"
#include "minimap.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define MAX_WORD (256)			// 最大文字数
#define PLAYERLIFE (1000)		// プレイヤーの体力
#define MAX_TEXPLAYER (128)		// テクスチャの最大数
#define MAX_JUMP (15.0f)		// ジャンプ量
#define MAX_MOVE (1.0f)			// プレイヤーの移動量
#define NUM_MTX (8)				// 剣の当たり判定のマトリクスの数
#define LANDINGEXPLOSION (6)	// 着地したときに出る煙

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void PlayerComb(MOTIONTYPE motiontype, int AttackState,int nCounterState, COMBOSTATE Combstate); // プレイヤーのコンボ処理
void LoadMotion(int Weponmotion);																 // モーションのロード処理
void StickPad(void);																			 // パッドの移動処理

void LoadPlayer(int nType);                                                                      // プレイヤーのロード処理
int LoadFilename(FILE* pFile, int nNumModel, char* aString, int nType);							 // プレイヤーのモデルのロード処理
void LoadCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType);					 // プレイヤーのパーツの設定処理
void LoadMotionSet(FILE* pFile, char* aString, int nNumModel, int nType);						 // プレイヤーのモーションのロード処理
void LoadKeySet(FILE* pFile, char* aString, int nType, int nCntMotion);							 // プレイヤーのモーションのキーの読み込み処理

//**************************************************************************************************************
//グローバル変数宣言
//**************************************************************************************************************
//LPDIRECT3DTEXTURE9 g_apTexturePlayer[MAX_TEXPLAYER] = {};//プレイヤーのテクスチャへのポインタ
Player g_player;//プレイヤー構造体
Player g_LoadPlayer[PLAYERTYPE_MAX]; // プレイヤーのモデルを保存しておく変数
MOTION g_LoadMotion[MOTION_MAX];   // モーションの情報を保存しておく変数
int g_nCounterState,g_AttackState; // 状態カウンター
bool bNohand; // 投げたか投げてないか
bool bUsePad;
//bool bFirstChange;
int nCntMotion,nKey;

//===============================================================================================================
//プレイヤーの初期化処理
//===============================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	MODE mode = GetMode();//現在のモードを取得

	//プレイヤーの初期化
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);		   // 座標
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   // 角度
	g_player.rotDestPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 目的の角度
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   // 移動量
	g_player.bJump = false;								   // ジャンプ中か否か
	g_player.bDisp = true;								   // 使用状態
	g_player.bMove = false;								   // 
	g_player.nLife = PLAYERLIFE;						   // 体力
	g_player.state = PLAYERSTATE_NORMAL;				   // 状態
	g_player.Motion.bLoopMotion = true;					   // 
	g_player.Swordrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	   // 剣の角度
	g_player.Motion.nKey = 0;							   // キー数
	g_player.Motion.motionType = MOTIONTYPE_NEUTRAL;	   // モーションの種類
	g_player.SwordOffpos.x = 0.0f;						   // 剣のオフセットの座標x
	g_player.SwordOffpos.y = 85.0f;						   // 剣のオフセットの座標y
	g_player.SwordOffpos.z = 0.0f;						   // 剣のオフセットの座標z
	g_player.nCounterAction = 0;						   // アクションカウント
	g_nCounterState = 0;                                   // 状態カウンター
	g_AttackState = 0;									   // 攻撃状態のカウンター
	bNohand = false;									   // 物を投げたか投げてないか
	g_player.speed = 1.0f;								   // 足の速さ
	g_player.nDamage = 100;								   // 攻撃力
	bUsePad = false;
	//bFirstChange = false;
	g_player.nStockDamage = 100;
	g_player.fStockSpeed = 3.5f;
	g_player.FeverMode = false;
	g_player.SpMode = false;
	g_player.WeponMotion = MOTION_KATANA;
	g_player.AttackSp = false;
	g_player.bLandingOBB = false;
	nCntMotion = 0;
	nKey = 0;
	g_player.bCraft = false;
	// TODO : ここの処理考える

	// タイトルでロードをすると重くなるので
	if (mode != MODE_TITLE)
	{
		LoadMotion(0);
		LoadMotion(1);
		LoadMotion(2);
		LoadMotion(3);
		LoadMotion(4);
		LoadMotion(5);
		LoadMotion(6);
		LoadMotion(7);
		LoadMotion(8);
		LoadMotion(9);

		// 切り替わるモーションの数だけ
		for (int nCnt = 0; nCnt < MOTION_MAX; nCnt++)
		{
		}

		g_LoadPlayer[0].nIdxShadow = SetShadow(g_player.pos, g_player.rot, 20.0f, 1.0f);

		// タイプを代入
		g_LoadPlayer[0].PlayerType = PLAYERTYPE_NOHAND;
		g_LoadPlayer[1].PlayerType = PLAYERTYPE_WEPON;

		D3DXMATERIAL* pMat;//マテリアルへのポインタ

		for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
		{
			//必要な情報を設定
			//LoadModel(nCntPlayer);
			LoadPlayer(nCntPlayer);

			g_LoadPlayer[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_LoadPlayer[nCntPlayer].nLife = PLAYERLIFE;
			g_LoadPlayer[nCntPlayer].nMaxLife = PLAYERLIFE;
			g_LoadPlayer[nCntPlayer].bDisp = true;
			g_LoadPlayer[nCntPlayer].Motion.motionType = MOTIONTYPE_NEUTRAL;
			g_LoadPlayer[nCntPlayer].Motion.bLoopMotion = true;
			g_LoadPlayer[nCntPlayer].bJumpAttack = false;
			g_LoadPlayer[nCntPlayer].HandState = PLAYERHOLD_NO;
			g_LoadPlayer[nCntPlayer].state = PLAYERSTATE_NORMAL;
			g_LoadPlayer[nCntPlayer].Combostate = COMBO_ATTACK1;
			g_LoadPlayer[nCntPlayer].speed = 1.0f;
			g_LoadPlayer[nCntPlayer].nDamage = 100;
			g_LoadPlayer[nCntPlayer].SwordOffpos.x = 0.0f;						   // 剣のオフセットの座標x
			g_LoadPlayer[nCntPlayer].SwordOffpos.y = 85.0f;						   // 剣のオフセットの座標y
			g_LoadPlayer[nCntPlayer].SwordOffpos.z = 0.0f;						   // 剣のオフセットの座標z
			g_LoadPlayer[nCntPlayer].WeponMotion = MOTION_KATANA;				   // 剣のオフセットの座標z
			g_LoadPlayer[nCntPlayer].nStockDamage = 100;
			g_LoadPlayer[nCntPlayer].fStockSpeed = 3.5f;
			g_LoadPlayer[nCntPlayer].FeverMode = false;
			g_LoadPlayer[nCntPlayer].SpMode = false;
			g_LoadPlayer[nCntPlayer].AttackSp = false;
			g_LoadPlayer[nCntPlayer].bLandingOBB = false;
			g_LoadPlayer[nCntPlayer].bCraft = false;

			// アイテム分回す
			for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
			{
				g_LoadPlayer[nCntPlayer].Itembreak[nCnt] = false; // アイテムが壊れたか
			}
		
		}

		g_LoadPlayer[0].nIdxMap = SetMiniMap(g_player.pos, MINIMAPTEX_PLAYER);

		//最初に描画したいプレイヤーの情報を代入
		g_player = g_LoadPlayer[0];
	}

	/*Itemchange(30);
	MotionChange(MOTION_KATANA, 0);
	StatusChange(3.0f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 100);*/
}
//===============================================================================================================
//プレイヤーの終了処理
//===============================================================================================================
void UninitPlayer(void)
{
	// 音楽を停止
	StopSound();

	for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
	{
		for (int nCntModel = 0; nCntModel < g_LoadPlayer[nCntPlayer].Motion.nNumModel; nCntModel++)
		{
			//テクスチャの破棄
			for (int nCntMat = 0; nCntMat < (int)g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				if (g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pTexture[nCntMat] != NULL)
				{
					g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pTexture[nCntMat]->Release();
					g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pTexture[nCntMat] = NULL;
				}
			}

			//メッシュの破棄
			if (g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh != NULL)
			{
				 g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh->Release();
				 g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh = NULL;
			}

			//マテリアルの破棄
			if (g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pBuffMat != NULL)
			{
				 g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pBuffMat->Release();
				 g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pBuffMat = NULL;
			}
		}
	}

	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel; nCntModel++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_player.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_player.Motion.aModel[nCntModel].pTexture[nCntMat] != NULL)
			{
				g_player.Motion.aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		//メッシュの破棄
		if (g_player.Motion.aModel[nCntModel].pMesh != NULL)
		{
			g_player.Motion.aModel[nCntModel].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_player.Motion.aModel[nCntModel].pBuffMat != NULL)
		{
			g_player.Motion.aModel[nCntModel].pBuffMat = NULL;
		}
	}
}
//===============================================================================================================
//プレイヤーの更新処理
//===============================================================================================================
void UpdatePlayer(void)
{
	Camera* pCamera = GetCamera();
	Item* pItem = GetItem();

		/*StickPad();*/

	// フィーバーモードなら
	if (g_player.FeverMode)
	{
		SetParticle(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 25, g_player.pos.z), D3DXVECTOR3(D3DX_PI / 2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 2.0f, 1, 20, 10, 20.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	}

	StickPad(); // パッドの移動処理

	// パッドを使っていないかつ攻撃モーションじゃない
	if (!bUsePad &&
		g_player.Motion.motionType != MOTIONTYPE_ACTION&&
		g_player.Motion.motionType != MOTIONTYPE_ACTION2&&
		g_player.Motion.motionType != MOTIONTYPE_ACTION3&&
		g_player.Motion.motionType != MOTIONTYPE_ACTION4)
	{
		if (GetKeyboardPress(DIK_A))
		{
			//プレイヤーの移動(上)
			if (GetKeyboardPress(DIK_W) == true)
			{
				//g_player.Motion.motionType = MOTIONTYPE_MOVE;

				if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
				{
					g_player.Motion.motionType = MOTIONTYPE_MOVE;
				}

				g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.25f) * g_player.speed;
				g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.25f) * g_player.speed;

				g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.75f;
			}
			//プレイヤーの移動(下)
			else if (GetKeyboardPress(DIK_S))
			{
				if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
				{
					g_player.Motion.motionType = MOTIONTYPE_MOVE;
				}

				g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.75f) * g_player.speed;
				g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.75f) * g_player.speed;

				g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.25f;
			}
			//プレイヤーの移動(左)
			else
			{
				if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
				{
					g_player.Motion.motionType = MOTIONTYPE_MOVE;
				}

				g_player.move.z += sinf(pCamera->rot.y) * g_player.speed;
				g_player.move.x -= cosf(pCamera->rot.y) * g_player.speed;

				g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.5f;
			}
		}
		//プレイヤーの移動(右)
		else if (GetKeyboardPress(DIK_D))
		{
			//プレイヤーの移動(上)
			if (GetKeyboardPress(DIK_W))
			{
				if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
				{
					g_player.Motion.motionType = MOTIONTYPE_MOVE;
				}

				g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * g_player.speed;
				g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * g_player.speed;

				g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.75f;
			}
			//プレイヤーの移動(下)
			else if (GetKeyboardPress(DIK_S))
			{
				if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
				{
					g_player.Motion.motionType = MOTIONTYPE_MOVE;
				}

				g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * g_player.speed;
				g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * g_player.speed;

				g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.25f;
			}
			//プレイヤーの移動(右)
			else
			{
				if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
				{
					g_player.Motion.motionType = MOTIONTYPE_MOVE;
				}

				g_player.move.z -= sinf(pCamera->rot.y) * g_player.speed;
				g_player.move.x += cosf(pCamera->rot.y) * g_player.speed;

				g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.5f;
			}

		}
		//プレイヤーの移動(上)
		else if (GetKeyboardPress(DIK_W) == true)
		{
			if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
			{
				g_player.Motion.motionType = MOTIONTYPE_MOVE;
			}

			g_player.move.x += sinf(pCamera->rot.y) * g_player.speed;
			g_player.move.z += cosf(pCamera->rot.y) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI;
		}
		//プレイヤーの移動(下)
		else if (GetKeyboardPress(DIK_S) == true)
		{
		if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;
		}

			g_player.move.x -= sinf(pCamera->rot.y) * g_player.speed;
			g_player.move.z -= cosf(pCamera->rot.y) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y;
		}
		else
		{
			if (g_player.Motion.motionType == MOTIONTYPE_MOVE)
			{
				SetMotion(&g_player.Motion,MOTIONTYPE_NEUTRAL,MOTIONTYPE_NEUTRAL,true,40); // モーションをニュートラルにする
			}
		}
	}

	switch (g_player.Motion.motionType)
	{
	case MOTIONTYPE_NEUTRAL:
		break;
	case MOTIONTYPE_MOVE:
		break;
	case MOTIONTYPE_ACTION:
		break;
	case MOTIONTYPE_JUMP:
		break;
	case MOTIONTYPE_LANDING:
		break;
	case MOTIONTYPE_ACTION2:
		break;
	case MOTIONTYPE_ACTION3:
		break;
	case MOTIONTYPE_ACTION4:
		break;
	default:
		break;
	}

	D3DXVECTOR3 SwordPos(
		g_player.SwordMtx._41, // X方向
		g_player.SwordMtx._42, // Y方向
		g_player.SwordMtx._43  // Z方向
	);

	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:
		break;
	case PLAYERSTATE_MOVE:
		g_nCounterState--;

		if (g_nCounterState < 0)
		{
			g_player.state = PLAYERSTATE_NORMAL; // 状態をノーマルにする
		}
		break;
	case PLAYERSTATE_ATTACK:
		g_nCounterState--;

		// 武器がないとき
		if (g_player.Motion.nNumModel != 15)
		{
			if (g_player.nElement == WEPONELEMENT_STANDARD)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 1.0f, 2, 30, 10, 10.0f, 5.0f, false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if (g_player.nElement == WEPONELEMENT_BLOOD)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 2.0f, 2, 30, 10, 10.0f, 5.0f, false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if (g_player.nElement == WEPONELEMENT_FIRE)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 3.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 3.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
			}
			else if (g_player.nElement == WEPONELEMENT_FREEZE)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 2, 60, 10, 1.0f, 5.0f, false, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
			}
			else if (g_player.nElement == WEPONELEMENT_SPARK)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 1.0f, 2, 15, 30, 30.0f, 5.0f, false, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
			}
			else if (g_player.nElement == WEPONELEMENT_AQUA)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 4.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, -4.0f, 0.0f));
			}
			else if (g_player.nElement == WEPONELEMENT_DARK)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 4.0f, 2, 45, 10, 6.0f, 5.0f, false, D3DXVECTOR3(0.0f, -4.0f, 0.0f));
			}
		}

		if (g_nCounterState < 0)
		{
			g_player.state = PLAYERSTATE_NORMAL; // 状態をノーマルにする
		}
		break;
	case PLAYERSTATE_JUMP:
		g_nCounterState--;
		if (g_nCounterState < 0)
		{
			g_player.state = PLAYERSTATE_NORMAL; // 状態をノーマルにする
		}
		break;
	case PLAYERSTATE_LANDING:
		g_nCounterState--;

		if (g_nCounterState < 0)
		{
			g_player.state = PLAYERSTATE_NORMAL; // 状態をノーマルにする
		}
		break;
	case PLAYERSTATE_DAMAGE:
		g_nCounterState--;

		if (g_nCounterState < 0)
		{
			g_player.state = PLAYERSTATE_NORMAL; // 状態をノーマルにする
		}
		break;
	default:
		break;
	}

	switch (g_player.Combostate)
	{
	case COMBO_NO:
		break;
	case COMBO_ATTACK1:
		g_AttackState--;
		if (g_AttackState < 0)
		{
			g_player.Combostate = COMBO_NO; // コンボ状態を初期化
		}
		break;
	case COMBO_ATTACK2:
		g_AttackState--;
		if (g_AttackState < 0)
		{
			g_player.Combostate = COMBO_NO; // コンボ状態を初期化
		}
		break;
	case COMBO_ATTACK3:
		g_AttackState--;
		if (g_AttackState < 0)
		{
			g_player.Combostate = COMBO_NO; // コンボ状態を初期化
		}
		break;
	case COMBO_ATTACK4:
		g_AttackState--;
		if (g_AttackState < 0)
		{
			g_player.Combostate = COMBO_NO; // コンボ状態を初期化
		}
		break;
	default:
		break;
	}

	//移動量を減衰
	g_player.move.x += (0.0f - g_player.move.x) * 0.5f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.5f;

	//前回の位置を保存
	g_player.posOld = g_player.pos;

	//プレイヤーの位置の更新
	g_player.pos += g_player.move;

	//if (CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.Size))
	//{
	//	// モーションがジャンプだったら
	//	if (g_player.Motion.motionType == MOTIONTYPE_JUMP)
	//	{
	//		// 着地のときに出す煙分
	//		for (int nCnt = 0; nCnt < LANDINGEXPLOSION; nCnt++)
	//		{
	//			// 角度を求める
	//			float fAngle = (D3DX_PI * 2.0f) / LANDINGEXPLOSION * nCnt;

	//			// 煙を出す
	//			SetExplosion(D3DXVECTOR3(g_player.pos.x, g_player.pos.y, g_player.pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	//				60, 15.0f, 15.0f, EXPLOSION_LANDING);
	//		}
	//		SetMotion(&g_player.Motion, MOTIONTYPE_LANDING, MOTIONTYPE_NEUTRAL, true, 10); // モーションを着地にする
	//	}
	//	g_player.bJump = true; // ジャンプを可能にする
	//}

	if (CollisionField())
	{
		g_player.bLandingOBB = false;
		// モーションがジャンプだったら
		if (g_player.Motion.motionType == MOTIONTYPE_JUMP)
		{
			// 着地のときに出す煙分
			for (int nCnt = 0; nCnt < LANDINGEXPLOSION; nCnt++)
			{
				// 角度を求める
				float fAngle = (D3DX_PI * 2.0f) / LANDINGEXPLOSION * nCnt;

				// 煙を出す
				SetExplosion(D3DXVECTOR3(g_player.pos.x,0.0f,g_player.pos.z),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),
					60,15.0f,15.0f, EXPLOSION_LANDING);
				g_player.pos.y = 0.0f;
			}
			SetMotion(&g_player.Motion, MOTIONTYPE_LANDING, MOTIONTYPE_NEUTRAL, true, 10); // モーションを着地にする
		}
		g_player.bJump = true; // ジャンプを可能にする
	}
	else
	{
		g_player.bJump = false;
	}

	CollisionWall();

	//プレイヤーの重力
	g_player.move.y -= 1.0f;

	// 影の計算
	SetPositionShadow(g_player.nIdxShadow,g_player.pos,30.0f + 30.0f * g_player.pos.y / 200.0f, 1.0f / (1.0f + g_player.pos.y / 30.0f));

	SetMiniMapPotision(g_player.nIdxMap, &g_player.pos);

	////壁の衝突判定
	//CollisionWall();

	if (JoypadTrigger(JOYKEY_A) || KeyboardTrigger(DIK_SPACE))
	{//aボタン or Enterキーが押された

		// 音楽再生
		PlaySound(SOUND_LABEL_JUMP_SE);

		if (g_player.bJump == true && g_player.Motion.motionType != MOTIONTYPE_LANDING && g_player.AttackSp == false)
		{
			g_player.bJump = false;						 // ジャンプをできなくする
			g_player.Motion.nKey = 0;					 // キーを0から始める
			g_player.Motion.nCountMotion = 0;            // モーションカウントを0から始める
			g_player.Motion.motionType = MOTIONTYPE_JUMP;// モーションタイプをジャンプにする
			g_player.move.y = 15.0f;					 // 頒布量		
		}
	}

	//SetEffect(D3DXVECTOR3(g_player.Motion.aModel[5].mtxWorld._41, g_player.Motion.aModel[5].mtxWorld._42, g_player.Motion.aModel[5].mtxWorld._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 20.0f);

	// プレイヤーの状態が攻撃じゃないかつ地面にいる
	if (g_player.bDisp && !bNohand && !g_player.AttackSp)
	{
		if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X))&&g_player.Combostate == COMBO_NO && g_AttackState <= 30)
		{
			PlayerComb(MOTIONTYPE_ACTION, 40, 40, COMBO_ATTACK1); // コンボ1
		}
		else if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Combostate == COMBO_ATTACK1 && g_AttackState <= 30)
		{
			PlayerComb(MOTIONTYPE_ACTION2, 40, 40, COMBO_ATTACK2); // コンボ2
		}
		else if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Combostate == COMBO_ATTACK2 && g_AttackState <= 30)
		{
			PlayerComb(MOTIONTYPE_ACTION3, 40, 40, COMBO_ATTACK3); // コンボ3
		}
		else if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Combostate == COMBO_ATTACK3 && g_AttackState <= 30)
		{
			PlayerComb(MOTIONTYPE_ACTION4, 45, 40, COMBO_ATTACK4); // コンボ4
		}
	}

	// 投げ物を持っているときの攻撃
	if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Combostate == COMBO_NO && bNohand && !g_player.AttackSp)
	{
		PlayerComb(MOTIONTYPE_ACTION, 40, 20, COMBO_ATTACK1); // コンボ1
	}

	// モーションの種類が歩き
	if (g_player.Motion.motionType == MOTIONTYPE_MOVE)
	{
		// キ一1番目かつカウントが5
		if (g_player.Motion.nKey == 1 && g_player.Motion.nCountMotion == 5)
		{
			SetExplosion(D3DXVECTOR3(g_player.Motion.aModel[14].mtxWorld._41, g_player.Motion.aModel[14].mtxWorld._42, g_player.Motion.aModel[14].mtxWorld._43),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 20.0f, 20.0f, 0);
		}
		// キ一3番目かつカウントが5
		else if (g_player.Motion.nKey == 3 && g_player.Motion.nCountMotion == 5)
		{
			SetExplosion(D3DXVECTOR3(g_player.Motion.aModel[11].mtxWorld._41, g_player.Motion.aModel[11].mtxWorld._42, g_player.Motion.aModel[11].mtxWorld._43),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 20.0f, 20.0f, 0);
		}
	}

	//プレイヤーの角度の正規化
	if (g_player.rotDestPlayer.y - g_player.rot.y >= D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDestPlayer.y - g_player.rot.y <= -D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	// フォーバーモード
	if (g_player.FeverMode)
	{
		g_player.speed = g_player.fStockSpeed * 1.8f;
		if (g_player.Motion.motionType == MOTIONTYPE_MOVE)
		{
			g_player.Motion.nCountMotion++;
		}
		g_player.nDamage = g_player.nStockDamage * 1.5f;
	}
	else
	{
		// ステータスをもとに戻す
		g_player.speed = g_player.fStockSpeed;
		g_player.nDamage = g_player.nStockDamage;
	}

	// 武器を持っているかつプレイヤーの持っているアイテムが壊れた
	if (g_player.Motion.nNumModel == 16 && g_player.Itembreak[g_player.ItemIdx])
	{
		// モーションをニュートラルにする
		SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, MOTIONTYPE_NEUTRAL, true, 40); // モーションをニュートラルにする

		// モーションを歩きにする(第2引数に1を入れる)
		MotionChange(MOTION_DBHAND, 1);

		// 素手の時のモーション情報を代入
		for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
		{
			g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].Motion.aModel[nCntModel]; // モデルの情報を代入
		}
		for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
		{
			g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].Motion.aMotionInfo[nCntMotion];
		}

		// 投げた後に武器を消す
		g_player.Motion.nNumModel -= 1;

		// プレイヤーの状態を何も持っていない状態にする
		g_player.HandState = PLAYERHOLD_NO;

		// 状態を変更
		pItem[g_player.ItemIdx].state = ITEMSTATE_NORMAL;

	}

	// 持っているアイテムを置く処理
	if (g_player.Motion.nNumModel == 16 && (KeyboardTrigger(DIK_G) || JoypadTrigger(JOYKEY_Y)))
	{
		// モーションを歩きにする(第2引数に1を入れる)
		MotionChange(MOTION_DBHAND, 1);

		// 素手の時のモーション情報を代入
		for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
		{
			g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].Motion.aModel[nCntModel]; // モデルの情報を代入
		}
		for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
		{
			g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].Motion.aMotionInfo[nCntMotion];
		}

		// 投げた後に武器を消す
		g_player.Motion.nNumModel -= 1;

		// プレイヤーの状態を何も持っていない状態にする
		g_player.HandState = PLAYERHOLD_NO;

		SetItem(g_player.pos, pItem[g_player.ItemIdx].nType, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		pItem[g_player.ItemIdx].state = ITEMSTATE_NORMAL;
	}	

	// 大型武器モーションの時
	if (g_player.WeponMotion == MOTION_BIGWEPON && g_player.Motion.motionType == MOTIONTYPE_ACTION4 && g_player.Motion.nKey == 0 && GetKeyboardPress(DIK_W))
	{
		g_player.move.x = sinf(g_player.rot.y + D3DX_PI) * 30.0f;
		g_player.move.z = cosf(g_player.rot.y + D3DX_PI) * 30.0f;
	}

	static int FiverCnt = 0; // 回数制限

	if (g_player.FeverMode && FiverCnt == 0 && !g_player.AttackSp)
	{
		SetGameUI(D3DXVECTOR3(620.0f, 360.0f, 0.0f), UITYPE_SYUTYUSEN, 660.0f, 380.0f, 0);
		SetGameUI(D3DXVECTOR3(640.0f, 650.0f, 0.0f), UITYPE_FIVER, 200.0f, 80.0f, 0);
		FiverCnt = 1; // 制限回数を超えた
	}
	if (!g_player.FeverMode)
	{
		FiverCnt = 0; // 制限回数をリセット
	}

	// スペシャルモードになった時の攻撃
	if ((KeyboardTrigger(DIK_Q) || JoypadTrigger(JOYKEY_LS) || JoypadTrigger(JOYKEY_RS)) && g_player.Motion.nNumModel == 16 && g_player.HandState != PLAYERHOLD_HOLD)
	{
		if (g_player.Combostate == COMBO_NO && // 攻撃していない
			!g_player.AttackSp &&              // SP技を発動していない
			g_player.SpMode)                   // SPゲージがたまった
		{

			g_player.AttackSp = true;          // SP技を発動している

			switch (g_player.WeponMotion)
			{
			case MOTION_KATANA:
				g_player.SwordOffpos.y = 250.0f;
				MotionChange(MOTION_SP, 0);
				SetGameUI(D3DXVECTOR3(640.0f, 360.0f, 0.0f), UITYPE_BLACK, 640.0f, 380.0f, 0);
				PlayerComb(MOTIONTYPE_ACTION, 120, 120, COMBO_ATTACK1); // コンボ1
				break;
			case MOTION_BIGWEPON:
				g_player.SwordOffpos.y = 100.0f;
				MotionChange(MOTION_SPHAMMER, 0);
				PlayerComb(MOTIONTYPE_ACTION, 120, 120, COMBO_ATTACK1); // コンボ1
				break;
			case MOTION_DBHAND:
				g_player.SwordOffpos.y = 250.0f;
				MotionChange(MOTION_SPDOUBLE, 0);
				PlayerComb(MOTIONTYPE_ACTION, 240, 120, COMBO_ATTACK1); // コンボ1
				break;
			case MOTION_ONE_HAND:
				g_player.SwordOffpos.y = 250.0f;
				MotionChange(MOTION_ONEHANDBLOW, 0);
				PlayerComb(MOTIONTYPE_ACTION, 120, 120, COMBO_ATTACK1); // コンボ1
				break;
			case MOTION_PIERCING:
				g_player.SwordOffpos.y = 200.0f;
				MotionChange(MOTION_SPPIERCING, 0);
				PlayerComb(MOTIONTYPE_ACTION, 180, 120, COMBO_ATTACK1); // コンボ1
				break;
			default:
				break;
			}

			// 音楽を再生
			PlaySound(SOUND_LABEL_SP_SE);

		}
	}

	// スペシャルモーションを発動したら
	if (g_player.Motion.motionType == MOTIONTYPE_ACTION && g_player.AttackSp)
	{
		// パーティクル
		SetParticle(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 25, g_player.pos.z), D3DXVECTOR3(D3DX_PI / 2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 2.0f, 1, 20, 10, 20.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	}

	// スペシャルモーションからもとに戻す
	if (g_player.AttackSp && g_player.Motion.nKey >= g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION].nNumkey - 1)
	{
		SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, MOTIONTYPE_NEUTRAL, true, 40); // モーションをニュートラルにする
		g_player.SwordOffpos.y = 65.0f;		// 判定の長さを戻す
		MotionChange(MOTION_DBHAND, 1);		// 素手に戻す
		g_player.Motion.nNumModel = 15;		// 武器を消す
		g_player.HandState = PLAYERHOLD_NO; // 何も持っていない状態にする
		g_player.AttackSp = false;
		pItem[g_player.ItemIdx].state = ITEMSTATE_NORMAL;
	}

	if (g_player.WeponMotion == MOTION_SPHAMMER && g_player.AttackSp && g_player.Motion.nKey <= 15)
	{
		g_player.speed = 7.0f; // スピードを設定
		g_player.move.x += sinf(g_player.rot.y + D3DX_PI) * g_player.speed; // 移動量を加算
		g_player.move.z += cosf(g_player.rot.y + D3DX_PI) * g_player.speed; // 移動量を加算
		SetParticle(SwordPos,
			g_player.rot,
			D3DXVECTOR3(3.14f, 3.14f, 3.14f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f),
			2.0f, 4, 30, 60, 4.0f, 0.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (g_player.WeponMotion == MOTION_SPPIERCING &&		// 槍のspモーションだったら
		g_player.AttackSp && g_player.Motion.nKey == 21 &&  // モーションのキーが21番目
		g_player.Motion.nCountMotion == 1)                  // モーションのフレームが1番目
	{
		SetParticle(g_player.pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(1.57f, 3.14f, 1.57f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			6.0f, 4, 80, 500, 6.0f, 30.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}	//モーションの更新

	// アイテムのストック
	if (g_player.PlayerType == PLAYERTYPE_NOHAND && g_player.HandState == PLAYERHOLD_HOLD && KeyboardTrigger(DIK_F))
	{
		pItem[g_player.ItemIdx].state = ITEMSTATE_STOCK;

		// モーションを歩きにする(第2引数に1を入れる)
		MotionChange(MOTION_DBHAND, 1);

		// 素手の時のモーション情報を代入
		for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
		{
			g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].Motion.aModel[nCntModel]; // モデルの情報を代入
		}
		for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
		{
			g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].Motion.aMotionInfo[nCntMotion];
		}

		// 投げた後に武器を消す
		g_player.Motion.nNumModel -= 1;

		// プレイヤーの状態を何も持っていない状態にする
		g_player.HandState = PLAYERHOLD_NO;
	}

	UpdateMotion(&g_player.Motion);

	//プレイヤーの向きを目的の向きに近づける
	g_player.rot.y += (g_player.rotDestPlayer.y - g_player.rot.y) * 0.1f;
}
//===============================================================================================================
// プレイヤーの描画処理
//===============================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans,mtxSize,mtxShadow;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	int nCnt = 0;

	if (g_player.bDisp == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.mtxWorldPlayer);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
		D3DXMatrixMultiply(&g_player.mtxWorldPlayer, &g_player.mtxWorldPlayer, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
		D3DXMatrixMultiply(&g_player.mtxWorldPlayer, &g_player.mtxWorldPlayer, &mtxTrans);

		//D3DXMatrixShadow(&mtxShadow, &Light, &plane);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorldPlayer);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//全モデルパーツの描画
		for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransform;//計算用
			D3DXMATRIX mtxParent;//親のマトリックス

			//パーツのマトリックスの初期化
			D3DXMatrixIdentity(&g_player.Motion.aModel[nCntModel].mtxWorld);

			//パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.Motion.aModel[nCntModel].rot.y, g_player.Motion.aModel[nCntModel].rot.x, g_player.Motion.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_player.Motion.aModel[nCntModel].mtxWorld, &g_player.Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

			//パーツの位置(オフセット)を反映
			D3DXMatrixTranslation(&mtxTransform, g_player.Motion.aModel[nCntModel].pos.x, g_player.Motion.aModel[nCntModel].pos.y, g_player.Motion.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_player.Motion.aModel[nCntModel].mtxWorld, &g_player.Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

			//パーツの[親のマトリックス]を設定
			if (g_player.Motion.aModel[nCntModel].nIdxModelParent != -1)
			{
				//親モデルがある場合
				mtxParent = g_player.Motion.aModel[g_player.Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{//親モデルがない場合
				mtxParent = g_player.mtxWorldPlayer;
			}

			//算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
			D3DXMatrixMultiply(&g_player.Motion.aModel[nCntModel].mtxWorld,
				&g_player.Motion.aModel[nCntModel].mtxWorld,
				&mtxParent);//自分自分親

			//パーツのワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD,
				&g_player.Motion.aModel[nCntModel].mtxWorld);


			for (int nCntMat = 0; nCntMat < (int)g_player.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				//マテリアルのデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_player.Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

				D3DXMATERIAL color;

				if (g_player.state != PLAYERSTATE_DAMAGE)
				{
					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				else if (g_player.state == PLAYERSTATE_DAMAGE)
				{
					color = pMat[nCntMat];

					color.MatD3D.Diffuse.r = 1.0f;
					color.MatD3D.Diffuse.g = 0.3f;
					color.MatD3D.Diffuse.b = 0.3f;
					color.MatD3D.Diffuse.a = 1.0f;

					//マテリアルの設定
					pDevice->SetMaterial(&color.MatD3D);
				}

				//テクスチャの設定
				pDevice->SetTexture(0, g_player.Motion.aModel[nCntModel].pTexture[nCntMat]);

				//モデル(パーツ)の描画
				g_player.Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}

			nCnt++;
			if (nCnt == 15)
			{
				SetMtxPos(); // 剣のワールドマトリックスを設定
			}
		}
	}
	//マテリアルの設定
	pDevice->SetMaterial(&matDef);
}
//===============================================================================================================
//プレイヤーの取得処理
//===============================================================================================================
Player* GetPlayer(void)
{
	return &g_player;
}
//===============================================================================================================
////ワールドマトリックスのオフセット設定処理
//===============================================================================================================
void SetMtxPos(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans,mtxParent;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.SwordMtx);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.Motion.aModel[15].rot.y,
		g_player.Motion.aModel[15].rot.x,
		g_player.Motion.aModel[15].rot.z);

	D3DXMatrixMultiply(&g_player.SwordMtx,&g_player.SwordMtx,&mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.SwordOffpos.x,g_player.SwordOffpos.y,g_player.SwordOffpos.z);
	D3DXMatrixMultiply(&g_player.SwordMtx,&g_player.SwordMtx, &mtxTrans);

	mtxParent = g_player.Motion.aModel[15].mtxWorld;

	//算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
	D3DXMatrixMultiply(&g_player.SwordMtx,
		&g_player.SwordMtx,
		&mtxParent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.SwordMtx);
}
//===============================================================================================================
// プレイヤーと敵の判定
//===============================================================================================================
void HitPlayer(int nDamage)
{
	if (!g_player.AttackSp)
	{
		g_player.nLife -= nDamage;

		if (g_player.nLife <= 0)
		{
			g_player.state = PLAYERSTATE_FALL;
			g_player.bDisp = false;
			KillShadow(g_player.nIdxShadow); // 影を消す
		}
		else
		{
			g_nCounterState = 30;
			g_player.state = PLAYERSTATE_DAMAGE;
		}
	}
}
//===============================================================================================================
// プレイヤーのスティック操作
//===============================================================================================================
void StickPad(void)
{
	XINPUT_STATE* pStick;

	pStick = GetJoyStickAngle();

	Camera* pCamera = GetCamera();

	if (GetJoyStick() == true && g_player.Combostate == COMBO_NO)
	{
		float LStickAngleY = pStick->Gamepad.sThumbLY;
		float LStickAngleX = pStick->Gamepad.sThumbLX;

		float deadzone = 10920;
		float magnitude = sqrtf(LStickAngleX * LStickAngleX + LStickAngleY * LStickAngleY);

		if (magnitude > deadzone)
		{
			bUsePad = true;
			float normalizeX = (LStickAngleX / magnitude);
			float normalizeY = (LStickAngleY / magnitude);

			float moveX = normalizeX * cosf(-pCamera->rot.y) - normalizeY * sinf(-pCamera->rot.y);
			float moveZ = normalizeX * sinf(-pCamera->rot.y) + normalizeY * cosf(-pCamera->rot.y);

			g_player.move.x += moveX * g_player.speed;
			g_player.move.z += moveZ * g_player.speed;

			g_player.rotDestPlayer.y = atan2f(-moveX, -moveZ);
			
			if (bUsePad)
			{
				g_player.Motion.motionType = MOTIONTYPE_MOVE;
			}
			
		}
		else
		{
			g_player.Motion.motionType = MOTIONTYPE_NEUTRAL;
		}
	}
}
//===============================================================================================================
// プレイヤーのステータス変更
//===============================================================================================================
void StatusChange(float speed, D3DXVECTOR3 SwordOffpos, int nDamage)
{
	g_player.speed = speed; // 足の速さ
	g_player.fStockSpeed = speed; // 足の速さの計算用

	// 剣の当たり判定の広さ
	g_player.SwordOffpos.x = SwordOffpos.x; 
	g_player.SwordOffpos.y = SwordOffpos.y;
	g_player.SwordOffpos.z = SwordOffpos.z;

	g_player.nDamage = nDamage; // プレイヤーの攻撃力
	g_player.nStockDamage = nDamage; // プレイヤーの攻撃力計算用

}
//===============================================================================================================
// プレイヤーの剣と敵の当たり判定
//===============================================================================================================
void HitSowrd(ENEMY* pEnemy,int nCntEnemy)
{
	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	D3DXVECTOR3 mtxDis,SwordPos;

	// プレイヤーが武器を持っているかつスペシャル攻撃じゃない
	if (g_player.Motion.nNumModel == 16 && !g_player.AttackSp)
	{
		//剣の長さを求める
		mtxDis.x = (g_player.SwordMtx._41 - g_player.Motion.aModel[15].mtxWorld._41);
		mtxDis.y = (g_player.SwordMtx._42 - g_player.Motion.aModel[15].mtxWorld._42);
		mtxDis.z = (g_player.SwordMtx._43 - g_player.Motion.aModel[15].mtxWorld._43);

		// マトリクスの数分だけ回す
		for (int nCnt = 0; nCnt < NUM_MTX; nCnt++)
		{
			// 剣の位置を全て求める
			SwordPos.x = g_player.Motion.aModel[15].mtxWorld._41 + mtxDis.x * 0.125f * nCnt;
			SwordPos.y = g_player.Motion.aModel[15].mtxWorld._42 + mtxDis.y * 0.125f * nCnt;
			SwordPos.z = g_player.Motion.aModel[15].mtxWorld._43 + mtxDis.z * 0.125f * nCnt;

			D3DXVECTOR3 DisPos; // 距離算出用

			DisPos.x = pEnemy->pos.x - SwordPos.x; // 距離Xを求める
			DisPos.y = pEnemy->pos.y - SwordPos.y; // 距離Yを求める
			DisPos.z = pEnemy->pos.z - SwordPos.z; // 距離Zを求める

			float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z); // 距離を求める

			float Radius1, Radius2; // 半径

			Radius1 = 15.0f;
			Radius2 = 50.0f;

			float fRadius = Radius1 + Radius2; // 半径を求める

			fRadius = (fRadius * fRadius); // 半径を求める

			// 範囲内に入った
			if (fDistance <= fRadius && pEnemy->state != ENEMYSTATE_DAMAGE && g_player.Combostate != COMBO_NO)
			{
				// 敵に当たった
				HitEnemy(nCntEnemy, (pPlayer->nDamage * 5));

				// 耐久絵欲を減らす
				pItem[g_player.ItemIdx].durability--;

				// アイテムの耐久力が0になったら
				if (pItem[g_player.ItemIdx].durability <= 0)
				{
					// 0になったアイテムを消す
					g_player.Itembreak[g_player.ItemIdx] = true;
				}
				break;
			}
		}
	}
	// 素手の時かつ攻撃がスペシャルじゃない
	else if(g_player.Motion.nNumModel == 15 && !g_player.AttackSp)
	{
		// モデルの位置を変数に代入
		D3DXVECTOR3 ModelPos(g_player.Motion.aModel[4].mtxWorld._41, g_player.Motion.aModel[4].mtxWorld._42, g_player.Motion.aModel[4].mtxWorld._43);

		// 円の範囲
		if (sphererange(&ModelPos, &pEnemy->pos, 30.0f, 65.0f) && g_player.Combostate != COMBO_NO && pEnemy->state!=ENEMYSTATE_DAMAGE)
		{
			if (g_player.Motion.motionType == MOTIONTYPE_ACTION && g_player.Motion.nKey >= 2)
			{
				HitEnemy(nCntEnemy, g_player.nDamage * 3); // 敵に当たった
			}
		}
	}
	// 剣を持っているかつスペシャル攻撃中
	else if (g_player.Motion.nNumModel == 16 && g_player.AttackSp)
	{
		//剣の長さを求める
		mtxDis.x = (g_player.SwordMtx._41 - g_player.Motion.aModel[15].mtxWorld._41);
		mtxDis.y = (g_player.SwordMtx._42 - g_player.Motion.aModel[15].mtxWorld._42);
		mtxDis.z = (g_player.SwordMtx._43 - g_player.Motion.aModel[15].mtxWorld._43);

		// マトリクスの数分だけ回す
		for (int nCnt = 0; nCnt < NUM_MTX; nCnt++)
		{
			// 剣の位置を全て求める
			SwordPos.x = g_player.Motion.aModel[15].mtxWorld._41 + mtxDis.x * 0.125f * nCnt;
			SwordPos.y = g_player.Motion.aModel[15].mtxWorld._42 + mtxDis.y * 0.125f * nCnt;
			SwordPos.z = g_player.Motion.aModel[15].mtxWorld._43 + mtxDis.z * 0.125f * nCnt;

			D3DXVECTOR3 DisPos; // 距離算出用

			DisPos.x = pEnemy->pos.x - SwordPos.x; // 距離Xを求める
			DisPos.y = pEnemy->pos.y - SwordPos.y; // 距離Yを求める
			DisPos.z = pEnemy->pos.z - SwordPos.z; // 距離Zを求める

			float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z); // 距離を求める

			float Radius1, Radius2; // 半径

			Radius1 = 200.0f;
			Radius2 = 50.0f;

			float fRadius = Radius1 + Radius2; // 半径を求める

			fRadius = (fRadius * fRadius); // 半径を求める

			// 範囲内にいたら
			if (fDistance <= fRadius &&g_player.WeponMotion != MOTION_SPPIERCING && g_player.WeponMotion != MOTION_SPDOUBLE&&
				pEnemy->state != ENEMYSTATE_DAMAGE && g_player.Combostate != COMBO_NO && g_player.Motion.nKey >= 3)
			{
				// 敵にダメージを与える
				HitEnemy(nCntEnemy, (pPlayer->nDamage * 50));
				break;
			}
			// 範囲内にいたら
			if (sphererange(&g_player.pos, &pEnemy->pos, 200.0f, 50.0f) && g_player.WeponMotion == MOTION_SPPIERCING &&
				pEnemy->state != ENEMYSTATE_DAMAGE && g_player.Combostate != COMBO_NO && g_player.Motion.nKey >= 19)
			{
				// 敵にダメージを与える
				HitEnemy(nCntEnemy, (pPlayer->nDamage * 50));
				break;
			}

			// 範囲内にいたら
			if (sphererange(&g_player.pos, &pEnemy->pos, 200.0f, 50.0f) && g_player.WeponMotion == MOTION_SPDOUBLE &&
				pEnemy->state != ENEMYSTATE_DAMAGE && g_player.Combostate != COMBO_NO && g_player.Motion.nKey >= 6)
			{
				// 敵にダメージを与える
				HitEnemy(nCntEnemy, (pPlayer->nDamage * 50));
				break;
			}
		}

	}
}
//===============================================================================================================
// 物を投げる
//===============================================================================================================
void ThrowItem(void)
{
	Item* pItem = GetItem();
	ENEMY* pEnemy = GetEnemy();

	int nIdx = g_player.ItemIdx; // 手に持っているアイテムのインデックス情報を代入
	int nIdxEnemy = 0;

	float fDistanceNow = 0.0f;
	float fDistanceStock = 0.0f;
	float fDistance = 0.0f;

	bool bFirst = true;

	// 発射地点を設定
	pItem[nIdx].pos.x = g_player.pos.x;
	pItem[nIdx].pos.y = g_player.Motion.aModel[2].pos.y;
	pItem[nIdx].pos.z = g_player.pos.z;

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (pEnemy[nCnt].bUse)
		{
			// 距離を求める
			float DisposX = pEnemy[nCnt].pos.x - g_player.pos.x;
			float DisposY = pEnemy[nCnt].pos.y - g_player.pos.y;
			float DisposZ = pEnemy[nCnt].pos.z - g_player.pos.z;

			// 距離を求める
			fDistanceNow = sqrtf((DisposX * DisposX) + (DisposY * DisposY) + (DisposZ * DisposZ));

			// 最初だけ通す
			if (bFirst)
			{
				fDistanceStock = fDistanceNow; // ストックに最初の値を入れておく
				bFirst = false; // 最初しか通したくないのでfalse
				nIdxEnemy = nCnt; // インデックスを保存
			}
			else
			{
				// 今の距離がストックされた距離より小さかったら
				if (fDistanceNow < fDistanceStock)
				{
					fDistanceStock = fDistanceNow; // 距離を保存
					nIdxEnemy = nCnt; // 近い敵のインデックスを保存
				}
			}

			if (sphererange(&g_player.pos, &pEnemy[nCnt].pos, 50.0f, 300.0f) && pEnemy[nCnt].nType == ENEMYTYPE_SIX)
			{
				nIdxEnemy = nCnt;
				break;
			}
		}
	}

	Boss* pBoss = Getboss();

	// 全ボス分回す
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (sphererange(&g_player.pos, &pEnemy[nIdxEnemy].pos,50.0f, 300.0f) && pEnemy[nIdxEnemy].nType == ENEMYTYPE_SIX)
		{
			D3DXVECTOR3 dest = pEnemy[nIdxEnemy].pos - g_player.pos; // 近い敵の方向を求める
			D3DXVec3Normalize(&dest, &dest); // 正規化する

			// 飛ばす方向を設定
			pItem[nIdx].move.x = dest.x * 10.0f;
			pItem[nIdx].move.y = dest.y * 10.0f;
			pItem[nIdx].move.z = dest.z * 10.0f;
			pItem[nIdx].bUse = true; // 使用状態をtrueにする

				// プレイヤーの向きを一番近い敵の場所にする
			float fAngle = atan2f(pEnemy[nIdxEnemy].pos.x - g_player.pos.x, pEnemy[nIdxEnemy].pos.z - g_player.pos.z);
			g_player.rotDestPlayer.y = fAngle + D3DX_PI;
			break;
		}
		// ボスが範囲内にいたら
		else if (sphererange(&g_player.pos, &pBoss[nCntBoss].pos, 50.0f, 150.0f) && pBoss[nCntBoss].bUse)
		{
			D3DXVECTOR3 dest = pBoss[nCntBoss].pos - g_player.pos; // 近いボスの方向を求める
			D3DXVec3Normalize(&dest, &dest); // 正規化する

			// 飛ばす方向を設定
			pItem[nIdx].move.x = dest.x * 10.0f;
			pItem[nIdx].move.z = dest.z * 10.0f;
			pItem[nIdx].bUse = true; // 使用状態をtrueにする

				// プレイヤーの向きを一番近いボスの場所にする
			float fAngle = atan2f(pBoss[nCntBoss].pos.x - g_player.pos.x, pBoss[nCntBoss].pos.z - g_player.pos.z);
			g_player.rotDestPlayer.y = fAngle + D3DX_PI;

			break;
		}
		// 敵が範囲内にいたら
		else if (sphererange(&g_player.pos, &pEnemy[nIdxEnemy].pos, 50.0f, 300.0f))
		{
			D3DXVECTOR3 dest = pEnemy[nIdxEnemy].pos - g_player.pos; // 近い敵の方向を求める
			D3DXVec3Normalize(&dest, &dest); // 正規化する

			// 飛ばす方向を設定
			pItem[nIdx].move.x = dest.x * 10.0f;
			pItem[nIdx].move.z = dest.z * 10.0f;
			pItem[nIdx].bUse = true; // 使用状態をtrueにする

				// プレイヤーの向きを一番近い敵の場所にする
			float fAngle = atan2f(pEnemy[nIdxEnemy].pos.x - g_player.pos.x, pEnemy[nIdxEnemy].pos.z - g_player.pos.z);
			g_player.rotDestPlayer.y = fAngle + D3DX_PI;
			break;

		}
		// 範囲内に誰もいなかったら
		else
		{
			// 飛ばす方向を設定
			pItem[nIdx].move.x = sinf(g_player.rot.y + D3DX_PI) * 10.0f;
			pItem[nIdx].move.z = cosf(g_player.rot.y + D3DX_PI) * 10.0f;
			pItem[nIdx].bUse = true; // 使用状態をtrueにする
		}
		
		
	}

	// 素手の時のモーション情報を代入
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
	{
		g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].Motion.aModel[nCntModel]; // モデルの情報を代入
	}
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].Motion.aMotionInfo[nCntMotion];
	}

	// 投げた後に武器を消す
	g_player.Motion.nNumModel -= 1;

	// プレイヤーの状態を何も持っていない状態にする
	g_player.HandState = PLAYERHOLD_NO;

	// 状態を投げられてる状態にする
	pItem[nIdx].state = ITEMSTATE_THROW;

	g_player.speed = 3.0f; // プレイヤーの移動速度をリセット
	g_player.fStockSpeed = 3.0f; // プレイヤーの移動速度を保存
}
//===============================================================================================================
// プレイヤーと敵の当たり判定
//===============================================================================================================
void CollisionPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pMove, float PLradius, float ENradius)
{
	D3DXVECTOR3 DisPos; // 計算用

	// 距離を計算
	DisPos.x = g_player.pos.x - pPos->x;
	DisPos.y = g_player.pos.y - pPos->y;
	DisPos.z = g_player.pos.z - pPos->z;

	// 距離を求める
	float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z);

	// 半径を計算
	float fRadius = PLradius + ENradius;

	// 半径を求める
	fRadius = fRadius * fRadius;

	// 範囲内に入った
	if (fDistance <= fRadius && g_player.state != PLAYERSTATE_DAMAGE)
	{
		// 音楽再生
		PlaySound(SOUND_LABEL_DAMAGE_SE);

		// 敵を戻す
		pMove->x -= DisPos.x * 0.1f;
		pMove->z -= DisPos.z * 0.1f;
		HitPlayer(50);
	}
}
//===============================================================================================================
// 円の判定
//===============================================================================================================
bool sphererange(D3DXVECTOR3* pPos1, D3DXVECTOR3* pPos2, float radius1, float radius2) // 円の当たり判定
{
	bool bRange = false;

	D3DXVECTOR3 DisPos; // 計算用

	// 距離XYZを求める
	DisPos.x = pPos1->x - pPos2->x;
	DisPos.y = pPos1->y - pPos2->y;
	DisPos.z = pPos1->z - pPos2->z;

	// 距離を求める
	float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z);

	// 半径を求める
	float fRadius = radius1 + radius2;

	// 半径を求める
	fRadius = fRadius * fRadius;

	if (fDistance <= fRadius)
	{
		bRange = true;
	}
	return bRange;
}

//===============================================================================================================
// プレイヤーとアイテムの判定
//===============================================================================================================
bool CollisionItem(int nIdx, float Itemrange, float plrange)
{
	Item* pItem = GetItem();

	bool bCollision = false; // 当たっているかどうか

	float fDistanceX = g_player.pos.x - pItem[nIdx].pos.x; // 距離Xを計算
	float fDistanceY = g_player.pos.y - pItem[nIdx].pos.y; // 距離Yを計算
	float fDistanceZ = g_player.pos.z - pItem[nIdx].pos.z; // 距離Zを計算

	//距離を算出
	float fDistance = (fDistanceX * fDistanceX) + (fDistanceY * fDistanceY) + (fDistanceZ * fDistanceZ);

	// 半径を計算
	float Radius = Itemrange + plrange;

	// 半径を算出
	Radius = Radius * Radius;

	// 範囲内に入った
	if (fDistance <= Radius)
	{
		bCollision = true;

		if ((KeyboardTrigger(DIK_E) || JoypadTrigger(JOYKEY_RIGHT_B) || JoypadTrigger(JOYKEY_LEFT_B) || OnMouseTriggerDown(RIGHT_MOUSE)) && g_player.Combostate == COMBO_NO)
		{
			g_player.Motion.motionType = MOTIONTYPE_NEUTRAL;
			// 音楽再生
			PlaySound(SOUND_LABEL_ITEM_SE);

			Itemchange(pItem[nIdx].nType); // アイテムを拾う

			pItem[nIdx].bUse = false;      // 消す

			pItem[nIdx].state = ITEMSTATE_HOLD;

			g_player.ItemIdx = nIdx;	   // インデックスを渡す
			//g_player.nOldItem = pItem[nIdx].nType;	   // インデックスを渡す

			switch (pItem[nIdx].nType)
			{
			case ITEMTYPE_BAT:
				MotionChange(MOTION_DBHAND, 0); // アイテムにあったモーションタイプを入れる(素手の場合は引数2に1を入れる)
				StatusChange(3.5f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 70);
				break;
			case ITEMTYPE_GOLF:
				MotionChange(MOTION_KATANA, 0);
				StatusChange(3.5f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 60);
				break;
			case ITEMTYPE_HUNMER:
				MotionChange(MOTION_BIGWEPON, 0);
				StatusChange(2.8f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 80);
				break;
			case ITEMTYPE_STONE:
				MotionChange(MOTION_BIGWEPON, 1);
				StatusChange(3.5f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 50);
				break;
			case ITEMTYPE_WOOD:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 60);
				break;
			case ITEMTYPE_STONEBAT:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 90);
				break;
			case ITEMTYPE_LIGHT:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(3.7f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 60);
				break;
			case ITEMTYPE_LIGHTWOOD:
				MotionChange(MOTION_KATANA, 0);
				StatusChange(3.4f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 60);
				break;
			case ITEMTYPE_HARISEN:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(3.4f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 60);
				break;
			case ITEMTYPE_ICEBLOCK:
				MotionChange(MOTION_DBHAND, 1);
				StatusChange(3.4f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 50);
				break;
			case ITEMTYPE_ICEBLOCKSOWRD:
				MotionChange(MOTION_KATANA, 0);
				StatusChange(2.9f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 100);
				break;
			case ITEMTYPE_IRON:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 90);
				break;
			case ITEMTYPE_IRONBAT:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 90);
				break;
			case ITEMTYPE_SURFBOARD:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(2.8f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 80);
				break;
			case ITEMTYPE_TORCH:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 50);
				break;
			case ITEMTYPE_TORCHSWORD:
				MotionChange(MOTION_KATANA, 0);
				StatusChange(2.9f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 100);
				break;
			case ITEMTYPE_BAR:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 90);
				break;
			case ITEMTYPE_HEADSTATUE:
				MotionChange(MOTION_BIGWEPON, 1);
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 50);
				break;
			case ITEMTYPE_HEADSTATUTORSO:
				MotionChange(MOTION_BIGWEPON, 0);
				StatusChange(2.9f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 80);
				break;
			case ITEMTYPE_MEGAPHONE:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 60);
				break;
			case ITEMTYPE_RUBBERCUP:
				MotionChange(MOTION_PIERCING, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 80);
				break;
			case ITEMTYPE_TELEPHONEPOLE:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(2.5f, D3DXVECTOR3(0.0f, 150.0f, 0.0f), 120);
				break;
			case ITEMTYPE_TORSO:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 70);
				break;
			case ITEMTYPE_FLUORESCENTLIGHTMEGAPHONE:
				MotionChange(MOTION_DBHAND, 1);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 50);
				break;
			case ITEMTYPE_BONESPEAR:
				MotionChange(MOTION_PIERCING, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 100);
				break;
			case ITEMTYPE_FISH:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(2.8f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 80);
				break;
			case ITEMTYPE_HEX:
				MotionChange(MOTION_DBHAND, 1);
				StatusChange(1.5f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 300);
				break;
			case ITEMTYPE_HEXMANDOLIN:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(2.5f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 100);
				break;
			case ITEMTYPE_SURFBOARDFISH:
				MotionChange(MOTION_BIGWEPON, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 80);
				break;
			case ITEMTYPE_TUTORIAL:
				MotionChange(MOTION_DBHAND, 1);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 50);
				break;
			case ITEMTYPE_KATANA:
				MotionChange(MOTION_KATANA, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 100);
				break;
			default:
				break;
			}
		}
	}

	return bCollision;
}

//===============================================================================================================
// プレイヤーのコンボ
//===============================================================================================================
void PlayerComb(MOTIONTYPE motiontype, int AttackState, int nCounterState, COMBOSTATE Combstate)
{
	Camera* pCamera = GetCamera();

	ENEMY* pEnemy = GetEnemy();
	float fDistanceNow = 0.0f;
	float fDistanceStock = 0.0f;
	bool bFirst = true;
	int nIdxEnemy = 0;

	g_player.Motion.nKey = 0;                 // キーを0から始める
	g_player.Motion.nCountMotion = 0;	      // モーションカウントを0から始める
	g_player.Motion.motionType = motiontype;  // モーションの種類を変更
	g_nCounterState = nCounterState;		  // 状態カウンターを設定
	g_AttackState = AttackState;			  // 攻撃状態カウンターを設定
	g_player.state = PLAYERSTATE_ATTACK;	  // プレイヤーの状態を攻撃にする	
	g_player.Combostate = Combstate;		  // コンボの状態を設定
	SetMotion(&g_player.Motion, motiontype, MOTIONTYPE_NEUTRAL, true, 28);

	// 敵の最大数分求める
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		// 使用状態のみ
		if (pEnemy[nCnt].bUse && pEnemy[nCnt].nType != ENEMYTYPE_SIX)
		{
			// 距離を求める
			float DisposX = pEnemy[nCnt].pos.x - g_player.pos.x;
			float DisposY = pEnemy[nCnt].pos.y - g_player.pos.y;
			float DisposZ = pEnemy[nCnt].pos.z - g_player.pos.z;

			// 距離を求める
			fDistanceNow = sqrtf((DisposX * DisposX) + (DisposY * DisposY) + (DisposZ * DisposZ));

			// 最初だけ通す
			if (bFirst)
			{
				fDistanceStock = fDistanceNow;
				bFirst = false;
				nIdxEnemy = nCnt;
			}
			else
			{
				// 今の距離がストックされた距離より小さかったら
				if (fDistanceNow < fDistanceStock)
				{
					fDistanceStock = fDistanceNow; // 距離を保存
					nIdxEnemy = nCnt; // 近い敵のインデックスを保存
				}
			}
		}
	}

	Boss* pBoss = Getboss();

	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		// ボスがプレイヤーの近くにいたら
		if (sphererange(&g_player.pos, &pBoss[nCnt].pos, 50.0f, 140.0f) && pBoss[nCnt].bUse)
		{
			// 範囲にいる間だけロックオン
			if (sphererange(&g_player.pos, &pBoss[nCnt].pos, 150.0f, 150.0f))
			{
				// 角度を求める
				float fAngle = atan2f(pBoss[nCnt].pos.x - g_player.pos.x, pBoss[nCnt].pos.z - g_player.pos.z);
				g_player.rotDestPlayer.y = fAngle + D3DX_PI;
			}

			// ボスの場所を向く
			if (g_player.WeponMotion == MOTION_DBHAND && g_player.Motion.motionType == MOTIONTYPE_ACTION2 && GetKeyboardPress(DIK_W))
			{
				g_player.move.x = sinf(g_player.rot.y + D3DX_PI) * 80.0f;
				g_player.move.z = cosf(g_player.rot.y + D3DX_PI) * 80.0f;
				break;
			}
		}
		else
		{
			// 範囲にいる間だけロックオン
			if (sphererange(&g_player.pos, &pEnemy[nIdxEnemy].pos, 150.0f, 150.0f))
			{
				// 角度を求める
				float fAngle = atan2f(pEnemy[nIdxEnemy].pos.x - g_player.pos.x, pEnemy[nIdxEnemy].pos.z - g_player.pos.z);
				g_player.rotDestPlayer.y = fAngle + D3DX_PI;
			}

			if (g_player.WeponMotion == MOTION_DBHAND && g_player.Motion.motionType == MOTIONTYPE_ACTION2 && GetKeyboardPress(DIK_W))
			{
				g_player.move.x = sinf(g_player.rot.y + D3DX_PI) * 80.0f;
				g_player.move.z = cosf(g_player.rot.y + D3DX_PI) * 80.0f;
				break;
			}
		}
	}
}
//===============================================================================================================
// プレイヤーのモーションの変更
//===============================================================================================================
void MotionChange(int itemtype,int LoadPlayer)
{
	g_player.Motion.nNumModel = 16; // 最大数をもとに戻す

	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
	{
		g_player.Motion.aModel[nCntModel] = g_LoadPlayer[LoadPlayer].Motion.aModel[nCntModel]; // モデルの情報を代入
	}
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[LoadPlayer].Motion.aMotionInfo[nCntMotion]; // モーションの情報を代入
	}

	if (LoadPlayer == PLAYERTYPE_WEPON) // プレイヤーがノーハンドだったら情報を代入しない
	{
		g_player.WeponMotion = itemtype; // アイテムごとのモーションタイプ
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION] = g_LoadMotion[itemtype].aMotionInfo[0];		// 攻撃1の情報を代入
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION2] = g_LoadMotion[itemtype].aMotionInfo[1];    // 攻撃2の情報を代入
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION3] = g_LoadMotion[itemtype].aMotionInfo[2];	// 攻撃3の情報を代入
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION4] = g_LoadMotion[itemtype].aMotionInfo[3];	// 攻撃4の情報を代入
		g_player.Motion.aModel[15].offpos = g_LoadMotion[itemtype].aModel[15].offpos;				// オフセットの情報を代入
		g_player.Motion.aModel[15].offrot = g_LoadMotion[itemtype].aModel[15].offrot;				// オフセットのの情報を代入
		g_player.HandState = PLAYERHOLD_NO;
		bNohand = false;// プレイヤーをノーハンドにする
		g_player.PlayerType = PLAYERTYPE_WEPON;
	}
	else if(LoadPlayer == PLAYERTYPE_NOHAND)
	{
		g_player.WeponMotion = itemtype; // アイテムごとのモーションタイプ
		g_player.HandState = PLAYERHOLD_HOLD;	// プレイヤーをノーハンドにする
		g_player.PlayerType = PLAYERTYPE_NOHAND;
		bNohand = true;
	}
	
}

//===============================================================================================================
// プレイヤーのモーションのロード
//===============================================================================================================
void LoadMotion(int Weponmotion)
{
	FILE* pFile; // ファイルのポインタ
	switch (Weponmotion)
	{
	case MOTION_KATANA:
		pFile = fopen("data\\MOTION_CHANGE\\motionSamurai2.txt", "r");
		break;
	case MOTION_BIGWEPON:
		pFile = fopen("data\\MOTION_CHANGE\\hammer.txt", "r");
		break;
	case MOTION_DBHAND:
		pFile = fopen("data\\MOTION_CHANGE\\bat.txt", "r");
		break;
	case MOTION_ONE_HAND:
		pFile = fopen("data\\MOTION_CHANGE\\onehand.txt", "r");
		break;
	case MOTION_PIERCING:
		pFile = fopen("data\\MOTION_CHANGE\\piercing.txt", "r");
		break;
	case MOTION_SP:
		pFile = fopen("data\\MOTION_CHANGE\\sp.txt", "r");
		break;
	case MOTION_SPHAMMER:
		pFile = fopen("data\\MOTION_CHANGE\\sphammer.txt", "r");
		break;
	case MOTION_ONEHANDBLOW:
		pFile = fopen("data\\MOTION_CHANGE\\sponehandblow.txt", "r");
		break;
	case MOTION_SPPIERCING:
		pFile = fopen("data\\MOTION_CHANGE\\sppiercing.txt", "r");
		break;
	case MOTION_SPDOUBLE:
		pFile = fopen("data\\MOTION_CHANGE\\doublehand.txt", "r");
		break;
	default:
		pFile = NULL;
		break;
	}

	int nCntMotion = 0;
	char Skip[3];
	int nCntKey = 0;
	int nCntPosKey = 0;
	int nCntRotkey = 0;

	if (pFile != NULL)
	{
		char aString[MAX_WORD];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "PARTSSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aModel[15].offpos.x);
						fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aModel[15].offpos.y);
						fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aModel[15].offpos.z);
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aModel[15].offrot.x);
						fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aModel[15].offrot.y);
						fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aModel[15].offrot.z);
					}
					else if (strcmp(&aString[0], "END_PARTSSET") == 0)
					{
						break;
					}
				}
			}
			
			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{// MOTIONSETを読み取ったら
				while (1)
				{
					// 文字を読み飛ばす
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "LOOP") == 0)
					{// LOOP を読み取ったら
						// 文字を読み飛ばす
						fscanf(pFile, "%s", &Skip[0]);
						// 値を代入
						fscanf(pFile, "%d", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].bLoop);
					}
					else if (strcmp(aString, "NUM_KEY") == 0)
					{// NUM_KEYを読み取ったら
						// 文字を読み飛ばす
						fscanf(pFile, "%s", &Skip[0]);
						// 値を代入
						fscanf(pFile, "%d", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].nNumkey);

						while (nCntKey < g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].nNumkey)
						{
							// 文字を読み飛ばす
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(aString, "KEYSET") == 0)
							{// KEYSETを読み取ったら

								while (1)
								{
									// 文字を読み飛ばす
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "FRAME") == 0)
									{// FRAMEを読み取ったら
										// 文字を読み飛ばす
										fscanf(pFile, "%s", &Skip[0]);

										fscanf(pFile, "%d", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
										break;
									}
								}

								while (1)
								{
									// 文字を読み飛ばす
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "KEY") == 0)
									{// KEYを読みとったら
										while (1)
										{
											// 文字を読み飛ばす
											fscanf(pFile, "%s", &aString[0]);

											if (strcmp(&aString[0], "POS") == 0)
											{// POSを読み取ったら
												// 文字を読み飛ばす
												fscanf(pFile, "%s", &Skip[0]);
												fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosX);
												fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosY);
												fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosZ);
												nCntPosKey++;		// インクリメント										
											}

											else if (strcmp(&aString[0], "ROT") == 0)
											{// ROTを読み取ったら
												// 文字を読み飛ばす
												fscanf(pFile, "%s", &Skip[0]);
												fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotX);
												fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotY);
												fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotZ);
												nCntRotkey++;		// インクリメント									
											}

											else if (strcmp(&aString[0], "END_KEY") == 0)
											{// END_KEYを読み取ったら
												break;
											}
										}
									}
									else if (strcmp(&aString[0], "END_KEYSET") == 0)
									{// END_KEYSETを読み取ったら
										nCntRotkey = 0;
										nCntPosKey = 0;
										nCntKey++;			// インクリメント
										break;
									}


								}

							}

						}
					}
					if (strcmp(&aString[0], "END_MOTIONSET") == 0)
					{// END_MOTIONSETを読み取ったら
						nCntMotion++;		// モーションの更新
						nCntKey = 0;		// 0から始める
						break;
					}
				}
			}

			if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	else
	{
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(Player.cpp)", MB_OK);
		return;
    }
	fclose(pFile);
}
//===============================================================================================================
// プレイヤーのロード処理
//===============================================================================================================
void LoadPlayer(int nType)
{
	FILE* pFile;
	char aStr[MAX_WORD] = {};
	char Skip[3] = {};

	int nNumModel = 0;
	int nLoadCnt = 0;
	int nNumParts = 0;

	switch (nType)
	{
	case 0:
		// ファイルを開く
		pFile = fopen("data/MOTION/motionSamurai.txt", "r");
		break;
	case 1:
		pFile = fopen("data/MOTION_CHANGE/motionSamurai_Shot.txt", "r");
		break;
	default:
		pFile = NULL; //NULLにする
		break;
	}

	if (pFile != NULL)
	{
		while (1)
		{
			// 文字列を読み込む
			fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], "#") == 0)
			{//コメントが来たら
				//処理を読み飛ばす
				continue;
			}

			// NUM_MODELを読み取ったら
			if (strcmp(&aStr[0], "NUM_MODEL") == 0)
			{
				fscanf(pFile, "%s", &Skip[0]);						// [=]を読み飛ばす
				fscanf(pFile, "%d", &nNumModel);					// モデルの最大数を代入
				g_LoadPlayer[nType].Motion.nNumModel = nNumModel;	// モデルの最大数を代入
			}

			// モデルの読み込みがまだ終わっていなかったら
			if (nLoadCnt < nNumModel)
			{
				// モデルの読み込んだ数を返す
				nLoadCnt = LoadFilename(pFile, nNumModel, &aStr[0], nType);
			}

			// CHARACTERSETを読み取ったら
			if (strcmp(&aStr[0], "CHARACTERSET") == 0)
			{
				// パーツの設定処理
				LoadCharacterSet(pFile, &aStr[0], nNumParts, nType);
			}

			// MOTIONSETを読み取ったら
			if (strcmp(&aStr[0], "MOTIONSET") == 0)
			{
				// モーションの設定処理
				LoadMotionSet(pFile, &aStr[0], nNumModel, nType);
			}

			// END_SCRIPTを読み取ったら
			if (strcmp(&aStr[0], "END_SCRIPT") == 0)
			{
				nCntMotion = 0; // モーションのカウントをリセットする
				break;          // While文を抜ける
			}
		}
	}
	else
	{
		// ファイルが開けなかったら
		MessageBox(NULL, "ファイルが開けません。", "エラー(LoadPlayer)", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);

}
//===============================================================================================================
// プレイヤーのモデルロード
//===============================================================================================================
int LoadFilename(FILE* pFile, int nNumModel, char* aString, int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL* pMat;//マテリアルへのポインタ

	//頂点座標の取得
	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマットのサイズ
	BYTE* pVtxBuff;	//頂点バッファへのポインタ

	char Skip[3] = {}; // [=]読み飛ばしよう変数

	int nCntLoadModel = 0; // モデルのロードのカウンター

	// カウントがモデル数より下だったら
	while (nCntLoadModel < nNumModel)
	{
		// 文字を読み取る
		fscanf(pFile, "%s", aString);

		// MODEL_FILENAMEを読み取ったら
		if (strcmp(aString, "MODEL_FILENAME") == 0)
		{
			fscanf(pFile, "%s", &Skip[0]); // [=]を読み飛ばす
			fscanf(pFile, "%s", aString);  // ファイル名を読み取る

			const char* FILE_NAME = {};    // ファイル名代入用変数

			FILE_NAME = aString;           // ファイル名を代入

			//Xファイルの読み込み
			D3DXLoadMeshFromX(FILE_NAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pBuffMat,
				NULL,
				&g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].dwNumMat,
				&g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pMesh);

			//マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//このファイル名を使用してテクスチャを読み込む
						//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pTexture[nCntMat]);
				}
			}

			//頂点数の取得
			nNumVtx = g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pMesh->GetNumVertices();

			//頂点フォーマットのサイズ取得
			sizeFVF = D3DXGetFVFVertexSize(g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pMesh->GetFVF());

			//頂点バッファのロック
			g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//頂点座標を比較してブロックの最小値,最大値を取得
				if (vtx.x < g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.x)
				{
					g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.y)
				{
					g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.z)
				{
					g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.x)
				{
					g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.y)
				{
					g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.z)
				{
					g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			////サイズを代入
			//g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			//g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			//g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

			//各モデルごとのサイズを代入
			g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].Size.x = g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.x - g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.x;
			g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].Size.y = g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.y - g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.y;
			g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].Size.z = g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.z - g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.z;

			//頂点バッファのアンロック
			g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pMesh->UnlockVertexBuffer();

			nCntLoadModel++; // モデルのカウントを増やす
		}
	}
	return nCntLoadModel; // モデルのカウントを返す

}
//===============================================================================================================
// プレイヤーのキャラクターセット
//===============================================================================================================
void LoadCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType)
{
	int nIdx = 0; // インデックス格納変数
	char Skip[3] = {}; // [=]読み飛ばし変数

	// END_CHARACTERSETを読み取ってなかったら
	while (strcmp(aString, "END_CHARACTERSET") != 0)
	{
		// 文字を読み取る
		fscanf(pFile, "%s", aString);

		// INDEXを読み取ったら
		if (strcmp(aString, "INDEX") == 0)
		{
			fscanf(pFile, "%s", &Skip[0]); // [=]読み飛ばす
			fscanf(pFile, "%d", &nIdx);    // インデックスを代入
		}
		// PARENTを読み取ったら
		else if (strcmp(aString, "PARENT") == 0)
		{
			// [=]読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// 親のインデックスを保存
			fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aModel[nIdx].nIdxModelParent);
		}
		// POSを読み取ったら
		else if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.x);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.y);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.z);
		}
		// ROTを読み取ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offrot.x);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offrot.y);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offrot.z);
		}
	}
}
//===============================================================================================================
// プレイヤーのモーションセット
//===============================================================================================================
void LoadMotionSet(FILE* pFile, char* aString, int nNumModel, int nType)
{
	char Skip[3] = {}; // [=]読み飛ばし変数

	while (1)
	{
		// 文字を読み取る
		fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0 || strcmp(aString, "<<") == 0)
		{// コメントが来たら
			// コメントを読み飛ばす
			continue;
		}

		// LOOPを読み通ったら
		if (strcmp(aString, "LOOP") == 0)
		{
			// [=]読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// ループするかしないか
			fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].bLoop);
		}
		// NUM_KEYを読み通ったら
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// キーの最大数を代入
			fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].nNumkey);
		}
		// KEYSETを読み通ったら
		if (strcmp(aString, "KEYSET") == 0)
		{
			// キーの設定処理
			LoadKeySet(pFile, aString, nType, nCntMotion);
		}
		// END_MOTIONSETを読み通ったら
		if (strcmp(aString, "END_MOTIONSET") == 0)
		{
			nCntMotion++; // モーションのカウントをリセット
			nKey = 0;     // キーをリセット
			break;
		}
	}
}
//===============================================================================================================
// プレイヤーのモーションのキーセット
//===============================================================================================================
void LoadKeySet(FILE* pFile, char* aString, int nType, int nCntMotion)
{
	char Skip[3] = {}; // [=]読み飛ばし変数
	int nCntPos = 0;   // 位置のカウント
	int nCntRot = 0;   // 角度のカウント

	while (1)
	{
		// 文字を読み取る
		fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0)
		{// コメントが来たら
			// コメントを読み飛ばす
			continue;
		}

		// FRAMEを読み通ったら
		if (strcmp(aString, "FRAME") == 0)
		{
			// [=]読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// フレームを読み込む
			fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].nFrame);
		}

		// POSを読み通ったら
		if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// 位置を読み込む
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosX);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosY);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROTを読み通ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// 角度を読み込む
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotX);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotY);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotZ);
			nCntRot++;
		}
		// END_KEYSETを読み通ったら
		else if (strcmp(aString, "END_KEYSET") == 0)
		{
			nKey++;		 // キーを増やす
			nCntPos = 0; // 位置のカウントをリセット
			nCntRot = 0; // 角度のカウントをリセット
			break;
		}
	}
}
