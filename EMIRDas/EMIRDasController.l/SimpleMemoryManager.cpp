#include "SimpleMemoryManager.h"
#include <iostream>

SimpleMemoryManager::SimpleMemoryManager(const size_t size, unsigned int poolSize) {
  int index, err_index;

  trace_ = new Trace("SimpleMemoryManager");

  pool_size = poolSize;

  fullQueue = (void **)malloc(pool_size * sizeof(void *));
  freeQueue = (void **)malloc(pool_size * sizeof(void *));

  for ( index = 0; index < pool_size; ++index) {
    fullQueue[index] = NULL;
    // If size is 0 then the freeQueue will loaded by the calling program.
    // If the buffers should be preserved after the object is deleted, then
    // theh queues should be refilled with NULLs.
    if ( size == 0 )
      freeQueue[index] = NULL;
    else {
      if ((freeQueue[index] = (void *)ACE_OS::malloc(size)) == NULL) {
        std::cout << "SimpleMemoryManager::SimpleMemoryManager() - ERROR allocating memory" << endl;
        for ( err_index = 0; err_index < index; ++err_index)
          free(freeQueue[index]);
        pool_size = -1;
        // Need to send error up
      }
    }
  }

  buffSize = size;
  freeHead = pool_size - 1;
  fullHead = 0;
  fullTail = 0;
  freeTail = 0;
  freeCount = pool_size;
  fullCount = 0;

  freeSema_ = new ACE_Semaphore(pool_size - 1);
  fullSema_ = new ACE_Semaphore(0);
}

SimpleMemoryManager::~SimpleMemoryManager() {
  int index;

  // If the buffers should be preserved after the object is deleted, then
  // the queues should be refilled with NULLs.
  if ( pool_size != -1 ){
    for ( index = freeHead; index < freeTail; ++index)
      if ( freeQueue[index] != NULL )
        free(freeQueue[index]);

    for ( index = fullHead; index < fullTail; ++index)
      if ( fullQueue[index] != NULL )
        free(fullQueue[index]);

    free(fullQueue);
    free(freeQueue);
  }

}


void *SimpleMemoryManager::getFreeBuffer(bool wait) {
  void *buffer = NULL;

  freeMutex_.acquire();

  if ( freeHead == freeTail ) {
    freeMutex_.release();
  } else {
    --freeCount;
    buffer = freeQueue[freeTail];
    freeQueue[freeTail] = NULL;
    freeTail = ( freeTail + 1 ) % pool_size;
    freeMutex_.release();
  }

  return buffer;

}

void *SimpleMemoryManager::getFullBuffer(bool wait) {
  void *buffer = NULL;

  if ( fullHead == fullTail ) {
    emptyMutex_.release();
    fullMutex_.release();
  } else {
    --fullCount;
    buffer = fullQueue[fullTail];
    fullQueue[fullTail] = NULL;
    fullTail = ( fullTail + 1 ) % pool_size;
    fullMutex_.release();
  }

  return buffer;
}

int SimpleMemoryManager::putFreeBuffer(void *buffer) {

  freeMutex_.acquire();

  if ( ! freeCount )
    emptyMutex_.acquire();

  ++freeCount;
  freeQueue[freeHead] = buffer;
  freeHead = (freeHead + 1) % pool_size;

  freeMutex_.release();

  freeSema_->release();

  return pool_size - freeCount;
}

int SimpleMemoryManager::putFullBuffer(void *buffer) {


  fullMutex_.acquire();

  ++fullCount;
  fullQueue[fullHead] = buffer;
  fullHead = (fullHead + 1) % pool_size;

  fullMutex_.release();

  fullSema_->release();

  return pool_size - freeCount;
}

int SimpleMemoryManager::getBufferCount() {

  return pool_size;
}

int SimpleMemoryManager::getFreeCount() {

  return freeCount;
}

int SimpleMemoryManager::getFullCount() {

  return fullCount;
}

int SimpleMemoryManager::getBufferSize() {

  return buffSize;
}

void SimpleMemoryManager::waitForFull() {

  fullSema_->acquire();
}

void SimpleMemoryManager::waitForFree() {

  freeSema_->acquire();
}

// Block until full queue is empty
void SimpleMemoryManager::waitForEmpty() {

  emptyMutex_.acquire();
  emptyMutex_.release();
}

// Load the memory manager with user provided buffers

void SimpleMemoryManager::loadMemoryManager(void *buffer) {
      getFreeBuffer(false);
      putFullBuffer(buffer);
      waitForFull();
      putFreeBuffer(getFullBuffer(true));
}
