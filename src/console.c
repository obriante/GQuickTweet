/*
 * Copyright © 2012 - Alfredo Liguoro <freddy@haxaddicts.net>
 * Orazio Briante <orazio.briante@hotmail.it>
 *
 * Licensed under the GNU General Public License Version 3
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 * or write to the Free Software Foundation, Inc., 51 Franklin St
 * Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <logc/logc.h>

#include <console.h>
#include <twitc/functions.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HELP_ARG "--help"
#define DEBUG_ARG "--debug"
#define VERSION_ARG "--version"

#ifdef __cplusplus
extern "C"
  {
#endif

void
printHelp(string_t  argv[])
{
	printVersion(argv);

  printf("\n%s:\t%s [%s]\n\n\n%s:\n\n", "Usage", argv[0], "options",
		  "options");
  fprintf(stdout, "  %s\t\t\t%s\n", DEBUG_ARG, "Enable debug output");
  fprintf(stdout, "  %s\t\t\t%s\n\n", VERSION_ARG, "Show Program Version");
  fprintf(stdout, "  %s\t\t\t%s\n", HELP_ARG, "Show this message");
  fprintf(stdout, "\n");
}

void
printVersion(string_t  argv[])
{
  string_t version = readRawTextFile(PROG_DIR "/VERSION");

  if (version)
    //Headers Print
    printf("\n%s - \"%s\", %s\n%s %s, %s\n\n", argv[0], "GTK+ Desktop Gadget for quickly sending tweets",
        version, "Copyright (C)", "2012", "Alfredo Liguoro and Orazio Briante");

  else
    printf("\n%s - \"%s\",\n%s %s, %s\n\n", argv[0], "GTK+ Desktop Gadget for quickly sending tweets",
    		"Copyright (C)", "2012", "Alfredo Liguoro and Orazio Briante");

  if (version)
    free(version);
}

/**
 * Prompt Args
 */
byte_t
shellParam(int argc, string_t  argv[])
{

  if (argc == 2)
    {

      int count;

      for (count = 1; count < argc; count++)
        {
          if (strcmp(argv[count], DEBUG_ARG) == 0)
            {
        	  initLog(FILE_VIDEO_LOG, FILE_VIDEO_LOG);
        	  return 2;
            }
          if (strcmp(argv[count], HELP_ARG) == 0)
            {
              printHelp(argv);
              return EXIT_FAILURE;
            }
          else if (strcmp(argv[count], VERSION_ARG) == 0)
            {
              printVersion(argv);
              return EXIT_FAILURE;
            }
          else
            {
              printHelp(argv);
              return EXIT_FAILURE;
            }
        }
    }
  else if (argc > 2)
    {
      printHelp(argv);
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

#ifdef __cplusplus
}
#endif
