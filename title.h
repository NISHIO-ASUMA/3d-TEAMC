//============================
//
// タイトル [title.h]
// Author: TEAM_C
//
//============================

#ifndef _TITLE_H_
#define _TITLE_H_

//****************************
//インクルードファイル
//****************************
#include"main.h"

//****************************
// 選択の種類
//****************************
typedef enum
{
	TITLESELECT_GAME = 0,
	TITLESELECT_TUTO,
	TITLESELECT_MAX
}TITLESELECT;

//****************************
// タイトルの種類
//****************************
typedef enum
{
	TITLETYPE_TITLE = 0,
	TITLETYPE_TUTO,
	TITLETYPE_MAX
}TITLETYPE;

static const char* TITLE_TYPE[TITLETYPE_MAX] =
{
	"data\\TEXTURE\\start.png",
	"data\\TEXTURE\\tutorial000.png",
};

//****************************
// タイトルの構造体
//****************************
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXVECTOR3 rot; //向き
	D3DXVECTOR3 move; //移動量
	float fWidth, fHeight; //幅、高さ
	int nType; //種類
	bool bUse; //使用状態
	int TitleMenu; // タイトル
}TITLE;

//****************************
//プロトタイプ宣言
//****************************
void InitTitle(void);//タイトル画面の初期化処理
void UninitTitle(void);//タイトル画面の終了処理
void UpdateTitle(void);//タイトル画面の更新処理
void DrawTitle(void);//タイトル画面の描画処理
void SetTitle(D3DXVECTOR3 pos,int nType,float fWidth,float fHeight);//タイトルの設定処理
void SelectTitle(int select);// タイトル画面の選択
#endif
