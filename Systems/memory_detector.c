#include <stdio.h>
#include <stdlib.h>

typedef struct MemNode
{
    void *ptr;
    size_t size;

    const char *file;
    int line;

    struct MemNode *next;

} MemNode;

static MemNode *g_head = NULL;

/*=========================================================
 * Add allocation record
 *========================================================*/
static void add_record(
    void *ptr,
    size_t size,
    const char *file,
    int line)
{
    MemNode *node =
        (MemNode *)malloc(sizeof(MemNode));

    if (node == NULL)
    {
        printf("Failed to create tracking node\n");
        return;
    }

    node->ptr = ptr;
    node->size = size;
    node->file = file;
    node->line = line;

    node->next = g_head;
    g_head = node;
}

/*=========================================================
 * Remove allocation record
 *========================================================*/
static int remove_record(void *ptr)
{
    MemNode **curr = &g_head;

    while (*curr)
    {
        if ((*curr)->ptr == ptr)
        {
            MemNode *tmp = *curr;

            *curr = (*curr)->next;

            free(tmp);

            return 1;
        }

        curr = &((*curr)->next);
    }

    return 0;
}

/*=========================================================
 * Custom malloc
 *========================================================*/
void *dbg_malloc(
    size_t size,
    const char *file,
    int line)
{
    void *ptr = malloc(size);

    if (ptr == NULL)
    {
        return NULL;
    }

    add_record(
        ptr,
        size,
        file,
        line);

    return ptr;
}

/*=========================================================
 * Custom free
 *========================================================*/
void dbg_free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    if (remove_record(ptr) == 0)
    {
        printf(
            "[ERROR] Invalid or Double Free : %p\n",
            ptr);

        return;
    }

    free(ptr);
}

/*=========================================================
 * Leak Report
 *========================================================*/
void dump_memory_leaks(void)
{
    MemNode *curr = g_head;

    size_t totalLeak = 0;

    printf("\n");
    printf("=================================\n");
    printf(" MEMORY LEAK REPORT\n");
    printf("=================================\n");

    if (curr == NULL)
    {
        printf("No Memory Leak Found.\n");
        return;
    }

    while (curr)
    {
        printf(
            "LEAK -> PTR=%p SIZE=%zu bytes FILE=%s LINE=%d\n",
            curr->ptr,
            curr->size,
            curr->file,
            curr->line);

        totalLeak += curr->size;

        curr = curr->next;
    }

    printf("---------------------------------\n");
    printf("Total Leak = %zu bytes\n",
           totalLeak);
    printf("=================================\n");
}

/*=========================================================
 * Macros
 *========================================================*/
#define MALLOC(sz) \
    dbg_malloc(sz, __FILE__, __LINE__)

#define FREE(ptr) \
    dbg_free(ptr)

/*=========================================================
 * Example functions
 *========================================================*/
void leak_example(void)
{
    char *buf = MALLOC(100);

    (void)buf;

    /* intentionally no free */
}

void normal_example(void)
{
    char *buf = MALLOC(50);

    FREE(buf);
}

void double_free_example(void)
{
    char *buf = MALLOC(30);

    FREE(buf);

    FREE(buf);
}

/*=========================================================
 * Main
 *========================================================*/
int main(void)
{
    normal_example();

    leak_example();

    double_free_example();

    dump_memory_leaks();

    return 0;
}
