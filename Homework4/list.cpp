void listAll(std::string path, const File* f) {  // two-parameter overload
  if (f == nullptr) { return; }
  if (f->files() == nullptr) {
    std::cout << path << std::endl;
  } else {
    std::cout << path << std::endl;
    for (const File* f : *f->files()) {
      std::string new_path = path + "/" + f->name();
      listAll(new_path, f);
    }
  }
}