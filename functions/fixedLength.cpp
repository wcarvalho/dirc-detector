#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "../headers/histmaker.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Reconstruct one "Event"
================================================================================================*/

std::string fixedLength(int value, int digits)
{
    unsigned int uvalue = value;
    if (value < 0) {
        uvalue = -uvalue;
    }
    std::string result;
    while (digits-- > 0) {
        result += ('0' + uvalue % 10);
        uvalue /= 10;
    }
    if (value < 0) {
        result += '-';
    }
    std::reverse(result.begin(), result.end());
    return result;
}