#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "openal.h"
#include "sound.h"

#include "music.h"

static pthread_mutex_t cs_mutex = PTHREAD_MUTEX_INITIALIZER;

static bool _stream(music_context_t* context, sound_buffer_t buffer) {

    OggVorbis_File* poggStream = &context->oggStream;
    vorbis_info* vorbisInfo = context->vorbisInfo;
    ALenum format = context->format;

    char data[OGG_READ_BLOCK_SIZE];
    int  size = 0;
    int  section;
    int  result;

    while (size < OGG_READ_BLOCK_SIZE) {
        result = ov_read(poggStream, data + size, OGG_READ_BLOCK_SIZE - size, 0, 2, 1, &section);
        if (result > 0) size += result;
        else if (result < 0) return false;
        else if (ov_raw_seek(poggStream, 0) < 0) return false;
    }

    alBufferData(buffer, format, data, size, vorbisInfo->rate);

    return true;

}

static void _cleanup(music_context_t* context) {
    alSourceStop(context->source);
    alDeleteSources(1, &context->source);
    alDeleteBuffers(NUM_BUFFERS, context->buffers);
    ov_clear(&context->oggStream);
}

static bool _update(music_context_t* context) {

    int processed;
    bool active = true;
    alGetSourcei(context->source, AL_BUFFERS_PROCESSED, &processed);

    if (processed >= 0)
        while (processed--) {
            sound_buffer_t buffer;
            alSourceUnqueueBuffers(context->source, 1, &buffer);
            active = _stream(context, buffer);
            alSourceQueueBuffers(context->source, 1, &buffer);
        }

    return active;

}

static bool _playing(music_context_t* context) {
    ALenum state;
    alGetSourcei(context->source, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING || state == AL_PAUSED);
}

static bool _playback(music_context_t* context) {

    if (_playing(context)) return true;

    for (int i = 0; i < NUM_BUFFERS; i++) {
        if (!_stream(context, context->buffers[i])) return false;
    }

    alSourceQueueBuffers(context->source, NUM_BUFFERS, context->buffers);
    alSourcePlay(context->source);

    return true;

}

static void* _play_music_loop(void* _context) {

    music_context_t* context = (music_context_t*) _context;

    bool stop_requested = false;

    while (!stop_requested && !context->error && _update(context)) {
        if (!_playing(context))
            if (!_playback(context))
                context->error = true;
        pthread_mutex_lock(&cs_mutex);
        if (context->stopped) stop_requested = true;
        pthread_mutex_unlock(&cs_mutex);
    }

    _cleanup(context);

    return NULL;

}


static size_t _vorbis_read(void *ptr, size_t byteSize, size_t sizeToRead, void *_data_stream) {

	data_stream_t* data_stream = (data_stream_t*)_data_stream;

	size_t read_bytes;
	size_t can_read_bytes = data_stream->data_len - data_stream->data_pos;
	if (sizeToRead * byteSize <= can_read_bytes)
		read_bytes = sizeToRead * byteSize;
	else
		read_bytes = can_read_bytes;

	if (read_bytes > 0) {
		memcpy(ptr, data_stream->data + data_stream->data_pos, read_bytes);
		data_stream->data_pos += read_bytes;
	}

	return read_bytes;

}

static int _vorbis_seek(void *_data_stream, ogg_int64_t offset, int whence) {

	data_stream_t* data_stream = (data_stream_t*)_data_stream;

    int actual_offset;
    int can_read_bytes;

	switch (whence) {
        case SEEK_SET:
            if (offset < data_stream->data_len)
                actual_offset = offset;
            else
                actual_offset = data_stream->data_len-1;
            break;
        case SEEK_CUR:
            can_read_bytes = data_stream->data_len - data_stream->data_pos;
            if (offset <= can_read_bytes)
                actual_offset = data_stream->data_pos + offset;
            else
                actual_offset = data_stream->data_pos + can_read_bytes;
            break;
        case SEEK_END: // Seek from the end of the file
            actual_offset = data_stream->data_len;
            break;
        default:
            actual_offset = 0;
	};

	data_stream->data_pos = actual_offset;

	return 0;
}

static long _vorbis_tell(void *_data_stream) {
    data_stream_t* data_stream = (data_stream_t*) _data_stream;
    return data_stream->data_pos;
}

static int _vorbis_close(void *_data_stream) {
    return 1;
}

bool music_create_context_ogg(music_context_t* context, const unsigned char* data, int data_len, float gain) {

    OggVorbis_File  oggStream;
    vorbis_info*    vorbisInfo;

    context->data_stream.data = (unsigned char*)data;
    context->data_stream.data_len = data_len;
    context->data_stream.data_pos = 0;

    ov_callbacks vorbisCallbacks;
    vorbisCallbacks.read_func  = _vorbis_read;
    vorbisCallbacks.seek_func  = _vorbis_seek;
    vorbisCallbacks.tell_func  = _vorbis_tell;
    vorbisCallbacks.close_func = _vorbis_close;

    if (ov_open_callbacks((void*)&context->data_stream, &oggStream, NULL, 0, vorbisCallbacks) < 0) {
        return false;
    }

    vorbisInfo = ov_info(&oggStream, -1);
    ALenum format;
    if (vorbisInfo->channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (vorbisInfo->channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else {
        ov_clear(&oggStream);
        return false;
    }

    context->oggStream = oggStream;
    context->vorbisInfo = vorbisInfo;
    context->format = format;
    context->gain = gain;
    alGenBuffers(NUM_BUFFERS, context->buffers);
    alGenSources(1, &context->source);
    context->error = false;
    context->stopped = false;

    alSource3f(context->source, AL_POSITION,        0.0, 0.0, 0.0);
    alSource3f(context->source, AL_VELOCITY,        0.0, 0.0, 0.0);
    alSource3f(context->source, AL_DIRECTION,       0.0, 0.0, 0.0);
    alSourcef (context->source, AL_ROLLOFF_FACTOR,  0.0          );
    alSourcei (context->source, AL_SOURCE_RELATIVE, true         );
    alSourcei (context->source, AL_LOOPING,         false        );
    alSourcef (context->source, AL_GAIN,            context->gain);

    return true;

}

void music_start(music_context_t* context) {
    pthread_create(&context->thread, NULL, _play_music_loop, (void*) context);
}

void music_pause(music_context_t* context) {
    alSourcePause(context->source);
}

void music_resume(music_context_t* context) {
    alSourcePlay(context->source);
}

void music_stop(music_context_t* context) {
    pthread_mutex_lock(&cs_mutex);
    context->stopped = true;
    pthread_mutex_unlock(&cs_mutex);
    pthread_join(context->thread, NULL);
    pthread_detach(context->thread);
}
