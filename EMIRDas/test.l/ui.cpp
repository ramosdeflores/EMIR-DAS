
/* Compilation flags */
/* Use DBGMSG() macro */
/* #define DEBUG_MESSAGE 1 */

/* Use ERRMSG() macro */
#define ERROR_MESSAGE 1

/* Headers */
#include "fcntl.h"
#include <stdio.h>          /* fopen(), fclose(), fgets() */
#include "stdlib.h"         /* malloc(), free() */
#include "string.h"         /* strtok(), sscanf() */
#include "time.h"           /* difftime() */
#include "msgQLib.h"        /* MSG_Q_ID */
#include "semLib.h"
#include "taskLib.h"        /* taskDelay() */
#include "sysLib.h"         /* sysClkRateGet() */
#include "cacheLib.h"       /* CACHE_DMA_VIRT_TO_PHYS() */
#include <unistd.h>

#define CPLUS
#include "ct_interface.h"
#include "sdsu_emir_driver.h"    /* sdsu_init(), sdsu_cleanup() */
/* We do not want to include this header file for a stand alone SDSU test engineering system */
/*#include "sdsu_emir_auto.h"*/

/* Constants */
/* Number of frame header bytes */
#define SDSU_HDRSIZE 24
/* Macro for determing the data buffer size; arguments are number of columns and rows, x and y binning sizes */
#define SDSU_BUFSIZE(a,b,c,d) ((((a*b)*2)/(c*d))+SDSU_HDRSIZE)
/* Macro for determing the frame size; arguments are number of columns and rows, x and y binning sizes */
#define SDSU_FRAMESIZE(a,b,c,d) ((((a*b)*2)/(c*d)))

/* Start delimiter for parameters in SDSU application */
#define SDSU_START_PARAM "START_PAR_VALUES"
/* End delimiter for parameters in SDSU application */
#define SDSU_END_PARAM   "END_PAR_VALUES"

/* OUTPUT timing board application parameter value for left channel readout */
#define SDSU_LEFTCHAN  1
/* OUTPUT timing board application parameter value for right channel readout */
#define SDSU_RIGHTCHAN 0


#define	MAX_PARAM	256

#define	_BEGIN		0
#define	_DATA		1
#define	_SYMBOL		2
#define	_START		3
#define	_END		4
#define	_ENDOFFILE	5

#define	BUSY		1
#define	IDLE		0

#define NUM_FBUFFS	50

char appFile[256];
char appInfo[256];
int  numParam;
typedef struct {
     char   name[64];
     int    memType;
     UINT32 address;
     UINT32 value;
     int    readOnly;
} PARAM;

typedef struct {
     int writeF;
     char fileName[256];
     int number[NUM_FBUFFS];
     void *buffer[NUM_FBUFFS];
     long secstamp[NUM_FBUFFS];
     long nsecstamp[NUM_FBUFFS];
     int bufSize;
     int state;
     int numBuffs;
     unsigned int xSize;
     unsigned int ySize;
     unsigned int index;
} FRAME;

SEM_ID writeSem;
static int writeTask;

int cfp;

PARAM param[MAX_PARAM];
FRAME frame;
char fitsFile[256];

int saveFrameTask(void) {

    FILE *fp;
    int i, current;
    char fitsFile[256];
    char *p;
    int Chunk, step;

    frame.numBuffs = NUM_FBUFFS;

    while( 1 ) {
         semTake(writeSem, WAIT_FOREVER);
         frame.state = BUSY;
         Chunk = frame.bufSize / 1;

	   while ( (current = frame.number[frame.index]) > -1 ) {
              frame.number[frame.index] = -2;
              sprintf(fitsFile,"%s.%04d.fits", frame.fileName, current);

              if ((fp = fopen(fitsFile, "wb")) == NULL) {
                   return -1;
              }

              fprintf(fp, "SIMPLE  =                    T /                                                ");
              fprintf(fp, "BITPIX  =                   16 /                                                ");
              fprintf(fp, "NAXIS   =                    2 /                                                ");
/*              fprintf(fp, "NAXIS1  =                 %4.4d /                                                ", frame.xSize);
              fprintf(fp, "NAXIS2  =                 %4.4d /                                                ", frame.ySize);*/
              fprintf(fp, "NAXIS1  =                 %4.4d /                                                ", 2048);
              fprintf(fp, "NAXIS2  =                 %4.4d /                                                ", 2048);
              fprintf(fp, "BSCALE  =                  1.0 /                                                ");
              fprintf(fp, "BZERO   =                32768 /                                                ");
              fprintf(fp, "SECS    =                %05d /                                                ", frame.secstamp[frame.index]);
              fprintf(fp, "MSECS   =                 %04d /                                                ", frame.nsecstamp[frame.index] / 1000000);
              fprintf(fp, "END                                                                             ");
              for (i = 0; i < 26; i++)
                  fprintf(fp, "                                                                                ");


              p = (char *)frame.buffer[frame.index];
              for ( step = 0; step < 1; ++step) {
	           if (fwrite((char *) p, 1, Chunk, fp) != Chunk) {
                      return -1;
                   }
                   p += Chunk; 
              }

              fclose(fp);

              frame.number[frame.index] = -1;

              printf("%s\n", fitsFile);

              frame.index = ++frame.index % frame.numBuffs;
           }
         frame.state = IDLE;
      }

    return 0;
}

