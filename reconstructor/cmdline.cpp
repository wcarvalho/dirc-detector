/*
  File autogenerated by gengetopt version 2.22.6
  generated with the following command:
  gengetopt -i cmd.ggo 

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FIX_UNUSED
#define FIX_UNUSED(X) (void) (X) /* avoid warnings for unused params */
#endif

#include <getopt.h>

#include "cmdline.h"

const char *gengetopt_args_info_purpose = "Reconstructor for DirC.\n\nBy Wilka Carvalho <wcarvalho92@gmail.com>";

const char *gengetopt_args_info_usage = "Usage: " CMDLINE_PARSER_PACKAGE " [OPTIONS]...";

const char *gengetopt_args_info_versiontext = "";

const char *gengetopt_args_info_description = "";

const char *gengetopt_args_info_help[] = {
  "  -h, --help                    Print help and exit",
  "  -V, --version                 Print version and exit",
  "  -i, --input=STRING            path of particle-generated data",
  "  -D, --Directory=STRING        Sets the directory in which files will be saved\n                                  (by default saves in current directory",
  "  -W, --write-file=STRING       write file for reconstruction",
  "  -v, --verbose                 print data",
  "  -q, --quiet                   suppress all printing",
  "  -m, --make                    print graphs of the fits made",
  "  -g, --graph-prefix=STRING     directory where graphs will be stored",
  "  -l, --last=INT                only reconstructs the last l particles",
  "  -S, --Smear=DOUBLE            the smearing applied to the fitting (used for\n                                  width of gaussian)",
  "  -e, --expected-photons-case[=INT]\n                                case 1: look-up table. case 2: riemann sum\n                                  calculation.  (default=`1')",
  "  -L, --LookUpTable[=STRING]    file for look-up table  (default=`LookUpTable')",
    0
};

typedef enum {ARG_NO
  , ARG_STRING
  , ARG_INT
  , ARG_DOUBLE
} cmdline_parser_arg_type;

static
void clear_given (struct gengetopt_args_info *args_info);
static
void clear_args (struct gengetopt_args_info *args_info);

static int
cmdline_parser_internal (int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error);

static int
cmdline_parser_required2 (struct gengetopt_args_info *args_info, const char *prog_name, const char *additional_error);

static char *
gengetopt_strdup (const char *s);

static
void clear_given (struct gengetopt_args_info *args_info)
{
  args_info->help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->input_given = 0 ;
  args_info->Directory_given = 0 ;
  args_info->write_file_given = 0 ;
  args_info->verbose_given = 0 ;
  args_info->quiet_given = 0 ;
  args_info->make_given = 0 ;
  args_info->graph_prefix_given = 0 ;
  args_info->last_given = 0 ;
  args_info->Smear_given = 0 ;
  args_info->expected_photons_case_given = 0 ;
  args_info->LookUpTable_given = 0 ;
}

static
void clear_args (struct gengetopt_args_info *args_info)
{
  FIX_UNUSED (args_info);
  args_info->input_arg = NULL;
  args_info->input_orig = NULL;
  args_info->Directory_arg = NULL;
  args_info->Directory_orig = NULL;
  args_info->write_file_arg = NULL;
  args_info->write_file_orig = NULL;
  args_info->graph_prefix_arg = NULL;
  args_info->graph_prefix_orig = NULL;
  args_info->last_orig = NULL;
  args_info->Smear_orig = NULL;
  args_info->expected_photons_case_arg = 1;
  args_info->expected_photons_case_orig = NULL;
  args_info->LookUpTable_arg = gengetopt_strdup ("LookUpTable");
  args_info->LookUpTable_orig = NULL;
  
}

static
void init_args_info(struct gengetopt_args_info *args_info)
{


  args_info->help_help = gengetopt_args_info_help[0] ;
  args_info->version_help = gengetopt_args_info_help[1] ;
  args_info->input_help = gengetopt_args_info_help[2] ;
  args_info->Directory_help = gengetopt_args_info_help[3] ;
  args_info->write_file_help = gengetopt_args_info_help[4] ;
  args_info->verbose_help = gengetopt_args_info_help[5] ;
  args_info->quiet_help = gengetopt_args_info_help[6] ;
  args_info->make_help = gengetopt_args_info_help[7] ;
  args_info->graph_prefix_help = gengetopt_args_info_help[8] ;
  args_info->last_help = gengetopt_args_info_help[9] ;
  args_info->Smear_help = gengetopt_args_info_help[10] ;
  args_info->expected_photons_case_help = gengetopt_args_info_help[11] ;
  args_info->LookUpTable_help = gengetopt_args_info_help[12] ;
  
}

