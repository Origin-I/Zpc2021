#include <iostream>
#include <string>
#include <sstream>
#include <Date.hpp> 
using namespace std;

Date::Date() 
{ 
	m_year=0; 
	m_month=0; 
	m_day=0; 
	m_hour=0; 
	m_minute=0;
}

Date::Date ( int t_year, int t_month, int t_day, int t_hour, int t_minute ) 
{ 
	m_year=t_year; 
	m_month=t_month; 
	m_day=t_day; 
	m_hour=t_hour; 
	m_minute=t_minute;
}
	   
Date::Date (const std::string &dateString)
{
	if ( dateString.length() != 16 ) 
		m_year = m_month = m_day = m_hour = m_minute = 0;
	
	else {
		stringstream ss;
   	char t;
   	ss << dateString;
   	ss >> m_year >> t >> m_month >> t >> m_day >> t >> m_hour >> t >> m_minute;
	}
}

int Date::getYear(void)const
{
	return m_year;
}	   

void Date::setYear(const int t_year)
{
	m_year = t_year;	
}		

int Date::getMonth(void)const
{
	return m_month;
}

void Date::setMonth(const int t_month)
{
	m_month = t_month;
}

int Date::getDay(void)const
{
	return m_day;
}

void Date::setDay(const int t_day)
{
	m_day = t_day;
}

int Date::getHour(void)const
{
	return m_hour;
}

void Date::setHour(const int t_hour)
{
	m_hour = t_hour;
}

int Date::getMinute(void)const
{
	return m_minute;
}

void Date::setMinute(const int t_minute)
{
	m_minute = t_minute;
}

bool Date::isValid(const Date &t_date)
{
	if ( t_date.m_year < 1000 || t_date.m_year > 9999 || t_date.m_month < 1 || t_date.m_month > 12 || 
		  t_date.m_day < 1 || t_date.m_hour < 0 || t_date.m_hour > 23 || t_date.m_minute < 0 || t_date.m_minute > 59 )
		return false;
	else{
		if ( t_date.m_month == 1 || t_date.m_month == 3 || t_date.m_month == 5 || t_date.m_month == 7 ||
			  t_date.m_month == 8 || t_date.m_month == 10 || t_date.m_month ==12 ){
			if( t_date.m_day > 31 )
				return false;		  	
		}
		
		else if ( t_date.m_month == 4 || t_date.m_month == 6 || t_date.m_month == 9 || t_date.m_month == 11 ){
			if( t_date.m_day > 30 )
				return false;
		}
		
		else{
			if ( t_date.m_year % 4 == 0 )
	      {
	      	if ( t_date.m_year % 100 == 0 )
	         {
	            // // 这里如果被 400 整数是闰年
	            if ( t_date.m_year % 400 == 0 ){
	            	if ( t_date.m_day > 29 )
	            		return false;
					}
	            	
	            else {
	            	if ( t_date.m_day > 28 )
	            		return false;
					}      
	         }
	         
	         else {
	         	if ( t_date.m_day > 29 )
	            		return false;
	      	}
	      }
	      
	      else {
	      	if ( t_date.m_day > 28 )
	            		return false;
			}
	   }
	}
	return true;
}

Date Date::stringToDate(const std::string &t_dateString)
{
	stringstream ss;
   int y, m, d, h, mi;
   char t;
   ss << t_dateString;
   ss >> y >> t >> m >> t >> d >> t >> h >> t >> mi;
   return Date(y, m, d, h, mi);
}

std::string Date::dateToString(const Date &t_date)
{
	if ( !isValid( t_date ) )
		return "0000-00-00/00:00";
	
		char y[5], m[3], d[3], h[3], mm[3], date[20];
		sprintf ( y, "%d", t_date.m_year );
		sprintf ( m, "%02d", t_date.m_month );
		sprintf ( d, "%02d", t_date.m_day );
		sprintf ( h, "%02d", t_date.m_hour );
		sprintf ( mm, "%02d", t_date.m_minute );
		sprintf ( date, "%s-%s-%s/%s:%s", y, m, d, h, mm );
		string str ( date );
		return str;
	
}

Date& Date::operator=(const Date &t_date)
{
	m_year = t_date.m_year; 
	m_month = t_date.m_month;
	m_day = t_date.m_day;
	m_hour = t_date.m_hour;
	m_minute = t_date.m_minute;
}

bool Date::operator==(const Date &t_date)const
{
	if( m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day &&
		 m_hour == t_date.m_hour && m_minute == t_date.m_minute )
		return true;
	return false;
}

bool Date::operator>(const Date &t_date)const
{
	if( m_year > t_date.m_year )
		return true;
	else if( m_year == t_date.m_year ){
		if( m_month > t_date.m_month )
			return true;
		else if( m_month == t_date.m_month ){
			if( m_day > t_date.m_day )
				return true;
			else if( m_day == t_date.m_day ){
				if( m_hour > t_date.m_hour )
					return true;
				else if( m_hour == t_date.m_hour ){
					if( m_minute > t_date.m_minute)
						return true;
				}
			}
		}
	}
	return false;
}

bool Date::operator<(const Date &t_date)const
{
	if( m_year < t_date.m_year )
		return true;
	else if( m_year == t_date.m_year ){
		if( m_month < t_date.m_month )
			return true;
		else if( m_month == t_date.m_month ){
			if( m_day < t_date.m_day )
				return true;
			else if( m_day == t_date.m_day ){
				if( m_hour < t_date.m_hour )
					return true;
				else if( m_hour == t_date.m_hour ){
					if( m_minute < t_date.m_minute)
						return true;
				}
			}
		}
	}
	return false;
}

bool Date::operator>=(const Date &t_date)const
{
	if( m_year > t_date.m_year )
		return true;
	else if( m_year == t_date.m_year ){
		if( m_month > t_date.m_month )
			return true;
		else if( m_month == t_date.m_month ){
			if( m_day > t_date.m_day )
				return true;
			else if( m_day == t_date.m_day ){
				if( m_hour > t_date.m_hour )
					return true;
				else if( m_hour == t_date.m_hour ){
					if( m_minute >= t_date.m_minute )
						return true;
				}
			}
		}
	}
	return false;
}

bool Date::operator<=(const Date &t_date)const
{
	if( m_year < t_date.m_year )
		return true;
	else if( m_year == t_date.m_year ){
		if( m_month < t_date.m_month )
			return true;
		else if( m_month == t_date.m_month ){
			if( m_day < t_date.m_day )
				return true;
			else if( m_day == t_date.m_day ){
				if( m_hour < t_date.m_hour )
					return true;
				else if( m_hour == t_date.m_hour ){
					if( m_minute <= t_date.m_minute )
						return true;
				}
			}
		}
	}
	return false;
}

