import pre_processing_acc
import numpy as np
from time import time



def calc_grid_3d(grid_size_X = 10, grid_size_Y = 10) -> np.ndarray:
    M = grid_size_Y
    N = grid_size_X

    grid = []
    
    for m in range(M):
        for n in range(N):
            grid.append((np.sin(np.pi * m/M)*np.cos(2*np.pi* n/N), np.sin(np.pi * m/M)*np.sin(2*np.pi* n/N), np.cos(np.pi * m/M)))

    return grid



n = 10




file  = "geonames_be_smol.csv"


cities = pre_processing_acc.get_cities(file)

print(cities)




# T = np.empty(n, dtype=pre_processing_acc.coor)

# for i in range(len(T)):
#     T[i] = (i, i)

# print(T)


# s = time()
# # res = pre_processing_acc.calc_grid_3d(n, n)
# res = pre_processing_acc.calc_grid(T, n, n)

# adj = pre_processing_acc.calc_adj(T, T, 10)

# e = time()

# t1 = e - s
# print(t1)



# s = time()
# res = calc_grid_3d(n,n)
# e = time()

# t2 = e - s
# print(t2)


# print(t2/t1)




