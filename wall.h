//============================
//
// 壁[wall.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _WALL_H_
#define _WALL_H_

//****************************
//インクルードファイル
//****************************
#include"main.h"

//****************************
//マクロ定義
//****************************
#define MAX_WALL (8) //壁の最大数
#define WALL_WIDTH (100.0f) // 横幅
#define WALL_HEIGHT (150.0f) // 高さ

//****************************
//壁の構造体
//****************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	float fA;
	D3DXVECTOR3 Size;
	float fWidth;
	float fHeight;
	D3DXVECTOR3 fScal;
	bool bUse;
	D3DXVECTOR3 vecLine;
	D3DXVECTOR3 vecPos;
	D3DXMATRIX rotmtx;
	D3DXMATRIX posmtx;
	D3DXVECTOR3 vtxPos[4];
}WALL;

//****************************
//プロトタイプ宣言
//****************************
void InitWall(void);//壁の初期化処理
void UninitWall(void);//壁の終了処理
void UpdateWall(void);//壁の更新処理
void DrawWall(void);//壁の描画処理
//void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 fScal,float fA);//壁の設定処理
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fA, D3DXVECTOR3 Size);//壁の設定処理
void CollisionWall(void);
void DotWall(void);
WALL* GetWall(void);
int NumWall(void);
#endif