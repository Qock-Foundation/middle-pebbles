import sys
import numpy as np
import matplotlib.pyplot as plt

arr = np.array(list(map(int, open(sys.argv[1]).read().split())))
n = int(np.sqrt(len(arr) + .5))
arr = arr.reshape(n, n)

plt.figure(figsize=(12, 12))
plt.imshow(arr)
plt.savefig('pic.png')
