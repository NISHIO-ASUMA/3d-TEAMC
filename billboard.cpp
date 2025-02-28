//==================================
//
// �r���{�[�h���� [billboard.cpp]
// Author: Asuma Nishio
//
//==================================

//***************************
// �C���N���[�h�t�@�C���錾
//***************************
#include "billboard.h"
#include "block.h"
#include "game.h"
#include "input.h"
#include "player.h"
#include "easing.h"

//*****************************
// �O���[�o���ϐ��錾
//*****************************
Billboard g_Billboard[MAX_BILLBOARD];				// �\���̕ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	// ���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureBillboard[BILLBOARDTYPE_MAX] = {};// �e�N�X�`���̃|�C���^
int EasingCount, EasingCount1;

//=========================
//�@�r���{�[�h����������
//=========================
void InitBillboard(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < BILLBOARDTYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			BILLBOARD_TEXTURE[nCnt],
			&g_apTextureBillboard[nCnt]);
	}

	EasingCount = 0;
	EasingCount1 = 0;

	// �\���̕ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		g_Billboard[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���W
		g_Billboard[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �p�x
		g_Billboard[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_Billboard[nCnt].nType = 0;			                // ���
		g_Billboard[nCnt].fHeight = 0.0f;						// ����
		g_Billboard[nCnt].fWidth = 0.0f;						// ����
		g_Billboard[nCnt].state = BILLBOARDSTATE_NOSET;
		g_Billboard[nCnt].bUse = false;							// �g�p����
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	// ���_���̃|�C���^
	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
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
	g_pVtxBuffBillboard->Unlock();

}
//=========================
//�@�r���{�[�h�I������
//=========================
void UninitBillboard(void)
{
	// �e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < BILLBOARDTYPE_MAX; nCnt++)
	{
		if (g_apTextureBillboard[nCnt] != NULL)
		{
			g_apTextureBillboard[nCnt]->Release();
			g_apTextureBillboard[nCnt] = NULL;

		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{			 
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}

}
//=========================
//�@�r���{�[�h�X�V����
//=========================
void UpdateBillboard(void)
{
	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (!g_Billboard[nCnt].bUse)
		{// false��������
			continue;
		}

		switch (g_Billboard[nCnt].nType)
		{
		case BILLBOARDTYPE_FIRST:

			break;
		default:
			break;
		}

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_Billboard[nCnt].fWidth, g_Billboard[nCnt].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Billboard[nCnt].fWidth, g_Billboard[nCnt].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_Billboard[nCnt].fWidth, -g_Billboard[nCnt].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Billboard[nCnt].fWidth, -g_Billboard[nCnt].fHeight, 0.0f);

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffBillboard->Unlock();
}
//=========================
//�@�r���{�[�h�`�揈��
//=========================
void DrawBillboard(void)
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

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (!g_Billboard[nCnt].bUse)
		{// �g���Ă��Ȃ��Ƃ�
			// ���̏�����ʂ炸�J�E���g��i�߂�
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Billboard[nCnt].mtxWorld);

		// �r���[�}�g���b�N�X��錾
		D3DXMATRIX mtxView;

		// �r���[�}�g���b�N�X�̎擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �J�����̋t�s���ݒ�
		g_Billboard[nCnt].mtxWorld._11 = mtxView._11;
		g_Billboard[nCnt].mtxWorld._12 = mtxView._21;
		g_Billboard[nCnt].mtxWorld._13 = mtxView._31;
		g_Billboard[nCnt].mtxWorld._21 = mtxView._12;
		g_Billboard[nCnt].mtxWorld._22 = mtxView._22;
		g_Billboard[nCnt].mtxWorld._23 = mtxView._32;
		g_Billboard[nCnt].mtxWorld._31 = mtxView._13;
		g_Billboard[nCnt].mtxWorld._32 = mtxView._23;
		g_Billboard[nCnt].mtxWorld._33 = mtxView._33;

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Billboard[nCnt].pos.x, g_Billboard[nCnt].pos.y, g_Billboard[nCnt].pos.z);
		D3DXMatrixMultiply(&g_Billboard[nCnt].mtxWorld, &g_Billboard[nCnt].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Billboard[nCnt].mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		if (g_Billboard[nCnt].state != BILLBOARDSTATE_NOSET)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureBillboard[g_Billboard[nCnt].nType]);

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
//=========================
//�@�r���{�[�h�ݒ菈��
//=========================
void SetBillboard(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight,int state)
{
	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (!g_Billboard[nCnt].bUse)
		{// false��������

			g_Billboard[nCnt].pos = pos;	// ���W
			g_Billboard[nCnt].nType = nType;// ���
			g_Billboard[nCnt].state = state;// ���
			g_Billboard[nCnt].fWidth = fWidth; // ����
			g_Billboard[nCnt].fHeight = fHeight; // ����
			g_Billboard[nCnt].bUse = true;	// �g�p����

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-fWidth, fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fWidth,  fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fWidth,  -fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fWidth,  -fHeight, 0.0f);

			break;
		}

		// ���_���̃|�C���^��i�߂�
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffBillboard->Unlock();

}
//=========================================
//�@�r���{�[�h�̕\��
//=========================================
void CraftRange(BLOCK* pBlock)
{
	Player* pPlayer = GetPlayer();

	// �r���{�[�h����
	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		// �g�p��Ԃ���Ȃ������炩��ނ�
		if (!g_Billboard[nCnt].bUse && g_Billboard[nCnt].nType != BILLBOARDTYPE_FIRST)
		{
			continue;
		}

		// �\���͈͓��ɓ�����
		if (sphererange(&pBlock->pos, &pPlayer->pos, 100.0f, 50.0f) && pBlock->nType == BLOCKTYPE_WORKBENCH)
		{
			// ������悤�ɂ���
			g_Billboard[nCnt].state = BILLBOARDSTATE_SET;

			g_Billboard[nCnt].fHeight = 20.0f;
			g_Billboard[nCnt].fWidth = 30.0f;

			//EasingCount++;

			//float t0 = SetEase(EasingCount, 120);

			//// ������30�ȏォ�c����20.0f�ȏ�ɂȂ�����
			//if (g_Billboard[nCnt].fWidth >= 30.0f && g_Billboard[nCnt].fHeight >= 20.0f)
			//{
			//	// �����̊g����~�߂�
			//	g_Billboard[nCnt].fWidth = 30.0f;
			//}

			//// ������30�ȉ����c����20.0f�ȏ�ɂȂ�����


			//// �傫����20.0f�ȏ�ɂȂ�����
			//if (g_Billboard[nCnt].fHeight >= 20.0f)
			//{
			//	EasingCount1++;

			//	float t1 = SetEase(EasingCount1, 120);

			//	// �������g��
			//	g_Billboard[nCnt].fWidth += (30.0f - g_Billboard[nCnt].fWidth) * EaseInCubic(t1);

			//	// �����̊g����~�߂�
			//	g_Billboard[nCnt].fHeight = 20.0f;
			//}

			//// �������g�傷��
			//g_Billboard[nCnt].fHeight += (20.0f - g_Billboard[nCnt].fHeight) * EaseInCubic(t0);
		}
		// �\���͈͂���o��
		else if (!sphererange(&pBlock->pos, &pPlayer->pos, 100.0f, 50.0f) && pBlock->nType == BLOCKTYPE_WORKBENCH)
		{

			//// �����Ȃ�����
			//g_Billboard[nCnt].state = BILLBOARDSTATE_NOSET;
		}

		// �E����͈͂ɓ�����
		if (sphererange(&pBlock->pos, &pPlayer->pos, 20.0f, 50.0f) && pBlock->nType == BLOCKTYPE_WORKBENCH)
		{
			// �N���t�g��Ԃ���Ȃ�������
			if ((KeyboardTrigger(DIK_TAB) || JoypadTrigger(JOYKEY_Y)) && !pPlayer->bCraft && pPlayer->AttackSp == false)
			{
				// �N���t�g���
				pPlayer->bCraft = true;

				// �N���t�g��Ԃɂ���
				EnableCraft(true);
				break;
			}
			// �N���t�g��Ԃ�������
			else if ((KeyboardTrigger(DIK_TAB) || JoypadTrigger(JOYKEY_Y)) && pPlayer->bCraft && pPlayer->AttackSp == false)
			{
				// �N���t�g���
				pPlayer->bCraft = false;

				// �N���t�g��Ԃɂ��Ȃ�
				EnableCraft(false);
				break;
			}
		}
	}
}