//==============================================================================================================
//
// 敵 [enemy.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "enemy.h"
#include "player.h"
#include <stdio.h>
#include "Score.h"
#include "input.h"
#include "damagepop.h"
#include "block.h"
#include "item.h"
#include "Shadow.h"
#include "Particle.h"
#include "item.h"
#include "HPGauge.h"
#include "sound.h"
#include "spgauge.h"
#include "boss.h"
#include "time.h"
#include "game.h"
#include "Bullet.h"
#include "minimap.h"
#include "effectEdit.h"
#include "effect2.h"
#include "Effect.h"
#include "wall.h"
#include "camera.h"
#include "meshimpact.h"
#include "math.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include <cassert>
#include "count.h"
#include "event.h"
#include "billboard.h"
#include "mark.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define ENEMY_WORD (128)		 // 敵の最大数
#define SWORD_MTX (4)			 // 剣のマトリックスの数
#define TYPEONE_MOVE (2.5f)		 // 敵0の移動量
#define TYPETWO_MOVE (1.5f)		 // 敵1の移動量
#define TYPETHREE_MOVE (1.0f)	 // 敵2の移動量
#define MAX_TEXENEMY (128)		 // テクスチャの最大数
#define MAX_ENEMYMOVE (1.0f)     // 敵の移動量
#define SHADOWSIZEOFFSET (20.0f) // 影のサイズのオフセット
#define SHADOW_A (1.0f)			 // 影のアルファ
#define NUMSPAWN_ENEMY (25)			 // 敵の出現数
#define KICKATTACK_MOVE (15.0f)  // けり攻撃の移動量
#define NUM_SPAWNPOSITION (4)    // スポーン位置の数
#define SETNUM_TERRITORY (2)		// テリトリーを配置する数
#define TERRITORYRADIUS (500.0f)	// テリトリーの半径
#define FRAME (60)				    // フレーム

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void LoadEnemy(int nType);																	// 読み込み処理

int LoadEnemyFilename(FILE* pFile, int nNumModel, char* aString, int nType);				// 敵のモデルのロード処理
void LoadEnemyCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType);			// 敵のパーツの設定処理
void LoadEnemyMotionSet(FILE* pFile, char* aString, int nNumModel, int nType);				// 敵のモーションのロード処理
void LoadEnemyKeySet(FILE* pFile, char* aString, int nType, int nCntMotion);				// 敵のモーションのキーの読み込み処理

//bool AgentRange(float plrange, float playerrange, int nCntEnemy); // ホーミングの範囲にいるかどうか
void AgentEnemy(int nCntEnemy);									    // 敵のホーミング処理
void CollisionToEnemy(int nCntEnemy);							    // 敵と敵の当たり判定
void UpdateHomingEnemy(int nCntEnemy);                              // 敵のホーミング処理
void UpdateRunAwayEnemy(int nCntEnemy);                             // 逃げる敵の更新処理
void UpdateKickAttack(int nCntEnemy);                               // 敵の攻撃の更新処理
void UpdateDroneEnemy(int nCntEnemy);                               // 飛んでる敵の更新処理
void KickActionSet(int nCntEnemy,int nKey, int nCounter, int EndFrame, int LastKey, Player* pPlayer);        // けり攻撃の処理
void UpdateDeathParam(int nCntEnemy);																		 // 敵の死亡モーションの処理
void DeletTerritory(void);																					 // テリトリーの消去
void DecreaseTerritoryEnemy(int nCntEnemy);																	 // テリトリーの敵の減少処理
void NormalizeNumEnemy(void);																				 // 敵の数の正規化
bool KeepInTerritory(int nCntEnemy);																		 // テリトリーの中から出さない処理
void SetSpawnCount(void);																					 // スポーンカウントの設定
void SetTerritoryparam(int nTerritoryIdx,D3DXVECTOR3 pos, int SpawnerPos,bool bSetBoss);					 // テリトリーの設定
void OutTerritorySpawner(int nSpawner);																		 // テリトリーの外のスポナー
void UpdateTargetPosition(int nCntEnemy);																	 // ターゲットの位置の更新
void UpdateTerritoryMark(void);																				 // テリトリーにマークする
void UpdateScoreAndGage(int nCntEnemy);																		 // 敵を倒したときのスコアとゲージの更新処理

//**************************************************************************************************************
//グローバル変数宣言
//**************************************************************************************************************
ENEMY g_Enemy[MAX_ENEMY];		  // 構造体変数
MOTION g_LoadEnemy[ENEMYTYPE_MAX]; // 読み込み
int g_nNumEnemy;				  // 敵の総数カウント
bool g_bSound;
int nCntMotionEnemy, nKeyEnemy;
Territory g_Territory[SETNUM_TERRITORY];
int g_nBossPos = 0;
bool noFirstSetBoss = true;
int g_nNumTerritory = 0;
int g_TerritorySetTime = 0;

