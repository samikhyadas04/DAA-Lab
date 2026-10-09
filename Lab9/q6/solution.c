/*
 * Q6  Rearrange string so equal characters are >= K apart
 * Greedy: always place the available character with the LARGEST remaining count.
 *   A character placed at position j is "cooling down" and returns to the max-heap at position j+K.
 * Input : line 1 = string S (no newline in it),  line 2 = K
 * Output: the rearranged string, or an empty string if impossible.
 * Build : gcc -O2 -o q6 solution.c      Run: ./q6 < sample_input.txt | ./q6 --selftest
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int cnt; unsigned char ch; } E;
static E h[256]; static int hn;
static int better(E a, E b) { return a.cnt > b.cnt || (a.cnt == b.cnt && a.ch < b.ch); }
static void push(E v){int i=hn++;h[i]=v;while(i&&better(h[i],h[(i-1)/2])){int p=(i-1)/2;E t=h[i];h[i]=h[p];h[p]=t;i=p;}}
static E pop(void){E top=h[0];h[0]=h[--hn];int i=0;for(;;){int l=2*i+1,r=l+1,m=i;if(l<hn&&better(h[l],h[m]))m=l;if(r<hn&&better(h[r],h[m]))m=r;if(m==i)break;E t=h[i];h[i]=h[m];h[m]=t;i=m;}return top;}

/* returns malloc'd result; result[0]==0 and return value NULL-like empty string if impossible */
static char *reorganize(const char *s, int K) {
    int n = strlen(s); char *out = calloc(n + 1, 1);
    if (K <= 1) { strcpy(out, s); return out; }
    int cnt[256] = {0}; for (int i = 0; i < n; i++) cnt[(unsigned char)s[i]]++;
    hn = 0; for (int c = 0; c < 256; c++) if (cnt[c]) push((E){cnt[c], (unsigned char)c});
    E *placed = malloc((n + 1) * sizeof(E));                       /* remaining count after each placement */
    for (int i = 0; i < n; i++) {
        if (i >= K && placed[i-K].cnt > 0) push(placed[i-K]);      /* cooldown over */
        if (hn == 0) { out[0] = 0; free(placed); return out; }      /* impossible */
        E e = pop(); out[i] = e.ch; e.cnt--; placed[i] = e;
    }
    out[n] = 0; free(placed); return out;
}

static int valid(const char *s, const char *o, int K) {
    int n = strlen(s); if ((int)strlen(o) != n) return 0;
    int a[256] = {0}, b[256] = {0};
    for (int i = 0; i < n; i++) { a[(unsigned char)s[i]]++; b[(unsigned char)o[i]]++; }
    if (memcmp(a, b, sizeof a)) return 0;
    for (int i = 0; i < n; i++) for (int j = i + 1; j < n && j < i + K; j++) if (o[i] == o[j]) return 0;
    return 1;
}

static int dfs(int pos, int n, int *cnt, int *last, int K) {        /* exhaustive feasibility */
    if (pos == n) return 1;
    for (int c = 0; c < 4; c++) if (cnt[c] && (last[c] < 0 || pos - last[c] >= K)) {
        int sv = last[c]; cnt[c]--; last[c] = pos;
        if (dfs(pos + 1, n, cnt, last, K)) { cnt[c]++; last[c] = sv; return 1; }
        cnt[c]++; last[c] = sv; }
    return 0;
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "--selftest")) {
        srand(1); int bad = 0, feas = 0, T = 20000;
        for (int tc = 0; tc < T; tc++) {
            int n = 1 + rand() % 9, K = rand() % 6; char s[16]; int cnt[4] = {0}, last[4] = {-1,-1,-1,-1};
            for (int i = 0; i < n; i++) { s[i] = 'a' + rand() % 4; cnt[s[i]-'a']++; } s[n] = 0;
            int f = (K <= 1) ? 1 : dfs(0, n, cnt, last, K);
            char *o = reorganize(s, K);
            if (f != (o[0] != 0) || (f && !valid(s, o, K))) { bad++; printf("FAIL tc=%d s=%s K=%d\n", tc, s, K); }
            feas += f; free(o);
        }
        printf("selftest: %d cases (%d feasible), %d failures\n", T, feas, bad); return bad != 0;
    }
    char s[100005]; int K;
    if (!fgets(s, sizeof s, stdin)) return 1; s[strcspn(s, "\r\n")] = 0;
    if (scanf("%d", &K) != 1) return 1;
    char *o = reorganize(s, K);
    if (o[0] == 0 && s[0] != 0) printf("Impossible -> \"\"\n"); else printf("%s\n", o);
    return 0;
}
