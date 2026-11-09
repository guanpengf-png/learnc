/* 第 34 章：buff/debuff 单链表 + 二级指针删除 */
#ifndef DS_LIST_H
#define DS_LIST_H

#include "../common.h"

typedef struct Buff {
    char name[24];
    int turns;
    int damage_per_turn;
    int atk_bonus;
    struct Buff *next;
} Buff;

void buff_add(Buff **head, const char *name, int turns, int dmg, int atk);
int buff_tick(Buff **head); /* 结算并删过期，返回总伤害 */
void buff_clear(Buff **head);

#endif
