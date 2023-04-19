#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

int n, m, k;

int var_cell(int i, int j) {
  assert(0 <= i && i < m);
  assert(0 <= j && j < n);
  return i * n + j + 1;
}
int cnt_vars_cell, cntsums;
int var_prefix_sum(int i, int sum) {
  assert(0 <= i && i < m * n);
  return cnt_vars_cell + i * cntsums + sum + 1;
}
int cnt_vars_prefix_sum, cnt_vars;

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage\n";
    return 1;
  }
  std::cout << "started" << std::endl;
  m = atoi(argv[1]), n = atoi(argv[2]), k = atoi(argv[3]);
  cnt_vars_cell = m * n;
  cntsums = m * n + 1;
  cnt_vars_prefix_sum = m * n * cntsums;
  cnt_vars = cnt_vars_cell + cnt_vars_prefix_sum;
  std::vector<std::vector<int>> problem;
  // main constraints
  for (int i1 = 0; i1 < m; ++i1) {
    for (int j1 = 0; j1 < n; ++j1) {
      for (int i2 = 0; i2 < m; ++i2) {
        for (int j2 = 0; j2 < n; ++j2) {
          if (i2 > i1 || i2 == i1 && j2 >= j1) {
            continue;
          }
          int i3 = 2 * i2 - i1, j3 = 2 * j2 - j1, i4 = 2 * i3 - i2, j4 = 2 * j3 - j2;
          if (0 <= i3 && i3 < m && 0 <= j3 && j3 < n && 0 <= i4 && i4 < m && 0 <= j4 && j4 < n) {
            problem.push_back({-var_cell(i1, j1), -var_cell(i2, j2), -var_cell(i3, j3), -var_cell(i4, j4)});
          }
        }
      }
    }
  }
  // prefix sums
  for (int i = 0; i < m * n; ++i) {
    problem.emplace_back();
    for (int sum = 0; sum < cntsums; ++sum) {
      problem.back().push_back(var_prefix_sum(i, sum));
    }
    for (int sum1 = 0; sum1 < cntsums; ++sum1) {
      for (int sum2 = 0; sum2 < sum1; ++sum2) {
        problem.push_back({-var_prefix_sum(i, sum1), -var_prefix_sum(i, sum2)});
      }
    }
  }
  problem.push_back({var_cell(0, 0), var_prefix_sum(0, 0)});
  problem.push_back({-var_cell(0, 0), var_prefix_sum(0, 1)});
  for (int i = 1; i < m * n; ++i) {
    for (int prev_sum = 0; prev_sum < cntsums - 1; ++prev_sum) {
      problem.push_back({-var_prefix_sum(i - 1, prev_sum), var_cell(i / n, i % n), var_prefix_sum(i, prev_sum)});
      problem.push_back({-var_prefix_sum(i - 1, prev_sum), -var_cell(i / n, i % n), var_prefix_sum(i, prev_sum + 1)});
    }
  }
  problem.push_back({var_prefix_sum(m * n - 1, k)});
  std::cout << "problem generated" << std::endl;
  std::ofstream fout("p.cnf");
  fout << "p cnf " << cnt_vars << " " << problem.size() << "\n";
  for (const std::vector<int>& clause : problem) {
    for (int literal : clause) {
      fout << literal << " ";
    }
    fout << "0\n";
  }
  fout.close();
  std::cout << "problem written" << std::endl;
}
