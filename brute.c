/*
 * Copyright (c) 2019 Daniel W. Crompton <bfl@specialbrands.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

char *nextpw(char *prev) {
  char *local = prev;

  while(1) {
    if((*local) < 32) {
      (*local) = ' ';
      goto end;
    }
    if((*local) != 126) {
      (*local)++;
      goto end;
    } else {
      *local = ' ';
      local++;
    }
  }

end:
  return prev;
}

#ifdef TEST
void testStringEquals( const char *msg, const char *expected, const char *actual);
void testStringNotEquals( const char *msg, const char *expected, const char *actual);
char *print_hex(const char *s);

int main() {
  char *testval = calloc(255,sizeof(char));
  char *actual;
  
  *testval = 'a';
  actual = nextpw(testval);
  testStringEquals("One char", "b", actual);

  strcpy(testval, "ba");
  actual = nextpw(testval);
  testStringEquals("Two char", "ca", actual);

  strcpy(testval, "~a");
  actual = nextpw(testval);
  testStringEquals("Rollover", " b", actual);

  strcpy(testval, "~aa");
  actual = nextpw(testval);
  testStringEquals("Rollover long", " ba", actual);

  strcpy(testval, "~~a");
  actual = nextpw(testval);
  testStringEquals("Rollover twice", "  b", actual);

  strcpy(testval, "AOEUI");
  for(int i = 0; i < ((255^2)*100); i++ ) {
    actual = nextpw(testval);
    strcpy(testval, actual);
  }
  testStringEquals("Rollover", "_<HUI", actual);

  strncpy(testval, "\0\0", 2);
  actual = nextpw(testval);
  testStringEquals("NULL string", " ", actual);

  strncpy(testval, "~\0\0\0", 4);
  actual = nextpw(testval);
  testStringEquals("NULL string", "  ", actual);

  return 0;
}

void testStringEquals( const char *msg, const char *expected, const char *actual) {
  if(strcmp( expected, actual) != 0) {
    printf("%s: expected '%s' (%s) to equal '%s' (%s)\n", msg, expected, print_hex(expected), actual, print_hex(actual));
    exit(-1);
  }
}

void testStringNotEquals( const char *msg, const char *expected, const char *actual) {
  if(strcmp( expected, actual) == 0) {
    printf("%s: expected '%s' (%s) to not equal '%s' (%s)\n", msg, expected, print_hex(expected), actual, print_hex(actual));
    exit(-1);
  }
}

char *print_hex(const char *s) {
  char *retval = calloc((strlen(s)*2)+1, sizeof(char));
  while(*s)
    sprintf(retval, "%02x", (unsigned int) *s++);
  return retval;
}

#endif /* TEST */
