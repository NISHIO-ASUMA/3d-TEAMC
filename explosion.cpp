//============================
//
// ���� [explosion.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "explosion.h"
#include "camera.h"
#include "player.h"

//****************************
//�}�N����`
//****************************
#define EXPLOSION_WIDTH (25.0f) //����
#define EXPLOSION_HEIGHT (25.0f) //����
#define MAX_EXPLOSION (80) //�����̍ő吔
#define LANDINGEXPLOSION (8)

//****************************
//�v���g�^�C�v�錾
//****************************

//****************************
//�O���[�o���ϐ��錾
//****************************
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL; //���_�o�b�t�@�ւ̃|�C���^
EXPLOSION g_Explosion[MAX_EXPLOSION];

//============================
//�����̏���������
//============================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\explosion.png",
		&g_pTextureExplosion);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_3D* pVtx = NULL;

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		g_Explosion[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Explosion[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Explosion[nCnt].nLife = 0;
		g_Explosion[nCnt].nCounterAnim = 0;
		g_Explosion[nCnt].fHeight = 0;
		g_Explosion[nCnt].fWidth = 0;
		g_Explosion[nCnt].nPatternAnim = 0;
		g_Explosion[nCnt].speed = 10.0f;
		g_Explosion[nCnt].radius = 30.0f;
		g_Explosion[nCnt].nType = EXPLOSION_MOVE;
		g_Explosion[nCnt].bUse = false;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Explosion[nCnt].pos.x - EXPLOSION_WIDTH, g_Explosion[nCnt].pos.y + EXPLOSION_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Explosion[nCnt].pos.x + EXPLOSION_WIDTH, g_Explosion[nCnt].pos.y + EXPLOSION_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Explosion[nCnt].pos.x - EXPLOSION_WIDTH, g_Explosion[nCnt].pos.y - EXPLOSION_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Explosion[nCnt].pos.x + EXPLOSION_WIDTH, g_Explosion[nCnt].pos.y - EXPLOSION_HEIGHT, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.3f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 0.3f);

		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}
//============================
//�����̏I������
//============================
void UninitExplosion(void)
{
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̉��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}
//============================
//�����̍X�V����
//============================
void UpdateExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^
	Player* pPlayer = GetPlayer();

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	VERTEX_3D* pVtx;//���_���̃|�C���^

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_Explosion[nCntExplosion].nCounterAnim++;

		if (g_Explosion[nCntExplosion].bUse == true)
		{
			g_Explosion[nCntExplosion].nLife--;

			if (g_Explosion[nCntExplosion].nLife <= 0)
			{
				g_Explosion[nCntExplosion].bUse = false;//�������s����
			}

			//�ʒu�̍X�V
			g_Explosion[nCntExplosion].move.x += (0.0f - g_Explosion[nCntExplosion].move.x) * 0.01f;
			g_Explosion[nCntExplosion].move.z += (0.0f - g_Explosion[nCntExplosion].move.z) * 0.01f;
			   
			g_Explosion[nCntExplosion].pos += g_Explosion[nCntExplosion].move;
			   

			if (g_Explosion[nCntExplosion].nCounterAnim >= 4)
			{
				g_Explosion[nCntExplosion].nCounterAnim = NULL;//�J�E���^�[�������l�ɖ߂�

				g_Explosion[nCntExplosion].nPatternAnim++;//�p�^�[���i���o�[���X�V

				pVtx[0].tex = D3DXVECTOR2(g_Explosion[nCntExplosion].nPatternAnim * 0.25f, (g_Explosion[nCntExplosion].nPatternAnim / 4) * 0.333f);
				pVtx[1].tex = D3DXVECTOR2(0.25f + g_Explosion[nCntExplosion].nPatternAnim * 0.25f, (g_Explosion[nCntExplosion].nPatternAnim / 4) * 0.333f);
				pVtx[2].tex = D3DXVECTOR2(g_Explosion[nCntExplosion].nPatternAnim * 0.25f, (g_Explosion[nCntExplosion].nPatternAnim / 4) * 0.333f + 0.333f);
				pVtx[3].tex = D3DXVECTOR2(0.25f + g_Explosion[nCntExplosion].nPatternAnim * 0.25f, (g_Explosion[nCntExplosion].nPatternAnim / 4) * 0.333f + 0.333f);

			}

			if (g_Explosion[nCntExplosion].nPatternAnim > 11)
			{
				g_Explosion[nCntExplosion].nPatternAnim = 0;//�p�^�[���i���o�[�������l�ɖ߂�
				g_Explosion[nCntExplosion].bUse = false;
			}

			if (g_Explosion[nCntExplosion].nType == EXPLOSION_LANDING)
			{
				for (int nCnt = 0; nCnt < LANDINGEXPLOSION; nCnt++)
				{
					float fAngle = (D3DX_PI * 2.0f) / LANDINGEXPLOSION * nCnt;

					g_Explosion[nCnt].move.x = sinf(fAngle) * 1.0f;
					g_Explosion[nCnt].move.y = 0.3f;
					g_Explosion[nCnt].move.z = cosf(fAngle) * 1.0f;
				}
			}
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}
//============================
//�����̕`�揈��
//============================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, NULL);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//�[�b�g�e�X�g
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxView;

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_Explosion[nCnt].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Explosion[nCnt].mtxWorld);

			pDevice->GetTransform(D3DTS_VIEW, &mtxView);//�r���[�}�g���b�N�X�̎擾

			//�J�����̋t�s��̐ݒ�
			g_Explosion[nCnt].mtxWorld._11 = mtxView._11;
			g_Explosion[nCnt].mtxWorld._12 = mtxView._21;
			g_Explosion[nCnt].mtxWorld._13 = mtxView._31;
			g_Explosion[nCnt].mtxWorld._21 = mtxView._12;
			g_Explosion[nCnt].mtxWorld._22 = mtxView._22;
			g_Explosion[nCnt].mtxWorld._23 = mtxView._32;
			g_Explosion[nCnt].mtxWorld._31 = mtxView._13;
			g_Explosion[nCnt].mtxWorld._32 = mtxView._23;
			g_Explosion[nCnt].mtxWorld._33 = mtxView._33;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Explosion[nCnt].pos.x, g_Explosion[nCnt].pos.y, g_Explosion[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Explosion[nCnt].mtxWorld, &g_Explosion[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Explosion[nCnt].mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureExplosion);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
	//�A���t�@�e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//�[�b�g�e�X�g
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//============================
//�����̐ݒ菈��
//============================
void SetExplosion(D3DXVECTOR3 pos,D3DXCOLOR col, int nLife,float fWidth,float fHeight, int nType)
{
	VERTEX_3D* pVtx;//���_���̃|�C���^

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_Explosion[nCnt].bUse == false)
		{
			g_Explosion[nCnt].pos = pos;
			g_Explosion[nCnt].fWidth = fWidth;
			g_Explosion[nCnt].fHeight = fHeight;
			g_Explosion[nCnt].col = col;
			g_Explosion[nCnt].nLife = nLife;
			g_Explosion[nCnt].nType = nType;
			g_Explosion[nCnt].bUse = true;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-fWidth, fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fWidth,  -fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fWidth,  -fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}
