//============================
//
// ブロック[block.h]
// Author:YOSHIDA YUUTO
//
//============================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//***************************
// インクルードファイル
//***************************
#include"main.h"
#include"player.h"

//***************************
// マクロ定義
//***************************
#define MAX_TEX (32)
#define MAX_BLOCK (64)
#define INDXPARENT (5)

//***************************
// ブロックの種類
//***************************
typedef enum
{
	BLOCKTYPE_APARTMENT = 0,
	BLOCKTYPE_BILL000,
	BLOCKTYPE_BILL001,
	BLOCKTYPE_BILL002,

	BLOCKTYPE_BANNER,
	BLOCKTYPE_BUS_STOP,
	BLOCKTYPE_DUMPSTER,
	BLOCKTYPE_HYDRANT,
	BLOCKTYPE_ORVIS,
	BLOCKTYPE_PHONEBOOTH,
	BLOCKTYPE_TOILE,
	BLOCKTYPE_PYLON000,
	BLOCKTYPE_PYLON001,
	BLOCKTYPE_PYLONBAR000,
	BLOCKTYPE_PYLONBAR001,
	BLOCKTYPE_VENDING_MACINE000,
	BLOCKTYPE_VENDING_MACINE001,
	BLOCKTYPE_VENDING_MACINE002,
	BLOCKTYPE_BILL003,

	BLOCKTYPE_TUTORIAL000,
	BLOCKTYPE_TUTORIAL100,
	BLOCKTYPE_TUTORIAL200,

	BLOCKTYPE_CAR000,
	BLOCKTYPE_CAR001,
	BLOCKTYPE_CAR002,
	BLOCKTYPE_CAR003,
	BLOCKTYPE_CAR004,

	BLOCKTYPE_VACANT_LAND,
	BLOCKTYPE_PUBLIC_TOILET2,
	BLOCKTYPE_HYDRAULIC_EXECAVATOR,
	BLOCKTYPE_WORKBENCH,
	BLOCKTYPE_MANHOLE,
	BLOCKTYPE_CONTAINER,

	BLOCKTYPE_MAX
}BLOCKTYPE;

//***************************
// ブロックの状態の種類
//***************************
typedef enum
{
	BLOCKSTATE_NORMAL=0,
	BLOCKSTATE_HOLD,
	BLOCKSTATE_THROW,
	BLOCKSTATE_MAX
}BLOCKSTATE;

//***************************
// ブロックのテクスチャ構造体
//***************************
typedef struct
{
	LPD3DXMESH g_pMeshBlock;		// メッシュ(頂点座標)へのポインタ
	LPD3DXBUFFER g_pBuffMatBlock;   // マテリアルへのポインタ
	DWORD g_dwNumMatBlock;          // マテリアルの数
	LPDIRECT3DTEXTURE9 g_apTextureBlock[MAX_TEX]; // テクスチャ
	D3DXVECTOR3 vtxMin, vtxMax;		// 最小値,最大値
}BLOCKTEX;

//***************************
//ブロックの構造体
//***************************
typedef struct
{
	D3DXVECTOR3 CenterPos; // 中心点の座標
	D3DXVECTOR3 VecRot[3];   // 回転
	float Length[3];         // XYZの長さ
	D3DXMATRIX ObbMtx;
}OBB;

//***************************
// ブロックの構造体
//***************************
typedef struct
{
	D3DXVECTOR3 pos; // 位置
	D3DXVECTOR3 move;// 移動
	D3DXVECTOR3 rot; // 向き
	D3DXVECTOR3 Scal;// 大きさ
	D3DXVECTOR3 Size;// サイズ
	D3DXVECTOR3 nor; // 法線ベクトル

	D3DXMATRIX mtxWorldBlock;// ワールドマトリックス
	bool bUse;
	int nType;
	int nLife;
	BLOCKSTATE state; // ブロックの状態
	float fRadius;
	BLOCKTEX BlockTex[BLOCKTYPE_MAX];
	D3DXMATRIX mtxParent;
	OBB Obb;
	int nIdxShadow;   // 影のインデックス
}BLOCK;

//***************************
//プロトタイプ宣言
//***************************
void InitBlock(void);			// ブロックの初期化処理
void UninitBlock(void);			// ブロックの終了処理
void UpdateBlock(void);			// ブロックの更新処理
void DrawBlock(void);			// ブロックの描画処理
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nType, D3DXVECTOR3 Scal);	// ブロックの設定処理
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,D3DXVECTOR3 *pMove,D3DXVECTOR3 *pSize);	// 当たり判定
bool CollisionBlockItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize);// アイテムとの当たり判定
int NumBlock(void);	// ブロック数の取得
void LoadTitleState(void);	// タイトルのブロック読み込み処理
BLOCK* GetBlock();	// ブロック情報の取得
void tutoload(void);// チュートリアルでの読み込み処理
void CreateObb(int nCnt);
bool collisionObb(int nCnt);
bool collisionObbEne(int nCntBlock);

float LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3);

#endif
