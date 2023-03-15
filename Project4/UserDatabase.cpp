#include "UserDatabase.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "User.h"

using namespace std;

UserDatabase::UserDatabase() : users(), map() {}

bool UserDatabase::load(const string& filename) {
  ifstream file;
  file.open(filename);
  string line;

  string full_name;
  string email;
  string s_num_movies;
  int num_movies;
  while (getline(file, full_name)) {
    if (!getline(file, email)) {
      file.close();
      return false;
    }
    if (!getline(file, s_num_movies)) {
      file.close();
      return false;
    }
    num_movies = stoi(s_num_movies);
    if (num_movies < 0) {
      file.close();
      return false;
    }

    vector<string> watch_history(num_movies);
    for (int i = 0; i < num_movies; ++i) {
      getline(file, line);
      if (line == "") {
        file.close();
        return false;
      }
      watch_history[i] = line;
    }
    getline(file, line);

    // cout << "New user: " << full_name << " (" << email
    //           << "): " << num_movies << " in history" << endl;

    User* new_user = new User(full_name, email, watch_history);
    users.push_back(new_user);
    map.insert(new_user->get_email(), new_user);
  }
  file.close();
  return true;
}

User* UserDatabase::get_user_from_email(const string& email) const {
  const auto it = map.find(email);
  return it.is_valid() ? it.get_value() : nullptr;
}

UserDatabase::~UserDatabase() {
  for (User* user : users) {
    delete user;
  }
}