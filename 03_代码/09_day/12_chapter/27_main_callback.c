#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// 产生随机数的函数
int get_rand(int x){
  srand((unsigned int)time(NULL)); // 种子
  return rand() % x;
}
// 翻倍的函数
int get_double(int x){
  return x * 2;
}
// 加10的函数
int get_add_num(int x){
  return x + 10;
}
// int (*ptr)(int) = get_rand;
void change_array(int *arr, int len, int (*ptr)(int)){
  for (int i = 0; i < len; i++){
    // arr[i] = rand() % arr[i];
    arr[i] = ptr(arr[i]);
  }
}
// 遍历数组的
void printf_array(int *arr, int len){
  for (int i = 0; i < len; i++)
    printf("%d ", arr[i]);
  printf("\n");
  printf("\n=================\n");
}
int main()
{
  // 回调函数:函数A作为另一个函数B的参数传递,在该函数B内部去调用这个函数A,此时这个函数A就是回调函数
  // 需求:通过调用函数,实现传入进去的数组中的数据是随机数(10个)
  // 通过调用这个函数,还可以实现数组中的所有数据翻倍
  // 通过调用这个函数,还可以实现每个数据加10运算
  int nums[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
  int len = sizeof(nums) / sizeof(int);
  printf_array(nums, len);              // 遍历数组
  change_array(nums, len, get_rand);    // 产生随机数
  printf_array(nums, len);              // 遍历数组
  change_array(nums, len, get_double);  // 翻倍数据
  printf_array(nums, len);              // 遍历数组
  change_array(nums, len, get_add_num); // 翻倍数据
  printf_array(nums, len);              // 遍历数组

  return 0;
}