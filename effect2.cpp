//=====================================================================================================
//
// �G�t�F�N�g[effect.cpp]
// Author:YOSHIDA YUUTO
//
//=====================================================================================================

//*****************************************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************************************
#include "camera.h"
#include "player.h"
#include "input.h"
#include "effect2.h"

//*****************************************************************************************************
//�}�N����`
//*****************************************************************************************************
#define MAX_EFFECT (4096) //�����̍ő吔
#define XTEX (8) // �e�N�X�`���̉���
#define YTEX (1) // �e�N�X�`���̍���
#define UV (1.0f / XTEX)
#define HV (1.0f / YTEX)

//*****************************************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************************************
void SetTextureAnim(int nCnt, int XtexPos, int YtexPos, int Animspeed);

//*****************************************************************************************************
//�O���[�o���ϐ��錾
//*****************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureEffectX[EFFECT_MAX] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffectX = NULL; //���_�o�b�t�@�ւ̃|�C���^
EFFECT g_EffectX[MAX_EFFECT];

//=====================================================================================================
// �G�t�F�N�g�̏���������
//=====================================================================================================
void InitEffectX(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�̎擾

	// ��ޕ���
	for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			EFFECT_TEXTURE[nCnt],
			&g_pTextureEffectX[nCnt]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffectX,
		NULL);

	// ���_�o�b�t�@�̃|�C���^
	VERTEX_3D* pVtx = NULL;

	// �G�t�F�N�g����
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		g_EffectX[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �ʒu
		g_EffectX[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړ��� 
		g_EffectX[nCnt].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f); // �F 
		g_EffectX[nCnt].nLife = 0; // ����
		g_EffectX[nCnt].g_nCounterAnim = 0; // �A�j���[�V�����J�E���^�[
		g_EffectX[nCnt].g_nPatternAnim = 0; // �A�j���[�V�����J�E���^�[
		g_EffectX[nCnt].decfAlv = 0.02f; // ���l�̌����l
		g_EffectX[nCnt].nType = EFFECTTYPE_PLAYER; // ���
		g_EffectX[nCnt].fRadius = 20.0f; // ���a
		g_EffectX[nCnt].EffectType = EFFECT_NORMAL; // �G�t�F�N�g�̎��
		g_EffectX[nCnt].bUse = false; // �ʒu
	}
	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffectX->Lock(0, 0, (void**)&pVtx, 0);

	//�G�t�F�N�g����
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_EffectX[nCnt].pos.x - g_EffectX[nCnt].fRadius, g_EffectX[nCnt].pos.y + g_EffectX[nCnt].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_EffectX[nCnt].pos.x + g_EffectX[nCnt].fRadius, g_EffectX[nCnt].pos.y + g_EffectX[nCnt].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_EffectX[nCnt].pos.x - g_EffectX[nCnt].fRadius, g_EffectX[nCnt].pos.y - g_EffectX[nCnt].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_EffectX[nCnt].pos.x + g_EffectX[nCnt].fRadius, g_EffectX[nCnt].pos.y - g_EffectX[nCnt].fRadius, 0.0f);

		//�e���_�̖@���̐ݒ�(�x�N�g���̑傫���͂P�ɂ���K�v������)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);//�J�����ɑ΂��Đ�����
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffEffectX->Unlock();
}
//=====================================================================================================
// �G�t�F�N�g�I������
//=====================================================================================================
void UninitEffectX(void)
{
	// �e�N�X�`���̉��
	for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
	{
		if (g_pTextureEffectX[nCnt] != NULL)
		{
			g_pTextureEffectX[nCnt]->Release();
			g_pTextureEffectX[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̉��
	if (g_pVtxBuffEffectX != NULL)
	{
		g_pVtxBuffEffectX->Release();
		g_pVtxBuffEffectX = NULL;
	}
}
//=====================================================================================================
//�G�t�F�N�g�X�V����
//=====================================================================================================
void UpdateEffectX(void)
{
	// ���_�o�b�t�@�ւ̃|�C���^
	VERTEX_3D* pVtx;

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffectX->Lock(0, 0, (void**)&pVtx, 0);

	// �G�t�F�N�g����
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		// ���g�p��������
		if (!g_EffectX[nCnt].bUse)
		{
			//������ǂݔ�΂�
			continue;
		}

		// �ʒu�̍X�V
		g_EffectX[nCnt].pos += g_EffectX[nCnt].move;

		// ���������炷
		g_EffectX[nCnt].nLife--;

		// ���l������������
		g_EffectX[nCnt].col.a -= g_EffectX[nCnt].decfAlv;

		float fRadius = g_EffectX[nCnt].fRadius;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fRadius, fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRadius, fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fRadius, -fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRadius, - fRadius, 0.0f);

		//���_�J���[
		pVtx[0].col = g_EffectX[nCnt].col;
		pVtx[1].col = g_EffectX[nCnt].col;
		pVtx[2].col = g_EffectX[nCnt].col;
		pVtx[3].col = g_EffectX[nCnt].col;

		// �A�j���[�V�����̐ݒ�
		if (g_EffectX[nCnt].EffectType == EFFECT_SMORK)
		{
			SetTextureAnim(nCnt, 8, 1,15);
		}
		if (g_EffectX[nCnt].EffectType == EFFECT_SOUL)
		{
			SetTextureAnim(nCnt, 4, 1, 5);
		}
		if (g_EffectX[nCnt].EffectType == EFFECT_WATER)
		{
			SetTextureAnim(nCnt, 5, 1, 5);
		}
		if(g_EffectX[nCnt].EffectType == EFFECT_NORMAL)
		{
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// �������s���������
		if (g_EffectX[nCnt].nLife <= 0)
		{
			g_EffectX[nCnt].bUse = false;
		}
		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffEffectX->Unlock();
}
//=====================================================================================================
// �G�t�F�N�g�̕`�揈��
//=====================================================================================================
void DrawEffectX(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�̎擾

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxView, mtxScal;

	// �G�t�F�N�g����
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		// �G�t�F�N�g���g���Ă�����
		if (g_EffectX[nCnt].bUse == true)
		{
			// ��ނ���
			int EffectType = g_EffectX[nCnt].EffectType;

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			if (EffectType != EFFECT_SMORK && EffectType != EFFECT_WATER)
			{
				//�A���t�@�e�X�g��L��
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			//�[�b�g�e�X�g
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//a�u�����f�B���O
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, NULL);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_EffectX[nCnt].mtxWorld);

			pDevice->GetTransform(D3DTS_VIEW, &mtxView);//�r���[�}�g���b�N�X�̎擾

			//�J�����̋t�s��̐ݒ�
			g_EffectX[nCnt].mtxWorld._11 = mtxView._11;
			g_EffectX[nCnt].mtxWorld._12 = mtxView._21;
			g_EffectX[nCnt].mtxWorld._13 = mtxView._31;
			g_EffectX[nCnt].mtxWorld._21 = mtxView._12;
			g_EffectX[nCnt].mtxWorld._22 = mtxView._22;
			g_EffectX[nCnt].mtxWorld._23 = mtxView._32;
			g_EffectX[nCnt].mtxWorld._31 = mtxView._13;
			g_EffectX[nCnt].mtxWorld._32 = mtxView._23;
			g_EffectX[nCnt].mtxWorld._33 = mtxView._33;
			

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_EffectX[nCnt].pos.x, g_EffectX[nCnt].pos.y, g_EffectX[nCnt].pos.z);
			D3DXMatrixMultiply(&g_EffectX[nCnt].mtxWorld, &g_EffectX[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_EffectX[nCnt].mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffectX, 0, sizeof(VERTEX_3D));

			//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffectX[EffectType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);

			//���C�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//a�u�����f�B���O�����Ƃɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			//�A���t�@�e�X�g�𖳌�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//�[�b�g�e�X�g
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		}
	}
}
//=====================================================================================================
//�G�t�F�N�g�̐ݒ菈��
//=====================================================================================================
void SetEffectX(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius, float decfAlv, int EffectType)//�G�t�F�N�g�̐ݒ菈��
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffectX->Lock(0, 0, (void**)&pVtx, 0);

	// �G�t�F�N�g����
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		// ���g�p��������
		if (g_EffectX[nCnt].bUse == false)
		{
			g_EffectX[nCnt].pos = pos; // �ʒu
			g_EffectX[nCnt].fRadius = fRadius; // ���a
			g_EffectX[nCnt].move = move; // �ړ���
			g_EffectX[nCnt].col = col; // �F
			g_EffectX[nCnt].nLife = nLife; // ����
			g_EffectX[nCnt].decfAlv = decfAlv; // ���l�̌����l
			g_EffectX[nCnt].EffectType = EffectType; // �G�t�F�N�g�̎��
			g_EffectX[nCnt].bUse = true;// �g�p��Ԃɂ���

			switch (EffectType)
			{
			case EFFECT_NORMAL:
				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				break;
			case EFFECT_SMORK:
				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);
				break;
			case EFFECT_SOUL:
				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
				break;
			case EFFECT_WATER:
				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);
				break;
			default:
				break;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-fRadius, fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fRadius, fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fRadius, -fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fRadius, -fRadius, 0.0f);

			break;
		}
		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffEffectX->Unlock();

}
//=====================================================================================================
// �G�t�F�N�g�̃e�N�X�`���A�j���[�V����
//=====================================================================================================
void SetTextureAnim(int nCnt,int XtexPos, int YtexPos,int Animspeed)
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffectX->Lock(0, 0, (void**)&pVtx, 0);

	// �A�j���[�V�����J�E���^�[�����Z
	g_EffectX[nCnt].g_nCounterAnim++;

	// �e�N�X�`�����W���v�Z
	float Uv = 1.0f / XtexPos;
	float Hv = 1.0f / YtexPos;

	// �A�j���[�V�����̃X�s�[�h
	if (g_EffectX[nCnt].g_nCounterAnim >= Animspeed)
	{
		g_EffectX[nCnt].g_nCounterAnim = NULL;//�J�E���^�[�������l�ɖ߂�

		g_EffectX[nCnt].g_nPatternAnim++;//�p�^�[���i���o�[���X�V

		pVtx += 4 * nCnt;

		// �e�N�X�`�����W�̍X�V
		pVtx[0].tex = D3DXVECTOR2(g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv);
		pVtx[1].tex = D3DXVECTOR2(Uv + g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv);
		pVtx[2].tex = D3DXVECTOR2(g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv + Hv);
		pVtx[3].tex = D3DXVECTOR2(Uv + g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv + Hv);
	}

	// �p�^�[�����ő�ɂȂ�����
	if (g_EffectX[nCnt].g_nPatternAnim > XtexPos)
	{
		g_EffectX[nCnt].g_nPatternAnim = 0;//�p�^�[���i���o�[�������l�ɖ߂�
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffEffectX->Unlock();
}
