/** @file
  Implementacja interfejsu służącego do parsowania ciągów znaków 
  do wielomianów rzadkich wielu zmiennych

  @author Wiktor Chmielewski <wc429131@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "poly.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>

/** Najmniejsza długość wielomianu postaci (a,b),
 * gdzie a,b są liczbami całkowitymi
 */
#define MIN_SIMPLE_POLY_LENGTH 5

bool ContainsNoNum(char *s, size_t length) {
  for (size_t i = 0; i < length; i++) {
    if (s[i] < '0' || s[i] > '9') return true;
  }
  return false;
}

bool ContainsNoNumNoMinus(char *s, size_t length) {
  for (size_t i = 0; i < length; i++) {
    if ((s[i] < '0' || s[i] > '9') && s[i] != '-') return true;
  }
  return false;
}

/** Zwraca, czy ciąg znaków zaczynający się w s o długości length zawiera '+'
 * @param[in] s: początek ciągu znaków
 * @param[in] length: długość ciągu znaków
 * @return czy zawiera '+'?
 */
static bool ContainsPlus(char *s, size_t length) {
  for (size_t i = 0; i < length; i++) {
    if (s[i] == '+') return true;
  }
  return false;
}

/** Podnosi a do potęgi pow, ustawia wartość *correct na false
 * jeżeli konwersja się nie powiodła
 * @param[in] a: podstawa
 * @param[in] pow: wykładnik
 * @param[in] correct: wskaźnik na zmienną typu bool przechowującą
 * sukces operacji
 * @return a^{pow}
 */
static size_t PowerAt(size_t a, size_t pow, bool *correct) {
  size_t res = 1;
  while (pow > 0) {
    if (pow % 2 == 1) {
      if (__builtin_umull_overflow(res, a, &res)) CONVERT_ERROR();
    }
    pow /= 2;
    if (pow > 0) {
      if (__builtin_umull_overflow(a, a, &a)) CONVERT_ERROR();
    }
  }
  return res;
}

size_t ConvertStringToAt(char *s, size_t length, bool *correct) {
  size_t res = 0;
  for (size_t i = 0; i < length; i++) {
    size_t comp = PowerAt(10, i, correct);
    if (!(*correct)) CONVERT_ERROR();
    size_t digit = (size_t) (s[length - 1 - i] - '0');
    if (__builtin_umull_overflow(comp, digit, &comp)) CONVERT_ERROR();
    if (__builtin_uaddl_overflow(comp, res, &res)) CONVERT_ERROR();
  }
  return res;
}

/** Podnosi a do potęgi pow, ustawia wartość *correct na false
 * jeżeli konwersja się nie powiodła
 * @param[in] a: podstawa
 * @param[in] pow: wykładnik
 * @param[in] correct: wskaźnik na zmienną typu bool przechowującą
 * sukces operacji
 * @return a^{pow}
 */
static poly_coeff_t PowerCoeff(poly_coeff_t a, size_t pow, bool *correct) {
  poly_coeff_t res = 1;
  while (pow > 0) {
    if (pow % 2 == 1) {
      if (__builtin_smull_overflow(res, a, &res)) CONVERT_ERROR();
    }
    pow /= 2;
    if (pow > 0) {
        if (__builtin_smull_overflow(a, a, &a)) CONVERT_ERROR();
    }
  }
  return res;
}

poly_coeff_t ConvertStringToCoeff(char *s, size_t length, bool *correct) {
  bool minus = false;
  size_t start = 0;
  if (s[0] == '-') {
    if (length == 1 || ContainsNoNum(s + 1, length - 1)) CONVERT_ERROR();
    if (length > 2 && s[1] == '0') CONVERT_ERROR();
    minus = true;
    start = 1;
  }
  poly_coeff_t res = 0;
  for (size_t i = 0; i < length - start; i++) {
    poly_coeff_t comp = PowerCoeff(10, i, correct);
    if(!(*correct)) CONVERT_ERROR();
    if (minus) {
      if (__builtin_smull_overflow(comp, -1, &comp)) CONVERT_ERROR();
    }
    poly_coeff_t digit = (poly_coeff_t) (s[length - 1 - i] - '0');
    if (__builtin_smull_overflow(comp, digit, &comp)) CONVERT_ERROR();
    if (__builtin_saddl_overflow(comp, res, &res)) CONVERT_ERROR();
  }
  return res;
}

