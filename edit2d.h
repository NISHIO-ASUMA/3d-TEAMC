//==============================================================================================================
//
// edit2d [edit2d.h]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

#ifndef _EDIT2D_H_
#define _EDIT2D_H_

//****************************
// インクルードファイル
//****************************
#include"main.h"

//****************************
// editの構造体
//****************************
typedef struct
{
	LPDIRECT3DTEXTURE9 g_pTextureEdit2d;  //テクスチャへのポインタ
	D3DXVECTOR3 pos;					  // 位置
	D3DXVECTOR3 rot;					  // 回転
	D3DXVECTOR3 move;					  // 移動量
	D3DXMATRIX mtxWorld;				  // ワールドマトリックス
	float fWidth, fHeight,fLength;		  // 幅、高さ
	int nType;							  // 種類
	bool bUse;							  // 使用状態
	float fMove;						  // 移動量
}EDIT_INFO_2D;

//****************************
// プロトタイプ宣言
//****************************
void InitEdit2d(void);   // エディット2dの初期化処理
void UninitEdit2d(void); // エディット2dの終了処理
void UpdateEdit2d(void); // エディット2dの更新処理
void DrawEdit2d(void);   // エディット2dの描画処理
void SaveEdit2d(void);   // エディット2dの書き出し
void LoadEdit2d(void);   // エディット2dの読み込み
void ReLoadEdit2d(void); // エディット2dの再読み込み
EDIT_INFO_2D* GetEditInfo2D(void); // エディット2dの取得処理
int GetNum2d(void);            // 配置数取得
#endif
