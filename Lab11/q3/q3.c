/*
 * Q3: Bidirectional BFS for the shortest transformation sequence
 *
 * Words are nodes; two words are adjacent if they differ in exactly one
 * character. Instead of building the graph (O(N^2 L)), neighbours are
 * generated on the fly: change each of the L positions to each of the 25
 * other letters and look the result up in a hash table (open addressing).
 * BFS runs from A and from B at the same time; each round the SMALLER
 * frontier is expanded by one full level. When a generated word was already
 * visited by the other side, the answer is dist[u] + 1 + dist[v].
 *
 * Rules used: B must be in the dictionary (else -1); A need not be.
 *             If A == B the answer is 0.
 *
 * Input : N L
 *         N dictionary words
 *         A B
 * Output: minimum number of mutations k, or -1
 * Time  : O(N * 26 * L^2) worst case (hash of length L per candidate);
 *         bidirectional search usually explores far fewer nodes.
 * Space : O(N * L)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL 12
#define HBITS 18
#define HSIZE (1 << HBITS)         /* > 2 * (1e5 + 1) */
#define HMASK (HSIZE - 1)

static int table[HSIZE];
static char (*words)[MAXL];
static int cnt = 0;

static unsigned hash_str(const char *s) {
    unsigned h = 2166136261u;
    while (*s) { h ^= (unsigned char)*s++; h *= 16777619u; }
    return h;
}

static int find_word(const char *s) {
    unsigned h = hash_str(s) & HMASK;
    while (table[h] != -1) {
        if (strcmp(words[table[h]], s) == 0) return table[h];
        h = (h + 1) & HMASK;
    }
    return -1;
}

static int insert_word(const char *s) {
    unsigned h = hash_str(s) & HMASK;
    while (table[h] != -1) {
        if (strcmp(words[table[h]], s) == 0) return table[h];   /* duplicate */
        h = (h + 1) & HMASK;
    }
    strcpy(words[cnt], s);
    table[h] = cnt;
    return cnt++;
}

int main(void) {
    int N, L;
    if (scanf("%d %d", &N, &L) != 2) return 1;

    words = malloc((size_t)(N + 2) * MAXL);
    for (int i = 0; i < HSIZE; i++) table[i] = -1;

    char buf[MAXL], A[MAXL], B[MAXL];
    for (int i = 0; i < N; i++) {
        if (scanf("%11s", buf) != 1) return 1;
        insert_word(buf);
    }
    if (scanf("%11s %11s", A, B) != 2) return 1;

    if (strcmp(A, B) == 0) { printf("0\n"); return 0; }
    int ib = find_word(B);
    if (ib < 0) { printf("-1\n"); return 0; }   /* B must be in D */
    int ia = find_word(A);
    if (ia < 0) ia = insert_word(A);             /* A is only a start node */

    int total = cnt;
    char *side = calloc(total, 1);               /* 0 none, 1 from A, 2 from B */
    int *dist = calloc(total, sizeof(int));
    int *fr[2], *nx[2], fn[2];
    for (int k = 0; k < 2; k++) {
        fr[k] = malloc(total * sizeof(int));
        nx[k] = malloc(total * sizeof(int));
        fn[k] = 0;
    }

    side[ia] = 1; dist[ia] = 0; fr[0][fn[0]++] = ia;
    side[ib] = 2; dist[ib] = 0; fr[1][fn[1]++] = ib;

    int answer = -1;
    while (fn[0] > 0 && fn[1] > 0 && answer == -1) {
        int k = (fn[0] <= fn[1]) ? 0 : 1;        /* expand smaller frontier */
        int mark = k + 1, other = 2 - k;         /* other side's mark */
        int nn = 0;

        for (int i = 0; i < fn[k] && answer == -1; i++) {
            int u = fr[k][i];
            char cur[MAXL];
            strcpy(cur, words[u]);
            for (int pos = 0; pos < L && answer == -1; pos++) {
                char orig = cur[pos];
                for (char c = 'a'; c <= 'z'; c++) {
                    if (c == orig) continue;
                    cur[pos] = c;
                    int v = find_word(cur);
                    if (v < 0) continue;
                    if (side[v] == other) { answer = dist[u] + 1 + dist[v]; break; }
                    if (side[v] == 0) {
                        side[v] = mark;
                        dist[v] = dist[u] + 1;
                        nx[k][nn++] = v;
                    }
                }
                cur[pos] = orig;
            }
        }
        int *t = fr[k]; fr[k] = nx[k]; nx[k] = t;
        fn[k] = nn;
    }

    printf("%d\n", answer);
    return 0;
}