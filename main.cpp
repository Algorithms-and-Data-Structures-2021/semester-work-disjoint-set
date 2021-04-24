#include <iostream>

#include "data_structure.hpp"

using namespace itis;

int main(int argc, char *argv[]) {
  for (int index = 0; index < argc; index++) {
    std::cout << index << ": " << argv[index] << std::endl;
  }

  return 0;
}