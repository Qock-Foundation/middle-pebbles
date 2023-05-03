#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>

const int n = 600;
static bool field[n][n];

bool check(int i0, int j0) {
  for (int i1 = 0; i1 < n; ++i1) {
    for (int j1 = 0; j1 < n; ++j1) {
      if (i1 == i0 && j1 == j0 || !field[i1][j1]) {
        continue;
      }
      int i2 = 2 * i1 - i0, j2 = 2 * j1 - j0,
          i3 = 2 * i0 - i1, j3 = 2 * j0 - j1;
      if (0 <= i2 && i2 < n && 0 <= j2 && j2 < n && field[i2][j2] && 0 <= i3 && i3 < n && 0 <= j3 && j3 < n && field[i3][j3]) {
        return false;
      }
      // 5 3 0 1 2 4
      int i4 = 2 * i2 - i1, j4 = 2 * j2 - j1,
          i5 = 2 * i3 - i0, j5 = 2 * j3 - j0;
      if (0 <= i2 && i2 < n && 0 <= j2 && j2 < n && field[i2][j2] && 0 <= i4 && i4 < n && 0 <= j4 && j4 < n && field[i4][j4]) {
        return false;
      }
      if (0 <= i3 && i3 < n && 0 <= j3 && j3 < n && field[i3][j3] && 0 <= i5 && i5 < n && 0 <= j5 && j5 < n && field[i5][j5]) {
        return false;
      }
    }
  }
  return true;
}

int main() {
  srand(time(0));
  int cnt = 0;
  for (double Temp = 10; Temp > 0.1; Temp /= 1.00000001) {
    int i = rand() % n, j = rand() % n;
    field[i][j] ^= true;
    int new_cnt = cnt + (field[i][j] ? 1 : -1);
    if (field[i][j] && !check(i, j)) {
      field[i][j] = false;
      continue;
    }
    bool print = rand() % 1000 == 0;
    if (print) {
      std::clog << "Temp " << Temp << " cnt " << cnt << " new_cnt " << new_cnt << " ";
    }
    if (new_cnt > cnt || rand() % 1000000 + 1 < exp((new_cnt - cnt) / Temp) * 1e6) {
      cnt = new_cnt;
      if (print) {
        std::clog << "accept" << std::endl;
      }
    } else {
      field[i][j] ^= true;
      if (print) {
        std::clog << "reject" << std::endl;
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << field[i][j] << " ";
    }
  }
  std::cout << std::endl;
}
