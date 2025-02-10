//======================================
//
// メッシュの球型の処理 [mashfan.h]
// Author: Asuma Nishio
//
//======================================
#ifndef _MESHFAN_H_
#define _MESHFAN_H_

//***************************
// インクルードファイル宣言
//***************************
#include "main.h"

//***************************
// マクロ定義
//***************************
#define meshwall_X_BLOCK 10 // X方向のブロック数
#define meshwall_Z_BLOCK 10 // Z方向のブロック数

#define meshwall_VERTEX_NUM ((meshwall_X_BLOCK + 1) * (meshwall_Z_BLOCK + 1)) // メッシュフィールドの頂点数

#define meshwall_INDEX_NUM (meshwall_X_BLOCK * meshwall_Z_BLOCK * 6) // メッシュフィールドのインデックス数

#define meshwall_PRIMITIVE_NUM ((meshwall_X_BLOCK * meshwall_Z_BLOCK * 2) + ((meshwall_Z_BLOCK - 1) * 2)) // メッシュフィールドのプリミティブ数

//***************************
// プロトタイプ宣言
//***************************
void InitmeshFan(void);   // 初期化処理
void UninitmeshFan(void); // 終了処理
void UpdatemeshFan(void); // 更新処理
void DrawmeshFan(void);   // 描画処理

#endif // !_MESHFAN_H_