int initCt() {

     int i;

     appFile[0] = '\0';
     numParam  = 0;
     for (i = 0; i < MAX_PARAM; ++i ) {
          param[i].name[0] = '\0';
          param[i].memType = -1;
          param[i].address = 0;
          param[i].value   = 0;
     }

     /* Initialize the SDSU PMC board interface */
     if ( (cfp = copen("sdsu controller", 0)) == ERR_OPENCT ) {
          return ERR_OPENCT;
     }

     if ( (cfp = fgopen("sdsu frame grabber", 0)) == ERR_OPENFG ) {
          return ERR_OPENFG;
     }

     writeSem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
     writeTask = taskSpawn("writert", 50, 0, 10000, (FUNCPTR)saveFrameTask,0,0,0,0,0,0,0,0,0,0);

     return 0;
}

int MatchKeyWord(char *keyWord) {

     if ( strcmp(keyWord, "_DATA") == 0 ) {
         return _DATA;
     }

     if ( strcmp(keyWord, "_SYMBOL") == 0 ) {
         return _SYMBOL;
     }

     if ( strcmp(keyWord, "_END") == 0 ) {
         return _END;
     }

     if ( strcmp(keyWord, "_START") == 0 ) {
         return _START;
     }

     return -1;
}

int LoadData(FILE *fp, char *appTempName, char *keyWord)  {

     char charMemType = 'N';
     unsigned int memType;
     int key;
     int retVal;
     INT32 address = 0;
     INT32 value;

     if ( fscanf(fp, " %c %x", &charMemType, &address) == 2 ) {
          switch (charMemType) {
               case 'P' :
               case 'p' : memType = MEMP;
                         break;

               case 'X' :
               case 'x' : memType = MEMX;
                         break;

               case 'Y' :
               case 'y' : memType = MEMY;
                         break;

               default: 
                        return -1;
                        break;
         }
     } else {
         return -1;
     }

     if ( (retVal = cioctl(CONTROLLER, SET_MTYPE, &memType)) != NO_ERROR ) {
         return retVal;
     }

     if ( (retVal = clseek(CONTROLLER, address, SEEK_SET )) < NO_ERROR ) {
         return retVal;
     }

     fscanf(fp, "%s", keyWord);
     while (keyWord[0] != '_') {
          sscanf(keyWord, "%x", &value);
          cwrite(CONTROLLER, &value, 1);
          fscanf(fp, "%s", keyWord);
     }

     key = MatchKeyWord(keyWord);

     return key;
}
   
int FindKeyWord(FILE *fp, char *keyWord) {

     int key;

     fscanf(fp, "%s", keyWord) ;
     do {
          if ( (key = MatchKeyWord(keyWord)) != -1 ) {
               return key;
          }
     } while ( fscanf(fp, "%s", keyWord) != EOF );

     return _ENDOFFILE;
}

int LoadSymbol(FILE *fp, char *appTempName, char *keyWord) {
     
     char charMemType;
     UINT32 memType;
     INT32 value;
     int i;
     int key;
     int retVal;

     if ( fscanf(fp, " %c", &charMemType) == 1 ) {
          switch (charMemType) {
               case 'P' :
               case 'p' : memType = MEMP;
                         break;

               case 'X' :
               case 'x' : memType = MEMX;
                         break;

               case 'Y' :
               case 'y' : memType = MEMY;
                         break;

               case 'N':
               case 'n': break;

               default: 
                        return -1;
                        break;
         }
     } else {
         return -1;
     }

     if ( (retVal = cioctl(CONTROLLER, SET_MTYPE, &memType)) != NO_ERROR ) {
         return retVal;
     }

     
     fscanf(fp, "%s", keyWord);
     while (keyWord[0] != '_') {
          for ( i = 0; i < numParam; ++i) {
               if ( strcmp(param[i].name, keyWord) == 0 ) {
                    param[i].memType = memType;
                    fscanf(fp, " %*c %x\n", &param[i].address);
                    if ( param[i].readOnly == 0 ) {
                    	if ( (retVal = clseek(CONTROLLER, param[i].address, SEEK_SET)) < NO_ERROR ) {
				return retVal;
			}
			value = param[i].value;
			cwrite(CONTROLLER, &value, 1);
                    }
                    break;
               }
          }
          fscanf(fp, "%s", keyWord);
     }

     key = MatchKeyWord(keyWord);

     return key;
}

