//=====================================================================================================================
//
//ダメージ表示関連のヘッダー
//Author;長尾悠成
//
//=====================================================================================================================

// 重複確認
#ifndef _DAMEGE_H_
#define _DAMEGE_H_

// インクルードファイル
#include "Main.h"

// プロトタイプ宣言
void InitDamege(void);
void UninitDamege(void);
void UpdateDamege(void);
void DrawDamege(void);
void SetDamege(D3DXVECTOR3 pos, int nDamege, int nLife, bool Heal);

#endif