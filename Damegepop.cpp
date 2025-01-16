//===================================
//
//�_���[�W�\���֘A�̃v���O����
//Author;�����I��
//
//===================================

#include "Main.h"
//#include "player.h"
#include "Damagepop.h"

// �\������ő吔�Ƃ��̌���
#define MAXDAMEGE (500)
#define MAXDIGI (6)
LPDIRECT3DTEXTURE9 g_pTexture_Damege = NULL;
LPDIRECT3DTEXTURE9 g_pTexture_Heal = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDamege = NULL;

// �\���́A���S�̏ォ�琔�l�A�\���t���[���A���݂��邩�ۂ��A�񕜂��ۂ�
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	D3DXMATRIX mtxWorld;
	int nDamege;
	int nLife;
	bool bUse;
	bool bHeal;
}Damege;
Damege g_aDamege[MAXDAMEGE];
int g_aPosTexU[MAXDIGI];

//=================
// Init�����F�X
//=================

void InitDamege(void)
{
	int nCntDamege;
	int nCnt = 0;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	// �_���[�W���󂯂����̃t�H���g
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\FuryNum.png", &g_pTexture_Damege);

	// �񕜂������̃t�H���g
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\CarmNum.png", &g_pTexture_Heal);

	// ������
	for (nCntDamege = 0; nCntDamege < MAXDAMEGE; nCntDamege++)
	{
		g_aDamege[nCntDamege].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aDamege[nCntDamege].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aDamege[nCntDamege].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aDamege[nCntDamege].nLife = 100;
		g_aDamege[nCntDamege].nDamege = 0;
		g_aDamege[nCntDamege].bUse = false;
		g_aDamege[nCntDamege].bHeal = false;
	}
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 6 * MAXDAMEGE * MAXDIGI, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffDamege, NULL);
	VERTEX_3D* pVtx;
	g_pVtxBuffDamege->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntDamege = 0; nCntDamege < MAXDAMEGE * MAXDIGI; nCntDamege++)
	{
		// �����傫���Ɗ�l
		pVtx[0].pos = D3DXVECTOR3(-25, 50, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(25, 50, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-25, -50, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(25, -50, 0.0f);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		/*pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);*/

		// �e�N�X�`���A���͂P�P�p�^�[���̂�Ȃ̂łP�O�p�^�[���Ȃ�11��10�ɂ��鎖
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 11.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 11.0f + (1.0f / 11.0f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 11.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 11.0f + (1.0f / 11.0f), 1.0f);

		pVtx += 4;

		nCnt++;
		if (nCnt >= MAXDIGI)
		{
			nCnt = 0;
		}
	}
	g_pVtxBuffDamege->Unlock();
}


//=================
// Uninit�����F�X
//=================

void UninitDamege(void)
{
	if (g_pTexture_Damege != NULL)
	{
		g_pTexture_Damege->Release();
		g_pTexture_Damege = NULL;
	}
	if (g_pVtxBuffDamege != NULL)
	{
		g_pVtxBuffDamege->Release();
		g_pVtxBuffDamege = NULL;
	}
	if (g_pTexture_Heal != NULL)
	{
		g_pTexture_Heal->Release();
		g_pTexture_Heal = NULL;
	}
}

//=================
// �X�V�����F�X
//=================

void UpdateDamege(void)
{
	int nCntDamege;
	int nMath;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	VERTEX_3D* pVtx;
	g_pVtxBuffDamege->Lock(0, 0, (void**)&pVtx, 0);
	// �S�Č��񂵂�
	for (nCntDamege = 0; nCntDamege < MAXDAMEGE; nCntDamege++)
	{
		// �������݂���Ȃ�
		if (g_aDamege[nCntDamege].bUse == true)
		{
			// �������Ȃ��瓧�������Ă���
			g_aDamege[nCntDamege].pos.y += g_aDamege[nCntDamege].move.y;
			g_aDamege[nCntDamege].col.a -= 0.01f;
			if (g_aDamege[nCntDamege].col.a < 0.0f)
			{
				g_aDamege[nCntDamege].col.a = 0.0f;
			}

			// ���̐��𑪂�V�X�e��
			g_aPosTexU[0] = g_aDamege[nCntDamege].nDamege / 100000 % 10;
			g_aPosTexU[1] = g_aDamege[nCntDamege].nDamege / 10000 % 10;
			g_aPosTexU[2] = g_aDamege[nCntDamege].nDamege / 1000 % 10;
			g_aPosTexU[3] = g_aDamege[nCntDamege].nDamege / 100 % 10;
			g_aPosTexU[4] = g_aDamege[nCntDamege].nDamege / 10 % 10;
			g_aPosTexU[5] = g_aDamege[nCntDamege].nDamege % 10;

			// �����ɂ���Đ��l��TEXTURE���W��ς���
			for (int nCnt = 0; nCnt < MAXDIGI; nCnt++)
			{
				nMath = pow(10, 5 - nCnt);
				// �P�̈ʈȊO���ő�̌����𒴂��Ă���Ȃ�\�����Ȃ�
				if (g_aDamege[nCntDamege].nDamege >= nMath || nCnt == 5)
				{
					pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 11.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 11.0f + (1.0f / 11.0f), 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 11.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 11.0f + (1.0f / 11.0f), 1.0f);
				}
				else
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
				}
				pVtx[0].col = g_aDamege[nCntDamege].col;
				pVtx[1].col = g_aDamege[nCntDamege].col;
				pVtx[2].col = g_aDamege[nCntDamege].col;
				pVtx[3].col = g_aDamege[nCntDamege].col;

				// �ꏊ�𒲐�����
				pVtx[0].pos = D3DXVECTOR3(-85 + (nCnt * 30), 50, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(-35 + (nCnt * 30), 50, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-85 + (nCnt * 30), -50, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(-35 + (nCnt * 30), -50, 0.0f);
				pVtx += 4;
			}

			// �����Ŏ���
			g_aDamege[nCntDamege].nLife--;
			if (g_aDamege[nCntDamege].nLife <= 0)
			{
				g_aDamege[nCntDamege].bUse = false;
			}
		}
		else
		{
			// 4�~�����Ȃ̂�24�i��
			pVtx += 4 * MAXDIGI;
		}
	}
	g_pVtxBuffDamege->Unlock();
}

//=================
// �`�ʏ����F�X
//=================
void DrawDamege(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�擾
	pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	//�A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 5);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCnt = 0; nCnt < MAXDAMEGE; nCnt++)
	{
		if (g_aDamege[nCnt].bUse == true)
		{
			// �r���{�[�h�̐F�X
			D3DXMATRIX mtxView;
			D3DXMatrixIdentity(&g_aDamege[nCnt].mtxWorld);
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			g_aDamege[nCnt].mtxWorld._11 = mtxView._11;
			g_aDamege[nCnt].mtxWorld._12 = mtxView._21;
			g_aDamege[nCnt].mtxWorld._13 = mtxView._31;
			g_aDamege[nCnt].mtxWorld._21 = mtxView._12;
			g_aDamege[nCnt].mtxWorld._22 = mtxView._22;
			g_aDamege[nCnt].mtxWorld._23 = mtxView._32;
			g_aDamege[nCnt].mtxWorld._31 = mtxView._13;
			g_aDamege[nCnt].mtxWorld._32 = mtxView._23;
			g_aDamege[nCnt].mtxWorld._33 = mtxView._33;

			g_aDamege[nCnt].mtxWorld._41 = 0.0f;
			g_aDamege[nCnt].mtxWorld._42 = 0.0f;
			g_aDamege[nCnt].mtxWorld._43 = 0.0f;
			mtxView._14 = g_aDamege[nCnt].mtxWorld._41;
			mtxView._24 = g_aDamege[nCnt].mtxWorld._42;
			mtxView._34 = g_aDamege[nCnt].mtxWorld._43;

			D3DXMatrixTranslation(&mtxTrans, g_aDamege[nCnt].pos.x, g_aDamege[nCnt].pos.y, g_aDamege[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aDamege[nCnt].mtxWorld, &g_aDamege[nCnt].mtxWorld, &mtxTrans);
			pDevice->SetTransform(D3DTS_WORLD, &g_aDamege[nCnt].mtxWorld);
			pDevice->SetStreamSource(0, g_pVtxBuffDamege, 0, sizeof(VERTEX_3D));

			// �񕜂Ȃ�񕜗p��TEXTURE���f�A�t�Ȃ�_���[�W�p��TEXTURE
			if (g_aDamege[nCnt].bHeal == true)
			{
				pDevice->SetTexture(0, g_pTexture_Heal);
			}
			else
			{
				pDevice->SetTexture(0, g_pTexture_Damege);
			}
			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);
			//�|���S���̕`��
			for (int nCnt2 = 0; nCnt2 < MAXDIGI; nCnt2++)
			{
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 + (nCnt * 4 * MAXDIGI) + (nCnt2 * 4), 2);
			}
		}
	}
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);*/
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=================
// �z�u�����F�X
//=================

// �����珇�ɏꏊ�A���l�A�����A�񕜂��ǂ���
void SetDamege(D3DXVECTOR3 pos, int nDamege, int nLife, bool Heal)
{
	int nCnt;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	VERTEX_2D* pVtx;
	g_pVtxBuffDamege->Lock(0, 0, (void**)&pVtx, 0);
	// �S�Č��񂵂�
	for (nCnt = 0; nCnt < MAXDAMEGE; nCnt++)
	{
		// �����ĂȂ��z�ɌĂт�����
		if (g_aDamege[nCnt].bUse == false)
		{
			g_aDamege[nCnt].pos = pos;
			g_aDamege[nCnt].nDamege = nDamege;
			g_aDamege[nCnt].nLife = nLife;
			g_aDamege[nCnt].bHeal = Heal;
			g_aDamege[nCnt].move = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
			g_aDamege[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aDamege[nCnt].bUse = true;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffDamege->Unlock();
}