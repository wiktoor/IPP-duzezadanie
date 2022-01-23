/** @file
  Implementacja interfejsu klasy wielomianów rzadkich wielu zmiennych

  @authors Wiktor Chmielewski <wc429131@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "poly.h"
#include "parse.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** Zwraca większą z liczb
 * @param[in] a: liczba @f$a@f$
 * @param[in] b: liczba @f$b@f$
 * @return większa z liczb @f$a@f$ i @f$b@f$
 */
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/** Funkcja porównująca dwa jednomiany pod względem ich wykładnika
 * @param[in] m1: pierwszy z jednomianów
 * @param[in] m2: drugi z jednomianów
 * @return @f$true@f$ jeżeli pierwsza ma większy wykładnik oraz
 * @f$false @f$ w przeciwnym przypadku
 */
static int CompareMonos(const void *m1, const void *m2) {
    return ((Mono *) m1)->exp - ((Mono *) m2)->exp;
}

/** Sprawdza, czy @f$p@f$ jest złożonym zerem, tj. wielomianem postaci
 * @f$0*x_0^{e_0}*...*x_n^{e_n}@f$ 
 * @param[in] p: wielomian @f$p@f$
 * @return czy wielomian @f$p@f$ jest złożonym zerem?
 */
static bool PolyIsComplexZero(Poly *p) {
    return (PolyIsCoeff(p) ? PolyIsZero(p) : 
        (p->size == 1 && PolyIsComplexZero(&p->arr[0].p)));
}

bool PolyIsComplexCoeff(const Poly *p) {
    return ((PolyIsCoeff(p)) ? true : 
        (p->size == 1 && p->arr[0].exp == 0 && 
        PolyIsComplexCoeff(&p->arr[0].p)));
}

poly_coeff_t PolyGetComplexCoeff(const Poly *p) {
    return ((PolyIsCoeff(p)) ? p->coeff : PolyGetComplexCoeff(&p->arr[0].p));
}

/** Sprawdza, czy jednomian jest zerem, tj. postaci @f$p*x_0^e@f$, gdzie
 * @f$p@f$ jest złożonym zerem
 * @param[in] m: jednomian @f$m@f$
 * @return czy jednomian jest zerem?
 */
static bool MonoIsZero(Mono *m) {
    return PolyIsComplexZero(&m->p);
}

// zdefiniowano poniżej
static Mono MonoAddMonos(size_t count, const Mono monos[]);

/** Sprawdza, czy wielomian jest posortowany względem wykładników
 * @param[in] p: wielomian @f$p@f$
 * @return czy wielomian @f$p@f$ jest posortowany względem wykładników?
 */
static bool PolyIsSorted(Poly *p) {
    assert(!PolyIsCoeff(p));
    for (size_t i = 0; i < p->size; i++) {
        if (i != 0 && p->arr[i].exp < p->arr[i - 1].exp) return false;
    }
    return true;
}

/** Upraszcza wielomian, dodając do siebie jednomiany o tych samych potęgach
 * @param[in] p: wielomian @f$p@f$
 */
static void PolySimplify(Poly *p) {
    if (PolyIsCoeff(p)) return;
    if (!PolyIsSorted(p)) qsort(p->arr, p->size, sizeof(Mono), CompareMonos);
    size_t uniqueDegrees = 0;
    for (size_t i = 0; i < p->size; i++) {
        if (i == 0 || MonoGetExp(&p->arr[i]) != MonoGetExp(&p->arr[i - 1])) 
            uniqueDegrees++;
    }
    Mono *arr = malloc(uniqueDegrees * sizeof(Mono));
    CHECK_PTR(arr);
    size_t shift = 0;
    size_t idx = 0;
    while (shift < p->size) {
        size_t cnt = 0;
        poly_exp_t exp = MonoGetExp(&p->arr[shift]);
        while (shift + cnt < p->size && 
            MonoGetExp(&p->arr[shift + cnt]) == exp) cnt++;
        // Mono m = MonoAddMonos(cnt, p->arr + shift);
        Mono m;
        if (cnt == 1) m = p->arr[shift];
        else {
            m = MonoAddMonos(cnt, p->arr + shift);
            for (size_t i = 0; i < cnt; i++) MonoDestroy(&p->arr[shift + i]);
        }
        if (!MonoIsZero(&m)) {
            arr[idx] = m;
            idx++;
        }
        else MonoDestroy(&m);
        shift += cnt;
    }
    free(p->arr);
    if (idx == 0) {
        free(arr);
        *p = PolyZero();
    }
    else *p = (Poly) { .arr = arr, .size = idx };
}

/** Dodaje do siebie wielomiany o takich samych wykładnikach
 * @param[in] count: liczba wielomianów do dodania
 * @param[in] monos: tablica wielomianów
 * @return suma jednomianów
 */
