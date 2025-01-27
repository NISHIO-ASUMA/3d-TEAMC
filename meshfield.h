//=================================
//
// メッシュフィールド[meshfield.h]
// Author: TEAM_C
//
//=================================

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//****************************
//インクルードファイル
//****************************
#include"main.h"

//****************************
//マクロ定義
//****************************
#define MAX_WIDTH (2000.0f) //横幅
#define MAX_HEIGHT (2000.0f) //高さ

#define XVTX (5) //Xの値
#define ZVTX (5) //yの値

#define VERTEX ((XVTX + 1) * (ZVTX + 1)) //頂点数
#define INDEX (POLYGON + 2)  //インデックス
#define POLYGON (((XVTX * ZVTX) * 2)) + (4 * (ZVTX - 1)) //ポリゴン数

//****************************
//メッシュフィールドの構造体
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//メッシュフィールドの位置
	D3DXVECTOR3 rot;//メッシュフィールドの向き
	D3DXMATRIX mtxWorldMeshField;//メッシュフィールドのワールドマトリックス
	D3DXVECTOR3 vtxPos[2];
}MESHFILED;

//****************************
//プロトタイプ宣言
//****************************
void InitMeshField(void);//メッシュフィールドの初期化処理
void UninitMeshField(void);//メッシュフィールドの終了処理
void UpdateMeshField(void);//メッシュフィールドの更新処理
void DrawMeshField(void);//メッシュフィールドの描画処理
bool CollisionField(void);
#endif
