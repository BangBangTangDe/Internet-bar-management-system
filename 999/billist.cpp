#include"stdafx.h"
#include<string>
#include<iostream>
#include<fstream>

using namespace std;
#include "menu.h" 
#include "global.h"
#include"model.h"
#include"cardlist.h"
#include"billist.h"
#include"baocuncard.h"
#include"tool.h"



// ���ļƷ���Ϣ�Ƿ���ڷ��ؼƷ��������еĽڵ�
Billing* BillingIsExist(string strNo, int& nBillingIndex, BillingNode *pBillNodeHead)
{    
	BillingNode *pBillingNode = pBillNodeHead; 


       nBillingIndex = 0; 
while (pBillingNode != NULL)
{ 
	if (strcmp(pBillingNode->data.aCardName, strNo.c_str()) == 0 && pBillingNode->data.nStatus == NOSETTLEMENT) 
	{ 
		return &(pBillingNode->data);
	
	}   
	pBillingNode = pBillingNode->next;   
	nBillingIndex++;
}

return NULL; 
}  

//�ж��Ƿ����ͳ�Ʋ�ѯ�����ı�������
Billing* BillingIsExisttwo(string strNo, int& nBillingIndex, BillingNode *pBillNodeHead)
{
	BillingNode *pBillingNode = pBillNodeHead;


	nBillingIndex = 0;
	while (pBillingNode != NULL)
	{
		if (strcmp(pBillingNode->data.aCardName, strNo.c_str()) == 0 && pBillingNode->data.nStatus == YESSETTLEMENT)
		{
			return &(pBillingNode->data);

		}
		pBillingNode = pBillingNode->next;
		nBillingIndex++;
	}

	return NULL;
}

