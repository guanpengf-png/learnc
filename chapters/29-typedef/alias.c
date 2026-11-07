/*
 * 章节：typedef
 * 知识点：
 *   typedef 给已有类型起别名，不产生新类型，也不影响运行速度，只是写起来短、读起来清楚。
 *   别名习惯首字母大写，和系统的 int、char 区分开。
 *   可以给基本类型、指针、数组、结构体、共用体起别名。
 *   typedef struct { ... } 名字; 可以省掉结构体原来的标签，以后直接用别名声明变量。
 *   typedef int *IntPtr 之后，IntPtr p 是指针。这一点从名字上不一定看得出来，阅读时要回到 typedef。
 *   typedef int Array[5] 里的 Array 是“5 个 int 的数组”这个类型。Array a 就是 int a[5]。
 */
#include <stdio.h>

typedef int Integer;
typedef char *String;

typedef struct {
    int id;
    String name;
    String desc;
} User;

typedef int FiveInt[5];
typedef int (*IntArrayPointer)[3];

int main(void)
{
    Integer a = 10;
    Integer b = 20;
    printf("a + b = %d\n", a + b);

    User user;
    user.id = 101;
    user.name = "小甜甜";
    user.desc = "哈哈,我是一个帅气的男人";
    printf("id = %d, name = %s, desc = %s\n", user.id, user.name, user.desc);

    FiveInt five = {10, 20, 30, 40, 50};
    printf("数组别名的第一个元素 = %d\n", five[0]);

    int arr[3] = {10, 20, 30};
    IntArrayPointer ptr = &arr;
    printf("数组指针别名读到 %d\n", (*ptr)[1]);
    return 0;
}
