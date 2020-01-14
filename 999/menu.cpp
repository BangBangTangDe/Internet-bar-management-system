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
		cout << endl << endl << "一张上机卡都没有！" << endl << endl;    
		return; 
	}                                                // /t的作用，向后跳四格

	cout << "卡号\t状态\t余额\t累计使用\t使用次数\t上次使用时间" << endl;

	CardNode* pCur = pCardNodeHead; 
	while (pCur != NULL)
	{
		char aLastTime[TIMELENGTH] = { 0 };         
		timeToString((pCur->data).tLast, aLastTime);    
		cout << pCur->data.aName << "\t" ;    
		if(pCur->data.nStatus == USING)         
			cout << "上机\t";        
		else if(pCur->data.nStatus == UNUSE)   
			cout << "未上机\t";    
		else if(pCur->data.nStatus == INVALID)        
			cout << "注销\t";         else     
			cout << "错误\t";   
		cout << pCur->data.balance << "\t";       
		cout << pCur->data.totaluse << "\t\t" <<  pCur->data.nusecount << "\t\t" <<  aLastTime << endl; 
		pCur = pCur->next;
	}


	 }
void displayBill(BillingNode* pBillNodeHead ,CardNode* pCardNodeHead)
{

	if (pBillNodeHead == NULL)
	{
		cout << endl << endl << "卡片的消费信息不存在！" << endl << endl;
		return;
	}                                                // /t的作用，向后跳四格

	cout << "卡号\t状态\t消费余额\t上机时间\t\t\t下机时间" << endl;
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
			cout << "上机\t";
		else if ((q->data).nStatus == UNUSE)
			cout << "未上机\t";
		else if ((q->data).nStatus == INVALID)
			cout << "注销\t";        
			
		cout << p->data.fAmount << "\t\t";
		cout << aStartTime << "\t\t" << aEndTime << endl;
		p = p->next;
	}







}



void outputMenu()
{
	cout << "~~~~~~~（小宝）计费菜单系统~~~~~~~~" << endl << endl;
	cout << "1.添加卡" << endl;
	cout << "2.查询卡" << endl;
	cout << "3.上机" << endl;
	cout << "4.下机" << endl;
	cout << "5.充值" << endl;
	cout << "6.退费" << endl;
	cout << "7.查询统计" << endl;
	cout << "8.注销卡" << endl;
	cout << "9.删除注销卡" << endl;
	cout << "10.恢复注销卡" << endl;
	cout << "0.退出" << endl << endl;
	cout << "请选择菜单项编号（0~8）:";
}


bool inputCardNo(string& strNo)
{
	while (true)
	{
		cout << "请输入卡号1-17：";
		cin >> strNo;
		cin.clear();
		cin.sync();
		if (strNo.length() >= 17)
		{
			cout << "卡号长度超过最大值" << endl;
			char ch = 'M';
			while (ch != 'N'&&ch != 'Y')
			{
				cout << "重新输入吗？（y/n）";
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
		cout << "请输入密码1-6：";
		cin >> strPwd;
		cin.clear();
		cin.sync();
		if (strPwd.length() >=6)
		{
			cout << "密码长度超过最大值" << endl;
			char ch = 'M';
			while (ch != 'N'&&ch != 'Y')
			{
				cout << "重新输入吗？（y/n）";
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
		cout << "请输入金额(限度为1000以内)：";
		cin >> fbalance;
		cout <<"您所添加的金额为："<< setiosflags(ios::fixed) << setprecision(2) << fbalance << endl;
		cin.clear();
		cin.sync();
		if (fbalance >= 10000) //金额限度是否正确   
		{
			cout << "金额限度度超过最大值！" << endl;
			char ch = 'M';
			while (ch != 'N' && ch != 'Y')
			{
				cout << "重新输入吗？（y/n）";
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
	if (!inputCardNo(strNo))//如果输入的是错误的才要执行
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