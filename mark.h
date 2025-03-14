//==============================================================================================================
//
// マーク処理　[mark.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _MARK_H_
#define _MARK_H_

//***************************
//インクルードファイル
//***************************
#include"main.h"

//***************************
//マクロ定義
//***************************
#define NULLVECTOR3 (D3DXVECTOR3(0.0f,0.0f,0.0f)) // 初期化用マクロ変数

//***************************
// マークの構造体
//***************************
typedef struct
{
	D3DXVECTOR3 pos;							//位置(オフセット)
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;							//向き
	D3DXVECTOR3 offpos;							//位置(オフセット)
	D3DXVECTOR3 offrot;							//向き
	D3DXVECTOR3 rotDest;						//向き
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
	bool bUse; // 使用状態
}Mark;

//***************************
//プロトタイプ宣言
//***************************
void InitMark(void);
void UninitMark(void);
void UpdateMark(void);
void DrawMark(void);
void SetMark(D3DXVECTOR3 pos,D3DXVECTOR3 rot);
Mark* GetMark(void);
#endif
