/*
  File autogenerated by gengetopt version 2.22.6
  generated with the following command:
  gengetopt -i cmdline.ggo -ccpp 

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

const char *gengetopt_args_info_purpose = "Generator for DirC.\n\nBy Wilka Carvalho <wcarvalho92@gmail.com>";

const char *gengetopt_args_info_usage = "Usage: " CMDLINE_PARSER_PACKAGE " [OPTIONS]...";

const char *gengetopt_args_info_versiontext = "";

const char *gengetopt_args_info_description = "";

const char *gengetopt_args_info_help[] = {
  "  -h, --help                    Print help and exit",
  "  -V, --version                 Print version and exit",
  "  -e, --events=INT              Number of events",
  "  -P, --particles=INT           Takes in two arguments for the range in the\n                                  number of particles per event",
  "  -m, --maxpars=INT             maximum number of particles to intersect DirC\n                                  (for anlytical-control puposes)",
  "  -r, --random=INT              value for seed of random numbers",
  "  -f, --filename=STRING         root filename (relative or absolute path). By\n                                  default written within directory as\n                                  generator.root",
  "  -v, --verbose                 print data",
  "  -q, --quiet                   suppress all printing",
  "  -d, --dirc-properties=STRING  file with dirc properties (in this order):\n                                  Length, Width, Height, Radial Distance,\n                                  Magnetic Field",
  "  -c, --custom-set=STRING       Takes a filename with parameters for the\n                                  experiment including number of particles,\n                                  range in eta, pt, and phi, and particle\n                                  charge and type",
  "  -D, --Directory=STRING        Sets the directory in which files will be saved\n                                  (by default saves in current directory",
  "      --pt-distribution-function=STRING\n                                The probability distribution that pt will\n                                  follow from pt = 0 to pt = 10GeV",
    0
};

typedef enum {ARG_NO
  , ARG_STRING
  , ARG_INT
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
  args_info->events_given = 0 ;
  args_info->particles_given = 0 ;
  args_info->maxpars_given = 0 ;
  args_info->random_given = 0 ;
  args_info->filename_given = 0 ;
  args_info->verbose_given = 0 ;
  args_info->quiet_given = 0 ;
  args_info->dirc_properties_given = 0 ;
  args_info->custom_set_given = 0 ;
  args_info->Directory_given = 0 ;
  args_info->pt_distribution_function_given = 0 ;
}

static
void clear_args (struct gengetopt_args_info *args_info)
{
  FIX_UNUSED (args_info);
  args_info->events_orig = NULL;
  args_info->particles_arg = NULL;
  args_info->particles_orig = NULL;
  args_info->maxpars_orig = NULL;
  args_info->random_orig = NULL;
  args_info->filename_arg = NULL;
  args_info->filename_orig = NULL;
  args_info->dirc_properties_arg = NULL;
  args_info->dirc_properties_orig = NULL;
  args_info->custom_set_arg = NULL;
  args_info->custom_set_orig = NULL;
  args_info->Directory_arg = NULL;
  args_info->Directory_orig = NULL;
  args_info->pt_distribution_function_arg = NULL;
  args_info->pt_distribution_function_orig = NULL;
  
}

static
void init_args_info(struct gengetopt_args_info *args_info)
{


  args_info->help_help = gengetopt_args_info_help[0] ;
  args_info->version_help = gengetopt_args_info_help[1] ;
  args_info->events_help = gengetopt_args_info_help[2] ;
  args_info->particles_help = gengetopt_args_info_help[3] ;
  args_info->particles_min = 0;
  args_info->particles_max = 0;
  args_info->maxpars_help = gengetopt_args_info_help[4] ;
  args_info->random_help = gengetopt_args_info_help[5] ;
  args_info->filename_help = gengetopt_args_info_help[6] ;
  args_info->verbose_help = gengetopt_args_info_help[7] ;
  args_info->quiet_help = gengetopt_args_info_help[8] ;
  args_info->dirc_properties_help = gengetopt_args_info_help[9] ;
  args_info->custom_set_help = gengetopt_args_info_help[10] ;
  args_info->Directory_help = gengetopt_args_info_help[11] ;
  args_info->pt_distribution_function_help = gengetopt_args_info_help[12] ;
  
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

/** @brief generic value variable */
union generic_value {
    int int_arg;
    char *string_arg;
    const char *default_string_arg;
};

/** @brief holds temporary values for multiple options */
struct generic_list
{
  union generic_value arg;
  char *orig;
  struct generic_list *next;
};

/**
 * @brief add a node at the head of the list 
 */
static void add_node(struct generic_list **list) {
  struct generic_list *new_node = (struct generic_list *) malloc (sizeof (struct generic_list));
  new_node->next = *list;
  *list = new_node;
  new_node->arg.string_arg = 0;
  new_node->orig = 0;
}

