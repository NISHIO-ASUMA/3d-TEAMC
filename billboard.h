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
#include "boss.h"

//***************************
// マクロ定義
//***************************
#define MAX_BILLBOARD (712)    // 最大数

//*****************************
// ビルボードの種類の列挙型宣言
//*****************************
typedef enum
{
	BILLBOARDSTATE_NOSET = 0,
	BILLBOARDSTATE_SET,
	BILLBOARDSTATE_MAX
}BILLBOARDSTATE;

//*****************************
// ビルボードの種類の列挙型宣言
//*****************************
typedef enum
{
	BILLBOARDTYPE_FIRST = 0,
	BILLBOARDTYPE_SECOND,
	BILLBOARDTYPE_TARGET,
	BILLBOARDTYPE_MAX
}BILLBOARDTYPE;

//************************************
// ビルボードのテクスチャの列挙型宣言
//************************************
static const char* BILLBOARD_TEXTURE[BILLBOARDTYPE_MAX] = 
{
	"data\\TEXTURE\\billboard_wepon.png",
	"data\\TEXTURE\\billboard_Key_get.png",
	"data\\TEXTURE\\target.png",
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
	int state;           // ビルボードの状態
	int nIdx;            // インデックス番号
}Billboard;

//**********************
// プロトタイプ宣言
//**********************
void InitBillboard(void);		// ビルボード初期化処理
void UninitBillboard(void);		// ビルボード終了処理
void UpdateBillboard(void);		// ビルボード更新処理
void DrawBillboard(void);		// ビルボード描画処理
int SetBillboard(D3DXVECTOR3 pos,int nType, float fWidth, float fHeight, int state); // ビルボードの設定処理
void DeletIdxBillboard(int nIdx);
Billboard* GetBillBoard(); // ビルボードの情報取得
void SetPositiontarget(int nIdx,D3DXVECTOR3 pos); // ターゲットの位置設定処理
#endif 