int LoadCfg(FILE *fp) {
    int i = 0;
    char var[256];
    INT32  value;

    fscanf(fp, "%s", var);
    do {
         if ( var[0] == '#' ) {
              fscanf(fp, "%*[^\n]%*c");
         } else {
              fscanf(fp, " = %d", &value);
              sprintf(param[i].name, "%s", var);
              param[i].value = value;
              param[i].readOnly = ( value == 99999 ) ? 1 : 0;
              ++i;
         }
     } while ( fscanf(fp, "%s", var) != EOF );

     numParam = i;

     return NO_ERROR;
}

int loadCt(char *appName) {

     char appTempName[256];
     char cfgTempName[256];
     char *appDir;
     char keyWord[256];
     FILE *fp, *cfgfp;
     int  retVal;
     int key = _BEGIN;
     int Status = _START;

     sprintf(appTempName, "%s.lod", appName);
     if ( (fp = fopen(appTempName, "r")) == NULL ) {
          return -1;
     }

     sprintf(cfgTempName, "%s.cfg", appName);
     if ( (cfgfp = fopen(cfgTempName, "r")) == NULL ) {
;
     } else {
          LoadCfg(cfgfp);
          fclose(cfgfp);
     }

     do {
     switch (key) {
          case _BEGIN : key = FindKeyWord(fp, keyWord);
                        break;

          case _START : if ( Status == _START ) {
                             (void) fscanf(fp, "%[^\n]", appInfo);
                             (void) fgetc(fp);
                             Status = _DATA;
                             key = FindKeyWord(fp, keyWord);
                        } else {
                             fclose(fp);
                             return -1;
                        }
                        break;

          
          case _DATA : if ( Status == _DATA ) {
                            key = LoadData(fp, appTempName, keyWord);
                       } else {
                             fclose(fp);
                             return -1;
                       }
                       break;

         case _SYMBOL:if ( Status == _DATA ) {
                           key = LoadSymbol(fp, appTempName, keyWord);
                           Status = _SYMBOL;
                       } else {
                             if ( Status == _SYMBOL ) {
                                  key = LoadSymbol(fp, appTempName,  keyWord);
                             } else {
                                  fclose(fp);
                                  return -1;
                             }
                       }
                       break;

        case _END: if ( Status == _SYMBOL ) {
                        fclose(fp);
                        return NO_ERROR;
                   } else {
                         fclose(fp);
                         return -1;
                   }
                   break;

       case _ENDOFFILE: 
                        fclose(fp);
                        return -1;
                        break;

       default: 
                fclose(fp);
                return -1;
                break;
       }

    } while ( 1 );

    return NO_ERROR;

}

int execCt() {

    return cioctl(CONTROLLER, START_SDSU, NULL);

}

int resetCt() {

    return cioctl(CONTROLLER, RESET, NULL);

}

int closeCt() {

    taskDelete(writeTask);
    semDelete(writeSem);

    cclose(CONTROLLER);
    fgclose(FRAME_GRABBER);

    return 0;
}

int listParamCt() {

  int i;
  int retVal;

  for (i = 0; i < numParam; ++i) {
     if ( param[i].memType == -1 ) {
          printf("%s=%d but not found in application.\n");
          continue;
     }

     if ( (retVal = cioctl(CONTROLLER, SET_MTYPE, &param[i].memType)) != NO_ERROR ) {
         return retVal;
     }

     if ( (retVal = clseek(CONTROLLER, param[i].address, SEEK_SET )) < NO_ERROR ) {
         return retVal;
     }

     if ( (retVal = cread(CONTROLLER, (INT32 *)&param[i].value, 1)) != 1 ) {
         return retVal;
     }

     printf("%s=%d\n", param[i].name, param[i].value);
  }

  return 0;
}

