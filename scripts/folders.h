// http://optionparser.sourceforge.net/
// example: http://optionparser.sourceforge.net/example__arg_8cc.html
#include "optionparser.h"
#include <stdio.h>
#include <stdlib.h>

enum  optionIndex {HELP, VERBOSE, SETTINGS, DIRECTORY, SIMDATA, RECDATA, CHEATDATA, FITDIR, GRAPHDIR, OUTPUT, THRESHINT, THRESHDOUBLE};
const option::Descriptor usage[] =
{
 {HELP, 0,"h", "help",option::Arg::None, "  --help  \tPrint usage and exit." },
 {VERBOSE, 0,"v", "verbose",option::Arg::None, "  -v  \tPrint data throughout run." },
 {SETTINGS, 0,"s", "settings",option::Arg::Optional, "  -s  \tsettings file" },

 {SIMDATA, 0,"S","simdata",option::Arg::Optional, "  -S  \tSimsulation Data." },
 {RECDATA, 0,"R","recdata",option::Arg::Optional, "  -R  \tReconstruction Data." },
{CHEATDATA, 0,"C","cheatdata",option::Arg::Optional, "  -R  \tCheat Data." },
 {DIRECTORY, 0,"D", "directory",option::Arg::Optional, "  -D  \tMain directory for output" },
 {FITDIR, 0,"f","fits_directory",option::Arg::Optional, "  -f  \tDirectory where fits are saved." },
 {GRAPHDIR, 0,"g","graphs_directory",option::Arg::Optional, "  -g  \tDirectory where graphs are saved." },
 {OUTPUT, 0,"o","output",option::Arg::Optional, "  -o  \tOutput file." },
 

 {THRESHINT, 0,"t","threshint", option::Arg::Optional, "  -t  \tA threshold (dummy int value)." },
 {THRESHDOUBLE, 0,"T","threshdouble", option::Arg::Optional, "  -T  \tA threshold (dummy double value)." },

 {0,0,0,0,0,0,}
};
