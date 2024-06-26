#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "STIL_generate.h"
#include <helper/log.h>




// int xfer_bytes(const uint8_t *commands,bool extend_length){
//     uint16_t transferred_bits = commands[1];
//     if (extend_length)
//         transferred_bits += 256;
//     if (transferred_bits > 62*8){
//         transferred_bits = 62*8;
//     }
//     return transferred_bits ;
// }

// bool max_frequency=true;

// bool verify_maxfrequency(uint32_t frequency){
//     if (frequency == 0) {
//     frequency = 1;
//     } else if (frequency > 1500) {
//     frequency = 1500;
//     }
//     return max_frequency = (frequency == 1500);
// }

// void generate_stiltitle(FILE *fp)
// {
//     fprintf(fp, "STIL 1.0; \n");
// }

// void generate_signals(FILE *fp, struct jtag_xfer *transfers,struct libusb_device_handle *dev_handle)
// {
//     fprintf(fp, "Signals{\n");
//     uint8_t *rxbuf = (uint8_t *)transfers->transfer->buffer;
//     //uint8_t *rxbuf =(uint8_t *)buffer;
//     uint8_t *commands = rxbuf;
//     uint32_t count = transfers->transfer->actual_length;
//     while ((commands < (rxbuf + count)) && (*commands != CMD_STOP))
//     {
//         switch ((*commands) & 0x0F)
//         {
//         // case CMD_INFO:
//         //     break;
//         case CMD_FREQ:
//             commands+=2;
//             break;
//         case CMD_XFER:
//             uint32_t trbytes=xfer_bytes(commands,(*commands & EXTEND_LENGTH));
//             fprintf(fp, "  TDI Out;TCK Out;\n");
//             fprintf(fp, "  TDO In;\n");
//             LOG_INFO("values of commmands[1]:%d",commands[1]);
//             LOG_INFO("values of trbytes:%d",trbytes);
//             commands+=(7+trbytes)/8+1;
//             break;
//         case CMD_SETSIG:
//             fprintf(fp, "  TDI Out;TCK Out;TMS Out;TRST Out;SRST Out;\n");
//             commands+=2;
//             break;
//         case CMD_GETSIG:
//             fprintf(fp, " TDO In;\n");
//             break;
//         case CMD_CLK:
//             fprintf(fp, "  TDI Out;TCK Out;TMS Out;\n");
//             commands+=2;
//             break;
//         default:
//             break;
//         }
//         commands++;
//     }
//     fprintf(fp, "}\n");
// }

// void generate_signalsgroups(FILE *fp, struct libusb_device_handle *dev_handle, struct jtag_xfer *transfers)
// {
//     fprintf(fp, "SignalsGroups{\n");
//     uint8_t *rxbuf = (uint8_t *)transfers->transfer->buffer;
//     uint8_t *commands = rxbuf;
//     uint32_t count = transfers->transfer->actual_length;
//     uint32_t trbytes=xfer_bytes(commands,*commands & EXTEND_LENGTH);
//     while ((commands < (rxbuf + count)) && (*commands != CMD_STOP))
//     {
//         switch ((*commands) & 0x0F)
//         {
//         case CMD_INFO:
//             break;
//         case CMD_FREQ:
//             commands+=2;
//             break;
//         case CMD_XFER:
//             fprintf(fp, "  ABUS='TDI+TCK';\n");
//             fprintf(fp, "  BBUS='TDO';\n");
//             fprintf(fp, "  ALL='ABUS+BBUS'; \n");
//             fprintf(fp, "  SI1='TDI';   {ScanIn 16;}\n");
//             fprintf(fp, "  SI2='TCK';   {ScanIn 16;}\n");
//             fprintf(fp, "  SO1='TDO';   {ScanIn 16;}\n");
//             commands+=(7+trbytes)/8+1;
//             break;
//         case CMD_SETSIG:
//             fprintf(fp, "  ABUS='TDI+TCK+TMS+TRST+SRST';\n");
//             fprintf(fp, "  ALL='ABUS'; \n");
//             fprintf(fp, "  SI1='TDI';   {ScanIn 16;}\n");
//             fprintf(fp, "  SI2='TCK';   {ScanIn 16;}\n");
//             fprintf(fp, "  SI3='TMS';   {ScanIn 16;}\n");
//             fprintf(fp, "  SI4='TRST';   {ScanIn 16;}\n");
//             fprintf(fp, "  SI5='SRST';   {ScanIn 16;}\n");
//             commands+=2;
//             break;
//         case CMD_GETSIG:
//             fprintf(fp, "  BBUS='TDO';\n");
//             fprintf(fp, "  ALL='BBUS'; \n");
//             fprintf(fp, "  SO1='TDO';   {ScanIn 16;}\n");
//             break;
//         case CMD_CLK:
//             fprintf(fp, "  ABUS='TDI+TCK+TMS';\n");
//             fprintf(fp, "  ALL='ABUS'; \n");
//             fprintf(fp, "  SI1='TDI';   {ScanIn 16;}\n");
//             fprintf(fp, "  SI2='TCK';   {ScanIn 16;}\n");
//             fprintf(fp, "  SI3='TMS';   {ScanIn 16;}\n");
//             commands+=2;
//             break;
//         }
//         commands++;
//     }
    
