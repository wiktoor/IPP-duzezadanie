/** @file
  Implementacja interfejsu stosu wielomianów rzadkich wielu zmiennych

  @authors Wiktor Chmielewski <wc429131@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "stack.h"

/** Początkowy rozmiar stosu
*/
#define CPCT 16

Stack Create() {
    Stack s = { .size = 0, .capacity = CPCT, 
        .arr = malloc(CPCT * sizeof(Poly)) };
    CHECK_PTR(s.arr);
    return s;
}

size_t Size(Stack *s) {
    return s->size;
}

bool IsEmpty(Stack *s) {
    return Size(s) == 0;
}

/** Zwraca, czy stos jest pełny
 * @param[in] s: stos @f$s@f$
 * @return czy stos @f$s@f$ jest pełny?
 */
static bool IsFull(Stack *s) {
    return s->size == s->capacity;
}

/** Rozszerza stos, powiększając jego pojemność dwukrotnie
 * @param[in] s: stos do rozszerzenia
 */
static void ExtendStack(Stack *s) {
    s->capacity *= 2;
    s->arr = realloc(s->arr, s->capacity * sizeof(Poly));
    CHECK_PTR(s->arr);
}

void Push(Stack *s, Poly *p) {
    if (IsFull(s)) ExtendStack(s);
    s->arr[s->size] = *p;
    s->size++;
}

Poly Peek(Stack *s, size_t n) {
    assert(Size(s) >= n);
    return s->arr[s->size - n];
}

Poly PeekFst(Stack *s) {
    return Peek(s, 1);
}

Poly PeekSnd(Stack *s) {
    return Peek(s, 2);
}

void Pop(Stack *s) {
    assert(!IsEmpty(s));
    PolyDestroy(&s->arr[Size(s) - 1]);
    s->size--;
}

void Destroy(Stack *s) {
    while (!IsEmpty(s)) Pop(s);
    free(s->arr);
    s->arr = NULL;
}