#include <stdbool.h>
#include "STIL_generate.h"
#include <helper/log.h>

bool checkExistence(FILE *file, const char *content);
void generate_signalspart(struct jtag_xfer *transfers);