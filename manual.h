//==============================================================================================================
//
// manual [manual.h]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

#ifndef _MANUAL_H_
#define _MANUAL_H_

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include"main.h"

//**************************************************************************************************************
// エディットのテクスチャ
//**************************************************************************************************************
typedef enum
{
	MANUALTYPE_ONE = 0,
	MANUALTYPE_MAX
}MANUALTYPE;

//**************************************************************************************************************
// マニュアルのテクスチャ宣言
//**************************************************************************************************************
static const char* MANUAL_TYPE[MANUALTYPE_MAX] =
{
	"data\\TEXTURE\\bossmanual.png",
};

//**************************************************************************************************************
// マニュアルの構造体
//**************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos; // 位置
	float fWidth, fHeight; // 幅、高さ
	int nType; // 種類
	bool bUse; // 使用状態
	bool bFirstSet; // 最初にセットしたらもう通さない
}Manual;

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void InitManual(void);   // マニュアルの初期化処理
void UninitManual(void); // マニュアルの終了処理
bool UpdateManual(void); // マニュアルの更新処理
void DrawManual(void);   // マニュアルの描画処理
void SetManual(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);
void EnableManual(bool bManual,int nType); // マニュアルの状態設定
bool GetManualState(void);	// マニュアルの状態を取得
#endif
