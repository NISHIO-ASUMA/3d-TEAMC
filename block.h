//============================
//
// ブロック[block.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _BLOCK_H_
#define _BLOCK_H_

//***************************
//インクルードファイル
//***************************
#include"main.h"
#include"player.h"

//***************************
//マクロ定義
//***************************
#define MAX_TEX (32)
#define MAX_BLOCK (64)
#define INDXPARENT (5)

//***************************
//ブロックの種類
//***************************
typedef enum
{
	BLOCKTYPE_APARTMENT = 0,
	BLOCKTYPE_BILL000,
	BLOCKTYPE_BILL001,
	BLOCKTYPE_BILL002,
	BLOCKTYPE_MAX
}BLOCKTYPE;

static const char* MODELTYPE[BLOCKTYPE_MAX] =
{
	"data\\MODEL\\OBJ\\apartment.x",
	"data\\MODEL\\OBJ\\bill000.x",
	"data\\MODEL\\OBJ\\bill001.x",
	"data\\MODEL\\OBJ\\bill002.x",

};

//***************************
//ブロックの状態の種類
//***************************
typedef enum
{
	BLOCKSTATE_NORMAL=0,
	BLOCKSTATE_HOLD,
	BLOCKSTATE_THROW,
	BLOCKSTATE_MAX
}BLOCKSTATE;

//***************************
//ブロックのテクスチャ構造体
//***************************
typedef struct
{
	LPD3DXMESH g_pMeshBlock;//メッシュ(頂点座標)へのポインタ
	LPD3DXBUFFER g_pBuffMatBlock;//マテリアルへのポインタ
	DWORD g_dwNumMatBlock;//マテリアルの数
	LPDIRECT3DTEXTURE9 g_apTextureBlock[MAX_TEX];
	D3DXVECTOR3 vtxMin, vtxMax;
}BLOCKTEX;

//***************************
//ブロックの構造体
//***************************
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 Scal;//大きさ
	D3DXMATRIX mtxWorldBlock;//ワールドマトリックス
	D3DXVECTOR3 Size;
	bool bUse;
	int nType;
	int nLife;
	BLOCKSTATE state;//ブロックの状態
	float fRadius;
	BLOCKTEX BlockTex[BLOCKTYPE_MAX];
	D3DXMATRIX mtxParent;
}BLOCK;

//***************************
//プロトタイプ宣言
//***************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 Scal);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,D3DXVECTOR3 *pMove,D3DXVECTOR3 *pSize);
int NumBlock(void);
BLOCK* GetBlock();
#endif