static Mono MonoAddMonos(size_t count, const Mono monos[]) {
    if (count == 1) return MonoClone(&monos[0]);
    poly_exp_t exp = monos[0].exp;
    size_t size = 0;
    poly_coeff_t coeff = 0;
    for (size_t i = 0; i < count; i++) {
        assert(monos[i].exp == exp);
        if (PolyIsCoeff(&monos[i].p)) coeff += monos[i].p.coeff;
        else size += monos[i].p.size;
    }
    if (size == 0) {
        Poly c = PolyFromCoeff(coeff);
        return MonoFromPoly(&c, exp);
    }
    Mono *arr = malloc((size + 1) * sizeof(Mono));
    CHECK_PTR(arr);
    size_t shift = 0;
    for (size_t i = 0; i < count; i++) {
        if (!PolyIsCoeff(&monos[i].p)) {
            memcpy(arr + shift, monos[i].p.arr, 
                monos[i].p.size * sizeof(Mono));
            shift += monos[i].p.size;
        }
    }
    if (coeff != 0) {
        Poly c = PolyFromCoeff(coeff);
        arr[size] = MonoFromPoly(&c, 0);
        size++;
    }
    Poly tmp = (Poly) { .arr = arr, .size = size };
    Poly p = PolyClone(&tmp);
    free(arr);
    PolySimplify(&p);
    Mono res = MonoFromPoly(&p, exp);
    return res;
}

void PolyDestroy(Poly *p) {
    if (!PolyIsCoeff(p)) {
        for (size_t i = 0; i < p->size; i++) MonoDestroy(&p->arr[i]);
        free(p->arr);
        p->arr = NULL;
    }
}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p)) return PolyFromCoeff(p->coeff);
    else {
        Mono *arr = malloc(p->size * sizeof(Mono));
        CHECK_PTR(arr);
        for (size_t i = 0; i < p->size; i++)
            arr[i] = MonoClone(&p->arr[i]);
        return (Poly) { .arr = arr, .size = p->size };
    }
}

/** Dodaje do siebie wielomiany, oba niebędące stałymi
 * @param[in] p: niebędący stałą wielomian @f$p@f$
 * @param[in] q: niebędący stałą wielomian @f$q@f$
 * @return @f$p*q@f$
 */
Poly MergePolies(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && !PolyIsCoeff(q));
    Mono *arr = malloc((p->size + q->size) * sizeof(Mono));
    CHECK_PTR(arr);
    size_t pidx = 0, qidx = 0;
    if (p->arr[p->size - 1].exp < q->arr[0].exp) {
        memcpy(arr, p->arr, p->size * sizeof(Mono));
        memcpy(arr + p->size, q->arr, q->size * sizeof(Mono));
        pidx = p->size;
        qidx = q->size;
    }
    else if (p->arr[0].exp > q->arr[q->size - 1].exp) {
        memcpy(arr, q->arr, q->size * sizeof(Mono));
        memcpy(arr + q->size, p->arr, p->size * sizeof(Mono));
        pidx = p->size;
        qidx = q->size;
    }
    while (pidx < p->size || qidx < q->size) {
        if (pidx == p->size) {
            memcpy(arr + pidx + qidx, q->arr + qidx, 
                (q->size - qidx) * sizeof(Mono));
            qidx = q->size;
        }
        else if (qidx == q->size) {
            memcpy(arr + pidx + qidx, p->arr + pidx, 
                (p->size - pidx) * sizeof(Mono));
            pidx = p->size;
        }
        else {
            if (p->arr[pidx].exp < q->arr[qidx].exp) {
                arr[pidx + qidx] = p->arr[pidx];
                pidx++;
            }
            else {
                arr[pidx + qidx] = q->arr[qidx];
                qidx++;
            }
        }
    }
    Poly tmp = (Poly) { .arr = arr, .size = p->size + q->size };
    Poly res = PolyClone(&tmp);
    free(arr);
    PolySimplify(&res);
    return res;
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) 
        return PolyFromCoeff(p->coeff + q->coeff);
    else if (PolyIsCoeff(p)) {
        if (PolyIsZero(p))
            return PolyClone(q);
        else {
            Mono *arr = malloc((q->size + 1) * sizeof(Mono));
            CHECK_PTR(arr);
            arr[0] = MonoFromPoly(p, 0);
            memcpy(arr + 1, q->arr, q->size * sizeof(Mono));
            Poly tmp = (Poly) { .arr = arr, .size = q->size + 1 };
            Poly res = PolyClone(&tmp);
            free(arr);
            PolySimplify(&res);
            return res;
        }
    }
    else if (PolyIsCoeff(q)) return PolyAdd(q, p);
    else return MergePolies(p, q);
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
    Mono *arr = malloc(count * sizeof(Mono));
    CHECK_PTR(arr);
    memcpy(arr, monos, count * sizeof(Mono));
    Poly p = { .arr = arr, .size = count };
    PolySimplify(&p);
    return p;
}

