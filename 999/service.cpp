
//菜单功能实现的函数
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
			cout << endl << endl << "卡【" << strNo << "】已经存在，添加新 卡失败！" << endl << endl;
			break;
		}
		case SUCCESS:
		{
			cout << endl << endl << "添加新卡成功!" << endl << endl;
			cout << "新卡卡号：" << strNo << endl;
			cout << "新卡金额：" << fBalance << endl;
			break;
		}
		default:
		{
			cout << endl << endl << "系统错误!" << endl << endl;
			break;
		}
		}
	}
	else
	{ 
		cout << endl << endl << "输入的【卡号，密码，金额】等信息格式不符号要求， 添加卡失败!" << endl << endl;
	}

}
void queryCard(CardNode* const pCardNodeHead)
{


	while (1)
	{
		int b;
		cout << "请选择查询的具体功能：" << endl;
		cout << "1 .查询单张卡    \t  2 .查询所有的卡   \t 0 . 退出" << endl;
		cout << "请输入序号：";
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
				cout << endl << "----------卡信息----------" << endl;

				if (single == NULL)
				{
					cout << endl << endl << "上机卡没有！" << endl << endl;
					return;
				}                                                // /t的作用，向后跳四格

				cout << "卡号\t状态\t余额\t累计使用\t使用次数\t上次使用时间" << endl;



				if (single != NULL)
				{
					char aLastTime[TIMELENGTH] = { 0 };
					timeToString(single->tLast, aLastTime);
					cout << single->aName << "\t";
					if (single->nStatus == USING)
						cout << "上机\t";
					else if (single->nStatus == UNUSE)
						cout << "未上机\t";
					else if (single->nStatus == INVALID)
						cout << "注销\t";         else
						cout << "错误\t";
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
	CardNode *pCardNode;  //删除节点防止占用电脑内存 链表是动态生成的

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
	LogonInfo* pInfo = new LogonInfo;   // 上机信息，用于显示    
	char aTime[TIMELENGTH] = { 0 };       // 上机时间，用于显示 




	if (inputNoPwd(strNo, strPwd))
	{
		cout << endl << "----------上机信息----------" << endl;

		// 根据上机结果，提示不同信息      
		int nResult = logon(strNo, strPwd, pInfo, pCardNodeHead, ppBillingNodeHead);
		switch (nResult)
		{
		case NOFINDCARD:
		case NOMATCH:
		{                 cout << endl << endl << "卡不存在，或卡信息不对，不能上机！" << endl << endl;                 break;             }

		case SUCCESS:

		{
			timeToString(pInfo->tLogon, aTime);//时间转换函数

			cout << endl;
			cout << "卡号: \t" << strNo << endl;
			cout << "余额: \t" << pInfo->fbalance << endl;
			cout << "上机时间：\t" << aTime << endl;
			break;
		}
		case USING: {                 cout << endl << "该卡正在使用，不能重复上机！" << endl;                 break;             }
		case INVALID: {                 cout << endl << "该卡已注销，不能上机！" << endl;                 break;             }
		case ENOUGHMONEY: {                 cout << endl << "卡余额为0，不能上机！" << endl;                 break;             }
		default: {                 break;             }





		}
	}
	else {
		cout << endl << endl << "卡号或者密码格式不正确，上机失败!" << endl << endl;
	}

	delete pInfo;
}
void xiaJi(CardNode* pCardNodeHead, BillingNode* pBillingNodeHead) 
{
	string strNo;

	string strPwd;


	SettleInfo* pInfo = new SettleInfo; // 下机信息   
	char aStartTime[TIMELENGTH] = { 0 };  // 上机时间 
	char aEndTime[TIMELENGTH] = { 0 };    // 下机时间 

	if (inputNoPwd(strNo, strPwd))
	{
		cout << "----------下机信息如下----------\n";

		int nResult = settle(strNo, strPwd, pInfo, pCardNodeHead, pBillingNodeHead);
		switch (nResult)


		{

		case NOFINDCARD:
		case NOMATCH:
		{                 cout << endl << endl << "卡不存在，或卡信息不对，下机失败！" << endl << endl;                 break;

		}


		case SUCCESS:
		{                 timeToString(pInfo->tStart, aStartTime);
		timeToString(pInfo->tEnd, aEndTime);

		cout << endl;
		cout << "卡号: \t" << strNo << endl;
		cout << "消费: \t" << pInfo->fAmount << endl;
		cout << "余额: \t" << pInfo->fbalance << endl;
		cout << "上机时间：\t" << aStartTime << endl;
		cout << "下机时间：\t" << aEndTime << endl;

		break;
		}
		case UNUSE:
		{                 cout << endl << endl << "该卡没有上机！" << endl << endl;                 break;
		}
		case ENOUGHMONEY:
		{                 cout << endl << endl << "卡余额不足，请先充值再下机！" << endl << endl;

		break;
		}
		default:
		{
			break;
		}
		}
	}
	else { cout << endl << endl << "卡号或者密码格式不正确，下机失败!" << endl << endl; }

	delete pInfo;

}
void chongzhi(CardNode* pCardNodeHead)
{
	string strNo;
	string strPwd;
	addmoney *p = new addmoney;
	

		if (inputNoPwd(strNo, strPwd))
		{
			cout << endl << "----------充值信息----------" << endl;

		   
			int nResult = judgeaddmoney(strNo, strPwd, p, pCardNodeHead);
			switch (nResult)
			{
			case NOFINDCARD:
			case NOMATCH:
			{                 cout << endl << endl << "卡不存在，或卡信息不对，不能充值！" << endl << endl;                 break;             }

			case SUCCESS:

			{
			

				cout << endl;
				cout << "卡号: \t" << strNo << endl;
				cout << "充值之后的余额：\t" << p->afterbalance << endl;
				break;
			}
			
			case INVALID: {                 cout << endl << "该卡已注销，不能充值！" << endl;                 break;             }
		
			default: {                 break;             }





			}
		}
		else {
			cout << endl << endl << "卡号或者密码格式不正确，充值失败!" << endl << endl;
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
		cout << endl << "---------退费信息----------" << endl;
   
		int nResult = judgebackmoney(strNo, strPwd, pBack, pCardNodeHead);
		switch (nResult)
		{
		case NOFINDCARD:
		case NOMATCH:
		{                 cout << endl << endl << "卡不存在，或卡信息不对，不能正常退费！" << endl << endl;                 break;             }

		case SUCCESS:

		{
			

			cout << endl;
			cout << "卡号: \t" << strNo << endl;
			cout << "退费之后的余额: \t" << pBack->afterbackmoney << endl;
			
			break;
		}
		case USING: {                 cout << endl << "该卡正在使用，不能退费，请您在下机之后进行退费操作！" << endl;                 break;             }
		case INVALID: {                 cout << endl << "该卡已注销，不能上机！" << endl;                 break;             }
		case ENOUGHMONEY: {                 cout << endl << "卡里余额为0" << endl;                 break;             }
		default: {                 break;             }





		}
	}
	else {
		cout << endl << endl << "卡号或者密码格式不正确，退费操作失败!" << endl << endl;
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
		cout << endl << "---------注销卡信息----------" << endl;

		int nResult = judgelogout(strNo, strPwd, pout, pCardNodeHead);
		switch (nResult)
		{
		case NOFINDCARD:
		case NOMATCH:
		{                 cout << endl << endl << "卡不存在，或卡信息不对，不能正常注销！" << endl << endl;                 break;             }

		case SUCCESS:

		{


			cout << endl;
			cout << "卡号: \t" << strNo << endl;
			cout << "注销之后的余额: \t" << pout->money << endl;

			break;
		}
		case USING: {                 cout << endl << "该卡正在使用，不能注销，请您在下机之后进行注销操作！" << endl;                 break;             }
		case INVALID: {                 cout << endl << "该卡已注销！" << endl;                 break;             }
		case ENOUGHMONEY: {                 cout << endl << "卡里余额为0" << endl;                 break;             }
		default: {                 break;             }





		}
	}
	else {
		cout << endl << endl << "注销失败!" << endl << endl;
	}

	delete pout;


}
void statistics(BillingNode* pBillingNodeHead , CardNode* pCardNodeHead)
{
	
	    int  a=-1 ;
		do
		{
			cout << "-----------查询统计-----------" << endl;
			cout << "1.单卡消费记录查询" << endl;
			cout << "2.所有卡的消费记录查询" << endl;
			cout << "3.分时间段消费记录查询" << endl;
			cout << "4.统计总营业额" << endl;
			cout << "5.统计日营业额(今天0点——>截至到查询的时间)" << endl;
			cout << "6.统计年营业额" << endl;
			cout << "7.注销卡的统计" << endl;
			cout << "0.退出" << endl;
			cout << "请选择菜单项编号(0~7)";
			cin >> a;
			switch (a)
			{
			case 1:
			{
				querryonebill(pBillingNodeHead,pCardNodeHead);     //消费记录查询，根据卡号，查询该卡的消费记录
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
				cout << "统计查询结束，返回主菜单，祝您生活愉快" << endl;
				break;
			}
			default:cout << "输入的序号错误" << endl;
			}
		} while (a != 0);
}
void logcaarddelete(CardNode **pCardNodeHead)
{
	string strNo;
	CardNode * t = *pCardNodeHead;
	CardNode * p,*q;
	int nCardIndex=0;

	cout << "输入删除卡的账号" << endl;
	cin >> strNo;
	Card* pCard = NULL;
	pCard= CardIsExist(strNo, nCardIndex, t);

	if (pCard == NULL)
	{
		cout << "删除卡的信息不存在" << endl;
	}
	else if (pCard->nStatus != INVALID)
	{
		cout << "此卡还没有被注销，不能永久删除" << endl;
	}
	else
	{
		if (t->data.aName == pCard->aName)
		{
			*pCardNodeHead = t->next;
			cout << "注销的卡已经从系统文件中删除，请管理员进行查询核对" << endl;
			delete t;
		}
		else
		{
			while (t->next->data.aName != pCard->aName) { t = t->next; }
			p = t->next;
			t->next = p->next;
			delete p;

			cout << "注销的卡已经从系统文件中删除，请管理员进行查询核对" << endl;
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
	cout << "输入需要恢复的注销卡账号" << endl;
	cin >> number;
	int count = 0;
	Card *pCard = CardIsExist(number, count, *t);
	if (pCard)
	{
		if (pCard->nStatus == INVALID)
		{
			CardNode *s = *t;
			pCard->nStatus = INVALID;
			// 更新文件中的卡信息  
			while (s->data.aName != pCard->aName)
			{
				s = s->next;
			}

			s->data.nStatus =UNUSE;
			updateCard(pCard, CARDPATH, count);
			cout << "恢复成功" << endl;

		}
		else
		{
			cout << "此卡不是注销卡，不需要恢复" << endl;
		}
	}
	else
	{
		cout << "要恢复的卡不存在" << endl;

	}
	





}

