#include "User.h"

#include <string>
#include <vector>
using namespace std;

User::User(const string& full_name,
           const string& email,
           const vector<string>& watch_history)
    : full_name(full_name), email(email), watch_history(watch_history) {
  // Replace this line with correct code.
}

string User::get_full_name() const {
  return full_name;  // Replace this line with correct code.
}

string User::get_email() const {
  return email;  // Replace this line with correct code.
}

vector<string> User::get_watch_history() const {
  return watch_history;  // Replace this line with correct code.
}