/** Podnosi a do potęgi pow, ustawia wartość *correct na false
 * jeżeli konwersja się nie powiodła
 * @param[in] a: podstawa
 * @param[in] pow: wykładnik
 * @param[in] correct: wskaźnik na zmienną typu bool przechowującą
 * sukces operacji
 * @return a^{pow}
 */
static poly_exp_t PowerExp(poly_exp_t a, size_t pow, bool *correct) {
  poly_exp_t res = 1;
  while (pow > 0) {
    if (pow % 2 == 1) {
      if (__builtin_smul_overflow(res, a, &res)) CONVERT_ERROR();
    }
    pow /= 2;
    if (pow > 0) {
      if (__builtin_smul_overflow(a, a, &a)) CONVERT_ERROR();
    }
  }
  return res;
}

poly_exp_t ConvertStringToExp(char *s, size_t length, bool *correct) {
  poly_exp_t res = 0;
  for (size_t i = 0; i < length; i++) {
    poly_exp_t comp = PowerExp(10, i, correct);
    poly_exp_t digit = (poly_exp_t) (s[length - 1 - i] - '0');
    if (__builtin_smul_overflow(comp, digit, &comp)) CONVERT_ERROR();
    if (__builtin_sadd_overflow(comp, res, &res)) CONVERT_ERROR();
  }
  return res;
}

/** Zwraca wskaźnik na ostatni znak c w ciągu znaków rozpoczynającym się w s
 * o długości length
 * @param[in] s: początek ciągu znaków
 * @param[in] length: długość ciągu znaków
 * @param[in] c: poszukiwany znak
 * @return wskaźnik na poszukiwany znak
 */
static char *FindLast(char *s, size_t length, char c) {
  char *p = s + length - 1;
  while (p >= s) {
    if (*p == c) return p;
    p--;
  }
  return NULL;
}

/** Parsuje ciąg znaków rozpoczynający się w s o długości length
 * do wielomianu postaci (...,...) niezawierającego znaku +
 * Jeżeli konwersja się nie udała, ustawia wartość *correct na false
 * a następnie zwraca wielomian tożsamościowo równy 0
 * @param[in] s: ciąg znaków
 * @param[in] length: długość ciągu znaków
 * @param[in] correct: wskaźnik na zmienną typu bool przechowującą
 * sukces operacji
 * @return sparsowany wielomian
 */
static Poly ParseSimplePoly(char *s, size_t length, bool *correct) {
  if (!(*correct)) return PolyZero();
  if (length < MIN_SIMPLE_POLY_LENGTH ||
    s[0] != '(' || s[length - 1] != ')') PARSE_ERROR();
  char *comma = FindLast(s, length, ',');
  // error: brak przecinka
  if (comma == NULL) PARSE_ERROR();
  size_t left_length = comma - s - 1;
  size_t right_length = length - left_length - 3;
  char *left = s + 1;
  char *right = comma + 1;

  // error: pusta lewa lub prawa strona
  if (left_length == 0 || right_length == 0) PARSE_ERROR();

  // error: liczby zaczynające się na 0 i niebędące zerami
  if ((left_length > 1 && left[0] == '0') || 
    (right_length > 1 && right[0] == '0')) PARSE_ERROR();

  if (ContainsNoNum(right, right_length)) PARSE_ERROR();
  
  poly_exp_t exp = ConvertStringToExp(right, right_length, correct);
  if (!(*correct)) PARSE_ERROR();

  if (!ContainsNoNumNoMinus(left, left_length)) {
    poly_coeff_t coeff = ConvertStringToCoeff(left, left_length, correct);
    if (!(*correct)) PARSE_ERROR();
    if (coeff == 0) return PolyZero();
    if (exp == 0) return PolyFromCoeff(coeff);
    Poly q = PolyFromCoeff(coeff);
    Poly result = ExtendPoly(&q, exp);
    return result;
  }
  else {
    Poly q = ParseSimplePoly(left, left_length, correct);
    if (!(*correct)) {
      PolyDestroy(&q);
      PARSE_ERROR();
    }
    Poly result = ExtendPoly(&q, exp);
    return result;
  }
}

