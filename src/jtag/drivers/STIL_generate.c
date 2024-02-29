#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "STIL_generate.h"


enum SignalIdentifier
{
    SIG_TCK = 1 << 1,
    SIG_TDI = 1 << 2,
    SIG_TDO = 1 << 3,
    SIG_TMS = 1 << 4,
    SIG_TRST = 1 << 5,
    SIG_SRST = 1 << 6

};

enum CommandModifier {
  // CMD_XFER
  NO_READ = 0x80,
  EXTEND_LENGTH = 0x40,
  // CMD_CLK
  READOUT = 0x80,
};

static int xfer_bytes(const uint8_t *commands,bool extend_length, bool no_read){
    uint16_t transferred_bits = commands[1];
    if (extend_length)
        transferred_bits+=256;
    if (transferred_bits> 62*8){
        transferred_bits = 62*8;
    }
    return (transferred_bits + 7) / 8;
}

void generate_stiltitle(FILE *fp)
{
    fprintf(fp, "STIL 1.0; \n");
}

void generate_signals(FILE *fp, struct libusb_transfer *transfers,struct libusb_device_handle *dev_handle)
{
    fprintf(fp, "Signals{\n");
    uint8_t *rxbuf = (uint8_t *)transfers->buffer;
    //uint8_t *rxbuf =(uint8_t *)buffer;
    uint8_t *commands = rxbuf;
    uint32_t count = transfers->length;
    while ((commands < (rxbuf + count)) && (*commands != CMD_STOP))
    {
        switch ((*commands) & 0x0F)
        {
        case CMD_INFO:
            break;
        case CMD_FREQ:
            commands+=2;
            break;
        case CMD_XFER:
            fprintf(fp, "  TDI Out;TCK Out;\n");
            fprintf(fp, "  TDO In;\n");
            commands+=1+xfer_bytes(commands,*commands & EXTEND_LENGTH, *commands & NO_READ );
            break;
        case CMD_SETSIG:
            fprintf(fp, "  TDI Out;TCK Out;TMS Out;TRST Out;SRST Out;\n");
            commands+=2;
            break;
        case CMD_GETSIG:
            fprintf(fp, " TDO In;\n");
            break;
        case CMD_CLK:
            fprintf(fp, "  TDI Out;TCK Out;TMS Out;\n");
            commands+=2;
            break;
        }
        commands++;
    }
    fprintf(fp, "}\n");
}

void generate_signalsgroups(FILE *fp, struct libusb_device_handle *dev_handle, struct libusb_transfer *transfers)
{
    fprintf(fp, "SignalsGroups{\n");
    uint8_t *rxbuf = (uint8_t *)transfers->buffer;
    uint8_t *commands = rxbuf;
    uint32_t count = transfers->length;
    while ((commands < (rxbuf + count)) && (*commands != CMD_STOP))
    {
        switch ((*commands) & 0x0F)
        {
        case CMD_INFO:
            break;
        case CMD_FREQ:
            commands+=2;
            break;
        case CMD_XFER:
            fprintf(fp, "  ABUS='TDI+TCK';\n");
            fprintf(fp, "  BBUS='TDO';\n");
            fprintf(fp, "  ALL='ABUS+BBUS'; \n");
            fprintf(fp, "  SI1='TDI';   {ScanIn 16;}\n");
            fprintf(fp, "  SI2='TCK';   {ScanIn 16;}\n");
            fprintf(fp, "  SO1='TDO';   {ScanIn 16;}\n");
            commands+=1+xfer_bytes(commands,*commands & EXTEND_LENGTH, *commands & NO_READ );
            break;
        case CMD_SETSIG:
            fprintf(fp, "  ABUS='TDI+TCK+TMS+TRST+SRST';\n");
            fprintf(fp, "  ALL='ABUS'; \n");
            fprintf(fp, "  SI1='TDI';   {ScanIn 16;}\n");
            fprintf(fp, "  SI2='TCK';   {ScanIn 16;}\n");
            fprintf(fp, "  SI3='TMS';   {ScanIn 16;}\n");
            fprintf(fp, "  SI4='TRST';   {ScanIn 16;}\n");
            fprintf(fp, "  SI5='SRST';   {ScanIn 16;}\n");
            commands+=2;
            break;
        case CMD_GETSIG:
            fprintf(fp, "  BBUS='TDO';\n");
            fprintf(fp, "  ALL='BBUS'; \n");
            fprintf(fp, "  SO1='TDO';   {ScanIn 16;}\n");
            break;
        case CMD_CLK:
            fprintf(fp, "  ABUS='TDI+TCK+TMS';\n");
            fprintf(fp, "  ALL='ABUS'; \n");
            fprintf(fp, "  SI1='TDI';   {ScanIn 16;}\n");
            fprintf(fp, "  SI2='TCK';   {ScanIn 16;}\n");
            fprintf(fp, "  SI3='TMS';   {ScanIn 16;}\n");
            commands+=2;
            break;
        }
        commands++;
    }
    
    fprintf(fp, "}\n");
}

