PRJNAME := sub_rescue
OBJS := data.rel sub_rescue.rel

all: $(PRJNAME).sms

data.c: data/* data/font.1bpp data/sprites_tiles.psgcompr data/background_tiles.psgcompr \
		data/enemy_death.psg data/rescue_diver.psg data/fill_air.psg data/player_death.psg data/player_danger.psg \
		data/level_end.psg data/level_beep.psg
	folder2c data data
	
data/sprites_tiles.psgcompr: data/img/sprites.png
	BMP2Tile.exe data/img/sprites.png -noremovedupes -8x16 -palsms -fullpalette -savetiles data/sprites_tiles.psgcompr -savepalette data/sprites_palette.bin

data/background_tiles.psgcompr: data/img/background.png
	BMP2Tile.exe data/img/background.png -palsms -fullpalette -savetiles data/background_tiles.psgcompr -savetilemap data/background_tilemap.bin -savepalette data/background_palette.bin
	
data/player_death.psg: data/deflemask/player_death.vgm
	vgm2psg data/deflemask/player_death.vgm data/player_death.psg 23

data/enemy_death.psg: data/deflemask/enemy_death.vgm
	vgm2psg data/deflemask/enemy_death.vgm data/enemy_death.psg 23

data/rescue_diver.psg: data/deflemask/rescue_diver.vgm
	vgm2psg data/deflemask/rescue_diver.vgm data/rescue_diver.psg 23

data/fill_air.psg: data/deflemask/fill_air.vgm
	vgm2psg data/deflemask/fill_air.vgm data/fill_air.psg 23

data/player_danger.psg: data/deflemask/player_danger.vgm
	vgm2psg data/deflemask/player_danger.vgm data/player_danger.psg 23
	
data/level_end.psg: data/deflemask/level_end.vgm
	vgm2psg data/deflemask/level_end.vgm data/level_end.psg
	
data/level_beep.psg: data/deflemask/level_beep.vgm
	vgm2psg data/deflemask/level_beep.vgm data/level_beep.psg
	
%.vgm: %.wav
	psgtalk -r 512 -u 1 -m vgm $<

%.rel : %.c
	sdcc -c -mz80 --peep-file lib/peep-rules.txt $<

$(PRJNAME).sms: $(OBJS)
	sdcc -o $(PRJNAME).ihx -mz80 --no-std-crt0 --data-loc 0xC000 lib/crt0_sms.rel $(OBJS) SMSlib.lib lib/PSGlib.rel
	ihx2sms $(PRJNAME).ihx $(PRJNAME).sms	

clean:
	rm *.sms *.sav *.asm *.sym *.rel *.noi *.map *.lst *.lk *.ihx data.*
