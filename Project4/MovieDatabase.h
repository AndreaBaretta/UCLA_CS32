#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include "treemm.h"

class Movie;

class MovieDatabase {
  std::list<Movie*> movies;
  TreeMultimap<std::string, Movie*> id_map;
  TreeMultimap<std::string, Movie*> dir_map;
  TreeMultimap<std::string, Movie*> actor_map;
  TreeMultimap<std::string, Movie*> genre_map;

 public:
  MovieDatabase();
  bool load(const std::string& filename);
  Movie* get_movie_from_id(const std::string& id) const;
  std::vector<Movie*> get_movies_with_director(
      const std::string& director) const;
  std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
  std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;
  ~MovieDatabase();
 private:
  std::string str_tolower(std::string s) const;
};

#endif  // MOVIEDATABASE_INCLUDED