//�жϿ����ϻ�
int logon(string strNo, string strPwd, LogonInfo* pInfo, CardNode* pCardNodeHead, BillingNode **ppBillingNodeHead)
{
	Billing billing;  // �Ʒ���Ϣ   
	int nCardIndex;
	Card* pCard = CardIsExist(strNo, nCardIndex, pCardNodeHead);

	if (pCard == NULL)
		return NOFINDCARD;      // δ�ҵ��� 

	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
		return NOMATCH;         // ���벻ƥ�� 

	if (pCard->nStatus == USING)
	{
		return USING;           // �������ϻ� 
	}

	if (pCard->nStatus == INVALID)
	{
		return INVALID;         // ���Ѿ�ע��  
	}

	if (pCard->balance <= 0) {

		return ENOUGHMONEY;     // �������Ϊ0  
	}

	// ��������ϻ������¿���Ϣ 
	pCard->nStatus = USING;
	// ״̬Ϊ����ʹ��  
	pCard->nusecount++;
	// ʹ�ô�����1 
	pCard->tLast = time(NULL);
	// �������ʹ��ʱ��Ϊ��ǰʱ�� 
	
	// �����ļ��еĿ���Ϣ  
	updateCard(pCard, CARDPATH, nCardIndex);

	// ������Ѽ�¼  
	strcpy(billing.aCardName, strNo.c_str());
	// �ϻ����� 
	billing.tStart = time(NULL);
	// �ϻ�ʱ�� 
	billing.tEnd = 0;                            // �»�ʱ�� 
	billing.nStatus = NOSETTLEMENT;              // ����״̬ 
	billing.fAmount = 0;                         // ���ѽ�� 

												 // �Ƚ��Ʒ���Ϣ���浽�ļ���
	saveBilling(&billing, "billing.dat");

	// �ڼƷ�����������һ���Ʒ���Ϣ 
	BillingNode* pBillingNode = new BillingNode;  pBillingNode->data = billing;

	BillingNode* pBillingNodeTail = *ppBillingNodeHead;

	pBillingNode->next = NULL;
	if (*ppBillingNodeHead == NULL)
	{
		*ppBillingNodeHead = pBillingNode;
		//pBillingNodeTail = pBillingNode;
	}
	else
	{
		while (pBillingNodeTail->next != NULL)
			pBillingNodeTail = pBillingNodeTail->next;//whileѭ���ҵ�ĩβΪ�յĽڵ�


		pBillingNodeTail->next = pBillingNode;
		//pBillingNodeTail = pBillingNode;
	}   
	
	//pBillingNodeTail->next = NULL;

	// ��װ�ϻ���Ϣ 
	strcpy(pInfo->aCardName, strNo.c_str());
	pInfo->fbalance = pCard->balance;
	pInfo->tLogon = billing.tStart;
	return SUCCESS;
}
double getAmount(time_t tStart)
{
	double dbAmount = 0;       // ���ѽ��
	int nCount = 0;            // �ϻ���ʱ�䵥Ԫ����ÿ����Ԫ15����  
	int nSec = 0;              // ����ʱ��(��λ����)  
	int nMinutes = 0;          // ����ʱ��(��λ������)  
	time_t tEnd = time(NULL);  // ����ʱ��Ϊ��ǰʱ�� 


							   // ��������ʱ�� 
	nSec = (int)(tEnd - tStart);
	nMinutes = nSec / 60;

	// �������ѵ�ʱ�䵥Ԫ�� 
	if (nMinutes % UNIT == 0)
	{

		nCount = nMinutes / UNIT;//��С�ļ�ʱ��Ԫ
	}
	else 
	{
		nCount = nMinutes / UNIT + 1;//�������ʱ��Ҳ��15���Ӽ���
	}

	if (nCount == 0)
		nCount = 1;

	// �������ѽ��  
	dbAmount = nCount * CHARGE;

	return dbAmount;
}//�������ѽ��ĺ���
bool updateBilling(const Billing* pBilling, const string pPath, int nBillingIndex)
{
	fstream ofile;
	ofile.open(pPath, ios_base::in | ios_base::out);
	ofile.seekp(sizeof(Billing)*nBillingIndex, ios_base::beg);
	ofile.write((char*)pBilling, sizeof(Billing));
	ofile.close();

	return true;
}
int settle(string strNo, string strPwd, SettleInfo* pInfo, CardNode* const pCardNodeHead, BillingNode* const pBillingNodeHead)
{
	int nCardIndex;
	Card* pCard = CardIsExist(strNo, nCardIndex, pCardNodeHead);

	// δ�ҵ���     
	if (pCard == NULL)
		return NOFINDCARD;

	// ���벻ƥ��    
	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
		return NOMATCH;

	// �жϸÿ��Ƿ������ϻ���ֻ�������ϻ������ܽ����»�����  
	if (pCard->nStatus != USING)   return UNUSE;

	// ���ݿ��ţ���ѯ�Ʒ���Ϣ 
	int nBillingIndex;
	Billing* pBilling = BillingIsExist(strNo, nBillingIndex, pBillingNodeHead);


	// �����ѯ�Ʒ���ϢΪ�գ���ʾ�»�ʧ��
	if (pBilling == NULL) { cout << "�Ʒ���ϢΪ��" << endl;   return UNUSE; }

	// �������ѽ��
	double dbAmount = getAmount(pBilling->tStart);


	// ������С�����ѽ����ܽ����»�  
	float fBalance = pCard->balance - (float)dbAmount;
	if (fBalance < 0) { return ENOUGHMONEY; }

	// ���¿���Ϣ 
	pCard->balance = fBalance;        // ��� 
	pCard->nStatus = UNUSE;            // ״̬ 
	pCard->tLast = time(NULL);         // �ϴ�ʹ��ʱ�� 
	pCard->totaluse = pCard->totaluse + dbAmount;
									   // �����ļ��еĿ���Ϣ 
	updateCard(pCard, CARDPATH, nCardIndex);

	// ���¼Ʒ���Ϣ  
	pBilling->fAmount = (float)dbAmount;    // ������Ϣ 
	pBilling->nStatus = YESSETTLEMENT;       // ״̬���ѽ���
	pBilling->tEnd = time(NULL);            // �»�ʱ�� 

											// �����ļ��еļƷ���Ϣ  
	updateBilling(pBilling, BILLINGPATH, nBillingIndex);



	// ��װ�»���Ϣ  
	strcpy(pInfo->aCardName, strNo.c_str());    // ���� 
	pInfo->fAmount = (float)dbAmount;           // ���ѽ��  
	pInfo->fbalance = fBalance;                 // ���
	pInfo->tStart = pBilling->tStart;           // �ϻ�ʱ��
	pInfo->tEnd = time(NULL);                   // �»�ʱ�� 

	return SUCCESS;

}
// �����ϻ�ʱ�䣬�������ѽ��
//�ж��ܷ��ֵ
int judgeaddmoney(string strNo , string strPwd, addmoney* p, CardNode* pCardNodeHead)
{
	
	 int a;
	int nCardIndex;

	Card* pCard = CardIsExist(strNo, nCardIndex, pCardNodeHead);

if (pCard == NULL)
return NOFINDCARD;      // δ�ҵ��� 

if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
return NOMATCH;         // ���벻ƥ�� 
if (pCard->nStatus == INVALID)
{
	return INVALID;         // ���Ѿ�ע��  
}

cout << "���е����Ϊ��" << pCard->balance;
  cout << "������Ҫ��ֵ�Ľ��";
   cin >> a;
   pCard->balance + a;
 p->afterbalance = pCard->balance + a;

										
// ��װ��ֵ��Ϣ 
strcpy(p->aCardName, strNo.c_str());
 pCard->balance= p->afterbalance;


// �����ļ��еĿ���Ϣ  
updateCard(pCard, CARDPATH, nCardIndex);


return SUCCESS;
}
//�ж��ܷ��˷�
int judgebackmoney(string strNo, string strPwd, backmoney* pBack, CardNode* pCardNodeHead)
{

	 unsigned  int a;
	int nCardIndex;
	Card* pCard = CardIsExist(strNo, nCardIndex, pCardNodeHead);

	if (pCard == NULL)
		return NOFINDCARD;      // δ�ҵ��� 

	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
		return NOMATCH;         // ���벻ƥ�� 

	if (pCard->nStatus == USING)
	{
		return USING;           // �������ϻ� 
	}

	if (pCard->nStatus == INVALID)
	{
		return INVALID;         // ���Ѿ�ע��  
	}

	if (pCard->balance <= 0) {

		return ENOUGHMONEY;     // �������Ϊ0  
	}
	while (1)
	{
		cout << "���е����Ϊ��" << pCard->balance;
		cout << "������Ҫ�˷ѵĽ��";
		cin >> a;
			if (a <= pCard->balance)
			{
				pBack->afterbackmoney = pCard->balance - a;
				break;
			}
			else
			{
				cout << "�����������˷�Ҫ��" << endl;
			}
	}
	//��װ�˷ѵ���Ϣ
	strcpy(pBack->aCardName, strNo.c_str());
	pCard->balance = pBack->afterbackmoney;


	// �����ļ��еĿ���Ϣ  
	updateCard(pCard, CARDPATH, nCardIndex);



	return SUCCESS;
}

