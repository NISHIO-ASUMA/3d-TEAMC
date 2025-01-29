//=================================
//
// メッシュソード[meshsword.h]
// Author: NAGAO & YOSHIDA
//
//=================================

#ifndef _MESHSWORD_H_
#define _MESHSWORD_H_

//****************************
//インクルードファイル
//****************************
#include"main.h"

//****************************
//マクロ定義
//****************************
#define SOAD_WIDTH (10.0f) //横幅
#define SOAD_HEIGHT (10.0f) //高さ

#define X (8) // X
#define Z (1) // Z

#define ORBIT_VERTEX ((X + 1) * (Z + 1)) //頂点数
#define ORBIT_INDEX (ORBIT_VERTEX) // インデックス
#define ORBIT_POLYGON (ORBIT_VERTEX - 2) // ポリゴン数

//****************************
//剣の軌道の構造体
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//剣の軌道の位置
	D3DXVECTOR3 rot;//剣の軌道の向き
	D3DXCOLOR col;//色
	D3DXMATRIX mtxWorldMeshSword;//剣の軌道のワールドマトリックス
	D3DXVECTOR3 Scal;
	D3DXVECTOR3 oldvtx[ORBIT_VERTEX]; // 保存用
}MESHSOAD;

//****************************
//プロトタイプ宣言
//****************************
void InitMeshSword(void);//剣の軌道の初期化処理
void UninitMeshSword(void);//剣の軌道の終了処理
void UpdateMeshSword(void);//剣の軌道の更新処理
void DrawMeshSword(void);//剣の軌道の描画処理
void ResetMeshSword(void);
#endif