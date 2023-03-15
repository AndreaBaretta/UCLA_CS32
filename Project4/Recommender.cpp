#include "Recommender.h"
#include "Movie.h"
#include "MovieDatabase.h"
#include "User.h"
#include "UserDatabase.h"
#include "treemm.h"

#include <algorithm>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std;

constexpr const int ACTOR_POINTS = 30;
constexpr const int DIRECTOR_POINTS = 20;
constexpr const int GENRE_POINTS = 1;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
    : user_database(user_database), movie_database(movie_database) {}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email,
                                                   int movie_count) const {
  if (movie_count <= 0) {
    return vector<MovieAndRank>();
  }
  unordered_map<Movie*, int> recommends;

  std::vector<string> watch_history_ids = user_database.get_user_from_email(user_email)->get_watch_history();
  std::unordered_set<Movie*> watch_history_ptrs;

  for (string id : watch_history_ids) {
    Movie* movie = movie_database.get_movie_from_id(id);
    watch_history_ptrs.insert(movie);

    for (string actor : movie->get_actors()) {
      for (Movie* p : movie_database.get_movies_with_actor(actor)) {
        recommends[p] += ACTOR_POINTS;
      }
    }

    for (string director : movie->get_directors()) {
      for (Movie* p : movie_database.get_movies_with_director(director)) {
        recommends[p] += DIRECTOR_POINTS;
      }
    }

    for (string genre : movie->get_genres()) {
      for (Movie* p : movie_database.get_movies_with_genre(genre)) {
        recommends[p] += GENRE_POINTS;
      }
    }
  }

  // Flip all values to instantly put list in right order
  auto comp = [](const pair<Movie*, int>& a,
                 const pair<Movie*, int>& b) {
    if (a.second < b.second) {
      return true;
    } else if (a.second > b.second) {
      return false;
    } else if (a.first->get_rating() < b.first->get_rating()) {
      return true;
    } else if (a.first->get_rating() > b.first->get_rating()) {
      return false;
    } else {
      return a.first->get_title() < b.first->get_title();
    }
  };

  vector<pair<Movie*, int>> _ret(recommends.size());
  copy(recommends.begin(), recommends.end(), _ret.begin());

  sort(_ret.begin(), _ret.end(), comp);

  vector<MovieAndRank> ret;
  for (vector<pair<Movie*, int>>::reverse_iterator it = _ret.rbegin();
       it != _ret.rend(); ++it) {
    if (watch_history_ptrs.count((*it).first)) {
      continue;
    }
    ret.emplace_back((*it).first->get_id(), (*it).second);
    if (ret.size() == movie_count) {
      break;
    }
  }

  return ret;
}
