#include <chrono>
#include <filesystem>
#include <iostream>
#include "Movie.h"
#include "MovieDatabase.h"
#include "User.h"
#include "UserDatabase.h"
#include "Recommender.h"
#include "treemm.h"

using namespace std;

int main() {
  // TreeMultimap<string, int> tmm;
  // tmm.insert("carey", 5);
  // tmm.insert("carey", 6);
  // tmm.insert("carey", 7);
  // tmm.insert("david", 25);
  // tmm.insert("david", 425);
  // TreeMultimap<string, int>::Iterator it = tmm.find("carey");
  // // prints 5, 6, and 7 in some order
  // while (it.is_valid()) {
  //   cout << it.get_value() << endl;
  //   it.advance();
  // }
  // it = tmm.find("laura");
  // if (!it.is_valid())
  //   cout << "laura is not in the multimap!\n";

  UserDatabase u_db;
  auto t1 = chrono::high_resolution_clock::now();
  cout << (u_db.load(filesystem::current_path() / "users.txt")
                    ? "Successfully loaded user database."
                    : "Error loading users.")
            << endl;
  auto t2 = chrono::high_resolution_clock::now();

  chrono::duration<double, milli> time_milli = t2 - t1;
  cout << "Time to load users: " << time_milli.count() << " ms" << endl;
  // db.map.printBlackDist();
  User* user = u_db.get_user_from_email("RylaW06942@inbox.com");
  cout << user->get_email() << endl;
  cout << user->get_full_name() << endl;
  cout << user->get_watch_history().size() << endl;

  MovieDatabase m_db;
  t1 = chrono::high_resolution_clock::now();
  cout << (m_db.load(filesystem::current_path() / "movies.txt")
                    ? "Successfully loaded movie database."
                    : "Error loading movies.")
            << endl;
  t2 = chrono::high_resolution_clock::now();
  time_milli = t2 - t1;
  cout << "Time to load movies: " << time_milli.count() << " ms" << endl;

  string actor = "Christopher Lloyd";
  vector<Movie*> movies = m_db.get_movies_with_actor(actor);
  cout << actor << " movies: [";
  for (Movie* m : movies) {
    cout << m->get_title() << ",";
  }
  cout << "]" << endl;

  Recommender rec(u_db, m_db);
  cout << "Recommendations for " << user->get_email() << endl;
  t1 = chrono::high_resolution_clock::now();
  vector<MovieAndRank> recommendations = rec.recommend_movies(user->get_email(), 1000);
  t2 = chrono::high_resolution_clock::now();
  time_milli = t2 - t1;
  for (MovieAndRank& mvr : recommendations) {
    cout << m_db.get_movie_from_id(mvr.movie_id)->get_title() << " (compat score: " << mvr.compatibility_score << ")" << endl;
  }
  cout << "Time to load recommendations: " << time_milli.count() << " ms" << endl;
}