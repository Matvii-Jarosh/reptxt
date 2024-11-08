/*
Copyright (C) 2024 Matvii Jarosh

This file is part of reptxt.

reptxt is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

reptxt is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with reptxt. If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef REPLACE_H
#define REPLACE_H

#include <stdio.h>
#include "arg.h"

int
replace_text(FILE *input, FILE *output, const char *old, const char *new,
	     struct arguments *arguments);

#endif /* REPLACE_H   */
