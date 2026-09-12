# 链表操作速查手册（C 语言）

> 覆盖：定义、遍历、插入、删除、哨兵节点、反转、合并、找交点、去重。
> 所有代码均以 `struct ListNode` 为例。

---

## 0. 基本定义与约定

```c
#include <stdlib.h>
#include <stddef.h>

struct ListNode {
    int val;
    struct ListNode *next;
};
```

### 三条铁律

| # | 铁律 | 说明 |
|---|------|------|
| 1 | **先存、后改、再 free、最后前移** | 删节点的顺序，颠倒就 use-after-free |
| 2 | **改结构必须用 `->`** | `p = xxx` 只动局部"标签"，对外无影响 |
| 3 | **`malloc` 的指针要原样保存** | 挪过之后 `free` 必然 `invalid pointer` |

### 命名惯例（事实标准）

| 变量名 | 角色 |
|--------|------|
| `head` | 链表头（入口） |
| `cur` / `p` | 当前遍历节点 |
| `prev` / `pre` | 前驱节点 |
| `tail` | 结果链表的**末尾** |
| `dummy` | 哨兵（哑结点），不存数据 |
| `next` / `tmp` | 临时保存下一个节点 |

---

## 1. 遍历

```c
/* 打印所有节点 */
for (struct ListNode *p = head; p != NULL; p = p->next)
    printf("%d ", p->val);
```

**要小心**：
- 循环条件用 `p != NULL`，**不要**用 `p->next != NULL`（会漏掉最后一个）
- 循环体内如果需要 `free(p)`，不能再用 `p = p->next`（见铁律 1）

---

## 2. 哨兵节点（dummy / 哑结点）

### 是什么

一个**栈上的临时节点**，它的 `next` 指向结果链表的头。本身不存有效数据。

```c
struct ListNode dummy;
struct ListNode *tail = &dummy;   /* tail = 结果链表末尾 */
dummy.next = NULL;
```

### 为什么用

**不用哨兵时**，插入/删除**第一个节点**要特判：

```c
/* ❌ 无哨兵：头节点要单独处理 */
if (head == NULL) {
    head = newNode;                 /* 空链表：新节点就是头 */
} else {
    ...                             /* 非空：接到尾部 */
}
```

**用哨兵后**，空链表/非空链表统一处理：

```c
/* ✅ 有哨兵：永远不需要特判 */
tail->next = newNode;
tail = tail->next;
return dummy.next;                  /* 最后跳过哨兵返回 */
```

### 三种哨兵写法对比

| 写法 | 代码 | 优点 | 缺点 |
|------|------|------|------|
| **栈上结构体**（推荐） | `struct ListNode dummy;` | 不用 `free`、不会泄漏 | 只在函数栈内有效 |
| 栈上+指针别名 | `struct ListNode ans; struct ListNode *p = &ans;` | 同上，命名灵活 | 同上 |
| `malloc` 申请 | `malloc(sizeof *dummy)` | 生命周期长 | 需判 `NULL`、需初始化、**需 `free`**（易泄漏） |

```c
/* 推荐模板 */
struct ListNode dummy = {0, NULL};   /* val 无所谓，next 必须初始化 */
struct ListNode *tail = &dummy;

/* ... 用 tail->next 挂载 ... */

return dummy.next;
```

> ⚠️ **`return dummy.next` 是安全的**：返回的是堆节点的地址，不是栈上 `dummy` 的地址。
> ⚠️ 但如果写 `return &dummy;` 就是**悬空指针**（返回栈地址）。

---

## 3. 插入节点

### 3.1 头插（O(1)）

```c
struct ListNode *pushFront(struct ListNode *head, int val)
{
    struct ListNode *node = malloc(sizeof *node);
    if (node == NULL) return head;

    node->val  = val;
    node->next = head;      /* 新节点指向原来的头 */
    return node;            /* 新节点成为新的头 */
}
```

> **注意必须返回新头**，因为 `head` 本身变了。或者用**二级指针**：
> ```c
> void pushFront(struct ListNode **head, int val)
> {
>     struct ListNode *node = malloc(sizeof *node);
>     node->val = val;
>     node->next = *head;
>     *head = node;       /* ← 真的改了调用者的指针 */
> }
> ```

