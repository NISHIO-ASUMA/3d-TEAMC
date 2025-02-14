//======================
//
//Author:長尾悠成
//弾関係のヘッダー
//
//======================
#ifndef _BULLET_H_
#define _BULLET_H_

//****************************
// インクルードファイル宣言
//****************************
#include "main.h"

//****************************
// 構造体
//****************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posDest;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 dir;
	D3DXVECTOR3 move;
	D3DXMATRIX mtxWorld;
	int nLife;
	int nIdxShadow;
	int nDamege;
	float fSize;
	float fSpeed;
	bool bUse;
	bool bEnemy;
}Bullet;
//****************************
// 処理一覧
//****************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 dir, int nLife, int nDamege,float fSize, float fSpeed,bool Enemy);

Bullet* GetBullet();

void KillBullet(int nNum);
#endif