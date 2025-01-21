//============================
//
// アイテム [item.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _ITEM_H_
#define _ITEM_H_

//***************************
//インクルードファイル
//***************************
#include"main.h"
#include"player.h"

//***************************
//マクロ定義
//***************************
//#define MAX_TEX (32)
#define MAX_ITEM (64)
#define PARENT (5)

//***************************
//ブロックの種類
//***************************
typedef enum
{
	ITEMTYPE_ONE = 0,
	ITEMTYPE_MAX
}ITEMTYPE;

static const char* ITEMTYPE_INFO[ITEMTYPE_MAX] =
{
	"data\\MODEL\\OBJ\\wepon_stone.x",
};

//***************************
//ブロックの状態の種類
//***************************
typedef enum
{
	ITEMSTATE_NORMAL = 0,
	ITEMSTATE_HOLD,
	ITEMSTATE_THROW,
	ITEMSTATE_MAX
}ItemSTATE;

//***************************
//ブロックのテクスチャ構造体
//***************************
typedef struct
{
	LPD3DXMESH g_pMeshItem;//メッシュ(頂点座標)へのポインタ
	LPD3DXBUFFER g_pBuffMatItem;//マテリアルへのポインタ
	DWORD g_dwNumMatItem;//マテリアルの数
	LPDIRECT3DTEXTURE9 g_apTextureItem[32];
	D3DXVECTOR3 vtxMin, vtxMax;
}ItemTex;

//***************************
//ブロックの構造体
//***************************
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 Scal;//大きさ
	D3DXMATRIX mtxWorldItem;//ワールドマトリックス
	D3DXVECTOR3 Size;
	bool bUse;
	int nType;
	int nLife;
	ItemSTATE state;//ブロックの状態
	float fRadius;
	ItemTex ItemTex[ITEMTYPE_MAX];
	D3DXMATRIX mtxParent;
}Item;

//***************************
//プロトタイプ宣言
//***************************
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, int nType,D3DXVECTOR3 Scal);
bool CollisionItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize);
bool CollisionEnemy(D3DXVECTOR3* pPos, float ItemRadius, float EnemyRadius);
//Item* GetItem();
#endif
