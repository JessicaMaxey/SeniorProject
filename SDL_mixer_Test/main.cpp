#include <SDL_mixer.h>
#include <stdio.h>
#include <SDL.h>

int main(int argc, char **argv)
{
	Mix_Music *music;
    Uint16 format;
    char *format_str="Unknown";
    

    //start SDL with audio support
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) 
    {
        printf("SDL_Init: %s\n", SDL_GetError());
        return -1;
    }   
    
    //load support for the MP3 music format
    int flags = MIX_INIT_MP3;
    int initted = Mix_Init(flags);
    
    if(initted&flags != flags)
    {
        printf("Mix_Init: Failed to init required mp3 support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());
    }
    
    //open audio 44.1KHz, signed 16bit, system byte order, stereo audio,
    //using 1024 byte chunks
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
    {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        return -1;
    }
    
    //get and print the audio format in use
/*    int numtimesopened, frequency, channels;
    numtimesopened = Mix_QuerySpec(&frequency, &format, &channels);
    
    //Get the actual audio format in use by the opened audio device
    if(!numtimesopened)
    {
        printf("Mix_QuerySpec: %s\n", Mix_GetError());
    }
    else 
    {
        switch(format) 
        {
            case AUDIO_U8: format_str="U8"; break;
            case AUDIO_S8: format_str="S8"; break;
            case AUDIO_U16LSB: format_str="U16LSB"; break;
            case AUDIO_S16LSB: format_str="S16LSB"; break;
            case AUDIO_U16MSB: format_str="U16MSB"; break;
            case AUDIO_S16MSB: format_str="S16MSB"; break;
        }
    }

    printf("opened=%d times  frequency=%dHz  format=%s  channels=%d",
        numtimesopened, frequency, format_str, channels);

*/
    
    //load music file to use
    music = Mix_LoadMUS("/home/jess/Music/warning.mp3");
    
    //print error if something goes wrong
    if(!music)
    {
        printf("Mix_LoadMUS(\"music.mp3\"): %s\n", Mix_GetError());
    }
    
    //make sure the music isn't muted
    Mix_VolumeMusic(128);
    
  
    //Play music file that is stored at the music pointer
    if(Mix_PlayMusic(music, -1) != 0)
    { 
        printf("Mix_PlayMusic: %s\n", Mix_GetError());
        return -1;
    }
    
    //small delay to get time to listen to the music
    //SDL_Delay(1000*20);
    
    //free music
    Mix_FreeMusic(music);
    
    //end of mix
    Mix_CloseAudio();
    Mix_Quit();
    
    //end of sdl library
    SDL_Quit();
    
	return 0;
}
