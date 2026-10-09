/*
 * Q1  Fractional Knapsack with Deterioration Rate
 *
 * MODEL (see README): weight is consumed at 1 unit per unit time. A unit consumed at
 * time t is worth d_i - lambda_i*t, where d_i = v_i/w_i. We choose amounts x_i in [0,w_i],
 * sum x_i <= W, and an order.
 *   - Order: exchange argument  => sort by lambda DESCENDING (independent of amounts).
 *   - Amounts: objective becomes the concave quadratic
 *         f(x) = sum d_i x_i - 1/2 * sum_ij min(lam_i,lam_j) x_i x_j
 *     solved exactly (to 1e-12) by pairwise mass transfers (SMO-style) with an idle
 *     "dummy" item (d=0, lam=0, w=W) that absorbs unused capacity.
 * Also contains the naive "always take the best CURRENT density" greedy for comparison.
 *
 * Input : n W  then n lines: v_i w_i lambda_i
 * Build : gcc -O2 -o q1 solution.c -lm      Run: ./q1 < sample_input.txt | ./q1 --selftest
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct { double v, w, lam, d; int id; } Item;

static int cmp_lam_desc(const void *a, const void *b) {
    const Item *x = a, *y = b;
    if (x->lam != y->lam) return (x->lam < y->lam) - (x->lam > y->lam);
    return x->id - y->id;
}

/* value of a schedule: items in given order with amounts x[] (exact integral of d - lam*t) */
static double simulate(const Item *it, const double *x, int n) {
    double t = 0, val = 0;
    for (int i = 0; i < n; i++) {
        val += it[i].d * x[i] - it[i].lam * (x[i] * t + x[i] * x[i] / 2.0);
        t += x[i];
    }
    return val;
}

/* optimal amounts for items already sorted by lambda desc. x has n entries (no dummy). */
static double solve_amounts(const Item *it, int n, double W, double *x) {
    int m = n + 1;                       /* index n = dummy idle item */
    double *X = calloc(m, sizeof(double)), *g = malloc(m * sizeof(double));
    double *d = malloc(m * sizeof(double)), *l = malloc(m * sizeof(double)), *w = malloc(m * sizeof(double));
    for (int i = 0; i < n; i++) { d[i] = it[i].d; l[i] = it[i].lam; w[i] = it[i].w; }
    d[n] = 0; l[n] = 0; w[n] = W; X[n] = W;             /* start: all time idle, feasible */
    for (int i = 0; i < m; i++) g[i] = d[i];            /* gradient at x = (0,..,0,W): K row of dummy is 0 */
    for (int sweep = 0; sweep < 200000; sweep++) {
        double best_gain = 0;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++) {
                if (i == j) continue;
                double hi = fmin(w[i] - X[i], X[j]), lo = 0;   /* s >= 0: move s from j to i */
                if (hi <= 1e-15) continue;
                double c = fabs(l[i] - l[j]), diff = g[i] - g[j], s;
                if (diff <= 1e-15) continue;
                s = (c > 1e-18) ? diff / c : hi;
                if (s > hi) s = hi;
                if (s < lo) s = lo;
                double gain = diff * s - 0.5 * c * s * s;
                if (gain <= 1e-14) continue;
                X[i] += s; X[j] -= s;
                for (int k = 0; k < m; k++)
                    g[k] -= (fmin(l[k], l[i]) - fmin(l[k], l[j])) * s;
                if (gain > best_gain) best_gain = gain;
            }
        if (best_gain < 1e-13) break;
    }
    for (int i = 0; i < n; i++) x[i] = X[i];
    double val = simulate(it, x, n);
    free(X); free(g); free(d); free(l); free(w);
    return val;
}

/* naive baseline: at every instant consume the item with the highest CURRENT density */
static double naive_greedy(const Item *it, int n, double W) {
    double *rem = malloc(n * sizeof(double)); double t = 0, val = 0;
    for (int i = 0; i < n; i++) rem[i] = it[i].w;
    while (t < W - 1e-12) {
        int b = -1; double bd = 0;
        for (int i = 0; i < n; i++) if (rem[i] > 1e-12) {
            double dens = it[i].d - it[i].lam * t;
            if (b < 0 || dens > bd) { b = i; bd = dens; }
        }
        if (b < 0 || bd <= 0) break;
        double dt = fmin(rem[b], W - t);
        dt = fmin(dt, bd / it[b].lam);                      /* until its density hits 0 */
        for (int j = 0; j < n; j++) if (j != b && rem[j] > 1e-12 && it[j].lam < it[b].lam) {
            double tc = (it[b].d - it[j].d) / (it[b].lam - it[j].lam);   /* crossing time */
            if (tc > t + 1e-12) dt = fmin(dt, tc - t);
        }
        if (dt < 1e-12) dt = 1e-12;
        val += bd * dt - it[b].lam * dt * dt / 2.0;
        rem[b] -= dt; t += dt;
    }
    free(rem); return val;
}

