#ifndef __MONTECARLO_H_
#define __MONTECARLO_H_

#include "exchange.h"
#include "hand.h"
#include "utils.h"


#define NUMBER_OF_SAMPLES 10000

int * compute_expected_value(Hand * hand, int isCPU);

#endif