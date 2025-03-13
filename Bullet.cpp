//===================================
//
// 弾の処理 [ bullet.cpp ]
// Author:長尾悠成
//
//===================================

//***************************
// インクルードファイル宣言
//***************************
#include "Bullet.h"
#include "Shadow.h"
#include "Effect.h"
#include "wall.h"
#include "Player.h"
#include "block.h"
#include "explosion.h"
#include "math.h"

//***************************
// マクロ定義
//***************************
#define MAX_BULLET (300)

//***************************
// グローバル変数宣言
//***************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL; // 頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;      // テクスチャのポインタ
Bullet g_Bullet[MAX_BULLET];					 // 構造体変数

//==========================
// 弾の初期化処理
//==========================
void InitBullet(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, 
		"data\\TEXTURE\\BBBullet.png", 
		&g_pTextureBullet);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_BULLET * 4,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_3D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffBullet, 
		NULL);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファのロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// 構造体変数の初期化
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{//左、手前、下がマイナス

		g_Bullet[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 場所
		g_Bullet[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 角度
		g_Bullet[nCnt].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 方向ベクトル
		g_Bullet[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 移動量
		g_Bullet[nCnt].bUse = false; // 使用の有無
		g_Bullet[nCnt].nLife = 0;    // 寿命
		g_Bullet[nCnt].fSize = 3.0f; // 大きさ
		g_Bullet[nCnt].fSpeed = 1.0f;// 速度
		g_Bullet[nCnt].bEnemy = true;// 敵の弾かどうか

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x - g_Bullet[nCnt].fSize, g_Bullet[nCnt].pos.y + g_Bullet[nCnt].fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x + g_Bullet[nCnt].fSize, g_Bullet[nCnt].pos.y + g_Bullet[nCnt].fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x - g_Bullet[nCnt].fSize, g_Bullet[nCnt].pos.y - g_Bullet[nCnt].fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x + g_Bullet[nCnt].fSize, g_Bullet[nCnt].pos.y - g_Bullet[nCnt].fSize, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = COLOR_WHITE;
		pVtx[1].col = COLOR_WHITE;
		pVtx[2].col = COLOR_WHITE;
		pVtx[3].col = COLOR_WHITE;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点情報のポインタを進める
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffBullet->Unlock();
}
//==========================
// 弾の終了処理
//==========================
void UninitBullet(void)
{
	// テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}
//==========================
// 弾の更新処理
//==========================
void UpdateBullet(void)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// 最大数分回す
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		// 使われているなら
		if (g_Bullet[nCnt].bUse == true)
		{
			// 場所を保存
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
				// エフェクトをセット
				SetEffect(g_Bullet[nCnt].pos, g_Bullet[nCnt].dir, 5, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 3.0f);
			}
			// 敵の弾なら
			else
			{
				// エフェクトをセット
				SetEffect(g_Bullet[nCnt].pos, g_Bullet[nCnt].dir, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, 3.0f);

				// 当たり判定を纏わせる
				if (sphererange(&g_Bullet[nCnt].pos, &pPlayer->pos, g_Bullet[nCnt].fSize, 30.0f))
				{
					// プレイヤーにダメージ
					HitPlayer(g_Bullet[nCnt].nDamege,false,0,0);

					// 未使用にする関数
					KillBullet(nCnt);
				}
			}

			// 寿命で死ぬ
			if (g_Bullet[nCnt].nLife <= 0)
			{
				// 未使用にする関数
				KillBullet(nCnt);
			}
		}
	}
}
//==========================
// 弾の描画処理
//==========================
void DrawBullet(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// ライトの無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_Bullet[nCnt].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Bullet[nCnt].mtxWorld);

			// ビューマトリックス格納用の変数
			D3DXMATRIX mtxView;

			// ビューマトリックスの設定
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// カメラの逆行列を設定
			g_Bullet[nCnt].mtxWorld._11 = mtxView._11;
			g_Bullet[nCnt].mtxWorld._12 = mtxView._21;
			g_Bullet[nCnt].mtxWorld._13 = mtxView._31;
			g_Bullet[nCnt].mtxWorld._21 = mtxView._12;
			g_Bullet[nCnt].mtxWorld._22 = mtxView._22;
			g_Bullet[nCnt].mtxWorld._23 = mtxView._32;
			g_Bullet[nCnt].mtxWorld._31 = mtxView._13;
			g_Bullet[nCnt].mtxWorld._32 = mtxView._23;
			g_Bullet[nCnt].mtxWorld._33 = mtxView._33;

			//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Bullet[nCnt].rot.y, g_Bullet[nCnt].rot.x, g_Bullet[nCnt].rot.z);
			//D3DXMatrixMultiply(&g_Bullet[nCnt].mtxWorld, &g_Bullet[nCnt].mtxWorld, &mtxRot);
			
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Bullet[nCnt].pos.x, g_Bullet[nCnt].pos.y, g_Bullet[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Bullet[nCnt].mtxWorld, &g_Bullet[nCnt].mtxWorld, &mtxTrans);

			// ワールドマトリックスを設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Bullet[nCnt].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	// ライトを有効化する
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//==========================
// 弾の設定処理
//==========================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 dir, int nLife, int nDamege, float fSize,float fSpeed, bool Enemy)
{// 左から場所、ベクトル、向き、寿命、ダメージ、大きさ、速度、敵味方の判別

	// 最大数分回す
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// 使われてないなら
		if (g_Bullet[nCntBullet].bUse == false)
		{
			g_Bullet[nCntBullet].nIdxShadow = SetShadow(g_Bullet[nCntBullet].pos, g_Bullet[nCntBullet].rot, fSize, 1.0f); // 影の設定
			g_Bullet[nCntBullet].pos = pos;				// 座標
			g_Bullet[nCntBullet].dir = dir;				// 方向ベクトル
			g_Bullet[nCntBullet].nLife = nLife;			// 寿命
			g_Bullet[nCntBullet].nDamege = nDamege;		// ダメージ量
			g_Bullet[nCntBullet].fSpeed = fSpeed;		// 移動速度
			g_Bullet[nCntBullet].fSize = fSize;			// 大きさ
			g_Bullet[nCntBullet].bEnemy = Enemy;		// 判別フラグ
			g_Bullet[nCntBullet].move = move * fSpeed;  // 移動量
			g_Bullet[nCntBullet].bUse = true;           // 使用状態

			break;
		}
	}
}
//==========================
// 弾の情報取得処理
//==========================
Bullet* GetBullet(void)
{
	return &g_Bullet[0];
}
//==========================
// 未使用に戻す処理
//==========================
void KillBullet(int nNum)
{
	g_Bullet[nNum].bUse = false; // 未使用状態にする
	KillShadow(g_Bullet[nNum].nIdxShadow); // 影を消す
}