#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include <string>
#include <vector>
#include "treemm.h"

class User;

class UserDatabase {
 public: //TODO: change back to private
  std::vector<User*> users;
  TreeMultimap<std::string, User*> map;
 public:
  UserDatabase();
  bool load(const std::string& filename);
  User* get_user_from_email(const std::string& email) const;
  ~UserDatabase();
};

#endif  // USERDATABASE_INCLUDED
