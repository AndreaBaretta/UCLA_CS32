#include "Recommender.h"
#include "MovieDatabase.h"
#include "UserDatabase.h"

#include <string>
#include <vector>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
    : user_database(user_database), movie_database(movie_database) {
  // Replace this line with correct code.
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email,
                                                   int movie_count) const {
  return vector<MovieAndRank>();  // Replace this line with correct code.
}