int setParamCt(char *name, int value) {

  int i;
  int retVal = NO_ERROR;
  int result;

  for (i = 0; i < numParam; ++i) {
     if ( strcmp(name, param[i].name) == 0 ) {
        if ( param[i].memType == -1 ) {
            printf("setParamCt> Parameter %s defined in configuration file but not in application.\n", param[i].name);
            return -1;
        }

        if ( (result = cioctl(CONTROLLER, SET_MTYPE, &param[i].memType)) != NO_ERROR ) {
            return result;
        }

        if ( (result = clseek(CONTROLLER, param[i].address, SEEK_SET )) < NO_ERROR ) {
            return result;
        }

        if ( (result = cwrite(CONTROLLER, (INT32 *)&value, 1)) != 1 ) {
            return result;
        }

        break;
     }
  }

  if ( i == numParam ) {
        retVal = -1;
  }

  return retVal;
}

INT32 getParam(char *name) {

  int i;
  int retVal = NO_ERROR;
  INT32 value;

  for (i = 0; i < numParam; ++i) {
     if ( strcmp(name, param[i].name) == 0 ) {
        if ( param[i].memType == -1 ) {
            printf("getParam> Parameter %s defined in configuration file but not in application.\n", param[i].name);
            return -1;
        }

        if ( (retVal = cioctl(CONTROLLER, SET_MTYPE, &param[i].memType)) != NO_ERROR ) {
            return retVal;
        }

        if ( (retVal = clseek(CONTROLLER, param[i].address, SEEK_SET )) < NO_ERROR ) {
            return retVal;
        }

        if ( (retVal = cread(CONTROLLER, (INT32 *)&value, 1)) != 1 ) {
            return retVal;
        }

        break;
     }
  }

  if ( i == numParam ) {
        retVal = -1;
  }

  return value;
}

int stopCt() {

    return cioctl(CONTROLLER, STOP_SDSU, NULL);

}

int saveFrame(char *fitsFile, void *buffer, INT32 bufSize, INT32 xSize, INT32 ySize) {

    FILE *fp;
    int i;

    if ( fitsFile == NULL ) {
         return -1;
    }

    if ((fp = fopen(fitsFile, "wb")) == NULL) {
         return -1;
    }

    fprintf(fp, "SIMPLE  =                    T /                                                ");
    fprintf(fp, "BITPIX  =                   16 /                                                ");
    fprintf(fp, "NAXIS   =                    2 /                                                ");
    fprintf(fp, "NAXIS1  =                 %4.4d /                                                ", xSize);
    fprintf(fp, "NAXIS2  =                 %4.4d /                                                ", ySize);
    fprintf(fp, "BSCALE  =                    1 /                                                ");
    fprintf(fp, "BZERO   =                    0 /                                                ");
    fprintf(fp, "END                                                                             ");
    for (i = 0; i < 28; i++)
        fprintf(fp, "                                                                                ");

    if (fwrite((char *) buffer, 1, bufSize, fp) != bufSize) {
        return -1;
    }

    fclose(fp);

    return 0;
}


