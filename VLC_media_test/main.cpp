#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
using std::thread;
#include <chrono>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;


#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include <string>
using std::string;

#include <string.h>

#include <vector>
using std::vector;

#include <queue>
using std::queue;

#include <tag.h>
using namespace TagLib;

#include <taglib/taglib_export.h>
#include <tstring.h>
#include <fileref.h>
 
#include <vlc.h>


enum command_vals 
{
    PLAY,
    PAUSE,
    NEXT,
    PREVIOUS,
    STOP,
    SELECT,
    MENU,
};

libvlc_instance_t *vlc_instance;
libvlc_media_player_t *media_player = NULL;
libvlc_media_list_player_t *media_list_player = NULL;
vector<libvlc_media_t *> media;
libvlc_media_list_t *media_list;
vector<string> database_info;
vector<string> file_locations;


const int PLAYLIST_NUM = 5;
void * void_ptr = 0;
int done = 0;
int playlist_count = -1;
int num_songs_in_playlist = 3;
queue<string> input;
bool playlist_next = false;
bool playlist_previous = false;


static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    if(!done)
    {
       int i;

       
        for(i = 0; i < argc; i++)
        {
          //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
          
          database_info.push_back(argv[i]);
        }
        
        printf("\n");
        
    }
    return 0;
}

int ReadFromDatabase(string sql_str)
{
    database_info.clear();
        
    sqlite3 *db;

    char *zErrMsg = 0;
    int db_handle;

    const char *sql;
    
    //Open database
    db_handle = sqlite3_open("/home/jess/Documents/MusicDatabase/musicdb", &db);

    if( db_handle )
    {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }      
    
    sql = sql_str.c_str();
    
    //Execute SQL statement
    db_handle = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    if(db_handle != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Database queried successfully\n");
    }
    
    sqlite3_close(db);
    return 0;
}


void TimeFormat(libvlc_time_t time)
{
    string formated = "";
    
    time = time / 1000;
    
    long long seconds = (long long)time % 60; 
    long long minutes = ((long long)time / 60) % 60; 
    long long hours = ((long long)time / 60 / 60) % 24; 

    //cout << (long long)hours << ":" << (long long)minutes << ":" << (long long)seconds;

}

void GetMP3info (string file_path)
{
    FileRef file_ref (file_path.data());
    String artist = file_ref.tag()->artist();
    String title = file_ref.tag()->title();

    int time = file_ref.audioProperties()->length();
    
    int seconds = time % 60; 
    int minutes = (time / 60) % 60; 
    int hours = (time / 60 / 60) % 24; 
        
    file_ref.save();
    
    cout << artist << " " << title << " " << hours << ":" << minutes << ":" << seconds << endl;
    
}

static void HandleEvent(const libvlc_event_t * p_event, void*)
{
    libvlc_time_t time;
    
    switch(p_event->type)
    {
        case libvlc_MediaPlayerEndReached:
            if ((playlist_count + 1) == libvlc_media_list_count(media_list))
            {
                //stop playing
                //libvlc_media_player_stop(media_player);
                libvlc_media_list_player_stop(media_list_player);

                //free the media player
                //libvlc_media_player_release(media_player);
                libvlc_media_list_player_release(media_list_player);

                libvlc_release(vlc_instance);
            }
                done = 1;
            break;
        case libvlc_MediaPlayerPaused:
            break;
        case libvlc_MediaPlayerPlaying:
            GetMP3info(file_locations[playlist_count]);
            break;
        case libvlc_MediaPlayerStopped:
            playlist_count = 0;
            break;
        case libvlc_MediaPlayerPositionChanged:
            /*time = libvlc_media_player_get_time(media_player);
            printf("MediaPlayerTimeChanged %lld ms\n", (long long)time);
            cout << "MediaPlayerTimeChanged ";
            TimeFormat(time);
            cout << " \n";*/
            break;
        case libvlc_MediaPlayerMediaChanged:
            if ((playlist_count + 1) == libvlc_media_list_count(media_list) && playlist_previous != true)
                done = 1;
            else if (playlist_next == true)
            {
                playlist_count++;
                playlist_next = false;
            }
            else if (playlist_previous == true)
            {
                playlist_count--;
                playlist_previous = false;
            }
            else
                playlist_count++;
            break;
    }
}

