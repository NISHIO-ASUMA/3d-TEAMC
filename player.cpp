//============================
//
// プレイヤー[player.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include "player.h"
#include "input.h"
#include "camera.h"
#include <stdio.h>
#include <string.h>
#include "motion.h"
#include "meshfield.h"
#include "block.h"
#include "item.h"
#include "enemy.h"
#include "wall.h"
#include "mouse.h"
#include "Shadow.h"
#include "Effect.h"

//****************************
//マクロ定義
//****************************
#define MAX_WORD (256) // 最大文字数
#define PLAYERLIFE (50) // プレイヤーの体力
#define MAX_TEXPLAYER (128) // テクスチャの最大数
#define MAX_JUMP (15.0f) // ジャンプ量
#define MAX_MOVE (1.0f) // っプレイヤーの移動量
#define NUM_MTX (4) // 剣の当たり判定のマトリクスの数

//****************************
//プロトタイプ宣言
//****************************
void LoadModel(int nType); // プレイヤーのロード処理
void PlayerComb(MOTIONTYPE motiontype, int AttackState,int nCounterState, COMBOSTATE Combstate); // プレイヤーのコンボ処理
void LoadMotion(int Weponmotion); // モーションのロード処理
void MotionChange(int itemtype,int LoadPlayer); // モーション変更

//****************************
//グローバル変数宣言
//****************************
//LPDIRECT3DTEXTURE9 g_apTexturePlayer[MAX_TEXPLAYER] = {};//プレイヤーのテクスチャへのポインタ
Player g_player;//プレイヤー構造体
Player g_LoadPlayer[PLAYERTYPE_MAX]; // プレイヤーのモデルを保存しておく変数
MODEL g_LoadWepon[ITEMTYPE_MAX];     // プレイヤーの武器を保存しておく変数
MOTION g_LoadMotion[MOTION_MAX];   // モーションの情報を保存しておく変数
int g_nCounterState,g_AttackState; // 状態カウンター
bool bNohand; // 投げたか投げてないか

