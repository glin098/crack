#include <stdio.h>
#include <unistd.h>
#include <crypt.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

char salt[3];
char* target;
char start_position = 'a';
char end_position = 'z';
//int num_threads;
//int keysize;

//pthread_mutex_t mutex;


void extractSalt(char* target, char* salt){ 
        int pos = 0;
        
        salt[0] = target[0];
        salt[1] = target[1];
        salt[2] = '\0';

        for (int i = 2; i < strlen(target); i++){
                target[pos++] = target[i];
        }

	target[pos] = '\0';
}


int crack(char* salt, char* word, char* target, int keysize){
        struct crypt_data d;
        d.initialized = 0;
	
	char *hash = crypt_r(word, salt, &d);
        
	extractSalt(hash, salt);

	if(hash == NULL){
		perror("crypt_r() failed\n");
		exit(-1);;
	}

	if(strcmp(hash, target) == 0){
		return 1;
        }
	else{
		return 0;
	}

	return crack(salt, word, target, keysize);
}

void iterate(char sequence[], int k, int keysize){
	int result = 0;

	if(k == 0){
		return;
	}

	for(char i = start_position; i <= end_position; i++){
		char newSequence[9] = {0};
		newSequence[9] = '\0';

		strcpy(newSequence, sequence);
		newSequence[strlen(sequence)] = i;

//		printf("%s, %s, %s, %d\n", salt, newSequence, target, keysize - k + 1);

//		iterate(newSequence, k-1, keysize);


		result = crack(salt, newSequence, target, keysize - k+1);

		if(result == 1){
                        printf("password found: %s\n", newSequence);
                        break;
                }
		iterate(newSequence, k-1, keysize);
	}

}


/*
int* iterate(char sequence[], int k, int keysize, char start_position, char end_position, struct crypt_data* d){
	
	int result = 0;
	
//	if(k == 0){
//		printf("sike\n");
//	}

	for(char i = start_position; i <= end_position; i++){
		//sequence[start_position] = i;
		
		char newSequence[9] = {0};
		newSequence[9] = '\0';
			
		strcpy(newSequence, sequence);	
		newSequence[strlen(sequence)] = i;
		
		//printf("%s\n", newSequence);
		printf("%s, %s, %s, %d\n", salt, newSequence, target, keysize - k + 1);		
		
		if(start_position == end_position){
			result = crack(salt, newSequence, target, keysize - k+1);
	
			if(result == 1){
                        	printf("password found: %s\n", newSequence);
                        	break;
                	}
		
		}
		else{
			iterate(newSequence, k-1, keysize, start_position + 3, end_position, d);
		}
		
	}
		
}
*/

/* with arg pointers?
void* iterate2(void* args){
	char sequence[] = {'\0'};
  	struct thread_args *argptr = args;
  	
        if(argptr->size == 0){
               // break;
        }

        for(char i = 'a'; i <= 'z'; i++){
                char newSequence[9] = {0};
                newSequence[9] = '\0';

                strcpy(newSequence, argptr->sequence);
                newSequence[strlen(argptr->sequence)] = i;

                //printf("%s, %s, %s, %d\n", salt, newSequence, target, keysize - k + 1);

//                iterate(newSequence, arg->size-1, arg->size);

                int result = crack(argptr->salt, newSequence, argptr->target, argptr->size - argptr->idx);
                if(result == 1){
                        printf("password found: %s\n", newSequence);
                        break;
                }

        }
                //iterate(newSequence, k-1, keysize);

}
*/

/*
void* thread_entry(void* args){
	struct crypt_data d;
	d.initialized = 0;

	char seq[] = {'\0'};
	
	int* result = iterate(seq, keysize, keysize, start_position, end_position, &d);

	return NULL;

}
*/


int main( int argc, char* argv[] ){

        if( argc < 3 ){
                printf("not enough arguments\n");
                printf("crack <threads> <keysize> <target>\n");
                return 0;
        }

        int num_threads = atoi(argv[1]);
        int keysize = atoi(argv[2]);
        target = argv[3];

        if(num_threads < 1){
                printf("number of threads needs to be positive number");
                return 0;
        }

        if(keysize < 1 || keysize > 8){
                printf("keysize needs to be between 1 and 8");
                return 0;
        }

	//char salt[3];	
	extractSalt(target,salt);
	
	/* //threading

	pthread_t thread[num_threads];
	pthread_mutex_init(&mutex, NULL);


	for(int i = 0; i < num_threads; i++){
		pthread_create(&thread[i], NULL, thread_entry, NULL);
	}

	for(int i = 0; i < num_threads; i++){
		pthread_join(thread[i], NULL);
	}	
	*/

	char seq[] = {'\0'};	
	iterate(seq, keysize, keysize);

        return 0;
}
