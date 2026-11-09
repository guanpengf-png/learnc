#include "list.h"

void buff_add(Buff **head, const char *name, int turns, int dmg, int atk)
{
    Buff *b = calloc(1, sizeof *b);
    if (!b) {
        return;
    }
    snprintf(b->name, sizeof b->name, "%s", name);
    b->turns = turns;
    b->damage_per_turn = dmg;
    b->atk_bonus = atk;
    b->next = *head;
    *head = b;
}

int buff_tick(Buff **head)
{
    int total = 0;
    Buff **pp = head;
    while (*pp) {
        Buff *b = *pp;
        total += b->damage_per_turn;
        if (--b->turns <= 0) {
            *pp = b->next;
            free(b);
        } else {
            pp = &b->next;
        }
    }
    return total;
}

void buff_clear(Buff **head)
{
    while (*head) {
        Buff *n = (*head)->next;
        free(*head);
        *head = n;
    }
}
