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
auto find_equiv(FwIter start, FwIter stop, Compare &&cmp = std::less<>())
    -> std::pair<FwIter, FwIter> {
  auto cmp2 = [&cmp](FwIter itr1, FwIter itr2) -> bool {
    return cmp(*itr1, *itr2);
  };
  auto S = std::set<FwIter, decltype(cmp2)>(cmp2); // initially empty
  auto res = std::pair<FwIter, FwIter>(stop, stop);
  for (auto itr = start; itr != stop; ++itr) {
    auto itr2 = S.find(itr); // <
    if (itr2 != S.end()) {
      res = std::make_pair(itr, *itr2);
      break;
    } else {
      S.insert(itr);
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
