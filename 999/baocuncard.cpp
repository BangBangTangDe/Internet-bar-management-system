#include"stdafx.h"
#include<iostream>
#include<fstream>


using namespace std;


#include "model.h"


//保存添加卡文档
bool saveCard(const Card* pCard, const char* pPath)
{
	ofstream ofile(pPath, ios::binary | ios::app);

	ofile.write((char*)pCard, sizeof(Card));  
	ofile.close();

	return true;
}
//初始化卡片
CardNode* CardListInit(const string cardFilename)
{

	ifstream cardfile(cardFilename);
	CardNode *pCardNode, *pCardNodeHead, *pCardNodeTail;
	Card card;
	int flag = 0;
	if (!cardfile.is_open())
	{
		return NULL;
	}

	pCardNodeHead = NULL;
	pCardNodeTail = NULL;
	

	while (1)
	{
		cardfile.read((char*)&card, sizeof(Card));

		if (cardfile.eof())
		{
			break;
		}
		pCardNode = new CardNode;
		pCardNode->data = card;

		if (pCardNodeHead == NULL)
		{
			pCardNodeHead = pCardNode;
			pCardNodeTail = pCardNode;
		}
		else
		{
			pCardNodeTail->next = pCardNode;
			pCardNodeTail = pCardNode;
		}
		flag = 1;
	}

	if(flag==1)
	pCardNodeTail->next = NULL;

	cardfile.close();

	return pCardNodeHead;
}

//保存上机卡文档
BillingNode* BillListInit(const string billingFilename)
{
	ifstream billingfile(billingFilename);
	int flag = 0;
	BillingNode *pBillingNode, *pBillingNodeHead, *pBillingNodeTail;     Billing billing;


	if (!billingfile.is_open())
	{
		return NULL;
	}

	pBillingNodeHead = NULL;
	pBillingNodeTail = NULL;

	while (1)
	{
		billingfile.read((char*)&billing, sizeof(Billing));
		if (billingfile.eof())
		{
			break;
		}

		pBillingNode = new BillingNode;
		pBillingNode->data = billing;


		if (pBillingNodeHead == NULL)
		{
			pBillingNodeHead = pBillingNode;

			pBillingNodeTail = pBillingNode;

		}
		else

		{
			pBillingNodeTail->next = pBillingNode;
			pBillingNodeTail = pBillingNode;
		}
		flag = 1;
	}
	if(flag)
	pBillingNodeTail->next = NULL;

	billingfile.close();

	return pBillingNodeHead;
}
bool saveBilling(const Billing* pBilling, const char* pPath)
{
	ofstream ofile(pPath, ios::binary | ios::app);

	ofile.write((char*)pBilling, sizeof(Billing));
	ofile.close();

	return true;
}


//保存充值文档