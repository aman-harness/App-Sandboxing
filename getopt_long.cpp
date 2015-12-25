#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/* Flag set by ‘--tree’. */
static int tree_flag;

int
main (int argc, char **argv)
{
  int c;

  while (1)
    {
      static struct option long_options[] =
        {
          /* These options set a flag. */
          {"tree", no_argument,       &tree_flag, 1},
          {"group",   no_argument,       &tree_flag, 0},
          /* These options don’t set a flag.
             We distinguish them by their indices. */ 
          {"add",     no_argument,       0, 'a'},
          {"append",  no_argument,       0, 'b'},
          {"time",  required_argument, 0, 't'},
          {"memory",  required_argument, 0, 'm'},
          {"frequency", required_argument, 0, 'f'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      // no colon afterwards ->  No arguments Required.
      // : -> compulsory argument.
      // :: -> Optional Arguments.
      c = getopt_long (argc, argv, "abt:m:f:",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
        {
        case 0:
          /* If this option set a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            break;
          printf ("option %s", long_options[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;

        case 'a':
          puts ("option -a\n");
          break;

        case 'b':
          puts ("option -b\n");
          break;

        case 't':
          printf ("option -t with value `%s'\n", optarg);
          break;

        case 'm':
          printf ("option -m with value `%s'\n", optarg);
          break;

        case 'f':
          printf ("option -f with value `%s'\n", optarg);
          break;

        case '?':
          /* getopt_long already printed an error message. */
          break;

        default:
          abort ();
        }
    }

  /* Instead of reporting ‘--tree’
     and ‘--group’ as they are encountered,
     we report the final status resulting from them. */
  if (tree_flag)
    puts ("tree flag is set");

  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (optind < argc)
        printf ("%s ", argv[optind++]);
      putchar ('\n');
    }

  exit (0);
}