#include <fstream>      // ifstream
#include <iostream>     // cout
#include <string>       // string, stoi
#include <string_view>  // string_view
#include <chrono>       // high_resolution_clock, duration_cast, nanoseconds
#include <errno.h>      // for errno
#include <stdlib.h>     // for strtol

// подключаем структуру данных
#include "data_structure.hpp"

using namespace std;
using namespace itis;

// абсолютный путь до набора данных и папки проекта
static constexpr auto kDatasetPath = string_view{PROJECT_DATASET_DIR};
static constexpr auto kProjectPath = string_view{PROJECT_SOURCE_DIR};

int main(int argc, char **argv) {

  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " NUMBER_OF_ELEMENTS" << " NUMBER_OF_TRIALS" << std::endl;
  } else {
    // Строки для проверки ошибок
    char *elements_err;
    char *trials_err;
    int num_elem;
    int num_trials;
    errno = 0;
    long dataset_elements = strtol(argv[1], &elements_err, 10);
    long trials = strtol(argv[2], &trials_err, 10);

    // Валидация аргументов
    if (errno != 0 || *elements_err != '\0' || dataset_elements > 5000000 || dataset_elements <= 0) {
      throw std::invalid_argument("Number of elements must be greater than zero and not greater than 5000000");
    } else {
      num_elem = dataset_elements;
    }

    if (errno != 0 || *trials_err != '\0' || trials <= 0) {
      throw std::invalid_argument("Number of trials must be greater than zero");
    } else {
      num_trials = trials;
    }

    const auto path = string(kDatasetPath);
    const auto path_project = string(kProjectPath);
    ofstream output_file;
    string str_num_elem;
    str_num_elem = to_string(num_elem);
    DisjointSet* d_set;
    output_file = ofstream(path_project + "/benchmark/test_results/make_set_multiple/" + str_num_elem + ".csv");

    double average = 0;

    // Делаем нужное количество попыток
    for (int i = 1; i <= num_trials; i++) {
      double result = 0;

      // Создаем свежую структуру данных
      d_set = new DisjointSet(0);

      // Делаем нужное количество добавлений
      // Считаем общее время выполнения <num_elem> добавлений
      for (int j = 0; j < num_elem; j++) {
        const auto time_point_before = chrono::high_resolution_clock::now();
        d_set->make_set();
        const auto time_point_after = chrono::high_resolution_clock::now();
        const auto time_diff = time_point_after - time_point_before;
        const long time_elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(time_diff).count();
        result += time_elapsed_ns;
      }

      // Выводим результат, очищаем структуру данных
      result = result/1000000;
      average += result;
      output_file << result << endl;
      delete d_set;
    }

    // Выводим среднее значение, закрываем текущий output
    output_file << "Average: " << average/num_trials;
    output_file.close();
  }

  return 0;
}
