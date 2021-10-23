#include <fstream>
#include <cstring> 
#include <Storage.hpp>
using namespace std;
std::shared_ptr<Storage> Storage::m_instance = std::make_shared<Storage>();

Storage::Storage()
{
	m_dirty = 0;
	readFromFile();
}

bool Storage::readFromFile(void)
{
	ifstream iu("/tmp/users.csv");
	
	if(!iu.is_open())
		return false;
	std::string stru,na,pa,em,ph;

	char ss[100],name[20][20],pass[20][20],email[20][20],phone[20][20];

	User tmp;
	int i,j,tag=0,unum=0;
	while(!iu.eof()){
		iu>>stru;
		stru.copy(ss,stru.length(),0);
		for(i=0;i<stru.length();i++){
			if(ss[i]=='"'){
				tag++;
				j=0;
				i++;
			}
			if(tag==1)
				name[unum][j++]=ss[i];
			else if(tag==3)
				pass[unum][j++]=ss[i];
			else if(tag==5)
				email[unum][j++]=ss[i];
			else if(tag==7)
				phone[unum][j++]=ss[i];	
		}
		na=name[unum];
		pa=pass[unum];
		em=email[unum];
		ph=phone[unum];
		tmp.setName(na);
		tmp.setPassword(pa);
		tmp.setEmail(em);
		tmp.setPhone(ph);
		m_userList.push_back(tmp);
		unum++;
		memset(ss,'\0',sizeof(ss));
	}
	
	ifstream im("/tmp/meetings.csv");
	if(!im.is_open())
		return false;
	tag=0;
	int mnum=0;
	Meeting mtmp;
	std::vector<std::string> vec;
	std::string strm,sp,par,st,en,ti;
	char spon[20][20],part[20],start[20][20],end[20][20],title[20][20];	
	while(!im.eof()){
		im>>strm;
		strm.copy(ss,strm.length(),0);
		for(i=0;i<strm.length();i++){
			if(ss[i]=='"'){
				tag++;
				j=0;
				i++;
			}
			if(tag==1)
				spon[mnum][j++]=ss[i];
			else if(tag==3){
				i++;
				while(ss[i]!='"'){
					if(ss[i]!='&')
						part[j++]=ss[i++];
					else{
						par=part;
						vec.push_back(pa);
						par.clear();
						memset(part,'\0',sizeof(part));
						j=0;
						i++;
					}		
				}
				par=part;
				vec.push_back(par);
			}
			else if(tag==5)
				start[mnum][j++]=ss[i];
			else if(tag==7)
				end[mnum][j++]=ss[i];
			else if(tag==9)
				title[mnum][j++]=ss[i];	
		}
		sp=spon[mnum];
		st=start[mnum];
		en=end[mnum];
		ti=title[mnum];
		mtmp.setSponsor(sp);
		mtmp.setParticipator(vec);
		mtmp.setStartDate(st);
		mtmp.setEndDate(en);
		mtmp.setTitle(ti);
		m_meetingList.push_back(mtmp);
		mnum++;
		memset(ss,'\0',sizeof(ss));
		vec.clear();
	}
	
	return true;
}

bool Storage::writeToFile(void)
{
	ofstream ou("/tmp/users.csv");
	if(!ou.is_open())
		return false;
	
	for(auto uit = m_userList.begin(); uit != m_userList.end(); uit++){
		ou<<"\""<<uit->getName()<<"\",\""<<uit->getPassword()<<"\",\""
		<<uit->getEmail()<<"\",\""<<uit->getPhone()<<"\""<<endl;
	}
	ou.close();
	
	ofstream om("/tmp/meetings.csv");
	if(!om.is_open())
		return false;
	int j=0;
	
	for(auto mit = m_meetingList.begin(); mit != m_meetingList.end(); mit++){
		om<<"\""<<mit->getSponsor()<<"\",\"";
		while(!mit->getParticipator().empty()){
			if(j!=0)
				om<<"&";
			om<<mit->getParticipator()[j++];
		}
		om<<"\",\""<<Date::dateToString(mit->getStartDate())<<"\",\""
		<<Date::dateToString(mit->getEndDate())<<"\",\""<<mit->getTitle()<<"\""<<endl;
	}
	om.close();
	
	m_dirty = 0;
	return true;
}

std::shared_ptr<Storage> Storage::getInstance(void)
{
	if(!m_instance)
		m_instance = std::make_shared<Storage>();
	return m_instance;
}

Storage::~Storage()
{
	writeToFile();
}

void Storage::createUser(const User &t_user)
{
	m_userList.push_back(t_user); 
	m_dirty = 1;
}
//返回值为bool，参数为User 
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const
{
	list<User> returnList;
   for(auto it = m_userList.begin(); it != m_userList.end(); it++) {
      if(filter(*it)){
         returnList.push_back(*it);
      }
   }
   return returnList;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher)
{
	int updU = 0;
   for(auto it = m_userList.begin();it != m_userList.end();it++){
      if(filter(*it)){
         switcher(*it);
         updU++;
      }
   }
   if(updU != 0)
   	m_dirty = 1;
   return updU;
}

int Storage::deleteUser(std::function<bool(const User &)> filter)
{
	int delU = 0;
	for(auto it = m_userList.begin();it != m_userList.end();it++){
		if(filter(*it)){
         m_userList.push_back(*it);
         delU++;
      }
	}
	if(delU != 0)
		m_dirty = 1;
	return delU;
}

void Storage::createMeeting(const Meeting &t_meeting)
{
	m_meetingList.push_back(t_meeting);
	m_dirty = 1;
}

std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const
{
	list<Meeting> returnList;
   for(auto it = m_meetingList.begin();it != m_meetingList.end();it++){
      if(filter(*it)){
         returnList.push_back(*it);
      }
   }
   return returnList;
}   

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
               std::function<void(Meeting &)> switcher)
{
	int updM = 0;
   for(auto it = m_meetingList.begin();it != m_meetingList.end();it++){
      if(filter(*it)){
         switcher(*it);
         updM++;
      }
   }
   if(updM != 0)
   	m_dirty = 1;
   return updM;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter)
{
	int delM = 0;
   for(auto it = m_meetingList.begin();it != m_meetingList.end();it++){
      if(filter(*it)){
         m_meetingList.push_back(*it);
         delM++;
      }
   }
   if(delM != 0)
   	m_dirty = 1;
   return delM;
}

bool Storage::sync(void)
{
   if (m_dirty){
      writeToFile();
      m_dirty = 0;
      return true;
   }
   else 
      return false;  
}
