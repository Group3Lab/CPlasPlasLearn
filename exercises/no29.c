#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

/* ---------- 我的写法：重复元素保留一个（1→1→2 → 1→2） ---------- */
struct ListNode *deleteDuplication(struct ListNode *head)
{
    struct ListNode *after;

    /* ① 空链表或单节点：直接返回 */
    if (head == NULL || head->next == NULL)
        return head;

    after = head;

    for (struct ListNode *i = head->next; i != NULL; )
    {
        struct ListNode *next = i->next;   /* ② 先存下一个，防止 free 后丢失 */

        if (i->val == after->val)
        {
            after->next = next;            /* 跳过重复节点 */
            free(i);                       /* 释放它 */
        }
        else
        {
            after = i;                     /* after 前移 */
        }

        i = next;                          /* ③ 手动前移，不用 for 的第三段 */
    }

    return head;
}

/* ---------- 参考：重复元素一个都不留（1→2→3→3→4→4→5 → 1→2→5） ----------
 * 本题（no29）要求的就是这个版本：
 *   样例1: 1→2→3→3→4→4→5  →  1→2→5
 *   样例2: 1→1→1→2→3      →  2→3
 *
 * 与上面最大的区别：**头节点本身也可能被整段删掉**，所以必须用哨兵节点，
 * 并且返回的是 dummy.next（而不是 head）。
 * ---------------------------------------------------------------------- */
struct ListNode *deleteDuplicationAll(struct ListNode *head)
{
    struct ListNode dummy = {0, NULL};   /* 哨兵：dummy.next 才是结果链表的头 */
    struct ListNode *tail = &dummy;      /* tail = 结果链表的末尾 */

    dummy.next = head;                   /* ⚠️ 哨兵先接住整条链表 */

    struct ListNode *cur = head;

    while (cur != NULL)
    {
        /* 发现一段重复：cur->val == cur->next->val */
        if (cur->next != NULL && cur->val == cur->next->val)
        {
            int dupVal = cur->val;

            /* 把这一整段相同值的节点全部删掉 */
            while (cur != NULL && cur->val == dupVal)
            {
                struct ListNode *next = cur->next;   /* ① 先存下一个 */
                free(cur);                           /* ② 再释放 */
                cur = next;                          /* ③ 最后前移 */
            }

            tail->next = cur;   /* tail 跨过被删的整段，接到新位置 */
        }
        else
        {
            /* cur 是唯一的 → 保留它 */
            tail->next = cur;   /* 接上（防止前面删过东西后断层） */
            tail = cur;         /* tail 前移 */
            cur = cur->next;
        }
    }

    tail->next = NULL;       /* 收尾：封闭链表 */

    return dummy.next;       /* 跳过哨兵，返回真正的头（可能为 NULL） */
}