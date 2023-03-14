#include "Movie.h"

#include <string>
#include <vector>
using namespace std;

Movie::Movie(const string& id,
             const string& title,
             const string& release_year,
             const vector<string>& directors,
             const vector<string>& actors,
             const vector<string>& genres,
             float rating)
    : id(id),
      title(title),
      release_year(release_year),
      directors(directors),
      actors(actors),
      genres(genres),
      rating(rating) {}

string Movie::get_id() const {
  return id;
}

string Movie::get_title() const {
  return title;
}

string Movie::get_release_year() const {
  return release_year;
}

float Movie::get_rating() const {
  return rating;
}

vector<string> Movie::get_directors() const {
  return directors;
}

vector<string> Movie::get_actors() const {
  return actors;
}

vector<string> Movie::get_genres() const {
  return genres;
}
