#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct Song
{
    char *filename;
    struct Song *next;
} Song;

void play_song(char *filename)
{
    char command[1024];
    sprintf(command, " start /min %s", filename);
    system(command);
}

void play_songs(Song *head)
{
    Song *current = head;
    while (current != NULL)
    {
        play_song(current->filename);
        current = current->next;
    }
}

int main()
{
    Song *song1 = malloc(sizeof(Song));
    song1->filename = "a.mp3";
    Song *song2 = malloc(sizeof(Song));
    song2->filename = "b.mp3";

    song1->next = song2;
    play_songs(song1);

    return 0;
}
