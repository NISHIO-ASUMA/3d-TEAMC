//=====================================================================================================================
//
// 2D�~�j�}�b�v���� [ minimap.cpp]
// Author: NISHIO & YOSHIDA
//
//=====================================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//**************************************************************************************************************
#include "minimap.h"
#include "player.h"
#include "meshfield.h"

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void InitMapField(void);
void UninitMapField(void);
void UpdateMapField(void);
void DrawMapField(void);

//**************************************************************************************************************
// �O���[�o���ϐ��錾
//**************************************************************************************************************
MINIMAP g_MiniMap[MAX_TREXMINIMAP]; // �e�N�X�`���p�\���̏��
LPDIRECT3DTEXTURE9 g_pTextureMiniMap[MINIMAPTEX_MAX] = {}; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMiniMap = NULL;		   // ���_�o�b�t�@�ւ̃|�C���^

LPDIRECT3DTEXTURE9 g_pTextureMiniMapField = NULL;           // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMiniMapField = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 MapFiledPos;

//==============================================================================================================
// �~�j�}�b�v����������
//==============================================================================================================
void InitMiniMap()
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	for (int nCnt = 0; nCnt < MINIMAPTEX_MAX; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			CAMERA_TEX[nCnt],
			&g_pTextureMiniMap[nCnt]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TREXMINIMAP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMiniMap,
		NULL);

	// �\���̕ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_TREXMINIMAP; nCnt++)
	{
		g_MiniMap[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ���W
		g_MiniMap[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �p�x
		g_MiniMap[nCnt].nType = 0; // ���
		g_MiniMap[nCnt].fHeight = 0.0f; // ����
		g_MiniMap[nCnt].fWidth = 0.0f; // ����
		g_MiniMap[nCnt].bUse = false; // ���g�p����
		g_MiniMap[nCnt].fAngle = 0.0f; // �p�x�v�Z�p
	}

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffMiniMap->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�֌W
	for (int nCntCamera = 0; nCntCamera < MAX_TREXMINIMAP; nCntCamera++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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

		// ���_����i�߂�
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffMiniMap->Unlock();

	InitMapField();

}
//==============================================================================================================
// �~�j�}�b�v�I������
//==============================================================================================================
void UninitMinMap()
{
	UninitMapField();

	// �e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MINIMAPTEX_MAX; nCnt++)
	{
		if (g_pTextureMiniMap[nCnt] != NULL)
		{			  
			g_pTextureMiniMap[nCnt]->Release();
			g_pTextureMiniMap[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffMiniMap != NULL)
	{
		g_pVtxBuffMiniMap->Release();
		g_pVtxBuffMiniMap = NULL;
	}
}
//==============================================================================================================
// �~�j�}�b�v�X�V����
//==============================================================================================================
void UpdateMiniMap()
{

}
//==============================================================================================================
// �~�j�}�b�v�`�揈��
//==============================================================================================================
void DarwMinimap()
{
	DrawMapField();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMiniMap, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �G���G�̃}�[�J�[�T�m
	for (int nCnt = 0; nCnt < MAX_TREXMINIMAP; nCnt++)
	{
		// ��ނ�ۑ�
		int nType = g_MiniMap[nCnt].nType;

		if (g_MiniMap[nCnt].bUse && g_MiniMap[nCnt].nType == 1)
		{// �g�p����̂��̂���

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMiniMap[nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	// �{�X�̃}�[�J�[�T�m
	for (int nCnt = 0; nCnt < MAX_TREXMINIMAP; nCnt++)
	{
		// ��ނ�ۑ�
		int nType = g_MiniMap[nCnt].nType;

		if (g_MiniMap[nCnt].bUse && g_MiniMap[nCnt].nType == 2)
		{// �g�p����̂��̂���

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMiniMap[nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	// �v���C���[�̃}�[�J�[�T�m
	for (int nCnt = 0; nCnt < MAX_TREXMINIMAP; nCnt++)
	{
		// ��ނ�ۑ�
		int nType = g_MiniMap[nCnt].nType;

		if (g_MiniMap[nCnt].bUse && g_MiniMap[nCnt].nType == 0)
		{// �g�p����̂��̂���

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMiniMap[nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}
//==============================================================================================================
// �~�j�}�b�v�̈ʒu�ݒ菈��
//==============================================================================================================
void SetMiniMapPotision(int nIdx, D3DXVECTOR3* pPos)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffMiniMap->Lock(0, 0, (void**)&pVtx, 0);

	if (g_MiniMap[nIdx].bUse == true)
	{
		// ���_���W�̃|�C���^��i�߂�
		pVtx += 4 * nIdx;

		// �v���C���[�̍��W���~�j�}�b�v�̍��W�ɕϊ�
		g_MiniMap[nIdx].pos.x = (pPos->x / MAX_FIELDWIDTH) * MINIMAP_WIDTH;
		g_MiniMap[nIdx].pos.y = (-pPos->z / MAX_FIELDHEIGHT) * MINIMAP_HEIGHT;

		// �v���C���[�̌������擾
		float fAngle = pPlayer->rot.y;

		// �p�x�v�Z
		float cosA = cosf(fAngle);
		float sinA = sinf(fAngle);

		if (g_MiniMap[nIdx].nType == 0)
		{// ��ނ��v���C���[�̎�
			// ��]���l���������W�X�V
			pVtx[0].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + (-5.0f * cosA - (-5.0f) * sinA),
									  MapFiledPos.y + g_MiniMap[nIdx].pos.y + (-5.0f * sinA + (-5.0f) * cosA),
									  0.0f);
			pVtx[1].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + (5.0f * cosA - (-5.0f) * sinA),
									  MapFiledPos.y + g_MiniMap[nIdx].pos.y + (5.0f * sinA +(-5.0f) * cosA),
									  0.0f);
			pVtx[2].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + (-5.0f * cosA - (5.0f) * sinA),
									  MapFiledPos.y + g_MiniMap[nIdx].pos.y + (-5.0f * sinA + (5.0f) * cosA),
									  0.0f);
			pVtx[3].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + (5.0f * cosA - (5.0f) * sinA),
									  MapFiledPos.y + g_MiniMap[nIdx].pos.y + (5.0f * sinA + (5.0f) * cosA),
									  0.0f);
		}
		else if (g_MiniMap[nIdx].nType == 1)
		{// ��ނ��G�̎�
			// ���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x - 5.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y - 5.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + 5.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y - 5.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x - 5.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y + 5.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + 5.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y + 5.0f, 0.0f);

		}
		else if (g_MiniMap[nIdx].nType == 2)
		{// ��ނ��{�X�̎�
			// ���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x - 8.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y - 8.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + 8.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y - 8.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x - 8.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y + 8.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + 8.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y + 8.0f, 0.0f);

		}
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffMiniMap->Unlock();
}
//==============================================================================================================
// �~�j�}�b�v�ݒ菈��
//==============================================================================================================
int SetMiniMap(D3DXVECTOR3 pos, int nType)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	int nCnt;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffMiniMap->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < MAX_TREXMINIMAP; nCnt++)
	{
		if (g_MiniMap[nCnt].bUse == false)
		{
			g_MiniMap[nCnt].pos = pos;
			g_MiniMap[nCnt].nType = nType;
			g_MiniMap[nCnt].bUse = true;

			// ���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(MapFiledPos.x + pos.x - 5.0f, MapFiledPos.y + pos.y - 5.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(MapFiledPos.x + pos.x + 5.0f, MapFiledPos.y + pos.y - 5.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(MapFiledPos.x + pos.x - 5.0f, MapFiledPos.y + pos.y + 5.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(MapFiledPos.x + pos.x + 5.0f, MapFiledPos.y + pos.y + 5.0f, 0.0f);

			break;
		}

		// ���_���W�̃|�C���^��i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffMiniMap->Unlock();

	return nCnt;
}
//==============================================================================================================
// �w��ԍ��̃|���S��������
//==============================================================================================================
void EnableMap(int nIdx)
{
	g_MiniMap[nIdx].bUse = false;
}
//==============================================================================================================
// �~�j�}�b�v�̃t�B�[���h����������
//==============================================================================================================
void InitMapField(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\minimap.png",
		&g_pTextureMiniMapField);
	
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMiniMapField,
		NULL);

	MapFiledPos = D3DXVECTOR3(1100.0f, 180.0f, 0.0f);

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffMiniMapField->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�֌W
	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(1000.0f, 80.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1200.0f, 80.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(1000.0f, 280.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1200.0f, 280.0f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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

	// �A�����b�N
	g_pVtxBuffMiniMapField->Unlock();

}
//==============================================================================================================
// �~�j�}�b�v�̃t�B�[���h�I������
//==============================================================================================================
void UninitMapField(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureMiniMapField != NULL)
	{
		g_pTextureMiniMapField->Release();
		g_pTextureMiniMapField = NULL;
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffMiniMapField != NULL)
	{
		g_pVtxBuffMiniMapField->Release();
		g_pVtxBuffMiniMapField = NULL;
	}

}
//==============================================================================================================
// �~�j�}�b�v�̃t�B�[���h�X�V����
//==============================================================================================================
void UpdateMapField(void)
{
	// �Ȃ�
}
//==============================================================================================================
// �~�j�}�b�v�̃t�B�[���h�`�揈��
//==============================================================================================================
void DrawMapField(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMiniMapField, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMiniMapField);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
#if 0
// ��]���v�Z
g_MiniMap[nIdx].rot.y = sqrtf((pPlayer->rot.y * pPlayer->rot.y)) * 0.5f;
g_MiniMap[nIdx].rot.x = sqrtf((pPlayer->rot.z * pPlayer->rot.z)) * 0.5f;

g_MiniMap[nIdx].fAngle = atan2f(pPlayer->rot.y, pPlayer->rot.z);
#endif
