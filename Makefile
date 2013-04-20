CC		= gcc
CFLAGS		= -c -std=c99 -fmessage-length=0 -Wno-unused-result
CRELEASEFLAGS   = -DNDEBUG -O3 -fexpensive-optimizations
LDFLAGS		= -lglut -lGLU -lGL -lpng -lopenal -lvorbisfile -lvorbis -logg

SOURCES		= src/ase_importer.c src/explosion.c src/random.c src/utils.c \
			src/bounding_boxes.c  src/geom.c src/resources.c src/world.c \
			src/crc32.c src/mine_crisis.c src/sound.c \
			src/draw.c src/music.c src/texture.c

OBJECTS		= $(SOURCES:.c=.o)
EXECUTABLE 	= MineCrisis

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(CRELEASEFLAGS) $< -o $@

clean:
	@rm -f $(EXECUTABLE)
	@rm -f $(OBJECTS)

