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
void SetTextureAnimation(int pPosX, int pPosY, int pAnimSpeed, int* pCounterAnim, int* pPatternAnim, VERTEX_2D* pVtx, LPDIRECT3DVERTEXBUFFER9 pVtxBuff, int nCnt);
D3DXVECTOR3 SetMtxConversion(D3DXMATRIX mtxworld); // ワールドマトリックスをD3DXVECTOR3に変換する
D3DXVECTOR3 SetSubtractVectors(D3DXVECTOR3 targetpos, D3DXVECTOR3 pos); // 目標までのベクトルを計算する
float GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2); // 距離を計算する関数
#endif