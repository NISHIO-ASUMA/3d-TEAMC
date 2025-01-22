//============================
//
// �G[enemy.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _ENEMY_H_
#define _ENEMY_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"
#include"motion.h"

//****************************
//�}�N����`
//****************************
#define MAX_ENEMY (64)
#define ENEMYFILE001 ("data\\enemy.txt")
#define ENEMYFILE002 ("data\\enemy2.txt")
#define ENEMYFILE003 ("data\\enemy3.txt")

//**************************
//�v���C���[�̎��
//**************************
typedef enum
{
	ENEMYMOTION_NEUTRAL = 0,
	ENEMYMOTION_AGENT,
	ENEMYMOTION_ATTACK,
	ENEMYMOTION_ATTACK2,
	ENEMYMOTION_ATTACK3,
	ENEMYMOTION_MAX
}ENEMYMOTION;

//****************************
//�G�̍U�����
//****************************
typedef enum
{
	ENEMYATTACK_NO = 0,
	ENEMYATTACK_ONE,
	ENEMYATTACK_TWO,
	ENEMYATTACK_THREE,
	ENEMYATTACK_MAX
}ENEMYATTACK;

//****************************
//�G�̏��
//****************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_AGENT,
	ENEMYSTATE_ATTACK,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_DEATH,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//****************************
//�G�̎��
//****************************
typedef enum
{
	ENEMYTYPE_ONE = 0, 
	ENEMYTYPE_TWO,
	ENEMYTYPE_THREE,
	ENEMYTYPE_FOUR,
	ENEMYTYPE_FIVE,
	//ENEMYTYPE_SEVEN,
	//ENEMYTYPE_ATE,
	ENEMYTYPE_MAX
}ENEMYTYPE;

//****************************
//�G�̎�ލ\����
//****************************
typedef struct
{
	LPD3DXMESH pMeshEnemy;//���b�V��(���_���W)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMatEnemy;//�}�e���A���ւ̃|�C���^
	DWORD dwNumMatEnemy;//�}�e���A���̐�
	LPDIRECT3DTEXTURE9 pTextureEnemy[128];
	D3DCOLORVALUE Diffuse,FirstDiffuse;
}EnemyModel;

//****************************
//�G�̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 offpos, offrot;
	D3DXVECTOR3 rot;//����
	//D3DXVECTOR3 vtxMin,vtxMax;
	//D3DXVECTOR3 Size;
	D3DXMATRIX mtxWorldEnemy;
	float fDistance;
	ENEMYTYPE nType;
	bool bUse;//�g�p���Ă��邩�ǂ���
	MOTION Motion;
	D3DXVECTOR3 AttackEnemy;
	D3DXVECTOR3 Size;
	D3DXVECTOR3 vtxMin,vtxMax;
	float radius;
	float EnemyRadius;//�G�̔��a(�G�ƌ��̓����蔻��p)
	float fenemycollision;
	float posRadius;//�z�[�~���O�͈�
	float AttackRadius;//�U���͈�
	float radAttackEnemy;//�U�����[�V�����J�ڔ͈�
	float fcollision;//�G�ƓG
	int nLife;
	int nDamage;
	ENEMYSTATE state;
	int nCounterState;
	int nIdxShadow;
	int nIdxLifeBar,nIdxLifeFrame;
	int nIdxUi;
	EnemyModel EnemyModel[16];
	bool g_bDamage;
	float fMove;//�����_��
	ENEMYATTACK AttackState;
}ENEMY;

//****************************
//�v���g�^�C�v�錾
//****************************
void InitEnemy(void);//�G�̏���������
void UninitEnemy(void);//�G�̏I������
void UpdateEnemy(void);//�G�̍X�V����
void DrawEnemy(void);//�G�̕`�揈��
ENEMY* GetEnemy(void);//�G�̎擾����
void HitEnemy(int nCnt,int nDamage);//�G�̃q�b�g����
void SetEnemy(D3DXVECTOR3 pos,ENEMYTYPE nType,int nLife,D3DXVECTOR3 move);//�G�̐ݒ菈��
int GetNumEnemy(void);//�G�̑����擾����
#endif
