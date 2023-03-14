#include "MovieDatabase.h"
#include "Movie.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

MovieDatabase::MovieDatabase()
    : id_map(), dir_map(), actor_map(), genre_map() {}

bool MovieDatabase::load(const string& filename) {
  std::ifstream file;
  file.open(filename);
  std::string line;
  std::string substr;

  std::string id;
  std::string title;
  std::string release_year;
  float rating;
  while (std::getline(file, id)) {
    std::vector<std::string> directors;
    std::vector<std::string> actors;
    std::vector<std::string> genres;

    if (!std::getline(file, title)) {
      file.close();
      return false;
    }

    if (!std::getline(file, release_year)) {
      file.close();
      return false;
    }

    if (!std::getline(file, line)) {
      file.close();
      return false;
    }

    stringstream ss(line);
    while (ss.good()) {
      getline(ss, substr, ',');
      directors.push_back(substr);
    }

    if (!std::getline(file, line)) {
      file.close();
      return false;
    }

    ss.str(line);
    ss.clear();
    while (ss.good()) {
      getline(ss, substr, ',');
      actors.push_back(substr);
    }

    if (!std::getline(file, line)) {
      file.close();
      return false;
    }

    ss.str(line);
    ss.clear();
    while (ss.good()) {
      getline(ss, substr, ',');
      genres.push_back(substr);
    }

    if (!std::getline(file, line)) {
      file.close();
      return false;
    }

    rating = std::stof(line);
    if (rating < 0) {
      file.close();
      return false;
    }

    std::getline(file, line);

    // std::cout << "New user: " << full_name << " (" << email
    //           << "): " << num_movies << " in history" << std::endl;

    // std::cout << "New movie " << id << ": " << title << " (" << release_year
    //           << ") - Dir: [";

    Movie* new_movie =
        new Movie(id, title, release_year, directors, actors, genres, rating);
    movies.push_back(new_movie);
    id_map.insert(new_movie->get_id(), new_movie);
    for (std::string d : new_movie->get_directors()) {
      dir_map.insert(d, new_movie);
      // std::cout << d << ",";
    }
    // std::cout << "] - Actors: [";
    for (std::string a : new_movie->get_actors()) {
      actor_map.insert(a, new_movie);
      // std::cout << a << ",";
    }
    // std::cout << "] - Genres: [";
    for (std::string g : new_movie->get_genres()) {
      genre_map.insert(g, new_movie);
      // std::cout << g << ",";
    }
    // std::cout << "] - Rating: " << rating << std::endl;
  }
  file.close();
  return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const {
  auto it = id_map.find(id);
  return it.is_valid() ? it.get_value() : nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(
    const string& director) const {
  vector<Movie*> v;
  for (auto it = dir_map.find(director); it.is_valid(); it.advance()) {
    v.push_back(it.get_value());
  }
  return v;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const {
  vector<Movie*> v;
  for (auto it = actor_map.find(actor); it.is_valid(); it.advance()) {
    v.push_back(it.get_value());
  }
  return v;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const {
  vector<Movie*> v;
  for (auto it = genre_map.find(genre); it.is_valid(); it.advance()) {
    v.push_back(it.get_value());
  }
  return v;
}

MovieDatabase::~MovieDatabase() {
  for (Movie* p : movies) {
    delete p;
  }
}