void Skip ()
{
    libvlc_time_t time = libvlc_media_player_get_time(media_player);
    libvlc_media_player_set_time(media_player, float(time + 10000.0));
}

void Shuffle()
{
    
}

void InitPlaylist(string playlist_selection_num)
{

}

void PlayPlaylist()
{
    libvlc_media_list_player_play(media_list_player);
}

void DisplayMP3InPlaylist(string playlist_pk)
{
    string sql_str = "SELECT mp3_pk, name FROM mp3 " \
                     "WHERE mp3_pk IN (SELECT mp3_fk FROM mp3_playlists " \
                                      "WHERE playlists_fk = " + playlist_pk + ");";
    int count = 1;
    
    ReadFromDatabase(sql_str);
    
    for (int i = 0; i < database_info.size(); i++)
    {
        if (i % 2)
        {
            cout << count << "." << database_info[i] << endl;
            count++;
        }
    }
}

void DisplayMP3()
{
    string sql_str = "SELECT * FROM mp3;";
    int count = 1;
    
    ReadFromDatabase(sql_str);
    
    for (int i = 0; i < database_info.size(); i++)
    {
        if (i % 3 == 1)
        {
            cout << count << "." << database_info[i] << endl;
            count++;
        }
    }
}

void DisplayPlaylist()
{
    string sql_str = "SELECT * FROM playlists;";
    int count = 1;
    
    ReadFromDatabase(sql_str);
    
    for (int i = 0; i < (database_info.size()); i++)
    {
        if (i % 2)
        {
            cout << count << "." << database_info[i] << endl;
            count++;
        }
    }
    
}

void DeletePlaylist(string playlist_to_delete)
{
    string sql_str = "DELETE FROM playlists WHERE playlists_pk = " + playlist_to_delete + ";";
    
    ReadFromDatabase(sql_str);
    
    sql_str = "";
    
    sql_str = "DELETE FROM mp3_playlists WHERE playlists_fk = " + playlist_to_delete + ";";
    
    ReadFromDatabase(sql_str);
}

void DeleteMP3FromPlaylist(string playlist_to_delete)
{
    DisplayMP3InPlaylist(playlist_to_delete);
    
    string mp3_selection = "";
    
    vector<string> mp3 = database_info;

    cout << "Select a mp3: ";
    cin >> mp3_selection;
    cout << endl;

    int mp3_selection_num = atoi(mp3_selection.c_str());
    
    string mp3_to_delete = "";
    
    int mp3_key = 0;
    int count = 1;
    for (int i = 0; i < (mp3.size()); i++)
    {
        if (i % 2 == 0)
        {
            if (count == mp3_selection_num)
            {
                mp3_key = i;
                break;
            }
            else 
                count++;
        }
    }
    
    mp3_to_delete = mp3[mp3_key];
    
    string sql_str = "DELETE FROM mp3_playlists WHERE mp3_fk = " + mp3_to_delete + " AND playlists_fk = " + playlist_to_delete + ";";
    
    ReadFromDatabase(sql_str);
}

void DeleteMP3()
{
    DisplayMP3();
    
    string mp3_selection = "";
    
    vector<string> mp3 = database_info;

    cout << "Select a mp3: ";
    cin >> mp3_selection;
    cout << endl;

    string mp3_to_delete = "";
    
    int mp3_selection_num = atoi(mp3_selection.c_str());
    
    int mp3_key = 0;
    int count = 1;
    for (int i = 0; i < (mp3.size()); i++)
    {
        if (i % 3 == 0)
        {
            if (count == mp3_selection_num)
            {
                mp3_key = i;
                break;
            }
            else 
                count++;
        }
    }
    
    mp3_to_delete = mp3[mp3_key];
    
    string sql_str = "DELETE FROM mp3 WHERE mp3_pk = " + mp3_to_delete + ";";
    
    ReadFromDatabase(sql_str);
    
    sql_str = "";
    
    sql_str = "DELETE FROM mp3_playlists WHERE mp3_fk = " + mp3_to_delete + ";";
    
    ReadFromDatabase(sql_str);
}

