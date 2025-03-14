//==============================================================================================================
//
// エフェクトedit[effectedit.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _EFFECTEDIT_H_
#define _EFFECTEDIT_H_

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include"main.h"
#include"particle2.h"

//**************************************************************************************************************
// エフェクトの色の変更の種類
//**************************************************************************************************************
typedef enum
{
	COLORCHANGE_R = 0,
	COLORCHANGE_G,
	COLORCHANGE_B,
	COLORCHANGE_A,
	COLORCHANGE_MAX
}COLORCHANGE;

//**************************************************************************************************************
// エフェクトの構造体
//**************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXCOLOR col;			//色
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 fScal;
	float fRadius;
	int nLife;				//寿命
	bool bUse;				//使用しているかどうか
	int nType;
	float fSize;			// 大きさ
	float fMove;			// 移動
	D3DXVECTOR3 fAngle;		// アングル
	int nAngleValue;		// 角度の範囲
	int nNumParticle;		// 粒子の数
	int Max_Move;			// 移動量		
	int ColorType;			// カラー変更の種類
}EFFECTEDIT;

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void InitEffectEdit(void);		// エフェクトの初期化処理
void UninitEffectEdit(void);	// エフェクトの終了処理
void UpdateEffectEdit(void);	// エフェクトの更新処理
void DrawEffectEdit(void);		// エフェクトの描画処理
void SaveEffect(void);			// エフェクトのセーブ
void LoadEffect(int nType, D3DXVECTOR3 pos);
SETPARTICLE* GetEditEffect(void);
#endif
