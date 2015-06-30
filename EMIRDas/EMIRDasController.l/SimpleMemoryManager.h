// SimpleMemoryManager.h

#ifndef _SimpleMemoryManager_h_
#define _SimpleMemoryManager_h_

#include "ace/Mutex.h"
#include "ace/Semaphore.h"
#include "ace/Synch.h"

#include "Trace.h"

class SimpleMemoryManager {

public:

	SimpleMemoryManager(size_t size, unsigned int poolSize);

	~SimpleMemoryManager();

	void *getFreeBuffer(bool wait);		// Return an empty buffer
	void *getFullBuffer(bool wait);		// Return a loaded buffer
	int putFullBuffer(void *buffer);	// Queue a loaded buffer
	int putFreeBuffer(void *buffer);	// Queue an empty buffer
	int getBufferCount();			// Get the number of buffers to allocate
	int getBufferSize();			// Get the number of buffers to allocate
	int getFreeCount();			// Get the number of free buffers
	int getFullCount();			// Get the number of loaded buffers
	void waitForFull();			// Wait until all loaded buffers have been process
	void waitForFree();			// Wait until all loaded buffers have been process
	void waitForEmpty();
	void loadMemoryManager(void *buffer);


	Trace *trace_;

private:

	size_t buffSize;
        int pool_size;
	int freeCount;
	int fullCount;
        int freeHead, freeTail;
	int fullHead, fullTail;
	void **freeQueue, **fullQueue;

        ACE_Thread_Mutex fullMutex_;
        ACE_Thread_Mutex freeMutex_;
        ACE_Thread_Mutex emptyMutex_;
	ACE_Semaphore   *fullSema_;
	ACE_Semaphore   *freeSema_;

};

#endif
