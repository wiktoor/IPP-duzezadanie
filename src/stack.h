/** @file
  Interfejs stos wielomianów rzadkich wielu zmiennych

  @authors Wiktor Chmielewski <wc429131@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef STACK_H
#define STACK_H

#include "poly.h"

/** To jest struktura reprezentująca stos wielomianów rzadkich wielu zmiennych
 */
typedef struct Stack {
    /** To jest rozmiar stosu, tj. ile wielomianów jest na stosie */
    size_t size; 
    /** To jest pojemność stosu, tj. ile wielomianów może pomieścić stos */
    size_t capacity;
    /** To jest tablica przechowująca wielomiany ze stosu */
    Poly *arr;
} Stack;

/** Tworzy nowy, pusty stos
 * @return nowy, pusty stos
 */
Stack Create();

/** Zwraca rozmiar stosu
 * @param[in] s: stos @f$s@f$
 * @return rozmiar stosu @f$s@f$
 */
size_t Size(Stack *s);

/** Zwraca, czy stos jest pusty
 * @param[in] s: stos @f$s@f$
 * @return czy stos @f$s@f$ jest pusty?
 */
bool IsEmpty(Stack *s);

/** Dodaje wielomian na wierzch stosu
 * @param[in] s: stos @f$s@f$
 * @param[in] p: wielomian @f$p@f$ do dodania na wierzch stosu
 */
void Push(Stack *s, Poly *p);

/** Zwraca @f$n@f$-ty element z wierzchu stosu
 * @param[in] s: stos
 * @param[in] n: numer elementu
 * @return @f$n@f$-ty wielomian ze stosu
 */
Poly Peek(Stack *s, size_t n);

/** Zwraca wierzchni wielomian ze stosu
 * @param[in] s: stos
 * @return wierzchni wielomian ze stosu @f$s@f$
 */
Poly PeekFst(Stack *s);

/** Zwraca drugi z kolei wielomian ze stosu
 * @param[in] s: stos
 * @return drugi z kolei wielomian ze stosu @f$s@f$
 */
Poly PeekSnd(Stack *s);

/** Usuwa wierzchni wielomian ze stosu oraz niszczy ten wielomian
 * @param[in] s: stos
 */
void Pop(Stack *s);

/** Niszczy stos, usuwając z niego wszystkie wielomiany
 * @param[in] s: stos
 */
void Destroy(Stack *s);

#endif /* STACK_H */