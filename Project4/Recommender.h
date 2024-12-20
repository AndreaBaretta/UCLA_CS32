#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>

class UserDatabase;
class MovieDatabase;
class Movie;

struct MovieAndRank {
  MovieAndRank(const std::string& id, int score)
      : movie_id(id), compatibility_score(score) {}

  std::string movie_id;
  int compatibility_score;
};

class Recommender {
 private:
  const UserDatabase& user_database;
  const MovieDatabase& movie_database;

 public:
  Recommender(const UserDatabase& user_database,
              const MovieDatabase& movie_database);
  std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                             int movie_count) const;

 private:
  std::string str_tolower(std::string s) const;
};

#endif  // RECOMMENDER_INCLUDED
