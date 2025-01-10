#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char* p;
    int   arrLen;
} MyStr;


//Allocate a new string
MyStr allocateStr(int strLenBytes)
{
    MyStr str = {0};
    if(strLenBytes >= 0)
    {
        str.arrLen = strLenBytes+1;
        str.p = malloc(str.arrLen);
        if(str.p == NULL)
        {
            str.arrLen = 0;
        }
        else
        {
            str.p[str.arrLen-1] = '\0';
        }
    }

    return str;
}

void freeStr(MyStr* pStr)
{
    if(pStr->p != NULL && pStr->arrLen > 0)
    {
        free(pStr->p);
    }
    //Prevent another free of the same string
    pStr->p = NULL;
    pStr->arrLen = 0;
}

void copyStr(MyStr srcStr, MyStr dstStr)
{
    int n;
    if(srcStr.arrLen > dstStr.arrLen)
    {
        n = dstStr.arrLen;
    }
    else
    {
        n = srcStr.arrLen;
    }

    for(int i = 0 ; i < n ; i++)
    {
        dstStr.p[i] = srcStr.p[i];
    }
}

void reallocStr(MyStr* pStr, int newLen)
{
    //Allocate new array
    MyStr newStr = allocateStr(newLen);
    //Copy the data
    copyStr(*pStr, newStr);
    //Release the array and update
    freeStr(pStr);
    *pStr = newStr;
}

void addCharToStr(MyStr* pStr, char newChar)
{
    if(pStr->arrLen == 0)
    {
        reallocStr(pStr, 0);
    }

    int len = strlen(pStr->p);
    //If string is full - reallocate
    if(len == pStr->arrLen - 1)
    {
        reallocStr(pStr, len + 1);
    }
    pStr->p[len] = newChar;
    pStr->p[len+1] = '\0';
}

// MyStr ScanStr()
// {
//     char c;
//     MyStr str = {0};

//     do
//     {
//         c = getchar();
//         addCharToStr(&str, c);
//     } while(c != '\0');
// }

char* ScanStr()
{
    char c;
    char* str = NULL;
    int size = 0;
    
    do
    {
        str = realloc(str, size + 1);
        if(size == 0)
        {
            scanf(" %c", &c);
        }
        else
        {
            scanf("%c", &c);
        }
            
        // c = getchar();
        str[size] = c;
        size++;
    } while(c != 10);
    // printf("ScanStr: received string with length %d\n",size);
    str[size-1] = '\0';

    return str;
}

typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char* name;
    Song* songs;
    int songsNum;
} Playlist;

typedef struct
{
    Playlist* playlists;
    int listCnt;
} PlaylistCont;

void printPlaylistsMenu();
void watchPlaylists(PlaylistCont* plArr);
void addPlaylist(PlaylistCont* plArr);
void removePlaylists(PlaylistCont* plArr);
void addASong(Playlist* playList);
void play_playlist(Playlist *selected_playlist);
void sort_playlist(Playlist *selected_playlist);

void delete_song_from_playlist(Playlist *selected_playlist);
void SortPlayList(Playlist* playList, int sortType);


int playlistMenuPrinter(Playlist *selected_playlist, int playlist_index, int playlist_choice);
void show_playlist(Playlist *selected_playlist);
void freeASong(Song *selected_Song);

//some recommendations for functions, you may implement however you want
int main(){
        PlaylistCont plArr = {0};
        int option = -1;
        while(option != 4){
            printPlaylistsMenu();
            scanf(" %d", &option);
            switch(option){
                case 4:
                    printf("Goodbye!\n");
                    break;
                case 1:
                    watchPlaylists(&plArr);
                    break;
                case 2:
                    addPlaylist(&plArr);
                    break;
                case 3:
                    removePlaylists(&plArr);
                    break;
                default:
                    printf("Invalid option\n");    
                    break;
        }
    }
}

#if 1
void watchPlaylists(PlaylistCont* plArr) {
    printf("Choose a playlist: \n");
    if(plArr->listCnt == 0){
        printf("1. Back to main menu\n");
    }
    else{
        for (int i = 0; i < plArr->listCnt ; i++){
            printf(" %d. %s\n", i + 1, plArr->playlists[i].name); // Display each playlist
        }
        printf(" %d. Back to main menu\n", plArr->listCnt + 1);
        int playlist_index;
        scanf(" %d", &playlist_index);
        playlist_index--;
        if (playlist_index == plArr->listCnt){
            return;
        }else if(playlist_index < 0 || playlist_index > plArr->listCnt){
            printf("Invalid option.\n");
        } else{
            Playlist *selected_playlist = &(plArr->playlists[playlist_index]);
            int playlist_choice = 0;
            playlistMenuPrinter(selected_playlist, playlist_index, playlist_choice);
        }
    }

    return;
}
#endif

int playlistMenuPrinter(Playlist *selected_playlist, int playlist_index, int playlist_choice){
    do {
        printf("Playlist '%s':\n", selected_playlist->name);
        printf("1. Show Playlist\n");
        printf("2. Add Song\n");
        printf("3. Delete Song\n");
        printf("4. Sort\n");
        printf("5. Play\n");
        printf("6. Back to main menu\n");
        scanf(" %d", &playlist_choice);
        // getchar(); // Consume newline
        switch(playlist_choice){
            case 1:
                show_playlist(selected_playlist);
                break;
            case 2:
                addASong(selected_playlist);
                break;
            case 3:
                delete_song_from_playlist(selected_playlist);
                break;
            case 4:
                sort_playlist(selected_playlist);
                break;
            case 5:
                play_playlist(selected_playlist);
                break;
            default:
                printf("Invalid option.\n");
                break;
        }
    } while (playlist_choice != 6);
}

