#include <stddef.h>   /* NULL */
#include <string.h>   /* memset */

struct ListNode {
    int val;
    struct ListNode *next;
};

/**
 * 双指针"走对方的路"：
 *   p 走 A独有 + 公共 + B独有
 *   q 走 B独有 + 公共 + A独有
 * 两者总步数相同，因此必然同时到达第一个公共节点（或同时为 NULL）。
 */
struct ListNode *findFirstCommonNode(struct ListNode *headA, struct ListNode *headB)
{
    struct ListNode *p = headA;
    struct ListNode *q = headB;

    while (p != q)
    {
        /* 走到尽头就换到对方链表的头部，继续走 */
        p = (p == NULL) ? headB : p->next;
        q = (q == NULL) ? headA : q->next;
    }

    return p;   /* 相遇点就是第一个公共节点；无公共节点时为 NULL */
}

/* ============================================================
 * 参考实现：手写哈希表（开放寻址法）
 * ============================================================
 * 思路：
 *   ① 把链表 A 的所有节点地址插入哈希表（相当于"做标记"）
 *   ② 遍历链表 B，第一个能在表中查到的节点就是第一个公共节点
 *
 * 复杂度：时间 O(a+b)，空间 O(a)
 * 注意：C 标准库没有 set/dict，所以这里手写一个最简单的哈希表。
 *       实际刷题优先用上面的双指针解法（O(1) 空间）。
 * ============================================================ */

#define HT_CAPACITY 65536   /* 必须是 2 的幂，且 > 最大节点数(30000) */

/* 哈希表：存"节点地址"的集合；用开放寻址（线性探测）解决冲突 */
typedef struct {
    struct ListNode *slots[HT_CAPACITY];  /* 每个槽存一个节点地址 */
    unsigned char used[HT_CAPACITY];      /* 该槽是否被占用 */
} AddrSet;

/* 把指针（地址）映射到槽下标：对 64 位地址做位混合，再取低位 */
static unsigned int hashPtr(const void *ptr)
{
    unsigned long long x = (unsigned long long)(size_t)ptr;
    /* 简单混合：高位右移 3 再异或低位，避免对齐导致的低位规律性 */
    x ^= (x >> 3);
    x *= 0x9E3779B97F4A7C15ULL;           /* 黄金比例乘子，打散分布 */
    x ^= (x >> 29);
    return (unsigned int)(x & (HT_CAPACITY - 1));
}

/* 插入一个地址（若已存在则忽略） */
static void addrSetAdd(AddrSet *set, struct ListNode *node)
{
    unsigned int idx = hashPtr(node);

    while (set->used[idx])
    {
        if (set->slots[idx] == node)
            return;                        /* 已存在 */
        idx = (idx + 1) & (HT_CAPACITY - 1); /* 线性探测，环形回绕 */
    }

    set->slots[idx] = node;
    set->used[idx] = 1;
}

/* 查询地址是否存在 */
static int addrSetContains(const AddrSet *set, struct ListNode *node)
{
    unsigned int idx = hashPtr(node);

    while (set->used[idx])
    {
        if (set->slots[idx] == node)
            return 1;                      /* 找到 */
        idx = (idx + 1) & (HT_CAPACITY - 1);
    }

    return 0;                              /* 遇到空槽 → 一定不存在 */
}

/**
 * 哈希表版本：把 A 的所有节点地址标记下来，再扫 B。
 * 找到的第一个"已标记"节点就是第一个公共节点。
 */
struct ListNode *findFirstCommonNodeHash(struct ListNode *headA, struct ListNode *headB)
{
    /* 注意：静态存储，避免在栈上放 64K*9 ≈ 576KB 的大数组导致栈溢出。
       若在 LeetCode 多线程环境，建议改用 malloc + memset 清零。 */
    static AddrSet set;
    memset(&set, 0, sizeof(set));

    /* ① 标记 A 的所有节点 */
    for (struct ListNode *p = headA; p != NULL; p = p->next)
        addrSetAdd(&set, p);

    /* ② 扫描 B，第一个命中的就是答案 */
    for (struct ListNode *q = headB; q != NULL; q = q->next)
        if (addrSetContains(&set, q))
            return q;

    return NULL;   /* 无公共节点 */
}