//============================
//
// �A�C�e��[item.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "input.h"
#include "mouse.h"
#include "camera.h"
#include "item.h"

//****************************
//�}�N����`
//****************************
#define MOUSE_SIZE (50.0f)

//****************************
//�v���g�^�C�v�錾
//****************************
void SetMtxItem(int nCnt);

//****************************
//�O���[�o���ϐ��錾
//****************************
Item g_Item[MAX_ITEM];
Item g_TexItem[ITEMTYPE_MAX];

//=============================
//�u���b�N�̏���������
//=============================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //���W
		g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
		g_Item[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�p�x
		g_Item[nCntItem].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);//�g�嗦
		g_Item[nCntItem].nType = ITEMTYPE_ONE;				  //���
		g_Item[nCntItem].bUse = false;						  //���g�p����
		g_Item[nCntItem].nLife = 20;						  //�̗�
		g_Item[nCntItem].state = ITEMSTATE_NORMAL;			  //���
		g_Item[nCntItem].fRadius = 100.0f;					  //���a
		g_Item[nCntItem].nLife = 180;						  //�̗�
	}

	//g_Item.vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_Item.vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntNum = 0; nCntNum < ITEMTYPE_MAX; nCntNum++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(ITEMTYPE_INFO[nCntNum],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem,
			NULL,
			&g_TexItem[nCntNum].ItemTex[nCntNum].g_dwNumMatItem,
			&g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem);
	}

	for (int nCntNum = 0; nCntNum < ITEMTYPE_MAX; nCntNum++)
	{
		//g_TexItem[nCntNum].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexItem[nCntNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexItem[nCntNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_TexItem[nCntNum].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);//�g�嗦
		g_TexItem[nCntNum].bUse = true;							//�g�p����
		g_TexItem[nCntNum].nLife = 120;							//�̗�

		D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

		//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_TexItem[nCntNum].ItemTex[nCntNum].g_dwNumMatItem; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
					//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntMat]);
			}
		}
	}

	//���_���W�̎擾
	int nNumVtx;//���_��
	DWORD sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntNum = 0; nCntNum < ITEMTYPE_MAX; nCntNum++)
	{
		//���_���̎擾
		nNumVtx = g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y�擾
		sizeFVF = D3DXGetFVFVertexSize(g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->GetFVF());

		//���_�o�b�t�@�̃��b�N
		g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
			if (vtx.x < g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.x)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.x = vtx.x;
			}
			else if (vtx.y < g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.y)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.y = vtx.y;
			}
			else if (vtx.z < g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.z)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.z = vtx.z;
			}
			else if (vtx.x > g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.x)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.x = vtx.x;
			}
			else if (vtx.y > g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.y)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.y = vtx.y;
			}
			else if (vtx.z > g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.z)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.z = vtx.z;
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;

			//�T�C�Y����
			g_TexItem[nCntNum].Size.x = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.x - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.x;
			g_TexItem[nCntNum].Size.y = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.y - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.y;
			g_TexItem[nCntNum].Size.z = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.z - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.z;
		}


		//���_�o�b�t�@�̃A�����b�N
		g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->UnlockVertexBuffer();
	}
}
//=============================
//�u���b�N�̏I������
//=============================
void UninitItem(void)
{
	for (int nCntNum = 0; nCntNum < ITEMTYPE_MAX; nCntNum++)
	{
		//�e�N�X�`���̔j��
		for (int nCntTex = 0; nCntTex < 32; nCntTex++)
		{
			if (g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntTex] != NULL)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntTex]->Release();
				g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntTex] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem != NULL)
		{
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->Release();
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem = NULL;
		}

		//�}�e���A��
		if (g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem != NULL)
		{
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem->Release();
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem = NULL;
		}
	}
}
//=============================
//�u���b�N�̍X�V����
//=============================
void UpdateItem(void)
{
	Player* pPlayer = GetPlayer();//�v���C���[�擾

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_Item[nCntItem].bUse)
		{
			//�g�p������Ȃ�������
			continue;
		}

		if (g_Item[nCntItem].state == ITEMSTATE_HOLD && KeyboardTrigger(DIK_RETURN))
		{
			//�v���C���[�̌����Ă�������ɔ�΂�
			g_Item[nCntItem].move.x = sinf(pPlayer->rot.y + D3DX_PI) * 10.0f;
			g_Item[nCntItem].move.y = 0.0f;
			g_Item[nCntItem].move.z = cosf(pPlayer->rot.y + D3DX_PI) * 10.0f;

			//���˒n�_��e�̈ʒu�ɂ���
			g_Item[nCntItem].pos.x = pPlayer->Motion.aModel[PARENT].mtxWorld._41;
			g_Item[nCntItem].pos.y = pPlayer->Motion.aModel[PARENT].mtxWorld._42;
			g_Item[nCntItem].pos.z = pPlayer->Motion.aModel[PARENT].mtxWorld._43;

			//�u���b�N�̓������Ă����Ԃɂ���
			g_Item[nCntItem].state = ITEMSTATE_THROW;

			//�v���C���[�����������Ă��Ȃ���Ԃɂ���
			pPlayer->HandState = PLAYERHOLD_NO;
		}
		if (g_Item[nCntItem].state == ITEMSTATE_HOLD && KeyboardTrigger(DIK_O))
		{
			g_Item[nCntItem].state = ITEMSTATE_NORMAL;
			pPlayer->HandState = PLAYERHOLD_NO;

			//���˒n�_��e�̈ʒu�ɂ���
			g_Item[nCntItem].pos.x = pPlayer->pos.x;
			g_Item[nCntItem].pos.y = 0.0f;
			g_Item[nCntItem].pos.z = pPlayer->pos.z;

		}

		if (g_Item[nCntItem].state == ITEMSTATE_THROW)
		{
			//�̗͂����炷
			g_Item[nCntItem].nLife--;

			if (g_Item[nCntItem].nLife <= 0)
			{//�̗͂�0�ȉ��Ȃ�
				//���g�p����
				g_Item[nCntItem].bUse = false;
			}
		}

		//�ʒu�̍X�V
		g_Item[nCntItem].pos += g_Item[nCntItem].move;
	}

}
//=============================
//�u���b�N�̕`�揈��
//=============================
void DrawItem(void)
{
	MODE mode = GetMode();//���݂̃��[�h���擾

	Player* pPlayer = GetPlayer();//�v���C���[�̎擾

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntNum = 0; nCntNum < ITEMTYPE_MAX; nCntNum++)
	{
		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
		{
			if (!g_Item[nCntItem].bUse)
			{//���g�p��ԂȂ�
				//���̏�����ʂ����J�E���g��i�߂�
				continue;
			}

			if (g_Item[nCntItem].state == ITEMSTATE_HOLD)
			{
				//�擾����
				SetMtxItem(nCntItem);
			}
			else
			{
				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_Item[nCntItem].mtxWorldItem);

				//�傫���𔽉f
				D3DXMatrixScaling(&mtxScal, g_Item[nCntItem].Scal.y, g_Item[nCntItem].Scal.x, g_Item[nCntItem].Scal.z);
				D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxScal);

				//�����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Item[nCntItem].rot.y, g_Item[nCntItem].rot.x, g_Item[nCntItem].rot.z);
				D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxRot);

				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z);
				D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxTrans);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_Item[nCntItem].mtxWorldItem);
			}

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			for (int nCntMat = 0; nCntMat < (int)g_Item[nCntItem].ItemTex[nCntNum].g_dwNumMatItem; nCntMat++)
			{
				D3DXMATERIAL color;

				//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_Item[nCntItem].ItemTex[nCntNum].g_pBuffMatItem->GetBufferPointer();

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Item[nCntItem].ItemTex[nCntNum].g_apTextureItem[nCntMat]);

				//�u���b�N(�p�[�c)�̕`��
				g_Item[nCntItem].ItemTex[nCntNum].g_pMeshItem->DrawSubset(nCntMat);
			}
		}
	}
}
//=======================
//�u���b�N�̐ݒ菈��
//=======================
void SetItem(D3DXVECTOR3 pos, int nType,D3DXVECTOR3 Scal)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_Item[nCntItem].bUse)
		{//���g�p��ԂȂ�
			g_Item[nCntItem] = g_TexItem[nType]; // �K�v�ȏ�����

			g_Item[nCntItem].pos = pos;	   //���W
			g_Item[nCntItem].nType = nType;//���
			g_Item[nCntItem].Scal = Scal;  //�g�嗦
			g_Item[nCntItem].bUse = true;  //�g�p����

			break;
		}
	}
}
//=======================
//�u���b�N�̔��菈��
//=======================
bool CollisionItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize)
{
	bool bLanding = false;// �����Ԃ��ϐ�

	Player* pPlayer = GetPlayer();//�v���C���[�̎擾

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_Item[nCntItem].bUse)
		{//���g�p��ԂȂ�
			//���̏�����ʂ����J�E���g��i�߂�
			continue;
		}

		if (pPosOld->y <= g_Item[nCntItem].pos.y + g_Item[nCntItem].Size.y * g_Item[nCntItem].Scal.y
			&& pPosOld->y + pSize->y >= g_Item[nCntItem].pos.y)
		{
			//���E�̂߂荞�ݔ���
			if (pPos->z - pSize->z * 0.5f < g_Item[nCntItem].pos.z + g_Item[nCntItem].Size.z * 0.5f * g_Item[nCntItem].Scal.z
				&& pPos->z + pSize->z * 0.5f > g_Item[nCntItem].pos.z - g_Item[nCntItem].Size.z * 0.5f * g_Item[nCntItem].Scal.z)
			{
				//x��������E�ɂ߂荞��	
				if (pPosOld->x + pSize->x * 0.5f < g_Item[nCntItem].pos.x - g_Item[nCntItem].Size.x * 0.5f * g_Item[nCntItem].Scal.y
					&& pPos->x + pSize->x * 0.5f > g_Item[nCntItem].pos.x - g_Item[nCntItem].Size.x * 0.5f * g_Item[nCntItem].Scal.y)
				{
					pPos->x = g_Item[nCntItem].pos.x - g_Item[nCntItem].Size.x * 0.5f * g_Item[nCntItem].Scal.x - pSize->x * 0.5f - 0.1f;

					//���������
					if (KeyboardTrigger(DIK_L) && pPlayer->HandState != PLAYERHOLD_HOLD)
					{
						g_Item[nCntItem].state = ITEMSTATE_HOLD;
						g_Item[nCntItem].pos = pPlayer->Motion.aModel[PARENT].pos;
						SetMtxItem(nCntItem);
						pPlayer->HandState = PLAYERHOLD_HOLD;
					}
				}
				//x���E���獶�ɂ߂荞��	
				else if (pPosOld->x - pSize->x * 0.5f > g_Item[nCntItem].pos.x + g_Item[nCntItem].Size.x * 0.5f * g_Item[nCntItem].Scal.x
					&& pPos->x - pSize->x * 0.5f < g_Item[nCntItem].pos.x + g_Item[nCntItem].Size.x * 0.5f * g_Item[nCntItem].Scal.x)
				{
					pPos->x = g_Item[nCntItem].pos.x + g_Item[nCntItem].Size.x * 0.5f * g_Item[nCntItem].Scal.x + pSize->x * 0.5f + 0.1f;
					//���������
					if (KeyboardTrigger(DIK_L) && pPlayer->HandState != PLAYERHOLD_HOLD)
					{
						g_Item[nCntItem].state = ITEMSTATE_HOLD;
						g_Item[nCntItem].pos = pPlayer->Motion.aModel[PARENT].pos;
						SetMtxItem(nCntItem);
						pPlayer->HandState = PLAYERHOLD_HOLD;
					}
				}
			}

			//�O�ƌ��̔���
			if (pPos->x - pSize->x * 0.5f < g_Item[nCntItem].pos.x + g_Item[nCntItem].Size.x * 0.5f * g_Item[nCntItem].Scal.x
				&& pPos->x + pSize->x * 0.5f > g_Item[nCntItem].pos.x - g_Item[nCntItem].Size.x * 0.5f * g_Item[nCntItem].Scal.x)
			{
				//z���O������߂荞��
				if (pPosOld->z + pSize->z * 0.5f < g_Item[nCntItem].pos.z - g_Item[nCntItem].Size.z * 0.5f * g_Item[nCntItem].Scal.z
					&& pPos->z + pSize->z * 0.5f > g_Item[nCntItem].pos.z - g_Item[nCntItem].Size.z * 0.5f * g_Item[nCntItem].Scal.z)
				{
					pPos->z = g_Item[nCntItem].pos.z - g_Item[nCntItem].Size.z * 0.5f * g_Item[nCntItem].Scal.z - pSize->z * 0.5f - 0.1f;

					//���������
					if (KeyboardTrigger(DIK_L) && pPlayer->HandState != PLAYERHOLD_HOLD)
					{
						g_Item[nCntItem].state = ITEMSTATE_HOLD;
						g_Item[nCntItem].pos = pPlayer->Motion.aModel[PARENT].pos;
						SetMtxItem(nCntItem);
						pPlayer->HandState = PLAYERHOLD_HOLD;
					}
				}
				//z���������߂荞��
				else if (pPosOld->z - pSize->z * 0.5f > g_Item[nCntItem].pos.z + g_Item[nCntItem].Size.z * 0.5f * g_Item[nCntItem].Scal.z
					&& pPos->z - pSize->z * 0.5f < g_Item[nCntItem].pos.z + g_Item[nCntItem].Size.z * 0.5f * g_Item[nCntItem].Scal.z)
				{
					pPos->z = g_Item[nCntItem].pos.z + g_Item[nCntItem].Size.z * 0.5f * g_Item[nCntItem].Scal.z + pSize->z * 0.5f + 0.5f;

					//���������
					if (KeyboardTrigger(DIK_L) && pPlayer->HandState != PLAYERHOLD_HOLD)
					{
						g_Item[nCntItem].state = ITEMSTATE_HOLD;
						g_Item[nCntItem].pos = pPlayer->Motion.aModel[PARENT].pos;
						SetMtxItem(nCntItem);
						pPlayer->HandState = PLAYERHOLD_HOLD;
					}
				}
			}
		}

		if (pPos->x - pSize->x * 0.5f <= g_Item[nCntItem].pos.x + g_Item[nCntItem].Size.x * 0.5f * g_Item[nCntItem].Scal.x
			&& pPos->x + pSize->x * 0.5f >= g_Item[nCntItem].pos.x - g_Item[nCntItem].Size.x * 0.5f * g_Item[nCntItem].Scal.x)
		{
			if (pPos->z - pSize->z * 0.5f <= g_Item[nCntItem].pos.z + g_Item[nCntItem].Size.z * 0.5f * g_Item[nCntItem].Scal.z
				&& pPos->z + pSize->z * 0.5f >= g_Item[nCntItem].pos.z - g_Item[nCntItem].Size.z * 0.5f * g_Item[nCntItem].Scal.z)
			{
				//�ォ�牺
				if (pPosOld->y >= g_Item[nCntItem].pos.y + g_Item[nCntItem].Size.y * g_Item[nCntItem].Scal.y
					&& pPos->y < g_Item[nCntItem].pos.y + g_Item[nCntItem].Size.y * g_Item[nCntItem].Scal.y)
				{
					bLanding = true;
					pPos->y = pPosOld->y;
					pMove->y = 0.0f;
				}
				//�������
				else if (pPosOld->y + pSize->y * 0.5f <= g_Item[nCntItem].pos.y - g_Item[nCntItem].Size.y * 0.5f * g_Item[nCntItem].Scal.y
					&& pPos->y + pSize->y * 0.5f > g_Item[nCntItem].pos.y - g_Item[nCntItem].Size.y * 0.5f * g_Item[nCntItem].Scal.y)
				{
					pPos->y = pPosOld->y;
					pMove->y = 0.0f;
				}

			}
		}
	}

	return bLanding;//�����Ԃ�
}
//=======================
//�u���b�N�ƓG�̔���
//=======================
bool CollisionEnemy(D3DXVECTOR3* pPos, float ItemRadius, float EnemyRadius)
{
	bool bHit = false; // �����Ԃ��ϐ�

	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		float fDistanceX = g_Item[nCnt].pos.x - pPos->x; // ����X���Z�o
		float fDistanceY = g_Item[nCnt].pos.y - pPos->y; // ����Y���Z�o
		float fDistanceZ = g_Item[nCnt].pos.z - pPos->z; // ����Z���Z�o

		float fDistance = (fDistanceX * fDistanceX) + (fDistanceY * fDistanceY) + (fDistanceZ * fDistanceZ);

		float Radius = ItemRadius + EnemyRadius;

		Radius = Radius * Radius;

		if (fDistance <= Radius && g_Item[nCnt].state == ITEMSTATE_THROW)
		{
			bHit = true;
			g_Item[nCnt].bUse = false;
		}
	}

	return bHit;//�����Ԃ�
}
////=======================
////�u���b�N�̎擾����
////=======================
//ITEM* GetItem()
//{
//	return &g_Item[0];
//}
//int NumItem(void)
//{
//	return g_NumItem;
//}
//=======================
//�u���b�N�̐e����
//=======================
void SetMtxItem(int nCnt)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;

	Player* pPlayer = GetPlayer();//�v���C���[�̎擾

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Item[nCnt].mtxWorldItem);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScal, g_Item[nCnt].Scal.y, g_Item[nCnt].Scal.x, g_Item[nCnt].Scal.z);
	D3DXMatrixMultiply(&g_Item[nCnt].mtxWorldItem, &g_Item[nCnt].mtxWorldItem, &mtxScal);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Item[nCnt].rot.y, g_Item[nCnt].rot.x, g_Item[nCnt].rot.z);
	D3DXMatrixMultiply(&g_Item[nCnt].mtxWorldItem, &g_Item[nCnt].mtxWorldItem, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Item[nCnt].pos.x, g_Item[nCnt].pos.y, g_Item[nCnt].pos.z);
	D3DXMatrixMultiply(&g_Item[nCnt].mtxWorldItem, &g_Item[nCnt].mtxWorldItem, &mtxTrans);

	g_Item[nCnt].mtxParent = pPlayer->Motion.aModel[PARENT].mtxWorld;

	//�Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
	D3DXMatrixMultiply(&g_Item[nCnt].mtxWorldItem,
		&g_Item[nCnt].mtxWorldItem,
		&g_Item[nCnt].mtxParent);//���������e

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Item[nCnt].mtxWorldItem);
}
