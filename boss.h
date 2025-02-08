//============================
//
// �{�X [boss.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _BOSS_H_
#define _BOSS_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"
#include"motion.h"

//****************************
//�}�N����`
//****************************
#define MAX_BOSS (10)

//****************************
//�{�X�̍U�����
//****************************
typedef enum
{
	BOSSATTACK_NO = 0,
	BOSSATTACK_ATTACK,
	BOSSATTACK_MAX
}BOSSATTACK;

//****************************
//�{�X�̏��
//****************************
typedef enum
{
	BOSSSTATE_NORMAL = 0,
	BOSSSTATE_AGENT,
	BOSSSTATE_ATTACK,
	BOSSSTATE_DAMAGE,
	BOSSSTATE_DEATH,
	BOSSSTATE_MAX
}BOSSSTATE;


//****************************
//�{�X�̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot;//����
	D3DXVECTOR3 vtxMin,vtxMax;
	D3DXVECTOR3 Size;
	D3DXMATRIX mtxWorld;
	int nType;
	bool bUse;//�g�p���Ă��邩�ǂ���
	MOTION Motion;
	int nLife;
	int nDamage;
	int state;
	int nCounterState;
	int nIdxShadow;
	int AttackState;
	float Speed; // ���̑���
}Boss;

//****************************
//�v���g�^�C�v�錾
//****************************
void InitBoss(void);//�{�X�̏���������
void UninitBoss(void);//�{�X�̏I������
void UpdateBoss(void);//�{�X�̍X�V����
void DrawBoss(void);//�{�X�̕`�揈��
void SetBoss(D3DXVECTOR3 pos, float speed,int nLife);
void HitBoss(int nDamage);//�{�X�̃q�b�g����
Boss* Getboss(void);
#endif
