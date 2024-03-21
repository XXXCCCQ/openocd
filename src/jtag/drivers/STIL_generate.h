#include <stdio.h>
#include <stdint.h>

#include <libusb.h>

#define STLINK_SWIM_ERR_OK             0x00
#define STLINK_SWIM_BUSY               0x01

#define STLINK_DEBUG_ERR_OK            0x80//databuf
#define STLINK_DEBUG_ERR_FAULT         0x81//databuf

#define STLINK_SWD_AP_WAIT             0x10
#define STLINK_SWD_AP_FAULT            0x11
#define STLINK_SWD_AP_ERROR            0x12
#define STLINK_SWD_AP_PARITY_ERROR     0x13

#define STLINK_JTAG_GET_IDCODE_ERROR   0x09//databuf
#define STLINK_JTAG_WRITE_ERROR        0x0c//databuf
#define STLINK_JTAG_WRITE_VERIF_ERROR  0x0d//databuf

#define STLINK_SWD_DP_WAIT             0x14
#define STLINK_SWD_DP_FAULT            0x15
#define STLINK_SWD_DP_ERROR            0x16
#define STLINK_SWD_DP_PARITY_ERROR     0x17

#define STLINK_SWD_AP_WDATA_ERROR      0x18
#define STLINK_SWD_AP_STICKY_ERROR     0x19
#define STLINK_SWD_AP_STICKYORUN_ERROR 0x1a

#define STLINK_BAD_AP_ERROR            0x1d

#define STLINK_CORE_RUNNING            0x80//databuf
#define STLINK_CORE_HALTED             0x81//databuf
#define STLINK_CORE_STAT_UNKNOWN       -1

#define STLINK_GET_VERSION             0xF1//no need 0xF2  getsig??
#define STLINK_DEBUG_COMMAND           0xF2//enter command??

#define STLINK_DFU_COMMAND             0xF3
#define STLINK_SWIM_COMMAND            0xF4

#define STLINK_GET_CURRENT_MODE        0xF5//getsig??
#define STLINK_GET_TARGET_VOLTAGE      0xF7//getsig


//stlink mode
#define STLINK_DEV_DFU_MODE            0x00
#define STLINK_DEV_MASS_MODE           0x01
#define STLINK_DEV_DEBUG_MODE          0x02
#define STLINK_DEV_SWIM_MODE           0x03
#define STLINK_DEV_BOOTLOADER_MODE     0x04
#define STLINK_DEV_UNKNOWN_MODE        -1

#define STLINK_DFU_EXIT                0x07

/*
	STLINK_SWIM_ENTER_SEQ
	1.3ms low then 750Hz then 1.5kHz

	STLINK_SWIM_GEN_RST
	STM8 DM pulls reset pin low 50us

	STLINK_SWIM_SPEED
	uint8_t (0=low|1=high)

	STLINK_SWIM_WRITEMEM
	uint16_t length
	uint32_t address

	STLINK_SWIM_RESET
	send synchronization seq (16us low, response 64 clocks low)
*/
#define STLINK_SWIM_ENTER                  0x00
#define STLINK_SWIM_EXIT                   0x01
#define STLINK_SWIM_READ_CAP               0x02
#define STLINK_SWIM_SPEED                  0x03
#define STLINK_SWIM_ENTER_SEQ              0x04
#define STLINK_SWIM_GEN_RST                0x05
#define STLINK_SWIM_RESET                  0x06
#define STLINK_SWIM_ASSERT_RESET           0x07
#define STLINK_SWIM_DEASSERT_RESET         0x08
#define STLINK_SWIM_READSTATUS             0x09
#define STLINK_SWIM_WRITEMEM               0x0a
#define STLINK_SWIM_READMEM                0x0b
#define STLINK_SWIM_READBUF                0x0c

#define STLINK_DEBUG_GETSTATUS             0x01//getsig
#define STLINK_DEBUG_FORCEDEBUG            0x02//setsig
#define STLINK_DEBUG_APIV1_RESETSYS        0x03//setsig
#define STLINK_DEBUG_APIV1_READALLREGS     0x04//getsig  enter stlink_usb_xfer_noerrcheck
#define STLINK_DEBUG_APIV1_READREG         0x05//getsig  enter stlink_usb_xfer_noerrcheck

#define STLINK_DEBUG_APIV1_WRITEREG        0x06//xfer
#define STLINK_DEBUG_READMEM_32BIT         0x07//getsig
#define STLINK_DEBUG_WRITEMEM_32BIT        0x08//xfer
#define STLINK_DEBUG_RUNCORE               0x09//setsig or xfer
#define STLINK_DEBUG_STEPCORE              0x0a//setsig or xfer
#define STLINK_DEBUG_APIV1_SETFP           0x0b//not find
#define STLINK_DEBUG_READMEM_8BIT          0x0c//getsig
#define STLINK_DEBUG_WRITEMEM_8BIT         0x0d//setsig or xfer
#define STLINK_DEBUG_APIV1_CLEARFP         0x0e//not find
#define STLINK_DEBUG_APIV1_WRITEDEBUGREG   0x0f//setsig or xfer
#define STLINK_DEBUG_APIV1_SETWATCHPOINT   0x10//not find


//stlink mode
#define STLINK_DEBUG_ENTER_JTAG_RESET      0x00
#define STLINK_DEBUG_ENTER_SWD_NO_RESET    0xa3
#define STLINK_DEBUG_ENTER_JTAG_NO_RESET   0xa4
#define STLINK_DEBUG_APIV1_ENTER           0x20