//===============================================================================================================
// 敵の初期化処理
//===============================================================================================================
void InitEnemy(void)
{
	// デバイスのポインタを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	nCntMotionEnemy = 0;
	nKeyEnemy = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_bSound = false;
		g_Enemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 座標
		g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 角度
		g_Enemy[nCntEnemy].bUse = false;								// 未使用状態
		g_Enemy[nCntEnemy].Motion.bLoopMotion = true;					// ループするか否か
		g_Enemy[nCntEnemy].nLife = 20;									// 体力
		g_Enemy[nCntEnemy].nMaxLife = 20;								// 最大体力
		g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;					// 状態
		g_Enemy[nCntEnemy].Speed = 0.0f;							    // 足の速さ
		g_Enemy[nCntEnemy].AttackState = ENEMYATTACK_NO;                // 敵が攻撃状態か
		g_Enemy[nCntEnemy].nCountAction = 0;							// 攻撃のインターバル値
		g_Enemy[nCntEnemy].HitStopCount = 0;						    // ヒット時のヒットストップ
		g_Enemy[nCntEnemy].nType = ENEMYTYPE_ONE;
		g_Enemy[nCntEnemy].Motion.motionType = MOTIONTYPE_NEUTRAL;// モーションの種類
		g_Enemy[nCntEnemy].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // サイズ
		g_Enemy[nCntEnemy].nIdxShadow = -1;
		g_Enemy[nCntEnemy].TerritoryNumber = -1;                  // テリトリーの番号
		g_Enemy[nCntEnemy].isKillCount = true;                    // キルカウントをできるかできないか
		g_Enemy[nCntEnemy].nIdxtarget = 0;						  // ターゲットのインデックス
	}

	//グローバル変数の初期化
	g_nNumEnemy = 0; // 敵の数
	g_nBossPos = 0;  // ボスがどのテリトリーにいるか
	noFirstSetBoss = true; // 最初にボスを出さない処理
	g_nNumTerritory = 0; // テリトリーのかず
	g_TerritorySetTime = 0; // テリトリーのカウント

	// テリトリーの数
	for (int nCnt = 0; nCnt < SETNUM_TERRITORY; nCnt++)
	{
		g_Territory[nCnt].CylinderIdx = 0;	// シリンダーのインデックス
		g_Territory[nCnt].nNumber = -1;		// テリトリーが何番目か
		g_Territory[nCnt].nNumEnemy = 0;	// テリトリーの敵の数
		g_Territory[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 位置
		g_Territory[nCnt].bBoss = false;    // ボスが出ているか出てないか
		g_Territory[nCnt].bUse = false;     // 使われているか
	}

	// 敵の種類分
	for (int nCntEnemyType = 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
	{
		// 敵の読み込み
		LoadEnemy(nCntEnemyType);
	}
}
//===============================================================================================================
//敵の終了処理
//===============================================================================================================
void UninitEnemy(void)
{
	// 音楽を止める
	StopSound();

	for (int nCntEnemyType = 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
	{
		for (int nCntModel = 0; nCntModel < g_LoadEnemy[nCntEnemyType].nNumModel; nCntModel++)
		{
			// テクスチャの破棄
			for (int TexCnt = 0; TexCnt < (int)g_LoadEnemy[nCntEnemyType].aModel[nCntModel].dwNumMat; TexCnt++)
			{
				if (g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pTexture[TexCnt] != NULL)
				{
					g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pTexture[TexCnt]->Release();
					g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pTexture[TexCnt] = NULL;
				}
			}

			// メッシュの破棄
			if (g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pMesh != NULL)
			{
				g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pMesh->Release();
				g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pMesh = NULL;
			}

			// マテリアルの破棄
			if (g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pBuffMat != NULL)
			{
				g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pBuffMat->Release();
				g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pBuffMat = NULL;
			}
		}
	}

	// 全敵分回す
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 全モデル分合わす
		for (int nCntModel = 0; nCntModel < g_Enemy[nCntEnemy].Motion.nNumModel; nCntModel++)
		{
			// テクスチャの最大数分回す
			for (int TexCnt = 0; TexCnt < (int)g_Enemy[nCntEnemy].Motion.aModel[nCntModel].dwNumMat; TexCnt++)
			{
				if (g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pTexture[TexCnt] != NULL)
				{
					g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pTexture[TexCnt] = NULL;
				}
			}

			// メッシュの破棄
			if (g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pMesh != NULL)
			{
				g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pMesh = NULL;
			}

			// マテリアルの破棄
			if (g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pBuffMat != NULL)
			{
				g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pBuffMat = NULL;
			}
		}
	}
}
//===============================================================================================================
//敵の更新処理
//===============================================================================================================
void UpdateEnemy(void)
{
	GAMESTATE gameState = GetGameState();
	ITEM_INFO* ItemInfo = GetItemInfo();

	g_bSound = false;
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();
	Camera* pCamera = GetCamera();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// ヒットストップのカウントを減らす
		g_Enemy[nCntEnemy].HitStopCount--;

		// ヒットストップしている
		if (g_Enemy[nCntEnemy].HitStopCount > 0)
		{
			g_Enemy[nCntEnemy].pos.x += (float)(rand() % 30 - 15.0f);
			g_Enemy[nCntEnemy].pos.z += (float)(rand() % 30 - 15.0f);
		}
		if (g_Enemy[nCntEnemy].bUse == false || g_Enemy[nCntEnemy].HitStopCount >= 0)
		{// 未使用状態だったら
			// とばしてカウントを進める
			continue;
		}
		
		// 最大体力を超えてたら調整
		if (g_Enemy[nCntEnemy].nMaxLife < g_Enemy[nCntEnemy].nLife)
		{
			g_Enemy[nCntEnemy].nLife = g_Enemy[nCntEnemy].nMaxLife;
		}

		// 敵の状態
		switch (g_Enemy[nCntEnemy].state)
		{
		case ENEMYSTATE_NORMAL:
			break;
		case ENEMYSTATE_AGENT:
			break;
		case ENEMYSTATE_ATTACK:
			g_Enemy[nCntEnemy].nCounterState--;

			if (g_Enemy[nCntEnemy].nCounterState <= 0)
			{
				g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			}
			break;
		case ENEMYSTATE_DAMAGE:
			g_Enemy[nCntEnemy].nCounterState--;

			if (g_Enemy[nCntEnemy].nCounterState <= 0)
			{
				g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			}
			break;
		case ENEMYSTATE_DEATH:
			break;
		default:
			break;
		}

		// 移動量の減衰
		g_Enemy[nCntEnemy].move.x += (0.0f - g_Enemy[nCntEnemy].move.x) * 0.5f;
		g_Enemy[nCntEnemy].move.z += (0.0f - g_Enemy[nCntEnemy].move.z) * 0.5f;

		// 前回の位置を保存
		g_Enemy[nCntEnemy].posOld = g_Enemy[nCntEnemy].pos;

		// 位置の更新
		g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

		if (g_Enemy[nCntEnemy].nType == ENEMYTYPE_SIX)
		{
			g_Enemy[nCntEnemy].pos.y = 170.0f;
		}

		CollisionField(&g_Enemy[nCntEnemy].pos,&g_Enemy[nCntEnemy].posOld);

		g_Enemy[nCntEnemy].move.y -= MAX_GLABITY;

		// 6番目の敵以外 && 7番目の敵以外
		if (g_Enemy[nCntEnemy].nType != ENEMYTYPE_SIX && g_Enemy[nCntEnemy].nType != ENEMYTYPE_SEVEN)
		{
			//モーションの更新
			UpdateMotion(&g_Enemy[nCntEnemy].Motion);
		}

		// 影の計算
		SetPositionShadow(g_Enemy[nCntEnemy].nIdxShadow, g_Enemy[nCntEnemy].pos, SHADOWSIZEOFFSET + SHADOWSIZEOFFSET * g_Enemy[nCntEnemy].pos.y / 200.0f, SHADOW_A / (SHADOW_A + g_Enemy[nCntEnemy].pos.y / 30.0f));
		SetMiniMapPotision(g_Enemy[nCntEnemy].nIdxMap, &g_Enemy[nCntEnemy].pos);

		// モーションの種類が死亡
		if (g_Enemy[nCntEnemy].Motion.motiontypeBlend == MOTIONTYPE_DEATH)
		{
			// 死亡モーションの更新処理
			UpdateDeathParam(nCntEnemy);

			// 処理を飛ばす
			continue;
		}

		// ゲームの状態がムービーだったら
		if (gameState == GAMESTATE_MOVIE)
		{
			continue;
		}

		// 壁との当たり判定
		CollisionWall(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld, &g_Enemy[nCntEnemy].move, g_Enemy[nCntEnemy].Speed);

		if(CollisionImpact(&g_Enemy[nCntEnemy].pos) == true)
		{
			HitEnemy(nCntEnemy, ImpactDamege(0));
		}

		// アイテムが当たったか
		if (HitThrowItem(&g_Enemy[nCntEnemy].pos, 10.0f, 40.0f) && g_Enemy[nCntEnemy].state != ENEMYSTATE_DAMAGE)
		{
			HitEnemy(nCntEnemy, pPlayer->nDamage * (int)1.5f);
		}

		// 剣と敵の当たり判定
		HitSowrd(&g_Enemy[nCntEnemy], nCntEnemy);

		// 敵をシリンダーの中にとどめる処理
		KeepInTerritory(nCntEnemy);

		// ホーミングの更新処理
		UpdateHomingEnemy(nCntEnemy);

		// 飛んでる敵の更新処理
		UpdateDroneEnemy(nCntEnemy);

		// 敵と敵の当たり判定
		CollisionToEnemy(nCntEnemy);
		
		// 攻撃範囲に入った
		if (sphererange(&pPlayer->pos, &g_Enemy[nCntEnemy].pos, 20.0f, 20.0f) && g_Enemy[nCntEnemy].Motion.motionType != MOTIONTYPE_ACTION)
		{
			g_Enemy[nCntEnemy].AttackState = ENEMYATTACK_ATTACK;
			SetMotion(&g_Enemy[nCntEnemy].Motion,MOTIONTYPE_ACTION, true, 20); // モーションをニュートラルにする

			SetParticle(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + g_Enemy[nCntEnemy].Size.y / 1.5f, g_Enemy[nCntEnemy].pos.z),
				D3DXVECTOR3(1.57f, g_Enemy[nCntEnemy].rot.y, 1.57f),
				D3DXVECTOR3(0.2f, 3.14f, 0.2f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
				2.0f, 3, 12, 10, 2.0f, 20.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}

		// 攻撃状態だったら動きを止める
		if (g_Enemy[nCntEnemy].Motion.motionType == MOTIONTYPE_ACTION)
		{
			g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		// モーションが最後まで行ったら攻撃状態をリセット
		if (g_Enemy[nCntEnemy].Motion.bFinishMotion == true)
		{
			g_Enemy[nCntEnemy].AttackState = ENEMYATTACK_NO;					// ボスの攻撃状態を攻撃してない状態にする
		}

		if (g_Enemy[nCntEnemy].nType == ENEMYTYPE_ONE || g_Enemy[nCntEnemy].nType == ENEMYTYPE_TWO || g_Enemy[nCntEnemy].nType == ENEMYTYPE_THREE)
		{
			// 敵の攻撃の更新処理
			UpdateKickAttack(nCntEnemy);
		}

		// ターゲットの位置の更新
		UpdateTargetPosition(nCntEnemy);

		// 敵の角度の正規化
		if (g_Enemy[nCntEnemy].rotDest.y - g_Enemy[nCntEnemy].rot.y >= D3DX_PI)
		{
			g_Enemy[nCntEnemy].rot.y += D3DX_PI * 2.0f;
		}
		else if (g_Enemy[nCntEnemy].rotDest.y - g_Enemy[nCntEnemy].rot.y <= -D3DX_PI)
		{
			g_Enemy[nCntEnemy].rot.y -= D3DX_PI * 2.0f;
		}

		// 目的の角度に近づける
		g_Enemy[nCntEnemy].rot.y += (g_Enemy[nCntEnemy].rotDest.y - g_Enemy[nCntEnemy].rot.y) * 0.1f;
	}

	// テリトリーに向ける矢印
	UpdateTerritoryMark();

	// テリトリーの消去
	DeletTerritory();

	// テリトリーの設定
	SetSpawnCount();
}
//===============================================================================================================
//敵の描画処理
//===============================================================================================================
void DrawEnemy(void)
{
	// プレイヤーを取得
	Player* pPlayer = GetPlayer();

	// デバイスのポインタを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef; // 現在のマテリアル保存用
	
	D3DXMATERIAL* pMat;  // マテリアルデータへのポインタ

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_Enemy[nCntEnemy].bUse == false)
		{// 未使用だったら
			// 処理を読み飛ばす
			continue;
		}

		// 種類を代入
		int nType = g_Enemy[nCntEnemy].nType;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Enemy[nCntEnemy].mtxWorldEnemy);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[nCntEnemy].rot.y, g_Enemy[nCntEnemy].rot.x, g_Enemy[nCntEnemy].rot.z);
		D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y, g_Enemy[nCntEnemy].pos.z);
		D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].mtxWorldEnemy);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// 全モデルパーツの描画
		for (int nCntModel = 0; nCntModel < g_Enemy[nCntEnemy].Motion.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransform; // 計算用
			D3DXMATRIX mtxParent;				  // 親のマトリックス

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.y, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.x, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTransform, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.x, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.y, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

			// パーツの[親のマトリックス]を設定
			if (g_Enemy[nCntEnemy].Motion.aModel[nCntModel].nIdxModelParent != -1)
			{
				// 親モデルがある場合
				mtxParent = g_Enemy[nCntEnemy].Motion.aModel[g_Enemy[nCntEnemy].Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{// 親モデルがない場合
				mtxParent = g_Enemy[nCntEnemy].mtxWorldEnemy;
			}

			// 算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld,
				&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld,
				&mtxParent);//自分自分親

			// パーツのワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD,
				&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld);

			// マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCntEnemy].Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				// カラー変更用の変数
				D3DXMATERIAL color;

				if (g_Enemy[nCntEnemy].state != ENEMYSTATE_DAMAGE)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				else if (g_Enemy[nCntEnemy].state == ENEMYSTATE_DAMAGE)
				{
					// 代入
					color = pMat[nCntMat];

					// カラーを設定
					color.MatD3D.Diffuse.r = 1.0f;
					color.MatD3D.Diffuse.g = 0.0f;
					color.MatD3D.Diffuse.b = 0.0f;
					color.MatD3D.Diffuse.a = 1.0f;

					// マテリアルの設定
					pDevice->SetMaterial(&color.MatD3D);
				}

				// テクスチャの設定
				pDevice->SetTexture(0, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pTexture[nCntMat]);

				// ブロック(パーツ)の描画
				g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}
		}	
	}
}
//=========================================================================================================
//敵の取得処理
//=========================================================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}
//=========================================================================================================
//敵のヒット処理
//=========================================================================================================
void HitEnemy(int nCnt,int nDamage)
{
	// アイテム取得
	Item* pItem = GetItem();

	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	GAMESTATE gamestate = GetGameState();

	g_Enemy[nCnt].nLife -= nDamage;

	// パーティクルをセット
	SetParticle(D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y / 1.5f, g_Enemy[nCnt].pos.z),
		g_Enemy[nCnt].rot,
		D3DXVECTOR3(3.14f, 3.14f, 3.14f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f),
		4.0f, 1, 20, 10, 8.0f, 0.0f,
		false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	int nMin = GetTimeMinute();
	int nSec = GetTimeSecond();

	if (g_Enemy[nCnt].nLife <= 0)
	{// 体力が0以下なら

		// キルカウントをできる
		if (g_Enemy[nCnt].isKillCount == true)
		{
			// ビルボードの消去
			DeletIdxBillboard(g_Enemy[nCnt].nIdxtarget);
			
			// ランダム
			int randum_spawner = rand() % 4;

			// スポナーから敵を出す
			OutTerritorySpawner(randum_spawner);

			// ダメージを設定
			SetDamege(D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y / 1.5f, g_Enemy[nCnt].pos.z), // 位置
				nDamage,	// ダメージ																								
				20,			// 寿命
				false);

			// 敵の数を減らす
			g_nNumEnemy--;

			// 死亡モーションが無い敵
			if (g_Enemy[nCnt].nType == ENEMYTYPE_SIX || g_Enemy[nCnt].nType == ENEMYTYPE_SEVEN)
			{
				g_Enemy[nCnt].bUse = false;

				// 敵の影を消す
				KillShadow(g_Enemy[nCnt].nIdxShadow);

				// マップから消す
				EnableMap(g_Enemy[nCnt].nIdxMap);

				// テリトリーの敵の減少処理
				DecreaseTerritoryEnemy(nCnt);
			}

			// 闇属性だったら
			if (pPlayer->nElement == WEPONELEMENT_DARK)
			{
				LoadEffect(1, D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + 50.0f, g_Enemy[nCnt].pos.z));
			}
			else
			{
				SetParticle(D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y / 1.5f, g_Enemy[nCnt].pos.z),
					g_Enemy[nCnt].rot,
					D3DXVECTOR3(3.14f, 3.14f, 3.14f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
					4.0f, 8, 15, 20, 5.0f, 0.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}

			// 敵を倒したときのスコアとゲージの更新処理
			UpdateScoreAndGage(nCnt);
		}

		// モーションの種類が死亡じゃなかったら
		if (g_Enemy[nCnt].Motion.motiontypeBlend != MOTIONTYPE_DEATH)
		{
			SetMotion(&g_Enemy[nCnt].Motion, MOTIONTYPE_DEATH, true, 10);
		}

		g_Enemy[nCnt].state = ENEMYSTATE_DEATH; // 敵の状態を死亡状態にする
		//KillShadow(g_Enemy[nCnt].nIdxShadow);   // 敵の影を消す
		//EnableMap(g_Enemy[nCnt].nIdxMap);		// マップから消す

		if (g_bSound == false) // もしそのフレーム中一度も音が鳴らされてないなら鳴らす
		{
			// 合成じゃない武器のサウンドを設定する
			SetSoundWepon(pItem[pPlayer->ItemIdx].nType);

			// 合成武器のサウンドを設定する
			SetCreateWeponSound(pItem[pPlayer->ItemIdx].nType);

			// ヒットストップの設定
			SetUpHitStop(&g_Enemy[nCnt].HitStopCount);

			g_bSound = true;
		}

		// カウントを止める
		g_Enemy[nCnt].isKillCount = false;

		// 左の振動の強さ
		float leftmotor = pPlayer->AttackSp ? 45000 : 10000;

		// 右の振動の強さ
		float rightmotor = pPlayer->AttackSp ? 45000 : 10000;

		SetVibration(leftmotor, rightmotor, 500);

		return;
	}
	else
	{
		// 左の振動の強さ
		float leftmotor = pPlayer->AttackSp ? 45000 : 10000;

		// 右の振動の強さ
		float rightmotor = pPlayer->AttackSp ? 45000 : 10000;

		SetVibration(leftmotor, rightmotor, 1000);
		if (g_Enemy[nCnt].state != ENEMYSTATE_DAMAGE)
		{
			// ヒットストップの設定
			SetUpHitStop(&g_Enemy[nCnt].HitStopCount);
		}

		if (g_bSound == false) // もしそのフレーム中一度も音が鳴らされてないなら鳴らす
		{
			// サウンドを設定する
			SetSoundWepon(pItem[pPlayer->ItemIdx].nType);

			// 合成武器のサウンドを設定する
			SetCreateWeponSound(pItem[pPlayer->ItemIdx].nType);

			g_bSound = true;
		}

		if (g_Enemy[nCnt].nLife > 0)
		{
			g_Enemy[nCnt].state = ENEMYSTATE_DAMAGE; // 敵の状態をダメージにする
		}

		g_Enemy[nCnt].nCounterState = 20;		 // ダメージ状態からノーマルに戻るまでの時間

		// スコアを加算
		if (gamestate != GAMESTATE_END)
		{
			AddScore(4300);
		}

		return;
	}
}
//=========================================================================================================
//敵の設定処理
//=========================================================================================================
void SetEnemy(D3DXVECTOR3 pos,int nType,int nLife,float Speed,int TerritoryNumber)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_Enemy[nCntEnemy].bUse == false)
		{
			g_Enemy[nCntEnemy].Motion = g_LoadEnemy[nType]; // 情報を代入

			g_Enemy[nCntEnemy].pos = pos;					// 座標
			g_Enemy[nCntEnemy].nType = nType;				// 種類
			g_Enemy[nCntEnemy].nLife = nLife;				// 体力
			g_Enemy[nCntEnemy].nMaxLife = nLife;			// 最大体力
			g_Enemy[nCntEnemy].Speed = Speed;				// 足の速さ
			g_Enemy[nCntEnemy].nCountAction = 0;			// カウンターアクションを初期化
			g_Enemy[nCntEnemy].TerritoryNumber = TerritoryNumber;	// テリトリーの番号
			g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;			// 敵の状態をリセット
			g_Enemy[nCntEnemy].nCounterState = 0;					// 敵の状態カウンターをリセット
			g_Enemy[nCntEnemy].bUse = true;							// 使用状態
			g_Enemy[nCntEnemy].isKillCount = true;					// キルカウントをリセット

			g_Enemy[nCntEnemy].rotDest.y = (float)(rand()% 628 - 314) * 0.01f;	  // 角度

			// テリトリーの敵だったら
			if (TerritoryNumber != -1)
			{
				// ビルボードをセット
				g_Enemy[nCntEnemy].nIdxtarget = SetBillboard(g_Enemy[nCntEnemy].pos,BILLBOARDTYPE_TARGET,5.0f,5.0f,BILLBOARDSTATE_SET);
			}

			if (nType == ENEMYTYPE_SIX)
			{
				g_Enemy[nCntEnemy].nLife = 1;
			}

			g_Enemy[nCntEnemy].nIdxMap = SetMiniMap(pos,MINIMAPTEX_ENEMY);

			// 影を設定
			g_Enemy[nCntEnemy].nIdxShadow = SetShadow(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, 1.0f, g_Enemy[nCntEnemy].pos.z), D3DXVECTOR3(0.0f,0.0f,0.0f), 40.0f, 1.0f);

			g_nNumEnemy++; // インクリメント
			break;
		}
	}
}
//=========================================================================================================
//敵の出現
//=========================================================================================================
void SpawnEnemy(int nSpawner,int TerritoryIdx)
{
	for (int nCnt = 0; nCnt < NUMSPAWN_ENEMY; nCnt++)
	{
		// スポナー0
		if (nSpawner == 0)
		{
			// 位置Xを求める
			float fPosX = TERRITTORYPOS_ONE.x + (float)(rand() % 10);

			// 位置Zを求める
			float fPosZ = TERRITTORYPOS_ONE.z + (float)(rand() % 10);

			// 位置を代入
			D3DXVECTOR3 pos(fPosX, 0.0f, fPosZ);

			// 敵をセット
			SetEnemy(pos, rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f), TerritoryIdx);

			// テリトリーの敵の数を加算
			g_Territory[TerritoryIdx].nNumEnemy++;
		}
		// スポナー1
		else if (nSpawner == 1)
		{
			// 位置Xを求める
			float fPosX = TERRITTORYPOS_TWO.x + (float)(rand() % 10);

			// 位置Zを求める
			float fPosZ = TERRITTORYPOS_TWO.z + (float)(rand() % 10);

			// 位置を代入
			D3DXVECTOR3 pos(fPosX, 0.0f, fPosZ);

			SetEnemy(pos, rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f), TerritoryIdx);

			// テリトリーの敵の数を加算
			g_Territory[TerritoryIdx].nNumEnemy++;
		}
		// スポナー2
		else if (nSpawner == 2)
		{
			// 位置Xを求める
			float fPosX = TERRITTORYPOS_THREE.x + (float)(rand() % 10);

			// 位置Zを求める
			float fPosZ = TERRITTORYPOS_THREE.z + (float)(rand() % 10);

			// 位置を代入
			D3DXVECTOR3 pos(fPosX, 0.0f, fPosZ);

			SetEnemy(pos, rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f), TerritoryIdx);

			// テリトリーの敵の数を加算
			g_Territory[TerritoryIdx].nNumEnemy++;
		}
		else if (nSpawner == 3)
		{
			// 位置Xを求める
			float fPosX = TERRITTORYPOS_FOUR.x + (float)(rand() % 10);

			// 位置Zを求める
			float fPosZ = TERRITTORYPOS_FOUR.z + (float)(rand() % 10);

			// 位置を代入
			D3DXVECTOR3 pos(fPosX, 0.0f, fPosZ);

			SetEnemy(pos, rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f), TerritoryIdx);

			// テリトリーの敵の数を加算
			g_Territory[TerritoryIdx].nNumEnemy++;
		}
	}
}
//=========================================================================================================
//敵の総数取得処理
//=========================================================================================================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}
//=========================================================================================================
// 敵の視界の判定
//=========================================================================================================
bool CollisionView(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, float fRange, float viewAngle)
{
	// 判定用変数
	bool bCollision = false;

	// プレイヤーを取得
	Player* pPlayer = GetPlayer();

	// 視界の範囲のベクトル
	D3DXVECTOR3 ViewVectorL = {};
	D3DXVECTOR3 ViewVectorR = {};

	// 外積
	D3DXVECTOR3 ViewCrossL = {};
	D3DXVECTOR3 ViewCrossR = {};

	// 左側の視界を作成
	ViewVectorL.x = (pPos->x + sinf(pRot->y + D3DX_PI * viewAngle) * fRange) - pPos->x;
	ViewVectorL.y = 0.0f;
	ViewVectorL.z = (pPos->z + cosf(pRot->y + D3DX_PI * viewAngle) * fRange) - pPos->z;

	// 右側の視界を作成
	ViewVectorR.x = (pPos->x + sinf(pRot->y - D3DX_PI * viewAngle) * fRange) - pPos->x;
	ViewVectorR.y = 0.0f;
	ViewVectorR.z = (pPos->z + cosf(pRot->y - D3DX_PI * viewAngle) * fRange) - pPos->z;

	// プレイヤーまでのベクトル
	D3DXVECTOR3 PlayerVec = pPlayer->pos - *pPos;

	// プレイヤーから敵までのベクトルとの外積
	D3DXVec3Cross(&ViewCrossL, &ViewVectorL, &PlayerVec);
	D3DXVec3Cross(&ViewCrossR, &ViewVectorR, &PlayerVec);

	// 正規化する
	D3DXVec3Normalize(&ViewCrossL, &ViewCrossL);
	D3DXVec3Normalize(&ViewCrossR, &ViewCrossR);


	//D3DXVECTOR3 pos(pPos->x + sinf(pRot->y + D3DX_PI * viewAngle) * fRange,0.0f, pPos->z + cosf(pRot->y + D3DX_PI * viewAngle) * fRange);
	//D3DXVECTOR3 pos2(pPos->x + sinf(pRot->y - D3DX_PI * viewAngle) * fRange, 0.0f, pPos->z + cosf(pRot->y - D3DX_PI * viewAngle) * fRange);

	//SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10.0f, 0.02f, 0);
	//SetEffect(pos2, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10.0f, 0.02f, 0);

	// 視界の範囲内だったら
	if (ViewCrossL.y > 0.0f && ViewCrossR.y < 0.0f)
	{
		// 視界内にいる
		bCollision = true;
	}

	// 判定を返す
	return bCollision;
}
//===============================================================================================================
// 敵の角度の設定
//===============================================================================================================
float SetAngle(D3DXVECTOR3* pRot, D3DXVECTOR3* pPos)
{
	Player* pPlayer = GetPlayer();

	float fAngle = atan2f(pPlayer->pos.x - pPos->x, pPlayer->pos.z - pPos->z); // 敵からプレイやまでの角度を求める

	// 角度を返す
	return fAngle;
}
//===============================================================================================================
//敵のロード処理
//===============================================================================================================
void LoadEnemy(int nType)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ローカル変数
	int nModel = 0;					// モデル数
	int nIdx = 0;					// インデックス
	int NumKey = 0;					// キー数
	int nCnt = 0;					// モデルカウント用
	int nNumParts = 0;				// パーツ数格納用
	int nCntKey = 0;				// キーカウント用
	int nCntPosKey = 0;				// posカウント
	int nCntRotkey = 0;				// rotカウント
	int nScanData = 0;              // 戻り値代入用

	// ファイルポインタを宣言
	FILE* pFile;

	switch (nType)
	{
	case 0:
		// ファイルを開く
		pFile = fopen("data/MOTION/motionEnemy01.txt", "r");
		break;
	case 1:
		pFile = fopen("data/MOTION/motionEnemy02.txt", "r");
		break;
	case 2:
		pFile = fopen("data/MOTION/motionEnemy03.txt", "r");
		break;
	case 3:
		pFile = fopen("data/MOTION/motionEnemy04.txt", "r");
		break;
	case 4:
		pFile = fopen("data/MOTION/motionEnemy05.txt", "r");
		break;
	case 5:
		pFile = NULL; //NULLにする
		g_LoadEnemy[nType].nNumModel = 1;
		g_LoadEnemy[nType].aModel[0].nIdxModelParent = -1;


		//Xファイルの読み込み
		D3DXLoadMeshFromX("data\\MODEL\\enemy600\\subenemy000_drone.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_LoadEnemy[nType].aModel[0].pBuffMat,
			NULL,
			&g_LoadEnemy[nType].aModel[0].dwNumMat,
			&g_LoadEnemy[nType].aModel[0].pMesh);
		break;
	case 6:
		pFile = NULL; //NULLにする
		g_LoadEnemy[nType].nNumModel = 1;
		g_LoadEnemy[nType].aModel[0].nIdxModelParent = -1;

		//Xファイルの読み込み
		D3DXLoadMeshFromX("data\\MODEL\\enemy700\\subenemy001_escape.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_LoadEnemy[nType].aModel[0].pBuffMat,
			NULL,
			&g_LoadEnemy[nType].aModel[0].dwNumMat,
			&g_LoadEnemy[nType].aModel[0].pMesh);
		break;
	default:
		pFile = NULL; //NULLにする
		break;
	}

	char Skip[3] = {};
	int nNumModel = 0;
	int nLoadCnt = 0;

	if (pFile != NULL)
	{//　NULL じゃない
		char aStr[ENEMY_WORD];

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
				g_LoadEnemy[nType].nNumModel = nNumModel;	// モデルの最大数を代入
			}

			// モデルの読み込みがまだ終わっていなかったら
			if (nLoadCnt < nNumModel)
			{
				// モデルの読み込んだ数を返す
				nLoadCnt = LoadEnemyFilename(pFile, nNumModel, &aStr[0], nType);
			}

			// CHARACTERSETを読み取ったら
			if (strcmp(&aStr[0], "CHARACTERSET") == 0)
			{
				// パーツの設定処理
				LoadEnemyCharacterSet(pFile, &aStr[0], nNumParts, nType);
			}

			// MOTIONSETを読み取ったら
			if (strcmp(&aStr[0], "MOTIONSET") == 0)
			{
				// モーションの設定処理
				LoadEnemyMotionSet(pFile, &aStr[0], nNumModel, nType);
			}

			// END_SCRIPTを読み取ったら
			if (nData == EOF)
			{
				nCntMotionEnemy = 0; // モーションのカウントをリセットする
				break;          // While文を抜ける
			}
		}// while文末
	}
	else
	{
		if (nType != ENEMYTYPE_SIX && nType != ENEMYTYPE_SEVEN)
		{
			//メッセージボックス
			MessageBox(NULL, "ファイルが開けません。", "エラー(enemy.cpp)", MB_OK);
		}
		return;
    }
	// ファイルを閉じる
	fclose(pFile);

}
//===============================================================================================================
// 敵のファイルネームのロード処理
//===============================================================================================================
int LoadEnemyFilename(FILE* pFile, int nNumModel, char* aString, int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL* pMat;//マテリアルへのポインタ

	//頂点座標の取得
	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマットのサイズ
	BYTE* pVtxBuff;	//頂点バッファへのポインタ

	char Skip[3] = {}; // [=]読み飛ばしよう変数

	int nCntLoadModel = 0; // モデルのロードのカウンター
	int nScanData = 0;              // 戻り値代入用

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
				&g_LoadEnemy[nType].aModel[nCntLoadModel].pBuffMat,
				NULL,
				&g_LoadEnemy[nType].aModel[nCntLoadModel].dwNumMat,
				&g_LoadEnemy[nType].aModel[nCntLoadModel].pMesh);

			//マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_LoadEnemy[nType].aModel[nCntLoadModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadEnemy[nType].aModel[nCntLoadModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//このファイル名を使用してテクスチャを読み込む
						//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_LoadEnemy[nType].aModel[nCntLoadModel].pTexture[nCntMat]);
				}
			}

			//頂点数の取得
			nNumVtx = g_LoadEnemy[nType].aModel[nCntLoadModel].pMesh->GetNumVertices();

			//頂点フォーマットのサイズ取得
			sizeFVF = D3DXGetFVFVertexSize(g_LoadEnemy[nType].aModel[nCntLoadModel].pMesh->GetFVF());

			//頂点バッファのロック
			g_LoadEnemy[nType].aModel[nCntLoadModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//頂点座標を比較してブロックの最小値,最大値を取得
				if (vtx.x < g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.x)
				{
					g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.y)
				{
					g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.z)
				{
					g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.x)
				{
					g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.y)
				{
					g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.z)
				{
					g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			////サイズを代入
			//g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			//g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			//g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

			//各モデルごとのサイズを代入
			g_LoadEnemy[nType].aModel[nCntLoadModel].Size.x = g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.x - g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.x;
			g_LoadEnemy[nType].aModel[nCntLoadModel].Size.y = g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.y - g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.y;
			g_LoadEnemy[nType].aModel[nCntLoadModel].Size.z = g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.z - g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.z;

			//頂点バッファのアンロック
			g_LoadEnemy[nType].aModel[nCntLoadModel].pMesh->UnlockVertexBuffer();

			nCntLoadModel++; // モデルのカウントを増やす
		}
	}
	return nCntLoadModel; // モデルのカウントを返す
}
//===============================================================================================================
// 敵のキャラクター設定処理
//===============================================================================================================
void LoadEnemyCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType)
{
	int nIdx = 0; // インデックス格納変数
	char Skip[3] = {}; // [=]読み飛ばし変数
	int nScanData = 0;          // 戻り値代入用

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
			nScanData = fscanf(pFile, "%d", &g_LoadEnemy[nType].aModel[nIdx].nIdxModelParent);
		}
		// POSを読み取ったら
		else if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aModel[nIdx].offpos.x);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aModel[nIdx].offpos.y);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aModel[nIdx].offpos.z);
		}
		// ROTを読み取ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aModel[nIdx].offrot.x);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aModel[nIdx].offrot.y);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aModel[nIdx].offrot.z);
		}
	}
}
//===============================================================================================================
// 敵のモーションのロード処理
//===============================================================================================================
void LoadEnemyMotionSet(FILE* pFile, char* aString, int nNumModel, int nType)
{
	char Skip[3] = {}; // [=]読み飛ばし変数
	int nScanData = 0;          // 戻り値代入用

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
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ループするかしないか
			nScanData = fscanf(pFile, "%d", &g_LoadEnemy[nType].aMotionInfo[nCntMotionEnemy].bLoop);
		}
		// NUM_KEYを読み通ったら
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// キーの最大数を代入
			nScanData = fscanf(pFile, "%d", &g_LoadEnemy[nType].aMotionInfo[nCntMotionEnemy].nNumkey);
		}
		// KEYSETを読み通ったら
		if (strcmp(aString, "KEYSET") == 0)
		{
			// キーの設定処理
			LoadEnemyKeySet(pFile, aString, nType, nCntMotionEnemy);
		}
		// END_MOTIONSETを読み通ったら
		if (strcmp(aString, "END_MOTIONSET") == 0)
		{
			nCntMotionEnemy++; // モーションのカウントをリセット
			nKeyEnemy = 0;     // キーをリセット
			break;
		}
	}

}
//===============================================================================================================
// 敵のモーションのキー処理
//===============================================================================================================
void LoadEnemyKeySet(FILE* pFile, char* aString, int nType, int nCntMotion)
{
	char Skip[3] = {}; // [=]読み飛ばし変数
	int nCntPos = 0;   // 位置のカウント
	int nCntRot = 0;   // 角度のカウント
	int nScanData = 0;          // 戻り値代入用

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
			nScanData = fscanf(pFile, "%d", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].nFrame);
		}

		// POSを読み通ったら
		if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// 位置を読み込む
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntPos].fPosX);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntPos].fPosY);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROTを読み通ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// 角度を読み込む
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntRot].fRotX);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntRot].fRotY);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntRot].fRotZ);
			nCntRot++;
		}
		// END_KEYSETを読み通ったら
		else if (strcmp(aString, "END_KEYSET") == 0)
		{
			nKeyEnemy++;		 // キーを増やす
			nCntPos = 0; // 位置のカウントをリセット
			nCntRot = 0; // 角度のカウントをリセット
			break;
		}
	}
}

