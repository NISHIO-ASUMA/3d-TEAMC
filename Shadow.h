//=====================================================================================================================
//
//影表示関連のヘッダー
//Author;長尾悠成
//
//=====================================================================================================================

// 重複確認
#ifndef _SHADOW_H_
#define _SHADOW_H_

// インクルードファイル
#include "main.h"

// 構造体宣言
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXCOLOR col;
	D3DXMATRIX mtxWorld;
	float fALv;
	float fSize;
	bool bUse;
	bool bHide;
}Shadow;

// プロトタイプ宣言
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize,float fAlv);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fSize, float fAlv);
void KillShadow(int nIdxShadow);
void HideShadow(int nIdxShadow);
void NoHideShadow(int nIdxShadow);
#endif