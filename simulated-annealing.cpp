#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

const int n = 100;
static bool field[n][n];

bool check(int i0, int j0) {
  for (int i1 = 0; i1 < n; ++i1) {
    for (int j1 = 0; j1 < n; ++j1) {
       if ((i1 != i0 || j1 != j0) && field[i1][j1]) {
        int i2 = 2 * i1 - i0, j2 = 2 * j1 - j0;
        if (0 <= i2 && i2 < n && 0 <= j2 && j2 < n && field[i2][j2]) {
          return false;
        }
        i2 = 2 * i0 - i1, j2 = 2 * j0 - j1;
        if (0 <= i2 && i2 < n && 0 <= j2 && j2 < n && field[i2][j2]) {
          return false;
        }
        if ((i0 + i1) % 2 == 0 && (j0 + j1) % 2 == 0) {
          i2 = (i0 + i1) / 2, j2 = (j0 + j1) / 2;
          if (0 <= i2 && i2 < n && 0 <= j2 && j2 < n && field[i2][j2]) {
            return false;
          }
        }
      }
    }
  }
  return true;
}

int main() {
  srand(time(0));
  int cnt = 0;
  for (double Temp = 10; Temp > 0.1; Temp /= 1.0000001) {
    int i = rand() % n, j = rand() % n;
    field[i][j] ^= true;
    int new_cnt = cnt + (field[i][j] ? 1 : -1);
    if (field[i][j] && !check(i, j)) {
      field[i][j] = false;
      continue;
    }
    bool print = rand() % 1000 == 0;
    if (print) {
      std::cout << "Temp " << Temp << " cnt " << cnt << " new_cnt " << new_cnt << " ";
    }
    if (new_cnt > cnt || rand() % 1000000 + 1 < exp((new_cnt - cnt) / Temp) * 1e6) {
      cnt = new_cnt;
      if (print) {
        std::cout << "accept" << std::endl;
      }
    } else {
      field[i][j] ^= true;
      if (print) {
        std::cout << "reject" << std::endl;
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