/** Zwraca długość następnego poprawnego wyrażenia nawiasowego w ciągu znaków
 * zaczynającym się w s o długości length. Jeżeli takie wyrażenie nie istnieje,
 * ustawia wartość *correct na false i zwraca 0
 * @param[in] s: ciąg znaków
 * @param[in] length: długość ciągu znaków
 * @param[in] correct: wskaźnik na zmienną typu bool przechowującą
 * sukces operacji
 * @return długość następnego poprawnego wyrażenia nawiasowego
 */
static size_t NextParenthesisExpressionLength(char *s, 
  size_t length, bool *correct) {
  size_t count = 0;
  size_t result = 0;
  for (size_t i = 0; i < length; i++) {
    result++;
    if (s[i] == '(') {
      count++;
    }
    else if (s[i] == ')') {
      if (count == 0) CONVERT_ERROR();
      else {
        count--;
      }
    }

    if (count == 0) {
      if (result == 0) CONVERT_ERROR();
      return result;
    }
  }
  CONVERT_ERROR();
}

Poly ParsePoly(char *s, size_t length, bool *correct);

/** Parsuje ciąg znaków rozpoczynający się w s o długości length
 * do wielomianu postaci ((...,...)+...+(...,...),...), tzn. takiego
 * który nie zawiera znaku '+' poza zewnętrznymi nawiasami
 * Jeżeli konwersja się nie udała, ustawia wartość *correct na false
 * a następnie zwraca wielomian tożsamościowo równy 0
 * @param[in] s: ciąg znaków
 * @param[in] length: długość ciągu znaków
 * @param[in] correct: wskaźnik na zmienną typu bool przechowującą
 * sukces operacji
 * @return sparsowany wielomian
 */
static Poly ParseComplexPoly(char *s, size_t length, bool *correct) {
  if (!ContainsPlus(s, length)) return ParseSimplePoly(s, length, correct);
  if (s[0] != '(' || s[length - 1] != ')') PARSE_ERROR();
  char *comma = FindLast(s, length, ',');
  if (comma == NULL) PARSE_ERROR();
  size_t left_lenght = comma - s - 1;
  size_t right_length = length - left_lenght - 3;
  char *left = s + 1;
  char *right = comma + 1;
  poly_exp_t exp = ConvertStringToExp(right, right_length, correct);
  if (!(*correct)) PARSE_ERROR();
  Poly q = ParsePoly(left, left_lenght, correct);
  if (!(*correct)) {
    PolyDestroy(&q);
    PARSE_ERROR();
  }
  Poly result = ExtendPoly(&q, exp);
  return result;
}

/** Niszczy wielomian result, a następnie wywołuje PARSE_ERROR()
 */
#define PARSE_ERROR_DESTROY() \
  do { \
    PolyDestroy(&result); \
    PARSE_ERROR(); \
  } while(0)

Poly ParsePoly(char *s, size_t length, bool *correct) {
  if (!ContainsPlus(s, length)) return ParseSimplePoly(s, length, correct);
  size_t dist = 0;
  Poly result = PolyZero();
  while (true) {
    size_t len = 
      NextParenthesisExpressionLength(s + dist, length - dist, correct);
    if (!(*correct)) PARSE_ERROR_DESTROY();
    Poly tmp = ParseComplexPoly(s + dist, len, correct);
    if (!(*correct)) PARSE_ERROR_DESTROY();
    Poly t = result;
    result = PolyAdd(&t, &tmp);
    PolyDestroy(&t);
    PolyDestroy(&tmp);
    dist += len;
    if (dist == length) return result;
    if (s[dist] != '+') PARSE_ERROR_DESTROY();
    dist++;
  }
  PARSE_ERROR_DESTROY();
}

Poly ParseCoeffPoly(char *s, size_t length, bool *correct) {
  poly_coeff_t coeff = ConvertStringToCoeff(s, length, correct);
  if (!(*correct)) return PolyZero();
  else return PolyFromCoeff(coeff);
}