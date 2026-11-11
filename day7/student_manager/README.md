# 学生成绩管理系统（结构体 + 链表 + 文件）

## 运行

```bash
cd day7/student_manager
make
./student
```

数据默认保存在当前目录的 `students.txt`。

## 菜单

| 选项 | 功能 |
|------|------|
| 1 | 添加（头插） |
| 2 | 删除（含删头节点，二级指针） |
| 3 | 修改 |
| 4 | 按学号查询 |
| 5 | 显示全部 |
| 6 | 保存到文件 |
| 7 | 从文件加载 |
| 0 | 退出并自动保存 |

姓名请不要包含空格（文件格式用空格分隔字段）。

## 知识点

| 内容 | 函数 |
|------|------|
| 结构体节点 | `Student` |
| 查找 | `find_by_id` |
| 增加 / 头插 | `list_add(Student **head, ...)` |
| 删除 | `list_delete` |
| 修改 | `list_update` |
| 遍历 | `list_print_all` |
| 释放整表 | `list_free` |
| 文件 | `list_save` / `list_load` |
