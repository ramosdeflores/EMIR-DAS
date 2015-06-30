#include <string.h>
#include <stdio.h>

#define CPLUS
#ifdef VXWORKS
#include "msgQLib.h"
#include "../SDSUlcu.l/ct_interface.h"
#include "../SDSUlcu.l/sdsu_emir_driver.h"    /* sdsu_init(), sdsu_cleanup() */
#else
#include "../SDSUsim.l/ct_interface.h"
#endif


#include "DetectorControllerDefs.h"
#include "DetectorControllerApp.h"

using namespace std;

/****************************************************************
 *
 *  matchKeyWord: checks if a given keyword is valid
 *
 *  returns: key value of NOT_FOUND
 *
 ****************************************************************/
int DetectorControllerApp::matchKeyWord(char *keyWord) {
   int index;
   const char *keyList[4] = { "_DATA", "_SYMBOL", "_END", "_START" };
   int keyVal[4] = { _DATA, _SYMBOL, _END, _START };

   for (index = 0; index < 4; ++index) {
      if ( strcmp(keyWord, keyList[index]) == 0 ) {
         return keyVal[index];
      }
   }

   return NOT_FOUND;
}

/****************************************************************
 *
 *  findKeyWord: searches for a keyword in the code
 *  pointed by p.
 *
 *  returns: the key value associated with the keyword, NOT_FOUND
 *     if the keyword is not recognized as valid or _ENDOFFILE
 *     if the end of the program is reached.
 *
 ****************************************************************/
int DetectorControllerApp::findKeyWord(char **p, char *keyWord) {
   int key;
   char hold[256];


   sscanf(*p, "%[^ ]", hold);

   do {
      *p += strlen(hold);

      sscanf(hold, "%s", keyWord);
      if ( (key = matchKeyWord(keyWord)) != NOT_FOUND ) {
         return key;
      }
   } while ( sscanf(*p, "%s", hold) != 0 );


   return _ENDOFFILE;
}

/****************************************************************
 *
 *   loadData: Write the data section of the program to the controller.
 *      The function writes to the controller the values starting from
 *      where p is pointing and ending when a symbol is found.
 *
 *   returns: APP_BAD if ta problem with the code is found,
 *      the error code returned by the io funtions used for accesing
 *      the controller,
 *      the key associated with the keyword
 *
 ****************************************************************/
int DetectorControllerApp::loadData(char **p, char *keyWord, bool wr) {
    char charMemType = 'N';
    unsigned int memType;
    int key,retVal, count;
    INT32 address = 0, value;


    if ( (count = sscanf(*p, " %c %x", &charMemType, &address)) == 2 ) {
         *p += 9;  /* Two spaces, the memory type and 6 characters for the address */
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

              default: return APP_BAD;
                       break;
        }
    } else {
        return APP_BAD;
    }


    if ( wr ) {
       if ( (retVal = cioctl(CONTROLLER, SET_MTYPE, &memType)) != NO_ERROR ) {
          return retVal;
       }

       if ( (retVal = clseek(CONTROLLER, address, SEEK_SET )) < NO_ERROR ) {
          return retVal;
       }
    }

    sscanf(*p, "%s", keyWord);
    *p += strlen(keyWord) + 1;
    while (keyWord[0] != '_') {
         sscanf(keyWord, "%x", &value);
       if ( wr ) {
         if ( (retVal = cwrite(CONTROLLER, &value, 1)) != 1 ) {
            return APP_BAD;
         }
       }
         if ( sscanf(*p, "%s", keyWord) != 1 ) {
            return _ENDOFFILE;
         }
         if ( strlen(keyWord) == 1 ) {
                 *p += 1; // Skip newline
                 if ( sscanf(*p, "%s", keyWord) != 1 ) {
                   return _ENDOFFILE;
                 }
         }
         *p += strlen(keyWord) + 1;
    }


    *p += 1;

    key = matchKeyWord(keyWord);

    return key;
}

/****************************************************************
 *
 *   loadSymbol: Advances to the end of the symbol section.
 *
 *   returns: APP_BAD if ta problem with the code is found or
 *      the key associated with the keyword
 *
 ****************************************************************/