void generate_Timing(FILE *fp, struct libusb_device_handle *dev_handle, struct libusb_transfer *transfers)
{
    fprintf(fp, "Timing{\n");
    uint8_t *rxbuf = (uint8_t *)transfers->buffer;
    uint8_t *commands = rxbuf;
    uint32_t count = transfers->length;
    while ((commands < (rxbuf + count)) && (*commands != CMD_STOP))
    {
        switch ((*commands) & 0x0F)
        {
        case CMD_INFO:
            fprintf(fp, "  WaveformTable scan{\n");
            fprintf(fp, "    Period '500ns';\n");
            fprintf(fp, "    Waveforms{\n");
            fprintf(fp, "      ALL{ 10 {'0ns' U/D;}}\n");
            fprintf(fp, "      ALL{ HLZX {'0ns' Z;'260ns' H/L/Z/X;}}\n");
            fprintf(fp, "    }\n");
            fprintf(fp, "  }\n");
            break;
        case CMD_FREQ:
            fprintf(fp, "  WaveformTable scan{\n");
            fprintf(fp, "    Period '500ns';\n");
            fprintf(fp, "    Waveforms{\n");
            fprintf(fp, "      ALL{ 10 {'0ns' U/D;}}\n");
            fprintf(fp, "      ALL{ HLZX {'0ns' Z;'260ns' H/L/Z/X;}}\n");
            fprintf(fp, "    }\n");
            fprintf(fp, "  }\n");
            commands+=2;
            break;
        case CMD_XFER:
            fprintf(fp, "  WaveformTable scan{\n");
            fprintf(fp, "    Period '500ns';\n");
            fprintf(fp, "    Waveforms{\n");
            fprintf(fp, "      ALL{ 10 {'0ns' U/D;}}\n");
            fprintf(fp, "      ALL{ HLZX {'0ns' Z;'260ns' H/L/Z/X;}}\n");
            fprintf(fp, "    }\n");
            fprintf(fp, "  }\n");
            commands+=1+xfer_bytes(commands,*commands & EXTEND_LENGTH, *commands & NO_READ );
            break;
        case CMD_SETSIG:
            fprintf(fp, "  WaveformTable scan{\n");
            fprintf(fp, "    Period '500ns';\n");
            fprintf(fp, "    Waveforms{\n");
            fprintf(fp, "      ALL{ 10 {'0ns' U/D;}}\n");
            fprintf(fp, "      ALL{ HLZX {'0ns' Z;'260ns' H/L/Z/X;}}\n");
            fprintf(fp, "    }\n");
            fprintf(fp, "  }\n");
            commands+=2;
            break;
        case CMD_GETSIG:
            fprintf(fp, "  WaveformTable scan{\n");
            fprintf(fp, "    Period '500ns';\n");
            fprintf(fp, "    Waveforms{\n");
            fprintf(fp, "      ALL{ 10 {'0ns' U/D;}}\n");
            fprintf(fp, "      ALL{ HLZX {'0ns' Z;'260ns' H/L/Z/X;}}\n");
            fprintf(fp, "    }\n");
            fprintf(fp, "  }\n");
            break;
        case CMD_CLK:
            fprintf(fp, "  WaveformTable scan{\n");
            fprintf(fp, "    Period '500ns';\n");
            fprintf(fp, "    Waveforms{\n");
            fprintf(fp, "      ALL{ 10 {'0ns' U/D;}}\n");
            fprintf(fp, "      ALL{ HLZX {'0ns' Z;'260ns' H/L/Z/X;}}\n");
            fprintf(fp, "    }\n");
            fprintf(fp, "  }\n");
            commands+=2;
            break;
        }
        commands++;
    }
    
    fprintf(fp, "}\n");
}

void generate_patternburst(FILE *fp)
{
    fprintf(fp, "PatternBurst 'scan_burst' {\n");
    fprintf(fp, "  PatList{ 'scan' ;}\n");
    fprintf(fp, "}\n");
}

void generate_patternexec(FILE *fp)
{
    fprintf(fp, "PatternExec {\n");
    fprintf(fp, "  PatternBurst scan_burst ;\n"  );
    fprintf(fp, "}\n");
}

void generate_xferpart(FILE *fp, uint16_t length,const uint8_t *in)
{
    uint32_t xfer_length;
    xfer_length = length;
    const uint8_t *xfer_in;
    xfer_in = in;
    fprintf(fp, "    SI1="); // tdi
    for (uint32_t i = 0; i < xfer_length; i++)
    {
        if ((xfer_in[i / 8] >> (7 - (i % 8))) & 1)
        {
            fprintf(fp, "H");
        }
        else
        {
            fprintf(fp, "L");
        }
    }
    fprintf(fp, ";\n");

    fprintf(fp, "    SI2=");
    for (uint32_t i = 0; i < xfer_length; i++)
    {
        fprintf(fp, "HL");
    }
    fprintf(fp, ";\n");
}

