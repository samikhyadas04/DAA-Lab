/*
 * Q3  Minimum number of refuelling stops  (reverse / "regret" greedy)
 * Fuel units = distance units (1 unit of fuel = 1 unit of distance).
 * Idea: drive as far as possible; every station passed is pushed into a max-heap
 *       (we "postpone" the decision to stop there). When fuel runs out, retroactively
 *       stop at the passed station with the LARGEST refuel amount.
 * Extension (title of the problem): min initial fuel F so that <= K stops suffice
 *       -> binary search on F, since min_stops(F) is non-increasing in F.
 * Input : F D n, then n lines "d_i f_i".   Optional arg: --minfuel K
 * Build : gcc -O2 -o q3 solution.c          Run: ./q3 < sample_input.txt | ./q3 --selftest
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { long long d, f; } St;
static int cmp_d(const void *a, const void *b) { const St *x=a,*y=b; return (x->d>y->d)-(x->d<y->d); }

static long long *hp; static int hn;
static void push(long long v){int i=hn++;hp[i]=v;while(i&&hp[i]>hp[(i-1)/2]){int p=(i-1)/2;long long t=hp[i];hp[i]=hp[p];hp[p]=t;i=p;}}
static long long pop(void){long long top=hp[0];hp[0]=hp[--hn];int i=0;for(;;){int l=2*i+1,r=l+1,m=i;if(l<hn&&hp[l]>hp[m])m=l;if(r<hn&&hp[r]>hp[m])m=r;if(m==i)break;long long t=hp[i];hp[i]=hp[m];hp[m]=t;i=m;}return top;}

/* stations must be sorted by d. returns min stops or -1 */
static int min_stops(long long F, long long D, const St *s, int n) {
    hp = malloc((n + 1) * sizeof(long long)); hn = 0;
    long long reach = F; int i = 0, stops = 0, ans;
    while (reach < D) {
        while (i < n && s[i].d <= reach) push(s[i++].f);
        if (hn == 0) { ans = -1; goto done; }
        reach += pop(); stops++;
    }
    ans = stops;
done: free(hp); return ans;
}

static int dp_check(long long F, long long D, const St *s, int n) {      /* O(n^2) DP, independent */
    long long *dp = malloc((n + 1) * sizeof(long long));
    for (int k = 0; k <= n; k++) dp[k] = -1; dp[0] = F;
    for (int i = 0; i < n; i++)
        for (int k = i; k >= 0; k--) if (dp[k] >= s[i].d && dp[k] + s[i].f > dp[k+1]) dp[k+1] = dp[k] + s[i].f;
    int ans = -1; for (int k = 0; k <= n; k++) if (dp[k] >= D) { ans = k; break; }
    free(dp); return ans;
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "--selftest")) {
        srand(3); int bad = 0;
        for (int tc = 0; tc < 5000; tc++) {
            int n = rand() % 9; St s[9]; long long D = 1 + rand() % 60, F = rand() % 15;
            for (int i = 0; i < n; i++) { s[i].d = 1 + rand() % 50; s[i].f = 1 + rand() % 25; }
            qsort(s, n, sizeof(St), cmp_d);
            if (min_stops(F, D, s, n) != dp_check(F, D, s, n)) { bad++; printf("FAIL tc=%d\n", tc); }
        }
        printf("selftest: 5000 cases, %d failures\n", bad); return bad != 0;
    }
    long long F, D; int n;
    if (scanf("%lld %lld %d", &F, &D, &n) != 3) return 1;
    St *s = malloc((n + 1) * sizeof(St));
    for (int i = 0; i < n; i++) if (scanf("%lld %lld", &s[i].d, &s[i].f) != 2) return 1;
    qsort(s, n, sizeof(St), cmp_d);
    int r = min_stops(F, D, s, n);
    if (r < 0) printf("Target unreachable\n"); else printf("Minimum refuelling stops = %d\n", r);
    if (argc > 2 && !strcmp(argv[1], "--minfuel")) {
        int K = atoi(argv[2]); long long lo = 0, hi = D;       /* F = D always needs 0 stops */
        while (lo < hi) { long long mid = (lo + hi) / 2; int m = min_stops(mid, D, s, n); if (m >= 0 && m <= K) hi = mid; else lo = mid + 1; }
        printf("Minimum initial fuel to finish with <= %d stops = %lld\n", K, lo);
    }
    return 0;
}
