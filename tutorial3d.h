//==================================
//
// チュートリアル3d [tutorial3d.h]
// Author: TEAM_C
//
//===================================
#ifndef _TUTORIAL3D_H_
#define _TUTORIAL3D_H_

//****************************
//インクルードファイル
//****************************
#include"main.h"

//****************************
//プロトタイプ宣言
//****************************
void InitTutorial3d(void);	// チュートリアル3d画面の初期化処理
void UninitTutorial3d(void);// チュートリアル3d画面の終了処理
void UpdateTutorial3d(void);// チュートリアル3d画面の更新処理
void DrawTutorial3d(void);	// チュートリアル3d画面の描画処理
bool GetEditStatetuto(void);// エディットの状態取得
#endif

