//
// Created by Tommy on 13/11/2023.
//

#include <stdio.h>
#include <windows.h>

DWORD WINAPI PlaySoundThread(LPVOID lpParam) {
    // Use SND_FILENAME to indicate that the first parameter is a file name
    // Use SND_ASYNC to play the sound asynchronously
    PlaySound(TEXT("C:\\Users\\Tommy\\CLionProjects\\untitled2\\for-elevator-jazz-music-124005.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    return 0;
}

int main() {
    // Create a thread to play the sound
    HANDLE soundThread = CreateThread(NULL, 0, PlaySoundThread, NULL, 0, NULL);

    /*ADD MAIN THREAD CODE HERE*/

    // Optional, wait for the user to close the program
    printf("Press Enter to exit.\n");
    getchar();

    // Optionally, wait for the sound thread to finish
    WaitForSingleObject(soundThread, INFINITE);

    // Close the handle to the thread
    CloseHandle(soundThread);

    return 0;
}