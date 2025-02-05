//============================
//
// �|���S��[shadow.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "player.h"
#include "camera.h"
#include "input.h"
#include"polygon.h"

//****************************
//�}�N����`
//****************************
#define MAX_POLYGON (10)

//****************************
//�O���[�o���ϐ��錾
//****************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL; //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureOrigin;//�e�N�X�`���ւ̃|�C���^�ۑ��p
Polygon_Info g_Polygon[MAX_POLYGON];//�|���S���̍\����

//=============================
//�|���S���̏���������
//=============================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		POLYGON_TYPE[0],
		&g_pTextureOrigin);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	VERTEX_3D* pVtx = NULL;

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		g_Polygon[nCntPolygon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Polygon[nCntPolygon].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Polygon[nCntPolygon].fWidth = 0.0f;
		g_Polygon[nCntPolygon].fHeight = 0.0f;
		g_Polygon[nCntPolygon].fLength = 0.0f;
		g_Polygon[nCntPolygon].bUse = false;
		g_Polygon[nCntPolygon].fMove = 10.0f;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�e���_�̖@���̐ݒ�(�x�N�g���̑傫���͂P�ɂ���K�v������)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	g_pVtxBuffPolygon->Unlock();
}
//=============================
//�|���S���̏I������
//=============================
void UninitPolygon(void)
{

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (g_Polygon[nCntPolygon].g_pTexturePolygon != NULL)
		{
			g_Polygon[nCntPolygon].g_pTexturePolygon = NULL;
		}
	}

	if (g_pTextureOrigin != NULL)
	{
		g_pTextureOrigin->Release();
		g_pTextureOrigin = NULL;
	}

	//���_�o�b�t�@�̉��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}
//=============================
//�|���S���̍X�V����
//=============================
void UpdatePolygon(void)
{
	Camera* pCamera = GetCamera();


}
//=============================
//�|���S���̕`�揈��
//=============================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (!g_Polygon[nCntPolygon].bUse)
		{
			continue;
		}

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Polygon[nCntPolygon].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Polygon[nCntPolygon].rot.y, g_Polygon[nCntPolygon].rot.x, g_Polygon[nCntPolygon].rot.z);
		D3DXMatrixMultiply(&g_Polygon[nCntPolygon].mtxWorld, &g_Polygon[nCntPolygon].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Polygon[nCntPolygon].pos.x, g_Polygon[nCntPolygon].pos.y, g_Polygon[nCntPolygon].pos.z);
		D3DXMatrixMultiply(&g_Polygon[nCntPolygon].mtxWorld, &g_Polygon[nCntPolygon].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Polygon[nCntPolygon].mtxWorld);

		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

		//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_Polygon[nCntPolygon].g_pTexturePolygon);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPolygon * 4, 2);
	}
}
//=============================
//�|���S���̐ݒ菈��
//=============================
void SetPolygon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nType)
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_POLYGON; nCnt++)
	{
		if (!g_Polygon[nCnt].bUse)
		{
			g_Polygon[nCnt].g_pTexturePolygon = g_pTextureOrigin; // �e�N�X�`��������
			g_Polygon[nCnt].pos = pos;
			g_Polygon[nCnt].rot = rot;
			g_Polygon[nCnt].fWidth = fWidth;
			g_Polygon[nCnt].fHeight = fHeight;
			g_Polygon[nCnt].nType = nType;
			g_Polygon[nCnt].bUse = true;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);
			
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffPolygon->Unlock();
}
