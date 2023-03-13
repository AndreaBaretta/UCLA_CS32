void removeBad(std::list<Movie*>& li) {
    for (std::list<Movie*>::iterator it = li.begin(); it != li.end();) {
        if ((*it)->rating() < 50) {
            delete *it;
            it = li.erase(it);
        } else {
            ++it;
        }
    }
}