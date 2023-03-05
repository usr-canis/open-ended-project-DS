#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>

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

DWORD WINAPI playall()
{
    if (head == NULL)
    {
        printf("\n=====================================================\n");
        printf("playlist is empty");
        printf("\n=====================================================\n");
        return 1;
    }

    current = head;
    while (current != NULL)
    {
        char command[200];
        sprintf(command, "vlc --one-instance --play-and-exit \"%s\"", current->song_name);

        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};
        BOOL success = CreateProcess(
            NULL,
            command,
            NULL,
            NULL,
            FALSE,
            CREATE_NEW_CONSOLE,
            NULL,
            NULL,
            &si,
            &pi);

        if (!success)
        {
            printf("\n=====================================================\n");
            printf("couldn't start the player");
            printf("\n=====================================================\n");
            break;
        }

        printf("\n=====================================================\n");
        printf("Now playing: %s", current->song_name);
        printf("\n=====================================================\n");

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        current = current->next;
    }

    return 0;
}

void show_mp3_files()
{
    system("dir /b *.mp3");
}

DWORD WINAPI playnext()
{
    if (current == NULL)
    {
        printf("\n=====================================================\n");
        printf("No song is playing");
        printf("\n=====================================================\n");
        return 1;
    }

    if (current->next == NULL)
    {
        printf("\nEnd of playlist reached. No more songs to play.\n");
        return 1;
    }

    current = current->next;
    char command[200];
    sprintf(command, "vlc --one-instance --play-and-exit  \"%s\"", current->song_name);

    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    BOOL success = CreateProcess(
        NULL,
        command,
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &pi);

    if (!success)
    {
        printf("\nCouldn't start the player\n");
        return 1;
    }

    printf("\n=====================================================\n");
    printf("Now playing: %s", current->song_name);
    printf("\n=====================================================\n");

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

DWORD WINAPI playprev()

{
    if (current == NULL)
    {
        printf("\n=====================================================\n");
        printf("No song is playing");
        printf("\n=====================================================\n");
        return 1;
    }

    if (current->prev == NULL)
    {
        printf("\n=====================================================\n");
        printf("\nBeginning of playlist reached. No previous songs to play.\n");
        printf("\n=====================================================\n");
        return 1;
    }

    current = current->prev;
    char command[200];
    sprintf(command, "vlc --one-instance --play-and-exit  \"%s\"", current->song_name);

    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    BOOL success = CreateProcess(
        NULL,
        command,
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &pi);

    if (!success)
    {
        printf("\n=====================================================\n");
        printf("Couldn't start the player");
        printf("\n=====================================================\n");
        return 1;
    }

    printf("\n=====================================================\n");
    printf("Now playing: %s", current->song_name);
    printf("\n=====================================================\n");

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

int main()
{
    char song_name[100];
    int choice;

    HANDLE playThread;
    HANDLE nextThread;
    HANDLE prevThread;

    DWORD threadId;

    while (1)
    {
        printf("\n\n=====================================================\n");
        printf("Console based Music Player\n");
        printf("=====================================================\n\n");

        printf("Please select an option:\n");
        printf("1. show all songs available\n");
        printf("2. Add a song to the playlist\n");
        printf("3. Start playing\n");
        printf("4. show songs in playlist\n");
        printf("5. Play previous song\n");
        printf("6. Pause\n");
        printf("7. Play next song\n");
        printf("8. Exit\n\n");

        printf("Enter your choice: ");
        scanf("%d/n", &choice);

        switch (choice)
        {
        case 1:
            printf("\n=====================================================\n");
            printf("showing all songs...\n");
            printf("=====================================================\n\n");
            show_mp3_files();
            break;

        case 2:
            printf("\n=====================================================\n");
            printf("Adding a song to the playlist...\n");
            printf("=====================================================\n\n");
            printf("Enter the name of the song to add: ");
            scanf("%s", song_name);
            addnode(song_name);
            break;

        case 3:
            printf("\n=====================================================\n");
            printf("Starting playback...\n");
            printf("=====================================================\n\n");
            playThread = CreateThread(NULL, 0, playall, 0, 0, &threadId);
            break;

        case 4:
            printf("\n=====================================================\n");
            printf("Showing the playlist...\n");
            printf("=====================================================\n\n");
            temp = head;
            while (temp != NULL)
            {
                printf("%s\n", temp->song_name);
                temp = temp->next;
            }
            break;

        case 5:
            prevThread = CreateThread(NULL, 0, playprev, 0, 0, &threadId);
            break;

        case 6:
            printf("\n=====================================================\n");
            printf("stop playback...\n");
            printf("=====================================================\n\n");
            TerminateThread(playThread, 0);
            TerminateThread(prevThread, 0);
            TerminateThread(nextThread, 0);
            system("taskkill /IM vlc.exe /F");
            break;

        case 7:
            nextThread = CreateThread(NULL, 0, playnext, 0, 0, &threadId);
            break;

        case 8:
            printf("\n=====================================================\n");
            printf("Exiting the music player...\n");
            printf("=====================================================\n\n");
            system("taskkill /IM vlc.exe /F");
            exit(0);
        }
    }

    return 0;
}