//�ж��ܷ�ע��
int judgelogout(string strNo, string strPwd,logoutcard* pout, CardNode* pCardNodeHead)
{
	unsigned int a;
	int nCardIndex;

	Card* pCard = CardIsExist(strNo, nCardIndex, pCardNodeHead);

	if (pCard == NULL)
		return NOFINDCARD;      // δ�ҵ��� 

	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
		return NOMATCH;         // ���벻ƥ�� 

	if (pCard->nStatus == USING)
	{
		return USING;           // �������ϻ� 
	}
	if (pCard->nStatus == INVALID)
	{
		return INVALID;         // ���Ѿ�ע��  
	}
	if (pCard->balance <0) 
	{

		return ENOUGHMONEY;     // �������Ϊ0  
	}
	
	while (1)
	{ 
		
		cout << "���е����Ϊ��" << pCard->balance;
		cout << "��ȷ��ע������ 1 �� " << endl;
	cout << "�����룺";
	cin >> a;
	   if (a=1)
	{
		
		cout << "��������Ѿ����." << endl;
		break;

	}
	   else
	   {
		   cout << "������ѡ�����" << endl;
	   }


	}

	pout->money = 0;
	pCard->nStatus = INVALID;
	// ��װע����Ϣ 
	strcpy(pout->aCardName, strNo.c_str());
	pCard->balance = pout->money;
	// �����ļ��еĿ���Ϣ  
	updateCard(pCard, CARDPATH, nCardIndex);



	return SUCCESS;

	
}

