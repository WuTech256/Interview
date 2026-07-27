/*
 * Circular Queue (Ring Buffer) - Interview Version
 *
 * Ý tưởng:
 * - Sử dụng mảng cố định để lưu dữ liệu.
 * - front: vị trí đọc (dequeue)
 * - rear : vị trí ghi (enqueue)
 * - Khi tới cuối mảng sẽ quay lại đầu bằng phép modulo.
 *
 * Ví dụ capacity = 5
 *
 * Index:
 *   0   1   2   3   4
 * +---+---+---+---+---+
 * |10 |20 |30 |   |   |
 * +---+---+---+---+---+
 *   ^           ^
 * front       rear
 *
 * rear luôn trỏ tới vị trí sẽ ghi tiếp theo.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int *buffer;     // vùng nhớ lưu dữ liệu
    int capacity;    // kích thước tối đa

    int front;       // vị trí đọc
    int rear;        // vị trí ghi

    int size;        // số phần tử hiện tại

} CircularQueue;

/* Khởi tạo queue */
void initQueue(CircularQueue *q, int capacity)
{
    q->buffer = (int *)malloc(sizeof(int) * capacity);

    q->capacity = capacity;
    q->front = 0;
    q->rear = 0;
    q->size = 0;
}

/* Queue rỗng ? */
int isEmpty(CircularQueue *q)
{
    return q->size == 0;
}

/* Queue đầy ? */
int isFull(CircularQueue *q)
{
    return q->size == q->capacity;
}

/*
 * ENQUEUE
 *
 * B1. Ghi dữ liệu vào vị trí rear
 * B2. Dịch rear sang vị trí tiếp theo
 * B3. Nếu tới cuối mảng thì quay vòng về đầu
 */
void enqueue(CircularQueue *q, int value)
{
    if (isFull(q))
    {
        printf("Queue Full\n");
        return;
    }

    q->buffer[q->rear] = value;

    q->rear = (q->rear + 1) % q->capacity;

    q->size++;
}

/*
 * DEQUEUE
 *
 * B1. Đọc dữ liệu tại front
 * B2. Dịch front sang phần tử kế tiếp
 * B3. Giảm size
 */
int dequeue(CircularQueue *q)
{
    if (isEmpty(q))
    {
        printf("Queue Empty\n");
        return -1;
    }

    int value = q->buffer[q->front];

    q->front = (q->front + 1) % q->capacity;

    q->size--;

    return value;
}

/* Xem phần tử đầu hàng đợi */
int peek(CircularQueue *q)
{
    if (isEmpty(q))
    {
        return -1;
    }

    return q->buffer[q->front];
}

/* In queue theo thứ tự logic */
void printQueue(CircularQueue *q)
{
    printf("Queue: ");

    for (int i = 0; i < q->size; i++)
    {
        int idx = (q->front + i) % q->capacity;
        printf("%d ", q->buffer[idx]);
    }

    printf("\n");
}

int main(void)
{
    CircularQueue q;

    initQueue(&q, 5);

    printf("=== ENQUEUE ===\n");

    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);

    printQueue(&q);

    printf("front value = %d\n\n", peek(&q));

    printf("=== DEQUEUE ===\n");

    printf("removed = %d\n", dequeue(&q));

    printQueue(&q);

    printf("\n=== WRAP AROUND ===\n");

    enqueue(&q, 40);
    enqueue(&q, 50);
    enqueue(&q, 60);

    printQueue(&q);

    /*
     * Lúc này rear có thể đã quay từ cuối mảng
     * về lại đầu mảng nhờ công thức:
     *
     * rear = (rear + 1) % capacity
     */

    free(q.buffer);

    return 0;
}

/*
 * Interview Notes
 * -----------------------------------------------------------------
 * Time Complexity
 *
 * enqueue : O(1)
 * dequeue : O(1)
 * peek    : O(1)
 *
 * Follow-up Question
 *
 * Không dùng biến size được không?
 *
 * Có.
 *
 * Embedded thường dùng:
 *
 *   front == rear
 *
 * => Queue Empty
 *
 *   (rear + 1) % capacity == front
 *
 * => Queue Full
 *
 * Cách này tiết kiệm RAM và thường thấy trong:
 * - UART Ring Buffer
 * - DMA Circular Buffer
 * - RTOS Stream Buffer
 * - Audio Buffer
 * - Network Packet Queue
 */
