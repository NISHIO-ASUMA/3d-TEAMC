//==============================================================================================================
//
// マップエディター [edit.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _EDIT_H_
#define _EDIT_H_

//***********************
//インクルードファイル
//***********************
#include"main.h"
#include"block.h"

//*******************
//マクロ定義
//*******************
#define MAX_TYPE (200) // 種類の数

//*******************
//エディットの種類
//*******************
typedef enum
{
	EDITMODE_BLOCK = 0,
	EDITMODE_ITEM,
	EDITMODE_MAX
}EDITMODE;

////**************************
////エディットモデル構造体宣言
////**************************
//typedef struct
//{
//	LPDIRECT3DTEXTURE9 g_pTextureEdit2d;	    // テクスチャへのポインタ
//	LPD3DXMESH g_pMeshEdit;				// メッシュ(頂点座標)へのポインタ
//	LPD3DXBUFFER g_pBuffMatEdit;		// マテリアルへのポインタ
//	DWORD g_dwNumMatEdit;				// マテリアルの数
//	LPDIRECT3DTEXTURE9 g_apTextureEdit[MAX_TYPE]; // テクスチャ設定
//	D3DXVECTOR3 vtxMin, vtxMax;			// 最大値,最小値
//}EditModel;

//***************************************************************************************************************
//エディットのテクスチャ構造体
//***************************************************************************************************************
typedef struct
{
	MODEL_INFO pModel[MAX_TYPE];
	int nNumModel;
}EditTex;

//*******************
//エディットの構造体
//*******************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 Scal;
	bool bUse;
	int EditCategory;
	int ObjCnt;
	int nBlockTypeNumber;
	int nType;
	EditTex Category[EDITMODE_MAX];
	float fMove;
}EDIT_INFO;

//*********************
//プロトタイプ宣言
//*********************
void InitEdit(void);	// editの初期化処理
void UninitEdit(void);  // editの終了処理
void UpdateEdit(void);  // editの更新処理
void DrawEdit(void);	// editの設定処理
void SaveEdit(void);	// editの書き出し処理
void LoadEdit(void);	// editの読み込み処理
void ReLoadEdit(void);	// editの再読み込み処理
EDIT_INFO* GetEdit(void); // editの取得処理
int GetNumobj(void);	// 配置数の取得
bool GetEdit2d(void);   // エディット2dモードかどうか
#endif
