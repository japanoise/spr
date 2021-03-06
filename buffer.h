/* Buffer for image data */
#ifndef BUFFER_H
#define BUFFER_H
#ifndef _MSC_VER
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include "slice.h"

typedef struct {
	SDL_Color *colors;
	int len;
	int size;
	int scroll;
} palette;

enum undoType {
	StartUndo,
	EndUndo,
	PixelUndo,
};

typedef enum undoType undoType;

typedef struct {
	int index;
	Uint32 oldColor;
	Uint32 newColor;
} pixDiff;

struct undo {
	struct undo *prev;
	struct undo *next;
	undoType type;
	union {
		char batchUndoData;
		pixDiff pixUndoData;
	} data;
};

typedef struct undo undo;

typedef struct {
	StrSlice *name;
	unsigned char *filename;
	unsigned int zoom;
	int panx;
	int pany;
	int sizex;
	int sizey;
	unsigned int datachannels;
	unsigned char *data;
	unsigned char changedp;
	unsigned char tool;
	unsigned int selcontext;
	SDL_Color primary;
	SDL_Color secondary;
	palette *pal;
	undo *undoList;
	undo *saveUndo;
} buffer;

palette *defaultPalette();

void addColorToPalette(palette * pal, SDL_Color color);

buffer *makeBuffer(char *name);

buffer *makeBufferFromFile(char *filename);

void setBufferFileName(char *filename, buffer * buf);

void killBuffer(buffer * buf);

SDL_Texture *textureFromBuffer(buffer * buf, SDL_Renderer * rend);

void bufferSetPixel(buffer * buf, int px, int py, SDL_Color color);

void bufferDrawLine(buffer * buf, SDL_Color color, int x0, int y0, int x1, int y1);

void bufferPencil(buffer * buf, int px, int py, SDL_Color color);

int bufferIsDirty(buffer * buf);

void bufferStartUndo(buffer * buf);

void bufferEndUndo(buffer * buf);

void bufferDoUndo(buffer * buf);

void bufferDoRedo(buffer * buf);

void bufferDoFloodFill(buffer * buf, int px, int py, SDL_Color new);

void bufferDoFloodFillDither(buffer * buf, int px, int py, SDL_Color a, SDL_Color b);

void bufferDoRectOutline(buffer * buf, int x1, int y1, int x2, int y2, SDL_Color color);

void bufferDoRectFill(buffer * buf, int x1, int y1, int x2, int y2, SDL_Color color,
		      SDL_Color border);

SDL_Color bufferGetColorAt(buffer * buf, int x, int y);

buffer *makeNewBuffer(SDL_Renderer * rend, SDL_Texture * font);
#endif