/** Mnoży przez siebie dwa jednomiany
 * @param[in] m: jednomian @f$m@f$
 * @param[in] n: jednomian @f$n@f$
 * @return @f$m*n@f$
 */
static Mono MonoMul(const Mono *m, const Mono *n) {
    Poly p = PolyMul(&m->p, &n->p);
    return MonoFromPoly(&p, m->exp + n->exp);
}

Poly PolyMul(const Poly *p, const Poly *q) {
    if (PolyIsZero(p) || PolyIsZero(q)) return PolyZero();
    else if (PolyIsCoeff(p) && PolyIsCoeff(q)) 
        return PolyFromCoeff(p->coeff * q->coeff);
    else if (PolyIsCoeff(p)) {
        Mono *arr = malloc(q->size * sizeof(Mono));
        CHECK_PTR(arr);
        for (size_t i = 0; i < q->size; i++) {
            Poly r = PolyMul(p, &q->arr[i].p);
            arr[i] = MonoFromPoly(&r, MonoGetExp(&q->arr[i]));
        }
        Poly res = (Poly) { .arr = arr, .size = q->size };
        PolySimplify(&res);
        return res;
    }
    else if (PolyIsCoeff(q)) return PolyMul(q, p);
    else {
        Mono *arr = malloc((p->size * q->size) * sizeof(Mono));
        CHECK_PTR(arr);
        for (size_t i = 0; i < p->size; i++) {
            for (size_t j = 0; j < q->size; j++) 
                arr[i * q->size + j] = MonoMul(&p->arr[i], &q->arr[j]);
        }
        Poly res = (Poly) { .arr = arr, .size = p->size * q->size };
        PolySimplify(&res);
        return res;
    }
}

/** Zwraca jednomian przeciwny
 * @param[in] m: jednomian @f$m@f$
 * @return jednomian przeciwny do @f$m@f$
 */
static Mono MonoNeg(const Mono *m) {
    Poly p = PolyNeg(&m->p);
    return MonoFromPoly(&p, m->exp);
}

Poly PolyNeg(const Poly *p) {
    if (PolyIsCoeff(p))
        return PolyFromCoeff((-1) * p->coeff);
    else {
        Mono *arr = malloc(p->size * sizeof(Mono));
        CHECK_PTR(arr);
        for (size_t i = 0; i < p->size; i++) 
            arr[i] = MonoNeg(&p->arr[i]);
        Poly res = (Poly) { .arr = arr, .size = p->size };
        PolySimplify(&res);
        return res;
    }
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly r = PolyNeg(q);
    Poly res = PolyAdd(p, &r);
    PolyDestroy(&r);
    PolySimplify(&res);
    return res;
}

/** Zwraca stopień jednomianu ze względu na zadaną zmienną
 * @param[in] m: jednomian
 * @param[in] var_idx: indeks zmiennej
 * @return stopień jednomianu @f$m@f$ ze względu na zmienną @f$var_idx@f$ 
 */
static poly_exp_t MonoDegBy(const Mono *m, size_t var_idx) {
    if (var_idx == 0) return m->exp;
    else return PolyDegBy(&m->p, var_idx - 1);
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    if (PolyIsCoeff(p)) return (PolyIsZero(p) ? -1 : 0);
    else {
        poly_exp_t res = 0;
        for (size_t i = 0; i < p->size; i++)
            res = MAX(res, MonoDegBy(&p->arr[i], var_idx));
        return res;
    }
}

/** Zwraca stopień jednomianu
 * @param[in] m: jednomian
 * @return stopień jednomianu @f$m@f$
 */
static poly_exp_t MonoDeg(const Mono *m) {
    return m->exp + PolyDeg(&m->p);
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsCoeff(p)) return (PolyIsZero(p) ? -1 : 0);
    else {
        poly_exp_t res = 0;
        for (size_t i = 0; i < p->size; i++)
            res = MAX(res, MonoDeg(&p->arr[i]));
        return res;
    }
}

/** Sprawdza, czy dwa jednomiany są sobie równe
 * @param[in] m: jednomian @f$m@f$
 * @param[in] n: jednomian @f$n@f$
 * @return @f$m=n@f$
 */