void generate_setsigpart(FILE *fp, const uint8_t *commands)
{
    uint8_t signal_mask, signal_status;
    signal_mask = commands[1];
    signal_status = commands[2];
    if (signal_mask & SIG_TDI)
    {
        if (signal_status & SIG_TDI)
        {
            fprintf(fp, "  SI1=H;\n");
        }
        else
        {
            fprintf(fp, "  SI1=L;\n");
        }
    }
    if (signal_mask & SIG_TCK)
    {
        if (signal_status & SIG_TCK)
        {
            fprintf(fp, "  SI2=H;\n");
        }
        else
        {
            fprintf(fp, "  SI2=L;\n");
        }
    }
    if (signal_mask & SIG_TMS)
    {
        if (signal_status & SIG_TMS)
        {
            fprintf(fp, "  SI3=H;\n");
        }
        else
        {
            fprintf(fp, "  SI3=L;\n");
        }
    }
    if (signal_mask & SIG_TRST)
    {
        if (signal_status & SIG_TRST)
        {
            fprintf(fp, "  SI4=H;\n");
        }
        else
        {
            fprintf(fp, "  SI4=L;\n");
        }
    }
    if (signal_mask & SIG_SRST)
    {
        if (signal_status & SIG_SRST)
        {
            fprintf(fp, "  SI5=H;\n");
        }
        else
        {
            fprintf(fp, "  SI5=L;\n");
        }
    }
}

void generate_clkpart(FILE *fp, const uint8_t *commands)
{
    uint8_t signals, clk_pulses;

    signals = commands[1];
    clk_pulses = commands[2];

    if (signals & SIG_TDI)
    {
        fprintf(fp, "  SI1=H;\n");
    }
    if (!(signals & SIG_TDI))
    {
        fprintf(fp, "  SI1=L;\n");
    }
    if (signals & SIG_TMS)
    {
        fprintf(fp, "  SI3=H;\n");
    }
    if (!(signals & SIG_TMS))
    {
        fprintf(fp, "  SI3=L;\n");
    }

    fprintf(fp, "    SO2="); // tck
    for (int i = 0; i < clk_pulses; i++)
    {
        fprintf(fp, "HL");
    }
    fprintf(fp, ";}\n");
}

void generate_pattern(FILE *fp, struct libusb_device_handle *dev_handle, struct libusb_transfer *transfers)
{
    fprintf(fp, "Pattern scan {\n");
    uint8_t *rxbuf = (uint8_t *)transfers->buffer;
    uint32_t count = transfers->length;
    uint8_t *commands = rxbuf;
    
    
    while ((commands < (rxbuf + count)) && (*commands != CMD_STOP))
    {
        switch ((*commands) & 0x0F)
        {
        case CMD_INFO:
            break;
        case CMD_FREQ:
            commands+=2;
            break;
        case CMD_XFER:
            generate_xferpart(fp, commands[1],commands+2);
            commands+=1+xfer_bytes(commands,*commands & EXTEND_LENGTH, *commands & NO_READ );
            break;
        case CMD_SETSIG:
            generate_setsigpart(fp, &(*commands));
            commands+=2;
            break;
        case CMD_GETSIG:
            break;
        case CMD_CLK:
            generate_clkpart(fp, &(*commands));
            commands+=2;
            break;
        }
        commands++;
    }
   
    fprintf(fp, "}\n");
}

void generate_stil(struct libusb_device_handle *dev_handle, struct libusb_transfer *transfers)
{
    FILE *fp;
    // static int file_count=1;
    // char filename[50];
    // snprintf(filename,sizeof(filename),"STIL.stil%d",file_count);
    // fp = fopen(filename, "w+");
    // while(fp!=NULL){
    //     fclose(fp);
    //     file_count++;
    //     snprintf(filename,sizeof(filename),"STIL.stil%d",file_count);
    //     fp = fopen(filename, "w+");
    // }
    // fprintf(fp,"qqq\n"); //I WRITE FOR TEST
    fp=fopen("STIL1.txt","a+");
    generate_stiltitle(fp);
    generate_signals( fp,transfers,dev_handle);
    generate_signalsgroups(fp,dev_handle,transfers);
    generate_Timing(fp,dev_handle,transfers);
    generate_patternburst(fp);
    generate_patternexec(fp);
    generate_pattern(fp, dev_handle, transfers);
    fclose(fp);
}
//void generate_signals(FILE *fp, struct libusb_transfer *transfer,struct libusb_device_handle *dev_handle,unsigned char *buffer,void *user_data)
