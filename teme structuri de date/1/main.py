import copy
from time import time
import numpy as np

def heapify(i, arr, n):
    largest =i
    l =2*i+1
    r =2*i+2
    if l < n and arr[largest] < arr[l]:
        largest = l
    if r < n and arr[largest] < arr[r]:
        largest = r
    if largest != i:
        aux=arr[i]
        arr[i]=arr[largest]
        arr[largest]=aux
        heapify(largest, arr, n)

def buildMaxHeap(arr):
    for i in range(len(arr) // 2 - 1, -1, -1):
        heapify(i, arr, len(arr))

def heapSort(arr):
    buildMaxHeap(arr)
    for i in range(len(arr) - 1, 0, -1):
        aux=arr[i]
        arr[i]=arr[0]
        arr[0]=aux
        heapify(0, arr, i)

def selectSort(arr):
    for i in range(len(arr)):
        min_index = i
        for j in range(i, len(arr)):
            min_index = j if arr[j] < arr[min_index] else min_index
        aux = arr[i]
        arr[i] = arr[min_index]
        arr[min_index] = aux

def test():
    aux = np.random.permutation(1000000)

    aux2 = copy.deepcopy(aux)
    print("Select sort")
    start_time_select = time()

    selectSort(aux2)
    end_time_select = time()
    print(aux2)

    print(end_time_select - start_time_select)

    print(aux)
    start_time_heap = time()

    heapSort(aux)
    end_time_heap = time()
    print(aux)

    print(end_time_heap - start_time_heap)

test()