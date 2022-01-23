/** @file
  Rozwiązanie trzeciego zadania zaliczeniowego

  @author Wiktor Chmielewski <wc429131@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#define _GNU_SOURCE
#include "poly.h"
#include "command.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/** Sprawdza, czy znak jest literą (wielką lub małą)
 * @param[in] c: znak
 * @return czy c jest literą?
 */
static bool IsLetter(char c) {
  if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) return true;
  return false;
}

/** Sprawdza, czy ciągi znaków s1 i s2 zgadzają się
 *  na pierwszych length znakach
 * @param[in] s1: ciąg znaków
 * @param[in] s2: ciąg znaków
 * @param[in] length: długość do porównania
 * @return czy ciągi znaków są takie same?
 */
static bool MatchingStrings(char *s1, char *s2, size_t length) {
  for (size_t i = 0; i < length; i++) {
    if (s1[i] != s2[i]) return false;
  }
  return true;
}

/** Wykonuje odpowiednie polecenie na stosie
 * @param[in] s: ciąg znaków odpowiadający poleceniu
 * @param[in] stack: stos, na którym polecenie zostanie wywołane
 * @param[in] line: numer linii
 */
static void ExecuteLine(char *s, Stack *stack, size_t line) {
  if (strlen(s) == 0 || s[0] == '#') return;
  else if (!ContainsNoNumNoMinus(s, strlen(s))) Coeff(s, stack, line);
  else if (s[0] == '(') NonCoeff(s, stack, line);
  else if (strcmp(s, "ZERO") == 0) Zero(stack);
  else if (strcmp(s, "IS_COEFF") == 0) IsCoeff(stack, line);
  else if (strcmp(s, "IS_ZERO") == 0) IsZero(stack, line);
  else if (strcmp(s, "CLONE") == 0) Clone(stack, line);
  else if (strcmp(s, "ADD") == 0) Add(stack, line);
  else if (strcmp(s, "MUL") == 0) Mul(stack, line);
  else if (strcmp(s, "NEG") == 0) Neg(stack, line);
  else if (strcmp(s, "SUB") == 0) Sub(stack, line);
  else if (strcmp(s, "IS_EQ") == 0) IsEq(stack, line);
  else if (strcmp(s, "DEG") == 0) Deg(stack, line);
  else if (strlen(s) >= 6 && MatchingStrings(s, "DEG_BY", 6)) 
    DegBy(s, stack, line);
  else if (strlen(s) >= 2 && MatchingStrings(s, "AT", 2)) At(s, stack, line);
  else if (strcmp(s, "PRINT") == 0) Print(stack, line);
  else if (strcmp(s, "POP") == 0) Remove(stack, line);
  else if (strlen(s) >= 7 && MatchingStrings(s, "COMPOSE", 7)) 
    Compose(s, stack, line);
  else if (IsLetter(s[0])) WRONG_COMMAND();
  else WRONG_POLY();
}


/** Uruchamia kalkulator wielomianów rzadkich wielu zmiennych
 */
int main() {
  Stack stack = Create();

  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  size_t num = 0;

  while ((nread = getline(&line, &len, stdin)) != -1) {
    num++;
    // sprawdzamy obecność białych znaków
    if ((size_t) nread != strlen(line)) {
      if (strlen(line) > 0 && IsLetter(line[0])) {
        fprintf(stderr, "ERROR %ld WRONG COMMAND\n", num); 
        continue;
      }
      else {
        fprintf(stderr, "ERROR %ld WRONG POLY\n", num); 
        continue;
      }
    }
    if (line[nread - 1] == '\n') {
      line[nread - 1] = '\0';
    }
    ExecuteLine(line, &stack, num);
  }
  Destroy(&stack);
  free(line);
}