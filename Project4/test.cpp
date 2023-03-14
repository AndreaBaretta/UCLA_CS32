#include <chrono>
#include <filesystem>
#include <iostream>
#include "Movie.h"
#include "MovieDatabase.h"
#include "User.h"
#include "UserDatabase.h"
#include "treemm.h"

int main() {
  // TreeMultimap<std::string, int> tmm;
  // tmm.insert("carey", 5);
  // tmm.insert("carey", 6);
  // tmm.insert("carey", 7);
  // tmm.insert("david", 25);
  // tmm.insert("david", 425);
  // TreeMultimap<std::string, int>::Iterator it = tmm.find("carey");
  // // prints 5, 6, and 7 in some order
  // while (it.is_valid()) {
  //   std::cout << it.get_value() << std::endl;
  //   it.advance();
  // }
  // it = tmm.find("laura");
  // if (!it.is_valid())
  //   std::cout << "laura is not in the multimap!\n";

  UserDatabase db;
  std::chrono::time_point t1 = std::chrono::high_resolution_clock::now();
  std::cout << (db.load(std::filesystem::current_path() / "users.txt")
                    ? "Successfully loaded user database."
                    : "Error loading users.")
            << std::endl;
  std::chrono::time_point t2 = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> db_loading = t2 - t1;
  std::cout << "Time to load users: " << db_loading.count() << " ms" << std::endl;
  // db.map.printBlackDist();
  User* user = db.get_user_from_email("KDo04@inbox.com");
  std::cout << user->get_email() << std::endl;
  std::cout << user->get_full_name() << std::endl;
  std::cout << user->get_watch_history().size() << std::endl;

  MovieDatabase m_db;
  t1 = std::chrono::high_resolution_clock::now();
  std::cout << (m_db.load(std::filesystem::current_path() / "movies.txt")
                    ? "Successfully loaded movie database."
                    : "Error loading movies.")
            << std::endl;
  t2 = std::chrono::high_resolution_clock::now();
  db_loading = t2 - t1;
  std::cout << "Time to load movies: " << db_loading.count() << " ms" << std::endl;

  std::vector<Movie*> movies = m_db.get_movies_with_actor("Will Smith");
  std::cout << "Bruce Willis movies: [";
  for (Movie* m : movies) {
    std::cout << m->get_title() << ",";
  }
  std::cout << "]" << std::endl;
}