### 3.2 尾插（O(n)，除非维护 tail）

```c
/* 有 tail 指针时 O(1) */
tail->next = node;
tail = node;
node->next = NULL;
```

**用哨兵 + tail 的完整尾插模板**：

```c
struct ListNode dummy = {0, NULL};
struct ListNode *tail = &dummy;

while (还有数据) {
    struct ListNode *node = malloc(sizeof *node);
    node->val = 值;
    node->next = NULL;

    tail->next = node;      /* 接到末尾 */
    tail = node;            /* tail 前移 */
}
return dummy.next;
```

### 3.3 在指定节点后插入

```c
/* 在 prev 之后插入 val */
struct ListNode *node = malloc(sizeof *node);
node->val  = val;
node->next = prev->next;    /* ① 先接后面（顺序很重要！） */
prev->next = node;          /* ② 再让 prev 指向新节点 */
```

> **顺序不能反**：如果先写 `prev->next = node`，`prev->next` 原来的值就丢失了，
> 新节点后面接不上原本的链表。

---

## 4. 删除节点

### 4.1 通用模板（⭐ 最重要）

```c
struct ListNode *next = cur->next;   /* ① 先存下一个 */
/* -------- 改指针（接上前驱） -------- */
free(cur);                           /* ② 再释放 */
cur = next;                          /* ③ 最后前移 */
```

**为什么顺序不能乱**：`free(cur)` 之后 `cur->next` 是**未定义行为**（内存已归还）。

### 4.2 删除所有值为 `target` 的节点（用哨兵）

```c
struct ListNode dummy = {0, NULL};
dummy.next = head;
struct ListNode *prev = &dummy;

while (prev->next != NULL)
{
    if (prev->next->val == target)
    {
        struct ListNode *target_node = prev->next;
        prev->next = target_node->next;   /* ① 跳过它 */
        free(target_node);                /* ② 释放 */
        /* prev 不动：因为新的 prev->next 可能也是 target */
    }
    else
    {
        prev = prev->next;                /* 不是目标，前移 */
    }
}
return dummy.next;
```

**关键点**：
- 用哨兵后，**删头节点不需要特判**；
- 删完**不要前移 `prev`**，因为 `prev->next` 换了新节点，还要继续检查。

### 4.3 删除重复元素

**题目 A：重复元素保留一个**（`1→1→2` → `1→2`）

```c
/* 不需要哨兵：至少保留第一个，head 不会被删 */
struct ListNode *after = head;
for (struct ListNode *i = head->next; i != NULL; )
{
    struct ListNode *next = i->next;
    if (i->val == after->val)
    {
        after->next = next;    /* 跳过重复的 */
        free(i);
    }
    else
    {
        after = i;             /* after 前移 */
    }
    i = next;
}
return head;
```

**题目 B：重复元素一个不留**（`1→2→3→3→4→4→5` → `1→2→5`）

```c
/* ⚠️ 必须用哨兵：head 可能整段被删掉 */
struct ListNode dummy = {0, NULL};
struct ListNode *tail = &dummy;
dummy.next = head;                  /* ⚠️⭐ 千万别漏！哨兵要先接住整条链表 */
struct ListNode *cur = head;

while (cur != NULL)
{
    if (cur->next != NULL && cur->val == cur->next->val)
    {
        int dupVal = cur->val;
        /* 把这一段全部删掉 */
        while (cur != NULL && cur->val == dupVal)
        {
            struct ListNode *next = cur->next;
            free(cur);
            cur = next;
        }
        tail->next = cur;      /* tail 跨过整段，接到新位置 */
    }
    else
    {
        tail->next = cur;      /* ⚠️ 也要接！否则后续节点永远挂不上 */
        tail = cur;            /* 保留，tail 前移 */
        cur  = cur->next;
    }
}
tail->next = NULL;             /* 收尾封闭 */
return dummy.next;
```

