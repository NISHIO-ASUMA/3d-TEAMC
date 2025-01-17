//============================
//
// タイトル [title.cpp]
// Author: TEAM_C
//
//============================

//****************************
//インクルードファイル
//****************************
#include "title.h"
#include "input.h"

//****************************
//マクロ定義
//****************************
#define NUM_TITLE (2) // タイトルのテクスチャの数

//****************************
//グローバル宣言
//****************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLETYPE_MAX] = {}; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL; // 頂点バッファへのポインタ
TITLE g_Title[NUM_TITLE];

//============================
//タイトルの初期化処理
//============================
void InitTitle(void)
{
	VERTEX_2D* pVtx;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタ

	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TITLE_TYPE[nCnt],
			&g_pTextureTitle[nCnt]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//頂点バッファをロック
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		g_Title[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Title[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Title[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Title[nCnt].bUse = false;
		g_Title[nCnt].fHeight = 0.0f;
		g_Title[nCnt].fWidth = 0.0f;
		g_Title[nCnt].nType = TITLETYPE_TITLE;
		g_Title[nCnt].TitleMenu = TITLESELECT_TITLE;

		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		pVtx += 4;
	}

	SetTitle(D3DXVECTOR3(640.0f, 200.0f, 0.0f), TITLETYPE_TITLE, 200.0f, 50.0f);
	SetTitle(D3DXVECTOR3(640.0f, 500.0f, 0.0f), TITLETYPE_TUTO, 200.0f, 50.0f);

	//頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}
//============================
//タイトルの終了処理
//============================
void UninitTitle(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
//============================
//タイトルの更新処理
//============================
void UpdateTitle(void)
{
	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		switch (g_Title[nCnt].TitleMenu)
		{
		case TITLESELECT_TITLE:
			if (KeyboardTrigger(DIK_DOWN))
			{
				g_Title[nCnt].TitleMenu = TITLESELECT_TUTO;
			}
			SelectTitle(0);

			break;
		case TITLESELECT_TUTO:
			if (KeyboardTrigger(DIK_UP))
			{
				g_Title[nCnt].TitleMenu = TITLESELECT_TITLE;
			}
			SelectTitle(1);
			break;
		default:
			break;
		}
	}
}
//============================
//タイトルの描画処理
//============================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 	pDevice = GetDevice();//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[g_Title[nCnt].nType]);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);//プリミティブの種類
	}
}
//============================
//タイトルの描画処理
//============================
void SetTitle(D3DXVECTOR3 pos, int nType,float fWidth, float fHeight)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロック
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		//未使用だったら
		if (!g_Title[nCnt].bUse)
		{
			g_Title[nCnt].pos = pos;
			g_Title[nCnt].nType = nType;
			g_Title[nCnt].bUse = true;

			switch (nType)
			{
			case TITLETYPE_TITLE:
				//頂点座標の更新
				pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);
				break;
			case TITLETYPE_TUTO:
				//頂点座標の更新
				pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);
				break;
			default:
				break;
			}
			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}

//============================
//タイトルの選択処理
//============================
void SelectTitle(int select)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロック
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		if (nCnt == select)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}