void AddToPlaylist(string playlist_adding_to)
{
    DisplayMP3();
    string mp3_selection = "";
    
    vector<string> mp3 = database_info;

    cout << "Select a mp3: ";
    cin >> mp3_selection;
    cout << endl;

    int mp3_selection_num = atoi(mp3_selection.c_str());
    
    string mp3_to_add = "";
    
    int mp3_key = 0;
    int count = 1;
    for (int i = 0; i < (mp3.size()); i++)
    {
        if (i % 3 == 0)
        {
            if (count == mp3_selection_num)
            {
                mp3_key = i;
                break;
            }
            else 
                count++;
        }
    }
    
    mp3_to_add = mp3[mp3_key];
    
    string sql_str = "INSERT INTO mp3_playlists (mp3_fk, playlists_fk) VALUES (" + mp3_to_add + " ," + playlist_adding_to + ");";
    
    ReadFromDatabase(sql_str);
}

void EditPlaylist()
{
    
}

void MP3Menu()
{
    string selection = "";
    
    cout << "Please select one of the following options:" << endl;
    cout << "1. MP3 options" << endl;
    //cout << "2. Add new MP3s"
    cout << "3. Exit" << endl;
    cout << "Selection: ";
    
    cin >> selection;
    cout << endl;
    
    int selection_num = atoi(selection.c_str());
    
    switch(selection_num)
    {
        case 1:
            {
                /*DisplayMP3();
                vector<string> mp3 = database_info;
                
                string mp3_selection = "";

                cout << "Select a mp3: ";
                cin >> mp3_selection;
                cout << endl;
                
                int mp3_selection_num = atoi(mp3_selection.c_str());
                            
                selection = "";*/

                cout << "Select one of the following mp3 options:" << endl;
                cout << "1. Play a MP3" << endl;
                cout << "2. Edit a MP3" << endl;
                cout << "3. Delete a MP3" << endl;
                cout << "4. Exit" << endl;
                cout << "Selection: ";

                cin >> selection;
                cout << endl;
                
                selection_num = 0;
                
                selection_num = atoi(selection.c_str());
                
                switch(selection_num)
                {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        DeleteMP3();
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    default:
                        break;
                    
                }
            }
            break;
        default:
            break;
    }
}

void PlaylistMenu()
{
    string selection = "";
    
    cout << "Please select one of the following options:" << endl;
    cout << "1. Display playlists" << endl;
    //cout << "2. Create new playlist" << endl;
    cout << "3. Exit" << endl;
    cout << "Selection: ";

    cin >> selection;
    cout << endl;
    
    int selection_num = atoi(selection.c_str());
    
    switch(selection_num)
    {
        case 1:
            {
                //Displays the playlists in the database
                DisplayPlaylist();
                vector<string> playlist = database_info;
                
                string playlist_selection = "";

                cout << "Select a playlist: ";
                cin >> playlist_selection;
                cout << endl;
                
                int playlist_selection_num = atoi(playlist_selection.c_str());
                
                
                selection = "";

                cout << "Select one of the following playlist options:" << endl;
                cout << "1. Play playlist" << endl;
                cout << "2. Add MP3s to playlist" << endl;
                cout << "3. Edit MP3s on playlist" << endl;
                cout << "4. Delete MP3s on playlist" << endl;
                cout << "5. Delete this playlist" << endl;
                cout << "6. Exit" << endl;
                cout << "Selection: ";

                cin >> selection;
                cout << endl;
                
                selection_num = 0;
                
                selection_num = atoi(selection.c_str());
                
                switch(selection_num)
                {
                    case 1:
                        InitPlaylist(playlist[playlist_selection_num - 1]);
                        PlayPlaylist();
                        break;
                    case 2:
                        AddToPlaylist(playlist[playlist_selection_num - 1]);
                        break;
                    case 3:
                        break;
                    case 4:
                        DeleteMP3FromPlaylist(playlist[playlist_selection_num - 1]);
                        break;
                    case 5:
                        DeletePlaylist(playlist[playlist_selection_num - 1]);
                        break;
                    default:
                        break;
                    
                }
            }
            break;
        case 2:
            break;
        default:
            break;
    }
}

void Menu()
{
    string selection = "";
    
    cout << "Please select one of the following options:" << endl;
    cout << "1. Playlist menu" << endl;
    cout << "2. MP3 menu" << endl;
    cout << "3. Exit" << endl;
    cout << "Selection: ";
    

    cin >> selection;
    cout << endl;
    
    int selection_num = atoi(selection.c_str());
    
    switch(selection_num)
    {
        case 1:
            PlaylistMenu();
            break;
        case 2:
            MP3Menu();
            break;
        case 3:
            break;
        default:
            break;
    }
    
}

