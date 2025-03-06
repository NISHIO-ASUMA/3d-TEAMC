//==============================================================================================================
//
// �{�X�̗̑̓Q�[�W [ bosslife.cpp ]
// Author:Asuma Nishio
//
//==============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//**************************************************************************************************************
#include "boss.h"
#include "bosslife.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define NUM_BOSSLIFE (100) // �ő吔
#define MAX_LENGTH (100.0f) // ����
#define TOPPOS (105.0f) // �����΂��̈ʒu
#define UNDERPOS (90.0f) // �����΂��̈ʒu

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void UpdateLifegage(Boss* pBoss); // �{�X�̗̑̓o�[�̍X�V

//**************************************************************************************************************
// �O���[�o���ϐ��錾
//**************************************************************************************************************
BossLife g_BossLife[NUM_BOSSLIFE]; // �\���̕ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBossLife[BOSSTEX_MAX] = {}; // �e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossLife = NULL; // ���_�o�b�t�@�̃|�C���^

//==============================================================================================================
// �{�X�̗̑̓o�[�̏���������
//==============================================================================================================
void InitBossLife()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < BOSSTEX_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			BOSSLIFE_TEX[nCnt],
			&g_pTextureBossLife[nCnt]);
	}

	// �\���̕ϐ��̏�����
	for (int nCnt = 0; nCnt < NUM_BOSSLIFE; nCnt++)
	{
		g_BossLife[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// ���W
		g_BossLife[nCnt].nType = 0;			                // ���
		g_BossLife[nCnt].fLength = 0.0f;                    // �o�[�̒���
		g_BossLife[nCnt].bUse = false;						// �g�p����
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_BOSSLIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBossLife,
		NULL);

	// ���_���̃|�C���^
	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBossLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_BOSSLIFE; nCnt++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �e���_�̖@��(�x�N�g��)�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_���̃|�C���^��i�߂�
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffBossLife->Unlock();
}
//==============================================================================================================
// �{�X�̗̑̓o�[�̏I������
//==============================================================================================================
void UninitBossLife()
{
	// �e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < BOSSTEX_MAX; nCnt++)
	{
		if (g_pTextureBossLife[nCnt] != NULL)
		{
			g_pTextureBossLife[nCnt]->Release();
			g_pTextureBossLife[nCnt] = NULL;

		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBossLife != NULL)
	{
		g_pVtxBuffBossLife->Release();
		g_pVtxBuffBossLife = NULL;
	}

}
//==============================================================================================================
// �{�X�̗̑̓o�[�̍X�V����
//==============================================================================================================
void UpdateBossLife(Boss* pBoss)
{
	UpdateLifegage(pBoss);
}
//==============================================================================================================
// �{�X�̗̑̓o�[�̕`�揈��
//==============================================================================================================
void DrawBossLife()
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �v�Z�p�̃}�g���b�N�X��錾
	D3DXMATRIX mtxRot, mtxTrans;

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//// Z�e�X�g
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCnt = 0; nCnt < NUM_BOSSLIFE; nCnt++)
	{
		if (!g_BossLife[nCnt].bUse)
		{// �g���Ă��Ȃ��Ƃ�
			// ���̏�����ʂ炸�J�E���g��i�߂�
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_BossLife[nCnt].mtxWorld);

		// �r���[�}�g���b�N�X��錾
		D3DXMATRIX mtxView;

		// �r���[�}�g���b�N�X�̎擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �J�����̋t�s���ݒ�
		g_BossLife[nCnt].mtxWorld._11 = mtxView._11;
		g_BossLife[nCnt].mtxWorld._12 = mtxView._21;
		g_BossLife[nCnt].mtxWorld._13 = mtxView._31;
		g_BossLife[nCnt].mtxWorld._21 = mtxView._12;
		g_BossLife[nCnt].mtxWorld._22 = mtxView._22;
		g_BossLife[nCnt].mtxWorld._23 = mtxView._32;
		g_BossLife[nCnt].mtxWorld._31 = mtxView._13;
		g_BossLife[nCnt].mtxWorld._32 = mtxView._23;
		g_BossLife[nCnt].mtxWorld._33 = mtxView._33;

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,  g_BossLife[nCnt].pos.x - (MAX_LENGTH * 0.5f), g_BossLife[nCnt].pos.y, g_BossLife[nCnt].pos.z);
		D3DXMatrixMultiply(&g_BossLife[nCnt].mtxWorld, &g_BossLife[nCnt].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_BossLife[nCnt].mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffBossLife, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		if (g_BossLife[nCnt].bUse == true)
		{
			// ��ނ�ۑ�
			int nType = g_BossLife[nCnt].nType;

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBossLife[nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	//// Z�e�X�g
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//==============================================================================================================
// �{�X�̗̑̓o�[�̐ݒ菈��
//==============================================================================================================
int SetBossLife(D3DXVECTOR3 pos, int nType)
{
	// ���_���̃|�C���^
	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBossLife->Lock(0, 0, (void**)&pVtx, 0);

	int nCnt = 0;

	for (nCnt = 0; nCnt < NUM_BOSSLIFE; nCnt++)
	{
		if (g_BossLife[nCnt].bUse == false)
		{// ���g�p�Ȃ�
			g_BossLife[nCnt].pos = pos; // ���W
			g_BossLife[nCnt].nType = nType; // ���
			g_BossLife[nCnt].bUse = true; // �g�p����

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, TOPPOS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(MAX_LENGTH, TOPPOS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, UNDERPOS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(MAX_LENGTH, UNDERPOS, 0.0f);

			break;
		}
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffBossLife->Unlock();

	return nCnt;
}
//==============================================================================================================
// �{�X�̗̑̓o�[�̈ʒu�ݒ�
//==============================================================================================================
void SetPositionLifeBar(int nIdxBar, int nIdxFrame, D3DXVECTOR3 pos)
{
	g_BossLife[nIdxBar].pos = pos;
	g_BossLife[nIdxFrame].pos = pos;
}
//==============================================================================================================
// �{�X�̗̑̓o�[�̏���
//==============================================================================================================
void DeleateLifeBar(int nIdxBar, int nIdxFrame)
{
	g_BossLife[nIdxBar].bUse = false;
	g_BossLife[nIdxFrame].bUse = false;
}
//==============================================================================================================
// �{�X�̗̑̓o�[�̍X�V
//==============================================================================================================
void UpdateLifegage(Boss* pBoss)
{
	// ���_���̃|�C���^
	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBossLife->Lock(0, 0, (void**)&pVtx, 0);

	float fRateLife = (float)pBoss->nLife / (float)pBoss->nMaxLife;

	g_BossLife[pBoss->nLifeBarIdx].fLength = fRateLife * MAX_LENGTH;

	pVtx += 4 * pBoss->nLifeBarIdx;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, TOPPOS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_BossLife[pBoss->nLifeBarIdx].fLength, TOPPOS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, UNDERPOS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_BossLife[pBoss->nLifeBarIdx].fLength, UNDERPOS, 0.0f);

	// �A�����b�N
	g_pVtxBuffBossLife->Unlock();
}
