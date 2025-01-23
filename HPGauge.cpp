//===================================
//
//HPゲージのプログラム
//Author;長尾悠成
//
//===================================
#include "Main.h"
#include "player.h"
#include "HPGauge.h"
LPDIRECT3DTEXTURE9 g_pTexture_Gauge = NULL;
LPDIRECT3DTEXTURE9 g_pTexture_Frame = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;

// 各ゲージの計算用変数
float g_fLength;
float g_fMaxLength;
float g_fPer;

//=================
// Init処理色々
//=================

void InitGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	// フレームとゲージのテクスチャ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\lifebar_frame.png", &g_pTexture_Frame);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\lifebar_gage.png", &g_pTexture_Gauge);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 8, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGauge, NULL);
	VERTEX_2D* pVtx;

	// 位置の調整、都合の良い場所探して下しあ(上が枠、下がゲージ)
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(5.0f, 9.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(975.0f, 9.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(5.0f, 30.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(975.0f, 30.0f, 0.0f);
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	g_pVtxBuffGauge->Unlock();

	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[4].pos = D3DXVECTOR3(20.0f, 15.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(960.0f, 15.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(20.0f, 26.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(960.0f, 26.0f, 0.0f);
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
	g_pVtxBuffGauge->Unlock();
}

//=================
// Uninit処理色々
//=================
void UninitGauge(void)
{
	if (g_pTexture_Frame != NULL)
	{
		g_pTexture_Frame->Release();
		g_pTexture_Frame = NULL;
	}
	if (g_pTexture_Gauge != NULL)
	{
		g_pTexture_Gauge->Release();
		g_pTexture_Gauge = NULL;
	}
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//=================
// 更新処理色々
//=================
void UpdateGauge(void)
{
	//// 残りHPと最大HPを測り反映
	//float fLeftHP;
	//float fMaxHP;
	//Player* pPlayer = GetPlayer();
	//fLeftHP = pPlayer->nLeftHP;
	//fMaxHP = pPlayer->nMaxHP;

	//// そこからその割合を計算し長さにする
	//g_fPer = fLeftHP / fMaxHP;
	//g_fLength = g_fPer * 940;
	//LPDIRECT3DDEVICE9 pDevice;
	//pDevice = GetDevice();
	//VERTEX_2D* pVtx;
	//g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	//// それを描写し直す
	//pVtx[4].pos = D3DXVECTOR3(20.0f, 15.0f, 0.0f);
	//pVtx[5].pos = D3DXVECTOR3(g_fLength + 20.0f, 15.0f, 0.0f);
	//pVtx[6].pos = D3DXVECTOR3(20.0f, 26.0f, 0.0f);
	//pVtx[7].pos = D3DXVECTOR3(g_fLength + 20.0f, 26.0f, 0.0f);
	//pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[5].tex = D3DXVECTOR2(g_fPer, 0.0f);
	//pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[7].tex = D3DXVECTOR2(g_fPer, 1.0f);
	//g_pVtxBuffGauge->Unlock();
}

//=================
// 描写処理色々
//=================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイス取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, g_pTexture_Frame);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pDevice->SetTexture(0, g_pTexture_Gauge);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
}

//=================
// 変更処理色々...でもほぼ同じだしもう要らないかも、一応保管
//=================
void SetGauge(void)
{
	//float fLeftHP;
	//float fMaxHP;
	//Player* pPlayer = GetPlayer();
	//fLeftHP = pPlayer->nLeftHP;
	//fMaxHP = pPlayer->nMaxHP;
	//g_fPer = fLeftHP / fMaxHP;
	//g_fLength = g_fPer * 940;
	//LPDIRECT3DDEVICE9 pDevice;
	//pDevice = GetDevice();
	//VERTEX_2D* pVtx;
	//g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);
	//pVtx[4].pos = D3DXVECTOR3(20.0f, 15.0f, 0.0f);
	//pVtx[5].pos = D3DXVECTOR3(g_fLength + 20.0f, 15.0f, 0.0f);
	//pVtx[6].pos = D3DXVECTOR3(20.0f, 26.0f, 0.0f);
	//pVtx[7].pos = D3DXVECTOR3(g_fLength + 20.0f, 26.0f, 0.0f);
	//pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[5].tex = D3DXVECTOR2(g_fPer, 0.0f);
	//pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[7].tex = D3DXVECTOR2(g_fPer, 1.0f);
	//g_pVtxBuffGauge->Unlock();
}
