#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct SLLNode {
    int key;
    struct SLLNode* next;
} SLLNode;

typedef struct DLLNode {
    int key;
    struct DLLNode* prev;
    struct DLLNode* next;
} DLLNode;

// 1. UNSORTED ARRAY (UA)
int search_UA(int* arr, int size, int k) {
    for (int i = 0; i < size; i++) if (arr[i] == k) return i;
    return -1;
}
void insert_UA(int* arr, int* size, int x) { arr[(*size)++] = x; }
void delete_UA(int* arr, int* size, int index) {
    if (index < 0 || index >= *size) return;
    arr[index] = arr[(*size) - 1]; // O(1) swap with last
    (*size)--;
}
int max_UA(int* arr, int size) {
    if (size == 0) return INT_MIN;
    int max = arr[0];
    for (int i = 1; i < size; i++) if (arr[i] > max) max = arr[i];
    return max;
}
int min_UA(int* arr, int size) {
    if (size == 0) return INT_MAX;
    int min = arr[0];
    for (int i = 1; i < size; i++) if (arr[i] < min) min = arr[i];
    return min;
}
int pred_UA(int* arr, int size, int index) {
    int target = arr[index], pred_val = INT_MIN, pred_idx = -1;
    for (int i = 0; i < size; i++) {
        if (arr[i] < target && arr[i] > pred_val) { pred_val = arr[i]; pred_idx = i; }
    }
    return pred_idx;
}
int succ_UA(int* arr, int size, int index) {
    int target = arr[index], succ_val = INT_MAX, succ_idx = -1;
    for (int i = 0; i < size; i++) {
        if (arr[i] > target && arr[i] < succ_val) { succ_val = arr[i]; succ_idx = i; }
    }
    return succ_idx;
}

// 2. SORTED ARRAY (SA)
int search_SA(int* arr, int size, int k) {
    int l = 0, r = size - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (arr[m] == k) return m;
        if (arr[m] < k) l = m + 1; else r = m - 1;
    }
    return -1;
}
void insert_SA(int* arr, int* size, int x) {
    int i = *size - 1;
    while (i >= 0 && arr[i] > x) { arr[i + 1] = arr[i]; i--; }
    arr[i + 1] = x; (*size)++;
}
void delete_SA(int* arr, int* size, int index) {
    for (int i = index; i < *size - 1; i++) arr[i] = arr[i + 1];
    (*size)--;
}
int max_SA(int* arr, int size) { return size == 0 ? INT_MIN : arr[size - 1]; }
int min_SA(int* arr, int size) { return size == 0 ? INT_MAX : arr[0]; }
int pred_SA(int size, int index) { return index <= 0 ? -1 : index - 1; }
int succ_SA(int size, int index) { return index >= size - 1 ? -1 : index + 1; }

// 3. SINGLY LINKED UNSORTED LIST (SLLU)
SLLNode* search_SLLU(SLLNode* head, int k) {
    while (head && head->key != k) head = head->next;
    return head;
}
void insert_SLLU(SLLNode** head, int x) {
    SLLNode* newNode = (SLLNode*)malloc(sizeof(SLLNode));
    newNode->key = x; newNode->next = *head; *head = newNode;
}
void delete_SLLU(SLLNode** head, SLLNode* target) {
    if (!*head || !target) return;
    if (*head == target) { *head = target->next; free(target); return; }
    SLLNode* curr = *head;
    while (curr->next && curr->next != target) curr = curr->next;
    if (curr->next == target) { curr->next = target->next; free(target); }
}
SLLNode* max_SLLU(SLLNode* head) {
    SLLNode *maxNode = head;
    while (head) { if (head->key > maxNode->key) maxNode = head; head = head->next; }
    return maxNode;
}
SLLNode* min_SLLU(SLLNode* head) {
    SLLNode *minNode = head;
    while (head) { if (head->key < minNode->key) minNode = head; head = head->next; }
    return minNode;
}
SLLNode* pred_SLLU(SLLNode* head, SLLNode* target) {
    SLLNode *predNode = NULL; int max_val = INT_MIN;
    while (head) {
        if (head->key < target->key && head->key > max_val) { max_val = head->key; predNode = head; }
        head = head->next;
    }
    return predNode;
}
SLLNode* succ_SLLU(SLLNode* head, SLLNode* target) {
    SLLNode *succNode = NULL; int min_val = INT_MAX;
    while (head) {
        if (head->key > target->key && head->key < min_val) { min_val = head->key; succNode = head; }
        head = head->next;
    }
    return succNode;
}

