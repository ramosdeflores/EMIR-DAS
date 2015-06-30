#include "SimpleMemoryManager.h"
#include "ace/Thread_Manager.h"

int thread_zero(SimpleMemoryManager& mng, int time) {
	void *buff;
	int index;


	printf("Function zero starts.\n");

	for (index = 0; index < 30; ++index) {
		sleep(time);
		mng.waitForFree();
	        buff = mng.getFreeBuffer(false);

		if ( buff == NULL )
			printf("\tthread_zero::NO FREE BUFFERS\n");
		else {
		        sprintf((char *)buff, "Prueba %d", index);
		        mng.putFullBuffer(buff);
                }
        }

	return 0;
}


int thread_one(SimpleMemoryManager *mng) {
	void *buff;
	int index;

	printf("Thread ONE starts.\n");
	while ( 1 ) {
		sleep(1);
		mng->waitForFull();
	        buff = mng->getFullBuffer(false);

	 	if ( buff == NULL )
			printf("\tthread_one::Got it wrong\n");
		else
		        printf("Got !%s! freeCount = %d, fullCount = %d\n", buff, mng->getFreeCount(), mng->getFullCount());

	        mng->putFreeBuffer(buff);
        }

	return 0;
}

int main() {

        int zero, one;

        SimpleMemoryManager mng(256, 60);

	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)thread_one, (void *)&mng);

	for ( zero = 0; zero < 100; ++zero) {
		thread_zero(mng,0);
		sleep(28);
	}

        exit(0);
}

