// AMS.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include <limits>
#include "menu.h"


using namespace std;

#include"service.h"
#include"global.h"
#include"baocuncard.h"
#include "model.h"


int main()
{
	int  selection;
	CardNode *pCardNodeHead = NULL;
	pCardNodeHead = CardListInit(CARDPATH);
	BillingNode  *pBillingNodeHead = NULL;
	pBillingNodeHead = BillListInit(BILLINGPATH);

	cout << endl;
	cout << "欢迎进入计费系统" << endl;
	cout << endl;

	do
	{
		outputMenu();

		selection = numeric_limits<int>::max();
		cin >> selection;
		cin.clear();
		cin.ignore();
		
		
		switch (selection)
		{
		case 1:
		{
			cout << endl << "-------开卡----------" << endl << endl;
			addCard(&pCardNodeHead);//一个链表的地址
			break;
		}
		case 2:
		{
			cout << endl << "------ - 查询卡--------" << endl << endl;
			queryCard(pCardNodeHead);
			break;

		}
		case 3:
		{
			shangJi(pCardNodeHead, &pBillingNodeHead);
			break;
		}
		case 4:
		{
			cout << endl << "------ - 下机----------" << endl << endl;
			xiaJi(pCardNodeHead, pBillingNodeHead);
			break;
		}

		case 5:
		{
			cout << endl << "-------充值----------" << endl << endl;
			chongzhi(pCardNodeHead);
			break;

		}
		case 6:
		{
			cout << endl << "-------退费----------" << endl << endl;
			backbalance(pCardNodeHead);
			break;
		}
		case 7:
		{
			cout << endl << "------查询系统---------" << endl << endl;
             statistics(pBillingNodeHead,pCardNodeHead);
			 break;
		}
		case 8:
		{
			cout << endl << "------注销卡---------" << endl << endl;
			logout(pCardNodeHead);
			break;
		}
		
		case 9:
		{
			cout << endl << "------卡片删除---------" << endl << endl;
			logcaarddelete(&pCardNodeHead);
			break;
		}
		case 10:
		{
			cout << endl << "------恢复注销卡---------" << endl << endl;
			recovery(&pCardNodeHead);
			break;
		}
		case 0:
		{
			clearData ( pCardNodeHead);
			cout << endl << "谢谢你使用本系统！" << endl << endl;
			break;
		}
		default:
		{
			cout << "您所输入的菜单编号不存在！/n";
			break;

		}
		}
		cout << endl;
	} while (selection != 0);


	return 0;
}

