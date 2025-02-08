//============================
//
// ボス [boss.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _BOSS_H_
#define _BOSS_H_

//****************************
//インクルードファイル
//****************************
#include"main.h"
#include"motion.h"

//****************************
//マクロ定義
//****************************
#define MAX_BOSS (10)

//****************************
//ボスの攻撃状態
//****************************
typedef enum
{
	BOSSATTACK_NO = 0,
	BOSSATTACK_ATTACK,
	BOSSATTACK_MAX
}BOSSATTACK;

//****************************
//ボスの状態
//****************************
typedef enum
{
	BOSSSTATE_NORMAL = 0,
	BOSSSTATE_AGENT,
	BOSSSTATE_ATTACK,
	BOSSSTATE_DAMAGE,
	BOSSSTATE_DEATH,
	BOSSSTATE_MAX
}BOSSSTATE;


//****************************
//ボスの構造体
//****************************
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
	int AttackState;
	float Speed; // 足の速さ
}Boss;

//****************************
//プロトタイプ宣言
//****************************
void InitBoss(void);//ボスの初期化処理
void UninitBoss(void);//ボスの終了処理
void UpdateBoss(void);//ボスの更新処理
void DrawBoss(void);//ボスの描画処理
void SetBoss(D3DXVECTOR3 pos, float speed,int nLife);
void HitBoss(int nDamage);//ボスのヒット処理
Boss* Getboss(void);
#endif
