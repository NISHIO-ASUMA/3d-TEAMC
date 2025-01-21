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

//*******************
//エディットの種類
//*******************
typedef enum
{
	EDITMODE_BLOCK = 0,
	EDITMODE_WALL,
	EDITMODE_MAX
}EDITMODE;

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
	int nEditMode;
	int ObjCnt;
	int nBlockTypeNumber;
	int nType;
	BLOCKTEX ModelTex[BLOCKTYPE_MAX];
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