//     fprintf(fp, "}\n");
// }

// void generate_Timing(FILE *fp, struct libusb_device_handle *dev_handle, struct jtag_xfer *transfers)
// {
//     fprintf(fp, "Timing{\n");
//     uint8_t *rxbuf = (uint8_t *)transfers->transfer->buffer;
//     uint8_t *commands = rxbuf;
//     uint32_t count = transfers->transfer->actual_length;
//     uint32_t trbytes=xfer_bytes(commands,*commands & EXTEND_LENGTH);
//     while ((commands < (rxbuf + count)) && (*commands != CMD_STOP))
//     {
//         switch ((*commands) & 0x0F)
//         {
//         case CMD_FREQ:
//             fprintf(fp, "  WaveformTable scan{\n");
//             fprintf(fp, "    Period '500ns';\n");
//             fprintf(fp, "    Waveforms{\n");
//             fprintf(fp, "      ALL{ 10 {'0ns' U/D;}}\n");
//             fprintf(fp, "      ALL{ HLZX {'0ns' Z;'260ns' H/L/Z/X;}}\n");
//             fprintf(fp, "    }\n");
//             fprintf(fp, "  }\n");
//             commands+=2;
//             break;
//         case CMD_XFER:
//             fprintf(fp, "  WaveformTable scan{\n");
//             fprintf(fp, "    Period '500ns';\n");
//             fprintf(fp, "    Waveforms{\n");
//             fprintf(fp, "      ALL{ 10 {'0ns' U/D;}}\n");
//             fprintf(fp, "      ALL{ HLZX {'0ns' Z;'260ns' H/L/Z/X;}}\n");
//             fprintf(fp, "    }\n");
//             fprintf(fp, "  }\n");
//             commands+=(7+trbytes)/8+1;
//             break;
//         case CMD_SETSIG:
//             fprintf(fp, "  WaveformTable scan{\n");
//             fprintf(fp, "    Period '500ns';\n");
//             fprintf(fp, "    Waveforms{\n");
//             fprintf(fp, "      ALL{ 10 {'0ns' U/D;}}\n");
//             fprintf(fp, "      ALL{ HLZX {'0ns' Z;'260ns' H/L/Z/X;}}\n");
//             fprintf(fp, "    }\n");
//             fprintf(fp, "  }\n");
//             commands+=2;
//             break;
//         case CMD_GETSIG:
//             fprintf(fp, "  WaveformTable scan{\n");
//             fprintf(fp, "    Period '500ns';\n");
//             fprintf(fp, "    Waveforms{\n");
//             fprintf(fp, "      ALL{ 10 {'0ns' U/D;}}\n");
//             fprintf(fp, "      ALL{ HLZX {'0ns' Z;'260ns' H/L/Z/X;}}\n");
//             fprintf(fp, "    }\n");
//             fprintf(fp, "  }\n");
//             break;
//         case CMD_CLK:
//             fprintf(fp, "  WaveformTable scan{\n");
//             fprintf(fp, "    Period '500ns';\n");
//             fprintf(fp, "    Waveforms{\n");
//             fprintf(fp, "      ALL{ 10 {'0ns' U/D;}}\n");
//             fprintf(fp, "      ALL{ HLZX {'0ns' Z;'260ns' H/L/Z/X;}}\n");
//             fprintf(fp, "    }\n");
//             fprintf(fp, "  }\n");
//             commands+=2;
//             break;
//         }
//         commands++;
//     }
    
//     fprintf(fp, "}\n");
// }

// void generate_patternburst(FILE *fp)
// {
//     fprintf(fp, "PatternBurst 'scan_burst' {\n");
//     fprintf(fp, "  PatList{ 'scan' ;}\n");
//     fprintf(fp, "}\n");
// }

