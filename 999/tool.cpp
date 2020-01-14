
#include"stdafx.h"
#include <time.h> 
#include <stdio.h> 

void timeToString(time_t t, char* pBuf) 
{
	struct tm * timeinfo;

	timeinfo = localtime(&t);
	strftime(pBuf, 20, "%Y-%m-%d %H:%M", timeinfo);

}

time_t stringToTime(int y,int m,int d) 
{
	
	struct tm tm1;  
	time_t time1;

	tm1.tm_year = y-1900;  // ���Ϊ��1900�꿪ʼ    
	tm1.tm_mon = m-1;      // �·�Ϊ0~11  
	tm1.tm_mday = d-1;
	tm1.tm_hour = 0;
	tm1.tm_min = 0;
	tm1.tm_sec = 0; 
	tm1.tm_isdst = -1; 

	time1 = mktime(&tm1);


	return time1;
}