#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "STIL_generate.h"
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


void generate_signalspart(struct jtag_xfer *transfers){
    FILE *fp;
    fp=fopen("signals_part","a+");
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

    fclose(fp);
}