// void generate_patternexec(FILE *fp)
// {
//     fprintf(fp, "PatternExec {\n");
//     fprintf(fp, "  PatternBurst scan_burst ;\n"  );
//     fprintf(fp, "}\n");
// }

// void generate_xferpart(FILE *fp, uint16_t xfer_length,const uint8_t *in)
// {
//     uint32_t length;
//     length=xfer_length;
//     LOG_INFO("length=%d",length);
//     const uint8_t *xfer_in;
//     xfer_in = in;
//     fprintf(fp, "    SI1="); // tdi
//     for (uint32_t i = 0; i < length; i++)
//     {
//         if ((xfer_in[i / 8] >> (7 - (i % 8))) & 1)
//         {
//             fprintf(fp, "H");
//         }
//         else
//         {
//             fprintf(fp, "L");
//         }
//     }
//     fprintf(fp, ";\n");

//     fprintf(fp, "    SI2=");
//     for (uint32_t i = 0; i < length; i++)
//     {
//         fprintf(fp, "HL");
//     }
//     fprintf(fp, ";\n");
// }

// void generate_setsigpart(FILE *fp, const uint8_t *commands)
// {
//     uint8_t signal_mask, signal_status;
//     signal_mask = commands[1];
//     signal_status = commands[2];
//     if (signal_mask & SIG_TDI)
//     {
//         if (signal_status & SIG_TDI)
//         {
//             fprintf(fp, "  SI1=H;\n");
//         }
//         else
//         {
//             fprintf(fp, "  SI1=L;\n");
//         }
//     }
//     if (signal_mask & SIG_TCK)
//     {
//         if (signal_status & SIG_TCK)
//         {
//             fprintf(fp, "  SI2=H;\n");
//         }
//         else
//         {
//             fprintf(fp, "  SI2=L;\n");
//         }
//     }
//     if (signal_mask & SIG_TMS)
//     {
//         if (signal_status & SIG_TMS)
//         {
//             fprintf(fp, "  SI3=H;\n");
//         }
//         else
//         {
//             fprintf(fp, "  SI3=L;\n");
//         }
//     }
//     if (signal_mask & SIG_TRST)
//     {
//         if (signal_status & SIG_TRST)
//         {
//             fprintf(fp, "  SI4=H;\n");
//         }
//         else
//         {
//             fprintf(fp, "  SI4=L;\n");
//         }
//     }
//     if (signal_mask & SIG_SRST)
//     {
//         if (signal_status & SIG_SRST)
//         {
//             fprintf(fp, "  SI5=H;\n");
//         }
//         else
//         {
//             fprintf(fp, "  SI5=L;\n");
//         }
//     }
// }

// void generate_clkpart(FILE *fp, const uint8_t *commands)
// {
//     uint8_t signals, clk_pulses;

//     signals = commands[1];
//     clk_pulses = commands[2];

//     if (signals & SIG_TDI)
//     {
//         fprintf(fp, "  SI1=H;\n");
//     }
//     if (!(signals & SIG_TDI))
//     {
//         fprintf(fp, "  SI1=L;\n");
//     }
//     if (signals & SIG_TMS)
//     {
//         fprintf(fp, "  SI3=H;\n");
//     }
//     if (!(signals & SIG_TMS))
//     {
//         fprintf(fp, "  SI3=L;\n");
//     }

//     fprintf(fp, "    SO2="); // tck
//     for (int i = 0; i < clk_pulses; i++)
//     {
//         fprintf(fp, "HL");
//     }
//     fprintf(fp, ";}\n");
// }

// void generate_pattern(FILE *fp, struct libusb_device_handle *dev_handle, struct jtag_xfer *transfers)
// {
//     fprintf(fp, "Pattern scan {\n");
//     uint8_t *rxbuf = (uint8_t *)transfers->transfer->buffer;
//     uint32_t count = transfers->transfer->actual_length;
//     uint8_t *commands = rxbuf;