//===============================================================================================================
// 敵のホーミング処理
//===============================================================================================================
void AgentEnemy(int nCntEnemy)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	float fRotDest, fRotDiff;

	fRotDest = atan2f(g_Enemy[nCntEnemy].pos.x - pPlayer->pos.x, g_Enemy[nCntEnemy].pos.z - pPlayer->pos.z);//目標の移動方向(角度)

	fRotDiff = fRotDest - g_Enemy[nCntEnemy].rot.y;//目標の移動方向(角度)

	g_Enemy[nCntEnemy].move.x += sinf(g_Enemy[nCntEnemy].rot.y) * -g_Enemy[nCntEnemy].Speed;

	g_Enemy[nCntEnemy].move.z += cosf(g_Enemy[nCntEnemy].rot.y) * -g_Enemy[nCntEnemy].Speed;
}
//===============================================================================================================
// 敵と敵の当たり判定
//===============================================================================================================
void CollisionToEnemy(int nCntEnemy)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_Enemy[nCnt].bUse && nCnt != nCntEnemy)
		{
			float fDistanceX = g_Enemy[nCntEnemy].pos.x - g_Enemy[nCnt].pos.x; // 距離Xを求める
			float fDistanceY = g_Enemy[nCntEnemy].pos.y - g_Enemy[nCnt].pos.y; // 距離Yを求める
			float fDistanceZ = g_Enemy[nCntEnemy].pos.z - g_Enemy[nCnt].pos.z; // 距離Zを求める

			// 距離を求める
			float fDistance = (fDistanceX * fDistanceX) + (fDistanceY * fDistanceY) + (fDistanceZ * fDistanceZ);

			// 半径を設定
			float Eradius = 15.0f; 

			// ホーミングしてくる半径
			float Radius = Eradius + Eradius;

			// 半径を算出
			Radius = Radius * Radius;

			//範囲内に入った
			if (fDistance <= Radius)
			{
				D3DXVECTOR3 vector = g_Enemy[nCntEnemy].pos - g_Enemy[nCnt].pos; // はじく方向ベクトル
				D3DXVec3Normalize(&vector, &vector);							 // 正規化

				// 移動量の設定
				g_Enemy[nCnt].move.x -= vector.x * g_Enemy[nCntEnemy].Speed;
				g_Enemy[nCnt].move.z -= vector.z * g_Enemy[nCntEnemy].Speed;
			}
		}
	}
}
//===============================================================================================================
// 敵の追跡処理の更新処理
//===============================================================================================================
void UpdateHomingEnemy(int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	// 絶対に追跡する範囲にいるかを判定
	const bool is_absolute = sphererange(&pPlayer->pos, &g_Enemy[nCntEnemy].pos, 50.0f, 100.0f) == true;

	// 追跡する範囲にいるか判定
	const bool is_sphereBounds = sphererange(&pPlayer->pos, &g_Enemy[nCntEnemy].pos, 50.0f, 300.0f) == true;

	// 視界内にいるかを判定
	const bool is_ViewBounds = CollisionView(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].rot, 300.0f, 0.75f) == true;

	// 攻撃状態かを判定
	const bool is_NotAction = g_Enemy[nCntEnemy].Motion.motionType != MOTIONTYPE_ACTION;

	// 敵の種類が7番目じゃないかを判定
	const bool is_NotTypeSeven = g_Enemy[nCntEnemy].nType != ENEMYTYPE_SEVEN;

	// 追跡できるかを判定s
	const bool is_CanAgent = ((is_sphereBounds == true && is_ViewBounds == true) || is_absolute == true) && is_NotAction == true && is_NotTypeSeven == true;

	// 追跡できる
	if (is_CanAgent == true)
	{
		// ホーミング処理
		AgentEnemy(nCntEnemy);

		// モーションを歩きモーションにする
		g_Enemy[nCntEnemy].Motion.motionType = MOTIONTYPE_MOVE;

		// 敵の角度を設定
		g_Enemy[nCntEnemy].rotDest.y = SetAngle(&g_Enemy[nCntEnemy].rot, &g_Enemy[nCntEnemy].pos) + D3DX_PI;
	}
	else
	{
		// 攻撃モーションじゃない
		if (g_Enemy[nCntEnemy].Motion.motionType != MOTIONTYPE_ACTION)
		{
			// モーションをニュートラルにする
			g_Enemy[nCntEnemy].Motion.motionType = MOTIONTYPE_NEUTRAL;
		}
	}
}
//===============================================================================================================
// 逃げる敵の更新処理
//===============================================================================================================
void UpdateRunAwayEnemy(int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	// 敵の種類がスコア高いやつなら
	if (g_Enemy[nCntEnemy].nType == ENEMYTYPE_SEVEN)
	{
		// 体力が最大値と同等、つまりノーダメなら何もしない
		if (g_Enemy[nCntEnemy].nMaxLife == g_Enemy[nCntEnemy].nLife)
		{

		}
		else // 少しでもダメージを受けてて
		{
			// プレイヤーの近くに居たら
			if (sphererange(&g_Enemy[nCntEnemy].pos, &pPlayer->pos, 50.0f, 80.0f))
			{
				// 敵をプレイヤーに向かわせる
				AgentEnemy(nCntEnemy);

				// 敵の角度を設定
				g_Enemy[nCntEnemy].rotDest.y = SetAngle(&g_Enemy[nCntEnemy].rot, &g_Enemy[nCntEnemy].pos);
			}

			// 虹の素
			float fColor[3];
			fColor[0] = ((float)(rand() % 100) / 100.0f);
			fColor[1] = ((float)(rand() % 100) / 100.0f);
			fColor[2] = ((float)(rand() % 100) / 100.0f);

			// 高価そうなオーラを出す
			SetParticle(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 10, g_Enemy[nCntEnemy].pos.z),
				D3DXVECTOR3(D3DX_PI / 2.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(fColor[0], fColor[1], fColor[2], 1.0f),
				2.0f, 1, 10, 5, 20.0f, 20.0f, true, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		}
	}
}
//===============================================================================================================
// 敵の攻撃モーションの更新処理
//===============================================================================================================
void UpdateKickAttack(int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	// モーションの種類
	int Motiontype = g_Enemy[nCntEnemy].Motion.motionType;

	// キー
	int nKey = g_Enemy[nCntEnemy].Motion.nKey;

	// 最後のキー
	int LastKey = g_Enemy[nCntEnemy].Motion.aMotionInfo[Motiontype].nNumkey - 1;

	// モーションのカウント
	int nCounter = g_Enemy[nCntEnemy].Motion.nCountMotion;

	// 最大フレーム
	int EndFrame = g_Enemy[nCntEnemy].Motion.aMotionInfo[Motiontype].aKeyInfo[LastKey].nFrame;

	// プレイヤーのモデルの情報を代入
	D3DXVECTOR3 PlayerModel = SetMtxConversion(pPlayer->Motion.aModel[0].mtxWorld);

	// 敵のモデルの情報を代入
	D3DXVECTOR3 EnemyModel = SetMtxConversion(g_Enemy[nCntEnemy].Motion.aModel[3].mtxWorld);
	
	// 範囲内にいるかを判定
	const bool is_sphereBounds = sphererange(&PlayerModel, &EnemyModel, 20.0f, 50.0f) == true;

	// 敵が攻撃状態かを判定
	const bool is_EnemyNotAction = g_Enemy[nCntEnemy].AttackState == ENEMYATTACK_ATTACK;

	// プレイヤーがダメージ状態かどうかを判定
	const bool is_PlayerNotDamage = pPlayer->state != PLAYERSTATE_DAMAGE;

	// 攻撃をできるかを判定
	const bool CanDamage = is_sphereBounds == true && is_EnemyNotAction == true && is_PlayerNotDamage == true;

	// キックアクションの更新処理
	KickActionSet(nCntEnemy, nKey, nCounter, EndFrame, LastKey, pPlayer);

	// 攻撃範囲に入った
	if (CanDamage == true && CheckMotionBounds(nKey, nCounter,4, LastKey,0, EndFrame) == true)
	{
		// プレイヤーにダメージを与える
		HitPlayer(50,false, 0, 0);
	}
}
//===============================================================================================================
// 飛んでる敵の更新処理
//===============================================================================================================
void UpdateDroneEnemy(int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	if (g_Enemy[nCntEnemy].nType != ENEMYTYPE_SIX)
	{
		// ドローンの敵じゃなかったら
		return;
	}

	float fDistance = GetDistance(g_Enemy[nCntEnemy].pos, pPlayer->pos);

	if (fDistance >= 250.0f)
	{
		// 追尾処理
		AgentEnemy(nCntEnemy);
	}
	else
	{
		g_Enemy[nCntEnemy].move.x = 0.0f;
		g_Enemy[nCntEnemy].move.z = 0.0f;
	}
	// 範囲内だったら
	if (sphererange(&pPlayer->pos, &g_Enemy[nCntEnemy].pos, 20.0f, 300.0f))
	{
		g_Enemy[nCntEnemy].nCountAction++;
	}

	float fAngle = atan2f(pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x, pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z); // 敵からプレイやまでの角度を求める

	// ワールドマトリックスを変換
	D3DXVECTOR3 ModelPos = SetMtxConversion(pPlayer->Motion.aModel[2].mtxWorld);

	D3DXVECTOR3 dest = ModelPos - g_Enemy[nCntEnemy].pos; // プレイヤーのベクトルを求める
	D3DXVec3Normalize(&dest, &dest); // 正規化する

	g_Enemy[nCntEnemy].rotDest.y = fAngle + D3DX_PI; // 角度を代入

	// 弾を発射する処理
	if (g_Enemy[nCntEnemy].nCountAction >= 120)
	{
		// 左から場所、ベクトル、方向、寿命、威力、大きさ、速度
		SetBullet(g_Enemy[nCntEnemy].pos, dest, D3DXVECTOR3(0.0f, fAngle, 0.0f), 60, 2, 50.0f, 3.0f, true);

		// こっちはショットガン化する為の処理
		/*float fRand[3];
		for (int nCount = 0; nCount < 10; nCount++)
		{
			fRand[0] = (rand() / (double)RAND_MAX) * 0.4 - 0.2;
			fRand[1] = (rand() / (double)RAND_MAX) * 0.4 - 0.2;
			fRand[2] = (rand() / (double)RAND_MAX) * 0.4 - 0.2;
			SetBullet(g_Enemy[nCntEnemy].pos, D3DXVECTOR3(dest.x + fRand[0], dest.y + fRand[1], dest.z + fRand[2]), D3DXVECTOR3(0.0f, fAngle, 0.0f), 60, 2, 10.0f, 3.0f, true);
		}*/
		g_Enemy[nCntEnemy].nCountAction = 0;
	}
	
}
//===============================================================================================================
// けり攻撃の処理
//===============================================================================================================
void KickActionSet(int nCntEnemy, int nKey, int nCounter, int EndFrame,int LastKey, Player* pPlayer)
{
	// 敵を上昇させる
	if (CheckMotionBounds(nKey, nCounter, 0, 2, 0, EndFrame) == true && g_Enemy[nCntEnemy].Motion.motiontypeBlend == MOTIONTYPE_ACTION)
	{
		// 位置の差分を求める
		D3DXVECTOR3 DiffPos = g_Enemy[nCntEnemy].pos - pPlayer->pos;

		// 目的の角度を代入
		g_Enemy[nCntEnemy].rotDest.y = atan2f(DiffPos.x, DiffPos.z);

		// 上昇させる
		g_Enemy[nCntEnemy].move.y = 1.0f;
	}

	if (CheckMotionBounds(nKey, nCounter, 4, 4, 0, 10) == true && g_Enemy[nCntEnemy].Motion.motionType == MOTIONTYPE_ACTION && g_Enemy[nCntEnemy].Motion.bFirstMotion == false)
	{
		// 移動方向を求める
		D3DXVECTOR3 move = SetSubtractVectors(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z), D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y, g_Enemy[nCntEnemy].pos.z));
		D3DXVec3Normalize(&move, &move);

		// 移動方向を代入
		g_Enemy[nCntEnemy].move.x = move.x * KICKATTACK_MOVE;
		g_Enemy[nCntEnemy].move.z = move.z * KICKATTACK_MOVE;
	}

	if (CheckMotionBounds(nKey, nCounter, 4, LastKey, 0, EndFrame) == true && g_Enemy[nCntEnemy].Motion.motionType == MOTIONTYPE_ACTION && g_Enemy[nCntEnemy].Motion.bFirstMotion == false)
	{
		// 位置を0に近づける
		g_Enemy[nCntEnemy].pos.y += (0.0f - g_Enemy[nCntEnemy].pos.y) * 0.3f;
	}

}
//===============================================================================================================
// 敵の死亡モーションの処理
//===============================================================================================================
void UpdateDeathParam(int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	// 現在のキー
	int nKey = g_Enemy[nCntEnemy].Motion.nKey;

	// 最後のキー
	int nLastKey = g_Enemy[nCntEnemy].Motion.aMotionInfo[g_Enemy[nCntEnemy].Motion.motionType].nNumkey - 1;

	int nCountMotion = g_Enemy[nCntEnemy].Motion.nCountMotion;

	// 吹き飛びモーション
	if (nKey == 0 && nCountMotion == 1 && g_Enemy[nCntEnemy].Motion.bFirstMotion == false)
	{
		g_Enemy[nCntEnemy].move.y = 5.0f;
	}

	// 吹っ飛びの量
	float BlowMoveX = pPlayer->AttackSp ? 4.0f : 1.0f;
	float BlowMoveZ = pPlayer->AttackSp ? 4.0f : 1.0f;

	// キーが3いないだったら
	if (nKey <= 5)
	{
		float fMoveX = sinf(g_Enemy[nCntEnemy].rot.y) * 4.0f + (4.0f / (float)pPlayer->nDamage) * BlowMoveX;
		float fMoveZ = cosf(g_Enemy[nCntEnemy].rot.y) * 4.0f + (4.0f / (float)pPlayer->nDamage) * BlowMoveZ;

		g_Enemy[nCntEnemy].move.x = fMoveX;
		g_Enemy[nCntEnemy].move.z = fMoveZ;
	}
	// 最後のキーまで行ったら
	if (nKey >= nLastKey)
	{
		// テリトリーの敵の減少処理
		DecreaseTerritoryEnemy(nCntEnemy);

		g_Enemy[nCntEnemy].bUse = false;

		// 敵の影を消す
		KillShadow(g_Enemy[nCntEnemy].nIdxShadow);

		// マップから消す
		EnableMap(g_Enemy[nCntEnemy].nIdxMap);
	}
}
//===============================================================================================================
// スポナーの処理
//===============================================================================================================
void UpdateEnemySpawn(void)
{
	// 前に発生した位置を保存する変数
	int OldPos = -1;

	// ボスを出現させるか
	bool EnableSetBoss = false;

	// ボスが何体出てるか
	static int nNumBoss = 0;

	// 発生させるテリトリー分
	for (int nCnt = 0; nCnt < SETNUM_TERRITORY; nCnt++)
	{
		// 敵のテリトリーの位置を決める
		int SpawnPos = rand() % NUM_SPAWNPOSITION;

		// テリトリーの位置が前の位置と同じだったら
		if (SpawnPos == OldPos)
		{
			// 同じじゃなくなるまでループ
			while (SpawnPos == OldPos)
			{
				// 別のテリトリーになるまで抽選
				SpawnPos = rand() % NUM_SPAWNPOSITION;
			}
		}

		// ボスが出る確率
		int BossSpawnChance = rand() % 101;

		// ボスが体なら && 40%の確率で出現
		if (BossSpawnChance <= 100 && nNumBoss == 0 && noFirstSetBoss == false)
		{
			// ボスを出現させる
			EnableSetBoss = true;

			// ボスの数のカウントを1体にする
			nNumBoss = 1;
		}
		else
		{
			// もとに戻す
			EnableSetBoss = false;
		}

		// 敵のテリトリーを設定
		switch (SpawnPos)
		{
		case 0:
			// テリトリーの設定
			SetTerritoryparam(nCnt, TERRITTORYPOS_ONE, SpawnPos, EnableSetBoss);

			break;
		case 1:
			// テリトリーの設定
			SetTerritoryparam(nCnt, TERRITTORYPOS_TWO, SpawnPos, EnableSetBoss);
			break;
		case 2:
			// テリトリーの設定
			SetTerritoryparam(nCnt, TERRITTORYPOS_THREE, SpawnPos, EnableSetBoss);
			break;
		case 3:
			// テリトリーの設定
			SetTerritoryparam(nCnt, TERRITTORYPOS_FOUR, SpawnPos, EnableSetBoss);
			break;
		default:
			break;
		}

		// 前回のテリトリーを代入
		OldPos = SpawnPos;
	}
	
	// 最初だけボスを出さない処理
	noFirstSetBoss = false;

	// もとに戻す
	nNumBoss = 0;
}
//==============================================================================================================
// ボスがどこに発生したか
//==============================================================================================================
int GetTerritoryBossPos(void)
{
	return g_nBossPos;
}
//===============================================================================================================
// 線と球の当たり判定
//===============================================================================================================
bool CollisionLine(D3DXVECTOR3* pFirstPos, D3DXVECTOR3* pEndPos,float fRadius)
{
	BLOCK* pBlock = GetBlock();
	Camera* pCamera = GetCamera();

	// 球の半径
	float radius = fRadius;

	// 球の半径を求める
	float Radius = radius * radius;

	// ブロックの最大数分回す
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		// ブロックが使われていなければ
		if (pBlock->bUse == false)
		{
			// 処理を読み飛ばす
			continue;
		}

		// 線分の終点から球体の中心点までのベクトル
		D3DXVECTOR3 pvEnd = *pFirstPos - pBlock->Obb.CenterPos;

		// 線分の終点から球体の中心点までのベクトル
		D3DXVECTOR3 pvFirst = *pEndPos - pBlock->Obb.CenterPos;

		// 始点～終点までのベクトル
		D3DXVECTOR3 sv = *pFirstPos - *pEndPos;

		// 長さX
		float LengthX = pFirstPos->x - pEndPos->x;
		// 長さY
		float LengthY = pFirstPos->y - pEndPos->y;
		// 長さZ
		float LengthZ = pFirstPos->z - pEndPos->z;

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
			pCamera->nBlockIdx = nCntBlock;
			return true;
		}
		// 交差した
		else if (D3DXVec3Length(&pvFirst) < Radius)
		{
			pCamera->nBlockIdx = nCntBlock;
			return true;
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
				pCamera->nBlockIdx = nCntBlock;
				return true;
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
				pCamera->nBlockIdx = nCntBlock;
				return true;
			}
		}
	}
	return false;
}
//===============================================================================================================
// 鳴らす音の設定
//===============================================================================================================
void SetSoundWepon(int nType)
{
	switch (nType)
	{
	case ITEMTYPE_BAT: // バット持っているとき
		// 音楽再生
		PlaySound(SOUND_LABEL_BAT_SE);
		break;

	case ITEMTYPE_HUNMER: // ハンマー持っているとき
		// 音楽再生
		PlaySound(SOUND_LABEL_HAMMER_SE);
		break;

	case ITEMTYPE_TORCH: // 炎持っているとき
		// 音楽再生
		PlaySound(SOUND_LABEL_WEPONFIRESE);
		break;

	case ITEMTYPE_KATANA: // 刀持っているとき
		// 音楽再生
		PlaySound(SOUND_LABEL_ACTION_SE);
		break;

	case ITEMTYPE_LIGHT: // 蛍光灯
		// 音楽再生
		PlaySound(SOUND_LABEL_ACTION_SE);
		break;

	case ITEMTYPE_FISH: // 魚系統
		// 音楽再生
		PlaySound(SOUND_LABEL_WATERWEPON);
		break;

	case ITEMTYPE_SPEAR: // 槍の武器音
		// 音楽再生
		PlaySound(SOUND_LABEL_SPEARWEPON);
		break;

	case ITEMTYPE_IRON: // 鉄系統の武器音
		// 音楽再生
		PlaySound(SOUND_LABEL_METALWEPON);
		break;

	case ITEMTYPE_HARISEN: // ハリセン武器音
		// 音楽再生
		PlaySound(SOUND_LABEL_HARISENWEPON);
		break;

	case ITEMTYPE_TELEPHONEPOLE: // 電柱
		// 音楽再生
		PlaySound(SOUND_LABEL_TELPHONWEPON);
		break;

	case ITEMTYPE_SURFBOARD: // サーフボード
		// 音楽再生
		PlaySound(SOUND_LABEL_WATERWEPON);
		break;

	case ITEMTYPE_TORSO: // トルソー
		// 音楽再生
		PlaySound(SOUND_LABEL_ACTION_SE);
		break;

	case ITEMTYPE_WOOD: // 木材
		// 音楽再生
		PlaySound(SOUND_LABEL_BAT_SE);
		break;

	case ITEMTYPE_BONE: // 骨
		// 音楽再生
		PlaySound(SOUND_LABEL_METALWEPON);
		break;

	case ITEMTYPE_GOLF: // ゴルフ
		// 音楽再生
		PlaySound(SOUND_LABEL_METALWEPON);
		break;

	case ITEMTYPE_BAR: // バール
		// 音楽再生
		PlaySound(SOUND_LABEL_METALWEPON);
		break;

	case ITEMTYPE_ICEBLOCK: // 氷ブロック
		// 音楽再生
		PlaySound(SOUND_LABEL_ITEMTHROW);
		break;

	case ITEMTYPE_STONE: // 石
		// 音楽再生
		PlaySound(SOUND_LABEL_ITEMTHROW);
		break;

	case ITEMTYPE_HEX: // 呪物
		// 音楽再生
		PlaySound(SOUND_LABEL_ITEMTHROW);
		break;

	case ITEMTYPE_MEGAPHONE: // メガホン
		// 音楽再生
		PlaySound(SOUND_LABEL_ITEMTHROW);
		break;

	case ITEMTYPE_HEADSTATUE: // 頭
		// 音楽再生
		PlaySound(SOUND_LABEL_ITEMTHROW);
		break;

	default: // 武器持っていない時 
		break;
	}

}
//===============================================================================================================
// 合成武器の音設定
//===============================================================================================================
void SetCreateWeponSound(int nType)
{
	switch (nType)
	{
	case ITEMTYPE_STONEBAT: // 石バット
		// 音楽再生
		PlaySound(SOUND_LABEL_BAT_SE);
		break;

	case ITEMTYPE_IRONBAT: // 鉄バット
		// 音楽再生
		PlaySound(SOUND_LABEL_METALWEPON);
		break;

	case ITEMTYPE_GOLFHUNMER: // ゴルフハンマー
		// 音楽再生
		PlaySound(SOUND_LABEL_HAMMER_SE);
		break;

	case ITEMTYPE_HEADSTATUTORSO: // トルソーと頭の合成
		// 音楽再生
		PlaySound(SOUND_LABEL_HAMMER_SE);
		break;
	case ITEMTYPE_BONESPEAR: // 骨の槍
		// 音楽再生
		PlaySound(SOUND_LABEL_SPEARWEPON);
		break;
	case ITEMTYPE_HEXMANDOLIN: // 呪物とマンドリンの合成
		// 音楽再生
		PlaySound(SOUND_LABEL_MUSICWEPON);
		break;

	case ITEMTYPE_ICEBLOCKSOWRD: // 氷の剣
		// 音楽再生
		PlaySound(SOUND_LABEL_ICEWEPONSE);
		break;

	case ITEMTYPE_TORCHSWORD: // 炎の剣
		// 音楽再生
		PlaySound(SOUND_LABEL_WEPONFIRESE);
		break;

	case ITEMTYPE_LIGHTWOOD: // 蛍光灯の刀
		// 音楽再生
		PlaySound(SOUND_LABEL_ACTION_SE);
		break;

	case ITEMTYPE_SURFBOARDFISH: // サーフボードの合成武器
		// 音楽再生
		PlaySound(SOUND_LABEL_WATERWEPON);
		break;

	default: // 武器持っていない時 
		break;
	}

}
//===============================================================================================================
// テリトリーの消去
//===============================================================================================================
void DeletTerritory(void)
{
	// テリトリーの数回す
	for (int nCnt = 0; nCnt < SETNUM_TERRITORY; nCnt++)
	{
		if (g_Territory[nCnt].nNumEnemy <= 0 && g_Territory[nCnt].bBoss == false && g_Territory[nCnt].bUse == true)
		{
			assert(g_Territory[nCnt].CylinderIdx >= 0 && "テリトリーIdxオーバーラン");
			DeleteCylinder(g_Territory[nCnt].CylinderIdx);

			// スコアを加算
			AddScore(100000);

			// 使用状態にする
			g_Territory[nCnt].bUse = false;
			g_nNumTerritory--;
		}
	}
}
//===============================================================================================================
// テリトリーの敵の減少処理
//===============================================================================================================
void DecreaseTerritoryEnemy(int nCntEnemy)
{
	// テリトリーの数ぶん回す
	for (int nCntTerritory = 0; nCntTerritory < SETNUM_TERRITORY; nCntTerritory++)
	{
		if (g_Enemy[nCntEnemy].TerritoryNumber == g_Territory[nCntTerritory].nNumber)
		{
			g_Territory[nCntTerritory].nNumEnemy--;
		}
	}
}
//===============================================================================================================
// 敵の数の正規化
//===============================================================================================================
void NormalizeNumEnemy(void)
{
	// テリトリーの数ぶん回す
	for (int nCntTerritory = 0; nCntTerritory < SETNUM_TERRITORY; nCntTerritory++)
	{
		if (g_Territory[nCntTerritory].nNumEnemy <= 0)
		{
			g_Territory[nCntTerritory].nNumEnemy = 0;
		}
	}
}
//===============================================================================================================
// テリトリーの中から出さない処理
//===============================================================================================================
bool KeepInTerritory(int nCntEnemy)
{
	// テリトリーの数ぶん回す
	for (int nCntTerritory = 0; nCntTerritory < SETNUM_TERRITORY; nCntTerritory++)
	{
		// 自分のテリトリーを確認
		if (g_Enemy[nCntEnemy].TerritoryNumber != g_Territory[nCntTerritory].nNumber)
		{
			continue;
		}

		// シリンダーの当たり判定
		if (CollisionCylinder(g_Territory[nCntTerritory].CylinderIdx, &g_Enemy[nCntEnemy].pos))
		{
			// 敵を押し戻す
			g_Enemy[nCntEnemy].pos = g_Enemy[nCntEnemy].posOld;
		}
	}

	return false;
}
//==============================================================================================================
// スポーンカウントの設定
//==============================================================================================================
void SetSpawnCount(void)
{
	// スポーンのカウンター
	static int SpawnCnt = 0;

	GAMESTATE gamestate = GetGameState();

	// 時間の取得
	int nSecond = GetTimeSecond();
	int nMinute = GetTimeMinute();

	// 時間が残り15秒だったら出さない
	if (nMinute <= 0 && nSecond <= 15) return;

	// すべての敵を倒した
	if (g_Territory[0].nNumEnemy <= 0 && g_Territory[1].nNumEnemy <= 0 && g_Territory[0].bBoss == false && g_Territory[1].bBoss == false)
	{
		if (g_TerritorySetTime == 0)
		{
			// タイマーを設置
			SetCounter(D3DXVECTOR3(1155.0f, 205.0f, 0.0f), COUNTER_COUNTDOWN, 10, 10.0f, 15.0f, COUNTERTYPE_TERRITORY);
		}
		
		// スポーンカウントを加算
		g_TerritorySetTime++;

		// 10秒たったら
		if (g_TerritorySetTime >= FRAME * 10)
		{
			// 敵の出現の更新処理
			UpdateEnemySpawn();
		}
	}
	else
	{
		// スポーンカウンターをリセット
		g_TerritorySetTime = 0;
	}
}
//==============================================================================================================
// テリトリーの設定
//==============================================================================================================
void SetTerritoryparam(int nTerritoryIdx, D3DXVECTOR3 pos,int SpawnerPos, bool bSetBoss)
{
	// ボスを出せる
	if (bSetBoss == true)
	{
		// ボスの出現が出現している状態にする
		g_Territory[nTerritoryIdx].bBoss = bSetBoss;

		// ボスの出現時の設定
		SetEventParam(pos);
		
		// ボスが発生した位置を代入
		g_nBossPos = SpawnerPos;

		// シリンダーのインデックス
		g_Territory[nTerritoryIdx].CylinderIdx = SetMeshCylinder(pos, CYLINDERTYPE_TERRITORY, 0, TERRITORYRADIUS, COLOR_CRIMSON, 16, 1, 0.0f, 3000.0f);
	}
	else
	{
		// サウンド再生
		PlaySound(SOUND_LABEL_ENEMYPOP_SE);

		// ボスがいない状態にする
		g_Territory[nTerritoryIdx].bBoss = bSetBoss;

		// シリンダーのインデックス
		g_Territory[nTerritoryIdx].CylinderIdx = SetMeshCylinder(pos, CYLINDERTYPE_TERRITORY, 0, TERRITORYRADIUS, COLOR_AQUA, 16, 1, 0.0f, 3000.0f);
	}

	g_nNumTerritory++;

	// 使用状態にする
	g_Territory[nTerritoryIdx].bUse = true;

	// 敵をスポーンさせる
	SpawnEnemy(SpawnerPos, nTerritoryIdx);

	// シリンダーの番号をもらう
	g_Territory[nTerritoryIdx].nNumber = nTerritoryIdx;

	// テリトリーの中心の位置
	g_Territory[nTerritoryIdx].pos = pos;
}
//==============================================================================================================
// テリトリーの外のスポナー
//==============================================================================================================
void OutTerritorySpawner(int nSpawner)
{
	// ランダムな値
	float randum_valueX = (float)(rand() % 10);
	float randum_valueZ = (float)(rand() % 10);

	if (g_nNumEnemy <= MAX_ENEMY / 2)
	{
		// スポナーの設定
		switch (nSpawner)
		{
		case 0:
			// 敵をセット
			SetEnemy(D3DXVECTOR3(1094.0f + randum_valueX, 0.0f, 288.0f + randum_valueZ), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f), -1);
			break;
		case 1:
			// 敵をセット
			SetEnemy(D3DXVECTOR3(-243.0f + randum_valueX, 0.0f, 169.0f + randum_valueZ), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f), -1);
			break;
		case 2:
			// 敵をセット
			SetEnemy(D3DXVECTOR3(135.0f + randum_valueX, 0.0f, -936.0f + randum_valueZ), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f), -1);
			break;
		case 3:
			// 敵をセット
			SetEnemy(D3DXVECTOR3(-118.0f + randum_valueX, 0.0f, 890.0f + randum_valueZ), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f), -1);
			break;
		default:
			break;
		}
	}
}
//==============================================================================================================
// ターゲットの位置の更新
//==============================================================================================================
void UpdateTargetPosition(int nCntEnemy)
{
	// テリトリー内の敵じゃ勝ったら
	if (g_Enemy[nCntEnemy].TerritoryNumber == -1)
	{
		// 関数を抜ける
		return;
	}

	// ドローンの敵
	if (g_Enemy[nCntEnemy].nType == ENEMYTYPE_SIX)
	{
		// ターゲットの位置設定処理
		SetPositiontarget(g_Enemy[nCntEnemy].nIdxtarget, D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 40.0f, g_Enemy[nCntEnemy].pos.z));
	}
	// それ以外
	else
	{
		// ターゲットの位置設定処理
		SetPositiontarget(g_Enemy[nCntEnemy].nIdxtarget, D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 70.0f, g_Enemy[nCntEnemy].pos.z));
	}
}
//==============================================================================================================
// テリトリーにマークする
//==============================================================================================================
void UpdateTerritoryMark(void)
{
	// 矢印の取得
	Mark* pMark = GetMark();

	// 保存用の距離
	float StockDistance = 0.0f;

	// 今の距離
	float fDistance = 0.0f;

	// 最初だけ通す
	bool bFirst = true;

	// インデックス
	int nIdx = 0;

	// 矢印のワールドマトリックスを変換
	D3DXVECTOR3 MarkPos = SetMtxConversion(pMark->mtxWorld);

	// テリトリー分回す
	for (int nCnt = 0; nCnt < SETNUM_TERRITORY; nCnt++)
	{
		if (g_Territory[nCnt].bUse == false) continue;

		// 距離を取得
		fDistance = GetDistance(g_Territory[nCnt].pos, MarkPos);

		// 最初に距離を持っておく
		if (bFirst == true)
		{
			// 距離を保存
			StockDistance = fDistance;

			// もう通さない
			bFirst = false;

			// インデックスを保存
			nIdx = nCnt;
		}
		else
		{
			// 今の距離が保存していた距離よりも小さかったら
			if (fDistance < StockDistance)
			{
				// 距離を上書き
				StockDistance = fDistance;

				// インデックスを上書き
				nIdx = nCnt;
			}
		}
	}

	// 差分を求める
	D3DXVECTOR3 DiffPos = MarkPos - g_Territory[nIdx].pos;

	// 角度を求める
	float fAngle = atan2f(DiffPos.x, DiffPos.z);

	// 目的の角度に代入
	pMark->rotDest.y = fAngle;

	// テリトリーが未使用状態だったら
	if (g_Territory[0].bUse == false && g_Territory[1].bUse == false)
	{
		// 未使用にする
		pMark->bUse = false;
	}
	// テリトリーの中に入ったら
	if (StockDistance <= TERRITORYRADIUS)
	{
		// 未使用にする
		pMark->bUse = false;
	}
	else
	{
		if (g_Territory[0].bUse == true || g_Territory[1].bUse == true)
		{
			// テリトリーの外だったら使用状態
			pMark->bUse = true;
		}
	}
	if (isPlayerAttaking() == true)
	{
		pMark->bUse = false;
	}
}
//==============================================================================================================
// 敵を倒したときのスコアとゲージの更新処理
//==============================================================================================================
void UpdateScoreAndGage(int nCntEnemy)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// ゲーム状態の取得
	GAMESTATE gamestate = GetGameState();

	// ゲームが終わっていたら関数を抜ける
	if (gamestate == GAMESTATE_END) return;

	// 種類がスコアの高い敵だったら
	const bool isTypeSeven = g_Enemy[nCntEnemy].nType == ENEMYTYPE_SEVEN;

	// テリトリーにいるか
	const bool isTerritory = g_Enemy[nCntEnemy].TerritoryNumber != -1;

	// フィーバーモードか判定
	const bool isFeverMode = pPlayer->FeverMode == true;
	
	// テリトリー内の敵だったら
	if (isTerritory == true)
	{
		// スコアを求める
		float score = isFeverMode ? (isTypeSeven ? 62000.0f : 26000.0f) : (isTypeSeven ? 36200.0f : 18100.0f);

		// スペシャルゲージを求める
		float spgage = isTypeSeven ? 8.0f : 4.0f;

		// フィーバーゲージ
		float fever = isFeverMode ? 0.0f : 8.0f;

		// フィーバーゲージを加算
		AddFever(fever);

		// スコアを加算
		AddScore(score);

		// spゲージを加算
		AddSpgauge(spgage);
	}
	else
	{
		// スコアを求める
		float score = isFeverMode ? (isTypeSeven ? 32000.0f : 16000.0f) : (isTypeSeven ? 16200.0f : 8100.0f);

		// スペシャルゲージを求める
		float spgage = isTypeSeven ? 5.0f : 2.5f;

		// フィーバーゲージ
		float fever = isFeverMode ? 0.0f : 4.0f;

		// フィーバーゲージを加算
		AddFever(fever);

		// スコアを加算
		AddScore(score);

		// spゲージを加算
		AddSpgauge(spgage);
	}


}
//==============================================================================================================
// ヒットストップの設定処理
//==============================================================================================================
void SetUpHitStop(int* pHitStopCount)
{
	Player* pPlayer = GetPlayer();

	// 攻撃してなかったら
	if (isPlayerAttaking() == false) return;

	// 片手のSPモーション
	const bool isOneHandSP = pPlayer->WeponMotion == MOTION_ONEHANDBLOW;

	// 敵のヒットストップ
	*pHitStopCount = isOneHandSP ? 25 : 0;

	// プレイヤーのヒットストップ
	pPlayer->HitStopCount = isOneHandSP ? 25 : 0;
	
	// 設定が完了した
	if (isOneHandSP == true) return;

	// 両手のSPモーション
	const bool isDoubleSP = pPlayer->WeponMotion == MOTION_SPDOUBLE && pPlayer->Motion.nKey >= 5;

	// 敵のヒットストップ
	*pHitStopCount = isDoubleSP ? 15 : 0;

	// プレイヤーのヒットストップ
	pPlayer->HitStopCount = isDoubleSP ? 15 : 0;

	// 設定が完了した
	if (isDoubleSP == true) return;

	// ハンマーのSPモーション
	const bool isHunmerSP = pPlayer->WeponMotion == MOTION_SPHAMMER;

	// 敵のヒットストップ
	*pHitStopCount = isHunmerSP ? 5 : 0;

	// プレイヤーのヒットストップ
	pPlayer->HitStopCount = isHunmerSP ? 10 : 0;

	// 設定が完了した
	if (isHunmerSP == true) return;

	// 刀のSPモーション
	const bool isKatanaSP = pPlayer->WeponMotion == MOTION_SP && pPlayer->Motion.nKey >= 4;

	// 敵のヒットストップ
	*pHitStopCount = isKatanaSP ? 15 : 0;

	// プレイヤーのヒットストップ
	pPlayer->HitStopCount = isKatanaSP ? 15 : 0;

	// 設定が完了した
	if (isKatanaSP == true) return;

	// 槍のSPモーション
	const bool isSpearSP = pPlayer->WeponMotion == MOTION_SPPIERCING;

	// 敵のヒットストップ
	*pHitStopCount = isSpearSP ? 15 : 0;

	// プレイヤーのヒットストップ
	pPlayer->HitStopCount = isSpearSP ? 15 : 0;
	
	// 設定が完了した
	if (isSpearSP == true) return;

	// 攻撃モーション1
	const bool isAttack1 = pPlayer->Motion.motiontypeBlend == MOTIONTYPE_ACTION;

	// 敵のヒットストップ
	*pHitStopCount = isAttack1 ? 4 : 0;

	// プレイヤーのヒットストップ
	pPlayer->HitStopCount = isAttack1 ? 4 : 0;

	// 設定が完了した
	if (isAttack1 == true) return;

	// 攻撃モーション4
	const bool isAttack4 = pPlayer->Motion.motiontypeBlend == MOTIONTYPE_ACTION4;

	// 敵のヒットストップ
	*pHitStopCount = isAttack4 ? 4 : 0;

	// プレイヤーのヒットストップ
	pPlayer->HitStopCount = isAttack4 ? 4 : 0;

	// 設定が完了した
	if (isAttack4 == true) return;

}
//==============================================================================================================
// テリトリーが何個あるか
//==============================================================================================================
int GetNumTeriitory(void)
{
	return g_nNumTerritory;
}
//==============================================================================================================
// ボスがいるテリトリーの消去
//==============================================================================================================
void SetEnableBossTerritory(bool bEnable)
{
	// すべてのテリトリーを調べる
	for (int nCnt = 0; nCnt < SETNUM_TERRITORY; nCnt++)
	{
		// テリトリーにボスがいたら
		if (g_Territory[nCnt].bBoss == true)
		{
			// 状態を変更
			g_Territory[nCnt].bBoss = bEnable;
		}
	}
}