int DetectorControllerApp::loadSymbol(char **p, char *keyWord) {

     int index, key, retVal, Count = 0;
     char charMemType, hold[256];
     UINT32 memType;
     INT32 value;

     if ( sscanf(*p, " %c", &charMemType) == 1 ) {
          *p += 2;
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

               default: return APP_BAD;
                        break;
         }
     } else {
         return APP_BAD;
     }

     sscanf(*p, "%s", keyWord);
     *p += strlen(keyWord);

     do {
        sscanf(*p, "%[^I]", hold);
        // Advance String + 'I' + space
        *p += strlen(hold) + 2;

        sscanf(*p, "%s", hold);
        *p += strlen(hold);
          ++Count;

          sscanf(*p, "%s", keyWord);
          *p += strlen(keyWord) + 1;
     } while ( keyWord[0] != '_');

     key = matchKeyWord(keyWord);


     return key;
}


int DetectorControllerApp::findSymbol(char **p, const char *keyWord, unsigned int *address, int *memType) {

    char charMemType, paramName[256], hold[256];
    int key;

    if ( sscanf(*p, " %c", &charMemType) == 1 ) {
         *p += 2;
         switch (charMemType) {
              case 'P' :
              case 'p' : *memType = MEMP;
                        break;

              case 'X' :
              case 'x' : *memType = MEMX;
                        break;

              case 'Y' :
              case 'y' : *memType = MEMY;
                        break;

              case 'N':
              case 'n': break;

              default: return APP_BAD;
                       break;
        }
    } else {
        return APP_BAD;
    }

    sscanf(*p, "%s", paramName);
    *p += strlen(paramName);


    do {
        sscanf(*p, "%[^I]", hold);
        // Advance String + 'I' + space
        *p += strlen(hold) + 2;

        sscanf(*p, "%s", hold);
        *p += strlen(hold);
        if ( strcmp(keyWord, paramName) == 0) {
            sscanf(hold,"%x", address);
           return PARAM_FOUND;
        }
        else {
        }

         sscanf(*p, "%s", paramName);
         *p += strlen(paramName) + 1;

    } while ( paramName[0] != '_');

    key = matchKeyWord(paramName);

    return key;
}

DetectorControllerApp::DetectorControllerApp(const void *application, const size_t appSize) {

   trace_ = new Trace("DetectorControllerApp");

   app = (void *)application;
   size = appSize;

   DetectorControllerApp::setStat(APP_IDLE);
   cd = 9999;

   // Get reference to the start of _SYMBOL
   DetectorControllerApp::parse(false);
}

int DetectorControllerApp::stop(void) {
   int result;

   if (cd == -1 || appStat != APP_LOADED)
      return APP_ERROR;

   if ( (result = cioctl(CONTROLLER, STOP_SDSU, NULL)) != NO_ERROR ) {
   //               SLOG(DAS::SLogger::SLOG_ERROR,"%s %d","DetectorControllerApp::stop cioctl returned", result);
           return APP_ERROR;
   }

   return APP_OK;
}

/****************************************************************
 *
 *   load: Load the application into the controller.
 *         A timer is set to detect connection problems
 *
 ****************************************************************/
int DetectorControllerApp::load() {

   // If the timeout runs out, then the assumption is that we
   // have lost communication with the controller. The reason
   // is flour from a different sack (Harina de otro costal, que le dicen)
   

   return DetectorControllerApp::parse(true);

}

