//=============================
//
// 弾の処理 [ bullet.h ]
// Author:長尾悠成
//
//=============================
#ifndef _BULLET_H_
#define _BULLET_H_

//****************************
// インクルードファイル宣言
//****************************
#include "main.h"

//****************************
// 弾の構造体宣言
//****************************
typedef struct
{
	D3DXVECTOR3 pos; // 座標
	D3DXVECTOR3 posDest; // 目的の座標
	D3DXVECTOR3 rot; // 角度
	D3DXVECTOR3 dir; // 方向ベクトル
	D3DXVECTOR3 move;// 移動量
	D3DXMATRIX mtxWorld; // ワールドマトリックス
	int nLife;       // 寿命
	int nIdxShadow;  // 影のインデックス
	int nDamege;     // ダメージ量
	float fSize;     // 弾の大きさ
	float fSpeed;    // 速度
	bool bUse;       // 使用しているか否か
	bool bEnemy;     // 敵か否か
}Bullet;

//****************************
// プロトタイプ宣言
//****************************
void InitBullet(void);  // 弾の初期化処理
void UninitBullet(void);// 弾の終了処理
void UpdateBullet(void);// 弾の更新処理
void DrawBullet(void);  // 弾の描画処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 dir, int nLife, int nDamege,float fSize, float fSpeed,bool Enemy); // 弾の設定処理
void KillBullet(int nNum); // 未使用にする処理
Bullet* GetBullet();    // 弾情報の取得処理

#endif