#define STLINK_DEBUG_EXIT                  0x21//stop??
#define STLINK_DEBUG_READCOREID            0x22//getsig

#define STLINK_DEBUG_APIV2_ENTER           0x30//mode

#define STLINK_DEBUG_APIV2_READ_IDCODES    0x31//getsig
#define STLINK_DEBUG_APIV2_RESETSYS        0x32//setsig
#define STLINK_DEBUG_APIV2_READREG         0x33//getsig
#define STLINK_DEBUG_APIV2_WRITEREG        0x34//setsig or xfer
#define STLINK_DEBUG_APIV2_WRITEDEBUGREG   0x35//setsig or xfer
#define STLINK_DEBUG_APIV2_READDEBUGREG    0x36//setsig or xfer

#define STLINK_DEBUG_APIV2_READALLREGS     0x3A//getsig
#define STLINK_DEBUG_APIV2_GETLASTRWSTATUS 0x3B//getsig
#define STLINK_DEBUG_APIV2_DRIVE_NRST      0x3C//setsig

#define STLINK_DEBUG_APIV2_GETLASTRWSTATUS2 0x3E//getsig

#define STLINK_DEBUG_APIV2_START_TRACE_RX  0x40//getsig
#define STLINK_DEBUG_APIV2_STOP_TRACE_RX   0x41//getsig
#define STLINK_DEBUG_APIV2_GET_TRACE_NB    0x42//getsig

#define STLINK_DEBUG_APIV2_SWD_SET_FREQ    0x43

#define STLINK_DEBUG_APIV2_JTAG_SET_FREQ   0x44//freq
#define STLINK_DEBUG_APIV2_READ_DAP_REG    0x45//getsig
#define STLINK_DEBUG_APIV2_WRITE_DAP_REG   0x46//setsig or xfer
#define STLINK_DEBUG_APIV2_READMEM_16BIT   0x47//getsig
#define STLINK_DEBUG_APIV2_WRITEMEM_16BIT  0x48//setsig or xfer

#define STLINK_DEBUG_APIV2_INIT_AP         0x4B//connect command?
#define STLINK_DEBUG_APIV2_CLOSE_AP_DBG    0x4C//stop connect

#define STLINK_DEBUG_WRITEMEM_32BIT_NO_ADDR_INC         0x50//setsig or xfer
#define STLINK_DEBUG_APIV2_RW_MISC_OUT     0x51//setsig or xfer
#define STLINK_DEBUG_APIV2_RW_MISC_IN      0x52//getsig

#define STLINK_DEBUG_READMEM_32BIT_NO_ADDR_INC          0x54//getsig

#define STLINK_APIV3_SET_COM_FREQ           0x61//setsig
#define STLINK_APIV3_GET_COM_FREQ           0x62//getsig

#define STLINK_APIV3_GET_VERSION_EX         0xFB//getsig

#define STLINK_DEBUG_APIV2_DRIVE_NRST_LOW   0x00//setsig
#define STLINK_DEBUG_APIV2_DRIVE_NRST_HIGH  0x01//setsig
#define STLINK_DEBUG_APIV2_DRIVE_NRST_PULSE 0x02//setsig

#define STLINK_DEBUG_PORT_ACCESS            0xffff//getsig

#define STLINK_TRACE_SIZE               4096
#define STLINK_TRACE_MAX_HZ             2000000
#define STLINK_V3_TRACE_MAX_HZ          24000000

#define STLINK_V3_MAX_FREQ_NB               10

#define REQUEST_SENSE        0x03//getsig
#define REQUEST_SENSE_LENGTH 18

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

enum CommandIdentifier
{
    CMD_STOP = 0x00,
    CMD_INFO = 0x01,
    CMD_FREQ = 0x02,
    CMD_XFER = 0x03,
    CMD_SETSIG = 0x04,
    CMD_GETSIG = 0x05,
    CMD_CLK = 0x06
};

enum CommandModifier {
  // CMD_XFER
  NO_READ = 0x80,
  EXTEND_LENGTH = 0x40,
  // CMD_CLK
  READOUT = 0x80,
};

// int xfer_bytes(const uint8_t *commands,bool extend_length);
// bool verify_maxfrequency(uint32_t frequency);
// void generate_stiltitle(FILE *fp);
// void generate_signals(FILE *fp, struct jtag_xfer *transfers,struct libusb_device_handle *dev_handle);
// void generate_signalsgroups(FILE *fp,struct libusb_device_handle *dev_handle, struct jtag_xfer *transfers);
// void generate_Timing(FILE *fp,struct libusb_device_handle *dev_handle, struct jtag_xfer *transfers);
// void generate_patternburst(FILE *fp);
// void generate_patternexec(FILE *fp);
// void generate_xferpart(FILE *fp, uint16_t length,const uint8_t *in);
// void generate_setsigpart(FILE *fp,const uint8_t *commands);
// void generate_clkpart(FILE *fp,const uint8_t *commands);
// void generate_pattern(FILE *fp,struct libusb_device_handle *dev_handle, struct jtag_xfer *transfers);
// void generate_stil(struct libusb_device_handle *dev_handle, struct jtag_xfer *transfers);


void enter_xfer(struct jtag_xfer *transfers);
void generate_stil11(const uint8_t *buf, uint16_t size);
