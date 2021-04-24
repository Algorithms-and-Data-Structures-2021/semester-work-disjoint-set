#include <fstream>      // ifstream
#include <iostream>     // cout
#include <string>       // string, stoi
#include <string_view>  // string_view
#include <chrono>       // high_resolution_clock, duration_cast, nanoseconds
#include <sstream>      // stringstream
#include <regex>
#include <errno.h>      // for errno
#include <stdlib.h>     // for strtol
#include <random>

// подключаем структуру данных
#include "data_structure.hpp"

using namespace std;
using namespace itis;

// абсолютный путь до набора данных и папки проекта
static constexpr auto kDatasetPath = string_view{PROJECT_DATASET_DIR};
static constexpr auto kProjectPath = string_view{PROJECT_SOURCE_DIR};

int main(int argc, char **argv) {

  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " NUMBER_OF_ELEMENTS" << " NUMBER_OF_DATASETS" << " NUMBER_OF_TRIALS" << std::endl;
  } else {
    // Строки для проверки ошибок
    char *elements_err;
    char *datasets_err;
    char *trials_err;
    int num_elem;
    int num_datasets;
    int num_trials;
    errno = 0;
    long dataset_elements = strtol(argv[1], &elements_err, 10);
    long datasets = strtol(argv[2], &datasets_err, 10);
    long trials = strtol(argv[3], &trials_err, 10);

    // Валидация аргументов
    if (errno != 0 || *elements_err != '\0' || dataset_elements > 5000000 || dataset_elements <= 0) {
      throw std::invalid_argument("Number of elements must be greater than zero and not greater than 5000000");
    } else {
      num_elem = dataset_elements;
    }

    if (errno != 0 || *datasets_err != '\0' || datasets > 10 || datasets <= 0) {
      throw std::invalid_argument("Number of datasets must be greater than zero and not greater than 10");
    } else {
      num_datasets = datasets;
    }

    if (errno != 0 || *trials_err != '\0' || trials <= 0) {
      throw std::invalid_argument("Number of trials must be greater than zero");
    } else {
      num_trials = trials;
    }

    const auto path = string(kDatasetPath);
    const auto path_project = string(kProjectPath);
    ofstream output_file;
    ifstream join_dataset;
    string str_num_elem;
    str_num_elem = to_string(num_elem);
    DisjointSet* d_set;

    // Тестируем каждый набор данных
    for (int i = 1; i <= num_datasets; i++) {
      // Открываем файлы
      if (i < 10) {
        output_file = ofstream(path_project + "/benchmark/test_results/find_multiple/" + str_num_elem + "/0" + to_string(i) + ".csv");
        join_dataset = ifstream(path + "/join/0" + to_string(i) + "/" + str_num_elem + ".csv");
      } else {
        output_file = ofstream(path_project + "/benchmark/test_results/find_multiple/" + str_num_elem + "/" + to_string(i) + ".csv");
        join_dataset = ifstream(path + "/join/" + to_string(i) + "/" + str_num_elem + ".csv");
      }

      if (!join_dataset) {
        throw std::invalid_argument("No <join> dataset with requested amount of elements. Unable to create a disjoint set for testing.");
      }

      // Генерируем структуру данных
      d_set = new DisjointSet(num_elem + 1);
      string join_line;
      while (std::getline(join_dataset, join_line)) {
        int num1 = 0;
        int num2 = 0;
        join_line = regex_replace(join_line, regex(","), " ");
        auto create_set_ss = stringstream(join_line);
        create_set_ss >> num1;
        create_set_ss >> num2;
        d_set->join(num1, num2);
      }

      int num;
      double result;
      double average = 0;
      const auto seed = chrono::system_clock::now().time_since_epoch().count();
      auto engine = mt19937(seed);
      auto dist = uniform_int_distribution(0, num_elem - 1);

      // Замеряем время поиска <num_elem> элементов
      // Делаем нужное количество попыток
      for (int j = 0; j < num_trials; j++) {
        result = 0;
        for (int n = 0; n < num_elem; n++) {
          num = dist(engine);
          const auto time_point_before = chrono::high_resolution_clock::now();
          d_set->find(num);
          const auto time_point_after = chrono::high_resolution_clock::now();
          const auto time_diff = time_point_after - time_point_before;
          const long time_elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(time_diff).count();
          result += time_elapsed_ns;
        }
        result = result/1000000;
        output_file << result << endl;
        average += result;
      }

      // Выводим среднее значение, очищаем структуру данных, готовимся к следующему набору данных
      output_file << "Average: " << average/num_trials;
      delete d_set;
      output_file.close();
      join_dataset.close();
    }
  }

  return 0;
}
