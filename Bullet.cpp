//======================
//
//Author:長尾悠成
//弾描画の基礎
//
//======================

//***************************
// インクルードファイル宣言
//***************************
#include "main.h"
#include "Bullet.h"
#include "Shadow.h"
#include "Effect.h"
#include "wall.h"
#include "Player.h"
#include "block.h"
#include "explosion.h"

//***************************
// グローバル変数達
//***************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;
#define MAX_BULLET (100)
Bullet g_Bullet[MAX_BULLET];

//==========================
// 初期化処理
//==========================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\BBBullet.png", &g_pTextureBullet);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_BULLET * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);
	VERTEX_3D* pVtx;
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
	//左、手前、下がマイナス
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		g_Bullet[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 場所
		g_Bullet[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 角度
		g_Bullet[nCnt].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 方向
		g_Bullet[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// ベクトル
		g_Bullet[nCnt].bUse = false;// 使用の有無
		g_Bullet[nCnt].nLife = 0;// 寿命
		g_Bullet[nCnt].fSize = 2.0f;// 大きさ
		g_Bullet[nCnt].fSpeed = 1.0f;// 速度
		g_Bullet[nCnt].bEnemy = true;// 敵の弾かどうか

		// 描画色々
		pVtx[0].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x - g_Bullet[nCnt].fSize, g_Bullet[nCnt].pos.y + g_Bullet[nCnt].fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x + g_Bullet[nCnt].fSize, g_Bullet[nCnt].pos.y + g_Bullet[nCnt].fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x - g_Bullet[nCnt].fSize, g_Bullet[nCnt].pos.y - g_Bullet[nCnt].fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x + g_Bullet[nCnt].fSize, g_Bullet[nCnt].pos.y - g_Bullet[nCnt].fSize, 0.0f);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock();
}

//==========================
// 終了処理
//==========================
void UninitBullet(void)
{
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
}

//==========================
// 更新処理
//==========================
void UpdateBullet(void)
{
	Player* pPlayer = GetPlayer();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_3D* pVtx;
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// 全て見回し
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		// 存在するなら
		if (g_Bullet[nCnt].bUse == true)
		{
			// 場所を保存し
			g_Bullet[nCnt].posDest = g_Bullet[nCnt].pos;
			
			// moveの分移動
			g_Bullet[nCnt].pos += g_Bullet[nCnt].move;
			
			// 寿命減少
			g_Bullet[nCnt].nLife--;

			// 影の場所移動
			SetPositionShadow(g_Bullet[nCnt].nIdxShadow, g_Bullet[nCnt].pos, 2.0f + 2.0f * g_Bullet[nCnt].pos.y / 200.0f, 1.0f / (1.0f + g_Bullet[nCnt].pos.y / 30.0f));
			
			// 味方の弾なら
			if (g_Bullet[nCnt].bEnemy == false)
			{
				SetEffect(g_Bullet[nCnt].pos, g_Bullet[nCnt].dir, 5, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 3.0f);
			}
			// 敵の弾なら
			else
			{
				// エフェクトを書き
				SetEffect(g_Bullet[nCnt].pos, g_Bullet[nCnt].dir, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, 3.0f);

				// 当たり判定を纏わせる
				if (sphererange(&g_Bullet[nCnt].pos, &pPlayer->pos, g_Bullet[nCnt].fSize, 30.0f))
				{
					//pPlayer->state != PLAYERSTATE_DAMAGE
					HitPlayer(g_Bullet[nCnt].nDamege);
					g_Bullet[nCnt].bUse = false;
					KillShadow(g_Bullet[nCnt].nIdxShadow);
				}
			}

			// 寿命で死ぬ
			if (g_Bullet[nCnt].nLife <= 0)
			{
				g_Bullet[nCnt].bUse = false;
				KillShadow(g_Bullet[nCnt].nIdxShadow);
			}
		}
	}
	g_pVtxBuffBullet->Unlock();
}

//==========================
// 描画処理
//==========================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイス取得
	pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_Bullet[nCnt].bUse == true)
		{
			D3DXMATRIX mtxView;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			D3DXMatrixIdentity(&g_Bullet[nCnt].mtxWorld);

			g_Bullet[nCnt].mtxWorld._11 = mtxView._11;
			g_Bullet[nCnt].mtxWorld._12 = mtxView._21;
			g_Bullet[nCnt].mtxWorld._13 = mtxView._31;
			g_Bullet[nCnt].mtxWorld._21 = mtxView._12;
			g_Bullet[nCnt].mtxWorld._22 = mtxView._22;
			g_Bullet[nCnt].mtxWorld._23 = mtxView._32;
			g_Bullet[nCnt].mtxWorld._31 = mtxView._13;
			g_Bullet[nCnt].mtxWorld._32 = mtxView._23;
			g_Bullet[nCnt].mtxWorld._33 = mtxView._33;

			g_Bullet[nCnt].mtxWorld._41 = g_Bullet[nCnt].pos.x;
			g_Bullet[nCnt].mtxWorld._42 = g_Bullet[nCnt].pos.y;
			g_Bullet[nCnt].mtxWorld._43 = g_Bullet[nCnt].pos.z;
			mtxView._14 = g_Bullet[nCnt].mtxWorld._41;
			mtxView._24 = g_Bullet[nCnt].mtxWorld._42;
			mtxView._34 = g_Bullet[nCnt].mtxWorld._43;

			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Bullet[nCnt].rot.y, g_Bullet[nCnt].rot.x, g_Bullet[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Bullet[nCnt].mtxWorld, &g_Bullet[nCnt].mtxWorld, &mtxRot);
			D3DXMatrixTranslation(&mtxTrans, g_Bullet[nCnt].rot.x, g_Bullet[nCnt].rot.y, g_Bullet[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Bullet[nCnt].mtxWorld, &g_Bullet[nCnt].mtxWorld, &mtxTrans);
			pDevice->SetTransform(D3DTS_WORLD, &g_Bullet[nCnt].mtxWorld);
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));
			pDevice->SetTexture(0, g_pTextureBullet);
			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================
// 設置処理
//==========================

// 左から場所、ベクトル、向き、寿命、ダメージ、大きさ、速度、敵味方の判別
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 dir, int nLife, int nDamege, float fSize,float fSpeed, bool Enemy)
{
	int nCntBullet;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_3D* pVtx;
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
	// 全て見回して
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// 使われてないなら
		if (g_Bullet[nCntBullet].bUse == false)
		{
			// 上から影の設定、場所、方向、寿命、ダメージ、速度、敵味方の区別、ベクトルと速度の合算、使用させる
			g_Bullet[nCntBullet].nIdxShadow = SetShadow(g_Bullet[nCntBullet].pos, g_Bullet[nCntBullet].rot, fSize, 1.0f);
			g_Bullet[nCntBullet].pos = pos;
			g_Bullet[nCntBullet].dir = dir;
			g_Bullet[nCntBullet].nLife = nLife;
			g_Bullet[nCntBullet].nDamege = nDamege;
			g_Bullet[nCntBullet].fSpeed = fSpeed;
			g_Bullet[nCntBullet].fSize = fSize;
			g_Bullet[nCntBullet].bEnemy = Enemy;
			g_Bullet[nCntBullet].move = move * fSpeed;
			g_Bullet[nCntBullet].bUse = true;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock();
}

//==========================
// 取得処理
//==========================
Bullet* GetBullet(void)
{
	return &g_Bullet[0];
}

//==========================
// 破壊処理
//==========================
void KillBullet(int nNum)
{
	g_Bullet[nNum].bUse = false;
	KillShadow(g_Bullet[nNum].nIdxShadow);
}