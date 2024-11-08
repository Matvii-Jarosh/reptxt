/*
Copyright (C) 2024 Matvii Jarosh

This file is part of reptxt.

reptxt is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

reptxt is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with reptxt. If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "arg.h"
#include "replace.h"

/* stream_rediraction  */
void stream_rediraction(FILE *input, FILE *output)
{
  char c;

  while ((c = (char)fgetc(input)) != EOF)
    fputc(c, output);
}

/* reptxt - replace old text with new  */
int
main(int argc, char **argv)
{
  int i;
  struct arguments arguments;
  FILE *outstream;

  arguments.outfile = NULL;
  arguments.verbose = 0;
  arguments.except = 0;
  arguments.interactive = 0;
  arguments.unique_output = 0;
  arguments.files = NULL;
  arguments.file_count = 0;

  /* parse arguments  */
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  
  /* parse file with whom we will be work  */
  if (arguments.file_count == 0)
    {
      arguments.file_count = 1;
      arguments.files = malloc(sizeof(FILE *));
      if (!arguments.files)
	{
	  printf("error malloc dont work");
	  return EXIT_FAILURE;
	}
      arguments.files[0] = stdin;
    }

  if (!arguments.unique_output && arguments.outfile)
    {
      /* clear file  */
      outstream = fopen(arguments.outfile, "w");
      if (!outstream)
	{
	  if (arguments.file_count == 1 && arguments.files[0] == stdin)
	    free(arguments.files);
	  else
	    { 
	      if (arguments.files)
		free(arguments.files);
	    }

	  printf("error not cant be open files");
	  return EXIT_FAILURE;
	}
      fclose(outstream);
    }
  
  /* go thougth each file and replace old text with new*/
  for (i = 0; i < arguments.file_count; i++)
    {
      FILE *f = arguments.files[i];

      FILE *temp = tmpfile();
      
      replace_text(f, temp, arguments.args[0], arguments.args[1], &arguments);

      if (!arguments.unique_output)
	{
	  if (arguments.outfile)	    
	      outstream = fopen(arguments.outfile, "a");	   
	  else
	    outstream = stdout;

	  rewind(temp);
	  
	  stream_rediraction(temp, outstream);

	  if (arguments.outfile)
	    fclose(outstream);    
	}
      else
	{
	  if (f != stdin)
	    {
	      if (freopen(NULL, "w+", f) == NULL)
            {
              printf("error write input file");
              if (arguments.file_count == 1 && arguments.files[0] == stdin)
		free(arguments.files);
	      else
		{
		  if (arguments.files)
		    free(arguments.files);
		}
	      fclose(temp);
		return EXIT_FAILURE;
            }
	      rewind(f);
	      rewind(temp);	    
	      stream_rediraction(temp, f);
	    }
	  else
	    {
	      if (arguments.file_count == 1 && arguments.files[0] == stdin)
		free(arguments.files);
	      else
		{ 
		  if (arguments.files)
		    free(arguments.files);
		}

	      fclose(f);
	      fclose(temp);
	      printf("error stdin cannot rediracte to stdin");
	      return EXIT_FAILURE;
	    }
	}

      fclose(temp);
      
      if (f != stdin)	
	fclose(f);	
    }

  /* free memory*/
  if (arguments.file_count == 1 && arguments.files[0] == stdin)
      free(arguments.files);   
  else
   {
     if (arguments.files)    
       free(arguments.files);
   }

  return 0;
}