// 4. SINGLY LINKED SORTED LIST (SLLS)
SLLNode* search_SLLS(SLLNode* head, int k) {
    while (head && head->key < k) head = head->next;
    return (head && head->key == k) ? head : NULL;
}
void insert_SLLS(SLLNode** head, int x) {
    SLLNode* newNode = (SLLNode*)malloc(sizeof(SLLNode)); newNode->key = x;
    if (!*head || (*head)->key >= x) { newNode->next = *head; *head = newNode; return; }
    SLLNode* curr = *head;
    while (curr->next && curr->next->key < x) curr = curr->next;
    newNode->next = curr->next; curr->next = newNode;
}
void delete_SLLS(SLLNode** head, SLLNode* target) { delete_SLLU(head, target); }
SLLNode* max_SLLS(SLLNode* head) {
    if (!head) return NULL;
    while (head->next) head = head->next;
    return head;
}
SLLNode* min_SLLS(SLLNode* head) { return head; }
SLLNode* pred_SLLS(SLLNode* head, SLLNode* target) {
    if (!head || head == target) return NULL;
    while (head && head->next != target) head = head->next;
    return head;
}
SLLNode* succ_SLLS(SLLNode* target) { return target ? target->next : NULL; }

// 5. DOUBLY LINKED UNSORTED LIST (DLLU)
DLLNode* search_DLLU(DLLNode* head, int k) {
    while (head && head->key != k) head = head->next;
    return head;
}
void insert_DLLU(DLLNode** head, int x) {
    DLLNode* newNode = (DLLNode*)malloc(sizeof(DLLNode));
    newNode->key = x; newNode->prev = NULL; newNode->next = *head;
    if (*head) (*head)->prev = newNode; *head = newNode;
}
void delete_DLLU(DLLNode** head, DLLNode* target) {
    if (!*head || !target) return;
    if (*head == target) *head = target->next;
    if (target->prev) target->prev->next = target->next;
    if (target->next) target->next->prev = target->prev;
    free(target);
}
DLLNode* max_DLLU(DLLNode* head) {
    DLLNode *maxNode = head;
    while (head) { if (head->key > maxNode->key) maxNode = head; head = head->next; }
    return maxNode;
}
DLLNode* min_DLLU(DLLNode* head) {
    DLLNode *minNode = head;
    while (head) { if (head->key < minNode->key) minNode = head; head = head->next; }
    return minNode;
}
DLLNode* pred_DLLU(DLLNode* head, DLLNode* target) {
    DLLNode *predNode = NULL; int max_val = INT_MIN;
    while (head) {
        if (head->key < target->key && head->key > max_val) { max_val = head->key; predNode = head; }
        head = head->next;
    }
    return predNode;
}
DLLNode* succ_DLLU(DLLNode* head, DLLNode* target) {
    DLLNode *succNode = NULL; int min_val = INT_MAX;
    while (head) {
        if (head->key > target->key && head->key < min_val) { min_val = head->key; succNode = head; }
        head = head->next;
    }
    return succNode;
}

// 6. DOUBLY LINKED SORTED LIST (DLLS)
DLLNode* search_DLLS(DLLNode* head, int k) {
    while (head && head->key < k) head = head->next;
    return (head && head->key == k) ? head : NULL;
}
void insert_DLLS(DLLNode** head, DLLNode** tail, int x) {
    DLLNode* newNode = (DLLNode*)malloc(sizeof(DLLNode)); newNode->key = x;
    if (!*head) { newNode->prev = newNode->next = NULL; *head = *tail = newNode; return; }
    DLLNode* curr = *head;
    while (curr && curr->key < x) curr = curr->next;
    if (curr == *head) { newNode->next = *head; newNode->prev = NULL; (*head)->prev = newNode; *head = newNode; } 
    else if (!curr) { newNode->prev = *tail; newNode->next = NULL; (*tail)->next = newNode; *tail = newNode; } 
    else { newNode->next = curr; newNode->prev = curr->prev; curr->prev->next = newNode; curr->prev = newNode; }
}
void delete_DLLS(DLLNode** head, DLLNode** tail, DLLNode* target) {
    if (!*head || !target) return;
    if (*head == target) *head = target->next;
    if (*tail == target) *tail = target->prev;
    if (target->prev) target->prev->next = target->next;
    if (target->next) target->next->prev = target->prev;
    free(target);
}
DLLNode* max_DLLS(DLLNode* tail) { return tail; } // O(1) using tail pointer
DLLNode* min_DLLS(DLLNode* head) { return head; }
DLLNode* pred_DLLS(DLLNode* target) { return target ? target->prev : NULL; }
DLLNode* succ_DLLS(DLLNode* target) { return target ? target->next : NULL; }