int main(int argc, char **argv)
{
    //thread listen_thread(Listen);
    
    string sql_str = "SELECT location FROM mp3 " \
                     "WHERE mp3_pk IN (SELECT mp3_fk FROM mp3_playlists " \
                                      "WHERE playlists_fk = " + std::to_string(PLAYLIST_NUM) + ");";
    ReadFromDatabase(sql_str);
    file_locations = database_info;
        
    //load the vlc engine
    vlc_instance = libvlc_new(0, NULL);

    
    //create new item
    for (unsigned int i = 0; i < file_locations.size(); i++)
    {
        media.push_back(libvlc_media_new_path(vlc_instance, file_locations[i].data()));
    }
    
    //create a media play playing environment
    media_player = libvlc_media_player_new_from_media(media[0]);
    
    //create a media list and media play list
    media_list = libvlc_media_list_new(vlc_instance);
    media_list_player = libvlc_media_list_player_new(vlc_instance);
    libvlc_media_list_player_set_media_list(media_list_player, media_list);
    libvlc_media_list_player_set_media_player(media_list_player, media_player);
    

    for (unsigned int i = 0; i < file_locations.size(); i++)
    {
        if(media[i])
        {
            libvlc_media_list_lock(media_list);
            libvlc_media_list_add_media(media_list, media[i]);
            libvlc_media_list_unlock(media_list);
        }
        
        //no need to keep the media now
        libvlc_media_release(media[i]);
    }
    

    
    
    libvlc_event_manager_t* event_manager = libvlc_media_player_event_manager(media_player);
    libvlc_event_attach(event_manager, libvlc_MediaPlayerEndReached, HandleEvent, void_ptr);
    libvlc_event_attach(event_manager, libvlc_MediaPlayerPaused, HandleEvent, void_ptr);
    libvlc_event_attach(event_manager, libvlc_MediaPlayerPlaying, HandleEvent, void_ptr);
    libvlc_event_attach(event_manager, libvlc_MediaPlayerPositionChanged, HandleEvent, void_ptr);
    libvlc_event_attach(event_manager, libvlc_MediaPlayerMediaChanged, HandleEvent, void_ptr);
    libvlc_event_attach(event_manager, libvlc_MediaPlayerStopped, HandleEvent, void_ptr);
    libvlc_event_attach(event_manager, libvlc_MediaPlayerForward, HandleEvent, void_ptr);
    
    //play the media_player
    //libvlc_media_player_play(media_player);
    //libvlc_media_list_player_play(media_list_player);

    while(!done)
    {
        string user_input;

        cin >> user_input;
        
        input.push(user_input);
        
        string command = "";
        const string commands [] = {"play", "pause", "next", "previous", "stop", "select", "menu"};
        int command_num = 0;
        

        while(input.size() == 0)
        {
            sleep(0);
        }
        
        command = input.front();
        input.pop();
    
        for (int i = 0; i < (sizeof(commands)/ sizeof(string)); i++)
        {
            if(!done)
            {
                command_num = i;
                
                if (command == commands[i])
                    break;
            }
        }
    
        switch (command_num)
        {
            case PLAY:
                PlayPlaylist();
                break;
            case PAUSE:
                libvlc_media_list_player_pause(media_list_player);
                break;
            case NEXT:
                playlist_next = true;
                libvlc_media_list_player_next(media_list_player);
                break;
            case PREVIOUS:
                playlist_previous = true;
                libvlc_media_list_player_previous(media_list_player);
                break;
            case STOP:
                libvlc_media_list_player_stop(media_list_player);
                break;
            case SELECT:
                break;
            case MENU:
                Menu();
                break;
            default:
                break;
            
        }
        

    }
    
    //stop playing
    //libvlc_media_player_stop(media_player);
    libvlc_media_list_player_stop(media_list_player);
    
    //free the media player
    //libvlc_media_player_release(media_player);
    libvlc_media_list_player_release(media_list_player);
    
    libvlc_release(vlc_instance);
    
    return 0;
}

