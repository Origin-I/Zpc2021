#include<AgendaService.hpp>

AgendaService::AgendaService() 
{
   startAgenda();
}

AgendaService::~AgendaService() 
{
   quitAgenda();
}

bool AgendaService::userLogIn(const std::string &userName, const std::string &password) 
{
   auto f = [userName, password](const User& u){
      return u.getName() == userName && u.getPassword() == password;
   };
   return m_storage->queryUser(f).size() > 0;
}

bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone) 
{
   auto f = [userName, password, email, phone](const User& u){
      return u.getName() == userName || u.getEmail() == email
            || u.getPhone() == phone;
   };
   if(m_storage->queryUser(f).size() == 0){
      m_storage->createUser(User(userName, password, email, phone));
      return true;
   }
   return false;
}
        
bool AgendaService::deleteUser(const std::string &userName, const std::string &password)
{
   auto f = [userName, password](const User& u){
      return u.getName() == userName && 
					u.getPassword() == password;
   };
   if(m_storage->Storage::deleteUser(f) > 0){
      deleteAllMeetings(userName);
      return true;
   }
   return false;
}
        // a user can only delete itself
std::list<User> AgendaService::listAllUsers(void)const
{
   auto f = [](const User& u){
      return true;
   };
   return m_storage->queryUser(f);
}

bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator)
{
   if(!Date::isValid(Date::stringToDate(startDate))
       || !Date::isValid(Date::stringToDate(endDate)))
      return false;
   
   auto f1 = [participator](const User& u){
      for(auto it = participator.cbegin();it != participator.cend();it++){
      	if(u.getName() == *it)
      		return true;
		}
		return false;
   };
   if(m_storage->queryUser(f1).size() <= 0)
      return false;
   
   auto f2 = [title](const Meeting& m){
      return m.getTitle() == title;
   };
   if(m_storage->queryMeeting(f2).size() > 0)
      return false;
   
   if(meetingQuery(userName, startDate, endDate).size() > 0)
      return false;
   
   m_storage->createMeeting(Meeting(userName, participator,
         Date::stringToDate(startDate), Date::stringToDate(endDate), title));
      return true;
}
        
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &title)const
{
   
	return m_storage->queryMeeting([userName, title](const Meeting &m){
		int i=0,count=m.getParticipator().size();
		if(m.getTitle() == title){
			if(m.getSponsor() == userName)
				return true;
			else{
				while(i++<count){
					if(m.getParticipator()[i] == userName)
						return true;
				}
			}
		}
		return false;
	});
}
        
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate)const
{
   Date sta = Date::stringToDate(startDate);
	Date end = Date::stringToDate(endDate);
   if(sta > end){
      std::list<Meeting> tmp;     
      return tmp;
   }
   
	return m_storage->queryMeeting([&](const Meeting &m){
		int i=0,count=m.getParticipator().size();
		if(m.getSponsor() == userName){
			if((m.getStartDate() <= sta && sta <= m.getEndDate())
				|| (m.getStartDate() <= end && end <= m.getEndDate()))
					return true;
		}
		else{
			if((m.getStartDate() <= sta && sta <= m.getEndDate())
				|| (m.getStartDate() <= end && end <= m.getEndDate())){
				while(i++<count){
					if(m.getParticipator()[i] == userName)
						return true;
				}
			}
		}
		return false;
	});
}
        
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName)const
{
   
	auto f = [userName](const Meeting& m){
		int i=0,count=m.getParticipator().size();
   	if(m.getSponsor() == userName)
   		return true;
   	else{
   		while(i++<count){
				if(m.getParticipator()[i] == userName)
					return true;
			}
		}
		return false;
   };
   return m_storage->queryMeeting(f);
}
        
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName)const
{
   auto f = [userName](const Meeting& m){
      return m.getSponsor() == userName;
   };
   return m_storage->queryMeeting(f);
}
        
std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const
{
   
	auto f = [userName](const Meeting& m){
		int i=0,count=m.getParticipator().size();
   	while(i++<count){
			if(m.getParticipator()[i] == userName)
				return true;
		}
      return false;
   };
   return m_storage->queryMeeting(f);
}
        
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) 
{
  
	auto f = [userName, title](const Meeting& m){
		int i=0,count=m.getParticipator().size();
   	if(m.getTitle() == title){
   		if(m.getSponsor() == userName)
   			return true;
   		else{
   			while(i++<count){
					if(m.getParticipator()[i] == userName)
						return true;
				}
			}
		}
		return false;
   };
   return m_storage->deleteMeeting(f) > 0;
}
        
bool AgendaService::deleteAllMeetings(const std::string &userName) 
{
   
	auto f = [userName](const Meeting& m) {
		int i=0,count=m.getParticipator().size();
   	if(m.getSponsor() == userName)
   		return true;
   	else{
   		while(i++<count){
				if(m.getParticipator()[i] == userName)
					return true;
			}
		}
		return false;
   };
   return m_storage->deleteMeeting(f) > 0;
}
      
void AgendaService::startAgenda(void) 
{
   m_storage = Storage::getInstance();
}
        
void AgendaService::quitAgenda(void) 
{
   m_storage->sync();
}

