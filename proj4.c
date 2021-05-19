#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void opt(int requests[]);
void lru(int requests[]);
void clk(int requests[]);
int searchFrames(int x, int frames[]);

int num_pages;
int num_frames;
int num_requests;

int main(int arg, char *argv[]){

    FILE *f = fopen(argv[1], "r");

    fscanf(f, "%d %d %d", &num_pages, &num_frames, &num_requests);

    int requests[num_requests];
    int i;
   
    for(i = 0; i < num_requests; i++){
     
        fscanf(f,"%d", &requests[i]);
    }

    fclose(f);


    if (strcmp(argv[2], "OPT") == 0)
	{
        printf("Number of pages: %d, Number of frames: %d, Number of requests: %d\n",num_pages,num_frames,num_requests);
        printf("OPT\n");
		printf("==============================================================\n");
		opt(requests);
	}
	else if (strcmp(argv[2], "LRU") == 0)
	{
        printf("Number of pages: %d, Number of frames: %d, Number of requests: %d\n",num_pages,num_frames,num_requests);
        printf("LRU\n");
		printf("==============================================================\n");
		lru(requests);
	}
	else if (strcmp(argv[2], "CLOCK") == 0)
	{
        printf("Number of pages: %d, Number of frames: %d, Number of requests: %d\n",num_pages,num_frames,num_requests);
        printf("ClOCK\n");
		printf("==============================================================\n");
		clk(requests);
	}

}

void opt(int requests[]){

    int pageFaults = 0;
    int frames[num_frames];
    int frame;
    int count = 0;
    int i, j, x, y;

    for(i = 0; i < num_frames; i++){
        frames[i] = -1;
    }

    for(i = 0; i < num_requests; i++){

        frame = searchFrames(requests[i],frames);

        if(frame != -1){
            printf("Page %d already in Frame %d\n", requests[i], frame);
        }
        else if(frames[count] == -1){
            frames[count] = requests[i];
            printf("Page %d loaded into Frame %d\n",requests[i],count);
            pageFaults++;
            count++;
        }
        else{

            int furthestPage[num_frames];
            for(y = 0; y < num_frames; y++){
                furthestPage[y] = -1;
            }

            for(x = 0; x < num_frames; x++){
                  
                for(j = num_requests; j >= i; j--){
                    
                    if(frames[x] == requests[j]){
                        furthestPage[x] = j;
                    }
                }
            }

            int replacement = furthestPage[0];
            int index = 0;

            for(j = 0; j < num_frames; j++){
                if(furthestPage[j] == -1){
                    index = j;
                    replacement = frames[j];
                    break;
                }
                else if(furthestPage[j] > replacement){
                    index = j;
                    replacement = frames[j];
                   
                }
            }
            int old = frames[index];
            frames[index] = requests[i];
            printf("Page %d unloaded from Frame %d, Page %d loaded into Frame %d\n", old,index,requests[i],index);
            pageFaults++;
            
        }
        
    }

    printf("%d page faults\n",pageFaults);

}

void lru(int requests[]){

    int pageFaults = 0;
    int frames[num_frames];
    int frame;
    int count = 0;
    int i, j, x, y;

    for(i = 0; i < num_frames; i++){
        frames[i] = -1;
    }

     for(i = 0; i < num_requests; i++){

        frame = searchFrames(requests[i],frames);

        if(frame != -1){
            printf("Page %d already in Frame %d\n", requests[i], frame);
        }
        else if(frames[count] == -1){
            frames[count] = requests[i];
            printf("Page %d loaded into Frame %d\n",requests[i],count);
            pageFaults++;
            count++;
        }
        else{

            int leastRecentPage[num_frames];
            for(y = 0; y < num_frames; y++){
                leastRecentPage[y] = -1;
            }

            for(j = 0; j < num_frames; j++){

                for(x = i-1; x >= 0; x--){
                    if(frames[j] == requests[x]){
                        leastRecentPage[j] = x;
                        break;
                    }
                }
            }

            int leastRecent = leastRecentPage[0];
            int index = 0;
            for(j = 0; j < num_frames; j++){

                if(leastRecentPage[j] < leastRecent){
                    leastRecent = leastRecentPage[j];
                    index = j;
                }
            }

            int old = frames[index];
            frames[index] = requests[i];
            printf("Page %d unloaded from Frame %d, Page %d loaded into Frame %d\n", old,index,requests[i],index);
            pageFaults++;
        }
    
    }

    printf("%d page faults\n",pageFaults);

}

void clk(int requests[]){

    int pageFaults = 0;
    int frames[num_frames];
    int frame;
    int count = 0;
    int i, j, x, y;

    int queue[num_frames];
    int queueCount = 0;

    for(i = 0; i < num_frames; i++){
        frames[i] = -1;
    }

     for(i = 0; i < num_requests; i++){

        frame = searchFrames(requests[i],frames);

        if(frame != -1){
            printf("Page %d already in Frame %d\n", requests[i], frame);
        }
        else if(frames[count] == -1){
            frames[count] = requests[i];
            printf("Page %d loaded into Frame %d\n",requests[i],count);
            queue[queueCount] = requests[i];
            queueCount++;
            pageFaults++;
            count++;
        }
        else{

            int old = queue[0];
            for(x = 0; x < queueCount-1; x++){
                queue[x] = queue[x+1];
            }
            queueCount--;

            int index;
            for(x = 0; x < num_frames; x++){
                if(frames[x] == old){
                    index = x;
                }
            }

            frames[index] = requests[i];
            queue[queueCount] = requests[i];
            queueCount++;
            printf("Page %d unloaded from Frame %d, Page %d loaded into Frame %d\n", old,index,requests[i],index);
            pageFaults++;
        }
    
    }

    printf("%d page faults\n",pageFaults);

}


int searchFrames(int x, int frames[]){
    int i;

    for(i = 0; i < num_frames; i++){

        if(frames[i] == x){
            return i;
        }
    }
    return -1;
}