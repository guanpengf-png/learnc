#include <stdio.h>
double price = 200.0;
void test01()
{
  printf("%.2f \n", price);
}
void test2()
{
  price = 250.0;
  printf("%.2f \n", price);
}
int main()
{
  printf("%.2f \n", price); // 200.00
  test01(); // 200.00
  test2(); // 250.00
  test01(); // 250.00
  return 0;
}
