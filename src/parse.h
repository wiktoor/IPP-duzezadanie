/** @file
  Interfejs służący do parsowania ciągów znaków 
  do wielomianów rzadkich wielu zmiennych

  @author Wiktor Chmielewski <wc429131@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef PARSE_H
#define PARSE_H

#include "poly.h"
#include <string.h>

/** Ustawia wartość *correct na false, a następnie zwraca PolyZero()
 */
#define PARSE_ERROR() \
  do {  \
    *correct = false; \
    return PolyZero();   \
  } while (0) 

/** Ustawia wartość *correct na false, a następnie zwraca 0
 */
#define CONVERT_ERROR() \
  do {  \
    *correct = false; \
    return 0;   \
  } while (0) 

/** Wypisuje na stderr komunikat WRONG COMMAND, a następnie wywołuje return
 */
#define WRONG_COMMAND() \
    do { \
        fprintf(stderr, "ERROR %ld WRONG COMMAND\n", line); \
        return; \
    } while(0)

/** Wypisuje na stderr komunikat WRONG POLY, a następnie wywołuje return
 */
#define WRONG_POLY() \
    do { \
        fprintf(stderr, "ERROR %ld WRONG POLY\n", line); \
        return; \
    } while(0)

/** Wypisuje na stderr komunikat STACK UNDERFLOW, a następnie wywołuje return
 */
#define STACK_UNDERFLOW() \
    do { \
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line); \
        return; \
    } while(0)

/** Wypisuje na stderr komunikat WRONG VARIABLE, a następnie wywołuje return
 */
#define WRONG_VARIABLE() \
    do { \
        fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", line); \
        return; \
    } while(0)

/** Wypisuje na stderr komunikat WRONG VALUE, a następnie wywołuje return
 */
#define WRONG_VALUE() \
    do { \
        fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", line); \
        return; \
    } while(0)

/** Wypisuje na stderr komunikat WRONG PARAMETER, a nstępnie wywołuje return
 */
#define WRONG_PARAMETER() \
    do { \
        fprintf(stderr, "ERROR %ld COMPOSE WRONG PARAMETER\n", line); \
        return; \
    } while(0)


/** Parsuje ciąg znaków rozpoczynający się w s o długości length
 * do wielomianu nie będącego stałą
 * Jeżeli konwersja się nie udała, ustawia wartość *correct na false
 * a następnie zwraca wielomian tożsamościowo równy 0
 * @param[in] s: ciąg znaków
 * @param[in] length: długość ciągu znaków
 * @param[in] correct: wskaźnik na zmienną typu bool przechowującą
 * sukces operacji
 * @return sparsowany wielomian
 */
Poly ParsePoly(char *s, size_t length, bool *correct);

/** Parsuje ciąg znaków rozpoczynający się w s o długości length
 * do wielomianu będącego stałą
 * Jeżeli konwersja się nie udała, ustawia wartość *correct na false
 * a następnie zwraca wielomian tożsamościowo równy 0
 * @param[in] s: ciąg znaków
 * @param[in] length: długość ciągu znaków
 * @param[in] correct: wskaźnik na zmienną typu bool przechowującą
 * sukces operacji
 * @return sparsowany wielomian
 */
Poly ParseCoeffPoly(char *s, size_t length, bool *correct);

/** Sprawdza, czy ciąg znaków zaczynający się w s o długości length zawiera
 * znak inny niż cyfra bądź '-'
 * @param[in] s: ciąg znaków
 * @param[in] length: długość ciągu znaków
 * @return Czy zawiera znak inny niż cyfra bądź '-'?
 */
bool ContainsNoNumNoMinus(char *s, size_t length);

/** Sprawdza, czy ciąg znaków zaczynający się w s o długości length zawiera
 * znak inny niż cyfra
 * @param[in] s: ciąg znaków
 * @param[in] length: długość ciągu znaków
 * @return Czy zawiera znak inny niż cyfra?
 */
bool ContainsNoNum(char *s, size_t length);

/** Konwertuje ciąg znaków do współczynnika. Jeżeli konwersja się nie udała,
 * ustawia wartość *correct na false, a następnie zwraca 0
 * @param[in] s: ciąg znaków
 * @param[in] length: długość ciągu znaków
 * @param[in] correct: wskaźnik na zmienną typu bool przechowującą
 * sukces operacji
 * @return skonwertowany ciąg znaków
 */
poly_coeff_t ConvertStringToCoeff(char *s, size_t length, bool *correct);

/** Konwertuje ciąg znaków do wykładnika. Jeżeli konwersja się nie udała,
 * ustawia wartość *correct na false, a następnie zwraca 0
 * @param[in] s: ciąg znaków
 * @param[in] length: długość ciągu znaków
 * @param[in] correct: wskaźnik na zmienną typu bool przechowującą
 * sukces operacji
 * @return skonwertowany ciąg znaków
 */
poly_exp_t ConvertStringToExp(char *s, size_t length, bool *correct);

/** Konwertuje ciąg znaków do liczby typu unsigned long. 
 * Jeżeli konwersja się nie udała, ustawia wartość *correct na false,
 * a następnie zwraca 0
 * @param[in] s: ciąg znaków
 * @param[in] length: długość ciągu znaków
 * @param[in] correct: wskaźnik na zmienną typu bool przechowującą
 * sukces operacji
 * @return skonwertowany ciąg znaków
 */
size_t ConvertStringToAt(char *s, size_t length, bool *correct);

#endif // PARSE_H