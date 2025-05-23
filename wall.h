//==============================================================================================================
//
// 壁[wall.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _WALL_H_
#define _WALL_H_

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include"main.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MAX_WALL (8)		 // 壁の最大数
#define WALL_WIDTH (100.0f)  // 横幅
#define WALL_HEIGHT (250.0f) // 高さ

//**************************************************************************************************************
// 壁の種類
//**************************************************************************************************************
typedef enum
{
	WALLTEXTURE_CITY = 0,
	WALLTEXTURE_MT,
	WALLTEXTURE_MAX
}WALLTEXTURE;

//**************************************************************************************************************
// 壁の構造体
//**************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 rot;		// 向き
	D3DXMATRIX mtxWorld;    // ワールドマトリックス
	float fA;				// α値
	D3DXVECTOR3 Size;		// 大きさ
	float fWidth;			// 横幅
	float fHeight;			// 高さ
	D3DXVECTOR3 fScal;		// 大きさ
	bool bUse;			    // 使用状態
	D3DXVECTOR3 vtxPos[4];  // 頂点の位置
	int nType;              // 種類
}WALL;

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void InitWall(void);   // 壁の初期化処理
void UninitWall(void); // 壁の終了処理
void UpdateWall(void); // 壁の更新処理
void DrawWall(void);   // 壁の描画処理
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fA, D3DXVECTOR3 Size, int nType); // 壁の設定処理
void CollisionWall(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld,D3DXVECTOR3* pMove, float speed);
void DotWall(void);
WALL* GetWall(void);
int NumWall(void);
#endif