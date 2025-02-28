//===================================================================================================================
//
// メッシュインパクト[meshimpact.h]
// Author: YOSHIDA YUTO
//
//===================================================================================================================

#ifndef _MESHIMPACT_H_
#define _MESHIMPACT_H_

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include"main.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define IMPACT_Z (1)

//**************************************************************************************************************
// メッシュインパクトの種類
//**************************************************************************************************************
typedef enum
{
	IMPACTTYPE_NORMAL = 0,
	IMPACTTYPE_PLAYER,
	IMPACTTYPE_ENEMY,
	IMPACTTYPE_SPKATANA,
	IMPACTTYPE_MAX
}IMPACTTYPE;

//**************************************************************************************************************
//メッシュインパクトの構造体
//**************************************************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 g_pTextureMeshImpact;	    // テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshImpact; // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshImpact;  // インデックスバッファへのポインタ
	D3DXVECTOR3 pos;			//メッシュインパクトの位置
	D3DXVECTOR3 rot;			//メッシュインパクトの向き
	D3DXMATRIX mtxWorld;		//メッシュインパクトのワールドマトリックス
	D3DXVECTOR3 move;			// 移動量
	D3DXCOLOR col;              // 色
	int Vertex,Polygon,Index;	// 頂点数、ポリゴン数、インデックス数
	int nImpactPosX;            // インパクトのXの分割数
	float OutRadius,InRadius;	// 外側の半径、内側の半径
	int nType;					// インパクトの種類
	float speed;				// インパクトの速度
	float decAlv;				// インパクトの透明化の速度
	int nLife;                  // 寿命
	bool bUse;                  // 使用状態
	int nDamege;                // 攻撃力
}MeshImpact;

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void InitMeshImpact(void);//メッシュインパクトの初期化処理
void UninitMeshImpact(void);//メッシュインパクトの終了処理
void UpdateMeshImpact(void);//メッシュインパクトの更新処理
void DrawMeshImpact(void);//メッシュインパクトの描画処理
void SetImpact(D3DXVECTOR3 pos, D3DXCOLOR col,int nImpactPosX,float OutRadius,float InRadius,float Speed,int nLife,int nType,int nDamege); // インパクトの設定処理(Out > In)
void CreateImpact(int nCntImpact, int nImpactPosX, int Vertex, int Index);                                                     // インパクトの作成処理
bool CollisionImpact(D3DXVECTOR3 *pPos);
int ImpactDamege(int nID);
#endif

