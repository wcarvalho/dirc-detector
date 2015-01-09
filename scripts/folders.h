#include "optionparser.h"

enum  optionIndex { HELP, VERBOSE, DIRECTORY, SIMDATA, RECDATA, FITDIR, GRAPHDIR};
const option::Descriptor usage[] =
{
 {HELP, 0,"h", "help",option::Arg::None, "  --help  \tPrint usage and exit." },
 {VERBOSE, 0,"v", "verbose",option::Arg::None, "  -v  \tPrint data throughout run." },
 
 {DIRECTORY, 0,"D", "directory",option::Arg::Optional, "  -D  \tMain directory for output" },
 {SIMDATA, 0,"S","simdata",option::Arg::Optional, " -S  \tMain Directory." },
 {RECDATA, 0,"R","recdata",option::Arg::Optional, " -R  \tMain Directory." },
 {FITDIR, 0,"f","fits_directory",option::Arg::Optional, " -f  \tDirectory where graphs are saved." },
 {GRAPHDIR, 0,"g","graphs_directory",option::Arg::Optional, " -g  \tDirectory where graphs are saved." },
 
 {GRAPHDIR, 0,"g","graphs_directory",option::Arg::Optional, " -g  \tDirectory where graphs are saved." },

 {0,0,0,0,0,0}
};