/*
//�жϵ������Ƿ�������֮��
int querrycard(string strNo, string strPwd, querryonecard* q, CardNode* pCardNodeHead)
{
	int a;
	int nCardIndex;

	Card* pCard = CardIsExist(strNo, nCardIndex, pCardNodeHead);

	if (pCard == NULL)
		return NOFINDCARD;      // δ�ҵ��� 

	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
		return NOMATCH;         // ���벻ƥ�� 
	if (pCard->nStatus == INVALID)
	{
		return INVALID;         // ���Ѿ�ע��  
	}

	return SUCCESS;
}

*/

//��ѯͳ�ƺ������ֵľ��庯������~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���ҵ�������������Ϣ�����п���������Ϣ
void querryonebill(BillingNode* pBillNodeHead, CardNode* pCardNodeHead)
{
	string strNo;
	Billing * pbill;
	CardNode * q = pCardNodeHead;
	if (inputCardNo(strNo))
	{
		int nBillingIndex;
		nBillingIndex = 0;
		pbill = BillingIsExisttwo(strNo, nBillingIndex, pBillNodeHead);
		cout << endl << "----------����������Ϣ----------" << endl;

		if (pbill == NULL)
		{
			cout << endl << endl << "������Ϣ�����ڣ�" << endl << endl;
			return;
		}                                                // /t�����ã�������ĸ�

		cout << "����\t״̬\t���ѽ��\t �ϻ�ʱ��\t\t�»�ʱ��" << endl;
		if (pbill != NULL)
		{
			char aStartTime[TIMELENGTH] = { 0 };
			char aEndTime[TIMELENGTH] = { 0 };
			timeToString(pbill->tStart, aStartTime);
			timeToString(pbill->tEnd, aEndTime);
			cout << pbill->aCardName << "\t";
			if ((q->data).nStatus == USING)
				cout << "�ϻ�\t";
			else if ((q->data).nStatus == UNUSE)
				cout << "δ�ϻ�\t";
			else if ((q->data).nStatus == INVALID)
				cout << "ע��\t";

			cout << pbill->fAmount << "\t\t";
			cout << aStartTime << "    " << "\t" << aEndTime << endl;

		}
	}

}
//��Ӫҵ��ͳ��
void totalMoney(BillingNode* pBillNodeHead)
{
	float tMoney = 0;
	BillingNode * t = pBillNodeHead;
	if (t == NULL)
	{
		cout << "���������Ѷ�" << endl;
	}

	while (t != NULL)
	{
		tMoney += (t->data).fAmount;
		t = t->next;
	}

	cout << "��Ӫҵ��Ϊ��" << tMoney << endl;
}
//��ʱ�β�ѯ�������Ѽ�¼
void recordtimebill(BillingNode* pBillNodeHead, CardNode* pCardNodeHead)
{//time_tֻ��һ�������ε�����   tm��һ���ṹ��

	int year1, month1, day1;
	int year2, month2, day2;
	cout << "�������ѯ���Ѽ�¼��ʱ��Σ�" << endl;
	cout << "��ѯ��ʼ���ڣ�"<<endl; 
	cout << "��--->"; cin >> year1;   cout << "��--->"; cin >> month1;  cout << "��--->"; cin >> day1; cout << endl;
	cout << "��ѯ�������ڣ�"<<endl;
	cout << "��--->"; cin >> year2;  cout << "��--->"; cin >> month2; cout << "��--->"; cin >> day2; cout << endl;
	struct tm t1, t2;
	t1.tm_year = year1 - 1900;
	t1.tm_mon = month1 - 1;
	t1.tm_mday = day1;
	t1.tm_hour = 0;
	t1.tm_isdst = 0;
	t1.tm_min = 0;
	t1.tm_yday = 0;
	t1.tm_sec = 0;

	t2.tm_year = year2 - 1900;
	t2.tm_mon = month2 -1;
	t2.tm_mday = day2;
	
	t2.tm_hour = 0;
	t2.tm_isdst = 0;
	t2.tm_min = 0;
	t2.tm_yday = 0;
	t2.tm_sec = 0;
	CardNode * q = pCardNodeHead;
	BillingNode *p = pBillNodeHead;
	cout << endl << "----------��������Ϣ----------" << endl;

	cout << "����\t״̬\t���ѽ��\t �ϻ�ʱ��\t\t\t�»�ʱ��" << endl;
	if (p == NULL)
	{
		cout << "������Ϣ�޼�¼" << endl;

      }
	while (p != NULL)
	{
		
		if (mktime(&t1) <= q->data.tLast    &&    q->data.tLast <= mktime(&t2))
		{
			char aStartTime[TIMELENGTH] = { 0 };
			char aEndTime[TIMELENGTH] = { 0 };
			timeToString((p->data).tStart, aStartTime);
			timeToString((p->data).tEnd, aEndTime);
			cout << p->data.aCardName << "\t";
			if ((q->data).nStatus == USING)
				cout << "�ϻ�\t";
			else if ((q->data).nStatus == UNUSE)
				cout << "δ�ϻ�\t";
			else if ((q->data).nStatus == INVALID)
				cout << "ע��\t";

			cout << p->data.fAmount << "\t\t";
			cout << aStartTime << "\t\t" << aEndTime << endl;

		}
		else
		{
			cout << "���Ѽ�¼������" << endl;
		}
		p = p->next;
		
	}
}
//����Ӫҵ��
void daymoney(BillingNode* pBillNodeHead)
{
	int year1, month1, day1;
	time_t t2;
	float money = 0;
	cout << "�������ѯ�����ڣ�" << endl;
	cin >> year1; cin >> month1; cin >> day1;

	struct tm t1;
	t1.tm_year = year1 - 1900;
	t1.tm_mon = month1 - 1;
	t1.tm_mday = day1;
	t1.tm_hour = 0;
	t1.tm_isdst = 0;
	t1.tm_min = 0;
	t1.tm_yday = 0;
	t1.tm_sec = 0;

	t2 = time(NULL);




	BillingNode *p = pBillNodeHead;
	if (p == NULL)
	{
		cout << "�����ѽ��Ϊ0" << endl;

	}
	while (p != NULL)
	{
		if (mktime(&t1) < p->data.tEnd    &&    p->data.tEnd <= t2)
		{

			money += p->data.fAmount;

		}

		p = p->next;
	}

	cout << "�����ѽ��Ϊ��" << money << endl;

}
//����Ӫҵ��
void yearmoney(BillingNode* pBillNodeHead)
{
	int year, month, day;
	float money = 0;



	cout << "��������Ҫ��ѯ�����" << endl;
	cin >> year;

	BillingNode *p = pBillNodeHead;
	if (p == NULL)
	{
		cout << "�����ѽ��Ϊ0" << endl;

	}
	while (p != NULL)
	{

		if (stringToTime(year, 1, 1) <= p->data.tEnd    &&  p->data.tEnd < stringToTime(year + 1, 1, 1))
		{

			money += p->data.fAmount;

		}

		p = p->next;
	}

	cout << "�����ѽ��Ϊ��" << money << endl;







}
//��ѯ�Ѿ�ע���Ŀ�
void logcard(CardNode *pCardNodeHead)
{
	cout << "�˺�" << "\t" << "״̬" << endl;
	int count = 0;
	CardNode * t = pCardNodeHead;
	if (t == NULL)
	{
		cout << "ע���Ŀ������ڼ�¼��" << endl;
	}
	while (t != NULL)
	{
		if (t->data.nStatus == INVALID)
		{
			cout << t->data.aName << "\t" << "��ע��" << endl;
			count = count + 1;
		}

		t = t->next;

	}
	cout << "ע����������Ϊ" << count << endl;
}