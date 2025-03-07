//=====================================================================================================================
//
// ���b�V���C���p�N�g[meshimpact.cpp]
// Author: YOSHIDA YUTO
//
//=====================================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "input.h"
#include "player.h"
#include "meshimpact.h"
#include "Effect.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MAX_IMPACT (50) // �Ռ��g�̍ő吔
#define A_OFFSET (0.7f) // ���l�̊

//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
MeshImpact g_MeshImpact[MAX_IMPACT];

//=================================================================================================================
//���b�V���t�B�[���h�̏���������
//=================================================================================================================
void InitMeshImpact(void)
{
	// �Ռ��g�̍ő吔����
	for (int nCnt = 0; nCnt < MAX_IMPACT; nCnt++)
	{
		// ����������
		g_MeshImpact[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �ʒu
		g_MeshImpact[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ����
		g_MeshImpact[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړ���
		g_MeshImpact[nCnt].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f); // �F
		g_MeshImpact[nCnt].nImpactPosX = 0;                      // �C���p�N�g��X�̕�����
		g_MeshImpact[nCnt].Vertex = 0;							 // ���_��
		g_MeshImpact[nCnt].Polygon = 0;                          // �|���S����
		g_MeshImpact[nCnt].Index = 0;							 // �C���f�b�N�X��
		g_MeshImpact[nCnt].decAlv = 0.0f;                        // a�l�̌�����
		g_MeshImpact[nCnt].speed = 0.0f;                         // ����
		g_MeshImpact[nCnt].nLife = 0;                            // ����
		g_MeshImpact[nCnt].nType = IMPACTTYPE_NORMAL;            // ���
		g_MeshImpact[nCnt].OutRadius = 0.0f;                     // �O���̔��a
		g_MeshImpact[nCnt].InRadius = 0.0f;                      // �����̔��a
		g_MeshImpact[nCnt].bUse = false;                         // �g�p���
	}

	//g_MeshImpact.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_MeshImpact.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

//=================================================================================================================
//���b�V���t�B�[���h�̏I������
//=================================================================================================================ccc
void UninitMeshImpact(void)
{
	for (int nCnt = 0; nCnt < MAX_IMPACT; nCnt++)
	{
		//�e�N�X�`���̉��
		if (g_MeshImpact[nCnt].g_pTextureMeshImpact != NULL)
		{
			g_MeshImpact[nCnt].g_pTextureMeshImpact->Release();
			g_MeshImpact[nCnt].g_pTextureMeshImpact = NULL;
		}

		//���_�o�b�t�@�̉��
		if (g_MeshImpact[nCnt].g_pVtxBuffMeshImpact != NULL)
		{
			g_MeshImpact[nCnt].g_pVtxBuffMeshImpact->Release();
			g_MeshImpact[nCnt].g_pVtxBuffMeshImpact = NULL;
		}

		//�C���f�b�N�X�o�b�t�@�̉��
		if (g_MeshImpact[nCnt].g_pIdxBuffMeshImpact != NULL)
		{
			g_MeshImpact[nCnt].g_pIdxBuffMeshImpact->Release();
			g_MeshImpact[nCnt].g_pIdxBuffMeshImpact = NULL;
		}
	}
}

//=================================================================================================================
//���b�V���t�B�[���h�̍X�V����
//=================================================================================================================
void UpdateMeshImpact(void)
{
	VERTEX_3D* pVtx;

	Player* pPlayer = GetPlayer();

	// �Ռ��g�̍ő吔����
	for (int nCnt = 0 ; nCnt < MAX_IMPACT; nCnt++)
	{
		if (g_MeshImpact[nCnt].bUse == false)
		{
			continue;
		}

		int nCntVertex = 0;

		if (g_MeshImpact[nCnt].nType != IMPACTTYPE_SPKATANA)
		{
			g_MeshImpact[nCnt].OutRadius += g_MeshImpact[nCnt].speed;
			g_MeshImpact[nCnt].InRadius += g_MeshImpact[nCnt].speed;
		}
		else
		{
			g_MeshImpact[nCnt].OutRadius -= g_MeshImpact[nCnt].speed;
			g_MeshImpact[nCnt].InRadius -= g_MeshImpact[nCnt].speed;
		}
		//���_�o�b�t�@�����b�N
		g_MeshImpact[nCnt].g_pVtxBuffMeshImpact->Lock(0, 0, (void**)&pVtx, 0);

		D3DXVECTOR3 pos = g_MeshImpact[nCnt].pos;

		for (int nCntImpact = 0; nCntImpact <= g_MeshImpact[nCnt].nImpactPosX; nCntImpact++)
		{
			float fAngel = (D3DX_PI * 2.0f) / g_MeshImpact[nCnt].nImpactPosX * nCntImpact;

			//���_���W�̐ݒ�
			pVtx[nCntVertex].pos = D3DXVECTOR3(
				sinf(fAngel) * g_MeshImpact[nCnt].OutRadius,
				2.0f,
				cosf(fAngel) * g_MeshImpact[nCnt].OutRadius);

			pVtx[nCntVertex].col = g_MeshImpact[nCnt].col;

			nCntVertex++;
		}

		for (int nCntImpact = 0; nCntImpact <= g_MeshImpact[nCnt].nImpactPosX; nCntImpact++)
		{
			float fAngel = (D3DX_PI * 2.0f) / g_MeshImpact[nCnt].nImpactPosX * nCntImpact;

			//���_���W�̐ݒ�
			pVtx[nCntVertex].pos = D3DXVECTOR3(
				sinf(fAngel) * g_MeshImpact[nCnt].InRadius,
				2.0f,
				cosf(fAngel) * g_MeshImpact[nCnt].InRadius);

			pVtx[nCntVertex].col = g_MeshImpact[nCnt].col;

			nCntVertex++;
		}

		// ���������炷
		g_MeshImpact[nCnt].nLife--;

		// a�l�����񂾂񌸏��w����
		g_MeshImpact[nCnt].col.a -= g_MeshImpact[nCnt].decAlv;

		// �������s����
		if (g_MeshImpact[nCnt].nLife <= 0)
		{
			// ���g�p�ɂ���
			g_MeshImpact[nCnt].bUse = false;
		}
		// ���_�o�b�t�@�̃A�����b�N
		g_MeshImpact[nCnt].g_pVtxBuffMeshImpact->Unlock();
	}
}
//=================================================================================================================
//���b�V���t�B�[���h�̕`�揈��
//=================================================================================================================
void DrawMeshImpact(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntImpact = 0; nCntImpact < MAX_IMPACT; nCntImpact++)
	{
		if (g_MeshImpact[nCntImpact].bUse == false)
		{
			continue;
		}

		////�A���t�@�e�X�g��L��
		//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_MeshImpact[nCntImpact].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshImpact[nCntImpact].rot.y, g_MeshImpact[nCntImpact].rot.x, g_MeshImpact[nCntImpact].rot.z);
		D3DXMatrixMultiply(&g_MeshImpact[nCntImpact].mtxWorld, &g_MeshImpact[nCntImpact].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_MeshImpact[nCntImpact].pos.x, g_MeshImpact[nCntImpact].pos.y, g_MeshImpact[nCntImpact].pos.z);
		D3DXMatrixMultiply(&g_MeshImpact[nCntImpact].mtxWorld, &g_MeshImpact[nCntImpact].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_MeshImpact[nCntImpact].mtxWorld);

		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_MeshImpact[nCntImpact].g_pVtxBuffMeshImpact, 0, sizeof(VERTEX_3D));

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_MeshImpact[nCntImpact].g_pIdxBuffMeshImpact);

		//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_MeshImpact[nCntImpact].g_pTextureMeshImpact);

		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_MeshImpact[nCntImpact].Vertex, 0, g_MeshImpact[nCntImpact].Polygon);
	}

	////a�u�����f�B���O�����Ƃɖ߂�
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=================================================================================================================
// �C���p�N�g�̍쐬����
//=================================================================================================================
void CreateImpact(int nCntImpact, int nImpactPosX, int Vertex, int Index)
{
	float fTexX = 1.0f / nImpactPosX;
	float fTexY = 1.0f / IMPACT_Z;

	////�e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\TEXTURE\\.png",
	//	&g_pTextureMeshImpact);

		// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_MeshImpact[nCntImpact].g_pVtxBuffMeshImpact,
		NULL);

	VERTEX_3D* pVtx = NULL;
	int nCnt = 0;

	//���_�o�b�t�@�����b�N
	g_MeshImpact[nCntImpact].g_pVtxBuffMeshImpact->Lock(0, 0, (void**)&pVtx, 0);

	// �O���̔��a
	float OutRadius = g_MeshImpact[nCntImpact].OutRadius;

	D3DXCOLOR col = g_MeshImpact[nCntImpact].col;

	D3DXVECTOR3 pos = g_MeshImpact[nCntImpact].pos;

	//��
	for (int nCntH = 0; nCntH <= nImpactPosX; nCntH++)
	{
		float fAngel = (D3DX_PI * 2.0f) / nImpactPosX * nCntH;

		//���_���W�̐ݒ�
		pVtx[nCnt].pos = D3DXVECTOR3(
			pos.x + sinf(fAngel) * OutRadius,
			2.0f,
			pos.z + cosf(fAngel) * OutRadius);

		//�@���x�N�g���̐ݒ�
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntH, 1.0f);

		nCnt++;//���Z
	}

	// �O���̔��a
	float InRadius = g_MeshImpact[nCntImpact].InRadius;

	//��
	for (int nCntH = 0; nCntH <= nImpactPosX; nCntH++)
	{
		float fAngel = (D3DX_PI * 2.0f) / nImpactPosX * nCntH;

		//���_���W�̐ݒ�
		pVtx[nCnt].pos = D3DXVECTOR3(
			pos.x + sinf(fAngel) * InRadius,
			2.0f,
			pos.z + cosf(fAngel) * InRadius);

		//�@���x�N�g���̐ݒ�
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntH, 1.0f);

		nCnt++;//���Z
	}

	g_MeshImpact[nCntImpact].g_pVtxBuffMeshImpact->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * Index,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_MeshImpact[nCntImpact].g_pIdxBuffMeshImpact,
		NULL);

	WORD* pIdx;

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	g_MeshImpact[nCntImpact].g_pIdxBuffMeshImpact->Lock(0, 0, (void**)&pIdx, 0);

	int IndxNum = nImpactPosX + 1;//X

	int IdxCnt = 0;//�z��

	int Num = 0;//

	//�C���f�b�N�X�̐ݒ�
	for (int IndxCount1 = 0; IndxCount1 < IMPACT_Z; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= nImpactPosX; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_MeshImpact[nCntImpact].g_pIdxBuffMeshImpact->Unlock();
}
//=================================================================================================================
// �C���p�N�g�̔��菈��
//=================================================================================================================
bool CollisionImpact(D3DXVECTOR3* pPos)
{
	VERTEX_3D* pVtx = NULL;
	Player* pPlayer = GetPlayer();
	bool bHit = false;

	// �Ռ��g�̍ő吔����
	for (int nCnt = 0; nCnt < MAX_IMPACT; nCnt++)
	{
		// ���g�p��������
		if (g_MeshImpact[nCnt].bUse == false || g_MeshImpact[nCnt].nType == IMPACTTYPE_NORMAL)
		{// ������ǂݔ�΂�
			continue;
		}

		//���_�o�b�t�@�����b�N
		g_MeshImpact[nCnt].g_pVtxBuffMeshImpact->Lock(0, 0, (void**)&pVtx, 0);

		// X�̕�������
		for (int nCntvtx = 0; nCntvtx < g_MeshImpact[nCnt].Vertex; nCntvtx++)
		{
			// �ʒu�̍��������
			float DiffPosX = pPos->x - (g_MeshImpact[nCnt].pos.x + pVtx[nCntvtx].pos.x);
			float DiffPosY = pPos->y - (g_MeshImpact[nCnt].pos.y + pVtx[nCntvtx].pos.y);
			float DiffPosZ = pPos->z - (g_MeshImpact[nCnt].pos.z + pVtx[nCntvtx].pos.z);

			// ���_�Ƃ̋��������߂�
			float fDistance = sqrtf((DiffPosX * DiffPosX) + (DiffPosY * DiffPosY) + (DiffPosZ * DiffPosZ));

			// �傫�������߂�
			float DiffSize = g_MeshImpact[nCnt].OutRadius - g_MeshImpact[nCnt].InRadius;

			// �������C���p�N�g�̑傫�������������Ȃ�����
			if (fDistance <= DiffSize * 0.5f && g_MeshImpact[nCnt].nType == IMPACTTYPE_PLAYER)
			{	
				// �������Ă���
				bHit = true;
			}
			else if (fDistance <= DiffSize * 0.5f && g_MeshImpact[nCnt].nType != IMPACTTYPE_PLAYER)
			{
				// �������Ă���
				HitPlayer(20);
			}
		}

		// ���_�o�b�t�@�̃A�����b�N
		g_MeshImpact[nCnt].g_pVtxBuffMeshImpact->Unlock();
	}

	// �����Ԃ�
	return bHit;
}
//=================================================================================================================
// �C���p�N�g�̃_���[�W�ݒ菈��
//=================================================================================================================
int ImpactDamege(int nID)
{
	return g_MeshImpact[nID].nDamege;
}

