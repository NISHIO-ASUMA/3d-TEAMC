//==============================================================================================================
//
// math [math.h]
// Author: YOSHIDA YUTO
//
//==============================================================================================================
#ifndef _MATH_H_
#define _MATH_H_

#include"main.h"

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
float SetSmoothAprroach(float fDestValue, float Value, float coefficient);
bool sphererange(D3DXVECTOR3* pPos1, D3DXVECTOR3* pPos2, float radius1, float radius2); // 円の当たり判定
//void SetTextureAnimation(int* pPosX, int* pPosY, int* pAnimSpeed, int* pCounterAnim, int* pPatternAnim);
D3DXVECTOR3 SetMtxConversion(D3DXMATRIX mtxworld); // ワールドマトリックスをD3DXVECTOR3に変換する
#endif