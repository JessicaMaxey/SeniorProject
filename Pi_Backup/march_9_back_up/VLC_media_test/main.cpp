#include <cstdio>
#include <ctime>
using std::clock_t;
using std::clock;

#include <unistd.h>
#include <signal.h>

#include <wiringPi.h>
#include <AudioAnalyzer.h>
#include <SoXDriver.h>
#include <Nano.h>
#include <API.h>
#include "../file_dialog_main/examplewindow.h"
#include <gtkmm/main.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
using std::thread;
using std::this_thread::sleep_for;
#include <chrono>
using std::chrono::milliseconds;
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

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
    EXIT,
};

struct Commands
{
  enum{NEW_SONG, START, PAUSE, STOP, SET_TIME} command_type;
  string song_name;
  float time;
};

struct LEDCommands
{
    enum{RAINBOW, SHIFT, FADE, RED, PURPLE, BLUE, TEAL, GREEN, YELLOW, STOP} command_type;
    int speed;

};

libvlc_instance_t *vlc_instance;
libvlc_media_player_t *media_player = NULL;
libvlc_media_list_player_t *media_list_player = NULL;
vector<libvlc_media_t *> media;
libvlc_media_list_t *media_list;
vector<string> database_info;
vector<string> file_locations;

volatile float sample_sleep_time = 0;
volatile float tempo = 1.0;
volatile char channels = 0;
void * void_ptr = 0;
int done = 0;
int playlist_count = -1;
int num_songs_in_playlist = 3;
queue<string> input;
queue<Commands> sox_command_queue;
queue<LEDCommands> led_command_queue;
bool playlist_next = false;
bool playlist_previous = false;
bool is_paused = false;
std::mutex accesslock;
int m_argc;
char **m_argv;

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
    db_handle = sqlite3_open("/home/pi/Documents/MusicDatabase/musicdb", &db);

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

void LEDThreadCommand()
{
    Nano n;


    while(1)
    {
        while(led_command_queue.size() == 0) sleep_for(std::chrono::milliseconds(10));

        accesslock.lock();
        LEDCommands cmd = led_command_queue.front();
        led_command_queue.pop();
        accesslock.unlock();

        cout << "after pop" << endl;

        while(1)
        {

            if(cmd.command_type == LEDCommands::RAINBOW)
            {
                n.LEDFadeRainbow(cmd.speed);
                cout << "rainbow" << endl;

                if(led_command_queue.size() != 0)
                {
                    cout << "size was not empty" << endl;
                    accesslock.lock();
                    cmd = led_command_queue.front();
                    led_command_queue.pop();
                    accesslock.unlock();
                }

                switch(cmd.command_type)
                {
                    n.LEDallOFF();
                    case Commands::STOP:
                        break;
                }
            }

            if(cmd.command_type == LEDCommands::SHIFT)
            {
                n.LEDColorShift(cmd.speed);
                cout << "shift" << endl;

                if(led_command_queue.size() != 0)
                {
                    accesslock.lock();
                    cmd = led_command_queue.front();
                    led_command_queue.pop();
                    accesslock.unlock();
                }

                switch(cmd.command_type)
                {
                    n.LEDallOFF();
                    case Commands::STOP:
                        break;
                }
            }
            if(cmd.command_type == LEDCommands::FADE)
            {
                n.LEDFadeONandOFF(cmd.speed);
                cout << "fade" << endl;

                if(led_command_queue.size() != 0)
                {
                    accesslock.lock();
                    cmd = led_command_queue.front();
                    led_command_queue.pop();
                    accesslock.unlock();
                }

                switch(cmd.command_type)
                {
                    n.LEDallOFF();
                    case Commands::STOP:
                        break;
                }
            }

            if(cmd.command_type == LEDCommands::RED)
            {
                n.LEDRed();
                cout << "fade" << endl;

                if(led_command_queue.size() != 0)
                {
                    accesslock.lock();
                    cmd = led_command_queue.front();
                    led_command_queue.pop();
                    accesslock.unlock();
                }

                switch(cmd.command_type)
                {
                    n.LEDallOFF();
                    case Commands::STOP:
                        break;
                }
            }

            if(cmd.command_type == LEDCommands::PURPLE)
            {
                n.LEDPurple();
                cout << "fade" << endl;

                if(led_command_queue.size() != 0)
                {
                    accesslock.lock();
                    cmd = led_command_queue.front();
                    led_command_queue.pop();
                    accesslock.unlock();
                }

                switch(cmd.command_type)
                {
                    n.LEDallOFF();
                    case Commands::STOP:
                        break;
                }
            }

            if(cmd.command_type == LEDCommands::BLUE)
            {
                n.LEDBlue();
                cout << "fade" << endl;

                if(led_command_queue.size() != 0)
                {
                    accesslock.lock();
                    cmd = led_command_queue.front();
                    led_command_queue.pop();
                    accesslock.unlock();
                }

                switch(cmd.command_type)
                {
                    n.LEDallOFF();
                    case Commands::STOP:
                        break;
                }
            }

            if(cmd.command_type == LEDCommands::TEAL)
            {
                n.LEDTeal();
                cout << "fade" << endl;

                if(led_command_queue.size() != 0)
                {
                    accesslock.lock();
                    cmd = led_command_queue.front();
                    led_command_queue.pop();
                    accesslock.unlock();
                }

                switch(cmd.command_type)
                {
                    n.LEDallOFF();
                    case Commands::STOP:
                        break;
                }
            }

            if(cmd.command_type == LEDCommands::GREEN)
            {
                n.LEDGreen();
                cout << "fade" << endl;

                if(led_command_queue.size() != 0)
                {
                    accesslock.lock();
                    cmd = led_command_queue.front();
                    led_command_queue.pop();
                    accesslock.unlock();
                }

                switch(cmd.command_type)
                {
                    n.LEDallOFF();
                    case Commands::STOP:
                        break;
                }
            }

            if(cmd.command_type == LEDCommands::YELLOW)
            {
                n.LEDYellow();
                cout << "fade" << endl;

                if(led_command_queue.size() != 0)
                {
                    accesslock.lock();
                    cmd = led_command_queue.front();
                    led_command_queue.pop();
                    accesslock.unlock();
                }

                switch(cmd.command_type)
                {
                    n.LEDallOFF();
                    case Commands::STOP:
                        break;
                }
            }

            //break;
        }
    }
}

