/*
Copyright (C) 2024 Matvii Jarosh

This file is part of reptxt.

reptxt is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

reptxt is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with reptxt. If not, see <https://www.gnu.org/licenses/>.
*/
/*
  arg.c - parse command line arguments  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include "arg.h"

const char *argp_program_version = "reptxt 1.0";
const char *argp_program_bug_address = "matviijarosh@gmail.com";

static struct argp_option options[] = {
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"output", 'o', "OUTFILE", 0, "Output to OUTFILE instead of to standard output"},
  {"except", 'x', 0, 0, "Replace everything new except old"},
  {"unique-output", 'u', 0, 0, "Outputs the data each to its own file"},
  {0}
};

static char args_doc[] = "OLD NEW [FILE]...";
static char doc[] = "reptxt - replace old text with new";

struct argp argp = {options, parse_opt, args_doc, doc};

/* parse commanf line arguments  */
error_t
parse_opt(int key, char *arg, struct argp_state *state)
{
  FILE *file;
  struct arguments *arguments = state->input;
  
  switch (key)
    {
    case 'v': /* verbose  */
      arguments->verbose = 1;
      break;
    case 'o': /* output  */
      arguments->outfile = arg;
      break;
    case 'x': /* except  */
      arguments->except = 1;
      break;
    case 'i': /* interactive  */
      arguments->interactive = 1;
      break;
    case 'u': /* unique-output  */
      arguments->unique_output = 1;
      break;
    case ARGP_KEY_ARG: /* OLD NEW  */
      if (state->arg_num < 2)	
	  arguments->args[state->arg_num] = arg;	
      else /* files  */
	{
	  if (arguments->file_count == 0)
            arguments->files = malloc(sizeof(FILE *) * INITIAL_BUFFER_SIZE);
          else if (arguments->file_count >= INITIAL_BUFFER_SIZE)
            arguments->files = realloc(arguments->files, sizeof(FILE *) * (size_t)(arguments->file_count + 1));

          file = fopen(arg, "r+");
          if (!file)
            {
              fprintf(stderr, "Error opening file: %s\n", arg);
              exit(EXIT_FAILURE);
            }
          arguments->files[arguments->file_count] = file;
          arguments->file_count++;
	}
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 2)	
	argp_usage(state);	
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

