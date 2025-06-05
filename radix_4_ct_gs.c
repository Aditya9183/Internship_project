#include <stdint.h>
#include <stdio.h>

#define N 4            
#define Q 7681
#define OMEGA 3383      


static inline int16_t mod_add(int16_t a, int16_t b) {
    return (a + b) % Q;
}

static inline int16_t mod_sub(int16_t a, int16_t b) {
    return (a - b + Q) % Q;
}

static inline int16_t mod_mul(int16_t a, int16_t b) {
    return ((int32_t)a * b) % Q;
}


void precompute_omega_tables(
    int16_t *omega1, int16_t *omega2, int16_t *omega3,
    int16_t omega, int n, int q
) {
    int16_t w = 1;
    for (int s = 0; s < n - 1; s++) {
        omega1[s] = w;
        omega2[s] = mod_mul(w, w);
        omega3[s] = mod_mul(omega2[s], w);
        w = mod_mul(w, omega);
    }
}


void radix4_ct_butterfly(
    int16_t *x0, int16_t *x1, int16_t *x2, int16_t *x3,
    int16_t omega1, int16_t omega2, int16_t omega3
) {
    int16_t y0 = *x0;
    int16_t y1 = mod_mul(*x1, omega1);
    int16_t y2 = mod_mul(*x2, omega2);
    int16_t y3 = mod_mul(*x3, omega3);

    int16_t t0 = mod_add(y0, y2);
    int16_t t1 = mod_add(y1, y3);
    int16_t t2 = mod_sub(y0, y2);
    int16_t t3 = mod_sub(y1, y3);
    *x0 = mod_add(t0, t1);
    *x1 = mod_add(t2,mod_mul(OMEGA,t3));
    *x2 = mod_sub(t0, t1);
    *x3 = mod_sub(t2, mod_mul(OMEGA,t3));
}


void ntt_radix4_ct(
    int16_t *a,
    const int16_t *omega1,
    const int16_t *omega2,
    const int16_t *omega3,
    int n
) {
    int t = n;
    int c = 0;

    for (int m = 1; m < n; m *= 4) {
        t = t / 4;
        for (int i = 0; i < m; i++) {
            int s = c + i;
            int16_t w1 = omega1[s];
            int16_t w2 = omega2[s];
            int16_t w3 = omega3[s];

            for (int j = 4 * i * t; j < (4 * i + 1) * t; j++) {
                radix4_ct_butterfly(
                    &a[j], &a[j + t], &a[j + 2 * t], &a[j + 3 * t],
                    w1, w2, w3
                );
            }
        }
        c += m;
    }
}


int main() {
    int16_t a[N] = {1, 2, 3, 4};

    int16_t omega1[N - 1], omega2[N - 1], omega3[N - 1];
    precompute_omega_tables(omega1, omega2, omega3, OMEGA, N, Q);

    printf("Input: ");
    for (int i = 0; i < N; i++) printf("%d ", a[i]);
    printf("\n");

    ntt_radix4_ct(a, omega1, omega2, omega3, N);

    printf("Radix-4 NTT output :\n");
    for (int i = 0; i < N; i++) printf("%d ", a[i]);
    printf("\n");

    return 0;
}
