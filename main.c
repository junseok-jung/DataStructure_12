#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PIVOT_FIRST   1
#define PIVOT_RANDOM  2
#define PIVOT_MEDIAN3 3

long long comp_count = 0; // 데이터 간 비교 횟수 누적

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 첫 번째, 중간, 마지막 원소 중 중앙값의 인덱스 반환
int get_median_of_three_index(int list[], int left, int right)
{
    int mid = (left + right) / 2;
    int a = list[left];
    int b = list[mid];
    int c = list[right];

    comp_count++;
    if (a < b)
    {
        comp_count++;
        if (b < c)
        {
            return mid; // a < b < c
        }
        else
        {
            comp_count++;
            if (a < c)
            {
                return right; // a < c <= b
            }
            else
            {
                return left; // c <= a < b
            }
        }
    }
    else
    {
        comp_count++;
        if (a < c)
        {
            return left; // b <= a < c
        }
        else
        {
            comp_count++;
            if (b < c)
            {
                return right; // b < c <= a
            }
            else
            {
                return mid; // c <= b <= a
            }
        }
    }
}

// 선택된 피벗을 구간의 맨 앞(left)으로 이동하여 분할 로직 통일
void select_pivot(int list[], int left, int right, int pivot_type)
{
    if (pivot_type == PIVOT_FIRST)
    {
        return;
    }
    else if (pivot_type == PIVOT_RANDOM)
    {
        int count = right - left + 1;
        int random_index = left + (rand() % count);
        swap(&list[left], &list[random_index]);
    }
    else if (pivot_type == PIVOT_MEDIAN3)
    {
        int median_index = get_median_of_three_index(list, left, right);
        swap(&list[left], &list[median_index]);
    }
}

// 맨 앞(left) 피벗 기준으로 좌우 분할 후 피벗의 최종 위치 반환
int partition(int list[], int left, int right)
{
    int pivot = list[left];
    int low = left + 1;
    int high = right;

    while (low <= high)
    {
        // 피벗보다 큰 값을 만날 때까지 오른쪽으로 이동
        while (low <= right)
        {
            comp_count++;
            if (list[low] <= pivot)
            {
                low++;
            }
            else
            {
                break;
            }
        }

        // 피벗보다 작거나 같은 값을 만날 때까지 왼쪽으로 이동
        while (high >= left + 1)
        {
            comp_count++;
            if (list[high] > pivot)
            {
                high--;
            }
            else
            {
                break;
            }
        }

        // 포인터가 엇갈리지 않았으면 두 원소 교환
        if (low < high)
        {
            swap(&list[low], &list[high]);
        }
    }

    // 엇갈린 후 작은 값 구역의 끝(high)과 피벗(left) 교환
    swap(&list[left], &list[high]);

    return high;
}

// 재귀적 퀵 정렬
void quick_sort_recursive(int list[], int left, int right, int pivot_type)
{
    if (left < right)
    {
        select_pivot(list, left, right, pivot_type);
        int pivot_index = partition(list, left, right);

        // 확정된 피벗 위치(pivot_index)를 제외하고 좌우 구간 재귀 호출
        quick_sort_recursive(list, left, pivot_index - 1, pivot_type);
        quick_sort_recursive(list, pivot_index + 1, right, pivot_type);
    }
}


void quick_sort_iterative(int list[], int left, int right, int pivot_type)
{
    if (left >= right)
    {
        return;
    }

    int n = right - left + 1;
    int* stack = (int*)malloc(sizeof(int) * (n * 2));
    int top = -1;

    // 초기 구간 push (left, right 순서로 저장)
    stack[++top] = left;
    stack[++top] = right;

    while (top >= 0)
    {
        // 넣은 역순으로 pop (right 먼저 꺼냄)
        int curr_right = stack[top--];
        int curr_left = stack[top--];

        select_pivot(list, curr_left, curr_right, pivot_type);
        int pivot_index = partition(list, curr_left, curr_right);

        // 왼쪽 구간에 원소가 2개 이상이면 스택에 push
        if (pivot_index - 1 > curr_left)
        {
            stack[++top] = curr_left;
            stack[++top] = pivot_index - 1;
        }

        // 오른쪽 구간에 원소가 2개 이상이면 스택에 push
        if (pivot_index + 1 < curr_right)
        {
            stack[++top] = pivot_index + 1;
            stack[++top] = curr_right;
        }
    }

    free(stack);
}

