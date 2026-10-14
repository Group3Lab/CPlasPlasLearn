/* yaml_test.c —— 文件的「创建 / 读取 / 修改」三件事
 *
 * 目的：练习文件读写 + 字符串匹配（找冒号、比键名）
 * 文件格式：每行一条  key: value
 *
 * 编译运行（在 e:\C_learning 目录下）：
 *   gcc -g -Wall -Wextra exercises\yaml_demo\yaml_test.c -o build\yaml_test.exe
 *   .\build\yaml_test.exe
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define PATH "exercises/yaml_demo/config.yaml"
#define MAXLINE 256

/* 去掉字符串首尾的空白，返回新的起点（注意要返回值！） */
static char *trim(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;      /* 跳过前面的空格 */
    if (*s == '\0') return s;                          /* 整行都是空白 */
    char *end = s + strlen(s) - 1;
    while (end >= s && isspace((unsigned char)*end)) end--;   /* 从后面往回退 */
    end[1] = '\0';
    return s;
}

/* ========== 1. 创建：把几行内容写进文件 ========== */
static void create_file(void) {
    FILE *fp = fopen(PATH, "w");        /* "w"：文件不存在就新建，存在就清空重写 */
    if (fp == NULL) {                   /* 打开失败必检 */
        printf("创建失败\n");
        return;
    }

    fputs("# 务器配服置\n", fp);        /* fputs 不会自动加换行，要自己写 \n */
    fputs("host: 192.168.1.10\n", fp);
    fputs("port: 8080\n", fp);
    fputs("name: my-server\n", fp);

    fclose(fp);                         /* 必须关，缓冲区里的数据才会真正写进磁盘 */
    printf("== 创建完成：%s ==\n", PATH);
}

/* ========== 2. 读取：逐行读出来，拆成键和值 ========== */
static void read_file(void) {
    FILE *fp = fopen(PATH, "r");        /* "r"：只读，文件不存在会失败 */
    if (fp == NULL) {
        printf("打开失败（文件不存在？）\n");
        return;
    }

    char line[MAXLINE];
    printf("== 读取内容 ==\n");

    /* fgets 一次读一行；读到文件末尾返回 NULL，循环就结束 */
    while (fgets(line, sizeof(line), fp) != NULL) {
        /* fgets 会把换行符 '\n' 也读进来，先去掉，否则值末尾挂个换行 */
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

        char *s = trim(line);                    /* 去掉首尾空白 */
        if (*s == '\0') continue;                /* 空行跳过 */
        if (*s == '#')  continue;                /* 注释行跳过 */

        char *colon = strchr(s, ':');             /* 找第一个冒号 */
        if (colon == NULL) {                      /* 没找到会返回 NULL，不判就会崩 */
            printf("  [跳过非法行] %s\n", s);
            continue;
        }

        *colon = '\0';                            /* 把冒号就地改成 '\0'，切开成两半 */
        char *key = trim(s);                      /* 左半：键 */
        char *val = trim(colon + 1);              /* 右半：值 */

        printf("  键=[%s]  值=[%s]\n", key, val);  /* 这就是"匹配出键值对" */
    }

    fclose(fp);
}

/* ========== 3. 匹配查找：按 key 找 value ========== */
static void find_key(const char *want) {
    FILE *fp = fopen(PATH, "r");
    if (fp == NULL) { printf("打开失败\n"); return; }

    char line[MAXLINE];
    int found = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

        char *s = trim(line);
        if (*s == '\0' || *s == '#') continue;

        char *colon = strchr(s, ':');
        if (colon == NULL) continue;

        *colon = '\0';
        char *key = trim(s);
        char *val = trim(colon + 1);

        if (strcmp(key, want) == 0) {             /* 字符串比较用 strcmp，不能用 == */
            printf("  找到 %s = %s\n", want, val);
            found = 1;
            break;                                 /* 找到就停 */
        }
    }
    if (!found) printf("  没有 %s 这个键\n", want);

    fclose(fp);
}

/* ========== 4. 修改：改掉某一行 / 追加一行 ==========
 *
 * 文本文件没法"原地改中间"，标准做法是：
 *   读原文件 → 边读边写进临时文件（顺手把要改的那行替换掉）→ 删原文件 → 临时文件改名
 */
static void modify_file(const char *key_to_change, const char *new_value,
                        const char *key_to_add,   const char *add_value) {
    FILE *in  = fopen(PATH, "r");
    if (in == NULL) { printf("打开失败\n"); return; }

    FILE *out = fopen("exercises/yaml_demo/tmp.txt", "w");   /* 临时文件 */
    if (out == NULL) { fclose(in); printf("创建临时文件失败\n"); return; }

    char line[MAXLINE];
    int  changed = 0;

    while (fgets(line, sizeof(line), in) != NULL) {
        char copy[MAXLINE];
        strcpy(copy, line);                       /* 备份一份，因为下面要切割它 */

        size_t len = strlen(copy);
        if (len > 0 && copy[len - 1] == '\n') copy[len - 1] = '\0';

        char *s = trim(copy);
        char *colon = (*s && *s != '#') ? strchr(s, ':') : NULL;

        if (colon != NULL) {
            *colon = '\0';
            char *key = trim(s);

            if (strcmp(key, key_to_change) == 0) {
                /* 命中要改的键：写出新的一行，原行丢弃 */
                fprintf(out, "%s: %s\n", key, new_value);
                changed = 1;
                continue;
            }
        }
        fputs(line, out);                         /* 其他行原样照抄 */
    }

    /* 要改的键不存在，就在末尾补一行 */
    if (!changed) {
        fprintf(out, "%s: %s\n", key_to_change, new_value);
        printf("  原文件里没有 %s，已在末尾添加\n", key_to_change);
    }

    /* 追加一个新键 */
    if (key_to_add != NULL) {
        fprintf(out, "%s: %s\n", key_to_add, add_value);
        printf("  已追加 %s: %s\n", key_to_add, add_value);
    }

    fclose(in);
    fclose(out);

    remove(PATH);                                        /* 删原文件 */
    rename("exercises/yaml_demo/tmp.txt", PATH);         /* 临时文件顶上 */
    printf("== 修改完成 ==\n");
}

int main(void) {
    printf("\n---------- 1. 创建文件 ----------\n");
    create_file();

    printf("\n---------- 2. 读取文件 ----------\n");
    read_file();

    printf("\n---------- 3. 查找键 ----------\n");
    find_key("port");
    find_key("desc");          /* 这个键不存在 */

    printf("\n---------- 4. 修改文件 ----------\n");
    modify_file("port", "9090", "timeout", "30");   /* 改 port，追加 timeout */

    printf("\n---------- 5. 再读一次看结果 ----------\n");
    read_file();

    return 0;
}