//============================
//プレイヤーの初期化処理
//============================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	MODE mode = GetMode();//現在のモードを取得

	//プレイヤーの初期化
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);		   //座標
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   //角度
	g_player.rotDestPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目的の角度
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   //移動量
	g_player.bJump = false;								   //ジャンプ中か否か
	g_player.bDisp = true;								   //使用状態
	g_player.bMove = false;								   //
	g_player.nLife = PLAYERLIFE;						   //体力
	g_player.state = PLAYERSTATE_NORMAL;				   //状態
	g_player.Motion.bLoopMotion = true;					   //
	g_player.Swordrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	   //剣の角度
	g_player.Motion.nKey = 0;							   //キー数
	g_player.Motion.motionType = MOTIONTYPE_NEUTRAL;	   //モーションの種類
	g_player.SwordOffpos.x = 0.0f;						   //剣のオフセットの座標x
	g_player.SwordOffpos.y = 85.0f;						   //剣のオフセットの座標y
	g_player.SwordOffpos.z = 0.0f;						   //剣のオフセットの座標z
	g_player.nCounterAction = 0;						   //アクションカウント
	g_nCounterState = 0;                                   //状態カウンター
	g_AttackState = 0;									   //攻撃状態のカウンター
	bNohand = false;									   //物を投げたか投げてないか

	//LoadWepon(); // アイテムのロード


	LoadMotion(0);
	LoadMotion(1);
	LoadMotion(2);

	// 切り替わるモーションの数だけ
	for (int nCnt = 0; nCnt < MOTION_MAX; nCnt++)
	{
		if (nCnt != MOTION_NO_HAND)
		{

		}
	}

	g_LoadPlayer[0].nIdxShadow = SetShadow(g_player.pos, g_player.rot, 40.0f);

	D3DXMATERIAL* pMat;//マテリアルへのポインタ

	for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
	{
		//必要な情報を設定
		LoadModel(nCntPlayer);

		g_LoadPlayer[nCntPlayer].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_LoadPlayer[nCntPlayer].nLife = PLAYERLIFE;
		g_LoadPlayer[nCntPlayer].nMaxLife = PLAYERLIFE;
		g_LoadPlayer[nCntPlayer].bDisp = true;
		g_LoadPlayer[nCntPlayer].Motion.motionType = MOTIONTYPE_NEUTRAL;
		g_LoadPlayer[nCntPlayer].Motion.bLoopMotion = true;
		g_LoadPlayer[nCntPlayer].bJumpAttack = false;
		g_LoadPlayer[nCntPlayer].HandState = PLAYERHOLD_NO;
		g_LoadPlayer[nCntPlayer].state = PLAYERSTATE_NORMAL;

		for (int nCntModel = 0; nCntModel < g_LoadPlayer[nCntPlayer].Motion.nNumModel; nCntModel++)
		{
			//マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//このファイル名を使用してテクスチャを読み込む
					//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,						
					pMat[nCntMat].pTextureFilename,
					&g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pTexture[nCntMat]);
				}
			}
		}
	}

	//頂点座標の取得
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ

	for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
	{
		for (int nCntModel = 0; nCntModel < g_LoadPlayer[nCntPlayer].Motion.nNumModel; nCntModel++)
		{
			//頂点数の取得
			nNumVtx = g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh->GetNumVertices();

			//頂点フォーマットのサイズ取得
			sizeFVF = D3DXGetFVFVertexSize(g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh->GetFVF());

			//頂点バッファのロック
			g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//頂点座標を比較してブロックの最小値,最大値を取得
				if (vtx.x < g_player.vtxMinPlayer.x)
				{
					g_player.vtxMinPlayer.x = vtx.x;
				}
				else if (vtx.y < g_player.vtxMinPlayer.y)
				{
					g_player.vtxMinPlayer.y = vtx.y;
				}
				else if (vtx.z < g_player.vtxMinPlayer.z)
				{
					g_player.vtxMinPlayer.z = vtx.z;
				}

				else if (vtx.x > g_player.vtxMaxPlayer.x)
				{
					g_player.vtxMaxPlayer.x = vtx.x;
				}

				else if (vtx.y > g_player.vtxMaxPlayer.y)
				{
					g_player.vtxMaxPlayer.y = vtx.y;
				}

				else if (vtx.z > g_player.vtxMaxPlayer.z)
				{
					g_player.vtxMaxPlayer.z = vtx.z;
				}

				if (vtx.x < g_player.Motion.aModel[nCntModel].vtxMin.x)
				{
					g_player.Motion.aModel[nCntModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_player.Motion.aModel[nCntModel].vtxMin.y)
				{
					g_player.Motion.aModel[nCntModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_player.Motion.aModel[nCntModel].vtxMin.z)
				{
					g_player.Motion.aModel[nCntModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_player.Motion.aModel[nCntModel].vtxMax.x)
				{
					g_player.Motion.aModel[nCntModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_player.Motion.aModel[nCntModel].vtxMax.y)
				{
					g_player.Motion.aModel[nCntModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_player.Motion.aModel[nCntModel].vtxMax.z)
				{
					g_player.Motion.aModel[nCntModel].vtxMax.z = vtx.z;
				}


				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			//サイズを代入
			g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

			//各モデルごとのサイズを代入
			g_player.Motion.aModel[nCntModel].Size.x = g_player.Motion.aModel[nCntModel].vtxMax.x - g_player.Motion.aModel[nCntModel].vtxMin.x;
			g_player.Motion.aModel[nCntModel].Size.y = g_player.Motion.aModel[nCntModel].vtxMax.y - g_player.Motion.aModel[nCntModel].vtxMin.y;
			g_player.Motion.aModel[nCntModel].Size.z = g_player.Motion.aModel[nCntModel].vtxMax.z - g_player.Motion.aModel[nCntModel].vtxMin.z;

			//頂点バッファのアンロック
			g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh->UnlockVertexBuffer();
		}
	}

	//最初に描画したいプレイヤーの情報を代入
	g_player = g_LoadPlayer[0];
}
//============================
//プレイヤーの終了処理
//============================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
	{
		for (int nCntModel = 0; nCntModel < g_LoadPlayer[nCntPlayer].Motion.nNumModel; nCntModel++)
		{
			//テクスチャの破棄
			for (int nCntMat = 0; nCntMat < MAX_TEXTURE; nCntMat++)
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

	// アイテム分回す
	for (int nCnt = 0; nCnt < ITEMTYPE_MAX; nCnt++)
	{
		// メッシュの破棄
		if (g_LoadWepon[nCnt].pMesh != NULL)
		{
			g_LoadWepon[nCnt].pMesh->Release();
			g_LoadWepon[nCnt].pMesh = NULL;
		}
		// マテリアルの破棄
		if (g_LoadWepon[nCnt].pBuffMat != NULL)
		{
			g_LoadWepon[nCnt].pBuffMat->Release();
			g_LoadWepon[nCnt].pBuffMat = NULL;
		}
	}

	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel; nCntModel++)
	{
		for (int nCntMat = 0; nCntMat < MAX_TEXTURE; nCntMat++)
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
//============================
//プレイヤーの更新処理
//============================
void UpdatePlayer(void)
{
	Camera* pCamera = GetCamera();

		/*StickPad();*/

	if (GetKeyboardPress(DIK_A) == true)
	{
		//プレイヤーの移動(上)
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;
	
			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.25f) * MAX_MOVE;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.25f) * MAX_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.75f;
		}
		//プレイヤーの移動(下)
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.75f) * MAX_MOVE;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.75f) * MAX_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.25f;
		}
		//プレイヤーの移動(左)
		else
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.z += sinf(pCamera->rot.y) * MAX_MOVE;
			g_player.move.x -= cosf(pCamera->rot.y) * MAX_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.5f;
		}
	}
	//プレイヤーの移動(右)
	else if (GetKeyboardPress(DIK_D) == true)
	{
		//プレイヤーの移動(上)
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * MAX_MOVE;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * MAX_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.75f;
		}
		//プレイヤーの移動(下)
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * MAX_MOVE;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * MAX_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.25f;
		}
		//プレイヤーの移動(右)
		else
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.z -= sinf(pCamera->rot.y) * MAX_MOVE;
			g_player.move.x += cosf(pCamera->rot.y) * MAX_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.5f;
		}

	}
	//プレイヤーの移動(上)
	else if (GetKeyboardPress(DIK_W) == true)
	{
		g_player.Motion.motionType = MOTIONTYPE_MOVE;

		g_player.move.x += sinf(pCamera->rot.y) * MAX_MOVE;
		g_player.move.z += cosf(pCamera->rot.y) * MAX_MOVE;

		g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI;
	}
	//プレイヤーの移動(下)
	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_player.Motion.motionType = MOTIONTYPE_MOVE;

		g_player.move.x -= sinf(pCamera->rot.y) * MAX_MOVE;
		g_player.move.z -= cosf(pCamera->rot.y) * MAX_MOVE;

		g_player.rotDestPlayer.y = pCamera->rot.y;
	}
	else
	{
		if (g_player.Motion.motionType == MOTIONTYPE_MOVE)
		{
			g_player.Motion.motionType = MOTIONTYPE_NEUTRAL; // キーを押していない時にニュートラルになる
		}
	}
	
	switch (g_player.Motion.motionType)
	{
	case MOTIONTYPE_NEUTRAL:
		break;
	case MOTIONTYPE_MOVE:
		if (!g_player.bJump)
		{
			g_player.Motion.motionType = MOTIONTYPE_JUMP; // モーションをジャンプにする
		}
		break;
	case MOTIONTYPE_ACTION:
		break;
	case MOTIONTYPE_JUMP:
		break;
	case MOTIONTYPE_LANDING:
		break;
	default:
		break;
	}

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
	g_player.move.x += (0.0f - g_player.move.x) * 0.25f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.25f;

	//前回の位置を保存
	g_player.posOld = g_player.pos;

	//プレイヤーの位置の更新
	g_player.pos += g_player.move;

	if (CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.Size))
	{
		g_player.bJump = true; // ジャンプを可能にする
	}
	else if (CollisionField())
	{
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
	SetPositionShadow(g_player.nIdxShadow,g_player.pos,60.0f + 60.0f * g_player.pos.y / 200.0f, 1.0f / (1.0f + g_player.pos.y / 30.0f));

	////壁の衝突判定
	//CollisionWall();

	if (JoypadTrigger(JOYKEY_A) == true || KeyboardTrigger(DIK_SPACE)==true)
	{//aボタン or Enterキーが押された
		if (g_player.bJump == true)
		{
			g_player.bJump = false;
			g_player.Motion.nKey = 0;
			g_player.Motion.motionType = MOTIONTYPE_JUMP;
			g_player.move.y = 15.0f;
		}
	}

	// プレイヤーの状態が攻撃じゃないかつ地面にいる
	if (g_player.bDisp && !bNohand)
	{
		if (OnMouseTriggerDown(LEFT_MOUSE)&&g_player.Combostate == COMBO_NO)
		{
			PlayerComb(MOTIONTYPE_ACTION, 60, 30, COMBO_ATTACK1); // コンボ1
		}
		else if (OnMouseTriggerDown(LEFT_MOUSE) && g_player.Combostate == COMBO_ATTACK1)
		{
			PlayerComb(MOTIONTYPE_ACTION2, 60, 30, COMBO_ATTACK2); // コンボ2
		}
		else if (OnMouseTriggerDown(LEFT_MOUSE) && g_player.Combostate == COMBO_ATTACK2)
		{
			PlayerComb(MOTIONTYPE_ACTION3, 60, 30, COMBO_ATTACK3); // コンボ3
		}
		else if (OnMouseTriggerDown(LEFT_MOUSE) && g_player.Combostate == COMBO_ATTACK3)
		{
			PlayerComb(MOTIONTYPE_ACTION4, 60, 30, COMBO_ATTACK4); // コンボ4
		}
	}

	// 投げ物を持っているときの攻撃
	if (OnMouseTriggerDown(LEFT_MOUSE) && g_player.Combostate == COMBO_NO && bNohand)
	{
		PlayerComb(MOTIONTYPE_ACTION, 60, 20, COMBO_ATTACK1); // コンボ1
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

	//SetEffect(SwordPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 10.0f);

	//モーションの更新
	UpdateMotion(&g_player.Motion);

	//プレイヤーの向きを目的の向きに近づける
	g_player.rot.y += (g_player.rotDestPlayer.y - g_player.rot.y) * 0.1f;
}
//============================
//プレイヤーの描画処理
//============================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans,mtxSize;

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
					color.MatD3D.Diffuse.g = 0.0f;
					color.MatD3D.Diffuse.b = 0.0f;
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
//============================
//プレイヤーの取得処理
//============================
Player* GetPlayer(void)
{
	return &g_player;
}
//=============================================
////ワールドマトリックスのオフセット設定処理
//=============================================
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
//======================
// プレイヤーと敵の判定
//======================
void HitPlayer(int nDamage)
{
	g_player.nLife -= nDamage;

	if (g_player.nLife <= 0)
	{
		g_player.state = PLAYERSTATE_FALL;
	}
	else
	{
		g_player.nCounterState = 60;
		g_player.state = PLAYERSTATE_DAMAGE;
	}
}
//============================
// プレイヤーのスティック操作
//============================
void StickPad(void)
{
	//XINPUT_STATE* pStick;

	//pStick = GetJoySticAngle();

	//CAMERA* pCamera = GetCamera();
	//if (g_player.state != PLAYERSTATE_FALL && g_player.state != PLAYERSTATE_DAMAGE&& g_player.bJumpAttack ==false && g_player.state != PLAYERSTATE_ATTACK)
	//{
	//	if (GetJoyStick() == true)
	//	{
	//		float LStickAngleY = pStick->Gamepad.sThumbLY;
	//		float LStickAngleX = pStick->Gamepad.sThumbLX;

	//		float deadzone = 10920;
	//		float magnitude = sqrtf(LStickAngleX * LStickAngleX + LStickAngleY * LStickAngleY);

	//		if (magnitude > deadzone)
	//		{
	//			bPad = true;
	//			float normalizeX = (LStickAngleX / magnitude);
	//			float normalizeY = (LStickAngleY / magnitude);

	//			float moveX = normalizeX * cosf(-pCamera->rot.y) - normalizeY * sinf(-pCamera->rot.y);
	//			float moveZ = normalizeX * sinf(-pCamera->rot.y) + normalizeY * cosf(-pCamera->rot.y);

	//			g_player.move.x += moveX * PLAYER_MOVE;
	//			g_player.move.z += moveZ * PLAYER_MOVE;

	//			g_player.rotDestPlayer.y = atan2f(-moveX, -moveZ);

	//			if (g_player.Motion.motionType != MOTIONTYPE_INVISIBLE)
	//			{
	//				g_player.state = PLAYERSTATE_MOVE;
//			}
//		}
//	}
//}
}
//================================
// プレイヤーの剣と敵の当たり判定
//================================
void HitSowrd(ENEMY* pEnemy,int nCntEnemy)
{
	D3DXVECTOR3 mtxDis,SwordPos;

	//剣の長さを求める
	mtxDis.x = (g_player.SwordMtx._41 - g_player.Motion.aModel[15].mtxWorld._41);
	mtxDis.y = (g_player.SwordMtx._42 - g_player.Motion.aModel[15].mtxWorld._42);
	mtxDis.z = (g_player.SwordMtx._43 - g_player.Motion.aModel[15].mtxWorld._43);

	// マトリクスの数分だけ回す
	for (int nCnt = 0; nCnt < NUM_MTX; nCnt++)
	{
		// 剣の位置を全て求める
		SwordPos.x = g_player.Motion.aModel[15].mtxWorld._41 + mtxDis.x * 0.25f * nCnt;
		SwordPos.y = g_player.Motion.aModel[15].mtxWorld._42 + mtxDis.y * 0.25f * nCnt;
		SwordPos.z = g_player.Motion.aModel[15].mtxWorld._43 + mtxDis.z * 0.25f * nCnt;

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

		if (fDistance <= fRadius && pEnemy->state != ENEMYSTATE_DAMAGE && g_player.state==PLAYERSTATE_ATTACK)
		{
			HitEnemy(nCntEnemy, 1);
			break;
		}
	}
}
//================================
// 物を投げる
//================================
void ThrowItem(void)
{
	Item* pItem = GetItem();
	int nIdx = g_player.ItemIdx; // 手に持っているアイテムのインデックス情報を代入

	// 発射地点を設定
	pItem[nIdx].pos.x = g_player.pos.x;
	pItem[nIdx].pos.y = g_player.Motion.aModel[2].pos.y;
	pItem[nIdx].pos.z = g_player.pos.z;

	// 飛ばす方向を設定
	pItem[nIdx].move.x = sinf(g_player.rot.y + D3DX_PI) * 10.0f;
	pItem[nIdx].move.z = cosf(g_player.rot.y + D3DX_PI) * 10.0f;
	pItem[nIdx].bUse = true; // 使用状態をtrueにする

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
}

//================================
// プレイヤーとアイテムの判定
//================================
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

		if (KeyboardTrigger(DIK_F))
		{
			Itemchange(pItem[nIdx].nType); // アイテムを拾う
			pItem[nIdx].bUse = false;      // 消す
			g_player.ItemIdx = nIdx;

			switch (pItem[nIdx].nType)
			{
			case ITEMTYPE_BAT:
				MotionChange(MOTION_DBHAND,0); // アイテムにあったモーションタイプを入れる(素手の場合は引数2に1を入れる)
				break;
			case ITEMTYPE_GOLF:
				MotionChange(MOTION_DBHAND,0);
				break;
			case ITEMTYPE_HUNMER:
				MotionChange(MOTION_BIGWEPON,0);
				break;
			case ITEMTYPE_STONE:
				MotionChange(MOTION_BIGWEPON, 1);
				break;
			case ITEMTYPE_WOOD:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_STONEBAT:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_LIGHT:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_LIGHTWOOD:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_HARISEN:
				break;
			case ITEMTYPE_ICEBLOCK:
				MotionChange(MOTION_DBHAND, 1);
				break;
			case ITEMTYPE_ICEBLOCKSOWRD:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_IRON:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_IRONBAT:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_SURFBOARD:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_TORCH:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_TORCHSWORD:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_HEADSTATUE:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_HEADSTATUTORSO:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_MEGAPHONE:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_RUBBERCUP:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_TELEPHONEPOLE:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_TORSO:
				MotionChange(MOTION_DBHAND, 0);
				break;
			default:
				break;
			}
		}
	}

	return bCollision;
}