> **易错点 1**：漏掉 `dummy.next = head;` → 若链表开头没有重复段，结果直接是 `NULL`。
> **易错点 2**：`else` 分支只写 `tail = cur;` 而不写 `tail->next = cur;` → 只挪游标没接上，链表断层。
> **记法**：`tail = cur` 是"挪游标"，`tail->next = cur` 才是"接线"。**每次挪游标前必须接线。**

### 4.4 删除"当前节点"但没有前驱（LeetCode 237）

**技巧：把后继的值抄过来，再删后继。**

```c
void deleteNode(struct ListNode *node)
{
    struct ListNode *tmp = node->next;  /* 要删的是后继 */
    node->val  = tmp->val;              /* ① 抄值 */
    node->next = tmp->next;             /* ② 跳过 */
    free(tmp);                          /* ③ 释放后继 */
}
```

> 前提：`node` **不是尾节点**（题目保证）。

---

## 5. 反转链表

### 5.1 迭代法（三指针，O(n) / O(1)）

```c
struct ListNode *reverseList(struct ListNode *head)
{
    struct ListNode *prev = NULL;
    struct ListNode *cur  = head;

    while (cur != NULL)
    {
        struct ListNode *next = cur->next;  /* ① 先存下一个 */
        cur->next = prev;                   /* ② 反转指向 */
        prev = cur;                         /* ③ prev 前移 */
        cur  = next;                        /* ④ cur 前移 */
    }

    return prev;    /* 结束时 prev 是新头 */
}
```

**为什么不需要特判**：
- `head == NULL` → 循环不进 → 返回 `NULL` ✅
- 单节点 → 循环一次 → 返回 `head` ✅

### 5.2 递归法（O(n) 栈空间）

```c
struct ListNode *reverseListRec(struct ListNode *head)
{
    if (head == NULL || head->next == NULL)
        return head;                        /* 递归基：空或单节点 */

    struct ListNode *newHead = reverseListRec(head->next);
    head->next->next = head;                /* 让后继指向自己 */
    head->next = NULL;                      /* 自己变成尾 */
    return newHead;
}
```

> 递归深度 = 链表长度，长链表会**爆栈**。实际优先用迭代法。

---

## 6. 合并两个有序链表

```c
struct ListNode *merge(struct ListNode *l1, struct ListNode *l2)
{
    struct ListNode dummy = {0, NULL};
    struct ListNode *tail = &dummy;

    while (l1 != NULL && l2 != NULL)
    {
        if (l1->val <= l2->val)     /* <= 保证稳定 */
        {
            tail->next = l1;
            l1 = l1->next;
        }
        else
        {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;          /* 接完统一前移 */
    }

    tail->next = (l1 != NULL) ? l1 : l2;   /* 接上剩余的整条 */

    return dummy.next;
}
```

**要点**：
- **两个循环**代替一堆 `if`：主循环处理"两条都有"，收尾一句挂上剩余；
- `tail = tail->next` 提到 `if/else` **外面**，避免重复；
- **复用原节点**，不 `malloc` 新节点。

---

## 7. 找两个链表的第一个公共节点

### 7.1 双指针"走对方的路"（最优，O(1) 空间）

```c
struct ListNode *findFirstCommonNode(struct ListNode *headA, struct ListNode *headB)
{
    struct ListNode *p = headA;
    struct ListNode *q = headB;

    while (p != q)
    {
        p = (p == NULL) ? headB : p->next;   /* 走完 A 换到 B */
        q = (q == NULL) ? headA : q->next;   /* 走完 B 换到 A */
    }

    return p;   /* 相遇点即答案；无公共节点时为 NULL */
}
```

**原理**：$a+c+b = b+c+a$，两个指针总步数相同，必然同时到达交点。

### 7.2 哈希表标记（通用但费空间）

思路：把 A 的所有节点**地址**存入哈希表，再扫 B 找第一个命中的。
C 语言需手写哈希表（约 60 行），空间 O(a)，与上面的 O(1) 相比不划算。

### 7.3 长度差法

先各算一次长度，让长的先走 $|a-b|$ 步，再同步前进。代码较长，但思路直白。

---

## 8. 快慢指针（找中点 / 判环 / 找环入口）

### 8.1 找中点