int DetectorControllerApp::parse(bool wr) {

   char keyWord[256], appInfo[256], *p;
   int key = _BEGIN, Status = _START;

   if (cd == -1  )
      return APP_ERROR;

   symbolStart = NULL;
   p = (char *)app;
   do   {
         switch (key) {
         case _BEGIN   :key = findKeyWord(&p, keyWord);
                        break;

         case _START   :if ( Status == _START ) {
                           (void) sscanf(p, "%[^\n]", appInfo);
                           p += strlen(appInfo);
                           Status = _DATA;
                           key = findKeyWord(&p, keyWord);
                        } else {
                           return APP_BAD;
                        }
                        break;

         case _DATA   :if ( Status == _DATA ) {
                          key = loadData(&p, keyWord, wr);
                       } else {
                          return APP_BAD;
                       }
                       break;

         case _SYMBOL: if ( Status == _DATA || Status == _SYMBOL ) {
                          if ( symbolStart == NULL )
                             symbolStart = p;
                             key = loadSymbol(&p, keyWord);
                             Status = _SYMBOL;
                          } else {
                             return APP_BAD;
                          }
                          break;

         case _END   : if ( Status == _SYMBOL ) {
                          if ( wr )
                             DetectorControllerApp::setStat(APP_LOADED);
                          return APP_OK;
                       } else {
                          return APP_BAD;
                       }
                       break;

         case _ENDOFFILE: return APP_BAD;
                          break;

         default      : return APP_BAD;
                        break;
         }
      } while ( true );

   // Should never get here
   return APP_ERROR;
}

int DetectorControllerApp::exec(void) {
   int result;

   if (cd == -1 || appStat != APP_LOADED)
      return APP_ERROR;

    if ( (result = cioctl(CONTROLLER, START_SDSU, NULL)) != NO_ERROR ) {
        return APP_ERROR;
    }


   return APP_OK;
}

int DetectorControllerApp::startAqu(void) {
   int result;

   if (cd == -1 || appStat != APP_LOADED)
      return APP_ERROR;

    if ( (result = cioctl(CONTROLLER, START_AQU, NULL)) != NO_ERROR ) {
        return APP_ERROR;
    }

   return APP_OK;
}

int DetectorControllerApp::run(void) {
   int result;

   if (cd == -1)
      return APP_ERROR;

   if ( appStat != APP_LOADED ) {
      if ((result = DetectorControllerApp::load()) != APP_OK) {
         return result;
      }
      return DetectorControllerApp::exec();
   }

   return APP_ERROR;
}

int DetectorControllerApp::write(const char *paramName, int value) {
   int index, memType, result;
   unsigned int address;
   char *p;

   if (appStat != APP_LOADED)
      return APP_ERROR;

   p = symbolStart;
   do {
      if ( (result = findSymbol(&p, paramName, &address, &memType)) == PARAM_FOUND) {
         if ( (result = cioctl(CONTROLLER, SET_MTYPE, &memType)) != NO_ERROR ) {
            return APP_ERROR;
         }

         if ( (result = clseek(CONTROLLER, address, SEEK_SET )) < NO_ERROR ) {
            return APP_ERROR;
         }

         if ( (result = cwrite(CONTROLLER, &value, 1)) != 1 ) {
            return APP_ERROR;
         }

         return APP_OK;
      }
   } while ( result == _SYMBOL );

   // If we end up here, the parameter has not been found.
   return NOT_FOUND;
}

int DetectorControllerApp::read(const char *paramName, int *value) {
   int index, memType, result;
   unsigned int address;
   char *p;

   *value = 9999;

   if (appStat != APP_LOADED)
      return APP_ERROR;

   p = symbolStart;
   do {
      if ( (result = findSymbol(&p, paramName, &address, &memType)) == PARAM_FOUND) {
         if ( (result = cioctl(CONTROLLER, SET_MTYPE, &memType)) != NO_ERROR ) {
            printf("DetectorControllerApp::read cioctl returned %d\n", result);
            return APP_ERROR;
         }

         if ( (result = clseek(CONTROLLER, address, SEEK_SET )) < NO_ERROR ) {
            printf("DetectorControllerApp::read clseek returned %d\n", result);
            return APP_ERROR;
         }

         if ( (result = cread(CONTROLLER, value, 1)) != 1 ) {
            printf("DetectorControllerApp::read cread returned %d\n", result);
            return APP_ERROR;
         }

         return APP_OK;
      }
   } while ( result == _SYMBOL );

   // If we end up here, the parameter has not been found.
   return NOT_FOUND;
}

int DetectorControllerApp::getStat(void) {
   return appStat;
}

int DetectorControllerApp::setStat(int stat) {
   appStat = stat;

   return stat;
}

void *DetectorControllerApp::getApp(void) {
   return app;
}

