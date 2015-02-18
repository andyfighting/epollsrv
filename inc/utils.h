#ifndef _UTILS_H_
#define _UTILS_H_

#define BUF_SIZE 512
#define RET_CHECK(entry) \
			do {\
				if(entry) {\
					perror("entry");\
					exit(-1);\
				}\
			} while(0)

#define RSLT_CHECK(result, entry) \
			do {\
				if((result = entry) < 0) {\
					perror("entry");\
					exit(-1);\
				}\
			} while(0)

#define FREE(x) do{if(x) free(x);}while(0)

#endif
