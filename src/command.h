/** @file
  Interfejs parsowania ciągów znaków do komend wywoływanych
  na stosie wielomianów rzadkich wielu zmiennych

  @author Wiktor Chmielewski <wc429131@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef POLY_COMMAND_H
#define POLY_COMMAND_H
#include "stack.h"

/** Dodaje wielomian będący współczynnikiem na wierzch stosu
 * @param[in] s: ciąg znaków, w którym zakodowany jest wielomian
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void Coeff(char *s, Stack *stack, size_t line);

/** Dodaje wielomian nie będący współczynnikiem na wierzch stosu
 * @param[in] s: ciąg znaków, w którym zakodowany jest wielomian
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void NonCoeff(char *s, Stack *stack, size_t line);

/** Dodaje wielomian tożsamościowo równy 0 na wierzch stosu
 * @param[in] stack: stos, na którym odbędzie się operacja
 */
void Zero(Stack *stack);

/** Sprawdza, czy wielomian na wierzchu stosu jest współczynnikiem
 * i w takim przypadku wypisuje 1 na wyjście standardowe, a w przeciwnym 
 * wypisuje 0
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void IsCoeff(Stack *stack, size_t line);

/** Sprawdza, czy wielomian na wierzchu stosu jest tożsamościowo równy 0
 * i w takim przypadku wypisuje 1 na wyjście standardowe, a w przeciwnym 
 * wypisuje 0
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void IsZero(Stack *stack, size_t line);

/** Dodaje na wierzch stosu wielomian będący aktualnie na wierzchu
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void Clone(Stack *stack, size_t line);

/** Dodaje dwa wierzchnie wielomiany ze stosu i wynik dodaje na wierzch,
 * uprzednio usuwając dodawane wielomiany
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void Add(Stack *stack, size_t line);

/** Mnoży dwa wierzchnie wielomiany ze stosu i wynik dodaje na wierzch,
 * uprzednio usuwając mnożone wielomiany
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void Mul(Stack *stack, size_t line);

/** Neguje wierzchni wielomian ze stosu
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void Neg(Stack *stack, size_t line);

/** Odejmuje dwa wierzchnie wielomiany ze stosu i wynik dodaje na wierzch,
 * uprzednio usuwając odejmowane wielomiany
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void Sub(Stack *stack, size_t line);

/** Sprawdza, czy dwa wierzchnie wielomiany ze stosu są tożsamościowo równe
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void IsEq(Stack *stack, size_t line);

/** Wypisuje na standardowe wyjście stopień wierzchniego wielomianu ze stosu
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void Deg(Stack *stack, size_t line);

/** Wypisuje na standardowe wyjście stopień wierzchniego wielomianu ze stosu
 * ze względu na zmienną zakodowaną w s
 * @param[in] s: ciąg znaków, w którym zakodowana jest zmienna
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void DegBy(char *s, Stack *stack, size_t line);

/** Zamienia wierzchni wielomian stosu na powstały poprzez podstawienie
 * zmiennej zakodowanej w s za x_0
 * @param[in] s: ciąg znaków, w którym zakodowana jest zmienna
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void At(char *s, Stack *stack, size_t line);

/** Wypisuje wierzchni wielomian stosu na standardowe wyjście
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void Print(Stack *stack, size_t line);

/** Usuwa wierzchni wielomian ze stosu
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void Remove(Stack *stack, size_t line);

/** Zdejmuje z wierzchołka stosu najpierw wielomian @f$p@f$, a potem kolejno 
 * wielomiany @f$q_{k - 1}@f$, @f$q_{k - 2}@f$, …, @f$q_0@f$ i umieszcza na
 * stosie wynik operacji złożenia
 * @param[in] s: ciąg znaków, odpowiadający wywołaniu operacji
 * @param[in] stack: stos, na którym odbędzie się operacja
 * @param[in] line: numer linii
 */
void Compose(char *s, Stack *stack, size_t line);

#endif // POLY_COMMAND_H