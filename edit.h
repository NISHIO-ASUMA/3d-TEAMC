//============================
//
// edit [edit.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _EDIT_H_
#define _EDIT_H_

//***********************
//インクルードファイル
//***********************

#include"main.h"
#include"block.h"
#include "item.h"

//*******************
//マクロ定義
//*******************
#define MAX_TYPE (100) // 種類の数

//*******************
//エディットの種類
//*******************
typedef enum
{
	EDITMODE_BLOCK = 0,
	EDITMODE_ITEM,
	EDITMODE_MAX
}EDITMODE;

const int EDIT_COUNT[EDITMODE_MAX] =
{
	BLOCKTYPE_MAX,
	ITEMTYPE_MAX,
};

typedef struct
{
	LPD3DXMESH g_pMeshEdit;//メッシュ(頂点座標)へのポインタ
	LPD3DXBUFFER g_pBuffMatEdit;//マテリアルへのポインタ
	DWORD g_dwNumMatEdit;//マテリアルの数
	LPDIRECT3DTEXTURE9 g_apTextureEdit[MAX_TEX];
	D3DXVECTOR3 vtxMin, vtxMax;
}EditModel;
//*****************************
//エディットのテクスチャ構造体
//*****************************
typedef struct
{
	EditModel pModel[MAX_TYPE];
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
void InitEdit(void);//editの初期化処理
void UninitEdit(void);//editの終了処理
void UpdateEdit(void);//editの更新処理
void DrawEdit(void);//editの設定処理
void SaveEdit(void);
void LoadEdit(void);
void ReLoadEdit(void);
EDIT_INFO* GetEdit(void);
int GetNumobj(void);
#endif
