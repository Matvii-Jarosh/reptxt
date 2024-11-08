/*
Copyright (C) 2024 Matvii Jarosh

This file is part of reptxt.

reptxt is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

reptxt is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with reptxt. If not, see <https://www.gnu.org/licenses/>.
*/
/*
  replace.c - contain a function for replace old text with new  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"
#include "replace.h"

/* replace old text with new from input stream to output stream  */
int
replace_text(FILE *input, FILE *output, const char *old, const char *new,
	     struct arguments *arguments)
{
  size_t old_len = strlen(old);
  char c, *buffer = malloc(old_len);
  int i, j, this_old, prev_old = 1;  

  if (buffer == NULL)
    {
      printf("error malloc buffer");
      return EXIT_FAILURE;
    }

  memset(buffer, 0, old_len);
  
  /* rewimnd point in streams  */
  rewind(input);
  rewind(output);

  while (1)
    {
      this_old = 0;
      /* if this not end of file then test char  */
      if ((c = (char)fgetc(input)) != EOF)
	{
	  if (c == '\n')
	    {
	      if (fputc('\n', output) == EOF)
		{
		  free(buffer);
		  printf("error put char in output");
		  return EXIT_FAILURE;
		}
	      continue;
	    }
	  
	  /* if this first char then test other chracter*/
	  if (c == old[0])
	    {
	      buffer[0] = c;
	      buffer[1] = '\0';
	      this_old = 1;
	      /* test on old  */
	      for (i = 1; i < old_len; ++i)
		{
		  /* if this not end of file then test char  */
		  if ((c = (char)fgetc(input)) != EOF)
		    {
		      buffer[i] = c;
		      buffer[i + 1] = '\0';
		      if (!arguments->except)
			{
			  /* if this is not char from old then write buffer in iutput  */
			  if (c != old[i])
			    {
			      for (j = 0; j < strlen(buffer) - 1; ++j)
				{
				  if (fputc(buffer[j], output) == EOF)
				    {
				      free(buffer);
				      printf("error write buffer in output");
				      return EXIT_FAILURE;
				    }			     
				}
			  			 
			      memset(buffer, 0, old_len);
			      this_old = 0;
			      break;
			    }		      
			}
		      else
			{
			  if (c != old[i])
			    {
			      this_old = 0;
			      break;
			    }
			}
		    }
		  /* if this end of file  */
		  else		    
		    {
		      if (!arguments->except)
			{
			  for (j = 0; j < strlen(buffer); ++j)
			    {
			      if (fputc(buffer[j], output) == EOF)
				{
				  free(buffer);
				  printf("error write buffer in output");
				  return EXIT_FAILURE;
				}
			    }
			  memset(buffer, 0, old_len);
			  this_old = 0;
			  break;
			}
		      else
			{
			  /* if prev word is not old word then write new word  */
			  if (!prev_old)
			    {
			      if (fputs(new, output) <= 0)
				{		
				  free(buffer);
				  printf("error replace old with new");
				  return EXIT_FAILURE;
				}
			      if (arguments->verbose)
				printf("print new word");
			    }
			  break;
			}
		    }
		}
	    }
	  /* if this old word  */
	  if (this_old)
	    {
	      if (arguments->verbose)
		printf("found old word\n");
	      /* put new strinf instead of old  */
	      if (!arguments->except)
		{
		  if (fputs(new, output) <= 0)
		    {		
		      free(buffer);
		      printf("error replace old with new");
		      return EXIT_FAILURE;
		    }
		  if (arguments->verbose)
		    printf("replace new word\n");
		}
	      else
		{
		  /* if prev word is not old word then write new word  */
		  if (!prev_old)
		    {
		      if (fputs(new, output) <= 0)
			{		
			  free(buffer);
			  printf("error replace old with new\n");
			  return EXIT_FAILURE;
			}
		      if (arguments->verbose)
			printf("print new line\n");
		    }
		  
		  if (fputs(old, output) <= 0)
		    {		
		      free(buffer);
		      printf("error replace old with new");
		      return EXIT_FAILURE;
		    }
		  if (arguments->verbose)
		    printf("print old line\n");
		}	      
	      prev_old = 1;
	    }
	  /* if this is not first char from old then put this char*/
	  else
	    {
	      if (!arguments->except)
		{
		  if (fputc(c, output) == EOF)
		    {
		      free(buffer);
		      printf("error put char in output");
		      return EXIT_FAILURE;
		    }
		}
	      else
		prev_old = 0;
	    }
	}
      else /* if end of file  */
	{
	  if (arguments->except && !prev_old)
	    {
	      if (fputs(new, output) <= 0)
		{		
		  free(buffer);
		  printf("error replace old with new");
		  return EXIT_FAILURE;
		}
	      if (arguments->verbose)
		printf("print new line\n");
	    }
	  fputc('\n', output);
	  break;
	}
    }

  free(buffer);
  
  return 0;
}