//============================
// プレイヤーのコンボ
//============================
void PlayerComb(MOTIONTYPE motiontype, int AttackState, int nCounterState, COMBOSTATE Combstate)
{
	g_player.Motion.nKey = 0;                 // キーを0から始める
	g_player.Motion.nCountMotion = 0;	      // モーションカウントを0から始める
	g_player.Motion.motionType = motiontype;  // モーションの種類を変更
	g_nCounterState = nCounterState;		  // 状態カウンターを設定
	g_AttackState = AttackState;			  // 攻撃状態カウンターを設定
	g_player.state = PLAYERSTATE_ATTACK;	  // プレイヤーの状態を攻撃にする	
	g_player.Combostate = Combstate;		  // コンボの状態を設定
}
//===============================
// プレイヤーのモーションの変更
//===============================
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

	if (LoadPlayer != PLAYERTYPE_NOHAND) // プレイヤーがノーハンドだったら情報を代入しない
	{
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION] = g_LoadMotion[itemtype].aMotionInfo[0];		// 攻撃1の情報を代入
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION2] = g_LoadMotion[itemtype].aMotionInfo[1];    // 攻撃2の情報を代入
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION3] = g_LoadMotion[itemtype].aMotionInfo[2];	// 攻撃3の情報を代入
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION4] = g_LoadMotion[itemtype].aMotionInfo[3];	// 攻撃4の情報を代入
		g_player.Motion.aModel[15].offpos = g_LoadMotion[itemtype].aModel[15].offpos;				// オフセットの情報を代入
		g_player.Motion.aModel[15].offrot = g_LoadMotion[itemtype].aModel[15].offrot;				// オフセットのの情報を代入
		g_player.HandState = PLAYERHOLD_NO;
		bNohand = false;// プレイヤーをノーハンドにする
	}
	else
	{
		g_player.HandState = PLAYERHOLD_HOLD;	// プレイヤーをノーハンドにする
		bNohand = true;
	}
}

