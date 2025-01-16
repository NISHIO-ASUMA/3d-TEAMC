//===================================
//
//影表示関連のヘッダー
//Author;長尾悠成
//
//===================================

// 重複確認とインクルード
#include "main.h"
#ifndef _SHADOW_H_
#define _SHADOW_H_

// 構造体一覧
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

// 処理の色々
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fSize, float fAlv);
void KillShadow(int nIdxShadow);
void HideShadow(int nIdxShadow);
void NoHideShadow(int nIdxShadow);
#endif