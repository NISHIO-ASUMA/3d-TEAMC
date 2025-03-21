//==============================================================================================================
//
// ボス [boss.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _BOSS_H_
#define _BOSS_H_

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include"main.h"
#include"motion.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define MAX_BOSS (10)

//**************************************************************************************************************
//ボスの攻撃状態
//**************************************************************************************************************
typedef enum
{
	BOSSATTACK_NO = 0,
	BOSSATTACK_ATTACK,
	BOSSATTACK_MAX
}BOSSATTACK;

//**************************************************************************************************************
//ボスの状態
//**************************************************************************************************************
typedef enum
{
	BOSSSTATE_NORMAL = 0,
	BOSSSTATE_AGENT,
	BOSSSTATE_ATTACK,
	BOSSSTATE_DAMAGE,
	BOSSSTATE_DEATH,
	BOSSSTATE_MAX
}BOSSSTATE;


//**************************************************************************************************************
//ボスの構造体
//**************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 vtxMin,vtxMax;
	D3DXVECTOR3 Size;
	D3DXMATRIX mtxWorld;
	int nType;
	bool bUse;//使用しているかどうか
	MOTION Motion;
	int nLife;
	int nDamage;
	int state;
	int nCounterState;
	int nIdxShadow;
	int nIdxMap;
	int AttackState; // 
	int nAttackPattern; //ボスの攻撃パターン設定
	float Speed; // 足の速さ
	int nHitStopCount; // ヒットストップのカウント
	int nLifeBarIdx,nLifeFrame,nLifeDelayIdx; // HPゲージのインデックス
	int nStateCharge[5]; //状態異常蓄積値
	int nStateCount[5]; //状態異常の残時間(0 = 出血、1 = 炎、2 = 氷、3 = 雷、4 = 水)
	int nMaxLife;
	D3DXMATERIAL BossMat; // ボスのマテリアル
	bool bTransparent;    // 透明にしていくフラグ
	int nIdxCylinder;     // シリンダーのインデックス
	bool isKillCount;     // キルのカウント
}Boss;

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void InitBoss(void);									// ボスの初期化処理
void UninitBoss(void);									// ボスの終了処理
void UpdateBoss(void);									// ボスの更新処理
void DrawBoss(void);									// ボスの描画処理
void SetBoss(D3DXVECTOR3 pos, float speed,int nLife);	// ボスの設定処理
void HitBoss(int nCnt,int nDamage);						// ボスのヒット処理
Boss* Getboss(void);									// ボスの取得処理
int GetNumBoss(void);									// ボスの数の取得
void BossPresentCylinderIdx(int nIdx);					// ボスにシリンダーのインデックスを渡す処理
#endif
