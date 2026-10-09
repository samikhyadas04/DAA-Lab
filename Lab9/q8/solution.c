/*
 * Q8  Minimum number of meeting rooms  (interval partitioning)
 * Main   : sort by start; min-heap of (end time, room id). If the earliest-ending room is
 *          free (end <= start) reuse it, else open a new room.  Also prints the assignment.
 * Check 1: sort starts and ends separately, two-pointer sweep.  Check 2: O(n^2) max-overlap count.
 * Convention: [s,e) half-open, so [0,30] and [30,40] can share a room.
 * Input : n, then n lines "s e".   Build: gcc -O2 -o q8 solution.c   Run: ./q8 < sample_input.txt | ./q8 --selftest
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { long long s, e; int id; } Iv;
static int cmp_s(const void *a, const void *b) { const Iv *x=a,*y=b; if (x->s!=y->s) return (x->s>y->s)-(x->s<y->s); return x->id-y->id; }
static int cmp_ll(const void *a, const void *b) { long long x=*(const long long*)a,y=*(const long long*)b; return (x>y)-(x<y); }

typedef struct { long long e; int room; } H;
static H *h; static int hn;
static int lt(H a, H b) { return a.e < b.e || (a.e == b.e && a.room < b.room); }
static void push(H v){int i=hn++;h[i]=v;while(i&&lt(h[i],h[(i-1)/2])){int p=(i-1)/2;H t=h[i];h[i]=h[p];h[p]=t;i=p;}}
static H pop(void){H top=h[0];h[0]=h[--hn];int i=0;for(;;){int l=2*i+1,r=l+1,m=i;if(l<hn&&lt(h[l],h[m]))m=l;if(r<hn&&lt(h[r],h[m]))m=r;if(m==i)break;H t=h[i];h[i]=h[m];h[m]=t;i=m;}return top;}

static int rooms_heap(Iv *a, int n, int *assign) {
    qsort(a, n, sizeof(Iv), cmp_s); h = malloc((n + 1) * sizeof(H)); hn = 0; int rooms = 0;
    for (int i = 0; i < n; i++) {
        int room;
        if (hn && h[0].e <= a[i].s) room = pop().room; else room = rooms++;
        if (assign) assign[a[i].id] = room;
        push((H){a[i].e, room});
    }
    free(h); return rooms;
}
static int rooms_sweep(const Iv *a, int n) {
    long long *S = malloc(n * sizeof(long long)), *E = malloc(n * sizeof(long long));
    for (int i = 0; i < n; i++) { S[i] = a[i].s; E[i] = a[i].e; }
    qsort(S, n, sizeof(long long), cmp_ll); qsort(E, n, sizeof(long long), cmp_ll);
    int j = 0, cur = 0, best = 0;
    for (int i = 0; i < n; i++) { while (E[j] <= S[i]) { j++; cur--; } cur++; if (cur > best) best = cur; }
    free(S); free(E); return best;
}
static int rooms_brute(const Iv *a, int n) {
    int best = 0;
    for (int i = 0; i < n; i++) { int c = 0; for (int j = 0; j < n; j++) if (a[j].s <= a[i].s && a[i].s < a[j].e) c++; if (c > best) best = c; }
    return best;
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "--selftest")) {
        srand(4); int bad = 0;
        for (int tc = 0; tc < 20000; tc++) {
            int n = 1 + rand() % 12; Iv a[12], b[12];
            for (int i = 0; i < n; i++) { a[i].s = rand() % 20; a[i].e = a[i].s + 1 + rand() % 10; a[i].id = i; }
            memcpy(b, a, sizeof a); int asg[12];
            int r1 = rooms_heap(b, n, asg), r2 = rooms_sweep(a, n), r3 = rooms_brute(a, n);
            int ok = (r1 == r2 && r2 == r3);
            for (int i = 0; i < n && ok; i++) for (int j = i + 1; j < n; j++)       /* assignment conflict-free? */
                if (asg[i] == asg[j] && a[i].s < a[j].e && a[j].s < a[i].e) { ok = 0; break; }
            if (!ok) { bad++; printf("FAIL tc=%d\n", tc); }
        }
        printf("selftest: 20000 cases, %d failures\n", bad); return bad != 0;
    }
    int n; if (scanf("%d", &n) != 1) return 1;
    Iv *a = malloc(n * sizeof(Iv)); int *asg = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) { if (scanf("%lld %lld", &a[i].s, &a[i].e) != 2) return 1; a[i].id = i; }
    Iv *c = malloc(n * sizeof(Iv)); memcpy(c, a, n * sizeof(Iv));
    int r = rooms_heap(c, n, asg);
    printf("Minimum rooms = %d (sweep check = %d)\n", r, rooms_sweep(a, n));
    for (int i = 0; i < n; i++) printf("  meeting %d [%lld,%lld) -> room %d\n", i + 1, a[i].s, a[i].e, asg[i] + 1);
    return 0;
}