void
cmdline_parser_print_version (void)
{
  printf ("%s %s\n",
     (strlen(CMDLINE_PARSER_PACKAGE_NAME) ? CMDLINE_PARSER_PACKAGE_NAME : CMDLINE_PARSER_PACKAGE),
     CMDLINE_PARSER_VERSION);

  if (strlen(gengetopt_args_info_versiontext) > 0)
    printf("\n%s\n", gengetopt_args_info_versiontext);
}

static void print_help_common(void) {
  cmdline_parser_print_version ();

  if (strlen(gengetopt_args_info_purpose) > 0)
    printf("\n%s\n", gengetopt_args_info_purpose);

  if (strlen(gengetopt_args_info_usage) > 0)
    printf("\n%s\n", gengetopt_args_info_usage);

  printf("\n");

  if (strlen(gengetopt_args_info_description) > 0)
    printf("%s\n\n", gengetopt_args_info_description);
}

void
cmdline_parser_print_help (void)
{
  int i = 0;
  print_help_common();
  while (gengetopt_args_info_help[i])
    printf("%s\n", gengetopt_args_info_help[i++]);
}

void
cmdline_parser_init (struct gengetopt_args_info *args_info)
{
  clear_given (args_info);
  clear_args (args_info);
  init_args_info (args_info);
}

void
cmdline_parser_params_init(struct cmdline_parser_params *params)
{
  if (params)
    { 
      params->override = 0;
      params->initialize = 1;
      params->check_required = 1;
      params->check_ambiguity = 0;
      params->print_errors = 1;
    }
}

struct cmdline_parser_params *
cmdline_parser_params_create(void)
{
  struct cmdline_parser_params *params = 
    (struct cmdline_parser_params *)malloc(sizeof(struct cmdline_parser_params));
  cmdline_parser_params_init(params);  
  return params;
}

static void
free_string_field (char **s)
{
  if (*s)
    {
      free (*s);
      *s = 0;
    }
}


static void
cmdline_parser_release (struct gengetopt_args_info *args_info)
{

  free_string_field (&(args_info->input_arg));
  free_string_field (&(args_info->input_orig));
  free_string_field (&(args_info->Directory_arg));
  free_string_field (&(args_info->Directory_orig));
  free_string_field (&(args_info->write_file_arg));
  free_string_field (&(args_info->write_file_orig));
  free_string_field (&(args_info->graph_prefix_arg));
  free_string_field (&(args_info->graph_prefix_orig));
  free_string_field (&(args_info->last_orig));
  free_string_field (&(args_info->Smear_orig));
  free_string_field (&(args_info->expected_photons_case_orig));
  free_string_field (&(args_info->LookUpTable_arg));
  free_string_field (&(args_info->LookUpTable_orig));
  
  

  clear_given (args_info);
}


static void
write_into_file(FILE *outfile, const char *opt, const char *arg, const char *values[])
{
  FIX_UNUSED (values);
  if (arg) {
    fprintf(outfile, "%s=\"%s\"\n", opt, arg);
  } else {
    fprintf(outfile, "%s\n", opt);
  }
}


int
cmdline_parser_dump(FILE *outfile, struct gengetopt_args_info *args_info)
{
  int i = 0;

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot dump options to stream\n", CMDLINE_PARSER_PACKAGE);
      return EXIT_FAILURE;
    }

  if (args_info->help_given)
    write_into_file(outfile, "help", 0, 0 );
  if (args_info->version_given)
    write_into_file(outfile, "version", 0, 0 );
  if (args_info->input_given)
    write_into_file(outfile, "input", args_info->input_orig, 0);
  if (args_info->Directory_given)
    write_into_file(outfile, "Directory", args_info->Directory_orig, 0);
  if (args_info->write_file_given)
    write_into_file(outfile, "write-file", args_info->write_file_orig, 0);
  if (args_info->verbose_given)
    write_into_file(outfile, "verbose", 0, 0 );
  if (args_info->quiet_given)
    write_into_file(outfile, "quiet", 0, 0 );
  if (args_info->make_given)
    write_into_file(outfile, "make", 0, 0 );
  if (args_info->graph_prefix_given)
    write_into_file(outfile, "graph-prefix", args_info->graph_prefix_orig, 0);
  if (args_info->last_given)
    write_into_file(outfile, "last", args_info->last_orig, 0);
  if (args_info->Smear_given)
    write_into_file(outfile, "Smear", args_info->Smear_orig, 0);
  if (args_info->expected_photons_case_given)
    write_into_file(outfile, "expected-photons-case", args_info->expected_photons_case_orig, 0);
  if (args_info->LookUpTable_given)
    write_into_file(outfile, "LookUpTable", args_info->LookUpTable_orig, 0);
  

  i = EXIT_SUCCESS;
  return i;
}

