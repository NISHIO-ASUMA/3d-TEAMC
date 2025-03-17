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
#include "effectEdit.h"
#include "icon.h"
#include "effect2.h"
#include "meshimpact.h"
#include "meshcylinder.h"
#include "billboard.h"
#include "mark.h"
#include "game.h"
#include "math.h"
#include "easing.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define MAX_WORD (256)			// 最大文字数
#define PLAYERLIFE (1000)		// プレイヤーの体力
#define MAX_TEXPLAYER (128)		// テクスチャの最大数
#define MAX_JUMP (12.0f)		// ジャンプ量
#define MAX_MOVE (1.0f)			// プレイヤーの移動量
#define NUM_MTX (8)				// 剣の当たり判定のマトリクスの数
#define LANDINGEXPLOSION (6)	// 着地したときに出る煙
#define HEAL_VALUE (100)		// 回復量
#define AVOID_MOVE (18.0f)      // 回避の移動量
#define DAMAGEBLOW (20.0f)      // ダメージを受けた時の吹き飛び量
#define BLOWCOUNT (5)           // 吹っ飛びカウント
#define PLAYER_DAMAGE (100)     // プレイヤーの攻撃力(基準)
#define PLAYER_SPEED (3.5f)     // プレイヤーの移動速度

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void PlayerComb(MOTIONTYPE motiontype, int AttackState,int nCounterState, COMBOSTATE Combstate); // プレイヤーのコンボ処理
void LoadMotion(int Weponmotion);																 // モーションのロード処理
void StickPad(void);																			 // パッドの移動処理

void LoadPlayer(int nType);                                                                      // プレイヤーのロード処理
int LoadFilename(FILE* pFile, int nNumModel, char* aString, int nType);							 // プレイヤーのモデルのロード処理
void LoadCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType);					 // プレイヤーのパーツの設定処理
void LoadWeponOffSet(FILE* pFile, char* aString,int nWepontype);								 // プレイヤーの武器のオフセットの設定処理
void LoadMotionSet(FILE* pFile, char* aString, int nNumModel, int nType);						 // プレイヤーのモーションのロード処理
void LoadKeySet(FILE* pFile, char* aString, int nType, int nCntMotion);							 // プレイヤーのモーションのキーの読み込み処理
void LoadWeponMotionSet(FILE* pFile, char* aString, int nNumModel, int nWepontype);				 // プレイヤーの武器のオフセットのロード処理
void LoadWeponKeySet(FILE* pFile, char* aString, int nWepontype, int nCntMotion);				 // プレイヤーの武器のオフセットのキーの読み込み処理

void SetElementEffect(void);																	 // 属性ごとのエフェクト
void SetMotionContller(void);																	 // モーションの設定処理
void PlayerMove(void);																			 // プレイヤーの移動処理
void UpdatePlayerCraft(void);                                                                    // プレイヤーのクラフト
void DestroyWepon(void);																		 // アイテムの破壊処理
void DropItem(void);                                                                             // アイテムのドロップ処理
void HandleSpecialAttack(void);																	 // スペシャルアタックの処理
void UpdateItemStock(void);																		 // プレイヤーのアイテムのストック処理
void UpdatePlayerAvoid(void);																	 // プレイヤーの回避処理
D3DXVECTOR3 SetMotionMoveAngle(void);                                                            // モーションのアングルの設定
void SetWeponEffect(void);                                                                       // 武器ごとのエフェクト処理
bool IsDamageAction(void);                                                                       // ダメージアクションかどうか
void ChangeItemParam(int nHaveIdx, int nType);													 // アイテムの変更時のパラメーター設定
void MotionTransition(void);																	 // モーションタイプの遷移
void StateTransition(void);																		 // 状態の遷移
void ComboTransition(void);																		 // コンボ状態の遷移
void SetUpPlayerAttack(void);																	 // プレイヤーの攻撃の設定処理
void SetUpJumpAction(int nKey, int nCounter, int nLastKey, int EndFrame);						 // ジャンプアクションの設定
void SetUpPlayerFirstWepon(void);																 // プレイヤーの初期武器の設定
void SetFeverTime(void);																		 // プレイヤーのフィーバータイムの設定
void LoadMinimapMatItem(int nHaveIdx,int nStockIdx);											 // ミニマップの合成のアイテムの材料の表示

//**************************************************************************************************************
//グローバル変数宣言
//**************************************************************************************************************
//LPDIRECT3DTEXTURE9 g_apTexturePlayer[MAX_TEXPLAYER] = {};//プレイヤーのテクスチャへのポインタ
Player g_player;//プレイヤー構造体
MOTION g_LoadPlayer[PLAYERTYPE_MAX]; // プレイヤーのモデルを保存しておく変数
MOTION g_LoadMotion[MOTION_MAX];   // モーションの情報を保存しておく変数
int g_nCounterState,g_AttackState; // 状態カウンター
bool bNohand; // 投げたか投げてないか
bool bUsePad;
int nCntMotion,nKey;
int g_EaseCount;

