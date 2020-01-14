
//�˵�����ʵ�ֵĺ���
#include"stdafx.h"
#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>


#include "menu.h" 
#include "global.h"
#include"model.h"
#include"cardlist.h"
#include"billist.h"
#include"baocuncard.h"
#include"tool.h"

void addCard(CardNode** ppCardNodeHead)
{
	string strNo;  
	string strPwd;   
	float fBalance;

	if (inputNoPwdBalance(strNo, strPwd, fBalance))
	{
		int nResult = addnewcard(strNo, strPwd, fBalance, ppCardNodeHead);
		switch (nResult)
		{
		case FINDCARD:
		{
			cout << endl << endl << "����" << strNo << "���Ѿ����ڣ������ ��ʧ�ܣ�" << endl << endl;
			break;
		}
		case SUCCESS:
		{
			cout << endl << endl << "����¿��ɹ�!" << endl << endl;
			cout << "�¿����ţ�" << strNo << endl;
			cout << "�¿���" << fBalance << endl;
			break;
		}
		default:
		{
			cout << endl << endl << "ϵͳ����!" << endl << endl;
			break;
		}
		}
	}
	else
	{ 
		cout << endl << endl << "����ġ����ţ����룬������Ϣ��ʽ������Ҫ�� ��ӿ�ʧ��!" << endl << endl;
	}

}
void queryCard(CardNode* const pCardNodeHead)
{


	while (1)
	{
		int b;
		cout << "��ѡ���ѯ�ľ��幦�ܣ�" << endl;
		cout << "1 .��ѯ���ſ�    \t  2 .��ѯ���еĿ�   \t 0 . �˳�" << endl;
		cout << "��������ţ�";
		cin >> b;
		if (b == 1)
		{

			string strNo;
			Card *single;
			if (inputCardNo(strNo))
			{
				CardNode *pCardNode = pCardNodeHead;
				int nCardIndex;
				nCardIndex = 0;
				single = CardIsExist(strNo, nCardIndex, pCardNodeHead);
				cout << endl << "----------����Ϣ----------" << endl;

				if (single == NULL)
				{
					cout << endl << endl << "�ϻ���û�У�" << endl << endl;
					return;
				}                                                // /t�����ã�������ĸ�

				cout << "����\t״̬\t���\t�ۼ�ʹ��\tʹ�ô���\t�ϴ�ʹ��ʱ��" << endl;



				if (single != NULL)
				{
					char aLastTime[TIMELENGTH] = { 0 };
					timeToString(single->tLast, aLastTime);
					cout << single->aName << "\t";
					if (single->nStatus == USING)
						cout << "�ϻ�\t";
					else if (single->nStatus == UNUSE)
						cout << "δ�ϻ�\t";
					else if (single->nStatus == INVALID)
						cout << "ע��\t";         else
						cout << "����\t";
					cout << single->balance << "\t";
					cout << single->totaluse << "\t\t" << single->nusecount << "\t\t" << aLastTime << endl;


				}
			}

			
		}
		else if (b == 2)
		{
			displayCard(pCardNodeHead);

		}
		else if (b == 0)
		{
			break;
		}


	}
}
void clearData(CardNode* pCardNodeHead)
{
	CardNode *pCardNode;  //ɾ���ڵ��ֹռ�õ����ڴ� �����Ƕ�̬���ɵ�

	while (pCardNodeHead != NULL)
	{
		pCardNode = pCardNodeHead;

		pCardNodeHead = pCardNodeHead->next;
		delete pCardNode;

	}
}
void shangJi(CardNode* pCardNodeHead, BillingNode **ppBillingNodeHead)
{
	string strNo;
	string strPwd;
	LogonInfo* pInfo = new LogonInfo;   // �ϻ���Ϣ��������ʾ    
	char aTime[TIMELENGTH] = { 0 };       // �ϻ�ʱ�䣬������ʾ 




	if (inputNoPwd(strNo, strPwd))
	{
		cout << endl << "----------�ϻ���Ϣ----------" << endl;

		// �����ϻ��������ʾ��ͬ��Ϣ      
		int nResult = logon(strNo, strPwd, pInfo, pCardNodeHead, ppBillingNodeHead);
		switch (nResult)
		{
		case NOFINDCARD:
		case NOMATCH:
		{                 cout << endl << endl << "�������ڣ�����Ϣ���ԣ������ϻ���" << endl << endl;                 break;             }

		case SUCCESS:

		{
			timeToString(pInfo->tLogon, aTime);//ʱ��ת������

			cout << endl;
			cout << "����: \t" << strNo << endl;
			cout << "���: \t" << pInfo->fbalance << endl;
			cout << "�ϻ�ʱ�䣺\t" << aTime << endl;
			break;
		}
		case USING: {                 cout << endl << "�ÿ�����ʹ�ã������ظ��ϻ���" << endl;                 break;             }
		case INVALID: {                 cout << endl << "�ÿ���ע���������ϻ���" << endl;                 break;             }
		case ENOUGHMONEY: {                 cout << endl << "�����Ϊ0�������ϻ���" << endl;                 break;             }
		default: {                 break;             }





		}
	}
	else {
		cout << endl << endl << "���Ż��������ʽ����ȷ���ϻ�ʧ��!" << endl << endl;
	}

	delete pInfo;
}
void xiaJi(CardNode* pCardNodeHead, BillingNode* pBillingNodeHead) 
{
	string strNo;

	string strPwd;


	SettleInfo* pInfo = new SettleInfo; // �»���Ϣ   
	char aStartTime[TIMELENGTH] = { 0 };  // �ϻ�ʱ�� 
	char aEndTime[TIMELENGTH] = { 0 };    // �»�ʱ�� 

	if (inputNoPwd(strNo, strPwd))
	{
		cout << "----------�»���Ϣ����----------\n";

		int nResult = settle(strNo, strPwd, pInfo, pCardNodeHead, pBillingNodeHead);
		switch (nResult)


		{

		case NOFINDCARD:
		case NOMATCH:
		{                 cout << endl << endl << "�������ڣ�����Ϣ���ԣ��»�ʧ�ܣ�" << endl << endl;                 break;

		}


		case SUCCESS:
		{                 timeToString(pInfo->tStart, aStartTime);
		timeToString(pInfo->tEnd, aEndTime);

		cout << endl;
		cout << "����: \t" << strNo << endl;
		cout << "����: \t" << pInfo->fAmount << endl;
		cout << "���: \t" << pInfo->fbalance << endl;
		cout << "�ϻ�ʱ�䣺\t" << aStartTime << endl;
		cout << "�»�ʱ�䣺\t" << aEndTime << endl;

		break;
		}
		case UNUSE:
		{                 cout << endl << endl << "�ÿ�û���ϻ���" << endl << endl;                 break;
		}
		case ENOUGHMONEY:
		{                 cout << endl << endl << "�����㣬���ȳ�ֵ���»���" << endl << endl;

		break;
		}
		default:
		{
			break;
		}
		}
	}
	else { cout << endl << endl << "���Ż��������ʽ����ȷ���»�ʧ��!" << endl << endl; }

	delete pInfo;

}
void chongzhi(CardNode* pCardNodeHead)
{
	string strNo;
	string strPwd;
	addmoney *p = new addmoney;
	

		if (inputNoPwd(strNo, strPwd))
		{
			cout << endl << "----------��ֵ��Ϣ----------" << endl;

		   
			int nResult = judgeaddmoney(strNo, strPwd, p, pCardNodeHead);
			switch (nResult)
			{
			case NOFINDCARD:
			case NOMATCH:
			{                 cout << endl << endl << "�������ڣ�����Ϣ���ԣ����ܳ�ֵ��" << endl << endl;                 break;             }

			case SUCCESS:

			{
			

				cout << endl;
				cout << "����: \t" << strNo << endl;
				cout << "��ֵ֮�����\t" << p->afterbalance << endl;
				break;
			}
			
			case INVALID: {                 cout << endl << "�ÿ���ע�������ܳ�ֵ��" << endl;                 break;             }
		
			default: {                 break;             }





			}
		}
		else {
			cout << endl << endl << "���Ż��������ʽ����ȷ����ֵʧ��!" << endl << endl;
		}

		delete p;



}
void backbalance(CardNode* pCardNodeHead)
{
	string strNo;
	string strPwd;
	backmoney* pBack = new backmoney;




	if (inputNoPwd(strNo, strPwd))
	{
		cout << endl << "---------�˷���Ϣ----------" << endl;
   
		int nResult = judgebackmoney(strNo, strPwd, pBack, pCardNodeHead);
		switch (nResult)
		{
		case NOFINDCARD:
		case NOMATCH:
		{                 cout << endl << endl << "�������ڣ�����Ϣ���ԣ����������˷ѣ�" << endl << endl;                 break;             }

		case SUCCESS:

		{
			

			cout << endl;
			cout << "����: \t" << strNo << endl;
			cout << "�˷�֮������: \t" << pBack->afterbackmoney << endl;
			
			break;
		}
		case USING: {                 cout << endl << "�ÿ�����ʹ�ã������˷ѣ��������»�֮������˷Ѳ�����" << endl;                 break;             }
		case INVALID: {                 cout << endl << "�ÿ���ע���������ϻ���" << endl;                 break;             }
		case ENOUGHMONEY: {                 cout << endl << "�������Ϊ0" << endl;                 break;             }
		default: {                 break;             }





		}
	}
	else {
		cout << endl << endl << "���Ż��������ʽ����ȷ���˷Ѳ���ʧ��!" << endl << endl;
	}

	delete pBack;
	
}
void logout(CardNode* pCardNodeHead)
{
	string strNo;
	string strPwd;
	logoutcard* pout = new logoutcard;




	if (inputNoPwd(strNo, strPwd))
	{
		cout << endl << "---------ע������Ϣ----------" << endl;

		int nResult = judgelogout(strNo, strPwd, pout, pCardNodeHead);
		switch (nResult)
		{
		case NOFINDCARD:
		case NOMATCH:
		{                 cout << endl << endl << "�������ڣ�����Ϣ���ԣ���������ע����" << endl << endl;                 break;             }

		case SUCCESS:

		{


			cout << endl;
			cout << "����: \t" << strNo << endl;
			cout << "ע��֮������: \t" << pout->money << endl;

			break;
		}
		case USING: {                 cout << endl << "�ÿ�����ʹ�ã�����ע�����������»�֮�����ע��������" << endl;                 break;             }
		case INVALID: {                 cout << endl << "�ÿ���ע����" << endl;                 break;             }
		case ENOUGHMONEY: {                 cout << endl << "�������Ϊ0" << endl;                 break;             }
		default: {                 break;             }





		}
	}
	else {
		cout << endl << endl << "ע��ʧ��!" << endl << endl;
	}

	delete pout;


}
void statistics(BillingNode* pBillingNodeHead , CardNode* pCardNodeHead)
{
	
	    int  a=-1 ;
		do
		{
			cout << "-----------��ѯͳ��-----------" << endl;
			cout << "1.�������Ѽ�¼��ѯ" << endl;
			cout << "2.���п������Ѽ�¼��ѯ" << endl;
			cout << "3.��ʱ������Ѽ�¼��ѯ" << endl;
			cout << "4.ͳ����Ӫҵ��" << endl;
			cout << "5.ͳ����Ӫҵ��(����0�㡪��>��������ѯ��ʱ��)" << endl;
			cout << "6.ͳ����Ӫҵ��" << endl;
			cout << "7.ע������ͳ��" << endl;
			cout << "0.�˳�" << endl;
			cout << "��ѡ��˵�����(0~7)";
			cin >> a;
			switch (a)
			{
			case 1:
			{
				querryonebill(pBillingNodeHead,pCardNodeHead);     //���Ѽ�¼��ѯ�����ݿ��ţ���ѯ�ÿ������Ѽ�¼
				break;
			}

			case 2:
			{
				displayBill(pBillingNodeHead, pCardNodeHead);
				break;
			}

			case 3:
			{
				recordtimebill(pBillingNodeHead, pCardNodeHead);
				break;

			}
			case 4:
			{
				totalMoney(pBillingNodeHead);
				break;
			}
			case 5:
			{
				daymoney(pBillingNodeHead);
				break;
			}
			case 6:
			{
				yearmoney(pBillingNodeHead);
				break;
			}
			
			case 7:
			{
				logcard(pCardNodeHead);
				break;
			}
			case 0:
			{
				cout << "ͳ�Ʋ�ѯ�������������˵���ף���������" << endl;
				break;
			}
			default:cout << "�������Ŵ���" << endl;
			}
		} while (a != 0);
}
void logcaarddelete(CardNode **pCardNodeHead)
{
	string strNo;
	CardNode * t = *pCardNodeHead;
	CardNode * p,*q;
	int nCardIndex=0;

	cout << "����ɾ�������˺�" << endl;
	cin >> strNo;
	Card* pCard = NULL;
	pCard= CardIsExist(strNo, nCardIndex, t);

	if (pCard == NULL)
	{
		cout << "ɾ��������Ϣ������" << endl;
	}
	else if (pCard->nStatus != INVALID)
	{
		cout << "�˿���û�б�ע������������ɾ��" << endl;
	}
	else
	{
		if (t->data.aName == pCard->aName)
		{
			*pCardNodeHead = t->next;
			cout << "ע���Ŀ��Ѿ���ϵͳ�ļ���ɾ���������Ա���в�ѯ�˶�" << endl;
			delete t;
		}
		else
		{
			while (t->next->data.aName != pCard->aName) { t = t->next; }
			p = t->next;
			t->next = p->next;
			delete p;

			cout << "ע���Ŀ��Ѿ���ϵͳ�ļ���ɾ���������Ա���в�ѯ�˶�" << endl;
		}
		fstream file("card.txt", ios::out);
		file.close();
		fstream my("card.txt",ios::binary|ios::app);
		q = *pCardNodeHead;
		while (q)
		{
			my.write((char*)&q->data, sizeof(Card));
			q = q->next;
		}
		my.close();


	}

	


}
void recovery(CardNode **t) {
	string number;
	cout << "������Ҫ�ָ���ע�����˺�" << endl;
	cin >> number;
	int count = 0;
	Card *pCard = CardIsExist(number, count, *t);
	if (pCard)
	{
		if (pCard->nStatus == INVALID)
		{
			CardNode *s = *t;
			pCard->nStatus = INVALID;
			// �����ļ��еĿ���Ϣ  
			while (s->data.aName != pCard->aName)
			{
				s = s->next;
			}

			s->data.nStatus =UNUSE;
			updateCard(pCard, CARDPATH, count);
			cout << "�ָ��ɹ�" << endl;

		}
		else
		{
			cout << "�˿�����ע����������Ҫ�ָ�" << endl;
		}
	}
	else
	{
		cout << "Ҫ�ָ��Ŀ�������" << endl;

	}
	





}

