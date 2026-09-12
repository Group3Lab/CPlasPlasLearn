#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode *merge(struct ListNode *l1, struct ListNode *l2)
{
    struct ListNode *p1 = l1, *p2 = l2;

    struct ListNode ans;

    ans.next = NULL;      
    struct ListNode *p = &ans;

    for (; p1 != NULL || p2 != NULL;)
    {
        if (p1 == NULL)
        {
            p->next = p2;
            p2 = p2->next;
            p = p->next;

            continue;
        }
        if (p2 == NULL)
        {
            p->next = p1;
            p1 = p1->next;
            p = p->next;

            continue;
        }

        if (p1->val < p2->val)
        {
            p->next = p1;
            p1 = p1->next;
            p = p->next;
        }
        else
        {
            p->next = p2;
            p2 = p2->next;
            p = p->next;
        }
    }

    return ans.next;
}

/* ---------- Plan B（v2）：哨兵节点 + 两个循环，消除重复分支 ---------- */
struct ListNode *merge_v2(struct ListNode *l1, struct ListNode *l2)
{
    struct ListNode dummy;          /* 栈上哨兵：不用 malloc，也不用 free */
    struct ListNode *tail = &dummy; /* tail 始终指向结果链表的末尾 */
    dummy.next = NULL;

    /* 主循环：两条链表都还有节点时，取小的接上 */
    while (l1 != NULL && l2 != NULL)
    {
        if (l1->val <= l2->val)
        {
            tail->next = l1;
            l1 = l1->next;
        }
        else
        {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;          /* 接完统一往后挪一步 */
    }

    /* 收尾：把剩下那条整条挂上（最多只有一条非空） */
    tail->next = (l1 != NULL) ? l1 : l2;

    return dummy.next;              /* 哨兵的下一个才是真正的头 */
}

