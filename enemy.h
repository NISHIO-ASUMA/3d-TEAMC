//==============================================================================================================
//
// �G[enemy.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include"main.h"
#include"motion.h"

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define MAX_ENEMY (100)
#define ENEMYFILE001 ("data\\enemy.txt")
#define ENEMYFILE002 ("data\\enemy2.txt")
#define ENEMYFILE003 ("data\\enemy3.txt")

//**************************************************************************************************************
//�G�̍U�����
//**************************************************************************************************************
typedef enum
{
	ENEMYATTACK_NO = 0,
	ENEMYATTACK_ATTACK,
	ENEMYATTACK_MAX
}ENEMYATTACK;

//**************************************************************************************************************
//�G�̏��
//**************************************************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_AGENT,
	ENEMYSTATE_ATTACK,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_DEATH,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//**************************************************************************************************************
//�G�̎��
//**************************************************************************************************************
typedef enum
{
	ENEMYTYPE_ONE = 0, 
	ENEMYTYPE_TWO,
	ENEMYTYPE_THREE,
	ENEMYTYPE_FOUR,
	ENEMYTYPE_FIVE,
	ENEMYTYPE_SIX,
	ENEMYTYPE_SEVEN,
	ENEMYTYPE_MAX
}ENEMYTYPE;

//**************************************************************************************************************
//�G�̍\����
//**************************************************************************************************************
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
	int nType;
	bool bUse;//�g�p���Ă��邩�ǂ���
	MOTION Motion;
	D3DXVECTOR3 Size;
	D3DXVECTOR3 vtxMin,vtxMax;
	int nLife;
	int nMaxLife;
	int nDamage;
	ENEMYSTATE state;
	int nCounterState;
	int nIdxShadow;
	int nIdxLifeBar,nIdxLifeFrame;
	int nIdxUi;
	bool g_bDamage;
	float fMove;//�����_��
	int nCountAction;
	int AttackState;
	float Speed; // ���̑���
	D3DXVECTOR3 rotDest; // �ړI�̊p�x
	D3DXVECTOR3 PosDest; // �ړI�̈ʒu
	int nIdxMap;
	int HitStopCount;    // �q�b�g�X�g�b�v
}ENEMY;

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void InitEnemy(void);//�G�̏���������
void UninitEnemy(void);//�G�̏I������
void UpdateEnemy(void);//�G�̍X�V����
void DrawEnemy(void);//�G�̕`�揈��
ENEMY* GetEnemy(void);//�G�̎擾����
void HitEnemy(int nCnt,int nDamage);//�G�̃q�b�g����
void SetEnemy(D3DXVECTOR3 pos,int nType,int nLife,float Speed);//�G�̐ݒ菈��
void SpawnEnemy(int nSpawner);
int GetNumEnemy(void);//�G�̑����擾����
bool CollisionView(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, float fRange, float viewAngle); // ���E�̔���
float SetAngle(D3DXVECTOR3* pRot, D3DXVECTOR3* pPos);
//bool CollisionLine(D3DXVECTOR3* pFirstPos, D3DXVECTOR3* pEndPos,float fRadius); // ���Ƌ��̓����蔻��
int GetNumKill(void); // �L����
void SetSoundWepon(int nType);									  // ���킪�G�ɓ����������̉��̐ݒ菈��
void SetCreateWeponSound(int nType);							  // �������킪�����������̃T�E���h�ݒ菈��

#endif