```c
struct ListNode *slow = head, *fast = head;
while (fast != NULL && fast->next != NULL)
{
    slow = slow->next;
    fast = fast->next->next;
}
/* slow 即中点（偶数个节点时指向后半段第一个） */
```

### 8.2 判断是否有环（Floyd 判圈）

```c
int hasCycle(struct ListNode *head)
{
    struct ListNode *slow = head, *fast = head;
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return 1;    /* 相遇 → 有环 */
    }
    return 0;
}
```

### 8.3 找环入口

```c
struct ListNode *detectCycle(struct ListNode *head)
{
    struct ListNode *slow = head, *fast = head;
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
        {
            /* 相遇后，一个从头走，一个从相遇点走，同速前进必在入口相遇 */
            struct ListNode *p = head;
            while (p != slow) { p = p->next; slow = slow->next; }
            return p;
        }
    }
    return NULL;
}
```

---

## 9. 内存管理检查表

| 检查项 | 说明 |
|--------|------|
| `malloc` 后判 `NULL` | `if (p == NULL) return NULL;` |
| `sizeof` 用对 | `malloc(sizeof *p)`，不要写 `sizeof(struct ListNode *)` |
| 字符串要 `+1` | `malloc(len + 1)` 放 `'\0'` |
| 释放顺序 | 先存后继，再 `free` |
| 不要 `free` 栈变量 | `free(&dummy)` ❌ 崩溃 |
| 不要 `free` 偏移后的指针 | `free(base + n)` ❌ 必须 `free(base)` |
| 改结构时保住原指针 | 让 `base` 始终指向 `malloc` 的返回值 |

### 调试利器

```bash
gcc -fsanitize=address -g -Wall -Wextra main.c -o main.exe
```

AddressSanitizer 能精确报出：**越界、use-after-free、double free、内存泄漏**。

---

## 10. 常见 Bug 速查

| 症状 | 原因 |
|------|------|
| `free(): invalid pointer` | ① 越界写坏了堆元数据 ② `free` 了偏移后的指针 ③ `free` 了栈变量 |
| `free(): double free` | 同一个节点放了两次 |
| 输出只有最后一个节点 | `tail->next` 反复覆盖同一个节点，忘了 `tail = tail->next` |
| 段错误（Segmentation fault） | 野指针写入（未初始化的指针 `p->next = ...`） |
| 结果链表断裂 | 忘了接剩余部分（`tail->next = l1 ? l1 : l2;`） |
| 丢失节点 | 改 `->next` 前没先保存原值 |
| 返回 `NULL` 但应该有结果 | 返回了循环结束后已变成 `NULL` 的指针（应该返回 `prev`/`t2`） |

---

## 11. 万能模板：构建一条结果链表

```c
struct ListNode *buildXXX(struct ListNode *head)
{
    struct ListNode dummy = {0, NULL};      /* ① 哨兵 */
    struct ListNode *tail = &dummy;         /* ② 尾指针 */

    for (struct ListNode *p = head; p != NULL; p = p->next)
    {
        /* 判断：这个节点要不要保留？ */
        if (/* 保留条件 */)
        {
            tail->next = p;                 /* ③ 挂到末尾 */
            tail = tail->next;              /* ④ 尾指针前移 */
        }
        /* 不保留：什么都不做，p 自然被跳过 */
    }

    tail->next = NULL;                      /* ⑤ 收尾，防止挂着旧尾巴 */
    return dummy.next;                      /* ⑥ 跳过哨兵 */
}
```

> 这个模板可以套用：**合并链表、去重、划分链表、奇偶重排、删除指定元素**……
> 记住它，链表题能解决一大半。

---

## 12. 复杂度速查

| 操作 | 数组 | 链表 |
|------|------|------|
| 随机访问第 k 个 | O(1) | O(n) |
| 头部插入 | O(n) | **O(1)** |
| 尾部插入 | O(1)* | O(n) / O(1)** |
| 中间插入/删除（已知位置） | O(n) | **O(1)** |
| 查找 | O(n) | O(n) |
| 内存布局 | 连续 | 分散 |
| 额外空间 | 无 | 每个节点一个指针 |

\* 需预留空间；\*\* 维护 `tail` 指针时为 O(1)
