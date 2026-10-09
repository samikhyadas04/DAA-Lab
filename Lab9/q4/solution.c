/*
 * Q4  Minimum cost to connect sticks  (Huffman-style merging with a min-heap)
 * Input : n, then n stick lengths.     Cost can exceed 32 bits -> long long.
 * Build : gcc -O2 -o q4 solution.c      Run: ./q4 < sample_input.txt | ./q4 --selftest
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long long *h; static int hn;
static void push(long long v){int i=hn++;h[i]=v;while(i&&h[i]<h[(i-1)/2]){int p=(i-1)/2;long long t=h[i];h[i]=h[p];h[p]=t;i=p;}}
static long long pop(void){long long top=h[0];h[0]=h[--hn];int i=0;for(;;){int l=2*i+1,r=l+1,m=i;if(l<hn&&h[l]<h[m])m=l;if(r<hn&&h[r]<h[m])m=r;if(m==i)break;long long t=h[i];h[i]=h[m];h[m]=t;i=m;}return top;}

static long long min_cost(const long long *a, int n) {
    h = malloc((n + 1) * sizeof(long long)); hn = 0;
    for (int i = 0; i < n; i++) push(a[i]);          /* (bottom-up heapify would give O(n); n pushes is O(n log n)) */
    long long cost = 0;
    while (hn > 1) { long long x = pop(), y = pop(); cost += x + y; push(x + y); }
    free(h); return cost;
}

static long long brute(long long *a, int n) {         /* try every pair at every step: exponential */
    if (n == 1) return 0; long long best = -1;
    for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) {
        long long b[8]; int m = 0;
        for (int k = 0; k < n; k++) if (k != i && k != j) b[m++] = a[k];
        b[m++] = a[i] + a[j];
        long long c = a[i] + a[j] + brute(b, m);
        if (best < 0 || c < best) best = c;
    }
    return best;
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "--selftest")) {
        srand(5); int bad = 0;
        for (int tc = 0; tc < 3000; tc++) {
            int n = 1 + rand() % 7; long long a[8]; for (int i = 0; i < n; i++) a[i] = 1 + rand() % 30;
            long long b[8]; memcpy(b, a, sizeof(a));
            if (min_cost(a, n) != brute(b, n)) { bad++; printf("FAIL tc=%d\n", tc); }
        }
        printf("selftest: 3000 cases, %d failures\n", bad); return bad != 0;
    }
    int n; if (scanf("%d", &n) != 1) return 1;
    long long *a = malloc(n * sizeof(long long));
    for (int i = 0; i < n; i++) if (scanf("%lld", &a[i]) != 1) return 1;
    printf("Minimum total cost = %lld\n", min_cost(a, n));
    return 0;
}
