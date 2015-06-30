// DetectorControllerApp.h

#ifndef _DetectorControllerApp_h_
#define _DetectorControllerApp_h_

#include <string>
#include "Trace.h"

using namespace std;

#define _BEGIN          0
#define _DATA           1
#define _SYMBOL         2
#define _START          3
#define _END            4
#define _ENDOFFILE      5
#define	PARAM_FOUND		9999


class DetectorControllerApp {

public:

	DetectorControllerApp(const void *, const size_t);

	int stop(void);								// Stop execution of the application on the controller
	int exec(void);								// Execute the application.
	int startAqu(void);							// Execute the application.
	int load(void);								// Loads the application into the controller
	int run(void);								// Loads and executes the application
	int write(const char *, int);						// Write a value on the controller
	int read(const char *, int *);						// Read a value from the controller
	int getStat(void);							// Get the application status (loaded, running, ...)
	int setStat(int stat);
	void *getApp(void);

	Trace *trace_;

private:

	// Application info
	size_t size;
	void *app;
	char *symbolStart;
	unsigned int appStat;							// APP_IDLE, ...
	unsigned int cd;							// Controller descriptor
	unsigned int fg;							// Controller descriptor

	int parse(bool);
	int matchKeyWord(char *);
	int findKeyWord(char **, char *);
	int loadData(char **, char *, bool);
	int loadSymbol(char **, char *);
	int findSymbol(char **, const char *, unsigned int *, int *);
};

#endif
