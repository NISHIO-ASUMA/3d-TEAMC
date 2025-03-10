//===================================
//
//チュートリアル解説のプログラム
//Author;長尾悠成
//
//===================================
#include "Main.h"
#include "player.h"
#include "TutorialSupport.h"
#include "sound.h"
#include "camera.h"
#include "input.h"
#include "spgauge.h"

LPDIRECT3DTEXTURE9 g_pTexture_Tutorial[10] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManager = NULL;
int nSteps = 0;
float fALv2 = 1.0f;
float bAmove = 0.0f;
float fPlusX = 0.0f;

void InitManager(void)
{
	nSteps = 0;
	fALv2 = 1.0f;
	bAmove = 0.0f;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial_Temporary\\tuto_step_1.png", &g_pTexture_Tutorial[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial_Temporary\\tuto_step_4.png", &g_pTexture_Tutorial[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial_Temporary\\tuto_step_3.png", &g_pTexture_Tutorial[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial_Temporary\\tuto_step_2.png", &g_pTexture_Tutorial[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial_Temporary\\tuto_step_3.5.png", &g_pTexture_Tutorial[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial_Temporary\\tuto_step_5.png", &g_pTexture_Tutorial[5]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial_Temporary\\tuto_step_6.png", &g_pTexture_Tutorial[6]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial_Temporary\\tuto_key_7.png", &g_pTexture_Tutorial[7]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\UI_craft.png", &g_pTexture_Tutorial[8]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial_Temporary\\tuto_step_8.png", &g_pTexture_Tutorial[9]);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffManager, NULL);
	VERTEX_2D* pVtx;
	g_pVtxBuffManager->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(950.0f + fPlusX, 70.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1250.0f + fPlusX, 70.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(950.0f + fPlusX, 240.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1250.0f + fPlusX, 240.0f, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	g_pVtxBuffManager->Unlock();
}

void UninitManager(void)
{
	if (g_pVtxBuffManager != NULL)
	{
		g_pVtxBuffManager->Release();
		g_pVtxBuffManager = NULL;
	}
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		if (g_pTexture_Tutorial[nCnt] != NULL)
		{
			g_pTexture_Tutorial[nCnt]->Release();
			g_pTexture_Tutorial[nCnt] = NULL;
		}
	}
}

void UpdateManager(void)
{
	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();
	Camera* pCamera = GetCamera();
	VERTEX_2D* pVtx;
	g_pVtxBuffManager->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[0].pos = D3DXVECTOR3(950.0f + fPlusX, 70.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1250.0f + fPlusX, 70.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(950.0f + fPlusX, 240.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1250.0f + fPlusX, 240.0f, 0.0f);
	g_pVtxBuffManager->Unlock();

	fALv2 += bAmove;
	if (fALv2 > 1.0f)
	{
		fALv2 = 1.0f;
		bAmove = 0.0f;
	}
	else if (fALv2 < 0.0f)
	{
		fALv2 = 0.0f;
		nSteps++;
		bAmove *= -1.0f;
	}
	fPlusX += bAmove * -100;

	if (nSteps == 0 && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_MOVE && fALv2 == 1.0f)
	{
		PlaySound(SOUND_LABEL_ENTER_SE);
		bAmove = -0.01f;
	}
	else if (nSteps == 1 && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_JUMP && fALv2 == 1.0f)
	{
		PlaySound(SOUND_LABEL_ENTER_SE);
		bAmove = -0.01f;
	}
	else if (nSteps == 2 && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_AVOID && fALv2 == 1.0f)
	{
		PlaySound(SOUND_LABEL_ENTER_SE);
		bAmove = -0.01f;
	}
	else if (nSteps == 3 && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_ACTION && fALv2 == 1.0f)
	{
		if (pPlayer->AttackSp == false)
		{
			PlaySound(SOUND_LABEL_ENTER_SE);
			AddSpgauge(300.0f);
			bAmove = -0.01f;
		}
	}
	else if (nSteps == 4 && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_ACTION && fALv2 == 1.0f)
	{
		if (pPlayer->AttackSp == true)
		{
			PlaySound(SOUND_LABEL_ENTER_SE);
			bAmove = -0.01f;
		}
	}
	else if (nSteps == 5 && pPlayer->Motion.nNumModel == 16 && fALv2 == 1.0f)
	{
		PlaySound(SOUND_LABEL_ENTER_SE);
		bAmove = -0.01f;
	}
	else if (nSteps == 6 && pItem[pPlayer->StockItemIdx].state == ITEMSTATE_STOCK && fALv2 == 1.0f)
	{
		PlaySound(SOUND_LABEL_ENTER_SE);
		bAmove = -0.01f;
	}
	else if (nSteps == 7 && fALv2 == 1.0f)
	{
		if (pItem[pPlayer->StockItemIdx].nType == ITEMTYPE_STONE && pItem[pPlayer->ItemIdx].nType == ITEMTYPE_BAT)
		{
			PlaySound(SOUND_LABEL_ENTER_SE);
			bAmove = -0.01f;
		}
		else if (pItem[pPlayer->StockItemIdx].nType == ITEMTYPE_BAT && pItem[pPlayer->ItemIdx].nType == ITEMTYPE_STONE)
		{
			PlaySound(SOUND_LABEL_ENTER_SE);
			bAmove = -0.01f;
		}
	}
	else if (nSteps == 8 && fALv2 == 1.0f)
	{
		if (pItem[pPlayer->ItemIdx].nType == ITEMTYPE_STONEBAT)
		{
			PlaySound(SOUND_LABEL_ENTER_SE);
			bAmove = -0.01f;
		}
	}
}

void DrawManager(void)
{
	int nCount;
	LPDIRECT3DDEVICE9 pDevice;
	//デバイス取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに
	pDevice->SetStreamSource(0, g_pVtxBuffManager, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, g_pTexture_Tutorial[nSteps]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}