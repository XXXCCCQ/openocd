#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "generate_signals.h"
#include <helper/log.h>
#include <string.h>


// 检查文件中是否已经存在相同内容
bool checkExistence(FILE *file, const char *content) {
    if (file == NULL) {
        return false;  // 文件不存在
    }
    rewind(file);
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
         if (strstr(buffer, content) != NULL) {
            return true;  // 找到相同内容
        }
    }
    return false;  // 未找到相同内容
}

int count0ccurrences(FILE *fp,const char *str){
    rewind(fp);
    char line[1024];
    int count=0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, str) != NULL) {
            count++;
        }
    }
    return count;
}

void generate_signalspart(FILE *fp, struct jtag_xfer *transfers){
    // FILE *fp;
    // fp=fopen("signals_part","a+");
    if(!checkExistence(fp,"signals{")){;
		fprintf(fp,"signals{;\n");
	}

    uint8_t *rxbuf = (uint8_t *)transfers[0].buf;
    uint8_t *commands = rxbuf;
	if(commands[0]==STLINK_GET_VERSION || commands[0]==STLINK_GET_CURRENT_MODE 
	|| commands[0]==STLINK_GET_TARGET_VOLTAGE){
    if(!checkExistence(fp,"TDO In")){;
		fprintf(fp,"  TDO In;\n");
	}
    }

    if(commands[0]==STLINK_DEBUG_COMMAND){
        switch(commands[1]){
            case STLINK_DEBUG_APIV2_SWD_SET_FREQ:
            case STLINK_DEBUG_APIV2_WRITE_DAP_REG:
            case STLINK_DEBUG_APIV2_ENTER:
            case STLINK_DEBUG_APIV2_GETLASTRWSTATUS2:
            case STLINK_DEBUG_APIV2_READ_IDCODES:
            case STLINK_DEBUG_APIV2_DRIVE_NRST:
            case STLINK_DEBUG_READMEM_32BIT:
                if(!checkExistence(fp,"TDO In")){
                    fprintf(fp,"  TDO In;\n");
                    }
                break;
            case STLINK_DEBUG_WRITEMEM_32BIT:
                if(!checkExistence(fp,"TDI Out;TCK Out")){
                    fprintf(fp,"  TDI Out;TCK Out\n");
                }
                break;
        }
    }

    if(!checkExistence(fp,"}")){
        fprintf(fp,"  }\n");
        }

    //fclose(fp);
}


void generate_signalsgrouppart(FILE *fp){
    if(!checkExistence(fp,"signalsgroup")){
        fprintf(fp,"signalsgroup{\n");
    }
    if(!checkExistence(fp,"ABUS='TDO'")){
        if(checkExistence(fp,"TDO")){
            fprintf(fp,"    ABUS='TDO';\n");
        }
    }
    if(!checkExistence(fp,"BBUS='TDI+TCK'")){
        if(checkExistence(fp,"TDI")){
            fprintf(fp,"    BBUS='TDI+TCK';\n");
        }
    }
    if(!checkExistence(fp,"SO1='TDI'")){
        if(checkExistence(fp,"BBUS='TDI+TCK'")){
            fprintf(fp,"    SO1='TDI'; {ScanOut 30;}\n");
            fprintf(fp,"    SO2='TCK'; {ScanOut 30;}\n");
        }
    }

    if(!checkExistence(fp,"SI1='TDO'")){
        if(checkExistence(fp,"ABUS='TDO'")){
            fprintf(fp,"    SI1='TDO'; {ScanIn 30;}\n");
        }
    }

    int count = count0ccurrences(fp,"}");

    if( count==1 ){
        fprintf(fp,"}\n");
    }
}

void generate_timingpart(FILE *fp){

    if(!checkExistence(fp,"Timing")){
        fprintf(fp, "Timing{\n");
        fprintf(fp, "  WaveformTable scan{\n");
        fprintf(fp, "    Period '500ns';\n");
        fprintf(fp, "    Waveforms{\n");
        fprintf(fp, "      ALL{ 10 {'0ns' U/D;}}\n");
        fprintf(fp, "      ALL{ HLZX {'0ns' Z;'260ns' H/L/Z/X;}}\n");
        fprintf(fp, "    }\n");
        fprintf(fp, "  }\n");
        fprintf(fp, " }\n");
    }
}

void generate_patternburstpart(FILE *fp){
    if(!checkExistence(fp,"PatternBurst")){
        fprintf(fp, "PatternBurst 'scan_burst' {\n");
        fprintf(fp, "  PatList{ 'scan' ;}\n");
        fprintf(fp, "}\n");
    }
}

void generate_patternExecpart(FILE *fp){
    if(!checkExistence(fp,"PatternExec")){
        fprintf(fp, "PatternExec {\n");
        fprintf(fp, "  PatternBurst scan_burst ;\n"  );
        fprintf(fp, "}\n");
    }
}

void generate_patternsignals(FILE *fp,const uint8_t *buf, uint16_t size){

    //LOG_INFO("length=%02X",size);
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
    
}

void generate_patternpart(FILE *fp,struct jtag_xfer *transfers){
    if(!checkExistence(fp,"Pattern 'Scan'")){;
		fprintf(fp,"Pattern 'Scan'{;\n");
	}

    uint8_t *rxbuf = (uint8_t *)transfers[0].buf;
    uint8_t *commands = rxbuf;

    if(commands[0]==STLINK_DEBUG_COMMAND){
        switch(commands[1]){
            case STLINK_DEBUG_WRITEMEM_32BIT:
                uint16_t length;
				length = (uint16_t)(commands[6]) | ((uint16_t)(commands[7]) << 8);
				generate_patternsignals(fp,transfers[1].buf,length);
                break;
        }
    }
    
    int count = count0ccurrences(fp,"}");

    if( count==6 ){
        fprintf(fp,"}\n");
    }

}