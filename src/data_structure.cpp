#include "data_structure.hpp"

namespace itis {

  DisjointSet::DisjointSet(int n) : subset_count{n}, size{n} {
    for (int i = 0; i < n; i++) {
      elements.emplace_back(Element(i));
    }
  }

  void DisjointSet::make_set() {
    elements.emplace_back(Element(size));
    size++;
    subset_count++;
  }

  int DisjointSet::find(int node) {
    // Валидация аргумента
    if (node < 0 || node >= size) {
      throw std::invalid_argument("No such element");
    }

    // Рекурсивно поднимаемся к корню дерева, попутно подвешивая все встречающиеся вершины к корню.
    // Это помогает в следующий раз быстрее найти представителей этих элементов.
    // Этот метод называется сжатие путей.
    if (elements[node].parent != node) {
      elements[node].parent = find(elements[node].parent);
    }

    return elements[node].parent;
  }

  void DisjointSet::join(int x, int y) {
    // Находим представителей элементов
    int root_x = find(x);
    int root_y = find(y);

    // Если элементы в одном подмножестве, то объединять их не надо
    if (root_x == root_y) {
      return;
    }

    // Чтобы не увеличивать высоту дерева, мы подвешиваем мЕньшее дерево к бОлльшему.
    // А если высоты у деревьев равны, то подвешиваем к первому и увеличиваем его ранг.
    // Метод называется Union By Rank.
    if (elements[root_x].rank < elements[root_y].rank) {
      elements[root_x].parent = root_y;
    } else if (elements[root_x].rank > elements[root_y].rank) {
      elements[root_y].parent = root_x;
    } else {
      elements[root_y].parent = root_x;
      elements[root_x].rank++;
    }

    // Успешное объединение уменьшает количество подмножеств на 1.
    subset_count--;
  }

}  // namespace itis