static double brute(const Item *base, int n, double W, int M) {   /* n<=3, grid search over order & amounts */
    int perm[3] = {0,1,2}, idx[3]; double best = 0;
    int P[6][3] = {{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};
    int np = (n==1)?1:(n==2)?2:6;
    for (int p = 0; p < np; p++) {
        Item it[3];
        for (int i = 0; i < n; i++) it[i] = base[(n==2 && p==1) ? (1-i) : (n==3 ? P[p][i] : i)];
        int tot = 1; for (int i = 0; i < n; i++) tot *= (M + 1);
        for (int c = 0; c < tot; c++) {
            int cc = c; double x[3], s = 0;
            for (int i = 0; i < n; i++) { idx[i] = cc % (M + 1); cc /= (M + 1); x[i] = it[i].w * idx[i] / M; s += x[i]; }
            if (s > W + 1e-12) continue;
            double v = simulate(it, x, n); if (v > best) best = v;
        }
    }
    (void)perm; return best;
}

static void print_solution(Item *it, int n, double W) {
    qsort(it, n, sizeof(Item), cmp_lam_desc);
    double *x = calloc(n, sizeof(double));
    double opt = solve_amounts(it, n, W, x), t = 0;
    printf("Optimal schedule (order = decay rate lambda descending):\n");
    printf("%-6s %-10s %-10s %-10s %-10s\n", "item", "start t", "amount", "fraction", "value");
    for (int i = 0; i < n; i++) {
        if (x[i] > 1e-9) {
            double val = it[i].d * x[i] - it[i].lam * (x[i] * t + x[i] * x[i] / 2.0);
            printf("%-6d %-10.4f %-10.4f %-10.4f %-10.4f\n", it[i].id + 1, t, x[i], x[i] / it[i].w, val);
            t += x[i];
        }
    }
    printf("Total value (optimal)        = %.6f\n", opt);
    printf("Total value (naive greedy)   = %.6f\n", naive_greedy(it, n, W));
    free(x);
}

static double rnd(double a, double b) { return a + (b - a) * (rand() / (double)RAND_MAX); }

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "--selftest")) {
        srand(7); int bad = 0, naive_worse = 0, T = 300;
        for (int tc = 0; tc < T; tc++) {
            int n = 1 + rand() % 3; double W = rnd(1, 8); Item it[3];
            for (int i = 0; i < n; i++) {
                it[i].w = rnd(0.5, 5); it[i].d = rnd(1, 10); it[i].v = it[i].d * it[i].w;
                it[i].lam = rnd(0.05, 3); it[i].id = i;
            }
            double bf = brute(it, n, W, 40);
            Item s[3]; memcpy(s, it, sizeof(it)); qsort(s, n, sizeof(Item), cmp_lam_desc);
            double x[3]; double opt = solve_amounts(s, n, W, x);
            double ng = naive_greedy(it, n, W);
            if (opt + 1e-9 < bf) { bad++; printf("MISMATCH tc=%d opt=%f brute=%f\n", tc, opt, bf); }
            if (ng > opt + 1e-7) { bad++; printf("naive beat optimal?! tc=%d\n", tc); }
            if (ng < opt - 1e-6) naive_worse++;
        }
        printf("selftest: %d cases, %d failures; naive greedy strictly worse in %d cases\n", T, bad, naive_worse);
        return bad != 0;
    }
    int n; double W;
    if (scanf("%d %lf", &n, &W) != 2) return 1;
    Item *it = malloc(n * sizeof(Item));
    for (int i = 0; i < n; i++) {
        scanf("%lf %lf %lf", &it[i].v, &it[i].w, &it[i].lam);
        it[i].d = it[i].v / it[i].w; it[i].id = i;
    }
    print_solution(it, n, W);
    free(it); return 0;
}
