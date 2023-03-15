#include "Recommender.h"
#include "Movie.h"
#include "MovieDatabase.h"
#include "User.h"
#include "UserDatabase.h"
#include "treemm.h"

#include <algorithm>
#include <list>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std;

constexpr const int ACTOR_POINTS = 30;
constexpr const int DIRECTOR_POINTS = 20;
constexpr const int GENRE_POINTS = 1;

bool Recommender::MoviePtrAndRank::operator<(const MoviePtrAndRank& a) const {
  return movie->get_id() < a.movie->get_id();
}

bool Recommender::MoviePtrAndRank::operator==(const MoviePtrAndRank& a) const {
  return movie->get_id() == a.movie->get_id();
}

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
    : user_database(user_database), movie_database(movie_database) {}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email,
                                                   int movie_count) const {
  if (movie_count <= 0) {
    return vector<MovieAndRank>();
  }
  TreeMultimap<Movie*, MoviePtrAndRank> recommends;
  vector<Movie*> recommends_ptrs;

  const vector<string> _watch_history =
      user_database.get_user_from_email(user_email)->get_watch_history();
  const set<string> watch_history(_watch_history.begin(), _watch_history.end());

  for (string id : watch_history) {
    Movie* movie = movie_database.get_movie_from_id(id);

    for (string actor : movie->get_actors()) {
      vector<Movie*> movies_with_actor =
          movie_database.get_movies_with_actor(actor);

      for (Movie* p : movies_with_actor) {
        if (watch_history.count(p->get_id()) == 0) {
          auto it = recommends.find(p);
          if (it.is_valid()) {
            it.get_value().compatibility_score += ACTOR_POINTS;
          } else {
            recommends_ptrs.push_back(p);
            recommends.insert(p, MoviePtrAndRank(p, ACTOR_POINTS));
          }
        }
      }
    }

    for (string director : movie->get_directors()) {
      vector<Movie*> movies_with_director =
          movie_database.get_movies_with_director(director);

      for (Movie* p : movies_with_director) {
        if (watch_history.count(p->get_id()) == 0) {
          auto it = recommends.find(p);
          if (it.is_valid()) {
            it.get_value().compatibility_score += DIRECTOR_POINTS;
          } else {
            recommends_ptrs.push_back(p);
            recommends.insert(p, MoviePtrAndRank(p, DIRECTOR_POINTS));
          }
        }
      }
    }

    for (string genre : movie->get_genres()) {
      vector<Movie*> movies_with_genre =
          movie_database.get_movies_with_genre(genre);

      for (Movie* p : movies_with_genre) {
        if (watch_history.count(p->get_id()) == 0) {
          auto it = recommends.find(p);
          if (it.is_valid()) {
            it.get_value().compatibility_score += GENRE_POINTS;
          } else {
            recommends_ptrs.push_back(p);
            recommends.insert(p, MoviePtrAndRank(p, GENRE_POINTS));
          }
        }
      }
    }
  }

  // Flip all values to instantly put list in right order
  auto comp = [](const Recommender::MoviePtrAndRank& a,
                 const Recommender::MoviePtrAndRank& b) {
    if (a.compatibility_score < b.compatibility_score) {
      return true;
    } else if (a.compatibility_score > b.compatibility_score) {
      return false;
    } else if (a.movie->get_rating() < b.movie->get_rating()) {
      return true;
    } else if (a.movie->get_rating() > b.movie->get_rating()) {
      return false;
    } else {
      return a.movie->get_title() < b.movie->get_title();
    }
  };

  vector<MoviePtrAndRank> _ret;
  for (int i = 0; i < recommends_ptrs.size(); ++i) {
    _ret.push_back(recommends.find(recommends_ptrs[i]).get_value());
  }
  std::sort(_ret.begin(), _ret.end(), comp);

  vector<MovieAndRank> ret;
  for (vector<MoviePtrAndRank>::reverse_iterator it = _ret.rbegin();
       it != _ret.rend(); ++it) {
    ret.emplace_back((*it).movie->get_id(), (*it).compatibility_score);
    if (ret.size() == movie_count) {
      break;
    }
  }

  return ret;
}
