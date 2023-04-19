#include <cstdlib>
#include <ctime>
#include <iostream>
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line) {
  if (code != cudaSuccess) {
    fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
    exit(code);
  }
}

const int n = 100;

__global__ void check_kernel(const bool field[n][n], bool values[n * n]) {
  int id = blockIdx.x * blockDim.x + threadIdx.x;
  if (id >= n * n) {
    return;
  }
  values[id] = true;
  int i0 = id / n, j0 = id % n;
  if (field[i0][j0]) {
    for (int i1 = 0; i1 < n; ++i1) {
      for (int j1 = 0; j1 < n; ++j1) {
        if (i1 == i0 && j1 == j0 || !field[i1][j1]) {
          continue;
        }
        int i2 = 2 * i1 - i0, j2 = 2 * j1 - j0;
        if (0 <= i2 && i2 < n && 0 <= j2 && j2 < n && field[i2][j2]) {
          values[id] = false;
        }
      }
    }
  }
}

const int k1 = 512, k2 = 32, k12 = k1 * k2;

bool check(const bool field[n][n]) {
  static bool *values = nullptr;
  if (values == nullptr) {
    gpuErrchk(cudaMallocManaged(&values, n * n * sizeof(bool)));
  }
  check_kernel<<<k1,k2>>>(field, values);
  gpuErrchk(cudaDeviceSynchronize());
  for (int i = 0; i < n * n; ++i) {
    if (!values[i]) {
      return false;
    }
  }
  return true;
}

int main() {
  srand(time(0));
  bool (*field)[n], *values;
  gpuErrchk(cudaMallocManaged(&field, n * n * sizeof(bool)));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      field[i][j] = false;
    }
  }
  gpuErrchk(cudaMallocManaged(&values, n * n * sizeof(bool)));
  int cnt = 0;
  for (double Temp = 10; Temp > 0.1; Temp /= 1.0000001) {
    int i = rand() % n, j = rand() % n;
    field[i][j] ^= true;
    int new_cnt = cnt + (field[i][j] ? 1 : -1);
    if (field[i][j] && !check(field)) {
      field[i][j] = false;
      continue;
    }
    bool print = rand() % 10 == 0;
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
}