//     while ((commands < (rxbuf + count)) && (*commands != CMD_STOP))
//     {
//         switch ((*commands) & 0x0F)
//         {
//         case CMD_INFO:
//             break;
//         case CMD_FREQ:
//             max_frequency=verify_maxfrequency((commands[1]<<9 | commands[2]));
//             commands+=2;
//             break;
//         case CMD_XFER:
//             uint32_t trbytes=xfer_bytes(commands,*commands & EXTEND_LENGTH);
//             uint32_t byte_length = max_frequency ? xfer_bytes(commands,*commands & EXTEND_LENGTH)/8 : 0;
//             uint16_t remaining_length = max_frequency ? xfer_bytes(commands,*commands & EXTEND_LENGTH) & 7 : xfer_bytes(commands,*commands & EXTEND_LENGTH);
//             if (remaining_length){
//                 generate_xferpart(fp, remaining_length,&(commands+2)[byte_length]);
//             }
//             commands+=(7+trbytes)/8+1;
//             break;
//         case CMD_SETSIG:
//             generate_setsigpart(fp, &(*commands));
//             commands+=2;
//             break;
//         case CMD_GETSIG:
//             break;
//         case CMD_CLK:
//             generate_clkpart(fp, &(*commands));
//             commands+=2;
//             break;
//         }
//         commands++;
//     }
   
//     fprintf(fp, "}\n");
// }

// void generate_stil(struct libusb_device_handle *dev_handle,struct jtag_xfer *transfers)
// {
//     FILE *fp;
//     // static int file_count=1;
//     // char filename[50];
//     // snprintf(filename,sizeof(filename),"STIL.stil%d",file_count);
//     // fp = fopen(filename, "w+");
//     // while(fp!=NULL){
//     //     fclose(fp);
//     //     file_count++;
//     //     snprintf(filename,sizeof(filename),"STIL.stil%d",file_count);
//     //     fp = fopen(filename, "w+");
//     // }
//     // fprintf(fp,"qqq\n"); //I WRITE FOR TEST
//     fp=fopen("STIL7.txt","a+");
//     generate_stiltitle(fp);
//     generate_signals( fp,transfers,dev_handle);
//     // generate_signalsgroups(fp,dev_handle,transfers);
//     // generate_Timing(fp,dev_handle,transfers);
//     // generate_patternburst(fp);
//     // generate_patternexec(fp);
//     generate_pattern(fp, dev_handle, transfers);
//     fclose(fp);
// }
//void generate_signals(FILE *fp, struct libusb_transfer *transfer,struct libusb_device_handle *dev_handle,unsigned char *buffer,void *user_data)



void enter_xfer(struct jtag_xfer *transfers){
    uint8_t *rxbuf=(uint8_t*)transfers[0].buf;
	uint8_t *commands=rxbuf;
	
	//LOG_INFO("commands[0]=%02X",commands[0]);
	// if(commands[0]==STLINK_GET_VERSION || commands[0]==STLINK_GET_CURRENT_MODE 
	// || commands[0]==STLINK_GET_TARGET_VOLTAGE){
	// 	LOG_INFO("signals=TDO;");
	// }
	if(commands[0]==STLINK_DEBUG_COMMAND){
		switch(commands[1]){
			case STLINK_DEBUG_APIV2_SWD_SET_FREQ://0x43 read 
				commands+=2;
				break;
			case STLINK_DEBUG_APIV2_WRITE_DAP_REG://0x46 read
				commands+=6;
				break;
			case STLINK_DEBUG_APIV2_ENTER://mode enter 0x30 read
				commands+=2;
				break;
			case STLINK_DEBUG_APIV2_GETLASTRWSTATUS2://0x3e read
				break;
			case STLINK_DEBUG_APIV2_READ_IDCODES://0x31 read
				break;
			case STLINK_DEBUG_APIV2_DRIVE_NRST://0x3c read
				commands+=1;
				break;
			case STLINK_DEBUG_READMEM_32BIT://0x07
				//LOG_INFO("read one time");
				commands+=4; //addr
				commands+=2; //len
				commands+=1; //ap_num 
				commands+=3; //csw
				break;
			case STLINK_DEBUG_WRITEMEM_32BIT://0x08
				// //commands+=4; //addr
				//LOG_INFO("write one time");
				uint16_t length;
				length = (uint16_t)(commands[6]) | ((uint16_t)(commands[7]) << 8);
				generate_stil11(transfers[1].buf,length);
				// commands+=2; //len
				// commands+=1; //ap_num 
				// commands+=3; //csw
				break;
		}
	}
	}

	 void generate_stil11(const uint8_t *buf, uint16_t size){
            FILE *fp;
            fp=fopen("stil11","a+");

            LOG_INFO("length=%02X",size);
            const uint8_t *xfer_in;
            xfer_in = buf;
            fprintf(fp, "    SI1="); // tdi
            for (uint32_t i = 0; i < size; i++)
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
            for (uint32_t i = 0; i < size; i++)
            {
                fprintf(fp, "HL");
            }
            fprintf(fp, ";\n");
			
			fclose(fp);
        }
        