//===============================================================================================================
//プレイヤーの初期化処理
//===============================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	MODE mode = GetMode();//現在のモードを取得

	//プレイヤーの初期化
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);					// 座標
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 角度
	g_player.rotDestPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目的の角度
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 移動量
	g_player.bJump = false;											// ジャンプ中か否か
	g_player.bDisp = true;											// 使用状態
	g_player.nLife = PLAYERLIFE;									// 体力
	g_player.nMaxLife = PLAYERLIFE;									// 最大体力
	g_player.state = PLAYERSTATE_NORMAL;							// 状態
	g_player.Motion.bLoopMotion = true;								// モーションのループ
	g_player.Motion.nKey = 0;										// キー数
	g_player.Motion.motionType = MOTIONTYPE_NEUTRAL;				// モーションの種類
	g_player.SwordOffpos.x = 0.0f;									// 剣のオフセットの座標x
	g_player.SwordOffpos.y = 85.0f;									// 剣のオフセットの座標y
	g_player.SwordOffpos.z = 0.0f;									// 剣のオフセットの座標z
	g_player.nCounterAction = 0;									// アクションカウント
	g_nCounterState = 0;											// 状態カウンター
	g_AttackState = 0;												// 攻撃状態のカウンター
	bNohand = false;												// 物を投げたか投げてないか
	g_player.speed = PLAYER_SPEED;									// 足の速さ
	g_player.nDamage = PLAYER_DAMAGE;								// 攻撃力
	bUsePad = false;												// パッドを使っているか
	g_player.nStockDamage = PLAYER_DAMAGE;							// ダメージのストック
	g_player.fStockSpeed = PLAYER_SPEED;							// スピードのストック
	g_player.FeverMode = false;										// フィーバーモードかどうか
	g_player.SpMode = false;										// スペシャルを発動できるかどうか
	g_player.WeponMotion = MOTION_KATANA;							// 武器ごとのモーション
	g_player.AttackSp = false;										// スペシャル攻撃中かどうか
	g_player.bLandingOBB = false;									// OBBに乗ってるか
	nCntMotion = 0;													// モーションのカウント
	nKey = 0;														// キーのカウント
	g_player.bCraft = false;										// クラフト中かどうか
	g_player.nElement = WEPONELEMENT_STANDARD;						// 属性の種類
	g_EaseCount = 0;												// イージングのカウント
	g_player.nIdxShadow = SetShadow(g_player.pos, g_player.rot, 20.0f, 1.0f); // 影を設定
	g_player.nIdxMap = SetMiniMap(g_player.pos, MINIMAPTEX_PLAYER); // ミニマップにプレイヤーを設定
	g_player.HandState = PLAYERHOLD_NO;								// 手の状態
	g_player.Combostate = COMBO_NO;                                 // コンボの状態
	g_player.AttackState = PLAYERATTACKSTATE_NO;                    // 攻撃の状態
	g_player.nCounterAttack = 0;                                    // 攻撃状態のカウンター
	g_player.avoidMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);             // 回避の移動量
	g_player.ItemIdx = 0;											// アイテムのインデックスの初期化
	g_player.BlowCounter = 0;										// 吹っ飛ぶまでのカウンター
	g_player.AttackerIdx = 0;										// 攻撃してきた敵のインデックス
	g_player.bstiffness = false;									// ダメージの硬直
	g_player.StockItemType = ITEMTYPE_NONEXISTENT;                  // ストックしているアイテムの種類
	g_player.HoldItemType = ITEMTYPE_KATANA;						// 持っているアイテムの種類
	g_player.bAvoid = false;										// 無敵時間
	g_player.HitStopCount = 0;										// ヒットストップ

	// アイテム分回す
	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		g_player.Itembreak[nCnt] = false; // アイテムが壊れたか
	}

	// TODO : ここの処理考える

	// タイトルでロードをすると重くなるので
	if (mode != MODE_TITLE)
	{
		// 切り替わるモーションの数だけ
		for (int nCnt = 0; nCnt < MOTION_MAX; nCnt++)
		{
			LoadMotion(nCnt);
		}

		// プレイヤーの種類分
		for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
		{
			//必要な情報を設定
			LoadPlayer(nCntPlayer);
			g_LoadPlayer[nCntPlayer].motionType = MOTIONTYPE_NEUTRAL;
			g_LoadPlayer[nCntPlayer].bLoopMotion = true;
		
		}

		//最初に描画したいプレイヤーの情報を代入
		g_player.Motion = g_LoadPlayer[0];
	}

	// 矢印を設定
	SetMark(g_player.pos, g_player.rot);

	// ダメージのUIを設定
	SetGameUI(D3DXVECTOR3(640.0f, 360.0f, 0.0f), UITYPE_DAMAGE, 740.0f, 460.0f, false, 0);

	// プレイヤーの初期武器の設定
	SetUpPlayerFirstWepon();
	
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
		for (int nCntModel = 0; nCntModel < g_LoadPlayer[nCntPlayer].nNumModel; nCntModel++)
		{
			//テクスチャの破棄
			for (int nCntMat = 0; nCntMat < (int)g_LoadPlayer[nCntPlayer].aModel[nCntModel].dwNumMat; nCntMat++)
			{
				if (g_LoadPlayer[nCntPlayer].aModel[nCntModel].pTexture[nCntMat] != NULL)
				{
					g_LoadPlayer[nCntPlayer].aModel[nCntModel].pTexture[nCntMat]->Release();
					g_LoadPlayer[nCntPlayer].aModel[nCntModel].pTexture[nCntMat] = NULL;
				}
			}

			//メッシュの破棄
			if (g_LoadPlayer[nCntPlayer].aModel[nCntModel].pMesh != NULL)
			{
				 g_LoadPlayer[nCntPlayer].aModel[nCntModel].pMesh->Release();
				 g_LoadPlayer[nCntPlayer].aModel[nCntModel].pMesh = NULL;
			}

			//マテリアルの破棄
			if (g_LoadPlayer[nCntPlayer].aModel[nCntModel].pBuffMat != NULL)
			{
				 g_LoadPlayer[nCntPlayer].aModel[nCntModel].pBuffMat->Release();
				 g_LoadPlayer[nCntPlayer].aModel[nCntModel].pBuffMat = NULL;
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
	GAMESTATE gameState = GetGameState();

	// 体力の現在値が最大値を超えてたら最大値にする
	if (g_player.nMaxLife <= g_player.nLife)
	{
		g_player.nLife = g_player.nMaxLife;
	}

	g_player.HitStopCount--;

	if (g_player.HitStopCount > 0)
	{
		g_player.pos.x += (float)(rand() % 3 - 1.5f);
		g_player.pos.z += (float)(rand() % 3 - 1.5f);
	}

	// ヒットストップのカウントが0以上有ったら
	if (g_player.HitStopCount >= 0) return;

	// ムービーじゃなかったら
	if (gameState != GAMESTATE_MOVIE)
	{
		// モーションの更新
		UpdateMotion(&g_player.Motion);

		// プレイヤーのクラフトの設定
		UpdatePlayerCraft();

		// モーションの演出処理
		SetMotionContller();
	}

	// クラフト状態なら
	if (g_player.bCraft == true)
	{
		// 関数を抜ける
		return;
	}

	// フィーバーモードなら
	if (g_player.FeverMode == true)
	{
		SetParticle(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 25, g_player.pos.z), D3DXVECTOR3(D3DX_PI / 2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 2.0f, 1, 20, 10, 20.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	}

	// パッドを使っていないかを判定
	const bool NotUsePad = GetJoyStick() == false;

	// アクションじゃないかを判定
	const bool NotAction = CheckActionMotion(&g_player.Motion) == true;

	// スペシャル攻撃じゃないか判定
	const bool NotSp = g_player.AttackSp == false;

	// 死んでるかどうかを判定
	const bool NotDeth = g_player.nLife > 0;

	// ダメージモーションかどうかを判定
	const bool NotDamage = g_player.bstiffness == false;

	// うごけるかどうかを判定
	const bool is_Move = NotUsePad && NotAction && NotSp && NotDeth && NotDamage && gameState != GAMESTATE_MOVIE;

	// うごける
	if (is_Move == true)
	{
		PlayerMove(); // プレイヤーの移動処理
	}

	const bool is_MovePad = !NotUsePad && NotAction && NotSp && NotDamage && gameState != GAMESTATE_MOVIE;

	if (is_MovePad == true)
	{
		StickPad(); // パッドの移動処理
	}

	// 剣のマトリクスを代入
	D3DXVECTOR3 SwordPos = SetMtxConversion(g_player.SwordMtx);

	switch (g_player.AttackState)
	{
	case PLAYERATTACKSTATE_NO:
		break;
	case PLAYERATTACKSTATE_ATTACK:
		g_player.nCounterAttack--;
		if (g_player.nCounterAttack <= 0)
		{
			g_player.AttackState = PLAYERATTACKSTATE_NO;
		}
		break;
	default:
		break;
	}
	
	// モーションタイプの遷移
	MotionTransition();

	// プレイヤーの状態の遷移
	StateTransition();

	// コンボ状態の遷移
	ComboTransition();

	// ムービーじゃなかったら
	if (gameState != GAMESTATE_MOVIE)
	{
		//移動量を減衰
		g_player.move.x += (0.0f - g_player.move.x) * 0.5f;
		g_player.move.z += (0.0f - g_player.move.z) * 0.5f;

		//前回の位置を保存
		g_player.posOld = g_player.pos;

		//プレイヤーの位置の更新
		g_player.pos += g_player.move;
	}
	else
	{
		//前回の位置を保存
		g_player.posOld.y = g_player.pos.y;

		//プレイヤーの位置の更新
		g_player.pos.y += g_player.move.y;
	}

	// メッシュフィールドの当たり判定
	if (CollisionField(&g_player.pos,&g_player.posOld) == true)
	{
		g_player.bLandingOBB = false;

		// モーションがジャンプだったら
		if (g_player.Motion.motiontypeBlend == MOTIONTYPE_JUMP && g_player.Motion.motiontypeBlend != MOTIONTYPE_LANDING)
		{
			SetMotion(&g_player.Motion,MOTIONTYPE_LANDING, true, 10); // モーションを着地にする
			//SetImpact(g_player.pos, COLOR_GOLD, 32, 10.0f, 0.0f, 3.0f, 60, IMPACTTYPE_NORMAL, 0);
		}
		g_player.bJump = true; // ジャンプを可能にする
	}
	else
	{
		g_player.bJump = false;
	}

	// メッシュウォール
	CollisionWall(&g_player.pos,&g_player.posOld,&g_player.move,g_player.speed);

	//プレイヤーの重力
	g_player.move.y -= MAX_GLABITY;

	// 影の計算
	SetPositionShadow(g_player.nIdxShadow, g_player.pos, 30.0f + 30.0f * g_player.pos.y / 200.0f, 1.0f / (1.0f + g_player.pos.y / 30.0f));

	// ミニマップの位置の設定
	SetMiniMapPotision(g_player.nIdxMap, &g_player.pos);

	if ((JoypadTrigger(JOYKEY_A) || KeyboardTrigger(DIK_SPACE)) && g_player.Motion.motiontypeBlend != MOTIONTYPE_DEATH && g_player.bstiffness == false)
	{//aボタン or Enterキーが押された

		// 音楽再生
		PlaySound(SOUND_LABEL_JUMP_SE);

		if (g_player.bJump == true && g_player.Motion.motionType != MOTIONTYPE_LANDING && g_player.AttackSp == false && gameState != GAMESTATE_MOVIE)
		{
			g_player.bJump = false;						 // ジャンプをできなくする
			SetMotion(&g_player.Motion, MOTIONTYPE_JUMP, true, 10);
			g_player.move.y = 18.0f;					 // ジャンプ量		
			g_player.Combostate = COMBO_NO;
		}
	}

	// プレイヤーの攻撃の設定処理
	SetUpPlayerAttack();

	// プレイヤーの角度の正規化
	if (g_player.rotDestPlayer.y - g_player.rot.y >= D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDestPlayer.y - g_player.rot.y <= -D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	// アイテムの破壊処理
	DestroyWepon();		

	//// アイテムのドロップ処理
	//DropItem();

	// アイテムのストックの更新処理
	UpdateItemStock();		

	// プレイヤーの回避の処理
	UpdatePlayerAvoid();

	// 必殺技の処理
	HandleSpecialAttack();

#ifdef _DEBUG

	if (KeyboardTrigger(DIK_H) == true)
	{
		// 衝撃波を発生指せる
		SetImpact(g_player.pos, D3DCOLOR_RGBA(100, 100, 100, 255), 32, 30.0f, 20.0f, 3.0f, 60, IMPACTTYPE_PLAYER, 0);

		AddSpgauge(100.0f);
	}

#endif // DEBUG

	// フォーバータイムの設定
	SetFeverTime();

	// 死亡モーションだったら
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_DEATH && g_player.Motion.nKey <= 0)
	{
		g_player.move.x = sinf(g_player.rot.y) * 50.0f;
		g_player.move.y = 10.0f;
		g_player.move.z = cosf(g_player.rot.y) * 50.0f;
	}
	
	// シリンダーの位置設定処理
	SetPotisionCylinder(g_player.nIdxCylinder, g_player.pos);

	//D3DXVec3TransformCoord
	// getactivewindow
	
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
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.SwordMtx);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.Motion.aModel[15].rot.y,
		g_player.Motion.aModel[15].rot.x,
		g_player.Motion.aModel[15].rot.z);

	D3DXMatrixMultiply(&g_player.SwordMtx, &g_player.SwordMtx, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.SwordOffpos.x, g_player.SwordOffpos.y, g_player.SwordOffpos.z);
	D3DXMatrixMultiply(&g_player.SwordMtx, &g_player.SwordMtx, &mtxTrans);

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
void HitPlayer(int nDamage,bool SetDamageMotion, int AttackerIdx, int AttackerType)
{
	// プレイヤーの状態がスペシャルだったら
	if (g_player.AttackSp == true) return;

	// 無敵時間だったら
	if (g_player.bAvoid == true) return;

	// プレイヤーの体力を減らす
	g_player.nLife -= nDamage;

	// プレイヤーの体力が0になったら
	if (g_player.nLife <= 0)
	{
		// マイナスでも0にする
		g_player.nLife = 0;

		if (g_player.Motion.motiontypeBlend != MOTIONTYPE_DEATH)
		{
			// モーションの設定
			SetMotion(&g_player.Motion, MOTIONTYPE_DEATH, false, 10);

			D3DXVECTOR3 HeadPos(g_player.Motion.aModel[2].mtxWorld._41, g_player.Motion.aModel[2].mtxWorld._42, g_player.Motion.aModel[2].mtxWorld._43);

			// 魂
			LoadEffect(1, HeadPos);
		}

		// プレイヤーを消す
		EnableMap(g_player.nIdxMap);    // マップから消す
	}
	else
	{
		// 状態カウンター
		g_nCounterState = 30;

		// プレイヤーの状態をダメージにする
		g_player.state = PLAYERSTATE_DAMAGE;

		// カメラを揺らす
		WaveCamera(15);

		// 吹っ飛べるなら
		if (SetDamageMotion == true)
		{
			// 吹き飛びカウント
			g_player.BlowCounter++;

			// カウントが最大になったら
			if (g_player.BlowCounter >= BLOWCOUNT)
			{
				g_player.bstiffness = true;

				// 攻撃してきた敵のインデックスを代入
				g_player.AttackerIdx = AttackerIdx;

				// アタッカーの角度を求める
				g_player.rotDestPlayer.y = SetAttackerAngle(AttackerIdx, AttackerType);

				g_player.BlowCounter = 0;

				// モーションの設定
				SetMotion(&g_player.Motion, MOTIONTYPE_DAMAGE, true, 5);
			}
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

	// ジャンプしてるかを判定
	const bool NotJump = g_player.Motion.motiontypeBlend != MOTIONTYPE_JUMP;

	// モーションがムーブかを判定
	const bool NotMove = g_player.Motion.motiontypeBlend != MOTIONTYPE_MOVE;

	// うごけるかを判定
	const bool is_MotionMove = NotJump == true && NotMove == true;

	// モーションが回避かつキーが3以下
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_AVOID && g_player.Motion.nKey <= KEY_THREE)
	{
		return;
	}

	if (GetJoyStick() == true)
	{
		// Lスティックの角度
		float LStickAngleY = pStick->Gamepad.sThumbLY;
		float LStickAngleX = pStick->Gamepad.sThumbLX;

		// デッドゾーン
		float deadzone = 10920;

		// スティックの傾けた角度を求める
		float magnitude = sqrtf(LStickAngleX * LStickAngleX + LStickAngleY * LStickAngleY);

		// 動かせる
		if (magnitude > deadzone)
		{
			// アングルを正規化
			float normalizeX = (LStickAngleX / magnitude);
			float normalizeY = (LStickAngleY / magnitude);

			// プレイヤーの移動量
			float moveX = normalizeX * cosf(-pCamera->rot.y) - normalizeY * sinf(-pCamera->rot.y);
			float moveZ = normalizeX * sinf(-pCamera->rot.y) + normalizeY * cosf(-pCamera->rot.y);

			// プレイヤーの移動
			g_player.move.x += moveX * g_player.speed;
			g_player.move.z += moveZ * g_player.speed;

			// プレイヤーの目的の角度を決める
			g_player.rotDestPlayer.y = atan2f(-moveX, -moveZ);

			bUsePad = true;
			// プレイヤーを歩きモーションにする
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
			}
		}
		else
		{
			if (g_player.Motion.motionType == MOTIONTYPE_MOVE)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, true, 5); // モーションをニュートラルにする
			}
			bUsePad = false;
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
	Item* pItem = GetItem();

	bool bHit = false;

	// 球の半径
	float Radius = 50.0f;

	// 剣先の位置
	D3DXVECTOR3 SwordTopPos = SetMtxConversion(g_player.SwordMtx);

	// 剣の持ち手の位置
	D3DXVECTOR3 SwordUnderPos = SetMtxConversion(g_player.Motion.aModel[15].mtxWorld);

	for (int nCntModel = 0; nCntModel < pEnemy->Motion.nNumModel; nCntModel++)
	{
		// 敵の位置
		D3DXVECTOR3 EnemyModel = SetMtxConversion(pEnemy->Motion.aModel[nCntModel].mtxWorld);

		// 線分の終点から球体の中心点までのベクトル
		D3DXVECTOR3 pvEnd = SwordTopPos - EnemyModel;

		// 線分の始点から球体の中心点までのベクトル
		D3DXVECTOR3 pvFirst = SwordUnderPos - EnemyModel;

		// 始点～終点までのベクトル
		D3DXVECTOR3 sv = SwordTopPos - SwordUnderPos;

		// 長さX
		float LengthX = SwordTopPos.x - SwordUnderPos.x;
		// 長さY
		float LengthY = SwordTopPos.y - SwordUnderPos.y;
		// 長さZ
		float LengthZ = SwordTopPos.z - SwordUnderPos.z;

		// 線分の長さを求める
		float Length = sqrtf((LengthX * LengthX) + (LengthY * LengthY) + (LengthZ * LengthZ));

		// 正規化する
		D3DXVec3Normalize(&sv, &sv);

		// ベクトルの終点と円の中心との内積
		float DotEnd = D3DXVec3Dot(&sv, &pvEnd);

		// ベクトルの始点と円の中心との内積
		float DotFirst = D3DXVec3Dot(&sv, &pvFirst);

		// 交差した
		if (D3DXVec3Length(&pvEnd) < Radius)
		{
			bHit = true;
			break;
		}
		// 交差した
		else if (D3DXVec3Length(&pvFirst) < Radius)
		{
			bHit = true;
			break;
		}

		// 内積の値が0より大きく線分のベクトルの大きさより小さいなら
		if (0 < DotEnd && DotEnd < Length)
		{
			// svを長さDotのベクトルにする
			sv *= DotEnd;

			// ベクトルの引き算でpvを[線分から球体の中心点までの最短ベクトルにする]
			pvEnd -= sv;

			// 交差した
			if (D3DXVec3Length(&pvEnd) < Radius)
			{
				bHit = true;
				break;
			}
		}
		// 内積の値が0より大きく線分のベクトルの大きさより小さいなら
		else if (0 < DotFirst && DotFirst < Length)
		{
			// svを長さDotのベクトルにする
			sv *= DotFirst;

			// ベクトルの引き算でpvを[線分から球体の中心点までの最短ベクトルにする]
			pvFirst -= sv;

			// 交差した
			if (D3DXVec3Length(&pvEnd) < Radius)
			{
				bHit = true;
				break;
			}
		}
	}

	// 武器を持っているか判定する
	const bool is_HaveWepon = g_player.Motion.nNumModel == MAX_MODEL;

	// スペシャル攻撃をしているかを判定
	const bool is_NotSpAttack = g_player.AttackSp == false;

	// 敵の状態を判定する
	const bool is_NotDamageState = pEnemy->state != ENEMYSTATE_DAMAGE;

	// 攻撃状態を判定する
	const bool is_NotAction = isPlayerAttaking() == true;

	// 攻撃できるかどうかを判定する
	const bool is_Damage = is_HaveWepon == true && is_NotSpAttack == true && is_NotDamageState == true && is_NotAction == true;

	// キー	
	int nKey = g_player.Motion.nKey;

	// 最大のキー
	int nLastKey = g_player.Motion.aMotionInfo[g_player.Motion.motionType].nNumkey - 1;

	// 最後のフレーム
	int LastFrame = g_player.Motion.aMotionInfo[g_player.Motion.motionType].aKeyInfo[nLastKey].nFrame * 0.5f;

	// モーションカウンター
	int nCounter = g_player.Motion.nCountMotion;

	// プレイヤーが武器を持っているかつスペシャル攻撃じゃない
	if (is_Damage == true && bHit == true && CheckMotionBounds(nKey, nCounter,1, nLastKey,0, LastFrame) == true)
	{
		// 敵に当たった
		HitEnemy(nCntEnemy, (g_player.nDamage * 5));

		//// 耐久絵欲を減らす
		pItem[g_player.ItemIdx].durability--;
		return;
	}

	// 武器を持っているか判定する
	const bool is_NotWepon = g_player.Motion.nNumModel == MAX_MODEL - 1;

	// 攻撃できるかどうかを判定する
	const bool is_NotHandDamage = is_NotWepon == true && is_NotSpAttack == true && is_NotAction == true && is_NotDamageState == true;

	// モデルの位置を変数に代入
	D3DXVECTOR3 ModelPos(g_player.Motion.aModel[4].mtxWorld._41, g_player.Motion.aModel[4].mtxWorld._42, g_player.Motion.aModel[4].mtxWorld._43);

	// 素手の時かつ攻撃がスペシャルじゃない
	if(is_NotHandDamage == true && sphererange(&ModelPos, &pEnemy->pos, 30.0f, 65.0f) == true)
	{
		if (g_player.Motion.motionType == MOTIONTYPE_ACTION && g_player.Motion.nKey >= 2)
		{
			HitEnemy(nCntEnemy, g_player.nDamage * 3); // 敵に当たった
			return;
		}
	}

	// スペシャル攻撃をしているかを判定
	const bool is_SpAttack = g_player.AttackSp == true;

	// 攻撃できるかどうかを判定する
	const bool is_SpDamage = is_HaveWepon == true && is_SpAttack == true && is_NotDamageState == true && is_NotAction == true;

	// 剣を持っているかつスペシャル攻撃中
	if (is_SpDamage == true && sphererange(&g_player.pos,&pEnemy->pos,200.0f,50.0f) == true)
	{
		// 範囲内にいたら
		if (g_player.WeponMotion != MOTION_SPPIERCING && g_player.WeponMotion != MOTION_SPDOUBLE && CheckMotionBounds(nKey, nCounter, 3, nLastKey, 0, LastFrame) == true)
		{
			// 敵にダメージを与える
			HitEnemy(nCntEnemy, (g_player.nDamage * 50));
			return;
		}
		// 範囲内にいたら
		if (sphererange(&g_player.pos, &pEnemy->pos, 200.0f, 50.0f) && g_player.WeponMotion == MOTION_SPPIERCING && CheckMotionBounds(nKey, nCounter, 19, nLastKey, 0, LastFrame) == true)
		{
			// 敵にダメージを与える
			HitEnemy(nCntEnemy, (g_player.nDamage * 50));
			return;
		}

		// 範囲内にいたら
		if (sphererange(&g_player.pos, &pEnemy->pos, 250.0f, 50.0f) && g_player.WeponMotion == MOTION_SPDOUBLE && CheckMotionBounds(nKey, nCounter, 3, nLastKey, 0, LastFrame) == true)
		{
			// 敵にダメージを与える
			HitEnemy(nCntEnemy, (g_player.nDamage * 50));
			return;
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
			pItem[nIdx].move.y = dest.y * 10.0f;
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

			pItem[nIdx].move.x = dest.x * 10.0f;
			pItem[nIdx].move.y = dest.y * 10.0f;
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
		g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].aModel[nCntModel]; // モデルの情報を代入
	}
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].aMotionInfo[nCntMotion];
	}

	// 投げた後に武器を消す
	g_player.Motion.nNumModel -= 1;

	// プレイヤーの状態を何も持っていない状態にする
	g_player.HandState = PLAYERHOLD_NO;

	// 状態を投げられてる状態にする
	pItem[nIdx].state = ITEMSTATE_THROW;

	g_player.speed = 4.0f; // プレイヤーの移動速度をリセット
	g_player.fStockSpeed = 4.0f; // プレイヤーの移動速度を保存
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
		//g_player.rotDestPlayer.y = SetAttackerAngle(nCnt, ATTACKER_BOSS);
	}
}
//===============================================================================================================
// プレイヤーとアイテムの判定
//===============================================================================================================
bool CollisionItem(int nIdx, float Itemrange, float plrange)
{
	GAMESTATE gamestate = GetGameState();

	// ムービーだったら
	if (gamestate == GAMESTATE_MOVIE || g_player.bCraft == true || g_player.AttackSp == true)
	{
		// 関数を抜ける
		return false;
	}

	Item* pItem = GetItem();

	ITEM_INFO* pItemInfo = GetItemInfo();

	Billboard* pBillboard = GetBillBoard();


	MODEL_INFO* ItemTexture = GetItemOrigin();

	bool bCollision = false; // 当たっているかどうか

	// ダメージ状態かを判定
	const bool is_Damage = g_player.bstiffness == false;

	// 範囲内かを判定
	const bool Inbounds = sphererange(&g_player.pos, &pItem[nIdx].pos, 100.0f, 20.0f) == true;

	// 状態がノーマルか判定
	const bool stateNormal = pItem[nIdx].state == ITEMSTATE_NORMAL;

	// 死んでいるかを判定
	const bool NotDeth = g_player.Motion.motionType != MOTIONTYPE_DEATH;

	// 拾えるかを判定
	const bool CanPickUp = is_Damage && sphererange(&g_player.pos, &pItem[nIdx].pos, 20.0f, 20.0f) == true && stateNormal && NotDeth;

	// 範囲内だったら
	if (Inbounds == true)
	{
		bCollision = true;
	}

	// 範囲内に入った
	if (CanPickUp == true)
	{	
		// アイテムの状態が普通だったら
		if (pItem[nIdx].state == ITEMSTATE_NORMAL)
		{
			int nIdxBillboard = pItem[nIdx].nIdxBillboardCount;
			// ビルボードセット
			pBillboard[nIdxBillboard].state = BILLBOARDSTATE_SET;
		}


		if ((KeyboardTrigger(DIK_E) || JoypadTrigger(JOYKEY_LEFT_B)) && g_player.Combostate == COMBO_NO)
		{
			// インデックス番号のビルボードを非表示
			DeletIdxBillboard(pItem[nIdx].nIdxBillboardCount);

			if (pItem[nIdx].nType == ITEMTYPE_ONIGIRI)
			{
				// 音楽再生
				PlaySound(SOUND_LABEL_LIFERECOVERY);

				// モデルの位置を代入
				D3DXVECTOR3 pos = SetMtxConversion(g_player.Motion.aModel[1].mtxWorld);

				// シリンダーをセット
				g_player.nIdxCylinder = SetMeshCylinder(D3DXVECTOR3(g_player.pos.x, g_player.pos.y, g_player.pos.z),1,60,40.0f,D3DCOLOR_RGBA(59,255,0,255),16,1,2.0f,12.0f);

				LoadEffect(2, pos);

				SetParticle(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + g_player.Size.y / 1.5f, g_player.pos.z),
					D3DXVECTOR3(1.57f, g_player.rot.y, 1.57f),
					D3DXVECTOR3(0.2f, 3.14f, 0.2f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f),
					2.0f, 4, 40, 40, 4.0f, 40.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

				SetParticle(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + g_player.Size.y / 1.5f, g_player.pos.z),
					D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z),
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f),
					2.0f, 4, 40, 20, 10.0f, 40.0f,
					true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));

				// Hpを増やす
				g_player.nLife += HEAL_VALUE;

				// アイテムを使用状態にする
				pItem[nIdx].bUse = false;

				// 関数を抜ける
				return false;
			}

			// ミニマップのアイテムのアイコンのリセット
			ResetItemMinimap();

			// モーションをニュートラルに戻す
			SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, true, 10);

			// 音楽再生
			PlaySound(SOUND_LABEL_ITEM_SE);

			pItem[g_player.ItemIdx].Power = pItemInfo[pItem[nIdx].nType].Power;

			Itemchange(nIdx,pItem[nIdx].nType); // アイテムを拾う
			
			pItem[nIdx].bUse = false;      // 消す

			if (g_player.Itembreak[g_player.ItemIdx] == true)
			{
				// アイテムが壊れた判定をリセット
				g_player.Itembreak[g_player.ItemIdx] = false;
			}

			// 武器を持っていたら
			if (g_player.Motion.nNumModel == MAX_MODEL)
			{
				// アイテム変更時のパラメータ
				ChangeItemParam(g_player.ItemIdx, pItem[g_player.ItemIdx].nType);
			}

			// 状態をホールドにする
			pItem[nIdx].state = ITEMSTATE_HOLD;

			// アイテムのステータス変更
			LoadItemChange(pItem[nIdx].nType,pItem[nIdx].Size.y);

			g_player.ItemIdx = nIdx;	   // インデックスを渡す

			LoadMinimapMatItem(g_player.ItemIdx, g_player.StockItemIdx);
		}
	}

	// 範囲外だったら
	if (sphererange(&g_player.pos, &pItem[nIdx].pos, Itemrange, plrange) == false)
	{
		int nIdxBillboard = pItem[nIdx].nIdxBillboardCount;

		// ビルボードを消す
		pBillboard[nIdxBillboard].state = BILLBOARDSTATE_NOSET;
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

	g_player.nCounterAttack = 30;						  // 状態カウンターを設定
	g_AttackState = AttackState;                          // コンボの状態
	g_player.AttackState = PLAYERATTACKSTATE_ATTACK;	  // プレイヤーの状態を攻撃にする	
	g_player.Combostate = Combstate;					  // コンボの状態を設定
	SetMotion(&g_player.Motion, motiontype, true,10);

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
		if (sphererange(&g_player.pos, &pBoss[nCnt].pos, 50.0f, 200.0f) && pBoss[nCnt].bUse)
		{
			// 範囲にいる間だけロックオン
			if (sphererange(&g_player.pos, &pBoss[nCnt].pos, 150.0f, 150.0f))
			{
				// 角度を求める
				float fAngle = atan2f(pBoss[nCnt].pos.x - g_player.pos.x, pBoss[nCnt].pos.z - g_player.pos.z);
				g_player.rotDestPlayer.y = fAngle + D3DX_PI;
				return;
			}

			// ボスの場所を向く
			if (g_player.WeponMotion == MOTION_DBHAND && g_player.Motion.motionType == MOTIONTYPE_ACTION2 && GetKeyboardPress(DIK_W))
			{
				g_player.move.x = sinf(g_player.rot.y + D3DX_PI) * 80.0f;
				g_player.move.z = cosf(g_player.rot.y + D3DX_PI) * 80.0f;
				return;
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
				return;
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
		g_player.Motion.aModel[nCntModel] = g_LoadPlayer[LoadPlayer].aModel[nCntModel]; // モデルの情報を代入
	}
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[LoadPlayer].aMotionInfo[nCntMotion]; // モーションの情報を代入
	}

	if (LoadPlayer == PLAYERTYPE_WEPON) // プレイヤーがノーハンドだったら情報を代入しない
	{
		g_player.WeponMotion = itemtype; // アイテムごとのモーションタイプ
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION] = g_LoadMotion[itemtype].aMotionInfo[0];		// 攻撃1の情報を代入
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION2] = g_LoadMotion[itemtype].aMotionInfo[1];    // 攻撃2の情報を代入
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION3] = g_LoadMotion[itemtype].aMotionInfo[2];	// 攻撃3の情報を代入
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION4] = g_LoadMotion[itemtype].aMotionInfo[3];	// 攻撃4の情報を代入
		g_player.Motion.aMotionInfo[MOTIONTYPE_JUMPACTION] = g_LoadMotion[itemtype].aMotionInfo[4];	// 攻撃4の情報を代入
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
// モーションの判定設定処理
//===============================================================================================================
bool CheckMotionBounds(int nKey, int nCountFrame, int StartKey, int EndKey, int startFrame, int EndFrame)
{
	// 判定用変数
	bool bFlag = false;

	if (nKey >= StartKey && nKey <= EndKey && nCountFrame >= startFrame && nCountFrame <= EndFrame)
	{
		// 判定開始
		bFlag = true;
	}

	// 判定を返す
	return bFlag;
}
//===============================================================================================================
// アタッカーを調べる関数
//===============================================================================================================
float SetAttackerAngle(int AttackerIdx, int AttackerType)
{
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_AVOID || g_player.Motion.motiontypeBlend == MOTIONTYPE_DEATH)
	{
		return g_player.rot.y;
	}

	// 角度
	float OutAngle = 0.0f;

	// 敵を取得
	ENEMY* pEnemy = GetEnemy();

	// ボスを取得
	Boss* pBoss = Getboss();

	// アタッカーが敵
	if (AttackerType == ATTACKER_ENEMY)
	{
		// 差分を求める
		D3DXVECTOR3 DiffPos = g_player.pos - pEnemy[AttackerIdx].pos;

		// 角度を求める
		OutAngle = atan2f(DiffPos.x, DiffPos.z);
	}
	// アタッカーがボス
	else if (AttackerType == ATTACKER_BOSS)
	{
		// 差分を求める
		D3DXVECTOR3 DiffPos = g_player.pos - pBoss[AttackerIdx].posOld;

		// 角度を求める
		OutAngle = atan2f(DiffPos.x, DiffPos.z);
	}

	return OutAngle;
}
//===============================================================================================================
// 線分と円の当たり判定
//===============================================================================================================
bool CollisionLine()
{
	return false;
}
//===============================================================================================================
// プレイヤーのモーションのロード
//===============================================================================================================
void LoadMotion(int Weponmotion)
{
	int nIdxPos = 0;
	int nIdxRot = 0;

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

	if (Weponmotion != MOTION_SP)
	{
		nIdxPos = 15;
		nIdxRot = 15;
	}

	int nCntKey = 0;
	int nCntPosKey = 0;
	int nCntRotkey = 0;

	if (pFile != NULL)
	{
		char aString[MAX_WORD];

		while (1)
		{
			// 文字を読み取る
			fscanf(pFile, "%s", &aString[0]);

			// コメントを飛ばす
			if (strcmp(&aString[0], "#") == 0 || strcmp(&aString[0], "<<") == 0)
			{
				continue;
			}

			// パーツセットを読み取ったら
			if (strcmp(&aString[0], "PARTSSET") == 0)
			{
				LoadWeponOffSet(pFile, &aString[0], Weponmotion);
			}

			// MOTIONSETを読み取ったら
			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{
				// モーションの設定処理
				LoadWeponMotionSet(pFile, &aString[0], 0, Weponmotion);
			}

			// END_SCRIPTを読み取ったら
			if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{

				nKey = 0;
				nCntMotion = 0; // モーションのカウントをリセットする
				break;          // While文を抜ける
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
	int nScanData = 0;

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
			int nData = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], "#") == 0)
			{//コメントが来たら
				//処理を読み飛ばす
				continue;
			}

			// NUM_MODELを読み取ったら
			if (strcmp(&aStr[0], "NUM_MODEL") == 0)
			{
				nScanData = fscanf(pFile, "%s", &Skip[0]);						// [=]を読み飛ばす
				nScanData = fscanf(pFile, "%d", &nNumModel);					// モデルの最大数を代入
				g_LoadPlayer[nType].nNumModel = nNumModel;			// モデルの最大数を代入
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
			if (nData == EOF)
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
	int nScanData = 0; // 返り値代入用

	// カウントがモデル数より下だったら
	while (nCntLoadModel < nNumModel)
	{
		// 文字を読み取る
		nScanData = fscanf(pFile, "%s", aString);

		// MODEL_FILENAMEを読み取ったら
		if (strcmp(aString, "MODEL_FILENAME") == 0)
		{
			nScanData = fscanf(pFile, "%s", &Skip[0]); // [=]を読み飛ばす
			nScanData = fscanf(pFile, "%s", aString);  // ファイル名を読み取る

			const char* FILE_NAME = {};    // ファイル名代入用変数

			FILE_NAME = aString;           // ファイル名を代入

			//Xファイルの読み込み
			D3DXLoadMeshFromX(FILE_NAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_LoadPlayer[nType].aModel[nCntLoadModel].pBuffMat,
				NULL,
				&g_LoadPlayer[nType].aModel[nCntLoadModel].dwNumMat,
				&g_LoadPlayer[nType].aModel[nCntLoadModel].pMesh);

			//マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_LoadPlayer[nType].aModel[nCntLoadModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadPlayer[nType].aModel[nCntLoadModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//このファイル名を使用してテクスチャを読み込む
						//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_LoadPlayer[nType].aModel[nCntLoadModel].pTexture[nCntMat]);
				}
			}

			//頂点数の取得
			nNumVtx = g_LoadPlayer[nType].aModel[nCntLoadModel].pMesh->GetNumVertices();

			//頂点フォーマットのサイズ取得
			sizeFVF = D3DXGetFVFVertexSize(g_LoadPlayer[nType].aModel[nCntLoadModel].pMesh->GetFVF());

			//頂点バッファのロック
			g_LoadPlayer[nType].aModel[nCntLoadModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//頂点座標を比較してブロックの最小値,最大値を取得
				if (vtx.x < g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.x)
				{
					g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.y)
				{
					g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.z)
				{
					g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.x)
				{
					g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.y)
				{
					g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.z)
				{
					g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			////サイズを代入
			//g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			//g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			//g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

			//各モデルごとのサイズを代入
			g_LoadPlayer[nType].aModel[nCntLoadModel].Size.x = g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.x - g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.x;
			g_LoadPlayer[nType].aModel[nCntLoadModel].Size.y = g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.y - g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.y;
			g_LoadPlayer[nType].aModel[nCntLoadModel].Size.z = g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.z - g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.z;

			//頂点バッファのアンロック
			g_LoadPlayer[nType].aModel[nCntLoadModel].pMesh->UnlockVertexBuffer();

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
	int nScanData = 0; // 返り値を代入する変数

	// END_CHARACTERSETを読み取ってなかったら
	while (strcmp(aString, "END_CHARACTERSET") != 0)
	{
		// 文字を読み取る
		nScanData = fscanf(pFile, "%s", aString);

		// INDEXを読み取ったら
		if (strcmp(aString, "INDEX") == 0)
		{
			nScanData = fscanf(pFile, "%s", &Skip[0]); // [=]読み飛ばす
			nScanData = fscanf(pFile, "%d", &nIdx);    // インデックスを代入
		}
		// PARENTを読み取ったら
		else if (strcmp(aString, "PARENT") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// 親のインデックスを保存
			nScanData = fscanf(pFile, "%d", &g_LoadPlayer[nType].aModel[nIdx].nIdxModelParent);
		}
		// POSを読み取ったら
		else if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aModel[nIdx].offpos.x);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aModel[nIdx].offpos.y);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aModel[nIdx].offpos.z);
		}
		// ROTを読み取ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aModel[nIdx].offrot.x);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aModel[nIdx].offrot.y);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aModel[nIdx].offrot.z);
		}
	}
}
//===============================================================================================================
// プレイヤーの武器のオフセットの設定処理
//===============================================================================================================
void LoadWeponOffSet(FILE* pFile, char* aString, int nWepontype)
{
	int nIdx = 15; // インデックス格納変数
	char Skip[3] = {}; // [=]読み飛ばし変数
	int nScanData = 0; // 返り値代入用

	// END_CHARACTERSETを読み取ってなかったら
	while (strcmp(aString, "END_PARTSSET") != 0)
	{
		// 文字を読み取る
		nScanData = fscanf(pFile, "%s", aString);

		// POSを読み取ったら
		if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aModel[nIdx].offpos.x);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aModel[nIdx].offpos.y);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aModel[nIdx].offpos.z);
		}
		// ROTを読み取ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aModel[nIdx].offrot.x);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aModel[nIdx].offrot.y);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aModel[nIdx].offrot.z);
		}
	}
}
//===============================================================================================================
// プレイヤーのモーションセット
//===============================================================================================================
void LoadMotionSet(FILE* pFile, char* aString, int nNumModel, int nType)
{
	char Skip[3] = {}; // [=]読み飛ばし変数
	int nScanData = 0; // 返り値代入用

	while (1)
	{
		// 文字を読み取る
		nScanData = fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0 || strcmp(aString, "<<") == 0)
		{// コメントが来たら
			// コメントを読み飛ばす
			continue;
		}

		// LOOPを読み通ったら
		if (strcmp(aString, "LOOP") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ループするかしないか
			nScanData = fscanf(pFile, "%d", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].bLoop);
		}
		// NUM_KEYを読み通ったら
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// キーの最大数を代入
			nScanData = fscanf(pFile, "%d", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].nNumkey);
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
	int nScanData = 0; // 返り値代入用

	while (1)
	{
		// 文字を読み取る
		nScanData = fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0)
		{// コメントが来たら
			// コメントを読み飛ばす
			continue;
		}

		// FRAMEを読み通ったら
		if (strcmp(aString, "FRAME") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// フレームを読み込む
			nScanData = fscanf(pFile, "%d", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].nFrame);
		}

		// POSを読み通ったら
		if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// 位置を読み込む
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosX);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosY);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROTを読み通ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// 角度を読み込む
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotX);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotY);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotZ);
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
//===============================================================================================================
// プレイヤーの武器ごとのモーションのロード処理
//===============================================================================================================
void LoadWeponMotionSet(FILE* pFile, char* aString, int nNumModel, int nWepontype)
{
	char Skip[3] = {}; // [=]読み飛ばし変数
	int nScanData = 0; // 返り値代入用

	while (1)
	{
		// 文字を読み取る
		nScanData = fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0 || strcmp(aString, "<<") == 0)
		{// コメントが来たら
			// コメントを読み飛ばす
			continue;
		}

		// LOOPを読み通ったら
		if (strcmp(aString, "LOOP") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ループするかしないか
			nScanData = fscanf(pFile, "%d", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].bLoop);
		}
		// NUM_KEYを読み通ったら
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// キーの最大数を代入
			nScanData = fscanf(pFile, "%d", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].nNumkey);
		}
		// KEYSETを読み通ったら
		if (strcmp(aString, "KEYSET") == 0)
		{
			// キーの設定処理
			LoadWeponKeySet(pFile, aString, nWepontype, nCntMotion);
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
// プレイヤーの武器のごとモーションのキーのロード処理
//===============================================================================================================
void LoadWeponKeySet(FILE* pFile, char* aString, int nWepontype, int nCntMotion)
{
	char Skip[3] = {}; // [=]読み飛ばし変数
	int nCntPos = 0;   // 位置のカウント
	int nCntRot = 0;   // 角度のカウント
	int nScanData = 0; // 返り値代入用

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
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// フレームを読み込む
			nScanData = fscanf(pFile, "%d", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].nFrame);
		}

		// POSを読み通ったら
		if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// 位置を読み込む
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosX);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosY);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROTを読み通ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// 角度を読み込む
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotX);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotY);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotZ);
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
//===============================================================================================================
// プレイヤーのエフェクトの設定処理
//===============================================================================================================
void SetElementEffect(void)
{
	D3DXVECTOR3 SwordPos(
		g_player.SwordMtx._41, // X方向
		g_player.SwordMtx._42, // Y方向
		g_player.SwordMtx._43  // Z方向
	);

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
}
//===============================================================================================================
// プレイヤーのモーションの演出設定
//===============================================================================================================
void SetMotionContller(void)
{
	// キー
	int nKey = g_player.Motion.nKey;

	// モーションカウント
	int nCounter = g_player.Motion.nCountMotion;

	// 両手のスペシャル攻撃の時
	if (g_player.AttackSp == true && g_player.WeponMotion == MOTION_SPDOUBLE && g_player.Motion.motionType == MOTIONTYPE_ACTION && CheckMotionBounds(nKey, nCounter, 2, 2, 5, 5) == true)
	{
		// コントローラーの振動(1000で一秒)
		SetVibration(35000, 35000, 500);

		// 衝撃波を発生指せる
		SetImpact(g_player.pos, COLOR_ORANGERED, 32, 60.0f, 15.0f, 3.0f, 60, IMPACTTYPE_PLAYER,20);

		// カメラの揺れ
		WaveCamera(25);
	}

	// 両手のスペシャル攻撃の時
	if (g_player.AttackSp == true && g_player.WeponMotion == MOTION_ONEHANDBLOW && CheckMotionBounds(nKey, nCounter, 4, 4, 5, 5) == true)
	{
		// コントローラーの振動(1000で一秒)
		SetVibration(45000, 45000,300);

		// 衝撃波を発生指せる
		SetImpact(g_player.pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 32, 60.0f, 15.0f, 10.0f, 60, IMPACTTYPE_PLAYER, g_player.nDamage * 50);

		// カメラの揺れ
		WaveCamera(25);
	}

	// 槍のスペシャル攻撃の時
	if (g_player.AttackSp == true && g_player.WeponMotion == MOTION_SPPIERCING && CheckMotionBounds(nKey, nCounter, 21, 21, 5, 5) == true)
	{
		// コントローラーの振動(1000で一秒)
		SetVibration(45000, 45000, 300);

		// 衝撃波を発生指せる
		SetImpact(g_player.pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 32, 60.0f, 15.0f, 3.0f, 60, IMPACTTYPE_PLAYER, 20);

		// カメラの揺れ
		WaveCamera(25);
	}

	// 槍のスペシャル攻撃の時
	if (g_player.AttackSp == true && g_player.WeponMotion == MOTION_SPPIERCING && g_player.Motion.nKey != NULL)
	{
		int SetKey = 4;

		// 発生キー
		if (g_player.Motion.nKey % SetKey== 0 && g_player.Motion.nKey <= 16)
		{
			// 衝撃波を発生指せる
			SetImpact(g_player.pos, D3DCOLOR_RGBA(100,100,100,255), 32, 30.0f, 20.0f, 3.0f, 60, IMPACTTYPE_NORMAL, 0);
		}

		if (CheckMotionBounds(nKey, nCounter, 21, 21, 0, 0) == true)
		{
			// カメラの揺れ
			WaveCamera(25);
		}
	}

	// 刀のスペシャル
	if (g_player.AttackSp == true && g_player.WeponMotion == MOTION_SP && CheckMotionBounds(nKey, nCounter, 0, 0, 0, 0) == true)
	{
		// 衝撃波を発生指せる
		SetImpact(g_player.pos, D3DCOLOR_RGBA(0, 161, 255, 255), 32, 200.0f, 180.0f, 1.6f, 90, IMPACTTYPE_SPKATANA, 1);
	}

	// 刀のスペシャル
	if (g_player.AttackSp == true && g_player.WeponMotion == MOTION_SP && CheckMotionBounds(nKey, nCounter, 4, 4, 1, 1) == true)
	{
		// コントローラーの振動(1000で一秒)
		SetVibration(45000, 45000, 300);

		// 衝撃波を発生指せる
		SetImpact(g_player.pos, D3DCOLOR_RGBA(0, 161, 255, 255), 32, 40.0f, 20.0f, 35.0f, 60, IMPACTTYPE_NORMAL, 1);
	}

	// モーションの種類が歩き
	if (g_player.Motion.motionType == MOTIONTYPE_MOVE && g_player.Motion.bFirstMotion == false)
	{
		// キ一1番目かつカウントが5
		if (CheckMotionBounds(nKey, nCounter, 1, 1, 5, 5) == true)
		{
			SetExplosion(D3DXVECTOR3(g_player.Motion.aModel[14].mtxWorld._41, g_player.Motion.aModel[14].mtxWorld._42, g_player.Motion.aModel[14].mtxWorld._43),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 20.0f, 20.0f, 0);
		}
		// キ一3番目かつカウントが5
		else if (CheckMotionBounds(nKey, nCounter, 3, 3, 5, 5) == true)
		{
			SetExplosion(D3DXVECTOR3(g_player.Motion.aModel[11].mtxWorld._41, g_player.Motion.aModel[11].mtxWorld._42, g_player.Motion.aModel[11].mtxWorld._43),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 20.0f, 20.0f, 0);
		}
	}

	// ワールドマトリックスを変換
	D3DXVECTOR3 SwordPos = SetMtxConversion(g_player.SwordMtx);

	// ハンマーのスペシャルモーションの設定
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
			2.0f, 4, 30, 40, 4.0f, 0.0f,
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
			6.0f, 4, 80, 300, 6.0f, 30.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}	//モーションの更新

	Boss* pBoss = Getboss();

	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_DAMAGE && g_player.Motion.bFirstMotion == true)
	{
		g_player.move.x = sinf(pBoss[g_player.AttackerIdx].rot.y + D3DX_PI) * 15.0f;
		g_player.move.y = 5.0f;
		g_player.move.z = cosf(pBoss[g_player.AttackerIdx].rot.y + D3DX_PI) * 15.0f;
	}

	// ダメージモーションだったら
	const bool isDamage = g_player.Motion.motiontypeBlend == MOTIONTYPE_DAMAGE && nKey <= 2;

	// ダメージだったら硬直する
	g_player.bstiffness = isDamage ? true : false;
	
	// ダメージモーションだったら
	const bool isAvoid = g_player.Motion.motiontypeBlend == MOTIONTYPE_AVOID && nKey <= 3;

	// 無敵時間だったら無敵にする
	g_player.bAvoid = isAvoid ? true : false;

	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_LANDING && CheckMotionBounds(nKey,nCounter,0,0,1,1) == true)
	{
		// 煙
		LoadEffect(0, SetMtxConversion(g_player.Motion.aModel[11].mtxWorld));
	}

	// 大型武器モーションの時
	if (g_player.WeponMotion == MOTION_BIGWEPON && g_player.Motion.motionType == MOTIONTYPE_ACTION4 && g_player.Motion.nKey == 0 && GetKeyboardPress(DIK_W))
	{
		g_player.move.x = sinf(g_player.rot.y + D3DX_PI) * 30.0f;
		g_player.move.z = cosf(g_player.rot.y + D3DX_PI) * 30.0f;
	}
}
//===============================================================================================================
// プレイヤーの移動処理
//===============================================================================================================
void PlayerMove(void)
{
	Camera* pCamera = GetCamera();

	// ジャンプしてるかを判定
	const bool NotJump = g_player.Motion.motiontypeBlend != MOTIONTYPE_JUMP;

	// モーションがムーブかを判定
	const bool NotMove = g_player.Motion.motiontypeBlend != MOTIONTYPE_MOVE;

	// うごけるかを判定
	const bool is_MotionMove = NotJump == true && NotMove == true;

	// モーションが回避かつキーが3以下
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_AVOID && g_player.Motion.nKey <= KEY_THREE)
	{
		return;
	}

	if (GetKeyboardPress(DIK_A))
	{
		//プレイヤーの移動(上)
		if (GetKeyboardPress(DIK_W) == true)
		{
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
			}

			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.25f) * g_player.speed;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.25f) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.75f;
		}
		//プレイヤーの移動(下)
		else if (GetKeyboardPress(DIK_S))
		{
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
			}

			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.75f) * g_player.speed;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.75f) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.25f;
		}
		//プレイヤーの移動(左)
		else
		{
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
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
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
			}

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * g_player.speed;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.75f;
		}
		//プレイヤーの移動(下)
		else if (GetKeyboardPress(DIK_S))
		{
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
			}

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * g_player.speed;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.25f;
		}
		//プレイヤーの移動(右)
		else
		{
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
			}

			g_player.move.z -= sinf(pCamera->rot.y) * g_player.speed;
			g_player.move.x += cosf(pCamera->rot.y) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.5f;
		}

	}
	//プレイヤーの移動(上)
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (is_MotionMove == true)
		{
			SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
		}

		g_player.move.x += sinf(pCamera->rot.y) * g_player.speed;
		g_player.move.z += cosf(pCamera->rot.y) * g_player.speed;

		g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI;
	}
	//プレイヤーの移動(下)
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (is_MotionMove == true)
		{
			SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
		}

		g_player.move.x -= sinf(pCamera->rot.y) * g_player.speed;
		g_player.move.z -= cosf(pCamera->rot.y) * g_player.speed;

		g_player.rotDestPlayer.y = pCamera->rot.y;
	}
	else
	{
		if (g_player.Motion.motionType == MOTIONTYPE_MOVE)
		{
			SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, true, 5); // モーションをニュートラルにする
		}
	}

}
//===============================================================================================================
// プレイヤーのクラフト処理
//===============================================================================================================
void UpdatePlayerCraft(void)
{
	Item* pItem = GetItem();

	// クラフト状態じゃなかったら
	if ((KeyboardTrigger(DIK_TAB) || JoypadTrigger(JOYKEY_Y)) && g_player.bCraft == false && g_player.AttackSp == false)
	{
		// クラフト状態
		g_player.bCraft = true;

		// クラフト状態にする
		EnableCraft(true);

		EnableCraftIcon(pItem[g_player.ItemIdx].nType, pItem[g_player.StockItemIdx].nType);
	}
	// クラフト状態だったら
	else if ((KeyboardTrigger(DIK_TAB) || JoypadTrigger(JOYKEY_Y)) && g_player.bCraft == true && g_player.AttackSp == false && GetIconAnim() == false)
	{
		// クラフト状態
		g_player.bCraft = false;

		// クラフト状態にしない
		EnableCraft(false);
	}
}
//===============================================================================================================
// アイテムの破壊処理
//===============================================================================================================
void DestroyWepon(void)
{
	Item* pItem = GetItem();

	// 武器を持っているかつプレイヤーの持っているアイテムが壊れた
	if (g_player.Motion.nNumModel == MAX_MODEL && g_player.Itembreak[g_player.ItemIdx] == true)
	{
		SetGameUI(D3DXVECTOR3(125.0f, 500.0f, 0.0f), UITYPE_DESTORY, 100.0f, 25.0f,true, 240);

		// ブレンドなしでニュートラルにする
		SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, false, 10);

		// モーションを歩きにする(第2引数に1を入れる)
		MotionChange(MOTION_DBHAND, 1);

		// 素手の時のモーション情報を代入
		for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
		{
			g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].aModel[nCntModel]; // モデルの情報を代入
		}
		for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
		{
			g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].aMotionInfo[nCntMotion];
		}

		StatusChange(4.0f, D3DXVECTOR3(0.0f, 30.0f, 0.0f), 50);

		// 投げた後に武器を消す
		g_player.Motion.nNumModel -= 1;

		// プレイヤーの状態を何も持っていない状態にする
		g_player.HandState = PLAYERHOLD_NO;

		// 状態を変更
		pItem[g_player.ItemIdx].state = ITEMSTATE_NORMAL;

		// モーションをニュートラルにする
		SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, true, 10); // モーションをニュートラルにする
	}
}
//===============================================================================================================
// アイテムのドロップ処理
//===============================================================================================================
void DropItem(void)
{
	Item* pItem = GetItem();

	// プレイヤーが武器を持っているかを判定
	const bool is_HaveWepon = g_player.Motion.nNumModel == MAX_MODEL;

	// プレイヤーが死んでいないかを判定
	const bool is_Alive = g_player.Motion.motionType != MOTIONTYPE_DEATH;

	// プレイヤーがスペシャル攻撃していないかを判定
	const bool is_NotSpAttack = g_player.AttackSp == false;

	// プレイヤーが攻撃してないかを判定
	const bool is_NotAction = g_player.Combostate == COMBO_NO;

	// プレイヤーがジャンプしてないかを判定
	const bool is_NotJumping = g_player.bJump == true;

	// 物を置けるか置けないかを判定
	const bool is_DropItem = is_HaveWepon == true && is_Alive == true && is_NotSpAttack == true && is_NotAction == true && is_NotJumping == true;

	// 持っているアイテムを置く処理
	if ((KeyboardTrigger(DIK_G) || JoypadTrigger(JOYKEY_B)) && is_DropItem == true)
	{
		// ブレンドなしでニュートラルにする
		SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, false, 10);

		// モーションを歩きにする(第2引数に1を入れる)
		MotionChange(MOTION_DBHAND, 1);

		StatusChange(4.0f, D3DXVECTOR3(0.0f, 30.0f, 0.0f), 50);

		// 投げた後に武器を消す
		g_player.Motion.nNumModel -= 1;

		// プレイヤーの状態を何も持っていない状態にする
		g_player.HandState = PLAYERHOLD_NO;

		pItem[g_player.ItemIdx].bUse = true;
		pItem[g_player.ItemIdx].pos = g_player.pos;

		pItem[g_player.ItemIdx].state = ITEMSTATE_RELEASE;
		pItem[g_player.ItemIdx].nCounterState = 20;
	}
}
//===============================================================================================================
// スペシャルアタックの処理
//===============================================================================================================
void HandleSpecialAttack(void)
{
	Item* pItem = GetItem();

	// 武器を持っているかを判定
	const bool is_HaveWepon = g_player.Motion.nNumModel == MAX_MODEL;

	// 投げ物を持っていないかを判定
	const bool is_NotHaveThrowItem = g_player.HandState != PLAYERHOLD_HOLD;

	// 生存しているかを判定
	const bool is_Alive = g_player.Motion.motionType != MOTIONTYPE_DEATH;

	// ダメージ状態か
	const bool is_Damage = g_player.bstiffness == false;

	// スペシャルモーションを発動できるかを判定
	const bool is_SpecialAttack = is_HaveWepon && is_NotHaveThrowItem && is_Alive && is_Damage && g_player.Motion.motiontypeBlend != MOTIONTYPE_AVOID;

	// スペシャルモードになった時の攻撃
	if ((KeyboardTrigger(DIK_Q) || JoypadTrigger(JOYKEY_LS) || JoypadTrigger(JOYKEY_RS)) && is_SpecialAttack == true)
	{
		if (g_player.Combostate == COMBO_NO && g_player.AttackSp == false && g_player.SpMode == true)                   // SPゲージがたまった
		{
			ResetMeshSword(); //剣の軌跡のリセット
			DecSpgauge(100.0f);
			g_player.AttackSp = true;          // SP技を発動している

			// ブレンドなしでニュートラルにする
			SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, false, 10);

			switch (g_player.WeponMotion)
			{
			case MOTION_KATANA:
				WaveCamera(120); // カメラを揺らす
				g_player.SwordOffpos.y = 250.0f;
				MotionChange(MOTION_SP, 0);
				SetGameUI(D3DXVECTOR3(640.0f, 360.0f, 0.0f), UITYPE_BLACK, 640.0f, 380.0f,false, 0);
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
				g_player.SwordOffpos.y = 100.0f;
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

	// 最後のキー
	int LastKey = g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION].nNumkey - 1;

	// スペシャル攻撃状態かを判定
	const bool is_SpAttack = g_player.AttackSp == true;

	// 最後のキーかを判定
	const bool is_LastKey = g_player.Motion.nKey >= LastKey;

	// モーションが終わったかどうかを判定
	const bool is_FinishMotion = g_player.Motion.bFinishMotion == true;

	// ブレンドが終わったかどうかを判定
	const bool is_FinishBlend = g_player.Motion.nCounterBlend >= g_player.Motion.nFrameBlend * 0.5f;

	// もとに戻せるかを判定
	const bool is_restoreMotion = is_SpAttack && is_LastKey && is_FinishMotion && is_FinishBlend;

	// スペシャルモーションからもとに戻す
	if (is_restoreMotion == true)
	{
		// 音楽再生
		PlaySound(SPUND_LABEL_WEPONBREAK);

		g_player.SwordOffpos.y = 65.0f;		// 判定の長さを戻す
		g_player.Itembreak[g_player.ItemIdx] = true;
		//StatusChange(4.0f, D3DXVECTOR3(0.0f, 30.0f, 0.0f), 50); //能力値を戻す
		g_player.AttackSp = false;
		pItem[g_player.ItemIdx].state = ITEMSTATE_RELEASE;
		pItem[g_player.ItemIdx].nCounterState = 60;
	}
}
//===============================================================================================================
// プレイヤーのアイテムのストック処理
//===============================================================================================================
void UpdateItemStock(void)
{
	Item* pItem = GetItem();

	// 生存しているかを判定
	const bool is_Alive = g_player.Motion.motionType != MOTIONTYPE_DEATH;

	// スペシャル攻撃してるかを判定
	const bool is_NotSpAttack = g_player.AttackSp == false;

	// 武器を持っているかを判定
	const bool is_HaveWepon = g_player.Motion.nNumModel == MAX_MODEL;

	// ダメージ状態か
	const bool is_Damage = g_player.bstiffness == false;

	// アイテムをストックできるかを判定
	const bool is_StockItem = is_Alive && is_NotSpAttack && is_HaveWepon && is_Damage;

	// アイテムのストック
	if ((KeyboardTrigger(DIK_F) || JoypadTrigger(JOYKEY_RIGHT_B)) && is_StockItem == true && CheckActionMotion(&g_player.Motion) == true)
	{// Fキー or RBボタン

		// ミニマップのアイテムのリセット
		ResetItemMinimap();

		// ミニマップのアイテムのロード
		LoadMinimapMatItem(g_player.ItemIdx, g_player.StockItemIdx);

		// ブレンドなしでニュートラルにする
		SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, false, 10);

		if (pItem[g_player.StockItemIdx].state == ITEMSTATE_STOCK)
		{
			ChangeItemParam(g_player.StockItemIdx, pItem[g_player.StockItemIdx].nType);
		}

		// もともとストックしていたアイテムをノーマルに戻す
		pItem[g_player.StockItemIdx].state = ITEMSTATE_RELEASE;

		// 持っているアイテムの状態をストックにする
		pItem[g_player.ItemIdx].state = ITEMSTATE_STOCK;

		// ストックされたアイテムのインデックスを渡す
		g_player.StockItemIdx = g_player.ItemIdx;

		// プレイヤーの速度をリセット
		StatusChange(3.5f, D3DXVECTOR3(0.0f, 30.0f, 0.0f), 50);

		// モーションを歩きにする(第2引数に1を入れる)
		MotionChange(MOTION_DBHAND, 1);
		//// 素手の時のモーション情報を代入
		//for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
		//{
		//	g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].aModel[nCntModel]; // モデルの情報を代入
		//}
		//for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
		//{
		//	g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].aMotionInfo[nCntMotion];
		//}

		// 投げた後に武器を消す
		g_player.Motion.nNumModel -= 1;

		// プレイヤーの状態を何も持っていない状態にする
		g_player.HandState = PLAYERHOLD_NO;
	}
}
//===============================================================================================================
// プレイヤーの回避処理
//===============================================================================================================
void UpdatePlayerAvoid(void)
{
	Camera* pCamera = GetCamera();
	
	// イージングのカウント
	static int avoidEaseCnt = 0;

	// インクリメント
	avoidEaseCnt++;

	// イージングを設定
	float time = SetEase(avoidEaseCnt, 20);

	// 回避モーションかを判定
	const bool NotAvoid = g_player.Motion.motiontypeBlend != MOTIONTYPE_AVOID;

	//// モーションが終わったかどうかを判定
	//const bool NotFinish = g_player.Motion.bFinishMotion == false;

	// 回避モーションを発動できるかを判定
	const bool CanAvoid = NotAvoid == true && g_player.AttackSp == false && g_player.nLife > 0;

	// モーションが回避じゃない
	if ((OnMouseTriggerDown(RIGHT_MOUSE) == true || JoypadTrigger(JOYKEY_B) == true) && CanAvoid == true)
	{
		// 音楽再生
		PlaySound(SOUND_LABEL_AVOIDSE);

		// 無敵
		g_player.bAvoid = true;
		g_player.bstiffness = false;
		g_player.avoidMove = SetMotionMoveAngle();

		avoidEaseCnt = 0;

		// モーションを回避にする
		SetMotion(&g_player.Motion, MOTIONTYPE_AVOID, true, 10);
	}

	// モーションの種類が回避だったら
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_AVOID && g_player.Motion.nKey <= KEY_TWO)
	{	
		// 移動量をだんだん近づける
		g_player.move.x += (g_player.avoidMove.x - g_player.move.x) * EaseInOutQuad(time);
		g_player.move.z += (g_player.avoidMove.z - g_player.move.z) * EaseInOutQuad(time);
	}
	else
	{
		// 0に近づける
		g_player.move.x += (0.0f - g_player.move.x) * 0.0001f;
		g_player.move.z += (0.0f - g_player.move.z) * 0.0001f;

		// イージングのカウントをリセット
		avoidEaseCnt = 0;
	}
}
//===============================================================================================================
// モーションのアングルの設定
//===============================================================================================================
D3DXVECTOR3 SetMotionMoveAngle(void)
{
	static D3DXVECTOR3 OutPutMove = D3DXVECTOR3(0.0f,0.0f,0.0f);
	XINPUT_STATE* pStick;

	pStick = GetJoyStickAngle();

	Camera* pCamera = GetCamera();

	// パッドを入力してなかったら
	if (GetJoyStick() == false)
	{
		if (GetKeyboardPress(DIK_A))
		{
			//プレイヤーの移動(上)
			if (GetKeyboardPress(DIK_W) == true)
			{
				OutPutMove.x = sinf(pCamera->rot.y - D3DX_PI * 0.25f) * AVOID_MOVE;
				OutPutMove.z = cosf(pCamera->rot.y - D3DX_PI * 0.25f) * AVOID_MOVE;

				g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.75f;
			}
			//プレイヤーの移動(下)
			else if (GetKeyboardPress(DIK_S))
			{
				OutPutMove.x = sinf(pCamera->rot.y - D3DX_PI * 0.75f) * AVOID_MOVE;
				OutPutMove.z = cosf(pCamera->rot.y - D3DX_PI * 0.75f) * AVOID_MOVE;

				g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.25f;
			}
			//プレイヤーの移動(左)
			else
			{
				OutPutMove.x = sinf(pCamera->rot.y + D3DX_PI * 0.5f) * -AVOID_MOVE;
				OutPutMove.z = cosf(pCamera->rot.y + D3DX_PI * 0.5f) * -AVOID_MOVE;

				g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.5f;
			}
		}
		//プレイヤーの移動(右)
		else if (GetKeyboardPress(DIK_D))
		{
			//プレイヤーの移動(上)
			if (GetKeyboardPress(DIK_W))
			{
				OutPutMove.x = sinf(pCamera->rot.y + D3DX_PI * 0.25f) * AVOID_MOVE;
				OutPutMove.z = cosf(pCamera->rot.y + D3DX_PI * 0.25f) * AVOID_MOVE;

				g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.75f;
			}
			//プレイヤーの移動(下)
			else if (GetKeyboardPress(DIK_S))
			{
				OutPutMove.x = sinf(pCamera->rot.y + D3DX_PI * 0.75f) * AVOID_MOVE;
				OutPutMove.z = cosf(pCamera->rot.y + D3DX_PI * 0.75f) * AVOID_MOVE;

				g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.25f;
			}
			//プレイヤーの移動(右)
			else
			{
				OutPutMove.x = sinf(pCamera->rot.y + D3DX_PI * 0.5f) * AVOID_MOVE;
				OutPutMove.z = cosf(pCamera->rot.y + D3DX_PI * 0.5f) * AVOID_MOVE;

				g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.5f;
			}
		}
		//プレイヤーの移動(上)
		else if (GetKeyboardPress(DIK_W) == true)
		{
			OutPutMove.x = sinf(pCamera->rot.y) * AVOID_MOVE;
			OutPutMove.z = cosf(pCamera->rot.y) * AVOID_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI;
		}
		//プレイヤーの移動(下)
		else if (GetKeyboardPress(DIK_S) == true)
		{
			OutPutMove.x = sinf(pCamera->rot.y) * -AVOID_MOVE;
			OutPutMove.z = cosf(pCamera->rot.y) * -AVOID_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y;
		}
		else
		{
			OutPutMove.x = sinf(g_player.rot.y + D3DX_PI) * AVOID_MOVE;
			OutPutMove.z = cosf(g_player.rot.y + D3DX_PI) * AVOID_MOVE;
		}
	}

	// パッドを使っているか
	if (GetJoyStick() == true)
	{
		// Lスティックの角度
		float LStickAngleY = pStick->Gamepad.sThumbLY;
		float LStickAngleX = pStick->Gamepad.sThumbLX;

		// デッドゾーン
		float deadzone = 10920;

		// スティックの傾けた角度を求める
		float magnitude = sqrtf(LStickAngleX * LStickAngleX + LStickAngleY * LStickAngleY);

		// 動かせる
		if (magnitude > deadzone)
		{
			// アングルを正規化
			float normalizeX = (LStickAngleX / magnitude);
			float normalizeY = (LStickAngleY / magnitude);

			// プレイヤーの移動量
			float moveX = normalizeX * cosf(-pCamera->rot.y) - normalizeY * sinf(-pCamera->rot.y);
			float moveZ = normalizeX * sinf(-pCamera->rot.y) + normalizeY * cosf(-pCamera->rot.y);

			// プレイヤーの目的の角度を決める
			g_player.rotDestPlayer.y = atan2f(-moveX, -moveZ);

			// プレイヤーの移動
			OutPutMove.x = sinf(g_player.rotDestPlayer.y + D3DX_PI) * AVOID_MOVE;
			OutPutMove.z = cosf(g_player.rotDestPlayer.y + D3DX_PI) * AVOID_MOVE;
		}
	}

	// 移動量を返す
	return OutPutMove;
}
//===============================================================================================================
// 武器ごとのエフェクト処理
//===============================================================================================================
void SetWeponEffect(void)
{
}
//===============================================================================================================
// ダメージアクションかどうか
//===============================================================================================================
bool IsDamageAction(void)
{
	return false;
}
//===============================================================================================================
// アイテムの変更のパラメータ表示
//===============================================================================================================
void ChangeItemParam(int nHaveIdx,int nType)
{
	Item* pItem = GetItem();

	MODEL_INFO* ItemTexture = GetItemOrigin();

	pItem[nHaveIdx].bUse = true;
	pItem[nHaveIdx].ItemTex[nType] = ItemTexture[nType];
	pItem[nHaveIdx].pos.x = g_player.pos.x + (float)(rand() % 50 - 25.0f);
	pItem[nHaveIdx].pos.z = g_player.pos.z + (float)(rand() % 50 - 25.0f);
	pItem[nHaveIdx].state = ITEMSTATE_RELEASE;
	pItem[nHaveIdx].nCounterState = 60;
}
//===============================================================================================================
// モーションタイプの遷移
//===============================================================================================================
void MotionTransition(void)
{
	// モーションタイプの遷移
	switch (g_player.Motion.motionType)
	{
	case MOTIONTYPE_NEUTRAL:
		break;
	case MOTIONTYPE_MOVE:
		break;
	case MOTIONTYPE_ACTION:
		SetElementEffect();
		break;
	case MOTIONTYPE_JUMP:
		break;
	case MOTIONTYPE_LANDING:
		break;
	case MOTIONTYPE_ACTION2:
		SetElementEffect();
		break;
	case MOTIONTYPE_ACTION3:
		SetElementEffect();
		break;
	case MOTIONTYPE_ACTION4:
		SetElementEffect();
		break;
	default:
		break;
	}
}
//===============================================================================================================
// 状態の遷移
//===============================================================================================================
void StateTransition(void)
{
	// プレイヤーの状態の遷移
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
}
//===============================================================================================================
// コンボ状態の遷移
//===============================================================================================================
void ComboTransition(void)
{
	GAMESTATE gameState = GetGameState();

	// ムービーじゃ無かったら
	if (gameState != GAMESTATE_MOVIE)
	{
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
	}

}
//===============================================================================================================
// プレイヤーの攻撃の設定処理
//===============================================================================================================
void SetUpPlayerAttack(void)
{
	GAMESTATE gamestate = GetGameState();

	// ゲームの状態がムービーだったら
	if (gamestate == GAMESTATE_MOVIE) return;

	// モーションが回避だったら
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_AVOID) return;

	// スペシャル攻撃中だったら
	if (g_player.AttackSp == true) return;

	// モーションが死亡だったら
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_DEATH) return;

	// モーションのキー
	int nKey = g_player.Motion.nKey;

	// モーションのカウンター
	int nCounter = g_player.Motion.nCountMotion;

	// モーションの最後のキー
	int EndKey = g_player.Motion.aMotionInfo[g_player.Motion.motionType].nNumkey - 1;

	// モーションの最後のフレーム
	int EndFrame = g_player.Motion.aMotionInfo[g_player.Motion.motionType].aKeyInfo[EndKey].nFrame;

	// 攻撃できるか判定
	const bool isAttack = bNohand == false && g_player.bJump == true;

	// プレイヤーの状態が攻撃じゃないかつ地面にいる
	if (isAttack == true)
	{
		if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && (g_player.Combostate == COMBO_NO || g_player.Motion.motiontypeBlend == MOTIONTYPE_JUMPACTION))
		{
			PlayerComb(MOTIONTYPE_ACTION, 40, 40, COMBO_ATTACK1); // コンボ1
			ResetMeshSword();
		}
		else if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Motion.motiontypeBlend == MOTIONTYPE_ACTION &&
			CheckMotionBounds(nKey, nCounter, 0, EndKey, 0, EndFrame) == true)
		{
			PlayerComb(MOTIONTYPE_ACTION2, 40, 40, COMBO_ATTACK2); // コンボ2
		}
		else if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Motion.motiontypeBlend == MOTIONTYPE_ACTION2 &&
			CheckMotionBounds(nKey, nCounter, 1, EndKey, 0, EndFrame) == true)
		{
			PlayerComb(MOTIONTYPE_ACTION3, 40, 40, COMBO_ATTACK3); // コンボ3
		}
		else if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Motion.motiontypeBlend == MOTIONTYPE_ACTION3 &&
			CheckMotionBounds(nKey, nCounter, 1, EndKey, 0, EndFrame) == true)
		{
			PlayerComb(MOTIONTYPE_ACTION4, 70, 40, COMBO_ATTACK4); // コンボ4
		}
	}

	// ジャンプ攻撃できるか判定
	const bool isJumpAttack = bNohand == false && g_player.bJump == false && g_player.Combostate == COMBO_NO;

	if (isJumpAttack == true && (OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)))
	{
		ResetMeshSword();
		PlayerComb(MOTIONTYPE_JUMPACTION, 70, 70, COMBO_ATTACK4);
	}

	// 攻撃していないかつ手に投的武器を持っている
	const bool canAction = g_player.Combostate == COMBO_NO && bNohand == true && g_player.Motion.nNumModel == MAX_MODEL;

	// 投げ物を持っているときの攻撃
	if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && canAction == true)
	{
		PlayerComb(MOTIONTYPE_ACTION, 40, 20, COMBO_ATTACK1); // コンボ1
	}

	// ジャンプアクションの設定
	SetUpJumpAction(nKey, nCounter, EndKey, EndFrame);
}
//===============================================================================================================
// ジャンプアクションの設定
//===============================================================================================================
void SetUpJumpAction(int nKey,int nCounter,int nLastKey,int EndFrame)
{
	// ジャンプモーションか
	const bool isJumpMotion = g_player.Motion.motiontypeBlend == MOTIONTYPE_JUMPACTION && g_player.Motion.bFirstMotion == false;

	// 刀のジャンプモーションか
	const bool isKatanaMotion = g_player.WeponMotion == MOTION_KATANA && isJumpMotion;

	if (isKatanaMotion && nKey == 0 && nCounter <= 5)
	{
		g_player.move.y = 5.0f;
	}

	// 両手武器のジャンプモーションか
	const bool isDouble = g_player.WeponMotion == MOTION_DBHAND && isJumpMotion;

	if (isDouble && nKey == 0 && nCounter <= 5)
	{
		g_player.move.y = 5.0f;
	}
	else if (isDouble && nKey >= 1 && nKey <= 4)
	{
		g_player.move.y = -MAX_GLABITY * 0.2f;
	}

	// 槍武器武器のジャンプモーションか
	const bool isSpear = g_player.WeponMotion == MOTION_PIERCING && isJumpMotion;

	if (isSpear && nKey >= 0)
	{
		g_player.move.y = -MAX_GLABITY * 0.5f;
	}
	if (isSpear && nKey == 1)
	{
		g_player.move.x = sinf(g_player.rot.y + D3DX_PI) * 20.0f;
		g_player.move.z = cosf(g_player.rot.y + D3DX_PI) * 20.0f;
	}

	// 大型武器のジャンプモーションか
	const bool isBigWepon = g_player.WeponMotion == MOTION_BIGWEPON && isJumpMotion;

	if (isBigWepon && nKey == 0 && nCounter == 1)
	{
		g_player.move.y = 10.0f;
	}
	else if (isBigWepon && nKey == 1 && nCounter == 5)
	{
		g_player.move.y = 10.0f;
	}

	// 大型武器のジャンプモーションか
	const bool isoneHand = g_player.WeponMotion == MOTION_ONE_HAND && isJumpMotion;

	if (isoneHand && nKey <= nLastKey)
	{
		g_player.move.y = -MAX_GLABITY * 0.5f;
	}
}
//===============================================================================================================
// プレイヤーの初期武器の設定
//===============================================================================================================
void SetUpPlayerFirstWepon(void)
{
	Item* pItem = GetItem();
	ITEM_INFO* pItem_Info = GetItemInfo();

	//// アイテムの種類
	//int nType = pItem[g_player.ItemIdx].nType;

	//// 最大の耐久力を代入
	//pItem[g_player.ItemIdx].Maxdurability = pItem_Info[nType].Maxdurability;

	//// 耐久力を代入
	//pItem[g_player.ItemIdx].durability = pItem_Info[nType].durability;

	//// アイテムの属性情報を代入 
	//g_player.nElement = pItem_Info[nType].nElement; 

	// モーションを歩きにする(第2引数に1を入れる)
	MotionChange(MOTION_DBHAND, 1);

	// 素手の時のモーション情報を代入
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
	{
		g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].aModel[nCntModel]; // モデルの情報を代入
	}
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].aMotionInfo[nCntMotion];
	}

	StatusChange(4.0f, D3DXVECTOR3(0.0f, 30.0f, 0.0f), 50);

	// 投げた後に武器を消す
	g_player.Motion.nNumModel -= 1;

	// プレイヤーの状態を何も持っていない状態にする
	g_player.HandState = PLAYERHOLD_NO;

	// モーションをニュートラルにする
	SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, true, 10); // モーションをニュートラルにする
}
//===============================================================================================================
// プレイヤーのフィーバータイムの設定
//===============================================================================================================
void SetFeverTime(void)
{
	// フォーバーモード
	if (g_player.FeverMode == true)
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

	static int FiverCnt = 0; // 回数制限

	if (g_player.FeverMode == true && FiverCnt == 0 && g_player.AttackSp == false)
	{
		SetGameUI(D3DXVECTOR3(620.0f, 360.0f, 0.0f), UITYPE_SYUTYUSEN, 660.0f, 380.0f, false, 0);
		SetGameUI(D3DXVECTOR3(640.0f, 600.0f, 0.0f), UITYPE_FIVER, 100.0f, 40.0f, false, 0);
		FiverCnt = 1; // 制限回数を超えた
	}
	if (g_player.FeverMode == false)
	{
		FiverCnt = 0; // 制限回数をリセット
	}

}
//===============================================================================================================
// ミニマップの合成のアイテムの材料の表示
//==============================================================================================================
void LoadMinimapMatItem(int nHaveIdx, int nStockIdx)
{
	// アイテムの取得
	Item* pItem = GetItem();

	FILE* pFile; // ファイルのポインタ

	// ファイルを開く
	pFile = fopen("data\\ITEM\\SetminimapItem.txt", "r");

	int playeritem = -1;
	int minimapItem = -2;

	// ファイルが開けたら
	if (pFile != NULL)
	{
		char aString[MAX_WORD] = {};
		char skip[10] = {};

		while (1)
		{
			// 文字を読み取る
			int nData = fscanf(pFile, "%s", &aString[0]);

			// PLAYER_ITEMを読み取ったら
			if (strcmp(&aString[0], "PLAYER_ITEM") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// プレイヤーのアイテムを読み取る
				nData = fscanf(pFile, "%d", &playeritem);
			}

			// MINIMAP_ITEMを読み取ったら
			if (strcmp(&aString[0], "MINIMAP_ITEM") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// 表示するアイテムを読み取る
				nData = fscanf(pFile, "%d", &minimapItem);

				// 持っているアイテムが一致していたら
				if (playeritem == pItem[nHaveIdx].nType || playeritem == pItem[nStockIdx].nType)
				{
					playeritem = -1;
					break;
				}
			}

			// EOFを読み取ったら
			if (nData == EOF)
			{
				playeritem = -1;
				minimapItem = -2;

				// ファイルを閉じる
				fclose(pFile);

				// while文を抜ける
				return;
			}
		}

	}
	else
	{
		// メッセージボックスを表示
		MessageBox(NULL, "ファイルが開けません", "LoadMinimapItem", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);

	// アイテム分回す
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// 未使用だったら
		if (pItem[nCntItem].bUse == false) continue;

		// 持っているアイテムと同じだったら
		if (nCntItem == nHaveIdx) continue;

		// ストックしているアイテムと同じだったら
		if (nCntItem == nStockIdx) continue;

		// 表示したいアイテムじゃなかったら
		if (pItem[nCntItem].nType != minimapItem) continue;

		// ミニマップに位置を設定
		pItem[nCntItem].nMinimapIdx = SetMiniMap(pItem[nCntItem].pos, MINIMAPTEX_ITEM);

		// ミニマップに位置を設定
		SetMiniMapPotision(pItem[nCntItem].nMinimapIdx, &pItem[nCntItem].pos);
	}

	// 初期化
	minimapItem = -2;
}
//===============================================================================================================
// プレイヤーが攻撃状態か
//===============================================================================================================
bool isPlayerAttaking(void)
{
	// 最初のブレンドが終わったら
	const bool NotFirstBlend = g_player.Motion.bFirstMotion == false;

	// モーションが終わって無かったら
	const bool NotFinishMotion = g_player.Motion.bFinishMotion == false;

	// モーションタイプ1がアクションだったら
	const bool isAttack1 = g_player.Motion.motionType == MOTIONTYPE_ACTION;

	// モーションタイプ2がアクションだったら
	const bool isAttack2 = g_player.Motion.motionType == MOTIONTYPE_ACTION2;

	// モーションタイプ3がアクションだったら
	const bool isAttack3 = g_player.Motion.motionType == MOTIONTYPE_ACTION3;

	// モーションタイプ4がアクションだったら
	const bool isAttack4 = g_player.Motion.motionType == MOTIONTYPE_ACTION4;

	// モーションがジャンプアクションだったら
	const bool isJumpAttack = g_player.Motion.motionType == MOTIONTYPE_JUMPACTION;

	// 攻撃状態かを判定
	const bool isAttacking = NotFinishMotion && (isAttack1 || isAttack2 || isAttack3 || isAttack4 || isJumpAttack);

	// 判定を返す
	return isAttacking;
}
//===============================================================================================================
// アイテムの変更する時のステータス読み込み
//===============================================================================================================
void LoadItemChange(int nType, float swordLength)
{
	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	FILE* pFile; // ファイルのポインタ

	// ファイルを開く
	pFile = fopen("data\\ITEM\\StatusChange.txt", "r");

	int pType = 0;
	char skip[10] = {};
	int changemotion = 0;
	int playerType = 0;
	float speed = 0.0f;
	int nLoad = 0;

	// ファイルがNULLじゃなかったら
	if (pFile != NULL)
	{
		// 文字読み取り変数
		char aString[MAX_WORD];

		while (1)
		{
			// 文字を読み取る
			int nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "ITEMTYPE") == 0)
			{
				//[=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// 種類を代入
				nData = fscanf(pFile, "%d", &pType);
			}
			
			// 読み取った種類と一致していたら
			if (pType == nType && nLoad != 1)
			{
				nLoad = LoadPlayerParamCharge(&aString[0], pFile, swordLength);
			}
			// EOFを読み取ったら
			if (nData == EOF)
			{
				// while分を抜ける
				break;
			}
		}
	}
}
//===============================================================================================================
// プレイヤーのパラメーター変更
//===============================================================================================================
int LoadPlayerParamCharge(char* aStr, FILE* pFile, float swordLength)
{
	Item* pItem = GetItem();
	Player* pPlayer = GetPlayer();

	int pType = 0;
	char skip[10] = {};
	int changemotion = 0;
	int playerType = 0;
	float speed = 0.0f;
	int nLoad = 0;

	while (1)
	{
		// 文字を読み取る
		int nData = fscanf(pFile, "%s", aStr);

		// CHANGEMOTIONを読み取ったら
		if (strcmp(aStr, "CHANGEMOTION") == 0)
		{
			//[=]を読み飛ばす
			nData = fscanf(pFile, "%s", &skip[0]);

			// 変更するモーションの種類
			nData = fscanf(pFile, "%d", &changemotion);
		}

		// PLAYERTYPEを読み取ったら
		if (strcmp(aStr, "PLAYERTYPE") == 0)
		{
			//[=]を読み飛ばす
			nData = fscanf(pFile, "%s", &skip[0]);

			// 変更するプレイヤーの種類
			nData = fscanf(pFile, "%d", &playerType);
		}

		// SPEEDを読み取ったら
		if (strcmp(aStr, "SPEED") == 0)
		{
			//[=]を読み飛ばす
			nData = fscanf(pFile, "%s", &skip[0]);

			// 歩くスピード
			nData = fscanf(pFile, "%f", &speed);
		}

		// END_PLAYERSETを読み取ったら
		if (strcmp(aStr, "END_PLAYERSET") == 0)
		{
			// モーションを変更する
			MotionChange(changemotion, playerType); 

			// ステータスを変更する
			StatusChange(speed, D3DXVECTOR3(0.0f, swordLength, 0.0f), pItem[pPlayer->ItemIdx].Power);

			// ロード完了を返す
			nLoad = 1;
			break;
		}

	}

	// ロード完了を返す
	return nLoad;
}


