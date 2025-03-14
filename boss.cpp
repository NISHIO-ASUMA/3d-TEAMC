//==============================================================================================================
//
// ボス [boss.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include "boss.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"
#include "player.h"
#include "Shadow.h"
#include "block.h"
#include "Damagepop.h"
#include "Particle.h"
#include "explosion.h"
#include "HPGauge.h"
#include "spgauge.h"
#include "Score.h"
#include "time.h"
#include "sound.h"
#include "game.h"
#include "polygon.h"
#include "wall.h"
#include "meshimpact.h"
#include "minimap.h"
#include "billboard.h"
#include "bosslife.h"
#include "Effect.h"
#include "math.h"
#include "meshcylinder.h"
#include "mark.h"
#include "event.h"
#include "effectEdit.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MAX_WORD (256)			 // 最大の文字数
#define SHADOWSIZEOFFSET (40.0f) // 影の大きさのオフセット
#define SHADOW_A (1.0f)          // 影の濃さの基準
#define NUM_MTX (8)				 //　剣のワールドマトリックスの数
#define BOSS_LIFE (10000)        // ボスの最大HP
#define TRANSPARENT_FRAME (300.0f) // 透明化のフレーム
#define MAX_ELEMENT (5) // 属性の最大数

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void LoadBoss(void);																		// ボスを読み込む
int LoadBossFilename(FILE* pFile, int nNumModel, char* aString, int nType);					// ボスのモデルのロード処理
void LoadBossCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType);			// ボスのパーツの設定処理
void LoadBossMotionSet(FILE* pFile, char* aString, int nNumModel, int nType);				// ボスのモーションのロード処理
void LoadBossKeySet(FILE* pFile, char* aString, int nType, int nCntMotion);					// ボスのモーションのキーの読み込み処理

void colisionSword(int nCntBoss);															// 剣とボスの当たり判定
void CollisionToBoss(int nCntBoss);															// ボスとボスの当たり判定
bool SetAbnormalCondition(int nType, int nTime, int nDamage,int nCntBoss);					// 状態異常の設定
void UpdateAbnormalCondition(int nCntBoss);													// 状態異常の更新処理
void SetRasuAttack(int nCntBoss);															// ボスの突進攻撃の設定
void SetDoubleRasuAttack(int nCntBoss);														// ボスの二回突進してくる攻撃処理
void UpdateAgentBoss(int nCntBoss);															// ボスの追跡の更新処理
void DeathMotionContlloer(int nCntBoss);													// ボスの死亡モーションの処理
void EndEventBossState(int nCntBoss);														// イベントが終わった後にボスを消す処理
void HitBossAbnormalCondition(int nCntBoss);												// ボスに当たった時のエフェクト
void HitBossAbnormalConditionParam(int nCntBoss, int nElement, int ChargeValue, int MaxCharge, int stateCnt);  // ボスに当たった時のエフェクトのパラメータ
void SpawnItem(D3DXVECTOR3 pos);

//**************************************************************************************************************
// グローバル変数宣言
//**************************************************************************************************************
Boss g_Boss[MAX_BOSS]; // ボスの情報
MOTION g_LoadBoss;
int nKeyBoss, nCntMotionBoss;