static bool MonoIsEq(const Mono *m, const Mono *n) {
    return m->exp == n->exp && PolyIsEq(&m->p, &n->p);
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    if (PolyIsComplexCoeff(p) && PolyIsComplexCoeff(q)) 
        return PolyGetComplexCoeff(p) == PolyGetComplexCoeff(q);
    else if (!PolyIsComplexCoeff(p) && !PolyIsComplexCoeff(q)) {
        if (p->size != q->size) return false;
        for (size_t i = 0; i < p->size; i++) {
            if (!MonoIsEq(&p->arr[i], &q->arr[i])) return false;
        }
        return true;
    }
    return false;
}

/** Podnosi współczynnik @f$a@f$ do potęgi @f$n@f$-tej
 * @param[in] a: podstawa
 * @param[in] n: wykładnik
 * @return @f$a^n@f$
 */
static poly_coeff_t PowerCoeff(poly_coeff_t a, poly_exp_t n) {
    poly_coeff_t res = 1;
    while (n > 0) {
        if (n % 2 == 1) res *= a;
        a *= a;
        n /= 2;
    }
    return res;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    if (PolyIsCoeff(p)) return PolyClone(p);
    else {
        Mono *arr = malloc(p->size * sizeof(Mono));
        CHECK_PTR(arr);
        for (size_t i = 0; i < p->size; i++) {
            Poly coeff = PolyFromCoeff(PowerCoeff(x, p->arr[i].exp));
            Poly q = PolyMul(&coeff, &p->arr[i].p);
            arr[i] = MonoFromPoly(&q, 0);
        }
        Mono m = MonoAddMonos(p->size, arr);
        for (size_t i = 0; i < p->size; i++) MonoDestroy(&arr[i]);
        free(arr);
        Poly res = m.p;
        PolySimplify(&res);
        return res;
    }
}

Poly PolyOwnMonos(size_t count, Mono *monos) {
    Poly res = (Poly) { .size = count, .arr = monos };
    PolySimplify(&res);
    return res;
}

Poly PolyCloneMonos(size_t count, const Mono monos[]) {
    Mono *arr = malloc(count * sizeof(Mono));
    CHECK_PTR(arr);
    for (size_t i = 0; i < count; i++) arr[i] = MonoClone(&monos[i]);
    Poly res = (Poly) { .size = count, .arr = arr };
    PolySimplify(&res);
    return res;
}

Poly ExtendPoly(Poly *p, poly_exp_t exp) {
    Mono *m = malloc(sizeof(Mono));
    CHECK_PTR(m);
    *m = (Mono) { .p = *p, .exp = exp };
    return (Poly) { .size = 1, .arr = m };
}

/** Podnosi wielomian @f$p@f$ do potęgi @f$n@f$
 * @param[in] p: wielomian @f$p@f$
 * @param[in] n: wykładnik
 * @return @f$p^n@f$
 */
static Poly PolyPower(const Poly *p, poly_exp_t n) {
    if (PolyIsCoeff(p)) return PolyFromCoeff(PowerCoeff(p->coeff, n));
    else if (p->size == 1) {
        poly_exp_t exp = p->arr[0].exp * n;
        Poly q = PolyPower(&p->arr[0].p, n);
        Poly res = ExtendPoly(&q, exp);
        return res;
    }
    else {
        Poly res = PolyFromCoeff(1);
        for (poly_exp_t i = 0; i < n; i++) {
            Poly tmp = res;
            res = PolyMul(&tmp, p);
            PolyDestroy(&tmp);
        }
        return res;
    }
}

/** Zwraca wielomian będący wynikiem podstawienia q[0], q[1], ..., q[k - 1]
 * pod zmienne x_0,...,x_{k - 1} oraz zer pod pozostałe zmienne do jednomianu m
 * @param[in] m: jednomian
 * @param[in] k: liczba wielomianów do podstawienia
 * @param[in] q: tablica wielomianów do podstawienia
 * @return wynik podstawienia q[i] za x_i w jednomianie m
 */
static Poly MonoCompose(const Mono *m, size_t k, const Poly q[]) {
    Poly s = PolyPower(&q[0], m->exp);
    Poly t = PolyCompose(&m->p, k - 1, q + 1);
    Poly res = PolyMul(&s, &t);
    PolyDestroy(&s);
    PolyDestroy(&t);
    return res;
}

Poly PolyCompose(const Poly *p, size_t k, const Poly q[]) {
    if (PolyIsCoeff(p)) return PolyFromCoeff(p->coeff);
    else if (k == 0) {
        Poly z = PolyZero();
        return PolyCompose(p, 1, &z);
    }
    else {
        Poly res = PolyZero();
        for (size_t i = 0; i < p->size; i++) {
            Poly cmp = MonoCompose(&p->arr[i], k, q);
            Poly tmp = res;
            res = PolyAdd(&tmp, &cmp);
            PolyDestroy(&cmp);
            PolyDestroy(&tmp);
        }
        return res;
    }
}