//=================================================================================================================
// �C���p�N�g�̐ݒ菈��
//=================================================================================================================
void SetImpact(D3DXVECTOR3 pos, D3DXCOLOR col, int nImpactPosX, float OutRadius, float InRadius, float Speed, int nLife, int nType, int nDamege)
{
	// �Ռ��g�̍ő吔����
	for (int nCntImpact = 0; nCntImpact < MAX_IMPACT; nCntImpact++)
	{
		// ���g�p��������
		if (g_MeshImpact[nCntImpact].bUse == false)
		{
			// �ݒ菈��
			g_MeshImpact[nCntImpact].Vertex = (nImpactPosX + 1) * (IMPACT_Z + 1);   // ���_��
			g_MeshImpact[nCntImpact].Polygon = g_MeshImpact[nCntImpact].Vertex - 2; // �|���S����
			g_MeshImpact[nCntImpact].Index = g_MeshImpact[nCntImpact].Vertex;       // �C���f�b�N�X��
			g_MeshImpact[nCntImpact].nImpactPosX = nImpactPosX;                     // X�̕�����
			g_MeshImpact[nCntImpact].pos = pos;										// �ʒu
			g_MeshImpact[nCntImpact].OutRadius = OutRadius;							// �O���̔��a
			g_MeshImpact[nCntImpact].InRadius = InRadius;							// �����̔��a
			g_MeshImpact[nCntImpact].speed = Speed;							        // ����
			g_MeshImpact[nCntImpact].nLife = nLife;							        // ����
			g_MeshImpact[nCntImpact].nDamege = nDamege;							    // �U����
			g_MeshImpact[nCntImpact].nType = nType;							        // ���
			g_MeshImpact[nCntImpact].col = col;										// �F
			g_MeshImpact[nCntImpact].col.a = A_OFFSET;								// ���炩���ߔ������Ă���
			g_MeshImpact[nCntImpact].decAlv = A_OFFSET / nLife;                     // a�l�̌����l
			g_MeshImpact[nCntImpact].bUse = true;							        // �g�p����

			// �C���p�N�g�̍쐬����
			CreateImpact(nCntImpact, nImpactPosX, g_MeshImpact[nCntImpact].Vertex, g_MeshImpact[nCntImpact].Index);

			break;
		}
	}

}