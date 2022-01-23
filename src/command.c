/** @file
  Implementacja interfejsu parsowania ciągów znaków do komend wywoływanych
  na stosie wielomianów rzadkich wielu zmiennych

  @author Wiktor Chmielewski <wc429131@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "stack.h"
#include "parse.h"
#include <stdio.h>

/** Zwraca długość wielomianu
 * @param[in] poly: wielomian
 * @return długość poly
 */
static inline size_t PolyLength(const Poly *poly) {
    return poly->size;
}

// zdefiniowane poniżej
static void PrintPoly(const Poly *poly);

/** Wyświetla jednomian zmiennej x_{arg_num}
 *  @param[in] mono: jednomian @f$mono@f$
 */
static void PrintMono(const Mono *mono) {
  printf("(");
  PrintPoly(&mono->p);
  printf(",");
  printf("%d", mono->exp);
  printf(")");
}

/** Wyświetla wielomian zmiennej x_{arg_num}
 * @param[in] poly: wielomian @f$poly@f$
 */
static void PrintPoly(const Poly *poly) {
    if (PolyIsComplexCoeff(poly)) {
        printf("%ld", PolyGetComplexCoeff(poly));
    }
    else {
        for (size_t k = 0; k < PolyLength(poly); ++k) {
            PrintMono(&poly->arr[k]);
            if (k < PolyLength(poly) - 1) printf("+");
        }
    }
}

void Coeff(char *s, Stack *stack, size_t line) {
    bool correct = true;
    Poly p = ParseCoeffPoly(s, strlen(s), &correct);
    if (correct) Push(stack, &p);
    else {
        PolyDestroy(&p);
        WRONG_POLY();
    }
}

void NonCoeff(char *s, Stack *stack, size_t line) {
    bool correct = true;
    Poly p = ParsePoly(s, strlen(s), &correct);
    if (correct) Push(stack, &p);
    else {
        PolyDestroy(&p);
        WRONG_POLY();
    }
}

void Zero(Stack *stack) {
    Poly p = PolyZero();
    Push(stack, &p);
}

void IsCoeff(Stack *stack, size_t line) {
    if (IsEmpty(stack)) STACK_UNDERFLOW();
    Poly p = PeekFst(stack);
    printf("%s", (PolyIsComplexCoeff(&p) ? "1\n" : "0\n"));
}

void IsZero(Stack *stack, size_t line) {
    if (IsEmpty(stack)) STACK_UNDERFLOW();
    Poly p = PeekFst(stack);
    printf("%s", (PolyIsZero(&p) ? "1\n" : "0\n"));
}

void Clone(Stack *stack, size_t line) {
    if (IsEmpty(stack)) STACK_UNDERFLOW();
    Poly p = PeekFst(stack);
    Poly q = PolyClone(&p);
    Push(stack, &q);
}

void Add(Stack *stack, size_t line) {
    if (Size(stack) < 2) STACK_UNDERFLOW();
    Poly p = PeekFst(stack);
    Poly q = PeekSnd(stack);
    Poly r = PolyAdd(&p, &q);
    Pop(stack);
    Pop(stack);
    Push(stack, &r);
}

void Mul(Stack *stack, size_t line) {
    if (Size(stack) < 2) STACK_UNDERFLOW();
    Poly p = PeekFst(stack);
    Poly q = PeekSnd(stack);
    Poly r = PolyMul(&p, &q);
    Pop(stack);
    Pop(stack);
    Push(stack, &r);
}

void Neg(Stack *stack, size_t line) {
    if (IsEmpty(stack)) STACK_UNDERFLOW();
    Poly p = PeekFst(stack);
    Poly q = PolyNeg(&p);
    Pop(stack);
    Push(stack, &q);
}

void Sub(Stack *stack, size_t line) {
    if (Size(stack) < 2) STACK_UNDERFLOW();
    Poly p = PeekFst(stack);
    Poly q = PeekSnd(stack);
    Poly r = PolySub(&p, &q);
    Pop(stack);
    Pop(stack);
    Push(stack, &r);
}

void IsEq(Stack *stack, size_t line) {
    if (Size(stack) < 2) STACK_UNDERFLOW();
    Poly p = PeekFst(stack);
    Poly q = PeekSnd(stack);
    printf("%s", (PolyIsEq(&p, &q) ? "1\n" : "0\n"));
}

void Deg(Stack *stack, size_t line) {
    if (IsEmpty(stack)) STACK_UNDERFLOW();
    Poly p = PeekFst(stack);
    printf("%d\n", PolyDeg(&p));
}

void DegBy(char *s, Stack *stack, size_t line) {
    if (strlen(s) <= 7) WRONG_VARIABLE();
    if (s[6] == '\t') WRONG_VARIABLE();
    if (s[6] != ' ') WRONG_COMMAND();
    if (ContainsNoNum(s + 7, strlen(s) - 7)) WRONG_VARIABLE();
    else if (strlen(s + 7) == 1 && *(s + 7) == '0') {
        if (IsEmpty(stack)) STACK_UNDERFLOW();
        Poly p = PeekFst(stack);
        printf("%d\n", PolyDegBy(&p, 0));
    }
    else {
        bool correct = true;
        size_t idx = ConvertStringToAt(s + 7, strlen(s) - 7, &correct);
        if (!correct) WRONG_VARIABLE();
        else {
            if (IsEmpty(stack)) STACK_UNDERFLOW();
            Poly p = PeekFst(stack);
            printf("%d\n", PolyDegBy(&p, idx));
        }
    }
}

void At(char *s, Stack *stack, size_t line) {
    if (strlen(s) < 4) WRONG_VALUE();
    if (s[2] == '\t') WRONG_VALUE();
    if (s[2] != ' ') WRONG_COMMAND();
    if (ContainsNoNumNoMinus(s + 3, strlen(s) - 3)) WRONG_VALUE();
    if (IsEmpty(stack)) STACK_UNDERFLOW();
    bool correct = true;
    poly_coeff_t val = ConvertStringToCoeff(s + 3, strlen(s) - 3, &correct);
    if (!correct) WRONG_VALUE();
    Poly p = PeekFst(stack);
    Poly q = PolyAt(&p, val);
    Pop(stack);
    Push(stack, &q);
}

void Print(Stack *stack, size_t line) {
    if (IsEmpty(stack)) STACK_UNDERFLOW();
    Poly p = PeekFst(stack);
    PrintPoly(&p);
    printf("\n");
}

void Remove(Stack *stack, size_t line) {
    if (IsEmpty(stack)) STACK_UNDERFLOW();
    Pop(stack);
}

void Compose(char *s, Stack *stack, size_t line) {
    if (strlen(s) < 9) WRONG_PARAMETER();
    if (s[7] != ' ') WRONG_COMMAND();
    if (ContainsNoNum(s + 8, strlen(s) - 8)) WRONG_PARAMETER();
    bool correct = true;
    size_t k = ConvertStringToAt(s + 8, strlen(s) - 8, &correct);
    if (!correct) WRONG_PARAMETER();
    if (k != 0 && Size(stack) - 1 < k) STACK_UNDERFLOW();
    Poly p = PeekFst(stack);
    Poly *q = malloc(k * sizeof(struct Poly));
    CHECK_PTR(q);
    for (size_t i = 0; i < k; i++) {
        q[i] = Peek(stack, k + 1 - i);
    }
    Poly res = PolyCompose(&p, k, q);
    for (size_t i = 0; i <= k; i++) Pop(stack);
    Push(stack, &res);
    free(q);
}