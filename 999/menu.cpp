#include"stdafx.h"
#include<string>
#include<iostream >
#include<iomanip>

using namespace std;

#include"global.h"
#include"model.h"
#include"tool.h"


void displayCard(CardNode* pCardNodeHead)
{
	if (pCardNodeHead == NULL)
	{ 
		cout << endl << endl << "һ���ϻ�����û�У�" << endl << endl;    
		return; 
	}                                                // /t�����ã�������ĸ�

	cout << "����\t״̬\t���\t�ۼ�ʹ��\tʹ�ô���\t�ϴ�ʹ��ʱ��" << endl;

	CardNode* pCur = pCardNodeHead; 
	while (pCur != NULL)
	{
		char aLastTime[TIMELENGTH] = { 0 };         
		timeToString((pCur->data).tLast, aLastTime);    
		cout << pCur->data.aName << "\t" ;    
		if(pCur->data.nStatus == USING)         
			cout << "�ϻ�\t";        
		else if(pCur->data.nStatus == UNUSE)   
			cout << "δ�ϻ�\t";    
		else if(pCur->data.nStatus == INVALID)        
			cout << "ע��\t";         else     
			cout << "����\t";   
		cout << pCur->data.balance << "\t";       
		cout << pCur->data.totaluse << "\t\t" <<  pCur->data.nusecount << "\t\t" <<  aLastTime << endl; 
		pCur = pCur->next;
	}


	 }
void displayBill(BillingNode* pBillNodeHead ,CardNode* pCardNodeHead)
{

	if (pBillNodeHead == NULL)
	{
		cout << endl << endl << "��Ƭ��������Ϣ�����ڣ�" << endl << endl;
		return;
	}                                                // /t�����ã�������ĸ�

	cout << "����\t״̬\t�������\t�ϻ�ʱ��\t\t\t�»�ʱ��" << endl;
	CardNode * q = pCardNodeHead;
	BillingNode * p = pBillNodeHead;
	while (p != NULL)
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
		p = p->next;
	}







}



void outputMenu()
{
	cout << "~~~~~~~��С�����ƷѲ˵�ϵͳ~~~~~~~~" << endl << endl;
	cout << "1.��ӿ�" << endl;
	cout << "2.��ѯ��" << endl;
	cout << "3.�ϻ�" << endl;
	cout << "4.�»�" << endl;
	cout << "5.��ֵ" << endl;
	cout << "6.�˷�" << endl;
	cout << "7.��ѯͳ��" << endl;
	cout << "8.ע����" << endl;
	cout << "9.ɾ��ע����" << endl;
	cout << "10.�ָ�ע����" << endl;
	cout << "0.�˳�" << endl << endl;
	cout << "��ѡ��˵����ţ�0~8��:";
}


bool inputCardNo(string& strNo)
{
	while (true)
	{
		cout << "�����뿨��1-17��";
		cin >> strNo;
		cin.clear();
		cin.sync();
		if (strNo.length() >= 17)
		{
			cout << "���ų��ȳ������ֵ" << endl;
			char ch = 'M';
			while (ch != 'N'&&ch != 'Y')
			{
				cout << "���������𣿣�y/n��";
				cin >> ch;
				ch = toupper(ch);
				cin.clear();
				cin.sync();
			}
			if (ch == 'N')
			{

				return false;
			}
		}
		else
		{

			break;
		}
	}
}
bool inputCardPwd(string& strPwd)
{
	while (true)
	{
		cout << "����������1-6��";
		cin >> strPwd;
		cin.clear();
		cin.sync();
		if (strPwd.length() >=6)
		{
			cout << "���볤�ȳ������ֵ" << endl;
			char ch = 'M';
			while (ch != 'N'&&ch != 'Y')
			{
				cout << "���������𣿣�y/n��";
				cin >> ch;
				ch = toupper(ch);
				cin.clear();
				cin.sync();
			}
			if (ch == 'N')
			{

				return false;
			}
		}
		else
		{

			break;
		}
	}





}
bool inputCardBalance(float& fbalance)
{
	while (true)
	{
		cout << "��������(�޶�Ϊ1000����)��";
		cin >> fbalance;
		cout <<"������ӵĽ��Ϊ��"<< setiosflags(ios::fixed) << setprecision(2) << fbalance << endl;
		cin.clear();
		cin.sync();
		if (fbalance >= 10000) //����޶��Ƿ���ȷ   
		{
			cout << "����޶ȶȳ������ֵ��" << endl;
			char ch = 'M';
			while (ch != 'N' && ch != 'Y')
			{
				cout << "���������𣿣�y/n��";
				cin >> ch;
				ch = toupper(ch);
				cin.clear();
				cin.sync();
			}
			if (ch == 'N')


			{
				return false;
			}
		}

		else { break; }
		
	}
}
bool inputNoPwd(string& strNo, string& strPwd)
	{

		if (!inputCardNo(strNo))
		{
			return false;
		}
		if (!inputCardPwd(strPwd))
		{
			return false;
		}
		return true;
	}
bool inputNoPwdBalance(string& strNo, string& strPwd, float& balance)
{
	if (!inputCardNo(strNo))//���������Ǵ���Ĳ�Ҫִ��
	{
		return false;
	}
	if (!inputCardPwd(strPwd))
	{
		return false;
	}
	if (!inputCardBalance(balance))
	{
		return false;
	}	
		return true;

}