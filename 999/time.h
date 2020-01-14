struct tm
{ 
	int tm_sec;  /*秒，0-59*/  
	int tm_min;  /*分钟，0-59*/ 
	int tm_hour; /*小时， 0-23*/   
	int tm_mday; /*日，1-31*/ 
	int tm_mon;  /*月，0-11*/     
	int tm_year;  /*年，从1900至今已经多少年*/   
	int tm_wday; /*星期，从星期日算起，0-6*/    
	int tm_yday; /*天数，0-365*/    
	int tm_isdst; /*日光节约时间的旗标*/ 
};