// BENCHMARKING FRAMEWORK

double time_diff(clock_t start, clock_t end) {
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main() {
    srand(time(NULL));
    
    // N values to benchmark
    int n_sizes[] = {1000, 2000, 4000, 8000, 10000};
    int num_tests = sizeof(n_sizes) / sizeof(n_sizes[0]);
    
    FILE *f = fopen("results.csv", "w");
    if (!f) { printf("Failed to open file.\n"); return 1; }
    
    // Write CSV Header
    fprintf(f, "N,Data_Structure,Time_Insert,Time_Search,Time_Max,Time_Min,Time_Pred,Time_Succ,Time_Delete\n");
    printf("Benchmarking all 42 functions... (This will take a minute for large N)\n\n");
    
    for (int t = 0; t < num_tests; t++) {
        int N = n_sizes[t];
        printf("Running operations for N = %d...\n", N);
        
        // Generate N random keys
        int* keys = (int*)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) keys[i] = rand() % 100000;
        
        clock_t s, e;
        double t_ins, t_srch, t_max, t_min, t_prd, t_suc, t_del;
        
        // 1. UNSORTED ARRAY
        int* ua = (int*)malloc(N * sizeof(int)); int ua_s = 0;
        s=clock(); for(int i=0;i<N;i++) insert_UA(ua, &ua_s, keys[i]); e=clock(); t_ins = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) search_UA(ua, ua_s, -1); e=clock(); t_srch = time_diff(s,e); // Worst case search
        s=clock(); for(int i=0;i<N;i++) max_UA(ua, ua_s); e=clock(); t_max = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) min_UA(ua, ua_s); e=clock(); t_min = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) pred_UA(ua, ua_s, i); e=clock(); t_prd = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) succ_UA(ua, ua_s, i); e=clock(); t_suc = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) delete_UA(ua, &ua_s, 0); e=clock(); t_del = time_diff(s,e); // Delete idx 0
        fprintf(f, "%d,Unsorted_Array,%f,%f,%f,%f,%f,%f,%f\n", N, t_ins, t_srch, t_max, t_min, t_prd, t_suc, t_del);
        free(ua);

        // 2. SORTED ARRAY
        int* sa = (int*)malloc(N * sizeof(int)); int sa_s = 0;
        s=clock(); for(int i=0;i<N;i++) insert_SA(sa, &sa_s, keys[i]); e=clock(); t_ins = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) search_SA(sa, sa_s, -1); e=clock(); t_srch = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) max_SA(sa, sa_s); e=clock(); t_max = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) min_SA(sa, sa_s); e=clock(); t_min = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) pred_SA(sa_s, i); e=clock(); t_prd = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) succ_SA(sa_s, i); e=clock(); t_suc = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) delete_SA(sa, &sa_s, 0); e=clock(); t_del = time_diff(s,e); // Worst case O(N) shift
        fprintf(f, "%d,Sorted_Array,%f,%f,%f,%f,%f,%f,%f\n", N, t_ins, t_srch, t_max, t_min, t_prd, t_suc, t_del);
        free(sa);

        // 3. SINGLY LINKED UNSORTED LIST (SLLU)
        SLLNode* sllu = NULL; SLLNode* sllu_nodes[10000];
        s=clock(); for(int i=0;i<N;i++) { insert_SLLU(&sllu, keys[i]); sllu_nodes[i] = sllu; } e=clock(); t_ins = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) search_SLLU(sllu, -1); e=clock(); t_srch = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) max_SLLU(sllu); e=clock(); t_max = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) min_SLLU(sllu); e=clock(); t_min = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) pred_SLLU(sllu, sllu_nodes[i]); e=clock(); t_prd = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) succ_SLLU(sllu, sllu_nodes[i]); e=clock(); t_suc = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) delete_SLLU(&sllu, sllu_nodes[i]); e=clock(); t_del = time_diff(s,e);
        fprintf(f, "%d,SLL_Unsorted,%f,%f,%f,%f,%f,%f,%f\n", N, t_ins, t_srch, t_max, t_min, t_prd, t_suc, t_del);

        // 4. SINGLY LINKED SORTED LIST (SLLS)
        SLLNode* slls = NULL; SLLNode* slls_nodes[10000];
        s=clock(); for(int i=0;i<N;i++) { insert_SLLS(&slls, keys[i]); } e=clock(); t_ins = time_diff(s,e);
        SLLNode* curr = slls; for(int i=0;i<N;i++) { slls_nodes[i] = curr; curr = curr->next; } // gather pointers
        s=clock(); for(int i=0;i<N;i++) search_SLLS(slls, -1); e=clock(); t_srch = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) max_SLLS(slls); e=clock(); t_max = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) min_SLLS(slls); e=clock(); t_min = time_diff(s,e);
        s=clock(); for(int i=1;i<N;i++) pred_SLLS(slls, slls_nodes[i]); e=clock(); t_prd = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) succ_SLLS(slls_nodes[i]); e=clock(); t_suc = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) delete_SLLS(&slls, slls_nodes[N-1-i]); e=clock(); t_del = time_diff(s,e); // Delete from tail for worst case SLL
        fprintf(f, "%d,SLL_Sorted,%f,%f,%f,%f,%f,%f,%f\n", N, t_ins, t_srch, t_max, t_min, t_prd, t_suc, t_del);

        // 5. DOUBLY LINKED UNSORTED LIST (DLLU)
        DLLNode* dllu = NULL; DLLNode* dllu_nodes[10000];
        s=clock(); for(int i=0;i<N;i++) { insert_DLLU(&dllu, keys[i]); dllu_nodes[i] = dllu; } e=clock(); t_ins = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) search_DLLU(dllu, -1); e=clock(); t_srch = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) max_DLLU(dllu); e=clock(); t_max = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) min_DLLU(dllu); e=clock(); t_min = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) pred_DLLU(dllu, dllu_nodes[i]); e=clock(); t_prd = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) succ_DLLU(dllu, dllu_nodes[i]); e=clock(); t_suc = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) delete_DLLU(&dllu, dllu_nodes[i]); e=clock(); t_del = time_diff(s,e); // O(1) delete
        fprintf(f, "%d,DLL_Unsorted,%f,%f,%f,%f,%f,%f,%f\n", N, t_ins, t_srch, t_max, t_min, t_prd, t_suc, t_del);

        // 6. DOUBLY LINKED SORTED LIST (DLLS)
        DLLNode* dlls = NULL; DLLNode* tail = NULL; DLLNode* dlls_nodes[10000];
        s=clock(); for(int i=0;i<N;i++) { insert_DLLS(&dlls, &tail, keys[i]); } e=clock(); t_ins = time_diff(s,e);
        DLLNode* dcurr = dlls; for(int i=0;i<N;i++) { dlls_nodes[i] = dcurr; dcurr = dcurr->next; }
        s=clock(); for(int i=0;i<N;i++) search_DLLS(dlls, -1); e=clock(); t_srch = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) max_DLLS(tail); e=clock(); t_max = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) min_DLLS(dlls); e=clock(); t_min = time_diff(s,e);
        s=clock(); for(int i=0;i<N;i++) pred_DLLS(dlls_nodes[i]); e=clock(); t_prd = time_diff(s,e); // O(1) using prev
        s=clock(); for(int i=0;i<N;i++) succ_DLLS(dlls_nodes[i]); e=clock(); t_suc = time_diff(s,e); // O(1) using next
        s=clock(); for(int i=0;i<N;i++) delete_DLLS(&dlls, &tail, dlls_nodes[i]); e=clock(); t_del = time_diff(s,e); // O(1) delete
        fprintf(f, "%d,DLL_Sorted,%f,%f,%f,%f,%f,%f,%f\n", N, t_ins, t_srch, t_max, t_min, t_prd, t_suc, t_del);

        free(keys);
    }
    
    fclose(f);
    printf("Benchmarking complete! Open 'results.csv' to view the data.\n");
    return 0;
}