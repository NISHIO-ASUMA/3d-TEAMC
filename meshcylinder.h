//===================================================================================================================
//
// メッシュシリンダー[meshcylinder.h]
// Author: YOSHIDA YUTO
//
//===================================================================================================================

#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include"main.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define CYLINDER_WIDTH (100.0f) //横幅
#define CYLINDER_HEIGHT (100.0f) //高さ

#define CYLINDER_X (8) //Xの値
#define CYLINDER_Z (1) //yの値

#define CYLINDER_VERTEX ((CYLINDER_X + 1) * (CYLINDER_Z + 1)) //頂点数
#define CYLINDER_POLYGON (((CYLINDER_X * CYLINDER_Z) * 2)) + (4 * (CYLINDER_Z - 1)) //ポリゴン数
#define CYLINDER_INDEX (CYLINDER_POLYGON + 2)  //インデックス

//**************************************************************************************************************
// メッシュシリンダーの種類
//**************************************************************************************************************
typedef enum
{
	CYLINDERTYPE_NORMAL = 0,
	CYLINDERTYPE_HEAL,
	CYLINDERTYPE_EVENT,
	CYLINDERTYPE_TERRITORY,
	CYLINDERTYPE_ITEM,
	CYLINDERTYPE_MAX
}CYLINDERTYPE;

//**************************************************************************************************************
// メッシュシリンダーの構造体
//**************************************************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder;	    // テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder; // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder;  // インデックスバッファへのポインタ
	D3DXVECTOR3 pos;								// メッシュシリンダーの位置
	D3DXVECTOR3 rot;								// メッシュシリンダーの向き
	D3DXMATRIX mtxWorld;							// メッシュシリンダーのワールドマトリックス
	int nType;										// メッシュシリンダーの種類
	int nLife;										// 寿命
	float fRadius;									// 半径
	bool bUse;										// 使用状態
	D3DXCOLOR col;									// 色
	float Speed;									// 速さ
	int Vertex, polygon, Index;						// 頂点数、ポリゴン、インデックス
	int nNumPosX, nNumPosZ;							// 頂点の分割数
	float decAlv;                                   // α値の減少値
	float fWidth, fHeight;                          // 横幅　高さ
}MeshCylinder;

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void InitMeshCylinder(void);//メッシュシリンダーの初期化処理
void UninitMeshCylinder(void);//メッシュシリンダーの終了処理
void UpdateMeshCylinder(void);//メッシュシリンダーの更新処理
void DrawMeshCylinder(void);//メッシュシリンダーの描画処理
int SetMeshCylinder(D3DXVECTOR3 pos, int nType, int nLife, float fRadius, D3DXCOLOR col, int nNumPosX, int nNumPosZ, float speed, float fHeight); // メッシュシリンダーの設定
void CreateMeshCylinder(int nCntCylinder, int Vertex, int Index, int nNumPosX, int nNumPosZ, float fRadius, float fHeight);// メッシュシリンダーの作成処理
void SetPotisionCylinder(int nIdx,D3DXVECTOR3 pos); // シリンダーの位置設定処理
void DeleteCylinder(int nIdx);
bool KeepInCylinder(D3DXVECTOR3 *pPos);
bool CollisionCylinder(int nIdx, D3DXVECTOR3* pPos);
#endif
