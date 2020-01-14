#include"stdafx.h"
#include<iostream>
#include<fstream>




using namespace std;




#include"model.h"
#include"baocuncard.h"
#include"global.h"



Card* CardIsExist(string strNo, int &nCardIndex, CardNode* const pCardNodeHead)
{
	//对卡的遍历
	CardNode *pCardNode = pCardNodeHead;
	nCardIndex = 0;  
	while (pCardNode != NULL)
	{
		//比较字符串的函数
		if (strcmp(pCardNode->data.aName, strNo.c_str()) == 0) //c_str函数的作用是找strno的首地址
		{
			return &(pCardNode->data);
		}      
		pCardNode = pCardNode->next;       
		nCardIndex++;
	}

	return NULL;
}
int addnewcard(string strNo, string strPwd, float balance, CardNode**ppCardNodeHead)
{

	int ncardindex = 0;//记录卡的数量
	if (CardIsExist(strNo, ncardindex, *ppCardNodeHead))
		return FINDCARD;
	Card card;//card类型的结构体变量
	strcpy_s(card.aName, strNo.c_str());
	strcpy_s(card.aPwd, strPwd.c_str());
	card.balance = balance;
	card.totaluse = 0;//累计使用金额
	card.nStatus = UNUSE; //显示状态正在使用
	card.nusecount = 0;
	card.tstart = card.tLast = time(NULL);
	CardNode* pCardNode = new CardNode; 
	pCardNode->data = card;

	pCardNode->next = NULL;

	CardNode *pCardNodeHead = *ppCardNodeHead;

	if (pCardNodeHead == NULL)
	{       
		*ppCardNodeHead = pCardNode; 

	}  else    
	{     
		CardNode *pCardNodeTail = *ppCardNodeHead;

		while(pCardNodeTail->next != NULL)           
			pCardNodeTail = pCardNodeTail->next;
		pCardNodeTail->next = pCardNode;
		pCardNode->next = NULL;
		   
	} 
	saveCard(&card, "card.txt");
	return SUCCESS;

}
bool updateCard(const Card* pCard, const string pPath, int nCardIndex)
{
	fstream ofile;
	ofile.open(pPath, ios_base::in | ios_base::out);
	ofile.seekp(sizeof(Card)*nCardIndex, ios_base::beg);
	ofile.write((char*)pCard, sizeof(Card));
	ofile.close();

	return true;
}
bool updatelogout(const Card* pCard, const string pPath, int nCardIndex)
{
	fstream ofile;
	ofile.open(pPath, ios_base::in | ios_base::out);
	ofile.write((char*)pCard, sizeof(Card));
	ofile.close();

	return true;
}
void writeallvec(CardNode*pCardNodeHead, int nCardIndex)
{
	CardNode *plocation;
	ofstream outf;
	outf.open("card.txt");
	plocation = pCardNodeHead;
	for (int i = 0; i < nCardIndex; i++)
	{

		outf.write((char *)plocation, sizeof(CardNode));
		plocation = plocation->next;
	}
	outf.close();
	
}