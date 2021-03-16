#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lib/SMSlib.h"
#include "lib/PSGlib.h"
#include "data.h"

void draw_meta_sprite(int x, int y, int w, int h, unsigned char tile) {
	for (char i = h; i; i--) {
		int sx = x;
		for (char j = w; j; j--) {
			SMS_addSprite(sx, y, tile);
			sx += 8;
			tile += 2;
		}
		y += 16;
	}
}

void clear_sprites() {
	SMS_initSprites();	
	SMS_finalizeSprites();
	SMS_copySpritestoSAT();
}

void interrupt_handler() {
	PSGFrame();
}

void load_standard_palettes() {
	SMS_loadBGPalette(test_pal);
	SMS_loadSpritePalette(sprites_palette_bin);
	SMS_setSpritePaletteColor(0, 0);
}

void load_tile_zero() {
	SMS_load1bppTiles(font_1bpp, 0, 8, 0, 1);
}

void configure_text() {
	load_tile_zero();
	SMS_load1bppTiles(font_1bpp, 352, font_1bpp_size, 0, 1);
	SMS_configureTextRenderer(352 - 32);
}

char gameplay_loop() {
	int frame = 0;

	SMS_waitForVBlank();
	SMS_displayOff();

	SMS_loadPSGaidencompressedTiles(sprites_tiles_psgcompr, 0);

	load_standard_palettes();

	clear_sprites();

	configure_text();
	
	SMS_setNextTileatXY(4, 12);
	puts("Hello, world!");
		
	SMS_displayOn();
	
	while(1) {
		SMS_initSprites();	
		draw_meta_sprite(16, 16, 3, 1, 2 + frame);
		draw_meta_sprite(32, 40, 3, 1, 20 + frame);
		draw_meta_sprite(16, 74, 3, 1, 64 + 2 + frame);
		draw_meta_sprite(32, 108, 3, 1, 64 + 20 + frame);
		SMS_finalizeSprites();		

		SMS_waitForVBlank();
		SMS_copySpritestoSAT();
		
		frame += 6;
		if (frame > 12) frame = 0;
		
		for (int i = 0; i != 3; i++) {
			SMS_waitForVBlank();
		}
	}
}

void main() {
	SMS_useFirstHalfTilesforSprites(1);
	SMS_setSpriteMode(SPRITEMODE_TALL);
	
	while (1) {			
		gameplay_loop();
	}
}

SMS_EMBED_SEGA_ROM_HEADER(9999,0); // code 9999 hopefully free, here this means 'homebrew'
SMS_EMBED_SDSC_HEADER(0,1, 2021,3,15, "Haroldo-OK\\2021", "Sub Rescue",
  "A subaquatic shoot-em-up.\n"
  "Built using devkitSMS & SMSlib - https://github.com/sverx/devkitSMS");