//================================
// プレイヤーのモーションのロード
//================================
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

//============================
// プレイヤーのロード処理
//============================
void LoadModel(int nType)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ローカル変数
	int nModel = 0;					// モデル数
	int nIdx = 0;					// インデックス
	int NumKey = 0;					// キー数
	int nCnt = 0;					// モデルカウント用
	int nNumParts = 0;				// パーツ数格納用
	int nCntMotion = 0;				// モーションカウント用
	int nCntKey = 0;				// キーカウント用
	int nCntPosKey = 0;				// posカウント
	int nCntRotkey = 0;				// rotカウント

	// ファイルポインタを宣言
	FILE* pFile;

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
	{//　NULL じゃない
		char aString[MAX_WORD];

		while (1)
		{
			// 読み飛ばし
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SCRIPT") == 0)
			{// SCRIPTを読み取ったら
				while (1)
				{
					// 読み飛ばし
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_MODEL") == 0)
					{// NUM_MODELを読み取ったら
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "=") == 0)
						{// 値を代入
							fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.nNumModel);
						}
					}

					else if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
					{
						// MODEL_FILENAMEを読み取ったら
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "=") == 0)
						{// 代入
							// 文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							const char* MODEL_FILE = {};	// モデルファイル名格納用の変数

							// 読み込んだ文字列を保存する
							MODEL_FILE = aString;

							//Xファイルの読み込み
							D3DXLoadMeshFromX(MODEL_FILE,
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&g_LoadPlayer[nType].Motion.aModel[nCnt].pBuffMat,
								NULL,
								&g_LoadPlayer[nType].Motion.aModel[nCnt].dwNumMat,
								&g_LoadPlayer[nType].Motion.aModel[nCnt].pMesh);

							nCnt++; // nCntをインクリメント
						}
					}
					else if (strcmp(&aString[0], "CHARACTERSET") == 0)
					{
						while (1)
						{
							// 文字列を読み飛ばす
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "NUM_PARTS") == 0)
							{// NUM_PARTSを読み取ったら

								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// 値を代入
									fscanf(pFile, "%d", &nNumParts);
								}
							}

							else if (strcmp(&aString[0], "PARTSSET") == 0)
							{
								while (1)
								{
									// 文字列を読み飛ばす
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "INDEX") == 0)
									{// INDEXを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 代入
											fscanf(pFile, "%d", &nIdx);
										}
									}

									if (strcmp(&aString[0], "PARENT") == 0)
									{// PARENTを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 代入
											// ペアネント
											fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aModel[nIdx].nIdxModelParent);
										}
									}


									if (strcmp(&aString[0], "POS") == 0)
									{// INDEXを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 座標を代入
											fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.x);
											fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.y);
											fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.z);
										}
									}

									if (strcmp(&aString[0], "ROT") == 0)
									{// INDEXを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 角度を代入
											fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].rot.x);
											fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].rot.y);
											fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].rot.z);
										}
									}

									if (strcmp(&aString[0], "END_PARTSSET") == 0)
									{// END_PARTSSETを読み取ったら
										// whileを抜ける
										break;
									}

								}// while文末
							}
							else if (strcmp(&aString[0], "END_CHARACTERSET") == 0)
							{
								break;
							}
						}
					}
					else if (strcmp(&aString[0], "MOTIONSET") == 0)
					{// MOTIONSETを読み取ったら


						while (1)
						{
							// 文字を読み飛ばす
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(aString, "LOOP") == 0)
							{// LOOP を読み取ったら
								// 文字を読み飛ばす
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = を読み取ったら
									// 値を代入
									fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].bLoop);
								}
							}

							else if (strcmp(aString, "NUM_KEY") == 0)
							{// NUM_KEYを読み取ったら
								// 文字を読み飛ばす
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = を読み取ったら
									// 値を代入
									fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].nNumkey);
								}

								while (nCntKey < g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].nNumkey)
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
												fscanf(pFile, "%s", &aString[0]);

												if (strcmp(&aString[0], "=") == 0)
												{// 値を代入
													fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
													break;
												}
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
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// 値を代入
															fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosX);
															fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosY);
															fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosZ);
															nCntPosKey++;		// インクリメント
														}
													}

													else if (strcmp(&aString[0], "ROT") == 0)
													{// ROTを読み取ったら
														// 文字を読み飛ばす
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// 値を代入
															fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotX);
															fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotY);
															fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotZ);
															nCntRotkey++;		// インクリメント
														}
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

					else if (strcmp(&aString[0], "END_SCRIPT") == 0)
					{
						break;
					}
					else
					{
						continue;
					}
				}// while文末

				break;
			}
		}// while文末
	}
	else
	{
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(Player.cpp)", MB_OK);
		return;
    }
	// ファイルを閉じる
	fclose(pFile);
}