void DoStuff()
{
    unsigned short count = 0;
    cout << "Do stuff" << endl;

    SoxDriver::init();
    API api;
    Nano n;

    while(1)
    {
        while(sox_command_queue.size() == 0) sleep_for(std::chrono::milliseconds(10));
        //sleep_for(milliseconds(1155));

        accesslock.lock();
        Commands cmd = sox_command_queue.front();
        sox_command_queue.pop();
        accesslock.unlock();

        if(cmd.command_type == Commands::NEW_SONG)
        {
            cout << "Received new media: " << cmd.song_name << endl;
            std::thread sox_stuff(SoxDriver::GenerateFreqFrames, cmd.song_name, tempo);
            //sleep_for(milliseconds(185));
            //sleep_for(milliseconds(1855));
            sleep_for(milliseconds(5));

            auto start = std::chrono::high_resolution_clock::now();

            //While the job has not finished
            while (SoxDriver::CheckRunning())
            {
                if (sox_command_queue.size() != 0)
                {
                    accesslock.lock();
                    cmd = sox_command_queue.front();
                    sox_command_queue.pop();
                    accesslock.unlock();

                    switch(cmd.command_type)
                    {
                        case Commands::STOP:
                            SoxDriver::EndJob();
                            while(SoxDriver::CheckRunning())
                            {
                                if (SoxDriver::CheckAvailable())
                                    SoxDriver::GetAvailableFrame();
                            }
                            break;
                        case Commands::PAUSE:
                            while(1)
                            {
                                if(sox_command_queue.size() != 0)
                                {
                                    accesslock.lock();
                                    cmd = sox_command_queue.front();
                                    sox_command_queue.pop();
                                    accesslock.unlock();

                                    if(cmd.command_type == Commands::START)
                                        break;

                                    else if (cmd.command_type == Commands::STOP)
                                    {
                                        SoxDriver::EndJob();
                                        while(SoxDriver::CheckAvailable())
                                        {
                                            SoxDriver::GetAvailableFrame();
                                        }

                                        break;
                                    }
                                }
                                sleep_for(std::chrono::microseconds(500));
                            }
                            break;
                        default:
                            break;

                    }
                }


                //cout << "is running" << endl;
                if (SoxDriver::CheckAvailable())
                {
                    //cout << "Got Frame" << endl
                    uint32_t microsecs = sample_sleep_time;
                    auto end = std::chrono::high_resolution_clock::now();

                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                    sleep_for(std::chrono::microseconds(microsecs) - duration);
                    start = std::chrono::high_resolution_clock::now();

                    auto buffer = SoxDriver::GetAvailableFrame();
                    auto ledVals = api.ReadBuff(buffer);
                    count++;
                    if (count % channels == 0)
                        n.LEDONandOFF(ledVals);
                }
                else
                {
                    sleep_for(std::chrono::microseconds(500));
                }
            }

            sox_stuff.join();

            //duration = (clock() - start) / (double)CLOCKS_PER_SEC;
            std::cout << "Ending sox_stuff";

            n.LEDallOFF();
        }
    }
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
    static std::thread doStuff(DoStuff);
    Commands sox_cmd;
    libvlc_media_track_t ** tracks = nullptr;
    uint32_t sample_per_sec = 0;

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
            cout << "Media Player Paused" << endl;
            accesslock.lock();
            sox_cmd.command_type = Commands::PAUSE;
            sox_command_queue.emplace(sox_cmd);
            accesslock.unlock();
            is_paused = true;
            break;
        case libvlc_MediaPlayerPlaying:
            if (is_paused == false)
            {
                cout << "Media Player Playing" << endl;
                GetMP3info(file_locations[playlist_count]);
                cout << file_locations[playlist_count] << endl;

                libvlc_media_t * instance_media = libvlc_media_list_item_at_index(media_list, playlist_count);
                libvlc_media_parse(instance_media);
                auto num_of_streams = libvlc_media_tracks_get(instance_media, &tracks);

                accesslock.lock();

                if(tracks)
                {
                    float num_frames_per_sec = 6.0;
                    //number of
                    sample_per_sec = tracks[0]->audio->i_rate;
                    channels = tracks[0]->audio->i_channels;
                    cout << "audio sample rate: " << sample_per_sec << endl;
                    cout << "audio channels: " << channels << endl;

                    tempo = (sample_per_sec) / (num_frames_per_sec * 4096);
                    sample_sleep_time = ((1.0 / (num_frames_per_sec)) * 1000000.0)/channels;

                    cout << "Calculated tempo: " << tempo << endl;
                    cout << "Updated sample_sleep_time: " << sample_sleep_time << endl;
                }

                sox_cmd.command_type = Commands::NEW_SONG;
                sox_cmd.song_name = file_locations[playlist_count];
                sox_cmd.time = 0;
                sox_command_queue.emplace(sox_cmd);
                accesslock.unlock();
            }
            else if (is_paused == true)
            {
                cout << "Media Player Playing" << endl;
                GetMP3info(file_locations[playlist_count]);
                cout << file_locations[playlist_count] << endl;

                accesslock.lock();
                sox_cmd.command_type = Commands::START;
                sox_cmd.song_name = file_locations[playlist_count];
                sox_cmd.time = 0;
                sox_command_queue.emplace(sox_cmd);
                accesslock.unlock();
                is_paused = false;
            }
            break;
        case libvlc_MediaPlayerStopped:
            cout << "Media Player Stopped" << endl;
            playlist_count = 0;
            accesslock.lock();
            sox_cmd.command_type = Commands::STOP;
            sox_cmd.time = 0;
            sox_cmd.song_name.clear();
            sox_command_queue.emplace(sox_cmd);
            accesslock.unlock();
            break;
        case libvlc_MediaPlayerPositionChanged:
            /*time = libvlc_media_player_get_time(media_player);
            printf("MediaPlayerTimeChanged %lld ms\n", (long long)time);
            cout << "MediaPlayerTimeChanged ";
            TimeFormat(time);
            cout << " \n";*/
            break;
        case libvlc_MediaListPlayerNextItemSet:
            cout << "Media Player Next Item Set" << endl;
            break;
        case libvlc_MediaPlayerEncounteredError:
            cout <<"Media Player Encountered Error" << endl;
            break;
        case libvlc_MediaPlayerMediaChanged:
            cout << "Media Player Changed" << endl;
            if ((playlist_count + 1) == libvlc_media_list_count(media_list) && playlist_previous != true)
                done = 1;
            else if (playlist_next == true)
            {
                playlist_count++;
                playlist_next = false;

                accesslock.lock();
                sox_cmd.command_type = Commands::STOP;
                sox_cmd.time = 0;
                sox_cmd.song_name.clear();
                sox_command_queue.emplace(sox_cmd);

                sox_cmd.command_type = Commands::NEW_SONG;
                sox_cmd.song_name = file_locations[playlist_count];
                sox_cmd.time = 0;
                //sox_command_queue.emplace(sox_cmd);
                accesslock.unlock();


            }
            else if (playlist_previous == true)
            {
                playlist_count--;
                playlist_previous = false;

                accesslock.lock();
                sox_cmd.command_type = Commands::STOP;
                sox_cmd.time = 0;
                sox_cmd.song_name.clear();
                sox_command_queue.emplace(sox_cmd);

                sox_cmd.command_type = Commands::NEW_SONG;
                sox_cmd.song_name = file_locations[playlist_count];
                sox_cmd.time = 0;
                //sox_command_queue.emplace(sox_cmd);
                accesslock.unlock();
            }
            else
            {

                playlist_count++;

                accesslock.lock();
                sox_cmd.command_type = Commands::STOP;
                sox_cmd.time = 0;
                sox_cmd.song_name.clear();
                sox_command_queue.emplace(sox_cmd);

                sox_cmd.command_type = Commands::NEW_SONG;
                sox_cmd.song_name = file_locations[playlist_count];
                sox_cmd.time = 0;
                //sox_command_queue.emplace(sox_cmd);
                accesslock.unlock();
            }
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

void InitPlaylist(string playlist_selection_num, bool playlist_or_mp3)
{
    //thread listen_thread(Listen);


    if (media_list_player != NULL)
    {
        //stop playing
        //libvlc_media_player_stop(media_player);
        libvlc_media_list_player_stop(media_list_player);

        media.clear();

        //free the media player
        //libvlc_media_player_release(media_player);
        libvlc_media_list_player_release(media_list_player);

        libvlc_release(vlc_instance);
    }

    string sql_str = "";

    if (playlist_or_mp3 == 0)
    {
            sql_str = "SELECT location FROM mp3 " \
                             "WHERE mp3_pk IN (SELECT mp3_fk FROM mp3_playlists " \
                                              "WHERE playlists_fk = " + playlist_selection_num + ");";
    }
    else
    {
            sql_str = "SELECT location FROM mp3 " \
                             "WHERE mp3_pk = " + playlist_selection_num + ";";
    }

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
    libvlc_event_attach(event_manager, libvlc_MediaListPlayerNextItemSet, HandleEvent, void_ptr);
    libvlc_event_attach(event_manager, libvlc_MediaPlayerEncounteredError, HandleEvent, void_ptr);
}

void PlayPlaylist(string playlist_selection_num, bool playlist_or_mp3)
{
    InitPlaylist(playlist_selection_num, playlist_or_mp3);
    playlist_count = -1;
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

void DeleteMP3(string mp3_to_delete)
{
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

void EditMP3()
{

}

void EditMP3OnPlaylist(string playlist_to_edit)
{
    DisplayMP3InPlaylist(playlist_to_edit);

    string mp3_selection = "";
    char* mp3_name;

    vector<string> mp3 = database_info;

    cout << "Select a mp3: ";
    cin >> mp3_selection;
    cout << endl;

    int mp3_selection_num = atoi(mp3_selection.c_str());

    cin.ignore();
    cout << "Enter new name for MP3: ";
    std::cin.getline(mp3_name, 256);
    string new_mp3_name = mp3_name;
    cout << "Hello, " << mp3_name << "!\n";

    string mp3_to_edit = "";

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

    mp3_to_edit = mp3[mp3_key];

    string sql_str = "UPDATE mp3 SET name = \"" + new_mp3_name + "\" WHERE mp3_pk = " + mp3_to_edit + ";";

    ReadFromDatabase(sql_str);
}

static void LEDMenu()
{
    static std::thread ledThread(LEDThreadCommand);
    LEDCommands led_cmd;
    Nano n;

    string selection = "";

    cout << "Please select one of the following options:" << endl;
    cout << "1. Fade Rainbow" << endl;
    cout << "2. Color Shift" << endl;
    cout << "3. Fade On and Off" << endl;
    cout << "4. Red" << endl;
    cout << "5. Purple" << endl;
    cout << "6. Blue" << endl;
    cout << "7. Teal" << endl;
    cout << "8. Green" << endl;
    cout << "9. Yellow" << endl;
    cout << "10. Stop" << endl;
    cout << "11. Exit" << endl;
    cout << "Selection: ";

    cin >> selection;
    cout << endl;

    int selection_num = atoi(selection.c_str());

    if(selection_num < 4)
    {

        string speed = "";

        cout << "Please select a speed 1-5:" << endl;
        cout << "Speed: ";

        cin >> speed;
        cout << endl;

        led_cmd.speed = atoi(speed.c_str());
    }

    switch(selection_num)
    {
        case 1:
            cout << "Fade Rainbow selected" << endl;
            accesslock.lock();
            led_cmd.command_type = LEDCommands::RAINBOW;
            led_command_queue.emplace(led_cmd);
            accesslock.unlock();
            break;
        case 2:
            cout << "Color Shift selected" << endl;
            accesslock.lock();
            led_cmd.command_type = LEDCommands::SHIFT;
            led_command_queue.emplace(led_cmd);
            accesslock.unlock();
            break;
        case 3:
            cout << "Fade On and Off selected" << endl;
            accesslock.lock();
            led_cmd.command_type = LEDCommands::FADE;
            led_command_queue.emplace(led_cmd);
            accesslock.unlock();
            break;
        case 4:
            cout << "Red selected" << endl;
            accesslock.lock();
            led_cmd.command_type = LEDCommands::RED;
            led_command_queue.emplace(led_cmd);
            accesslock.unlock();
            break;
        case 5:
            cout << "Purple selected" << endl;
            accesslock.lock();
            led_cmd.command_type = LEDCommands::PURPLE;
            led_command_queue.emplace(led_cmd);
            accesslock.unlock();
            break;
        case 6:
            cout << "Blue selected" << endl;
            accesslock.lock();
            led_cmd.command_type = LEDCommands::BLUE;
            led_command_queue.emplace(led_cmd);
            accesslock.unlock();
            break;
        case 7:
            cout << "Teal selected" << endl;
            accesslock.lock();
            led_cmd.command_type = LEDCommands::TEAL;
            led_command_queue.emplace(led_cmd);
            accesslock.unlock();
            break;
        case 8:
            cout << "Green selected" << endl;
            accesslock.lock();
            led_cmd.command_type = LEDCommands::GREEN;
            led_command_queue.emplace(led_cmd);
            accesslock.unlock();
            break;
        case 9:
            cout << "Yellow selected" << endl;
            accesslock.lock();
            led_cmd.command_type = LEDCommands::YELLOW;
            led_command_queue.emplace(led_cmd);
            accesslock.unlock();
            break;
        case 10:
            cout << "Turning leds off" << endl;
            accesslock.lock();
            led_cmd.command_type = LEDCommands::STOP;
            led_command_queue.emplace(led_cmd);
            accesslock.unlock();
            n.LEDallOFF();
            cout << "thread ended" << endl;
            break;
        default:
            break;
    }
}

void AddNewMP3InDatabase(string mp3_name, string new_mp3_location)
{
    string sql_str = "INSERT INTO mp3 (name, location) VALUES (\"" + mp3_name + "\" , \"" + new_mp3_location + "\");";

    FileRef file_ref (new_mp3_location.data());
    String title = file_ref.tag()->title();

    if(title == "")
    {
        file_ref.tag()->setTitle(mp3_name);
        file_ref.save();
    }

    ReadFromDatabase(sql_str);
}
void AddNewMP3()
{

    FILE *fp1, *fp2;
    char buffer[4096];
    long size;
    long size_remainder;
    size_t result = 0;
    string dest_name = "/home/pi/Music/";
    string mp3_name = "";
    string filename = "";

    Gtk::Main kit(m_argc, m_argv);

    ExampleWindow window;
    //Shows the window and returns when it is closed.
    Gtk::Main::run(window);

    filename = window.GetFilename();

    fp1 = fopen(filename.c_str(), "rb");

    size_t found = filename.find_last_of("/");
    mp3_name = filename.substr(found + 1);
    dest_name += filename.substr(found + 1);

    fp2 = fopen(dest_name.c_str(), "w+b");


    if(fp1 == NULL)
    {
        cout << "File error " << stderr << endl;
        exit(1);
    }

    fseek (fp1, 0, SEEK_END);
    size = (ftell (fp1)) / 4096;
    size_remainder = (ftell (fp1)) % 4096;
    rewind(fp1);

    if(buffer == NULL)
    {
        cout << "Memory error " << stderr << endl;
        exit(2);
    }

    for (int i = 0; i < size; i++)
    {
        result += fread(buffer, 1, 4096, fp1);
        fwrite(buffer, sizeof(char), 4096, fp2);
    }

    result += fread(buffer, 1, size_remainder, fp1);
    fwrite(buffer, sizeof(char), size_remainder, fp2);

    if(result != (ftell(fp1)))
    {
        cout << "Reading error " << stderr << endl;
        exit(3);
    }

    fclose(fp1);
    fclose(fp2);

    AddNewMP3InDatabase(mp3_name, dest_name);

}

void MP3Menu()
{
   string selection = "";

    cout << "Please select one of the following options:" << endl;
    cout << "1. MP3 options" << endl;
    cout << "2. Add new MP3s" << endl;
    cout << "3. Exit" << endl;
    cout << "Selection: ";

    cin >> selection;
    cout << endl;

    int selection_num = atoi(selection.c_str());

    switch(selection_num)
    {
        case 1:
            {

                selection = "";

                cout << "Select one of the following mp3 options:" << endl;
                cout << "1. Play a MP3" << endl;
                cout << "2. Edit a MP3" << endl;
                cout << "3. Delete a MP3" << endl;
                cout << "4. Exit" << endl;
                cout << "Selection: ";

                cin >> selection;
                cout << endl;

                DisplayMP3();

                vector<string> mp3 = database_info;

                string mp3_selection = "";

                cout << "Select a mp3: ";
                cin >> mp3_selection;
                cout << endl;

                int mp3_selection_num = atoi(mp3_selection.c_str());

                selection_num = 0;

                selection_num = atoi(selection.c_str());

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

                switch(selection_num)
                {
                    case 1:
                        PlayPlaylist(mp3[mp3_key], 1);
                        break;
                    case 2:
                        break;
                    case 3:
                        DeleteMP3(mp3[mp3_key]);
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
        case 2:
            AddNewMP3();
            break;
        case 3:
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


                int playlist_key = 0;
                int count = 1;
                for (int i = 0; i < (playlist.size()); i++)
                {
                    if (i % 2 == 0)
                    {
                        if (count == playlist_selection_num)
                        {
                            playlist_key = i;
                            break;
                        }
                        else
                            count++;
                    }
                }

                switch(selection_num)
                {
                    case 1:
                        PlayPlaylist(playlist[playlist_key], 0);
                        break;
                    case 2:
                        AddToPlaylist(playlist[playlist_key]);
                        break;
                    case 3:
                        EditMP3OnPlaylist(playlist[playlist_key]);
                        break;
                    case 4:
                        DeleteMP3FromPlaylist(playlist[playlist_key]);
                        break;
                    case 5:
                        DeletePlaylist(playlist[playlist_key]);
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
    cout << "3. LED menu" << endl;
    cout << "4. Exit" << endl;
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
            LEDMenu();
            break;
        case 4:
            break;
        default:
            break;
    }

}

int main(int argc, char **argv)
{
    m_argc = argc;
    m_argv = argv;

    while(!done)
    {
        string user_input;

        cin >> user_input;

        input.push(user_input);

        string command = "";
        const string commands [] = {"play", "pause", "next", "previous", "stop", "select", "menu", "exit"};
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
                if (media_list_player != NULL)
                {
                    libvlc_media_list_player_play(media_list_player);
                }
                break;
            case PAUSE:
                if (media_list_player != NULL)
                {
                    libvlc_media_list_player_pause(media_list_player);
                }
                break;
            case NEXT:
                if (media_list_player != NULL)
                {
                    playlist_next = true;
                    libvlc_media_list_player_next(media_list_player);
                }
                break;
            case PREVIOUS:
                if (media_list_player != NULL)
                {
                    playlist_previous = true;
                    libvlc_media_list_player_previous(media_list_player);
                }
                break;
            case STOP:
                if (media_list_player != NULL)
                {
                    libvlc_media_list_player_stop(media_list_player);
                }
                break;
            case SELECT:
                break;
            case MENU:
                Menu();
                break;
            case EXIT:
                done = 1;
                break;
            default:
                break;

        }


    }

    if (media_list_player != NULL)
    {
        //stop playing
        //libvlc_media_player_stop(media_player);
        libvlc_media_list_player_stop(media_list_player);

        //free the media player
        //libvlc_media_player_release(media_player);
        libvlc_media_list_player_release(media_list_player);

        libvlc_release(vlc_instance);
    }

    return 0;
}

