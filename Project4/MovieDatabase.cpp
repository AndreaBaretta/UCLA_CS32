#include "MovieDatabase.h"
#include "Movie.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

MovieDatabase::MovieDatabase()
    : id_map(), dir_map(), actor_map(), genre_map() {}

bool MovieDatabase::load(const string& filename) {
  ifstream file;
  file.open(filename);
  string line;
  string substr;

  string id;
  string title;
  string release_year;
  float rating;
  while (getline(file, id)) {
    vector<string> directors;
    vector<string> actors;
    vector<string> genres;

    if (!getline(file, title)) {
      file.close();
      return false;
    }

    if (!getline(file, release_year)) {
      file.close();
      return false;
    }

    if (!getline(file, line)) {
      file.close();
      return false;
    }

    stringstream ss(line);
    while (ss.good()) {
      getline(ss, substr, ',');
      directors.push_back(substr);
    }

    if (!getline(file, line)) {
      file.close();
      return false;
    }

    ss.str(line);
    ss.clear();
    while (ss.good()) {
      getline(ss, substr, ',');
      actors.push_back(substr);
    }

    if (!getline(file, line)) {
      file.close();
      return false;
    }

    ss.str(line);
    ss.clear();
    while (ss.good()) {
      getline(ss, substr, ',');
      genres.push_back(substr);
    }

    if (!getline(file, line)) {
      file.close();
      return false;
    }

    rating = stof(line);
    if (rating < 0) {
      file.close();
      return false;
    }

    getline(file, line);

    Movie* new_movie =
        new Movie(id, title, release_year, directors, actors, genres, rating);
    movies.push_back(new_movie);

    id_map.insert(str_tolower(new_movie->get_id()), new_movie);
    for (string d : new_movie->get_directors()) {
      dir_map.insert(str_tolower(d), new_movie);
    }
    for (string a : new_movie->get_actors()) {
      actor_map.insert(str_tolower(a), new_movie);
    }
    for (string g : new_movie->get_genres()) {
      genre_map.insert(str_tolower(g), new_movie);
    }
  }
  file.close();
  return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const {
  const string l_id = str_tolower(id);
  auto it = id_map.find(l_id);
  return it.is_valid() ? it.get_value() : nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(
    const string& director) const {
  vector<Movie*> v;
  const string l_director = str_tolower(director);
  for (auto it = dir_map.find(l_director); it.is_valid(); it.advance()) {
    v.push_back(it.get_value());
  }
  return v;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const {
  vector<Movie*> v;
  const string l_actor = str_tolower(actor);
  for (auto it = actor_map.find(l_actor); it.is_valid(); it.advance()) {
    v.push_back(it.get_value());
  }
  return v;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const {
  vector<Movie*> v;
  const string l_genre = str_tolower(genre);
  for (auto it = genre_map.find(l_genre); it.is_valid(); it.advance()) {
    v.push_back(it.get_value());
  }
  return v;
}

MovieDatabase::~MovieDatabase() {
  for (Movie* p : movies) {
    delete p;
  }
}

string MovieDatabase::str_tolower(string s) const {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return tolower(c); });
  return s;
}