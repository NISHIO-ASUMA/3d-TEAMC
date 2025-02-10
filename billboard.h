//================================
//
// ビルボード処理 [billboard.h]
// Author: Asuma Nishio
//
//================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//***************************
// インクルードファイル宣言
//***************************
#include "main.h"

//***************************
// マクロ定義
//***************************
#define MAX_BILLBOARD (50)    // 最大数

//*****************************
// ビルボードの種類の列挙型宣言
//*****************************
typedef enum
{
	BILLBOARDTYPE_FIRST = 0,
	BILLBOARDTYPE_MAX
}BILLBOARDTYPE;

//************************************
// ビルボードのテクスチャの列挙型宣言
//************************************
static const char* BILLBOARD_TEXTURE[BILLBOARDTYPE_MAX] = 
{
	"data\\TEXTURE\\billboard_wepon.png",
};

//***************************
// ビルボード構造体宣言
//***************************
typedef struct
{
	D3DXVECTOR3 pos;	 // 位置
	D3DXVECTOR3 rot;	 // 向き
	D3DXVECTOR3 move;	 // 移動量
	D3DXMATRIX  mtxWorld;// ワールドマトリックス
	bool bUse;			 // 使用判定
	int nType;		     // 種類
	float fWidth;		 // 横幅
	float fHeight;		 // 高さ
}Billboard;

//**********************
// プロトタイプ宣言
//**********************
void InitBillboard(void);		// ビルボード初期化処理
void UninitBillboard(void);		// ビルボード終了処理
void UpdateBillboard(void);		// ビルボード更新処理
void DrawBillboard(void);		// ビルボード描画処理
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight); // ビルボード設定処理
#endif 

