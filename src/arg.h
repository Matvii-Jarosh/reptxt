/*
Copyright (C) 2024 Matvii Jarosh

This file is part of reptxt.

reptxt is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

reptxt is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with reptxt. If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef ARG_H
#define ARG_H

#include <stdio.h>
#include <argp.h>

#define INITIAL_BUFFER_SIZE 1024

struct arguments {
  char *args[2];
  int verbose;
  int except;
  int interactive;
  int unique_output;
  char *outfile;
  FILE **files;
  int file_count;
};

extern const char *argp_program_version;
extern const char *argp_program_bug_address;
extern struct argp argp;

error_t
parse_opt(int key, char *arg, struct argp_state *state);

#endif