//===============================================================================================================
// ボスの初期化処理
//===============================================================================================================
void InitBoss(void)
{
	//InitBossLife();

	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// 全部のボス分回す
	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		g_Boss[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 座標
		g_Boss[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 移動量
		g_Boss[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 角度
		g_Boss[nCnt].bUse = false;						   // 未使用状態
		g_Boss[nCnt].nLife = BOSS_LIFE;					   // 体力
		g_Boss[nCnt].state = BOSSSTATE_NORMAL;			   // 状態
		g_Boss[nCnt].Speed = 5.0f;						   // 足の速さ
		g_Boss[nCnt].AttackState = BOSSATTACK_NO;		   // 攻撃状態
		g_Boss[nCnt].nHitStopCount = 0;                    // ヒットストップのカウント
		g_Boss[nCnt].BossMat = {};						   // ボスのマテリアル
		g_Boss[nCnt].bTransparent = false;                 // ボスを透明にするフラグ

		for (int nCnt2 = 0; nCnt2 < MAX_ELEMENT; nCnt2++)
		{
			g_Boss[nCnt].nStateCharge[nCnt2] = 0;
			g_Boss[nCnt].nStateCount[nCnt2] = 0;
		}
	}

	LoadBoss(); // ボスのロード
}
//===============================================================================================================
// ボスの終了処理
//===============================================================================================================
void UninitBoss(void)
{
	//UninitBossLife();

	for (int nCntModel = 0; nCntModel < g_LoadBoss.nNumModel; nCntModel++)
	{
		// テクスチャの破棄
		for (int nCntTex = 0; nCntTex < (int)g_LoadBoss.aModel[nCntModel].dwNumMat; nCntTex++)
		{
			if (g_LoadBoss.aModel[nCntModel].pTexture[nCntTex] != NULL)
			{
				g_LoadBoss.aModel[nCntModel].pTexture[nCntTex]->Release();
				g_LoadBoss.aModel[nCntModel].pTexture[nCntTex] = NULL;
			}
		}

		// メッシュの破棄
		if (g_LoadBoss.aModel[nCntModel].pMesh != NULL)
		{
			g_LoadBoss.aModel[nCntModel].pMesh->Release();
			g_LoadBoss.aModel[nCntModel].pMesh = NULL;
		}

		// マテリアルの破棄
		if (g_LoadBoss.aModel[nCntModel].pBuffMat != NULL)
		{
			g_LoadBoss.aModel[nCntModel].pBuffMat->Release();
			g_LoadBoss.aModel[nCntModel].pBuffMat = NULL;
		}
	}

	// 全ボス分回す
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		// モデル分回す
		for (int nCntModel = 0; nCntModel < g_Boss[nCntBoss].Motion.nNumModel; nCntModel++)
		{
			// テクスチャの破棄
			for (int nCntTex = 0; nCntTex < (int)g_Boss[nCntBoss].Motion.aModel[nCntModel].dwNumMat; nCntTex++)
			{
				if (g_Boss[nCntBoss].Motion.aModel[nCntModel].pTexture[nCntTex] != NULL)
				{
					g_Boss[nCntBoss].Motion.aModel[nCntModel].pTexture[nCntTex] = NULL;
				}
			}

			// メッシュの破棄
			if (g_Boss[nCntBoss].Motion.aModel[nCntModel].pMesh != NULL)
			{
				g_Boss[nCntBoss].Motion.aModel[nCntModel].pMesh = NULL;
			}

			// マテリアルの破棄
			if (g_Boss[nCntBoss].Motion.aModel[nCntModel].pBuffMat != NULL)
			{
				g_Boss[nCntBoss].Motion.aModel[nCntModel].pBuffMat = NULL;
			}
		}
	}

}
//===============================================================================================================
// ボスの更新処理
//===============================================================================================================
void UpdateBoss(void)
{
	Player* pPlayer = GetPlayer();
	GAMESTATE gameState = GetGameState();

	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		g_Boss[nCnt].nHitStopCount--;

		// 使用状態のみ
		if (g_Boss[nCnt].bUse == false || g_Boss[nCnt].nHitStopCount > 0)
		{
			continue;
		}

		// ボスのHPゲージの更新
		UpdateBossLife(&g_Boss[nCnt]);		

		// 状態異常の更新処理
		UpdateAbnormalCondition(nCnt);

		switch (g_Boss[nCnt].state)
		{
		case BOSSSTATE_NORMAL:
			break;
		case BOSSSTATE_AGENT:
			break;
		case BOSSSTATE_ATTACK:
			break;
		case BOSSSTATE_DAMAGE:
			g_Boss[nCnt].nCounterState--;

			if (g_Boss[nCnt].nCounterState <= 0)
			{
				g_Boss[nCnt].state = BOSSSTATE_NORMAL; // 敵の状態をノーマルにする
			}
			break;
		default:
			break;
		}

		// ミニマップの位置の設定
		SetMiniMapPotision(g_Boss[nCnt].nIdxMap, &g_Boss[nCnt].pos);

		// HPゲージの位置設定処理
		SetPositionLifeBar(g_Boss[nCnt].nLifeBarIdx, g_Boss[nCnt].nLifeFrame,g_Boss[nCnt].nLifeDelayIdx,g_Boss[nCnt].pos);

		// 移動量の減衰
		g_Boss[nCnt].move.x += (0.0f - g_Boss[nCnt].move.x) * 0.25f;
		g_Boss[nCnt].move.z += (0.0f - g_Boss[nCnt].move.z) * 0.25f;

		// 前回の位置を代入
		g_Boss[nCnt].posOld = g_Boss[nCnt].pos;

		// 位置の更新
		g_Boss[nCnt].pos += g_Boss[nCnt].move;

		// 影の位置の更新
		SetPositionShadow(g_Boss[nCnt].nIdxShadow, g_Boss[nCnt].pos, SHADOWSIZEOFFSET + SHADOWSIZEOFFSET * g_Boss[nCnt].pos.y / 200.0f, SHADOW_A / (SHADOW_A + g_Boss[nCnt].pos.y / 30.0f));

		// モーションの更新処理
		UpdateMotion(&g_Boss[nCnt].Motion);

		// モーションが終わったら
		if (g_Boss[nCnt].Motion.aMotionInfo[g_Boss[nCnt].Motion.motionType].bLoop == false && g_Boss[nCnt].Motion.nKey >= g_Boss[nCnt].Motion.aMotionInfo[g_Boss[nCnt].Motion.motionType].nNumkey - 1)
		{
			g_Boss[nCnt].AttackState = BOSSATTACK_NO;					// ボスの攻撃状態を攻撃してない状態にする
		}

		// ゲームの状態がムービーだったら
		if (gameState == GAMESTATE_MOVIE)
		{
			continue;
		}

		if (g_Boss[nCnt].Motion.motiontypeBlend == MOTIONTYPE_DEATH)
		{
			DeathMotionContlloer(nCnt);
			continue;
		}

		// 敵の攻撃の設定
		switch (g_Boss[nCnt].Motion.motiontypeBlend)
		{
		case MOTIONTYPE_ACTION:
			SetRasuAttack(nCnt);
			break;
		case MOTIONTYPE_ACTION2:
			SetDoubleRasuAttack(nCnt);
			break;
		default:
			break;
		}

		// アイテムが当たったか
		if (HitThrowItem(&g_Boss[nCnt].pos, 10.0f, 50.0f) && g_Boss[nCnt].state != BOSSSTATE_DAMAGE)
		{
			HitBoss(nCnt, (float)pPlayer->nDamage * 1.5f);
		}

		// 壁との当たり判定
		CollisionWall(&g_Boss[nCnt].pos, &g_Boss[nCnt].posOld, &g_Boss[nCnt].move, g_Boss[nCnt].Speed);

		// インパクトの当たり判定
		if (CollisionImpact(&g_Boss[nCnt].pos) == true)
		{
			HitBoss(nCnt, ImpactDamege(0));
		}
		
		// 範囲内にいる
		if (KeepInCylinder(&pPlayer->pos) == false)
		{
			// ボスの追跡処理の更新
			UpdateAgentBoss(nCnt);

			// 攻撃していない
			if (CheckActionMotion(&pPlayer->Motion) == false)
			{
				colisionSword(nCnt);   // 剣との当たり判定
			}
		}
		else
		{
			SetMotion(&g_Boss[nCnt].Motion, MOTIONTYPE_NEUTRAL, true, 10);
			g_Boss[nCnt].AttackState = BOSSATTACK_NO;
		}
		// 攻撃範囲に入ったら
		if (sphererange(&pPlayer->pos, &g_Boss[nCnt].pos, 50.0f, 50.0f) && g_Boss[nCnt].AttackState != BOSSATTACK_ATTACK)
		{
			// 行動パターンをランダム選択し
			g_Boss[nCnt].nAttackPattern = rand() % 2;

			// 攻撃状態の遷移
			switch (g_Boss[nCnt].nAttackPattern)
			{
			case 0:
				if (g_Boss[nCnt].Motion.motiontypeBlend != MOTIONTYPE_ACTION)
				{
					// モーションを攻撃にする
					SetMotion(&g_Boss[nCnt].Motion, MOTIONTYPE_ACTION, true, 10);
				}
				break;
			case 1:
				if (g_Boss[nCnt].Motion.motiontypeBlend != MOTIONTYPE_ACTION2)
				{
					// モーションを攻撃にする
					SetMotion(&g_Boss[nCnt].Motion, MOTIONTYPE_ACTION2, true, 10);
				}
				break;
			default:
				break;
			}

			g_Boss[nCnt].AttackState = BOSSATTACK_ATTACK; // 攻撃している
		}

		// ボスの攻撃判定範囲にいるかどうかを判定
		const bool is_sphereBounds = sphererange(&pPlayer->pos, &g_Boss[nCnt].pos, 50.0f, 20.0f) == true;

		// プレイヤーがダメージ状態じゃないかを判定
		const bool is_PlayerNotDamage = pPlayer->state != PLAYERSTATE_DAMAGE;

		// プレイヤーがスペシャルアタックをしてないかを判定
		const bool is_SpAttack = pPlayer->AttackSp == false;

		// 攻撃をできるかを判定
		const bool is_CanDamage = is_sphereBounds == true && is_PlayerNotDamage && is_PlayerNotDamage == true && is_SpAttack == true;

		// 攻撃範囲に入った
		if (is_CanDamage == true)
		{
			switch (g_Boss[nCnt].Motion.motiontypeBlend)
			{
			case MOTIONTYPE_ACTION:
				if (g_Boss[nCnt].Motion.nKey >= 3)
				{
					if (g_Boss[nCnt].nStateCount[3] > 0)
					{
						HitPlayer(30,true, nCnt, ATTACKER_BOSS);
					}
					else
					{
						HitPlayer(40,true, nCnt, ATTACKER_BOSS);
					}
				}
				break;
			case MOTIONTYPE_ACTION2:
				if (g_Boss[nCnt].Motion.nKey >= 2)
				{
					if (g_Boss[nCnt].nStateCount[3] > 0)
					{
						HitPlayer(30,true, nCnt, ATTACKER_BOSS);
					}
					else
					{
						HitPlayer(40,true, nCnt, ATTACKER_BOSS);
					}
				}
				break;
			default:
				break;
			}
		}


		if (EnableEvent() == false)
		{
			EndEventBossState(nCnt);
		}
		CollisionToBoss(nCnt); // ボスとボスの当たり判定
	}
}
//===============================================================================================================
// ボスの描画処理
//===============================================================================================================
void DrawBoss(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// 計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	D3DMATERIAL9 matDef; // 現在のマテリアル保存用

	D3DXMATERIAL* pMat;	 // マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		if (g_Boss[nCnt].bUse == false)
		{
			continue;
		}
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Boss[nCnt].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Boss[nCnt].rot.y, g_Boss[nCnt].rot.x, g_Boss[nCnt].rot.z);
		D3DXMatrixMultiply(&g_Boss[nCnt].mtxWorld, &g_Boss[nCnt].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Boss[nCnt].pos.x, g_Boss[nCnt].pos.y, g_Boss[nCnt].pos.z);
		D3DXMatrixMultiply(&g_Boss[nCnt].mtxWorld, &g_Boss[nCnt].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Boss[nCnt].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// 全モデルパーツの描画
		for (int nCntModel = 0; nCntModel < g_Boss[nCnt].Motion.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransform; // 計算用
			D3DXMATRIX mtxParent;				  // 親のマトリックス

			// パーツのマトリックスの初期化
			D3DXMatrixIdentity(&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld);

			// パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Boss[nCnt].Motion.aModel[nCntModel].rot.y, g_Boss[nCnt].Motion.aModel[nCntModel].rot.x, g_Boss[nCnt].Motion.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld, &g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

			// パーツの位置(オフセット)を反映
			D3DXMatrixTranslation(&mtxTransform, g_Boss[nCnt].Motion.aModel[nCntModel].pos.x, g_Boss[nCnt].Motion.aModel[nCntModel].pos.y, g_Boss[nCnt].Motion.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld, &g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

			// パーツの[親のマトリックス]を設定
			if (g_Boss[nCnt].Motion.aModel[nCntModel].nIdxModelParent != -1)
			{
				// 親モデルがある場合
				mtxParent = g_Boss[nCnt].Motion.aModel[g_Boss[nCnt].Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{// 親モデルがない場合
				mtxParent = g_Boss[nCnt].mtxWorld;
			}

			// 算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
			D3DXMatrixMultiply(&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld,
				&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld,
				&mtxParent); // 自分,自分,親

			// パーツのワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD,
				&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld);

			for (int nCntMat = 0; nCntMat < (int)g_Boss[nCnt].Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				// マテリアルのデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Boss[nCnt].Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

				// カラー変更用の変数
				D3DXMATERIAL color;

				if (g_Boss[nCnt].state != BOSSSTATE_DAMAGE)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				else if (g_Boss[nCnt].state == BOSSSTATE_DAMAGE)
				{
					// カラーを代入
					color = pMat[nCntMat];

					// カラーを変更
					color.MatD3D.Diffuse.r = 1.0f;
					color.MatD3D.Diffuse.g = 0.3f;
					color.MatD3D.Diffuse.b = 0.3f;
					color.MatD3D.Diffuse.a = 1.0f;

					// マテリアルの設定
					pDevice->SetMaterial(&color.MatD3D);
				}

				// テクスチャの設定
				pDevice->SetTexture(0, g_Boss[nCnt].Motion.aModel[nCntModel].pTexture[nCntMat]);

				// モデル(パーツ)の描画
				g_Boss[nCnt].Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}

		}
	}

	//// マテリアルをもとに戻す
	//pDevice->SetMaterial(&matDef);

	//DrawBossLife();
}
//===============================================================================================================
// ボスの設定処理
//===============================================================================================================
void SetBoss(D3DXVECTOR3 pos, float speed, int nLife)
{
	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		if (g_Boss[nCnt].bUse == false)
		{// 未使用なら
			g_Boss[nCnt].Motion = g_LoadBoss;         // 情報を代入
			g_Boss[nCnt].pos = pos;					  // 位置を代入
			//g_Boss.Speed = speed;					  // 足の速さ
			g_Boss[nCnt].nLife = nLife;				  // 体力を挿入
			g_Boss[nCnt].state = BOSSSTATE_NORMAL;	  // 状態
			g_Boss[nCnt].AttackState = BOSSATTACK_NO; // 攻撃状態
			g_Boss[nCnt].nMaxLife = nLife;            // 最大のHP
			SetMotion(&g_Boss[nCnt].Motion, MOTIONTYPE_NEUTRAL, true, 10);
			g_Boss[nCnt].move = NULLVECTOR3;

			// 状態異常関係
			for (int nCnt2 = 0; nCnt2 < 5; nCnt2++)
			{
				g_Boss[nCnt].nStateCharge[nCnt2] = 0;
				g_Boss[nCnt].nStateCount[nCnt2] = 0;
			}

			// 使用状態にする
			g_Boss[nCnt].bUse = true;   

			// ボスのモデルの位置を代入
			D3DXVECTOR3 BossPos(g_Boss[nCnt].mtxWorld._41, g_Boss[nCnt].mtxWorld._42, g_Boss[nCnt].mtxWorld._43);

			// ポリゴンをセット
			SetPolygon(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20.0f, 100.0f, POLYGON_TYPE_FIVE);

			// 影をセット
			g_Boss[nCnt].nIdxShadow = SetShadow(BossPos, D3DXVECTOR3(0.0f,0.0f,0.0f), 40.0f,1.0f);

			// ミニマップにボスアイコンを設定
			g_Boss[nCnt].nIdxMap = SetMiniMap(BossPos, MINIMAPTEX_BOSS);

			// ボスのライフゲージの設定
			g_Boss[nCnt].nLifeDelayIdx = SetBossLife(pos, BOSSTEX_DELAY);
			g_Boss[nCnt].nLifeBarIdx = SetBossLife(pos,BOSSTEX_LIFE);
			g_Boss[nCnt].nLifeFrame = SetBossLife(pos, BOSSTEX_FRAME);

			break;
		}
	}
}
//===============================================================================================================
// ボスのヒット処理
//===============================================================================================================
void HitBoss(int nCntBoss,int nDamage)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// アイテムの取得
	Item* pItem = GetItem();

	if (g_Boss[nCntBoss].nStateCount[4] > 0)
	{
		// HPを減らす
		g_Boss[nCntBoss].nLife -= nDamage * 1.5f;

		// ダメージを設定
		SetDamege(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z), // 位置
			nDamage * 1.5f,	// ダメージ																								
			20,			// 寿命
			false);
	}
	else
	{
		// HPを減らす
		g_Boss[nCntBoss].nLife -= nDamage;

		// ダメージを設定
		SetDamege(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z), // 位置
			nDamage,	// ダメージ																								
			20,			// 寿命
			false);
	}

	GAMESTATE gamestate = GetGameState();

	// ボスが死んだ
	if (g_Boss[nCntBoss].nLife <= 0 && g_Boss[nCntBoss].Motion.motiontypeBlend != MOTIONTYPE_DEATH)
	{
		// ゲージを消す
		DeleateLifeBar(g_Boss[nCntBoss].nLifeBarIdx, g_Boss[nCntBoss].nLifeFrame, g_Boss[nCntBoss].nLifeDelayIdx);

		// 死んだらパーティクルを出す(雑魚より派手に)
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z),
			g_Boss[nCntBoss].rot,
			D3DXVECTOR3(3.14f, 3.14f, 3.14f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			7.0f, 40, 60, 20, 7.0f, 20.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// モーションがDeathじゃなかったら
		if (g_Boss[nCntBoss].Motion.motiontypeBlend != MOTIONTYPE_DEATH)
		{
			SetMotion(&g_Boss[nCntBoss].Motion, MOTIONTYPE_DEATH, true, 10);
		}

		g_Boss[nCntBoss].nLife = -1;

		if (gamestate != GAMESTATE_END)
		{
			if (pPlayer->FeverMode)
			{
				AddScore(30000);		// スコアを取得
				AddSpgauge(10.5f);   // SPゲージを取得
			}
			else if (!pPlayer->FeverMode)
			{
				AddFever(10.0f);		// フィーバーポイントを取得
				AddScore(15000);		// スコアを取得
				AddSpgauge(2.0f);       // SPゲージを取得
			}
		}


		// 通常武器が当たった時のサウンド
		SetSoundWepon(pItem[pPlayer->ItemIdx].nType);

		// 合成武器当たった時のサウンド
		SetCreateWeponSound(pItem[pPlayer->ItemIdx].nType);

		return;
	}
	else
	{
		HitBossAbnormalCondition(nCntBoss);

		// 通常武器が当たった時のサウンド
		SetSoundWepon(pItem[pPlayer->ItemIdx].nType);

		// 合成武器当たった時のサウンド
		SetCreateWeponSound(pItem[pPlayer->ItemIdx].nType);

		AddFever(10.0f);		// フィーバーポイントを取得

		// パーティクルをセット
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z), g_Boss[nCntBoss].rot, D3DXVECTOR3(3.14f, 3.14f, 3.14f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), 4.0f, 1, 20, 10, 8.0f, 0.0f, false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		g_Boss[nCntBoss].state = ENEMYSTATE_DAMAGE;
		g_Boss[nCntBoss].nCounterState = 20;
		g_Boss[nCntBoss].nHitStopCount = 8;
		AddSpgauge(1.0f);   // SPゲージを取得
	}
}
//===============================================================================================================
// ボスの読み込み処理
//===============================================================================================================
void LoadBoss(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// ファイルポインタを宣言
	FILE* pFile; 

	// ファイルを開く
	pFile = fopen("data\\MOTION\\boss.txt", "r");

	// ローカル変数宣言-----------------------------
	int nCntKey = 0;
	int nCntPosKey = 0;
	int nCntRotkey = 0;
	int nCnt = 0;
	int nNumParts = 0;
	int nIdx = 0;
	int nScanData = 0;
	int nNumModel = 0;
	char Skip[3] = {};
	int nLoadCnt = 0;

	//----------------------------------------------

	if (pFile != NULL)
	{//　NULL じゃない
		// 文字列
		char aStr[MAX_WORD];

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
				g_LoadBoss.nNumModel = nNumModel;	// モデルの最大数を代入
			}

			// モデルの読み込みがまだ終わっていなかったら
			if (nLoadCnt < nNumModel)
			{
				// モデルの読み込んだ数を返す
				nLoadCnt = LoadBossFilename(pFile, nNumModel, &aStr[0], 0);
			}

			// CHARACTERSETを読み取ったら
			if (strcmp(&aStr[0], "CHARACTERSET") == 0)
			{
				// パーツの設定処理
				LoadBossCharacterSet(pFile, &aStr[0], nNumParts, 0);
			}

			// MOTIONSETを読み取ったら
			if (strcmp(&aStr[0], "MOTIONSET") == 0)
			{
				// モーションの設定処理
				LoadBossMotionSet(pFile, &aStr[0], nNumModel, 0);
			}

			// END_SCRIPTを読み取ったら
			if (nData == EOF)
			{
				nCntMotionBoss = 0; // モーションのカウントをリセットする
				break;          // While文を抜ける
			}
		}// while文末
	}
	else
	{
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(Boss.cpp)", MB_OK);
		return;
	}
	// ファイルを閉じる
	fclose(pFile);
}
//===========================================================================================================
// ボスのモデルのロード処理
//===========================================================================================================
int LoadBossFilename(FILE* pFile, int nNumModel, char* aString, int nType)
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
				&g_LoadBoss.aModel[nCntLoadModel].pBuffMat,
				NULL,
				&g_LoadBoss.aModel[nCntLoadModel].dwNumMat,
				&g_LoadBoss.aModel[nCntLoadModel].pMesh);

			//マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_LoadBoss.aModel[nCntLoadModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadBoss.aModel[nCntLoadModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//このファイル名を使用してテクスチャを読み込む
						//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_LoadBoss.aModel[nCntLoadModel].pTexture[nCntMat]);
				}
			}

			//頂点数の取得
			nNumVtx = g_LoadBoss.aModel[nCntLoadModel].pMesh->GetNumVertices();

			//頂点フォーマットのサイズ取得
			sizeFVF = D3DXGetFVFVertexSize(g_LoadBoss.aModel[nCntLoadModel].pMesh->GetFVF());

			//頂点バッファのロック
			g_LoadBoss.aModel[nCntLoadModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//頂点座標を比較してブロックの最小値,最大値を取得
				if (vtx.x < g_LoadBoss.aModel[nCntLoadModel].vtxMin.x)
				{
					g_LoadBoss.aModel[nCntLoadModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_LoadBoss.aModel[nCntLoadModel].vtxMin.y)
				{
					g_LoadBoss.aModel[nCntLoadModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_LoadBoss.aModel[nCntLoadModel].vtxMin.z)
				{
					g_LoadBoss.aModel[nCntLoadModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_LoadBoss.aModel[nCntLoadModel].vtxMax.x)
				{
					g_LoadBoss.aModel[nCntLoadModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_LoadBoss.aModel[nCntLoadModel].vtxMax.y)
				{
					g_LoadBoss.aModel[nCntLoadModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_LoadBoss.aModel[nCntLoadModel].vtxMax.z)
				{
					g_LoadBoss.aModel[nCntLoadModel].vtxMax.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			////サイズを代入
			//g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			//g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			//g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

			//各モデルごとのサイズを代入
			g_LoadBoss.aModel[nCntLoadModel].Size.x = g_LoadBoss.aModel[nCntLoadModel].vtxMax.x - g_LoadBoss.aModel[nCntLoadModel].vtxMin.x;
			g_LoadBoss.aModel[nCntLoadModel].Size.y = g_LoadBoss.aModel[nCntLoadModel].vtxMax.y - g_LoadBoss.aModel[nCntLoadModel].vtxMin.y;
			g_LoadBoss.aModel[nCntLoadModel].Size.z = g_LoadBoss.aModel[nCntLoadModel].vtxMax.z - g_LoadBoss.aModel[nCntLoadModel].vtxMin.z;

			//頂点バッファのアンロック
			g_LoadBoss.aModel[nCntLoadModel].pMesh->UnlockVertexBuffer();

			nCntLoadModel++; // モデルのカウントを増やす
		}
	}
	return nCntLoadModel; // モデルのカウントを返す
}
//===========================================================================================================
// ボスのパーツの設定処理
//===========================================================================================================
void LoadBossCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType)
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
			nScanData = fscanf(pFile, "%d", &g_LoadBoss.aModel[nIdx].nIdxModelParent);
		}
		// POSを読み取ったら
		else if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offpos.x);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offpos.y);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offpos.z);
		}
		// ROTを読み取ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// モデルのオフセットを代入
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offrot.x);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offrot.y);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offrot.z);
		}
	}
}
//===========================================================================================================
// ボスのモーションのロード処理
//===========================================================================================================
void LoadBossMotionSet(FILE* pFile, char* aString, int nNumModel, int nType)
{
	char Skip[3] = {}; // [=]読み飛ばし変数
	int nScanData = 0;          // 戻り値代入用

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
			nScanData = fscanf(pFile, "%d", &g_LoadBoss.aMotionInfo[nCntMotionBoss].bLoop);
		}
		// NUM_KEYを読み通ったら
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// キーの最大数を代入
			nScanData = fscanf(pFile, "%d", &g_LoadBoss.aMotionInfo[nCntMotionBoss].nNumkey);
		}
		// KEYSETを読み通ったら
		if (strcmp(aString, "KEYSET") == 0)
		{
			// キーの設定処理
			LoadBossKeySet(pFile, aString, nType, nCntMotionBoss);
		}
		// END_MOTIONSETを読み通ったら
		if (strcmp(aString, "END_MOTIONSET") == 0)
		{
			nCntMotionBoss++; // モーションのカウントをリセット
			nKeyBoss = 0;     // キーをリセット
			break;
		}
	}
}
//===========================================================================================================
// ボスのモーションのキーの読み込み処理
//===========================================================================================================
void LoadBossKeySet(FILE* pFile, char* aString, int nType, int nCntMotion)
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
			nScanData = fscanf(pFile, "%d", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].nFrame);
		}

		// POSを読み通ったら
		if (strcmp(aString, "POS") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// 位置を読み込む
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].aKey[nCntPos].fPosX);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].aKey[nCntPos].fPosY);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROTを読み通ったら
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]読み飛ばす
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// 角度を読み込む
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].aKey[nCntRot].fRotX);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].aKey[nCntRot].fRotY);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].aKey[nCntRot].fRotZ);
			nCntRot++;
		}
		// END_KEYSETを読み通ったら
		else if (strcmp(aString, "END_KEYSET") == 0)
		{
			nKeyBoss++;		 // キーを増やす
			nCntPos = 0; // 位置のカウントをリセット
			nCntRot = 0; // 角度のカウントをリセット
			break;
		}
	}
}
//===========================================================================================================
// 剣の当たり判定
//===========================================================================================================
void colisionSword(int nCntBoss)
{
	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	bool bHit = false;

	// 球の半径
	float Radius = 50.0f;

	// 剣先の位置
	D3DXVECTOR3 SwordTopPos(pPlayer->SwordMtx._41, pPlayer->SwordMtx._42, pPlayer->SwordMtx._43);

	// 剣の持ち手の位置
	D3DXVECTOR3 SwordUnderPos(pPlayer->Motion.aModel[15].mtxWorld._41, pPlayer->Motion.aModel[15].mtxWorld._42, pPlayer->Motion.aModel[15].mtxWorld._43);

	for (int nCntModel = 0; nCntModel < g_Boss[nCntBoss].Motion.nNumModel; nCntModel++)
	{
		// 敵の位置
		D3DXVECTOR3 BossModel(g_Boss[nCntBoss].Motion.aModel[nCntModel].mtxWorld._41, g_Boss[nCntBoss].Motion.aModel[nCntModel].mtxWorld._42, g_Boss[nCntBoss].Motion.aModel[nCntModel].mtxWorld._43);

		// 線分の終点から球体の中心点までのベクトル
		D3DXVECTOR3 pvEnd = SwordTopPos - BossModel;

		// 線分の始点から球体の中心点までのベクトル
		D3DXVECTOR3 pvFirst = SwordUnderPos - BossModel;

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

	// モーションのキー
	int nKey = pPlayer->Motion.nKey;

	// モーションのカウンター
	int nCounter = pPlayer->Motion.nCountMotion;

	// 最後のキー
	int LastKey = pPlayer->Motion.aMotionInfo[pPlayer->Motion.motionType].nNumkey - 1;

	// 最後のフレーム
	int EndFrame = pPlayer->Motion.aMotionInfo[pPlayer->Motion.motionType].aKeyInfo[LastKey].nFrame;

	// プレイヤーが武器を持っているか判定
	const bool is_HaveWepon = pPlayer->Motion.nNumModel == MAX_MODEL;

	// プレイヤーがスペシャル状態じゃないか判定
	const bool is_NotSpAttack = pPlayer->AttackSp == false;

	// ボスの状態がダメージじゃなかったら
	const bool is_NotDamage = g_Boss[nCntBoss].state != BOSSSTATE_DAMAGE;

	// プレイヤーが攻撃状態だったら
	const bool is_playerAction = isPlayerAttaking() == true;

	// 攻撃できる
	const bool CanSwordDamage = is_HaveWepon == true && is_NotSpAttack == true && is_NotDamage == true && is_playerAction == true;

	// ダメージカット
	static int fCutDamage = 1;

	// クラフト武器だったら
	if (strcmp(&pItem[pPlayer->ItemIdx].Itemtag[0], "CRAFT") == 0)
	{
		// ダメージカットの割合
		fCutDamage = 1;
	}
	// 普通の武器だったら
	else if (strcmp(&pItem[pPlayer->ItemIdx].Itemtag[0], "NORMAL") == 0)
	{
		// ダメージカットの割合
		fCutDamage = 5;
	}

	// スペシャル攻撃じゃない
	if (CanSwordDamage == true && bHit == true && CheckMotionBounds(nKey,nCounter,KEY_ONE,LastKey,0,EndFrame) == true)
	{
		HitBoss(nCntBoss, (pPlayer->nDamage * 5) / fCutDamage);

		pItem[pPlayer->ItemIdx].durability--;

		if (pItem[pPlayer->ItemIdx].durability <= 0)
		{
			pPlayer->Itembreak[pPlayer->ItemIdx] = true;
		}
	}

	// モデルの位置を変数に代入
	D3DXVECTOR3 ModelPos(pPlayer->Motion.aModel[4].mtxWorld._41, pPlayer->Motion.aModel[4].mtxWorld._42, pPlayer->Motion.aModel[4].mtxWorld._43);

	// 武器を持っていないかを判定
	const bool is_NotWepon = pPlayer->Motion.nNumModel == MAX_MODEL - 1;

	// 円の範囲内にいるかどうかを判定
	const bool is_sphereBounds = sphererange(&ModelPos, &g_Boss[nCntBoss].pos, 30.0f, 65.0f) == true;

	// ダメージを与えられるかを判定
	const bool CanPanchDamage = is_NotWepon == true && is_sphereBounds == true && is_NotDamage == true && is_playerAction == true;

	// 素手で攻撃できる
	if (CanPanchDamage == true && CheckMotionBounds(nKey, nCounter, KEY_TWO, LastKey, 0, EndFrame) == true)
	{
		HitBoss(nCntBoss,(pPlayer->nDamage * 3) / fCutDamage); // 敵に当たった
	}

	// 範囲内
	const bool InBounds = sphererange(&ModelPos, &g_Boss[nCntBoss].pos, 150.0f, 65.0f) == true;

	// ダメージを与えられる
	const bool is_CanSpDamage = pPlayer->Motion.nNumModel == MAX_MODEL && pPlayer->AttackSp == true && InBounds;

	// 刀のスペシャル攻撃
	if (is_CanSpDamage == true && pPlayer->WeponMotion == MOTION_SP && CheckMotionBounds(nKey, nCounter, KEY_THREE, LastKey, 0, EndFrame / 2) == true)
	{
		HitBoss(nCntBoss, (pPlayer->nDamage * 15) / fCutDamage); // 敵に当たった
	}
	// 両手持ちの必殺技
	if (is_CanSpDamage == true && pPlayer->WeponMotion == MOTION_SPDOUBLE && CheckMotionBounds(nKey, nCounter, KEY_FOUR, LastKey, 0, EndFrame / 2) == true)
	{
		HitBoss(nCntBoss, (pPlayer->nDamage * 15) / fCutDamage); // 敵に当たった
	}
	// ハンマーのスペシャル攻撃
	if (is_CanSpDamage == true && pPlayer->WeponMotion == MOTION_SPHAMMER && CheckMotionBounds(nKey, nCounter, KEY_ONE, LastKey, 0, EndFrame / 2) == true)
	{
		HitBoss(nCntBoss, (pPlayer->nDamage * 15) / fCutDamage); // 敵に当たった
	}
	// 片手の必殺技
	if (is_CanSpDamage == true && pPlayer->WeponMotion == MOTION_ONEHANDBLOW && CheckMotionBounds(nKey, nCounter, KEY_FOUR, LastKey, 0, EndFrame / 2) == true)
	{
		HitBoss(nCntBoss, (pPlayer->nDamage * 15) / fCutDamage); // 敵に当たった
	}
	// 槍の必殺技
	if (is_CanSpDamage == true && pPlayer->WeponMotion == MOTION_SPPIERCING && CheckMotionBounds(nKey, nCounter, KEY_EIGHTEEN, LastKey, 0, EndFrame / 2) == true)
	{
		HitBoss(nCntBoss, (pPlayer->nDamage * 15) / fCutDamage); // 敵に当たった
	}

}
//===========================================================================================================
// ボスとボスの当たり判定
//==========================================================================================================
void CollisionToBoss(int nCntBoss)
{
	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		if (g_Boss[nCnt].bUse && nCntBoss != nCnt)
		{
			if (sphererange(&g_Boss[nCntBoss].pos, &g_Boss[nCnt].pos, 50.0f, 50.0f))
			{
				D3DXVECTOR3 vector = g_Boss[nCnt].pos - g_Boss[nCntBoss].pos;

				D3DXVec3Normalize(&vector, &vector);
				g_Boss[nCntBoss].move.x -= vector.x * g_Boss[nCntBoss].Speed;
				g_Boss[nCntBoss].move.z -= vector.z * g_Boss[nCntBoss].Speed;
			}
		}
	}
}
//========================================================================================================
// 状態異常の設定
//========================================================================================================
bool SetAbnormalCondition(int nType, int nTime, int nDamage,int nCntBoss)
{			
	bool is_SetParticle = false;

	if (g_Boss[nCntBoss].nStateCount[nType] > 0)
	{
		// 状態異常出血の処理
		g_Boss[nCntBoss].nStateCount[nType]--;

		is_SetParticle = true;
		if (g_Boss[nCntBoss].nStateCount[nType] % nTime == 0)
		{
			if (nDamage > 0)
			{
				HitBoss(nCntBoss, nDamage);
			}
		}
	}
	return is_SetParticle;
}
//========================================================================================================
// 状態異常の更新処理
//========================================================================================================
void UpdateAbnormalCondition(int nCntBoss)
{
	if (SetAbnormalCondition(0, 60, g_Boss[nCntBoss].nLife / 20, nCntBoss) == true)
	{
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + 50.0f, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			2.0f, 2, 30, 6, 10.0f, 20.0f, true, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	}

	// 状態異常炎の処理
	if (SetAbnormalCondition(1, 60, g_Boss[nCntBoss].nLife / 20, nCntBoss) == true)
	{
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
			2.0f, 2, 30, 7, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 2.0f, 0.0f));
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f),
			2.0f, 2, 30, 7, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 2.0f, 0.0f));
	}

	// 状態異常氷の処理
	if (SetAbnormalCondition(2, 60, 0, nCntBoss) == true)
	{
		// ここで半分くらいのペースでモーションカウントを減らし、実質モーション速度半減にする
		if (g_Boss[nCntBoss].nStateCount[2] % 2 == 0)
		{
			g_Boss[nCntBoss].Motion.nCountMotion--;
		}
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + 50.0f, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			1.0f, 2, 60, 7, 1.0f, 25.0f, false, D3DXVECTOR3(0.0f, 4.0f, 0.0f));

		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + 50.0f, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.6f, 1.0f, 1.0f),
			1.0f, 2, 60, 7, 1.0f, 25.0f, false, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	}

	// 状態異常雷の処理
	if (SetAbnormalCondition(3, 60, 0, nCntBoss) == true)
	{
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + 50.0f, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
			1.0f, 2, 120, 6, 1.0f, 25.0f, false, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	}

	// 状態異常水の処理
	if (SetAbnormalCondition(4, 60, 0, nCntBoss) == true)
	{
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + 50.0f, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
			2.0f, 2, 30, 6, 10.0f, 20.0f, true, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	}
}
//========================================================================================================
// ボスの突進攻撃の設定
//========================================================================================================
void SetRasuAttack(int nCntBoss)
{
	Player* pPlayer = GetPlayer();

	if (g_Boss[nCntBoss].Motion.nKey == KEY_FIRST || g_Boss[nCntBoss].Motion.nKey == KEY_ONE)// 溜める動作中に
	{
		// 向きをプレイヤーに向ける
		float fAngle = atan2f(pPlayer->posOld.x - g_Boss[nCntBoss].pos.x, pPlayer->posOld.z - g_Boss[nCntBoss].pos.z);

		// ボスの向き代入
		g_Boss[nCntBoss].rot.y = fAngle + D3DX_PI;
	}
	else if (g_Boss[nCntBoss].Motion.nKey == KEY_TWO)// 溜まり切ったモーションになって
	{
		if (g_Boss[nCntBoss].Motion.nCountMotion == 1)// その最初にエフェクトを出す
		{
			SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z),
				D3DXVECTOR3(1.57f, g_Boss[nCntBoss].rot.y, 1.57f),
				D3DXVECTOR3(0.2f, 3.14f, 0.2f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
				2.0f, 3, 20, 30, 4.0f, 50.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	else if (g_Boss[nCntBoss].Motion.nKey == KEY_THREE && g_Boss[nCntBoss].Motion.bFirstMotion == false)// 突進モーション中の			
	{
		if (g_Boss[nCntBoss].Motion.nCountMotion == 1)// 最初は加速させて
		{
			g_Boss[nCntBoss].move.x = sinf(g_Boss[nCntBoss].rot.y + D3DX_PI) * 40.0f;
			g_Boss[nCntBoss].move.z = cosf(g_Boss[nCntBoss].rot.y + D3DX_PI) * 40.0f;
		}
		else // その後はエフェクトを纏いながら移動する
		{
			g_Boss[nCntBoss].pos += g_Boss[nCntBoss].move;
			SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z),
				g_Boss[nCntBoss].rot,
				D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
				2.0f, 4, 60, 30, 6.0f, 60.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
}
//========================================================================================================
// ボスの二回突進してくる攻撃処理
//========================================================================================================
void SetDoubleRasuAttack(int nCntBoss)
{
	Player* pPlayer = GetPlayer();

	if (g_Boss[nCntBoss].Motion.nKey == KEY_FIRST || g_Boss[nCntBoss].Motion.nKey == KEY_FOUR)// 溜める動作中に
	{
		// 向きをプレイヤーに向ける
		float fAngle = atan2f(pPlayer->posOld.x - g_Boss[nCntBoss].pos.x, pPlayer->posOld.z - g_Boss[nCntBoss].pos.z);

		// ボスの向き代入
		g_Boss[nCntBoss].rot.y = fAngle + D3DX_PI;
	}
	else if (g_Boss[nCntBoss].Motion.nKey == KEY_ONE)// 溜まり切ったモーションになって
	{
		if (g_Boss[nCntBoss].Motion.nCountMotion == 1)// その最初にエフェクトを出す
		{
			SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z),
				D3DXVECTOR3(1.57f, g_Boss[nCntBoss].rot.y, 1.57f),
				D3DXVECTOR3(0.2f, 3.14f, 0.2f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
				2.0f, 3, 20, 55, 4.0f, 50.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	else if (g_Boss[nCntBoss].Motion.nKey == KEY_SIX)// ハイキックモーション中の			
	{
		if (g_Boss[nCntBoss].Motion.nCountMotion == 1)// 最初は加速させる
		{
			g_Boss[nCntBoss].move.x = sinf(g_Boss[nCntBoss].rot.y + D3DX_PI) * 30.0f;
			g_Boss[nCntBoss].move.z = cosf(g_Boss[nCntBoss].rot.y + D3DX_PI) * 30.0f;
		}
		else
		{
			SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 2.0f, g_Boss[nCntBoss].pos.z),
				g_Boss[nCntBoss].rot,
				D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
				2.0f, 4, 60, 40, 6.0f, 60.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	else if (g_Boss[nCntBoss].Motion.nKey == KEY_TWO || g_Boss[nCntBoss].Motion.nKey == KEY_THREE)//アッパー中にはエフェクトを出す
	{
		D3DXVECTOR3 BlowPos(
			g_Boss[nCntBoss].Motion.aModel[5].mtxWorld._41, // X方向
			g_Boss[nCntBoss].Motion.aModel[5].mtxWorld._42, // Y方向
			g_Boss[nCntBoss].Motion.aModel[5].mtxWorld._43  // Z方向
		);

		g_Boss[nCntBoss].pos += g_Boss[nCntBoss].move;
		SetParticle(D3DXVECTOR3(BlowPos),
			g_Boss[nCntBoss].rot,
			D3DXVECTOR3(3.14f, 3.14f, 3.14f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			2.0f, 4, 20, 12, 2.0f, 5.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		if (g_Boss[nCntBoss].Motion.nCountMotion == 1 && g_Boss[nCntBoss].Motion.nKey == KEY_TWO)
		{
			g_Boss[nCntBoss].move.x = sinf(g_Boss[nCntBoss].rot.y + D3DX_PI) * 20.0f;
			g_Boss[nCntBoss].move.z = cosf(g_Boss[nCntBoss].rot.y + D3DX_PI) * 20.0f;
		}
	}
	else if (g_Boss[nCntBoss].Motion.nKey >= KEY_SIX)//蹴り中にもエフェクトを出す
	{
		D3DXVECTOR3 BlowPos(
			g_Boss[nCntBoss].Motion.aModel[11].mtxWorld._41, // X方向
			g_Boss[nCntBoss].Motion.aModel[11].mtxWorld._42, // Y方向
			g_Boss[nCntBoss].Motion.aModel[11].mtxWorld._43  // Z方向
		);

		g_Boss[nCntBoss].pos += g_Boss[nCntBoss].move;
		SetParticle(D3DXVECTOR3(BlowPos),
			g_Boss[nCntBoss].rot,
			D3DXVECTOR3(3.14f, 3.14f, 3.14f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			2.0f, 4, 20, 12, 2.0f, 5.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
}
//========================================================================================================
// ボスの追跡の更新処理
//========================================================================================================
void UpdateAgentBoss(int nCntBoss)
{
	Player* pPlayer = GetPlayer();

	// 範囲に入ったら(どこにいても追いかけてくるが一応円で取る)
	const bool is_sphereBounds = sphererange(&pPlayer->pos, &g_Boss[nCntBoss].pos, 50.0f, 700.0f) == true;

	// 攻撃モーション0かどうかを判定
	const bool is_NotAction0 = g_Boss[nCntBoss].Motion.motiontypeBlend != MOTIONTYPE_ACTION;

	// 攻撃モーション1かどうかを判定
	const bool is_NotAction1 = g_Boss[nCntBoss].Motion.motiontypeBlend != MOTIONTYPE_ACTION2;

	// 攻撃モーション2かどうかを判定
	const bool is_NotAction2 = g_Boss[nCntBoss].Motion.motiontypeBlend != MOTIONTYPE_ACTION3;

	// 追跡できるかどうかを判定
	const bool CanAgent = is_sphereBounds == true && is_NotAction0 == true && is_NotAction1 == true && is_NotAction2 == true;

	if (CanAgent == true)
	{
		// モデル情報を代入
		D3DXVECTOR3 HootR(g_Boss[nCntBoss].Motion.aModel[11].mtxWorld._41, g_Boss[nCntBoss].Motion.aModel[11].mtxWorld._42, g_Boss[nCntBoss].Motion.aModel[11].mtxWorld._43);
		D3DXVECTOR3 HootL(g_Boss[nCntBoss].Motion.aModel[14].mtxWorld._41, g_Boss[nCntBoss].Motion.aModel[14].mtxWorld._42, g_Boss[nCntBoss].Motion.aModel[14].mtxWorld._43);

		// モーションがムーブの時1キーの1フレーム目
		if (g_Boss[nCntBoss].Motion.motiontypeBlend == MOTIONTYPE_MOVE &&
			g_Boss[nCntBoss].Motion.nKey == 1 &&
			g_Boss[nCntBoss].Motion.nCountMotion == 1)
		{
			SetExplosion(HootR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 40.0f, 40.0f, EXPLOSION_MOVE);
		}

		// モーションがムーブの時3キーの1フレーム目
		else if (g_Boss[nCntBoss].Motion.motiontypeBlend == MOTIONTYPE_MOVE &&
			g_Boss[nCntBoss].Motion.nKey == 3 &&
			g_Boss[nCntBoss].Motion.nCountMotion == 1)
		{
			SetExplosion(HootL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 40.0f, 40.0f, EXPLOSION_MOVE);
		}

		if (g_Boss[nCntBoss].Motion.motiontypeBlend != MOTIONTYPE_MOVE)
		{
			// モーションをムーブにする
			SetMotion(&g_Boss[nCntBoss].Motion, MOTIONTYPE_MOVE, true, 10);
		}

		// ボスの向きをプレイヤーの位置を向くようにする
		float fAngle = atan2f(pPlayer->pos.x - g_Boss[nCntBoss].pos.x, pPlayer->pos.z - g_Boss[nCntBoss].pos.z);

		// ボスの向き代入
		g_Boss[nCntBoss].rot.y = fAngle + D3DX_PI;

		// プレイヤーの位置を算出
		D3DXVECTOR3 Dest = pPlayer->pos - g_Boss[nCntBoss].pos;

		// 正規化
		D3DXVec3Normalize(&Dest, &Dest);

		// 移動量に代入
		if (g_Boss[nCntBoss].nStateCount[2] > 0)
		{
			g_Boss[nCntBoss].move.x = Dest.x * g_Boss[nCntBoss].Speed / 2.0f;
			g_Boss[nCntBoss].move.z = Dest.z * g_Boss[nCntBoss].Speed / 2.0f;
		}
		else
		{
			g_Boss[nCntBoss].move.x = Dest.x * g_Boss[nCntBoss].Speed;
			g_Boss[nCntBoss].move.z = Dest.z * g_Boss[nCntBoss].Speed;
		}
	}
	//else
	//{
	//	if (g_Boss[nCntBoss].Motion.motionType != MOTIONTYPE_ACTION)
	//	{
	//		g_Boss[nCntBoss].Motion.motionType = MOTIONTYPE_NEUTRAL; // 攻撃してない
	//	}
	//}


}
//========================================================================================================
 // ボスの死亡モーションの処理
 //========================================================================================================
void DeathMotionContlloer(int nCntBoss)
{
	Player* pPlayer = GetPlayer();

	// キーが最後まで行ったかを判定
	const bool LastKey = g_Boss[nCntBoss].Motion.nKey >= g_Boss[nCntBoss].Motion.aMotionInfo[MOTIONTYPE_DEATH].nNumkey - 1;

	// 死亡モーションだったら
	if (g_Boss[nCntBoss].Motion.nKey <= 0)
	{
		g_Boss[nCntBoss].move.x = sinf(pPlayer->rot.y + D3DX_PI) * 20.0f;
		g_Boss[nCntBoss].move.z = cosf(pPlayer->rot.y + D3DX_PI) * 20.0f;
	}

	// 最後にキーになったら
	if (LastKey == true)
	{
		// 消す
		g_Boss[nCntBoss].bUse = false;

		// マップから消す
		EnableMap(g_Boss[nCntBoss].nIdxMap);

		// 影消す
		KillShadow(g_Boss[nCntBoss].nIdxShadow);

		AddTimeSecond(20); // 15秒増やす

		// テリトリーを消す
		SetEnableBossTerritory(false);

		SpawnItem(g_Boss[nCntBoss].pos);
	}
}
//========================================================================================================
// イベントが終わった後にボスを消す処理
//========================================================================================================
void EndEventBossState(int nCntBoss)
{
}
//========================================================================================================
// ボスに当たった時の状態異常
//========================================================================================================
void HitBossAbnormalCondition(int nCntBoss)
{
	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	// 属性のパラメータ設定
	switch (pItem[pPlayer->ItemIdx].nType)
	{
	case ITEMTYPE_STONEBAT:
		//石バットなら出血特殊効果を与える
		HitBossAbnormalConditionParam(nCntBoss, 0, 25, 100, 300);
		break;
	case ITEMTYPE_GOLFHUNMER:
		//ゴルフハンマーなら出血特殊効果を与える
		HitBossAbnormalConditionParam(nCntBoss, 0, 25, 100, 300);
		break;
	case ITEMTYPE_IRONBAT:
		//金属バットなら出血特殊効果を与える
		HitBossAbnormalConditionParam(nCntBoss, 0, 25, 100, 300);
		break;
	case ITEMTYPE_TORCHSWORD:
		//猛火剣なら炎特殊効果を与える
		HitBossAbnormalConditionParam(nCntBoss, 1, 25, 100, 300);
		break;
	case ITEMTYPE_ICEBLOCKSOWRD:
		// 凍結剣なら氷特殊効果を与える
		HitBossAbnormalConditionParam(nCntBoss, 2, 25, 100, 300);
		break;
	case ITEMTYPE_LIGHTWOOD:
		// 雷撃剣なら雷特殊効果を与える
		HitBossAbnormalConditionParam(nCntBoss, 3, 25, 100, 300);
		break;
	case ITEMTYPE_SURFBOARDFISH:
		// 鮫浮き輪なら水特殊効果を与える
		HitBossAbnormalConditionParam(nCntBoss, 4, 25, 100, 300);
		break;
	case ITEMTYPE_BONESPEAR:
		//骨槍なら確率で即死効果を与える
		if (rand() % 40 == 0)
		{
			HitBoss(nCntBoss, 99999);
		}
		break;
	case ITEMTYPE_HEXMANDOLIN:
		//ダークハープなら確率で即死効果を与える
		if (rand() % 32 == 0)
		{
			HitBoss(nCntBoss, 99999);
		}
		break;
	default:
		break;
	}
}
//========================================================================================================
// ボスに当たった時の状態異常のパラメータ
//========================================================================================================
void HitBossAbnormalConditionParam(int nCntBoss,int nElement,int ChargeValue,int MaxCharge,int stateCnt)
{
	// 状態カウントが0以下だったら
	if (g_Boss[nCntBoss].nStateCount[nElement] <= 0)
	{
		// 状態異常値を加算
		g_Boss[nCntBoss].nStateCharge[nElement] += ChargeValue;

		// 蓄積値が最大になったら
		if (g_Boss[nCntBoss].nStateCharge[nElement] >= MaxCharge)
		{
			// 状態異常の時間を設定
			g_Boss[nCntBoss].nStateCount[nElement] = stateCnt;

			// 蓄積値を0に戻す
			g_Boss[nCntBoss].nStateCharge[nElement] = 0;
		}
	}
}
//=======================================================================================================
// アイテムのスポーン
//=======================================================================================================
void SpawnItem(D3DXVECTOR3 pos)
{
	// アイテムを設定
	//SetItem(pos, rand() % ITEMTYPE_MAX);
}
//========================================================================================================
// ボスの取得処理
//========================================================================================================
Boss* Getboss(void)
{
	return &g_Boss[0];
}
