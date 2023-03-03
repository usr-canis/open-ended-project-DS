#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct node
{
    char song_name[100];
    struct node *prev;
    struct node *next;
} node;

node *head = NULL;
node *first = NULL;
node *tail = NULL;
node *temp = NULL;

node *current = NULL;

void addnode(char song[])
{

    node *ptr = (node *)malloc(sizeof(node));
    strcpy(ptr->song_name, song);

    ptr->next = NULL;
    ptr->prev = NULL;

    if (head == NULL)
    {
        head = ptr;
        first = head;
        tail = head;
    }
    else
    {
        temp = ptr;
        first->next = temp;
        temp->prev = first;
        first = temp;
        tail = temp;
    }
}

void playall()
{
    current = head;
    while (current != NULL)
    {
        char command[200];
        sprintf(command, "start /min %s", current->song_name);

        int status = system(command);

        if (status != 0)
        {
            printf("Error playing song: %s\n", current->song_name);
            break;
        }

        printf("Now playing: %s\n", current->song_name);

        current = current->next;
    }
}

void show_mp3_files()
{
    printf("avaialble songs here are\n");
    system("dir /b *.mp3");
}

int main()
{
    char song_name[100];
    int choice;

    while (1)
    {
        printf("enter your choice:\n1.display all songs\n2.add songs to playlist\n3.start playing\n4.play next song\n5.play previous song\n6.exit and kill player\n");
        scanf("%d/n", &choice);
        switch (choice)
        {
        case 1:
            show_mp3_files();
            break;

        case 2:
            printf("Enter song name to play: ");
            scanf("%s", song_name);
            addnode(song_name);
            break;

        case 3:
            play();
            break;

        case 6:
            system("taskkill /IM vlc.exe /F");
            exit(0);
        }
    }

    return 0;
}