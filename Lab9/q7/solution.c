/*
 * Q7  Minimise deviation (max-min) with ops: odd -> x2, even -> /2
 * Two-way greedy: first push every element to its MAXIMUM form (odd*2), so the only
 * remaining move is "halve an even number". Then repeatedly halve the current maximum
 * (the only element whose change can reduce max-min), tracking the best deviation seen;
 * stop when the maximum is odd (cannot be reduced).
 * Input : n, then n positive integers.   Build: gcc -O2 -o q7 solution.c   Run: ./q7 < sample_input.txt | ./q7 --selftest
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long long *h; static int hn;
static void push(long long v){int i=hn++;h[i]=v;while(i&&h[i]>h[(i-1)/2]){int p=(i-1)/2;long long t=h[i];h[i]=h[p];h[p]=t;i=p;}}
static long long pop(void){long long top=h[0];h[0]=h[--hn];int i=0;for(;;){int l=2*i+1,r=l+1,m=i;if(l<hn&&h[l]>h[m])m=l;if(r<hn&&h[r]>h[m])m=r;if(m==i)break;long long t=h[i];h[i]=h[m];h[m]=t;i=m;}return top;}

static long long min_deviation(const long long *a, int n) {
    h = malloc((n + 1) * sizeof(long long)); hn = 0; long long mn = -1, mx = 0;
    for (int i = 0; i < n; i++) { long long v = (a[i] & 1) ? a[i] * 2 : a[i]; push(v); if (mn < 0 || v < mn) mn = v; if (v > mx) mx = v; }
    long long best = mx - mn;
    for (;;) {
        long long top = pop(); if (top - mn < best) best = top - mn;
        if (top & 1) break;
        top /= 2; if (top < mn) mn = top; push(top);
    }
    free(h); return best;
}

static long long brute(const long long *a, int n) {                  /* enumerate every reachable value of every element */
    long long opts[8][40]; int cnt[8];
    for (int i = 0; i < n; i++) { long long v = a[i]; cnt[i] = 0;
        if (v & 1) { opts[i][cnt[i]++] = v; opts[i][cnt[i]++] = 2 * v; }
        else { while (1) { opts[i][cnt[i]++] = v; if (v & 1) break; v /= 2; } } }
    long long best = -1; int idx[8] = {0};
    for (;;) {
        long long mx = 0, mn = -1;
        for (int i = 0; i < n; i++) { long long v = opts[i][idx[i]]; if (v > mx) mx = v; if (mn < 0 || v < mn) mn = v; }
        if (best < 0 || mx - mn < best) best = mx - mn;
        int k = 0; while (k < n && ++idx[k] == cnt[k]) idx[k++] = 0; if (k == n) break;
    }
    return best;
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "--selftest")) {
        srand(2); int bad = 0;
        for (int tc = 0; tc < 20000; tc++) {
            int n = 1 + rand() % 5; long long a[8]; for (int i = 0; i < n; i++) a[i] = 1 + rand() % 200;
            if (min_deviation(a, n) != brute(a, n)) { bad++; printf("FAIL tc=%d\n", tc); }
        }
        printf("selftest: 20000 cases, %d failures\n", bad); return bad != 0;
    }
    int n; if (scanf("%d", &n) != 1) return 1;
    long long *a = malloc(n * sizeof(long long));
    for (int i = 0; i < n; i++) if (scanf("%lld", &a[i]) != 1) return 1;
    printf("Minimum deviation = %lld\n", min_deviation(a, n));
    return 0;
}
