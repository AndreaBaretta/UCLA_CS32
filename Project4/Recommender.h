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
  struct MoviePtrAndRank {
   public:
    const Movie* movie;
    int compatibility_score;

    MoviePtrAndRank(const Movie* movie, int score)
        : movie(movie), compatibility_score(score) {}

    bool operator<(const MoviePtrAndRank& a) const;

    bool operator==(const MoviePtrAndRank& a) const;
  };

  const UserDatabase& user_database;
  const MovieDatabase& movie_database;

 public:
  Recommender(const UserDatabase& user_database,
              const MovieDatabase& movie_database);
  std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                             int movie_count) const;

 private:
};

#endif  // RECOMMENDER_INCLUDED
