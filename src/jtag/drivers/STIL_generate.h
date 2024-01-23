#include <stdio.h>
#include <stdint.h>

#include "libusb.h"


struct jtag_xfer {
	int ep;
	uint8_t *buf;
	size_t size;
	/* Internal */
	int retval;
	int completed;
	size_t transfer_size;
	struct libusb_transfer *transfer;
};

void generate_stiltitle(FILE *fp);
void generate_signals(FILE *fp,struct libusb_device_handle *dev_handle, struct jtag_xfer *transfers);
void generate_signalsgroups(FILE *fp,struct libusb_device_handle *dev_handle, struct jtag_xfer *transfers);
void generate_Timing(FILE *fp,struct libusb_device_handle *dev_handle, struct jtag_xfer *transfers);
void generate_patternburst(FILE *fp);
void generate_patternexec(FILE *fp);
void generate_xferpart(FILE *fp, uint16_t length,const uint8_t *in);
void generate_setsigpart(FILE *fp,const uint8_t *commands);
void generate_clkpart(FILE *fp,const uint8_t *commands);
void generate_pattern(FILE *fp,struct libusb_device_handle *dev_handle, struct jtag_xfer *transfers);
void generate_stil(struct libusb_device_handle *dev_handle,struct jtag_xfer *transfers);