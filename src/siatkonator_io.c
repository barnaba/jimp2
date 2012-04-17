#include "siatkonator_io.h"
#include "node.h"
#include "poly.h"
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <assert.h>


int read_ele(const char* filename, triangulateio* input);
int read_node(const char* filename, triangulateio* input);


int read_poly(const char* filename, triangulateio* input){
  int error_value = 0;
  FILE *polyfile;
  int markers;

  siatkonator_log("reading %s: \n", basename((char*)filename));

  polyfile = fopen(filename, "r");
  if (polyfile == (FILE *) NULL) {
    printf("  Error:  Cannot access file %s.\n", filename);
    return FILE_ERR;
  }

  error_value = read_nodes_header(polyfile, input, &markers);
  if (error_value != SUCCESS) return error_value;

  allocate_nodes(input);

  error_value = read_nodes(polyfile, input, &markers);
  if (error_value != SUCCESS) return error_value;

  report(input,0,0,0,0,0,0);

  return SUCCESS;
}

int write_ele(const char* filename, struct triangulateio* output);
int write_node(const char* filename, struct triangulateio* output);

/*
 *  Functions and structures copied from triangle with modifications.
 *  (not available when compiled as library, not available as functions
 *  or not in triangle headers file)
 */

/*
 *  Since Triangle isn't free software this is probably illegal.
 */


/*****************************************************************************/
/*                                                                           */
/*  findfield()   Find the next field of a string.                           */
/*                                                                           */
/*  Jumps past the current field by searching for whitespace, then jumps     */
/*  past the whitespace to find the next field.                              */
/*                                                                           */
/*****************************************************************************/
char *findfield(char *string)
{
  char *result;

  result = string;
  /* Skip the current field.  Stop upon reaching whitespace. */
  while ((*result != '\0') && (*result != '#')
      && (*result != ' ') && (*result != '\t')) {
    result++;
  }
  /* Now skip the whitespace and anything else that doesn't look like a */
  /*   number, a comment, or the end of a line.                         */
  while ((*result != '\0') && (*result != '#')
      && (*result != '.') && (*result != '+') && (*result != '-')
      && ((*result < '0') || (*result > '9'))) {
    result++;
  }
  /* Check for a comment (prefixed with `#'). */
  if (*result == '#') {
    *result = '\0';
  }
  return result;
}
/*****************************************************************************/
/*                                                                           */
/*  readline()   Read a nonempty line from a file.                           */
/*                                                                           */
/*  A line is considered "nonempty" if it contains something that looks like */
/*  a number.  Comments (prefaced by `#') are ignored.                       */
/*                                                                           */
/*****************************************************************************/

char *readline(char *string, FILE *infile, int *error)
{
  char *result;

  /* Search for something that looks like a number. */
  do {
    result = fgets(string, INPUTLINESIZE, infile);
    if (result == (char *) NULL) {
      *error = FORMAT_ERR;
    }
    /* Skip anything that doesn't look like a number, a comment, */
    /*   or the end of a line.                                   */
    while ((*result != '\0') && (*result != '#')
           && (*result != '.') && (*result != '+') && (*result != '-')
           && ((*result < '0') || (*result > '9'))) {
      result++;
    }
  /* If it's a comment or end of line, read another line and try again. */
  } while ((*result == '#') || (*result == '\0'));
  return result;
}

