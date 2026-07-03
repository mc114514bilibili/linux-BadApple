#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<sys/wait.h>

void resetCursor()
{
    printf("\033[H");
    fflush(stdout);
}

void playWavAsync(const char *wavPath)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execlp("paplay", "paplay", wavPath, NULL);
        perror("paplay not found, install pulseaudio / pulseaudio-utils");
        _exit(1);
    }
}

void sleepMs(long ms)
{
    clock_t start = clock();
    long target = 1L * ms * CLOCKS_PER_SEC / 1000;
    while (clock() - start < target);
}

int main()
{
    int i = 0;
    const int FRAME_DELAY = 33;
    FILE *fp;
    char buf[1920], seat[]="out/BA (0000).txt", ai[5];	

    printf("-----Bad Apple ASCII art player-----\nPress Enter to play.\n");
    getchar();
    system("clear");

    playWavAsync("BadApple.wav");

    while(i <= 6570)
    {		
        strcpy(seat, "out/BA (");
        sprintf(ai, "%d", i);
        strcat(seat, ai);
        strcat(seat, ").txt");

        fp = fopen(seat, "r");
        if (fp == NULL)
        {
            perror("Frame file missing");
            i++;
            sleepMs(FRAME_DELAY);
            continue;
        }
        fread(buf, sizeof(buf)-1, 1, fp);
        buf[1919] = '\0';
        fclose(fp);

        printf("%sFrame:%d", buf, i);
        resetCursor();

        i++;
        sleepMs(FRAME_DELAY);
    }

    system("clear");
    printf("-----Bad Apple ASCII art player-----\nThanks for watching!\n\n");
    printf("Press Enter to Exit.\n");
    getchar();
    return 0;
}