int getFramesCt(char *file) {

    INT32 buffsize, numFrames, numSeq, optrst, nrst;
    INT32 nrdbl, numExp, nrdil, xSize, ySize, expTime;
    INT32 output, bufSize;
    int i, i1, retVal;
    void *buffer[NUM_FBUFFS];
    char *dataDir;
    int buffIndex;
    int currentFrame;
    int index;
    int hold;
    struct timespec timestamp, basestamp;

    dataDir = getenv("SDSU_DATADIR");

    if ( file == NULL ) {
         if ( fitsFile == NULL ) {
              sprintf(fitsFile, "%s/default.fits", dataDir);
         } 
    } else {
	if ( dataDir != NULL )
         sprintf(fitsFile, "%s/%s", dataDir, file);
	else
	 sprintf(fitsFile, "%s", file);
    }

    if ((numSeq = getParam("NSEC")) < 1 ) {
         return numSeq;
    }

    if ((numExp = getParam("NO_EXPOSURES")) < 1 ) {
         return numExp;
    }

    if ((nrst = getParam("NRST")) < 0 ) {
         return nrst;
    }

    if ((nrdbl = getParam("NRDBL")) < 0 ) {
         return nrdbl;
    }

    if ((nrdil = getParam("NRDIL")) < 0 ) {
         return nrdil;
    }

    if ((optrst = getParam("OPTRST")) < 0 ) {
         return optrst;
    }

    numFrames = numSeq * ( ((optrst & 2) ? nrst : 0) + nrdbl + numExp * nrdil);

/*    printf ("Reading %d = %d * ( ((%d & 2) ? %d : 0) + %d + %d * %d).\n", numFrames, numSeq, optrst, nrst, nrdbl, numExp, nrdil);*/
    printf ("FRAMES %d\n", numFrames);

    
    if ((expTime = getParam("EXPOSE_TIME")) < 0 ) {
         return expTime;
    }

    if ((xSize = getParam("X_SIZE")) < 0 ) {
         return xSize;
    }

    if ((ySize = getParam("Y_SIZE")) < 0 ) {
         return ySize;
    }

    bufSize = ((xSize * ySize) * 2 ) + SDSU_HDRSIZE;

/*    memset(buffer, 0, bufSize);*/

    sprintf(frame.fileName, "%s", fitsFile);
    frame.bufSize = bufSize - SDSU_HDRSIZE;
    frame.xSize = xSize;
    frame.ySize = ySize;

    for ( i = 0; i < frame.numBuffs; ++i )
         if ( (buffer[i] = (void *)memalign(SDSU_BURST_BLOCK, bufSize)) == NULL) {
              for ( i1 = 0; i1 < i; ++i1 )
                   free(buffer[i1]);
              return -1;
         }

    if ( (retVal = cioctl(CONTROLLER, START_AQU, NULL)) != NO_ERROR) {
         for ( i1 = 0; i1 < frame.numBuffs; ++i1 )
              free(buffer[i1]);
         return -1;
    }

    for ( i1 = 0; i1 < frame.numBuffs; ++i1 ) {
         frame.buffer[i1] = (char *)buffer[i1] + SDSU_HDRSIZE;
         frame.number[i1] = -1;
    }

    frame.index = 0;
    buffIndex = 0;

    clock_gettime(CLOCK_REALTIME, &basestamp);
    for (i = 0; i < numFrames; ++i) {
         clock_gettime(CLOCK_REALTIME, &timestamp);
         currentFrame = buffIndex;
         if ( fgread(FRAME_GRABBER, (UINT32 *)buffer[buffIndex], bufSize) != bufSize) {
              for ( i1 = 0; i1 < frame.numBuffs; ++i1 )
                   free(buffer[i1]);
              return -1;
         } else {
              hold = (buffIndex + 1) % frame.numBuffs;
              if ( frame.number[hold] > -1 || frame.number[hold] == -2 ) {
/*                   if ( frame.number[buffIndex] == -2 )
                        buffIndex = ((currentFrame == 0) ? frame.numBuffs : currentFrame) - 1;
                   else
                        buffIndex = currentFrame;
                   frame.number[buffIndex] = -1;*/
/*                   printf("Lost %d buffIndex = %d frame.number = %d frame.index = %d\n", i,buffIndex, frame.number[buffIndex], frame.index);*/
              } else {
/*                   printf("Got %d\n", i);*/
                   frame.secstamp[currentFrame] = timestamp.tv_sec - basestamp.tv_sec;
                   frame.nsecstamp[currentFrame] = timestamp.tv_nsec - basestamp.tv_nsec;
                   frame.number[currentFrame] = i;
                   semGive(writeSem);
                   buffIndex = hold;
              }
         }
   }

/*   printf("\n");*/

   sleep(1);
   while ( frame.state == BUSY )
        sleep(1);

   for ( i1 = 0; i1 < frame.numBuffs; ++i1 )
        free(buffer[i1]);

    return NO_ERROR;
}

int proto(void) {

	char strarg[256];
	int  intarg;
	int  cmd;
	int  cont = 1;

	while ( cont ) {
		printf("OK>\n");
		scanf("%d", &cmd);
		switch ( cmd ) {
			case 0:
				break;

			case 1: if ( initCt() )
					printf("NACK>\n");
				else
					printf("ACK>\n");
				break;

			case 2: printf("PAR>\n");
				scanf("%s", strarg);
				if ( loadCt(strarg) )
					printf("NACK>\n");
				else
					printf("ACK>\n");
				break;

			case 3: if ( execCt() )
					printf("NACK>\n");
				else
					printf("ACK>\n");
				break;

			case 4: if ( listParamCt() )
					printf("NACK>\n");
				else
					printf("ACK>\n");
				break;

			case 5: printf("PAR>\n");
				scanf("%s %d", strarg, &intarg);
				if ( setParamCt(strarg, intarg) )
					printf("NACK>\n");
				else
					printf("ACK>\n");
				break;

			case 6: printf("PAR>\n");
				scanf("%s", strarg);
				if ( getFramesCt(strarg) )
					printf("NACK>\n");
				else
					printf("ACK>\n");
					sleep(1);
				break;

			case 7: if ( closeCt() )
					printf("NACK>\n");
				else
					printf("ACK>\n");
				break;

			case 8: cont = 0;
				break;

			default: printf("NACK>\n");
				break;

		}
	}

	printf("OK>\n");
	return 0;
}

