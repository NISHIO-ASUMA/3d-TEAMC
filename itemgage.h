//==============================================================================================================
//
// アイテムのゲージ [itemgage.h]
// Author: YOSHIDA YUTO
//
//==============================================================================================================
#ifndef _ITEMGAGE_H_
#define _ITEMGAGE_H_

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include "main.h"

//**************************************************************************************************************
// アイテムゲージの種類
//**************************************************************************************************************
typedef enum
{
	ITEMGAGE_FRAME = 0,
	ITEMGAGE_GAGE,
	ITEMGAGE_MAX
}ITEMGAGE;

//**************************************************************************************************************
// アイテムゲージのテクスチャの種類
//**************************************************************************************************************
static const char* ITEMGAGE_INFO[ITEMGAGE_MAX] =
{
	"data\\TEXTURE\\lifebar_frame.png",
	"data\\TEXTURE\\Weapon_gage.png",
};
//**************************************************************************************************************
// アイテムゲージの構造体
//**************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos; // 位置
	D3DXVECTOR3 rot; // 向き
	float fWidth, fHeight,fLength; // 横幅、高さ,長さ
	bool bUse;             // 使用状態
	int nType;             // 種類
}ItemGage;

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void InitItemGage(void);   // アイテムのゲージの初期化処理
void UninitItemGage(void); // アイテムのゲージの終了処理
void UpdateItemGage(void); // アイテムのゲージの更新処理
void DrawItemGage(void);   // アイテムのゲージの描画処理
void SetItemGage(D3DXVECTOR3 pos, int nType, float fHeight);    // アイテムゲージの設定
#endif