// 원본 배열 복사
void copy_array(int src[], int dest[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        dest[i] = src[i];
    }
}

// 오름차순 정렬 여부 검증
int is_sorted(int list[], int n)
{
    int i;
    for (i = 0; i < n - 1; i++)
    {
        if (list[i] > list[i + 1])
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    srand((unsigned int)time(NULL));

    int sizes[6] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    int s, i, r;

    // 1. 피벗 선택 방법에 따른 비교 횟수 측정
    for (s = 0; s < 6; s++)
    {
        int n = sizes[s];
        int* original = (int*)malloc(sizeof(int) * n);
        int* copy = (int*)malloc(sizeof(int) * n);

        for (i = 0; i < n; i++)
        {
            original[i] = rand();
        }

        printf("Data Size : %d\n\n", n);

        // [1] First Element
        copy_array(original, copy, n);
        comp_count = 0;
        quick_sort_recursive(copy, 0, n - 1, PIVOT_FIRST);
        if (!is_sorted(copy, n)) printf("Sort Error (First)\n");
        printf("Quick Sort (First)    : %lld comparisons\n", comp_count);

        // [2] Random Pivot
        copy_array(original, copy, n);
        comp_count = 0;
        quick_sort_recursive(copy, 0, n - 1, PIVOT_RANDOM);
        if (!is_sorted(copy, n)) printf("Sort Error (Random)\n");
        printf("Quick Sort (Random)   : %lld comparisons\n", comp_count);

        // [3] Median-of-Three
        copy_array(original, copy, n);
        comp_count = 0;
        quick_sort_recursive(copy, 0, n - 1, PIVOT_MEDIAN3);
        if (!is_sorted(copy, n)) printf("Sort Error (Median-3)\n");
        printf("Quick Sort (Median-3) : %lld comparisons\n\n", comp_count);

        free(original);
        free(copy);
    }

    // 2. [보너스] 재귀적 vs 반복적 퀵 정렬 실행시간 비교
    int bonus_n = 10000;
    int repeat = 100; // 짧은 실행시간 보정을 위해 100회 평균 측정

    int* bonus_orig = (int*)malloc(sizeof(int) * bonus_n);
    int* bonus_copy = (int*)malloc(sizeof(int) * bonus_n);

    for (i = 0; i < bonus_n; i++)
    {
        bonus_orig[i] = rand();
    }

    double rec_total_ms = 0.0;
    double iter_total_ms = 0.0;

    for (r = 0; r < repeat; r++)
    {
        // 배열 복사 시간은 측정에서 제외
        copy_array(bonus_orig, bonus_copy, bonus_n);
        clock_t start_rec = clock();
        quick_sort_recursive(bonus_copy, 0, bonus_n - 1, PIVOT_MEDIAN3);
        clock_t end_rec = clock();
        if (!is_sorted(bonus_copy, bonus_n)) printf("Sort Error (Recursive)\n");
        rec_total_ms += (double)(end_rec - start_rec) * 1000.0 / CLOCKS_PER_SEC;

        copy_array(bonus_orig, bonus_copy, bonus_n);
        clock_t start_iter = clock();
        quick_sort_iterative(bonus_copy, 0, bonus_n - 1, PIVOT_MEDIAN3);
        clock_t end_iter = clock();
        if (!is_sorted(bonus_copy, bonus_n)) printf("Sort Error (Iterative)\n");
        iter_total_ms += (double)(end_iter - start_iter) * 1000.0 / CLOCKS_PER_SEC;
    }

    printf("Data Size : %d\n", bonus_n);
    printf("Pivot     : Median-of-Three\n\n");
    printf("Recursive Quick Sort : %.4f ms\n", rec_total_ms / repeat);
    printf("Iterative Quick Sort : %.4f ms\n", iter_total_ms / repeat);

    free(bonus_orig);
    free(bonus_copy);

    return 0;
}