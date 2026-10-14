/* struct_file.c —— 结构体的「存 / 读 / 改」三件事
 *
 * 用二进制文件（"wb"/"rb"）整块存结构体数组。
 * 为什么必须用二进制？先看一个坑：
 *
 *   同样写 "A\nB\n"，两种模式写出的字节数不一样：
 *     "w"  文本模式 -> 6 字节（每个 '\n' 被偷偷换成 "\r\n" 两字节）
 *     "wb" 二进制   -> 4 字节（'\n' 原样，一个字节就是一个 '\n'）
 *   结构体里只要出现 0x0A 这个字节，文本模式就会插进 0x0D，整个文件的
 *   字节偏移全部错位，fseek 定位和 fread 读回就都乱了。
 *   => 存数据一律用 "wb" / "rb"。
 *
 * 编译运行（在 e:\C_learning 目录下）：
 *   gcc -g -Wall -Wextra exercises\yaml_demo\struct_file.c -o build\struct_file.exe
 *   .\build\struct_file.exe
 */

#include <stdio.h>
#include <string.h>

#define PATH "exercises/yaml_demo/students.dat"
#define N    4        /* 学生个数 */

typedef struct {
    int   id;
    char  name[16];
    float score;
} Stu;

/* ========== 打印一批记录 ========== */
static void show(const Stu *a, int n) {
    for (int i = 0; i < n; i++)
        printf("  [%d] id=%d  name=%-8s score=%.1f\n",
               i, a[i].id, a[i].name, a[i].score);
}

/* ========== 1. 存：整块写进文件 ========== */
static void save_all(const Stu *a, int n) {
    FILE *fp = fopen(PATH, "wb");            /* "wb"：二进制写，文件不存就新建，存在就清空 */
    if (fp == NULL) { printf("open failed\n"); return; }

    /* fwrite(起始地址, 单个元素字节数, 元素个数, fp) —— 中间两个的乘积才是总字节数 */
    size_t w = fwrite(a, sizeof(Stu), n, fp);   /* 返回"实际写出的元素个数"，不是字节数 */
    printf("== saved %lu records, each %lu bytes ==\n",
           (unsigned long)w, (unsigned long)sizeof(Stu));

    fclose(fp);                              /* 必须关，缓冲区才真正落盘 */
}

/* ========== 2. 读：整块读回来 ========== */
static void load_all(void) {
    FILE *fp = fopen(PATH, "rb");
    if (fp == NULL) { printf("open failed\n"); return; }

    /* 先问出文件有多少字节，再算有多少条 —— 二进制每条等长才能这么算 */
    fseek(fp, 0L, SEEK_END);                 /* 光标移到末尾 */
    long bytes = ftell(fp);                  /* 当前偏移 = 总字节数 */
    rewind(fp);                              /* 光标回到开头，否则下面读不到东西 */

    int n = (int)(bytes / (long)sizeof(Stu));
    printf("== file %ld bytes, %d records ==\n", bytes, n);

    /* 注意：数组 a 只有 N 个位置。文件里条数超过 N 时，多出来的读不进来。
     * 真实项目不会用固定数组，而是按 n 动态申请：Stu *a = malloc(n * sizeof(Stu)); */
    Stu a[N];
    if (n > N) {
        printf("   (array holds only %d, reading first %d)\n", N, N);
        n = N;
    }
    size_t r = fread(a, sizeof(Stu), (size_t)n, fp);
    fclose(fp);

    printf("read back %lu records:\n", (unsigned long)r);
    show(a, (int)r);
}

/* ========== 3. 改：定位改写某一条 ==========
 *
 * 二进制每条等长，所以第 i 条的起始偏移 = i * sizeof(Stu)
 * 可以用 fseek 直接跳过去，不用从头遍历！
 * 注意必须用 "r+b"（读+写，文件必须已存在），不能用 "wb"（会清空整个文件）。
 */
static void update_one(int index, float new_score) {
    FILE *fp = fopen(PATH, "r+b");           /* r+ = 可读可写；加 b = 二进制 */
    if (fp == NULL) { printf("open failed (save first)\n"); return; }

    /* ① 跳到第 index 条的开头 */
    if (fseek(fp, (long)index * sizeof(Stu), SEEK_SET) != 0) {
        printf("seek failed\n");
        fclose(fp);
        return;
    }

    /* ② 读出来 */
    Stu s;
    if (fread(&s, sizeof(Stu), 1, fp) != 1) {   /* 返回值是元素个数，不是 1 就是没读到 */
        printf("record %d not found\n", index);
        fclose(fp);
        return;
    }
    printf("  before: id=%d name=%s score=%.1f\n", s.id, s.name, s.score);

    /* ③ 改内存里的值 */
    s.score = new_score;

    /* ④ 光标回退一条，把改好的整条覆盖写回去
     *    因为刚才 fread 让光标前进了 sizeof(Stu)，不退回就会写在下一条的位置上 */
    fseek(fp, (long)index * sizeof(Stu), SEEK_SET);
    fwrite(&s, sizeof(Stu), 1, fp);
    printf("  after : id=%d name=%s score=%.1f\n", s.id, s.name, s.score);

    fclose(fp);
}

/* ========== 4. 追加：在末尾加一条 ========== */
static void append_one(Stu s) {
    FILE *fp = fopen(PATH, "ab");             /* "ab"：二进制追加，写入位置永远在末尾 */
    if (fp == NULL) { printf("open failed\n"); return; }

    fwrite(&s, sizeof(Stu), 1, fp);
    fclose(fp);
    printf("== appended id=%d name=%s score=%.1f ==\n", s.id, s.name, s.score);
}

int main(void) {
    Stu data[N] = {
        {1, "Tom",   88.5f},
        {2, "Amy",   92.0f},
        {3, "Bob",   75.5f},
        {4, "Cathy", 95.0f}
    };

    printf("\n---------- 1. SAVE: write whole array ----------\n");
    save_all(data, N);

    printf("\n---------- 2. LOAD: read whole array back ----------\n");
    load_all();

    printf("\n---------- 3. UPDATE: change record[2] score ----------\n");
    update_one(2, 60.0f);

    printf("\n---------- 4. APPEND one record ----------\n");
    Stu extra = {5, "David", 81.0f};
    append_one(extra);

    printf("\n---------- 5. LOAD again to see the result ----------\n");
    load_all();

    return 0;
}
