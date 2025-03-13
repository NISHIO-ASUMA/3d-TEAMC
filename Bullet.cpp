//===================================
//
// �e�̏��� [ bullet.cpp ]
// Author:�����I��
//
//===================================

//***************************
// �C���N���[�h�t�@�C���錾
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
// �}�N����`
//***************************
#define MAX_BULLET (300)

//***************************
// �O���[�o���ϐ��錾
//***************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL; // ���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;      // �e�N�X�`���̃|�C���^
Bullet g_Bullet[MAX_BULLET];					 // �\���̕ϐ�

//==========================
// �e�̏���������
//==========================
void InitBullet(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, 
		"data\\TEXTURE\\BBBullet.png", 
		&g_pTextureBullet);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_BULLET * 4,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_3D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffBullet, 
		NULL);

	// ���_���̃|�C���^
	VERTEX_3D* pVtx = NULL;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// �\���̕ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{//���A��O�A�����}�C�i�X

		g_Bullet[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ꏊ
		g_Bullet[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �p�x
		g_Bullet[nCnt].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �����x�N�g��
		g_Bullet[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �ړ���
		g_Bullet[nCnt].bUse = false; // �g�p�̗L��
		g_Bullet[nCnt].nLife = 0;    // ����
		g_Bullet[nCnt].fSize = 3.0f; // �傫��
		g_Bullet[nCnt].fSpeed = 1.0f;// ���x
		g_Bullet[nCnt].bEnemy = true;// �G�̒e���ǂ���

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x - g_Bullet[nCnt].fSize, g_Bullet[nCnt].pos.y + g_Bullet[nCnt].fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x + g_Bullet[nCnt].fSize, g_Bullet[nCnt].pos.y + g_Bullet[nCnt].fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x - g_Bullet[nCnt].fSize, g_Bullet[nCnt].pos.y - g_Bullet[nCnt].fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Bullet[nCnt].pos.x + g_Bullet[nCnt].fSize, g_Bullet[nCnt].pos.y - g_Bullet[nCnt].fSize, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = COLOR_WHITE;
		pVtx[1].col = COLOR_WHITE;
		pVtx[2].col = COLOR_WHITE;
		pVtx[3].col = COLOR_WHITE;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_���̃|�C���^��i�߂�
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffBullet->Unlock();
}
//==========================
// �e�̏I������
//==========================
void UninitBullet(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}
//==========================
// �e�̍X�V����
//==========================
void UpdateBullet(void)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �ő吔����
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		// �g���Ă���Ȃ�
		if (g_Bullet[nCnt].bUse == true)
		{
			// �ꏊ��ۑ�
			g_Bullet[nCnt].posDest = g_Bullet[nCnt].pos;
			
			// move�̕��ړ�
			g_Bullet[nCnt].pos += g_Bullet[nCnt].move;
			
			// ��������
			g_Bullet[nCnt].nLife--;

			// �e�̏ꏊ�ړ�
			SetPositionShadow(g_Bullet[nCnt].nIdxShadow, g_Bullet[nCnt].pos, 2.0f + 2.0f * g_Bullet[nCnt].pos.y / 200.0f, 1.0f / (1.0f + g_Bullet[nCnt].pos.y / 30.0f));
			
			// �����̒e�Ȃ�
			if (g_Bullet[nCnt].bEnemy == false)
			{
				// �G�t�F�N�g���Z�b�g
				SetEffect(g_Bullet[nCnt].pos, g_Bullet[nCnt].dir, 5, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 3.0f);
			}
			// �G�̒e�Ȃ�
			else
			{
				// �G�t�F�N�g���Z�b�g
				SetEffect(g_Bullet[nCnt].pos, g_Bullet[nCnt].dir, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, 3.0f);

				// �����蔻���Z�킹��
				if (sphererange(&g_Bullet[nCnt].pos, &pPlayer->pos, g_Bullet[nCnt].fSize, 30.0f))
				{
					// �v���C���[�Ƀ_���[�W
					HitPlayer(g_Bullet[nCnt].nDamege,false,0,0);

					// ���g�p�ɂ���֐�
					KillBullet(nCnt);
				}
			}

			// �����Ŏ���
			if (g_Bullet[nCnt].nLife <= 0)
			{
				// ���g�p�ɂ���֐�
				KillBullet(nCnt);
			}
		}
	}
}
//==========================
// �e�̕`�揈��
//==========================
void DrawBullet(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �v�Z�p�̃}�g���b�N�X��錾
	D3DXMATRIX mtxRot, mtxTrans;

	// ���C�g�̖�����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_Bullet[nCnt].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Bullet[nCnt].mtxWorld);

			// �r���[�}�g���b�N�X�i�[�p�̕ϐ�
			D3DXMATRIX mtxView;

			// �r���[�}�g���b�N�X�̐ݒ�
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �J�����̋t�s���ݒ�
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
			
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Bullet[nCnt].pos.x, g_Bullet[nCnt].pos.y, g_Bullet[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Bullet[nCnt].mtxWorld, &g_Bullet[nCnt].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X��ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Bullet[nCnt].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	// ���C�g��L��������
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//==========================
// �e�̐ݒ菈��
//==========================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 dir, int nLife, int nDamege, float fSize,float fSpeed, bool Enemy)
{// ������ꏊ�A�x�N�g���A�����A�����A�_���[�W�A�傫���A���x�A�G�����̔���

	// �ő吔����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// �g���ĂȂ��Ȃ�
		if (g_Bullet[nCntBullet].bUse == false)
		{
			g_Bullet[nCntBullet].nIdxShadow = SetShadow(g_Bullet[nCntBullet].pos, g_Bullet[nCntBullet].rot, fSize, 1.0f); // �e�̐ݒ�
			g_Bullet[nCntBullet].pos = pos;				// ���W
			g_Bullet[nCntBullet].dir = dir;				// �����x�N�g��
			g_Bullet[nCntBullet].nLife = nLife;			// ����
			g_Bullet[nCntBullet].nDamege = nDamege;		// �_���[�W��
			g_Bullet[nCntBullet].fSpeed = fSpeed;		// �ړ����x
			g_Bullet[nCntBullet].fSize = fSize;			// �傫��
			g_Bullet[nCntBullet].bEnemy = Enemy;		// ���ʃt���O
			g_Bullet[nCntBullet].move = move * fSpeed;  // �ړ���
			g_Bullet[nCntBullet].bUse = true;           // �g�p���

			break;
		}
	}
}
//==========================
// �e�̏��擾����
//==========================
Bullet* GetBullet(void)
{
	return &g_Bullet[0];
}
//==========================
// ���g�p�ɖ߂�����
//==========================
void KillBullet(int nNum)
{
	g_Bullet[nNum].bUse = false; // ���g�p��Ԃɂ���
	KillShadow(g_Bullet[nNum].nIdxShadow); // �e������
}