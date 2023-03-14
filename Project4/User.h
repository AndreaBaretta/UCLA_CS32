#ifndef USER_INCLUDED
#define USER_INCLUDED

#include <string>
#include <vector>

class User {
 private:
  const std::string full_name;
  const std::string email;
  const std::vector<std::string> watch_history; 
 public:
  User(const std::string& full_name,
       const std::string& email,
       const std::vector<std::string>& watch_history);
  std::string get_full_name() const;
  std::string get_email() const;
  std::vector<std::string> get_watch_history() const;
};

#endif  // USER_INCLUDED