int
cmdline_parser_file_save(const char *filename, struct gengetopt_args_info *args_info)
{
  FILE *outfile;
  int i = 0;

  outfile = fopen(filename, "w");

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot open file for writing: %s\n", CMDLINE_PARSER_PACKAGE, filename);
      return EXIT_FAILURE;
    }

  i = cmdline_parser_dump(outfile, args_info);
  fclose (outfile);

  return i;
}

void
cmdline_parser_free (struct gengetopt_args_info *args_info)
{
  cmdline_parser_release (args_info);
}

/** @brief replacement of strdup, which is not standard */
char *
gengetopt_strdup (const char *s)
{
  char *result = 0;
  if (!s)
    return result;

  result = (char*)malloc(strlen(s) + 1);
  if (result == (char*)0)
    return (char*)0;
  strcpy(result, s);
  return result;
}

int
cmdline_parser (int argc, char **argv, struct gengetopt_args_info *args_info)
{
  return cmdline_parser2 (argc, argv, args_info, 0, 1, 1);
}

int
cmdline_parser_ext (int argc, char **argv, struct gengetopt_args_info *args_info,
                   struct cmdline_parser_params *params)
{
  int result;
  result = cmdline_parser_internal (argc, argv, args_info, params, 0);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser2 (int argc, char **argv, struct gengetopt_args_info *args_info, int override, int initialize, int check_required)
{
  int result;
  struct cmdline_parser_params params;
  
  params.override = override;
  params.initialize = initialize;
  params.check_required = check_required;
  params.check_ambiguity = 0;
  params.print_errors = 1;

  result = cmdline_parser_internal (argc, argv, args_info, &params, 0);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser_required (struct gengetopt_args_info *args_info, const char *prog_name)
{
  int result = EXIT_SUCCESS;

  if (cmdline_parser_required2(args_info, prog_name, 0) > 0)
    result = EXIT_FAILURE;

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser_required2 (struct gengetopt_args_info *args_info, const char *prog_name, const char *additional_error)
{
  int error_occurred = 0;
  FIX_UNUSED (additional_error);

  /* checks for required options */
  if (! args_info->input_given)
    {
      fprintf (stderr, "%s: '--input' ('-i') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error_occurred = 1;
    }
  
  
  /* checks for dependences among options */
  if (args_info->LookUpTable_given && ! args_info->expected_photons_case_given)
    {
      fprintf (stderr, "%s: '--LookUpTable' ('-L') option depends on option 'expected-photons-case'%s\n", prog_name, (additional_error ? additional_error : ""));
      error_occurred = 1;
    }

  return error_occurred;
}


static char *package_name = 0;

/**
 * @brief updates an option
 * @param field the generic pointer to the field to update
 * @param orig_field the pointer to the orig field
 * @param field_given the pointer to the number of occurrence of this option
 * @param prev_given the pointer to the number of occurrence already seen
 * @param value the argument for this option (if null no arg was specified)
 * @param possible_values the possible values for this option (if specified)
 * @param default_value the default value (in case the option only accepts fixed values)
 * @param arg_type the type of this option
 * @param check_ambiguity @see cmdline_parser_params.check_ambiguity
 * @param override @see cmdline_parser_params.override
 * @param no_free whether to free a possible previous value
 * @param multiple_option whether this is a multiple option
 * @param long_opt the corresponding long option
 * @param short_opt the corresponding short option (or '-' if none)
 * @param additional_error possible further error specification
 */
static
int update_arg(void *field, char **orig_field,
               unsigned int *field_given, unsigned int *prev_given, 
               char *value, const char *possible_values[],
               const char *default_value,
               cmdline_parser_arg_type arg_type,
               int check_ambiguity, int override,
               int no_free, int multiple_option,
               const char *long_opt, char short_opt,
               const char *additional_error)
{
  char *stop_char = 0;
  const char *val = value;
  int found;
  char **string_field;
  FIX_UNUSED (field);

  stop_char = 0;
  found = 0;

  if (!multiple_option && prev_given && (*prev_given || (check_ambiguity && *field_given)))
    {
      if (short_opt != '-')
        fprintf (stderr, "%s: `--%s' (`-%c') option given more than once%s\n", 
               package_name, long_opt, short_opt,
               (additional_error ? additional_error : ""));
      else
        fprintf (stderr, "%s: `--%s' option given more than once%s\n", 
               package_name, long_opt,
               (additional_error ? additional_error : ""));
      return 1; /* failure */
    }

  FIX_UNUSED (default_value);
    
  if (field_given && *field_given && ! override)
    return 0;
  if (prev_given)
    (*prev_given)++;
  if (field_given)
    (*field_given)++;
  if (possible_values)
    val = possible_values[found];

  switch(arg_type) {
  case ARG_INT:
    if (val) *((int *)field) = strtol (val, &stop_char, 0);
    break;
  case ARG_DOUBLE:
    if (val) *((double *)field) = strtod (val, &stop_char);
    break;
  case ARG_STRING:
    if (val) {
      string_field = (char **)field;
      if (!no_free && *string_field)
        free (*string_field); /* free previous string */
      *string_field = gengetopt_strdup (val);
    }
    break;
  default:
    break;
  };

  /* check numeric conversion */
  switch(arg_type) {
  case ARG_INT:
  case ARG_DOUBLE:
    if (val && !(stop_char && *stop_char == '\0')) {
      fprintf(stderr, "%s: invalid numeric value: %s\n", package_name, val);
      return 1; /* failure */
    }
    break;
  default:
    ;
  };

  /* store the original value */
  switch(arg_type) {
  case ARG_NO:
    break;
  default:
    if (value && orig_field) {
      if (no_free) {
        *orig_field = value;
      } else {
        if (*orig_field)
          free (*orig_field); /* free previous string */
        *orig_field = gengetopt_strdup (value);
      }
    }
  };

  return 0; /* OK */
}


int
cmdline_parser_internal (
  int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error)
{
  int c;	/* Character of the parsed option.  */

  int error_occurred = 0;
  struct gengetopt_args_info local_args_info;
  
  int override;
  int initialize;
  int check_required;
  int check_ambiguity;
  
  package_name = argv[0];
  
  override = params->override;
  initialize = params->initialize;
  check_required = params->check_required;
  check_ambiguity = params->check_ambiguity;

  if (initialize)
    cmdline_parser_init (args_info);

  cmdline_parser_init (&local_args_info);

  optarg = 0;
  optind = 0;
  opterr = params->print_errors;
  optopt = '?';

  while (1)
    {
      int option_index = 0;

      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "version",	0, NULL, 'V' },
        { "input",	1, NULL, 'i' },
        { "Directory",	1, NULL, 'D' },
        { "write-file",	1, NULL, 'W' },
        { "verbose",	0, NULL, 'v' },
        { "quiet",	0, NULL, 'q' },
        { "make",	0, NULL, 'm' },
        { "graph-prefix",	1, NULL, 'g' },
        { "last",	1, NULL, 'l' },
        { "Smear",	1, NULL, 'S' },
        { "expected-photons-case",	2, NULL, 'e' },
        { "LookUpTable",	2, NULL, 'L' },
        { 0,  0, 0, 0 }
      };

      c = getopt_long (argc, argv, "hVi:D:W:vqmg:l:S:e::L::", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          cmdline_parser_print_help ();
          cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'V':	/* Print version and exit.  */
          cmdline_parser_print_version ();
          cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'i':	/* path of particle-generated data.  */
        
        
          if (update_arg( (void *)&(args_info->input_arg), 
               &(args_info->input_orig), &(args_info->input_given),
              &(local_args_info.input_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "input", 'i',
              additional_error))
            goto failure;
        
          break;
        case 'D':	/* Sets the directory in which files will be saved (by default saves in current directory.  */
        
        
          if (update_arg( (void *)&(args_info->Directory_arg), 
               &(args_info->Directory_orig), &(args_info->Directory_given),
              &(local_args_info.Directory_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "Directory", 'D',
              additional_error))
            goto failure;
        
          break;
        case 'W':	/* write file for reconstruction.  */
        
        
          if (update_arg( (void *)&(args_info->write_file_arg), 
               &(args_info->write_file_orig), &(args_info->write_file_given),
              &(local_args_info.write_file_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "write-file", 'W',
              additional_error))
            goto failure;
        
          break;
        case 'v':	/* print data.  */
        
        
          if (update_arg( 0 , 
               0 , &(args_info->verbose_given),
              &(local_args_info.verbose_given), optarg, 0, 0, ARG_NO,
              check_ambiguity, override, 0, 0,
              "verbose", 'v',
              additional_error))
            goto failure;
        
          break;
        case 'q':	/* suppress all printing.  */
        
        
          if (update_arg( 0 , 
               0 , &(args_info->quiet_given),
              &(local_args_info.quiet_given), optarg, 0, 0, ARG_NO,
              check_ambiguity, override, 0, 0,
              "quiet", 'q',
              additional_error))
            goto failure;
        
          break;
        case 'm':	/* print graphs of the fits made.  */
        
        
          if (update_arg( 0 , 
               0 , &(args_info->make_given),
              &(local_args_info.make_given), optarg, 0, 0, ARG_NO,
              check_ambiguity, override, 0, 0,
              "make", 'm',
              additional_error))
            goto failure;
        
          break;
        case 'g':	/* directory where graphs will be stored.  */
        
        
          if (update_arg( (void *)&(args_info->graph_prefix_arg), 
               &(args_info->graph_prefix_orig), &(args_info->graph_prefix_given),
              &(local_args_info.graph_prefix_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "graph-prefix", 'g',
              additional_error))
            goto failure;
        
          break;
        case 'l':	/* only reconstructs the last l particles.  */
        
        
          if (update_arg( (void *)&(args_info->last_arg), 
               &(args_info->last_orig), &(args_info->last_given),
              &(local_args_info.last_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "last", 'l',
              additional_error))
            goto failure;
        
          break;
        case 'S':	/* the smearing applied to the fitting (used for width of gaussian).  */
        
        
          if (update_arg( (void *)&(args_info->Smear_arg), 
               &(args_info->Smear_orig), &(args_info->Smear_given),
              &(local_args_info.Smear_given), optarg, 0, 0, ARG_DOUBLE,
              check_ambiguity, override, 0, 0,
              "Smear", 'S',
              additional_error))
            goto failure;
        
          break;
        case 'e':	/* case 1: look-up table. case 2: riemann sum calculation..  */
        
        
          if (update_arg( (void *)&(args_info->expected_photons_case_arg), 
               &(args_info->expected_photons_case_orig), &(args_info->expected_photons_case_given),
              &(local_args_info.expected_photons_case_given), optarg, 0, "1", ARG_INT,
              check_ambiguity, override, 0, 0,
              "expected-photons-case", 'e',
              additional_error))
            goto failure;
        
          break;
        case 'L':	/* file for look-up table.  */
        
        
          if (update_arg( (void *)&(args_info->LookUpTable_arg), 
               &(args_info->LookUpTable_orig), &(args_info->LookUpTable_given),
              &(local_args_info.LookUpTable_given), optarg, 0, "LookUpTable", ARG_STRING,
              check_ambiguity, override, 0, 0,
              "LookUpTable", 'L',
              additional_error))
            goto failure;
        
          break;

        case 0:	/* Long option with no short option */
        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          goto failure;

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c%s\n", CMDLINE_PARSER_PACKAGE, c, (additional_error ? additional_error : ""));
          abort ();
        } /* switch */
    } /* while */



  if (check_required)
    {
      error_occurred += cmdline_parser_required2 (args_info, argv[0], additional_error);
    }

  cmdline_parser_release (&local_args_info);

  if ( error_occurred )
    return (EXIT_FAILURE);

  return 0;

failure:
  
  cmdline_parser_release (&local_args_info);
  return (EXIT_FAILURE);
}