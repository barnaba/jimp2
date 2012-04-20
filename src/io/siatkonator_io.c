#include "siatkonator_io.h"
#include "node.h"
#include "poly.h"
#include "ele.h"
#include "segments.h"

/*
 *change 'filename.ele' to 'filename.node'
 */
int replace_ele_with_node(char *node_filename, const char * ele_filename){
  int len = strlen(ele_filename);

  /* We use INPUTLINESIZE - 1 here, because .node suffix is one char longer */
  /* than .ele suffix*/
  if (len > INPUTLINESIZE - 1){
    siatkonator_log(INFO, "filename too long: %s\n", node_filename);
    return ARG_ERR;
  }

  strncpy(node_filename, ele_filename, INPUTLINESIZE-1);
  node_filename[INPUTLINESIZE - 1] = '\0';
  strncpy(node_filename + len - 3, "node", strlen("node"));

  return SUCCESS;
}

int ele_to_triangulateio(const char *ele_filename, triangulateio *input){
  int error_value = 0;
  int firstnode = 0;
  char node_filename[INPUTLINESIZE];

  error_value = replace_ele_with_node(node_filename, ele_filename);
  siatkonator_log(INFO, "reading node file: %s\n", node_filename);
  if (error_value != SUCCESS)
    return error_value;
  error_value = read_node_file(node_filename, input, &firstnode);
  if (error_value != SUCCESS)
    return error_value;

  siatkonator_log(INFO, "reading ele file: %s\n", ele_filename);
  error_value = read_ele_file(ele_filename, input, firstnode);
  if (error_value != SUCCESS)
    return error_value;

  input->numberofcorners = 3;
  
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

