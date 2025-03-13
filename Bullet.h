//=============================
//
// �e�̏��� [ bullet.h ]
// Author:�����I��
//
//=============================
#ifndef _BULLET_H_
#define _BULLET_H_

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "main.h"

//****************************
// �e�̍\���̐錾
//****************************
typedef struct
{
	D3DXVECTOR3 pos; // ���W
	D3DXVECTOR3 posDest; // �ړI�̍��W
	D3DXVECTOR3 rot; // �p�x
	D3DXVECTOR3 dir; // �����x�N�g��
	D3DXVECTOR3 move;// �ړ���
	D3DXMATRIX mtxWorld; // ���[���h�}�g���b�N�X
	int nLife;       // ����
	int nIdxShadow;  // �e�̃C���f�b�N�X
	int nDamege;     // �_���[�W��
	float fSize;     // �e�̑傫��
	float fSpeed;    // ���x
	bool bUse;       // �g�p���Ă��邩�ۂ�
	bool bEnemy;     // �G���ۂ�
}Bullet;

//****************************
// �v���g�^�C�v�錾
//****************************
void InitBullet(void);  // �e�̏���������
void UninitBullet(void);// �e�̏I������
void UpdateBullet(void);// �e�̍X�V����
void DrawBullet(void);  // �e�̕`�揈��
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 dir, int nLife, int nDamege,float fSize, float fSpeed,bool Enemy); // �e�̐ݒ菈��
void KillBullet(int nNum); // ���g�p�ɂ��鏈��
Bullet* GetBullet();    // �e���̎擾����

#endif