/**
 * The passed arg parameter is NOT set to 0 from this function
 */
static void
free_multiple_field(unsigned int len, void *arg, char ***orig)
{
  unsigned int i;
  if (arg) {
    for (i = 0; i < len; ++i)
      {
        free_string_field(&((*orig)[i]));
      }

    free (arg);
    free (*orig);
    *orig = 0;
  }
}


static void
cmdline_parser_release (struct gengetopt_args_info *args_info)
{

  free_string_field (&(args_info->events_orig));
  free_multiple_field (args_info->particles_given, (void *)(args_info->particles_arg), &(args_info->particles_orig));
  args_info->particles_arg = 0;
  free_string_field (&(args_info->maxpars_orig));
  free_string_field (&(args_info->random_orig));
  free_string_field (&(args_info->filename_arg));
  free_string_field (&(args_info->filename_orig));
  free_string_field (&(args_info->dirc_properties_arg));
  free_string_field (&(args_info->dirc_properties_orig));
  free_string_field (&(args_info->custom_set_arg));
  free_string_field (&(args_info->custom_set_orig));
  free_string_field (&(args_info->Directory_arg));
  free_string_field (&(args_info->Directory_orig));
  free_string_field (&(args_info->pt_distribution_function_arg));
  free_string_field (&(args_info->pt_distribution_function_orig));
  
  

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

static void
write_multiple_into_file(FILE *outfile, int len, const char *opt, char **arg, const char *values[])
{
  int i;
  
  for (i = 0; i < len; ++i)
    write_into_file(outfile, opt, (arg ? arg[i] : 0), values);
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
  if (args_info->events_given)
    write_into_file(outfile, "events", args_info->events_orig, 0);
  write_multiple_into_file(outfile, args_info->particles_given, "particles", args_info->particles_orig, 0);
  if (args_info->maxpars_given)
    write_into_file(outfile, "maxpars", args_info->maxpars_orig, 0);
  if (args_info->random_given)
    write_into_file(outfile, "random", args_info->random_orig, 0);
  if (args_info->filename_given)
    write_into_file(outfile, "filename", args_info->filename_orig, 0);
  if (args_info->verbose_given)
    write_into_file(outfile, "verbose", 0, 0 );
  if (args_info->quiet_given)
    write_into_file(outfile, "quiet", 0, 0 );
  if (args_info->dirc_properties_given)
    write_into_file(outfile, "dirc-properties", args_info->dirc_properties_orig, 0);
  if (args_info->custom_set_given)
    write_into_file(outfile, "custom-set", args_info->custom_set_orig, 0);
  if (args_info->Directory_given)
    write_into_file(outfile, "Directory", args_info->Directory_orig, 0);
  if (args_info->pt_distribution_function_given)
    write_into_file(outfile, "pt-distribution-function", args_info->pt_distribution_function_orig, 0);
  

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

static char *
get_multiple_arg_token(const char *arg)
{
  const char *tok;
  char *ret;
  size_t len, num_of_escape, i, j;

  if (!arg)
    return 0;

  tok = strchr (arg, ',');
  num_of_escape = 0;

  /* make sure it is not escaped */
  while (tok)
    {
      if (*(tok-1) == '\\')
        {
          /* find the next one */
          tok = strchr (tok+1, ',');
          ++num_of_escape;
        }
      else
        break;
    }

  if (tok)
    len = (size_t)(tok - arg + 1);
  else
    len = strlen (arg) + 1;

  len -= num_of_escape;

  ret = (char *) malloc (len);

  i = 0;
  j = 0;
  while (arg[i] && (j < len-1))
    {
      if (arg[i] == '\\' && 
	  arg[ i + 1 ] && 
	  arg[ i + 1 ] == ',')
        ++i;

      ret[j++] = arg[i++];
    }

  ret[len-1] = '\0';

  return ret;
}

static const char *
get_multiple_arg_token_next(const char *arg)
{
  const char *tok;

  if (!arg)
    return 0;

  tok = strchr (arg, ',');

  /* make sure it is not escaped */
  while (tok)
    {
      if (*(tok-1) == '\\')
        {
          /* find the next one */
          tok = strchr (tok+1, ',');
        }
      else
        break;
    }

  if (! tok || strlen(tok) == 1)
    return 0;

  return tok+1;
}

static int
check_multiple_option_occurrences(const char *prog_name, unsigned int option_given, unsigned int min, unsigned int max, const char *option_desc);

int
check_multiple_option_occurrences(const char *prog_name, unsigned int option_given, unsigned int min, unsigned int max, const char *option_desc)
{
  int error_occurred = 0;

  if (option_given && (min > 0 || max > 0))
    {
      if (min > 0 && max > 0)
        {
          if (min == max)
            {
              /* specific occurrences */
              if (option_given != (unsigned int) min)
                {
                  fprintf (stderr, "%s: %s option occurrences must be %d\n",
                    prog_name, option_desc, min);
                  error_occurred = 1;
                }
            }
          else if (option_given < (unsigned int) min
                || option_given > (unsigned int) max)
            {
              /* range occurrences */
              fprintf (stderr, "%s: %s option occurrences must be between %d and %d\n",
                prog_name, option_desc, min, max);
              error_occurred = 1;
            }
        }
      else if (min > 0)
        {
          /* at least check */
          if (option_given < min)
            {
              fprintf (stderr, "%s: %s option occurrences must be at least %d\n",
                prog_name, option_desc, min);
              error_occurred = 1;
            }
        }
      else if (max > 0)
        {
          /* at most check */
          if (option_given > max)
            {
              fprintf (stderr, "%s: %s option occurrences must be at most %d\n",
                prog_name, option_desc, max);
              error_occurred = 1;
            }
        }
    }
    
  return error_occurred;
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
  if (check_multiple_option_occurrences(prog_name, args_info->particles_given, args_info->particles_min, args_info->particles_max, "'--particles' ('-P')"))
     error_occurred = 1;
  
  
  /* checks for dependences among options */

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

/**
 * @brief store information about a multiple option in a temporary list
 * @param list where to (temporarily) store multiple options
 */
static
int update_multiple_arg_temp(struct generic_list **list,
               unsigned int *prev_given, const char *val,
               const char *possible_values[], const char *default_value,
               cmdline_parser_arg_type arg_type,
               const char *long_opt, char short_opt,
               const char *additional_error)
{
  /* store single arguments */
  char *multi_token;
  const char *multi_next;

  if (arg_type == ARG_NO) {
    (*prev_given)++;
    return 0; /* OK */
  }

  multi_token = get_multiple_arg_token(val);
  multi_next = get_multiple_arg_token_next (val);

  while (1)
    {
      add_node (list);
      if (update_arg((void *)&((*list)->arg), &((*list)->orig), 0,
          prev_given, multi_token, possible_values, default_value, 
          arg_type, 0, 1, 1, 1, long_opt, short_opt, additional_error)) {
        if (multi_token) free(multi_token);
        return 1; /* failure */
      }

      if (multi_next)
        {
          multi_token = get_multiple_arg_token(multi_next);
          multi_next = get_multiple_arg_token_next (multi_next);
        }
      else
        break;
    }

  return 0; /* OK */
}

/**
 * @brief free the passed list (including possible string argument)
 */
static
void free_list(struct generic_list *list, short string_arg)
{
  if (list) {
    struct generic_list *tmp;
    while (list)
      {
        tmp = list;
        if (string_arg && list->arg.string_arg)
          free (list->arg.string_arg);
        if (list->orig)
          free (list->orig);
        list = list->next;
        free (tmp);
      }
  }
}

/**
 * @brief updates a multiple option starting from the passed list
 */
static
void update_multiple_arg(void *field, char ***orig_field,
               unsigned int field_given, unsigned int prev_given, union generic_value *default_value,
               cmdline_parser_arg_type arg_type,
               struct generic_list *list)
{
  int i;
  struct generic_list *tmp;

  if (prev_given && list) {
    *orig_field = (char **) realloc (*orig_field, (field_given + prev_given) * sizeof (char *));

    switch(arg_type) {
    case ARG_INT:
      *((int **)field) = (int *)realloc (*((int **)field), (field_given + prev_given) * sizeof (int)); break;
    case ARG_STRING:
      *((char ***)field) = (char **)realloc (*((char ***)field), (field_given + prev_given) * sizeof (char *)); break;
    default:
      break;
    };
    
    for (i = (prev_given - 1); i >= 0; --i)
      {
        tmp = list;
        
        switch(arg_type) {
        case ARG_INT:
          (*((int **)field))[i + field_given] = tmp->arg.int_arg; break;
        case ARG_STRING:
          (*((char ***)field))[i + field_given] = tmp->arg.string_arg; break;
        default:
          break;
        }        
        (*orig_field) [i + field_given] = list->orig;
        list = list->next;
        free (tmp);
      }
  } else { /* set the default value */
    if (default_value && ! field_given) {
      switch(arg_type) {
      case ARG_INT:
        if (! *((int **)field)) {
          *((int **)field) = (int *)malloc (sizeof (int));
          (*((int **)field))[0] = default_value->int_arg; 
        }
        break;
      case ARG_STRING:
        if (! *((char ***)field)) {
          *((char ***)field) = (char **)malloc (sizeof (char *));
          (*((char ***)field))[0] = gengetopt_strdup(default_value->string_arg);
        }
        break;
      default: break;
      }
      if (!(*orig_field)) {
        *orig_field = (char **) malloc (sizeof (char *));
        (*orig_field)[0] = 0;
      }
    }
  }
}

int
cmdline_parser_internal (
  int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error)
{
  int c;	/* Character of the parsed option.  */

  struct generic_list * particles_list = NULL;
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
        { "events",	1, NULL, 'e' },
        { "particles",	1, NULL, 'P' },
        { "maxpars",	1, NULL, 'm' },
        { "random",	1, NULL, 'r' },
        { "filename",	1, NULL, 'f' },
        { "verbose",	0, NULL, 'v' },
        { "quiet",	0, NULL, 'q' },
        { "dirc-properties",	1, NULL, 'd' },
        { "custom-set",	1, NULL, 'c' },
        { "Directory",	1, NULL, 'D' },
        { "pt-distribution-function",	1, NULL, 0 },
        { 0,  0, 0, 0 }
      };

      c = getopt_long (argc, argv, "hVe:P:m:r:f:vqd:c:D:", long_options, &option_index);

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

        case 'e':	/* Number of events.  */
        
        
          if (update_arg( (void *)&(args_info->events_arg), 
               &(args_info->events_orig), &(args_info->events_given),
              &(local_args_info.events_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "events", 'e',
              additional_error))
            goto failure;
        
          break;
        case 'P':	/* Takes in two arguments for the range in the number of particles per event.  */
        
          if (update_multiple_arg_temp(&particles_list, 
              &(local_args_info.particles_given), optarg, 0, 0, ARG_INT,
              "particles", 'P',
              additional_error))
            goto failure;
        
          break;
        case 'm':	/* maximum number of particles to intersect DirC (for anlytical-control puposes).  */
        
        
          if (update_arg( (void *)&(args_info->maxpars_arg), 
               &(args_info->maxpars_orig), &(args_info->maxpars_given),
              &(local_args_info.maxpars_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "maxpars", 'm',
              additional_error))
            goto failure;
        
          break;
        case 'r':	/* value for seed of random numbers.  */
        
        
          if (update_arg( (void *)&(args_info->random_arg), 
               &(args_info->random_orig), &(args_info->random_given),
              &(local_args_info.random_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "random", 'r',
              additional_error))
            goto failure;
        
          break;
        case 'f':	/* root filename (relative or absolute path). By default written within directory as generator.root.  */
        
        
          if (update_arg( (void *)&(args_info->filename_arg), 
               &(args_info->filename_orig), &(args_info->filename_given),
              &(local_args_info.filename_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "filename", 'f',
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
        case 'd':	/* file with dirc properties (in this order): Length, Width, Height, Radial Distance, Magnetic Field.  */
        
        
          if (update_arg( (void *)&(args_info->dirc_properties_arg), 
               &(args_info->dirc_properties_orig), &(args_info->dirc_properties_given),
              &(local_args_info.dirc_properties_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "dirc-properties", 'd',
              additional_error))
            goto failure;
        
          break;
        case 'c':	/* Takes a filename with parameters for the experiment including number of particles, range in eta, pt, and phi, and particle charge and type.  */
        
        
          if (update_arg( (void *)&(args_info->custom_set_arg), 
               &(args_info->custom_set_orig), &(args_info->custom_set_given),
              &(local_args_info.custom_set_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "custom-set", 'c',
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

        case 0:	/* Long option with no short option */
          /* The probability distribution that pt will follow from pt = 0 to pt = 10GeV.  */
          if (strcmp (long_options[option_index].name, "pt-distribution-function") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->pt_distribution_function_arg), 
                 &(args_info->pt_distribution_function_orig), &(args_info->pt_distribution_function_given),
                &(local_args_info.pt_distribution_function_given), optarg, 0, 0, ARG_STRING,
                check_ambiguity, override, 0, 0,
                "pt-distribution-function", '-',
                additional_error))
              goto failure;
          
          }
          
          break;
        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          goto failure;

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c%s\n", CMDLINE_PARSER_PACKAGE, c, (additional_error ? additional_error : ""));
          abort ();
        } /* switch */
    } /* while */


  update_multiple_arg((void *)&(args_info->particles_arg),
    &(args_info->particles_orig), args_info->particles_given,
    local_args_info.particles_given, 0,
    ARG_INT, particles_list);

  args_info->particles_given += local_args_info.particles_given;
  local_args_info.particles_given = 0;
  
  if (check_required)
    {
      error_occurred += cmdline_parser_required2 (args_info, argv[0], additional_error);
    }

  cmdline_parser_release (&local_args_info);

  if ( error_occurred )
    return (EXIT_FAILURE);

  return 0;

failure:
  free_list (particles_list, 0 );
  
  cmdline_parser_release (&local_args_info);
  return (EXIT_FAILURE);
}
