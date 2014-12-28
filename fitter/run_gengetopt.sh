#!/bin/bash
gengetopt --input=fit-cmd.ggo --include-getopt
mv cmdline.c cmdline.cpp
