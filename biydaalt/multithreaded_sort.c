#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // sleep() функцийн төлөө

#define SIZE 10
#define MAX_THREADS 3

int arr[SIZE] = {10, 3, 15, 7, 8, 23, 74, 18, 2, 5};
int sorted_arr[SIZE];

void *sort_half(void *arg);
void *merge(void *arg);
void print_array(int arr[], int size);

typedef struct
{
    int start_index;
    int end_index;
    int thread_id; // Утасны ID-г хадгалах
} thread_args;

int main()
{
    printf("=========================================\n");
    printf("1. Програмыг эхлүүлж, олон утасны процессийн хурдыг шалгалаа.\n");
    printf("=========================================\n\n");

    pthread_t threads[MAX_THREADS];
    thread_args args[MAX_THREADS];

    // Эхний массивыг хэвлэх
    printf("Анхны массив:\n");
    print_array(arr, SIZE);
    printf("\n");

    // Хэсэгчлэн эрэмбэлэх утаснуудын параметрүүдийг тохируулах
    args[0].start_index = 0;
    args[0].end_index = SIZE / 2 - 1;
    args[0].thread_id = 1;

    args[1].start_index = SIZE / 2;
    args[1].end_index = SIZE - 1;
    args[1].thread_id = 2;

    printf("Эхний хагас эрэмбэлэх утас (Thread 1) эхэллээ: индекс [%d - %d]\n",
           args[0].start_index, args[0].end_index);
    pthread_create(&threads[0], NULL, sort_half, (void *)&args[0]);

    printf("Хоёр дахь хагас эрэмбэлэх утас (Thread 2) эхэллээ: индекс [%d - %d]\n",
           args[1].start_index, args[1].end_index);
    pthread_create(&threads[1], NULL, sort_half, (void *)&args[1]);

    printf("\nХоёр эрэмбэлэх утаснууд параллель ажиллаж байна...\n");
    printf("Хүлээж байна...\n\n");

    // Эрэмбэлэх утаснууд дуусахыг хүлээх
    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("=========================================\n");
    printf("2. Үр дүнгийн массивын зөв эсэхийг шалгалаа.\n");
    printf("=========================================\n\n");

    printf("Хэсэгчлэн эрэмбэлэгдсэн массив:\n");
    print_array(arr, SIZE);
    printf("\n");

    // Хоёр хэсгийг эрэмбэлсэн эсэхийг шалгах
    int is_first_half_sorted = 1;
    int is_second_half_sorted = 1;

    for (int i = 0; i < SIZE / 2 - 1; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            is_first_half_sorted = 0;
            break;
        }
    }

    for (int i = SIZE / 2; i < SIZE - 1; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            is_second_half_sorted = 0;
            break;
        }
    }

    printf("Шалгалт 1: Эхний хагас эрэмбэлэгдсэн эсэх: %s\n",
           is_first_half_sorted ? "ТИЙМ" : "ҮГҮЙ");
    printf("Шалгалт 2: Хоёр дахь хагас эрэмбэлэгдсэн эсэх: %s\n",
           is_second_half_sorted ? "ТИЙМ" : "ҮГҮЙ");

    if (is_first_half_sorted && is_second_half_sorted)
    {
        printf("✓ Хоёр хэсэг зөв эрэмбэлэгдсэн байна!\n\n");
    }
    else
    {
        printf("✗ Хэсгүүд зөв эрэмбэлэгдээгүй байна!\n\n");
    }

    printf("Нэгтгэх утас (Thread 3) эхэллээ...\n");
    pthread_create(&threads[2], NULL, merge, NULL);

    pthread_join(threads[2], NULL);
    printf("Нэгтгэх утас дууслаа.\n\n");

    printf("=========================================\n");
    printf("3. Эрэмбэлэлтийн үр дүнг хэвлэж, тэмдэглэл хийн, бүх утас дууссаны дараа нэгтгэсэн массивыг үзүүллээ.\n");
    printf("=========================================\n\n");

    printf("Бүх утаснууд ажиллаж дууслаа!\n");
    printf("Эцсийн эрэмбэлэгдсэн массив:\n");
    print_array(sorted_arr, SIZE);

    // Эцсийн массив эрэмбэлэгдсэн эсэхийг шалгах
    int is_fully_sorted = 1;
    for (int i = 0; i < SIZE - 1; i++)
    {
        if (sorted_arr[i] > sorted_arr[i + 1])
        {
            is_fully_sorted = 0;
            break;
        }
    }

    printf("\nЭцсийн шалгалт: Массив бүрэн эрэмбэлэгдсэн эсэх: %s\n",
           is_fully_sorted ? "ТИЙМ" : "ҮГҮЙ");

    printf("\n=========================================\n");
    printf("Програм амжилттай дууслаа!\n");
    printf("=========================================\n");

    return 0;
}

void *sort_half(void *arg)
{
    thread_args *args = (thread_args *)arg;
    int start = args->start_index;
    int end = args->end_index;
    int thread_id = args->thread_id;

    printf("Thread %d: Эрэмбэлэлт эхэллээ [индекс %d - %d]...\n",
           thread_id, start, end);

    // Bubble sort хэрэгжүүлэлт
    for (int i = start; i <= end; i++)
    {
        for (int j = start; j <= end - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    printf("Thread %d: Эрэмбэлэлт дууслаа [индекс %d - %d].\n",
           thread_id, start, end);

    // Богино хугацаанд унтаж, параллель ажиллагааг илтгэх
    sleep(1);

    return NULL;
}

void *merge(void *arg)
{
    printf("Thread 3: Хоёр эрэмбэлэгдсэн хэсгийг нэгтгэж эхэллээ...\n");

    int mid = SIZE / 2;
    int i = 0, j = mid, k = 0;

    while (i < mid && j < SIZE)
    {
        if (arr[i] < arr[j])
        {
            sorted_arr[k++] = arr[i++];
        }
        else
        {
            sorted_arr[k++] = arr[j++];
        }
    }

    while (i < mid)
    {
        sorted_arr[k++] = arr[i++];
    }

    while (j < SIZE)
    {
        sorted_arr[k++] = arr[j++];
    }

    printf("Thread 3: Нэгтгэлт дууслаа.\n");

    return NULL;
}

void print_array(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}