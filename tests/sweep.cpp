/*
 * Note:
 * =====
 *
 * - "Minimal" is easier than "Minimum"
 * - "Detection" is easier than "Construction"
 * - Don't hunt rabbits with a machine gun
 * - std::unordered_set<> is implemented by hash table
 * - std::set<> is implemented by a red-black tree
 */
#include <fmt/core.h>
#include <set>
#include <utility> // for pair
#include <vector>
//
struct Interval {
  int lb; /// lower bound
  int ub; /// upper bound
};

template <typename FwIter, typename Compare>
auto find_equiv(FwIter first, FwIter last, Compare &&cmp)
    -> std::pair<FwIter, FwIter> {
  auto cmp2 = [&cmp](FwIter iter1, FwIter iter2) -> bool {
    return cmp(*iter1, *iter2);
  };
  auto S = std::set<FwIter, decltype(cmp2)>(cmp2); // initially empty
  auto res = std::make_pair(last, last);
  for (auto iter = first; iter != last; ++iter) {
    auto iter2 = S.find(iter); // <
    if (iter2 != S.end()) {
      res = std::make_pair(iter, *iter2);
      break;
    } else {
      S.insert(iter);
    }
  }
  return res;
}

/*
int main() {
  auto lst = std::vector<Interval>{{3, 6}, {-4, 0}, {4, 8}, {2, 5}};
  auto cmp = [](auto &lhs, auto &rhs) -> bool { return lhs.ub < rhs.lb; };
  auto res = find_equiv(lst.begin(), lst.end(), cmp);
  if (res.first != lst.end()) {
    fmt::print("{}, {}\n", res.first->lb, res.first->ub);
    fmt::print("{}, {}\n", res.second->lb, res.second->ub);
  } else {
    fmt::print("Not found!\n");
  }
}
*/
