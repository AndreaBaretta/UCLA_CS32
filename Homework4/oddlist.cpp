void removeOdd(std::list<int>& li) {
    for (std::list<int>::iterator it = li.begin(); it != li.end();) {
        if (*it % 2 == 1) {
            it = li.erase(it);
        } else {
            ++it;
        }
    }
}