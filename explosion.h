//============================
//
// 爆発[explosion.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//****************************
//インクルードファイル
//****************************
#include"main.h"

//****************************
// 爆発の種類
//****************************
typedef enum
{
	EXPLOSION_MOVE = 0,
	EXPLOSION_LANDING,
	EXPLOSION_HIT,
	EXPLOSION_MAX
}EXPLOSIONTYPE;

//****************************
// 爆発の構造体
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;
	D3DXMATRIX mtxWorld;//ワールドマトリックス
	D3DXCOLOR col;//色
	int nCounterAnim;
	int nPatternAnim;
	int nIdxShadow;//番号
	int nLife;//寿命
	bool bUse;//使用しているかどうか
	float speed, radius;
	float fWidth, fHeight;//幅,高さ
	int nType; // 種類
}EXPLOSION;

//****************************
//プロトタイプ宣言
//****************************
void InitExplosion(void);//爆発の初期化処理
void UninitExplosion(void);//爆発の終了処理
void UpdateExplosion(void);//爆発の更新処理
void DrawExplosion(void);//爆発の描画処理
void SetExplosion(D3DXVECTOR3 pos,D3DXCOLOR col,int nLife,float fWidth,float fHeight,int nType);//爆発の設定処理
#endif
