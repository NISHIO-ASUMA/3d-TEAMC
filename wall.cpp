//==============================================================================================================
//
// �� [wall.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "wall.h"
#include "player.h"
#include "input.h"
#include "edit.h"

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define SIZE_VALUE (0.1f) // �ǂ̊g�嗦

//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;	   // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL; // ���_�o�b�t�@�ւ̃|�C���^
WALL g_Wall[MAX_WALL];						   // �ǂ̍\����
int nNumWall;								   // �ǂ̐�

//===============================================================================================================
// �ǂ̏���������
//===============================================================================================================
void InitWall(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\city.png",
		&g_pTextureWall);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = NULL;

	// �\���̂̏�����
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_Wall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[nCntWall].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[nCntWall].vtxPos[nCntWall] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[nCntWall].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[nCntWall].fA = 1.0f;
		g_Wall[nCntWall].bUse = false;
		g_Wall[nCntWall].fScal = D3DXVECTOR3(0.0f,0.0f,0.0f);//������߂�
	}

	// �O���[�o���ϐ��̏�����
	nNumWall = 0;

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos.x = -WALL_WIDTH;
		pVtx[0].pos.y = WALL_HEIGHT;
		pVtx[0].pos.z = 0.0f;
				   
		pVtx[1].pos.x = WALL_WIDTH;
		pVtx[1].pos.y = WALL_HEIGHT;
		pVtx[1].pos.z = 0.0f;
				   
		pVtx[2].pos.x = -WALL_WIDTH;
		pVtx[2].pos.y = -WALL_HEIGHT;
		pVtx[2].pos.z = 0.0f;
				   
		pVtx[3].pos.x = WALL_WIDTH;
		pVtx[3].pos.y = -WALL_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		// �e���_�̖@���̐ݒ�(�x�N�g���̑傫���͂P�ɂ���K�v������)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffWall->Unlock();
}
//===============================================================================================================
// �ǂ̏I������
//===============================================================================================================
void UninitWall(void)
{
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//���_�o�b�t�@�̉��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}
//===============================================================================================================
// �ǂ̍X�V����
//===============================================================================================================
void UpdateWall(void)
{
	// �Ȃ�
}
//===============================================================================================================
// �ǂ̕`�揈��
//===============================================================================================================
void DrawWall(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans,mtxScal;

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_Wall[nCntWall].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorld);

			// �傫���𔽉f
			D3DXMatrixScaling(&mtxScal, g_Wall[nCntWall].Size.x, g_Wall[nCntWall].Size.y, g_Wall[nCntWall].Size.z);
			D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxScal);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxTrans);
			
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			// �e�N�X�`���t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWall);
			 
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
		}
	}
}
//===============================================================================================================
//�ǂ̐ݒ菈��
//===============================================================================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fA, D3DXVECTOR3 Size)
{
	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_Wall[nCntWall].bUse == false)
		{
			g_Wall[nCntWall].pos = pos;	  // ���W
			g_Wall[nCntWall].Size = Size; // �傫��
			g_Wall[nCntWall].fA = fA;	  // �A���t�@�l
			g_Wall[nCntWall].rot = rot;	  // �p�x
										  
			g_Wall[nCntWall].bUse = true; // �g�p����

			// �e���_�̖@���̐ݒ�(�x�N�g���̑傫���͂P�ɂ���K�v������)
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// �J�E���g�����Z
			nNumWall++;

			break;
		}
		// ���_����i�߂�
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffWall->Unlock();
}
//============================================================================================================
// �ǂƂ̏Փ˔���
//============================================================================================================
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3* pPosOld,D3DXVECTOR3 *pMove,float speed)
{
	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	int nCntPos = 0;
	int nNextWall = 1;

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_Wall[nCnt].bUse == true)
		{
			g_Wall[nCnt].vtxPos[0].x = g_Wall[nCnt].pos.x - cosf(g_Wall[nCnt].rot.y) * WALL_WIDTH;
			g_Wall[nCnt].vtxPos[0].y = g_Wall[nCnt].pos.y + WALL_HEIGHT;
			g_Wall[nCnt].vtxPos[0].z = g_Wall[nCnt].pos.z + sinf(g_Wall[nCnt].rot.y) * WALL_WIDTH;

			g_Wall[nCnt].vtxPos[1].x = g_Wall[nCnt].pos.x + cosf(g_Wall[nCnt].rot.y) * WALL_WIDTH;
			g_Wall[nCnt].vtxPos[1].y = g_Wall[nCnt].pos.y + WALL_HEIGHT;
			g_Wall[nCnt].vtxPos[1].z = g_Wall[nCnt].pos.z - sinf(g_Wall[nCnt].rot.y) * WALL_WIDTH;

			g_Wall[nCnt].vtxPos[2].x = g_Wall[nCnt].pos.x - sinf(g_Wall[nCnt].rot.y) * WALL_WIDTH;
			g_Wall[nCnt].vtxPos[2].y = g_Wall[nCnt].pos.y - WALL_HEIGHT;
			g_Wall[nCnt].vtxPos[2].z = g_Wall[nCnt].pos.z - cosf(g_Wall[nCnt].rot.y) * WALL_WIDTH;

			D3DXVECTOR3 VecALine,VecALine2; // ���_0�`���_1�̃x�N�g��

			D3DXVECTOR3 vecWall;	// ���όv�Z�p�̃x�N�g��

			VecALine = g_Wall[nCnt].vtxPos[1] - g_Wall[nCnt].vtxPos[0]; // 0-1...
			VecALine2 = g_Wall[nCnt].vtxPos[2] - g_Wall[nCnt].vtxPos[0];

			D3DXVec3Normalize(&VecALine, &VecALine);	// ���K��
			D3DXVec3Normalize(&VecALine2, &VecALine2);	// ���K��

			D3DXVECTOR3 PlayerVec;	// �v���C���[�̃x�N�g��

			D3DXVECTOR3 VecBLine;	// �ǂ���v���C���[

			VecBLine = *pPos - g_Wall[nCnt].vtxPos[0];	// �ǂ���v���C���[�Ƀx�N�g��������

			D3DXVec3Normalize(&VecBLine, &VecBLine); // ���K��

			D3DXVECTOR3 Cross = {}; // �O��

			D3DXVec3Cross(&Cross, &VecALine, &VecBLine);	// �O�ς��v�Z
			D3DXVec3Cross(&vecWall, &VecALine, &VecALine2); // �O�ς��v�Z

			// ����
			float Dot = D3DXVec3Dot(&vecWall, &VecBLine);

			// �v���C���[���ǂ̊O�ɏo��
			if (Cross.y < 0)
			{
				D3DXVECTOR3 VecMoveF = *pPos - *pPosOld; // �i�s�x�N�g��

				D3DXVECTOR3 Vector1 = g_Wall[nCnt].vtxPos[1] - g_Wall[nCnt].vtxPos[0];
				D3DXVECTOR3 Vector2 = g_Wall[nCnt].vtxPos[2] - g_Wall[nCnt].vtxPos[0];
				D3DXVECTOR3 Nor;

				D3DXVec3Cross(&Nor, &Vector1, &Vector2);
				D3DXVec3Normalize(&Nor, &Nor);
				float VecA = D3DXVec3Dot(&VecMoveF, &Nor);

				D3DXVECTOR3 WallMove = VecMoveF - VecA * Nor;

				D3DXVec3Normalize(&WallMove, &WallMove);

				D3DXVECTOR3 vector = *pPosOld - WallMove;

				D3DXVec3Normalize(&vector, &vector);

				pMove->x = 0.0f;
				pMove->z = 0.0f;

				pPos->x -= vector.x * speed;
				pPos->z -= vector.z * speed;

				pMove->x += WallMove.x;
				pMove->z += WallMove.z;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffWall->Unlock();
}
////==========================================================================================================
////�e�̔���
////==========================================================================================================
//void CollisionBullet(BULLET* pBullet)
//{
//	VERTEX_3D* pVtx;
//
//	PLAYER* pPLayer = GetPlayer();
//
//	//���_�o�b�t�@�����b�N
//	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
//	{
//		if (pBullet->bUse == true)
//		{
//			D3DXVECTOR3 VecALine;
//
//			D3DXVECTOR3 pos, pos1;
//
//			VecALine = g_Wall[nCntWall].vtxPos[1] - g_Wall[nCntWall].vtxPos[0];//0-1...
//
//			D3DXVec3Normalize(&VecALine, &VecALine);
//
//			D3DXVECTOR3 PlayerVec;
//
//			D3DXVECTOR3 VecBLine;//�ǂ���v���C���[
//
//			VecBLine = pBullet->pos - g_Wall[nCntWall].vtxPos[0];
//
//			D3DXVec3Normalize(&VecBLine, &VecBLine);
//
//			D3DXVECTOR3 Cross = {};
//
//			D3DXVec3Cross(&Cross, &VecALine, &VecBLine);
//
//			if (Cross.y < 0)
//			{
//				D3DXVECTOR3 BulletVec,vecC,vecMove;
//				D3DXVECTOR3 WallLineB;
//				D3DXVECTOR3 BulletCross;
//
//				BulletVec = pBullet->pos - pBullet->posOld;
//
//				WallLineB = g_Wall[nCntWall].vtxPos[2] - g_Wall[nCntWall].vtxPos[0];
//
//				D3DXVec3Cross(&BulletCross, &VecALine, &WallLineB);
//
//				D3DXVec3Normalize(&BulletCross, &BulletCross);
//
//				float fDot = D3DXVec3Dot(&BulletCross, &BulletVec);
//
//				vecC = BulletCross * fDot;
//
//				D3DXVec3Normalize(&vecC, &vecC);
//
//				vecMove = BulletVec + (vecC * 2.0f * fDot);
//
//				D3DXVec3Normalize(&vecMove, &vecMove);
//
//				pBullet->move.x = vecMove.x * 20.0f;
//				pBullet->move.z = vecMove.z * 20.0f;
//			}
//		}
//	}
//	
//	g_pVtxBuffWall->Unlock();
//}
//============================================================================================================
// �ǂ̎擾
//============================================================================================================
WALL* GetWall(void)
{
	return &g_Wall[0];
}
//============================================================================================================
// �ǂ̐��̎擾
//===========================================================================================================
int NumWall(void)
{
	return nNumWall;
}