void delete_song_from_playlist(Playlist *selected_playlist){
    int song_index;
    printf("Enter the song number to delete: ");
    scanf(" %d", &song_index);
    freeASong(&selected_playlist->songs[song_index]);
    for(int i = song_index; i < selected_playlist->songsNum - 1; i++){
        selected_playlist->songs[i] = selected_playlist->songs[i + 1];
    }
    selected_playlist->songsNum--;
    selected_playlist = realloc(selected_playlist, selected_playlist->songsNum*sizeof(Song));
}

void freeASong(Song *selected_Song){
    free(selected_Song->artist);
    free(selected_Song->lyrics);
    free(selected_Song->title);
}

void sort_playlist(Playlist *selected_playlist){
    printf("Choose sorting option:\n");
    printf("1. Sort by year\n");
    printf("2. Sort by streams (ascending)\n");
    printf("3. Sort by streams (descending)\n");
    printf("4. Sort alphabetically\n");
    int sort_option;
    scanf(" %d", &sort_option);

    SortPlayList(selected_playlist, sort_option - 1);

    printf("sorted\n");
}

void show_playlist(Playlist *selected_playlist){
    printf("choose a song to play, or 0 to quit:\n");
    for(int i = 0; i < selected_playlist->songsNum; i++){
        printf("%d.\tTitle: %s\n", i+1,selected_playlist->songs[i].title);
        printf("\tArtist: %s\n", selected_playlist->songs[i].artist);
        printf("\tReleased: %d\n", selected_playlist->songs[i].year);
        printf("\tStreams: %d\n", selected_playlist->songs[i].streams);
    }
}

void play_playlist(Playlist *selected_playlist)
{
    for(int i = 0; i < selected_playlist->songsNum; i++){
        printf("Now playing %s:\n", selected_playlist->songs[i].title);
        printf("♪ %s ♪\n", selected_playlist->songs[i].lyrics);
        selected_playlist->songs[i].streams++;
    }
}


void addPlaylist(PlaylistCont* playlistsArr) {
    
    //Allocate the playlist array to a larger one
    playlistsArr->listCnt++;
    playlistsArr->playlists = realloc(playlistsArr->playlists,(playlistsArr->listCnt)*sizeof(Playlist));
    printf("Enter playlist's name:");
    //Fill the playlist needed data
    playlistsArr->playlists[playlistsArr->listCnt-1].name = ScanStr();
    playlistsArr->playlists[playlistsArr->listCnt-1].songs = NULL;
    playlistsArr->playlists[playlistsArr->listCnt-1].songsNum = 0;
    return;
}

void printPlaylistsMenu() {
    printf("Please Choose:\n"); 
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");   
}


void removePlaylists(PlaylistCont* plArr){
    printf("Choose a playlist: \n");
    for (int i = 0; i < plArr->listCnt ; i++){
        printf(" %d. %s\n", i + 1, plArr->playlists[i].name); // Display each playlist
    }
    printf(" %d. Back to main menu\n", plArr->listCnt + 1);
    
    int playlist_index;
    scanf(" %d", &playlist_index);
    if(playlist_index >= plArr->listCnt + 1)
    {
        return;
    }
    playlist_index--;
    //free all songs in the playlist.
    for(int j = 0; j < plArr->playlists->songsNum; j++){
        freeASong(&plArr->playlists[playlist_index].songs[j]);
    }
    //remove the playkist's entry from the array.
    free(plArr->playlists[playlist_index].name);
    for(int j = playlist_index ; j < plArr->listCnt - 1 ; j++){
        plArr->playlists[j] = plArr->playlists[j + 1];
    }
    plArr->listCnt--;
    plArr->playlists = realloc(plArr->playlists, plArr->listCnt*sizeof(Playlist));

}


void addASong(Playlist* playList)
{

    if(playList == NULL)
    {
        printf("ERROR! playlist is null in addASong\n");
        return;
    }
    playList->songsNum++;
    playList->songs = realloc(playList->songs,(playList->songsNum)*sizeof(Song));
    int year;
    printf("Enter song's details\n");
    printf("Title:\n");
    playList->songs[playList->songsNum - 1].title = ScanStr();
    printf("Artist:\n");
    playList->songs[playList->songsNum - 1].artist = ScanStr();
    printf("Year of release:\n");
    scanf(" %d", &year);
    playList->songs[playList->songsNum - 1].year = year;
    printf("Lyrics:\n");
    playList->songs[playList->songsNum - 1].lyrics = ScanStr();

    // add_song_to_playlist(selected_playlist, title, artist, year, lyrics);
    return;
}

void SwapTwoSongs(Song* songs, int i0, int i1) {
    Song temp = songs[i0];
    songs[i0] = songs[i1];
    songs[i1] = temp;
}

//Returns int according to comparison type:
//       * smaller than zero if song p0 is larger than p1
//       * zero if equal
//       * larger than 0 if p1 is larger than p0.
int CompareTwoSongs(Song* p0, Song* p1, int sortType) {
    switch(sortType)
    {
        //year
        case 0:
            return (p1->year - p0->year);
            break;
        case 1:
            return (p1->streams - p0->streams);
            break;
        case 2:
            return (p0->streams - p1->streams);
            break;
        default:
            return strcmp(p1->title, p0->title);
            break;
    }
    // return 0;
}

void SortPlayList(Playlist* playList, int sortType)
{
    //I do a bubble sort here, as suggested by my father
    for (int i = 0 ; i < playList->songsNum - 1 ; i++) {
        for (int j = i ; j < playList->songsNum - 1 ; j++){
            if(CompareTwoSongs(&playList->songs[j], &playList->songs[j+1], sortType) < 0)
            {
                SwapTwoSongs(playList->songs, j, j +1);
            }
        }
    }
}
