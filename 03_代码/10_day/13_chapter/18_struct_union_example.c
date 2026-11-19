#include <stdio.h>
// 定义一个人的结构体类型
struct Person
{
  int id;        // 编号
  char name[20]; //
  char gender;   // 性别
  int age;       // 年龄
  char work;     // 职业
  union
  {
    double score;    // 分数
    char course[20]; // 课程信息
  } sc;
};
int main()
{
  int len = 3; // 数组长度
  // 声明一个结构体类型数组
  struct Person persons[len];
  // 遍历数组,录入数据
  for (int i = 0; i < len; i++)
  {
    printf("请您输入第%d个的编号:\n", i + 1);
    scanf("%d", &persons[i].id);
    printf("请您输入第%d个的姓名:\n", i + 1);
    scanf("%s", persons[i].name);
    getchar();   // 获取一个字符----吃掉个回车
    printf("请您输入第%d个的性别:\n", i + 1); // 坑===============
    scanf("%c", &persons[i].gender);
    printf("请您输入第%d个的年龄:\n", i + 1);
    scanf("%d", &persons[i].age);
    getchar();  // 获取一个字符----吃掉个回车
    printf("请您输入第%d个的职业:\n", i + 1); // 坑===============
    scanf("%c", &persons[i].work);
    // 根据职业信息判断是学生还是老师
    if (persons[i].work == 's')
    {
      printf("请您输入第%d个的分数:\n", i + 1);
      scanf("%lf", &persons[i].sc.score);
    }
    else
    {
      printf("请您输入第%d个的课程:\n", i + 1); //
      scanf("%s", persons[i].sc.course);
    }
  }
  printf("编号\t姓名\t性别\t年龄\t职业\t\t分数/课程:\n");
  // 遍历结构体数组
  for (int i = 0; i < len; i++)
  {
    if (persons[i].work == 's')
      printf("%d\t%s\t%c\t%d\t%c\t\t%.2lf\n", persons[i].id, persons[i].name, persons[i].gender, persons[i].age, persons[i].work, persons[i].sc.score);
    else
      printf("%d\t%s\t%c\t%d\t%c\t\t%s\n", persons[i].id, persons[i].name, persons[i].gender, persons[i].age, persons[i].work, persons[i].sc.course);
  }

  return 0;
}

// char name[20];
// name = "fsdf"; // 不行
// char *name2="abc";
// name2 = "def"; // 可以