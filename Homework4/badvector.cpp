void removeBad(std::vector<Movie*>& v) {
    for (std::vector<Movie*>::iterator it = v.begin(); it != v.end();) {
        if ((*it)->rating() < 50) {
            delete *it;
            it = v.erase(it);
        } else {
            ++it;
        }
    }
}