//==============================================================================================================
//
// ボスの体力ゲージ [ bosslife.h ]
// Author:Asuma Nishio
//
//==============================================================================================================

#ifndef _BOSSLIFE_H_
#define _BOSSLIFE_H_

//***************************
// インクルードファイル宣言
//***************************
#include "main.h"

//***************************
// 種類の列挙型
//***************************
typedef enum
{
	BOSSTEX_LIFE = 0,
	BOSSTEX_FRAME,
	BOSSTEX_MAX
}BOSSTEX;

//***************************
// テクスチャ設定
//***************************
static const char* BOSSLIFE_TEX[BOSSTEX_MAX] =
{
	"data\\TEXTURE\\boss_life.png",
	"data\\TEXTURE\\boss_lifeframe.png",
};

//***************************
// 構造体宣言
//***************************
typedef struct
{
	D3DXMATRIX mtxWorld; // ワールドマトリックス
	D3DXVECTOR3 pos; //　座標
	int nType; // 種類
	bool bUse; // 使用判定
	float fLength; // 体力バーの長さ
}BossLife;

//***************************
// プロトタイプ宣言
//***************************
void InitBossLife(); // 体力バーの初期化処理
void UninitBossLife(); // 体力バーの終了処理
void UpdateBossLife(Boss* pBoss); // 体力バーの更新処理
void DrawBossLife(); // 体力バーの描画処理
int SetBossLife(D3DXVECTOR3 pos, int nType); // 体力バーの設定処理
void SetPositionLifeBar(int nIdx,D3DXVECTOR3 pos); // 
#endif
