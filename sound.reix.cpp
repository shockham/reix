
class Sound{
	public:
		Sound();
		~Sound();
		void load_sound(const char* snd_path);
		void play();
		void stop();
	private:
		Uint8 *audio_pos; // global pointer to the audio buffer to be played
		Uint32 audio_len; // remaining length of the sample we have to play
		Uint32 wav_length; // length of our sample
		Uint8 *wav_buffer; // buffer containing our audio file
		SDL_AudioSpec wav_spec; // the specs of our piece of music
		static void audio_callback(void *userdata, Uint8 *stream, int len);
};

Sound::Sound(){

}

Sound::~Sound(){
	// shut everything down
	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);
}

void Sound::load_sound(const char* snd_path){
	if( SDL_LoadWAV(snd_path, &wav_spec, &wav_buffer, &wav_length) == NULL ){
		fprintf(stderr, "Couldn't load audio: %s\n", SDL_GetError());
	}
	// set the callback function
	wav_spec.callback = audio_callback;
	wav_spec.userdata = this;
	// set our global static variables
	audio_pos = wav_buffer; // copy sound buffer
	audio_len = wav_length; // copy file length

	/* Open the audio device */
	if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		// exit(-1);
	}
}

void Sound::play(){
	SDL_PauseAudio(0);

	// while ( audio_len > 0 ) {
	// 	SDL_Delay(100);
	// }
	// // shut everything down
	// SDL_CloseAudio();
	// SDL_FreeWAV(wav_buffer);
}

void Sound::stop(){
	SDL_PauseAudio(1);
}

void Sound::audio_callback(void *userdata, Uint8 *stream, int len){
	//restores older sdl behaviour, need to fix properly
	SDL_memset(stream, 0, len);

	Sound * user_sound = static_cast<Sound *>(userdata);

	if (user_sound->audio_len ==0) return;

	len = ( len > user_sound->audio_len ? user_sound->audio_len : len );
	//SDL_memcpy (stream, audio_pos, len); // simply copy from one buffer into the other
	SDL_MixAudio(stream, user_sound->audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
	user_sound->audio_pos += len;
	user_sound->audio_len -= len;
}