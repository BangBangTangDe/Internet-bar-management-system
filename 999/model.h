#pragma once
#include<time.h>
struct Card
{
	char aName [18];
	char aPwd[8];
	int nStatus;  //��״̬(UNUSE - δ�ϻ���USING - �����ϻ���INVALID - ��ע��
	time_t tstart; //����ʱ��
	float totaluse;//�ۼƽ��
	time_t tLast; //���ʹ��ʱ��
	int nusecount; //ʹ�ô���
	float balance;//���
};
struct CardNode
{
	Card data;
	struct CardNode*next;

};


//��ѯ�����Ľṹ��
struct querryonecard
{
	char aCardName[18];
	float dollar;
};

//�Ʒ���Ϣ�ṹ��
struct Billing
{
	char aCardName[18];//�ϻ�����
	time_t tStart;
	time_t tEnd;
	float fAmount;//���ѽ��
	int nStatus;//�ϻ�״̬
	int nDel;//ɾ����ʶ


};

//�Ʒ���Ϣ�ڵ�
struct BillingNode
{
	Billing data;
	struct BillingNode*next;


};

//�ϻ��ṹ��
struct LogonInfo
{
	char aCardName[18];
	time_t tLogon;
	float fbalance;
};
//�»��ṹ��
struct SettleInfo
{
	char aCardName[18];
	time_t tStart;
	time_t tEnd;
	float fAmount; //���ѽ��
	float fbalance;  //���
};
//��ֵ�ṹ��
struct addmoney
{
	char aCardName[18];
	float afterbalance;

};
//�˷ѽṹ��
struct backmoney
{
	char aCardName[18];
	float afterbackmoney;

};
//ע�����Ľṹ��

struct logoutcard
{
	char aCardName[18];
	float money;


};


//��ѯ���������Ѽ�¼
struct onebillremember
{
	char aCardName[18];
	float money;
	float fAmount;
	time_t tStart;
	time_t tEnd;
};
