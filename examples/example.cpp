// Задача: Найти количество компонент связности в графе. Входные данные:
// Первая строчка: n m - кол-во элементов и кол-во ребер (элементы начинаются с 1)
// Следующие m строк формата a b обозначают ребро a-b (Например 1 2)
#include <iostream>
#include "data_structure.hpp"

using namespace std;
using namespace itis;

int main(int argc, char **argv) {
  int n, m;
  int num1, num2;
  cin >> n >> m;

  // Создаем "пустой" DisjointSet
  // На деле в нем создается n элементов, которые находятся в своих подмножествах и сами являются представителями
  // Значит изначально у нас n подмножеств - компонент связности
  DisjointSet* d_set = new DisjointSet(n);

  // Строим граф
  for (int i = 0; i < m; i++) {
    cin >> num1 >> num2;

    // Объединяем 2 подмножества(компоненты связности) в одно подмножество(компоненту связности)
    // Каждое успешное объединение уменьшает кол-во подмножеств(компонент связности) на 1
    d_set->join(num1-1, num2-1);
  }
  cout << "Number of connectivity components: " << d_set->subset_count << endl;
  return 0;
}