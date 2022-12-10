#include <iostream>
#include <vector>

using namespace std;

void quickSort(int *data, int start, int end) {
  if (start >= end) return;
  int pivot = start;
  int i = start + 1;
  int j = end;

  while (i <= j) {
    while (data[i] <= data[pivot])
      i++;
    while (data[j] >= data[pivot] && j > start)
      j--;
    if (i > j)
    {
      int temp = data[j];
      data[j] = data[pivot];
      data[pivot] = temp;
    } else {
      int temp = data[j];
      data[j] = data[i];
      data[i] = temp;
    }

    quickSort(data, start, j - 1);
    quickSort(data, j + 1, end);
  }
}

int main() {
  int data[7] = {38,27,43,9,3,82,10};
  int len = 7;
  quickSort(data, 0, len - 1);

  for (int i = 0; i < len; i++) cout << data[i] << ' ';

  return 0;
}