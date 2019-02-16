#include <allegro.h>
#include "funcheaders.h"
#include <stdio.h>
#include <math.h>

void CreateGraph(){
	allegro_init();//baþlatma fonk
	int res, depth;
	depth = desktop_color_depth();
	if (depth == 0)depth = 32;
	set_color_depth(depth);//allegro renk derinliði daha fazla seçenek sunmasý.
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1000, 700, 0, 0);//allegro pencere modu ayarý-window kýsmý fullscreen olursa tam ekran calýsýr
	if (res != 0){ //üstteki kodda res 0 deðerini alýyor almýyorsa sorun var demektir ekrana hata mesajý verecek.
		allegro_message(allegro_error);
		exit(-1);
	}
	install_keyboard();
	install_mouse();
	install_timer();
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "A");
}

void EndOfProg(){
	clear_keybuf();
}
void SetBackgrounds(BITMAP *buffer, BITMAP *background, BITMAP *background2) {
	blit(background, buffer, 0, 0, 700, 0, 300, 700);
	blit(background2, buffer, 0, 0, 0, 0, 800, 800);
}
void BallMovement(float *ballx, float *bally, float *direction){
	float x;
	static float a = 3, b = 1;
	/*KENARLAR*/
	if ((*ballx) < 8 && *direction == 1)*direction = 2;//top sol aþaðýdan geliyorsa sað yukarý yönlendir
	if ((*ballx) < 8 && *direction == 4)*direction = 3;//top sol yukarýdan geliyorsa sað aþaðý yönlendir
	if ((*ballx) > 792 && *direction == 3)*direction = 4;
	if ((*ballx) > 792 && *direction == 2)*direction = 1;
	if ((*bally) < 8 && *direction == 2)*direction = 3;
	if ((*bally) < 8 && *direction == 1)*direction = 4;
	if ((*bally) < 8 && *direction == 5)*direction = 6;
	/*topun y si 650 olduðu anda 3 ihtimali karþýlaþtýracak (topun aþaðýya saðdan soldan ve ortadan gelme ihtimali)*/
	if ((*bally) > 650 && (*bally) < 655 && * direction == 4){//sað
		x = mouse_x;
		if (x > 800)x = 800;//mouse ekran dýþýna cýkýnca mouse koordinatý yerine sabit degerler verildi bu kod olmazsa stick ciziliyor fakat içinden geçiyor top
		if (x < 125)x = 125;
		if (((*ballx) >= (x - 130)) && ((*ballx) < (x - 100))){ a = 1.7, b = 2.6664; *direction = 1; }
		else if (((*ballx) >= (x - 100)) && ((*ballx) < (x - 65))) { a = 3; b = 1; *direction = 1; }
		else if (((*ballx) >= (x - 65)) && ((*ballx) < (x - 60))) { a = 3; b = 1; *direction = 5; }
		else if (((*ballx) >= (x - 60)) && ((*ballx) < (x - 25))) { a = 3; b = 1; *direction = 2; }
		else if (((*ballx) >= (x - 25)) && ((*ballx) <= (x + 5))){ a = 1.7, b = 2.6664; *direction = 2; }
	}
	if ((*bally) > 650 && (*bally) < 655 && *direction == 3){//sol
		x = mouse_x;
		if (x > 800)x = 800;
		if (x < 125)x = 125;
		if (((*ballx) >= (x - 130)) && ((*ballx) < (x - 100))) { a = 1.7, b = 2.6664; *direction = 1; }
		else if (((*ballx) >= (x - 100)) && ((*ballx) < (x - 65))) { a = 3; b = 1; *direction = 1; }
		else if (((*ballx) >= (x - 65)) && ((*ballx) < (x - 60))) { a = 3; b = 1; *direction = 5; }
		else if (((*ballx) >= (x - 60)) && ((*ballx) < (x - 25))) { a = 3; b = 1; *direction = 2; }
		else if (((*ballx) >= (x - 25)) && ((*ballx) <= (x + 5))) { a = 1.7, b = 2.6664; *direction = 2; }
	}
	if ((*bally) > 650 && (*bally) < 655 && *direction == 6){//orta
		x = mouse_x;
		if (x > 800)x = 800;
		if (x < 125)x = 125;
		if (((*ballx) >= (x - 130)) && ((*ballx) < (x - 100))) { a = 1.7, b = 2.6664; *direction = 1; }
		else if (((*ballx) >= (x - 100)) && ((*ballx) < (x - 65))) { a = 3; b = 1; *direction = 1; }
		else if (((*ballx) >= (x - 65)) && ((*ballx) < (x - 60))) { a = 3; b = 1;  *direction = 5; }
		else if (((*ballx) >= (x - 60)) && ((*ballx) < (x - 25))) { a = 3; b = 1; *direction = 2; }
		else if (((*ballx) >= (x - 25)) && ((*ballx) <= (x + 5))) { a = 1.7, b = 2.6664; *direction = 2; }
	}
	/*KENARA YADA CUBUGA CARMPADIGI SURECE BU KODLAR ISLENECEK*/
#if 1
	if (*direction == 1){ (*ballx) -= b; (*bally) -= a; }
	else if (*direction == 2){ (*ballx) += b; (*bally) -= a; }
	else if (*direction == 3){ (*ballx) += b; (*bally) += a; }
	else if (*direction == 4){ (*ballx) -= b; (*bally) += a; }
	else if (*direction == 5){ (*bally) -= a; }
	else if (*direction == 6){ (*bally) += a; }
#endif
}
/*RESÝMLERÝ EKRANA GETÝRME FONKSÝYONU*/
void DrawBrick(BITMAP *buffer, BITMAP *BrckImage[10], EDGE *BrickEdge){
	float BWidth, BHeight, ballx, bally, BallDirection;
	int Fnum, i = 0;
	FILE *FileBricks = fopen("Levels/Temp.txt", "r");
	for (BHeight = 0; BHeight < 400; BHeight += 25)
	{
		for (BWidth = 0; BWidth < 800; BWidth += 80)
		{

			if (feof(FileBricks))break;//dosya sonuna geldiðinde bitirecek boþ yere dönmesini ve rastgele tugla atmasýný engelliyor
			fscanf(FileBricks, "%d", &Fnum);
			if (Fnum == 0){//dosyadan gelen sayý 0 sa blok çizmeyecek o yüzden 0 deðerleri verildi.
				BrickEdge[i].SLeft = 0;
				BrickEdge[i].SRight = 0;
				BrickEdge[i].STop = 0;
				BrickEdge[i].SBottom = 0;
				BrickEdge[i].Bnum = Fnum;

			}
			else{
				BrickEdge[i].SLeft = BWidth;
				BrickEdge[i].SRight = BWidth + 80;
				BrickEdge[i].STop = BHeight;
				BrickEdge[i].SBottom = BHeight + 25;
				BrickEdge[i].Bnum = Fnum;
			}i++;

			switch (Fnum)
			{
			case 1:
				blit(BrckImage[1], buffer, 0, 0, BWidth, BHeight, 80, 25);
				break;
			case 2:
				blit(BrckImage[2], buffer, 0, 0, BWidth, BHeight, 80, 25);
				break;
			case 3:
				blit(BrckImage[4], buffer, 0, 0, BWidth, BHeight, 80, 25);
				break;
			case 4:
				blit(BrckImage[4], buffer, 0, 0, BWidth, BHeight, 80, 25);
				break;
			case 5:
				blit(BrckImage[5], buffer, 0, 0, BWidth, BHeight, 80, 25);
				break;
			case 6:
				blit(BrckImage[6], buffer, 0, 0, BWidth, BHeight, 80, 25);
				break;
			case 7:
				blit(BrckImage[7], buffer, 0, 0, BWidth, BHeight, 80, 25);
				break;
			case 8:
				blit(BrckImage[8], buffer, 0, 0, BWidth, BHeight, 80, 25);
				break;
			case 9:
				blit(BrckImage[9], buffer, 0, 0, BWidth, BHeight, 80, 25);
				break;
			default:
				break;
			}
		}
	}
	fclose(FileBricks);
}

void CheckBrick(float *PTRballx, float *PTRbally, float *PTRdirection, EDGE *BrickEdge,int *ptrscore,SAMPLE *bubble){

	int BHeight, BWidth, i = 0,collision=0;
	FILE *FileBricks = fopen("Levels/Temp.txt", "w");
	for (BHeight = 0; BHeight < 300; BHeight += 25)
	{
		for (BWidth = 0; BWidth < 800; BWidth += 80)
		{
			if (BrickEdge[i].Bnum < 0)break;//0 dan küçük olmasý dosyaya rastgele rakam attýgý anlamýna gelir yani tugla koordinatlarý bitmiþ ve boþ yere dönüyordur.

#if 1
				/*topun tuglanýn SAÐ tarafýna deðmesinde olmasýný beklediðim durum*/
				if (fabs((*PTRballx) - BrickEdge[i].SRight) <= 8 && (*PTRbally) >= BrickEdge[i].STop && (*PTRbally) <= BrickEdge[i].SBottom && (*PTRdirection) == 1){ (*PTRdirection) = 2; collision = 1; }
				if (fabs((*PTRballx) - BrickEdge[i].SRight) <= 8 && (*PTRbally) >= BrickEdge[i].STop && (*PTRbally) <= BrickEdge[i].SBottom && (*PTRdirection) == 4){(*PTRdirection) = 3; collision = 1;}
			
				/*SOL*/
				if (fabs((*PTRballx) - BrickEdge[i].SLeft) <= 8 && (*PTRbally) >= BrickEdge[i].STop && (*PTRbally) <= BrickEdge[i].SBottom && (*PTRdirection) == 3){ (*PTRdirection) = 4; collision = 1; }
				if (fabs((*PTRballx) - BrickEdge[i].SLeft) <= 8 && (*PTRbally) >= BrickEdge[i].STop && (*PTRbally) <= BrickEdge[i].SBottom && (*PTRdirection) == 2){ (*PTRdirection) = 1; collision = 1; }

				/*ALT*/
				if (fabs((*PTRbally) - BrickEdge[i].SBottom) <= 8 && (*PTRballx) >= BrickEdge[i].SLeft && (*PTRballx) <= BrickEdge[i].SRight && (*PTRdirection) == 2){ (*PTRdirection) = 3; collision = 1; }
				if (fabs((*PTRbally) - BrickEdge[i].SBottom) <= 8 && (*PTRballx) >= BrickEdge[i].SLeft && (*PTRballx) <= BrickEdge[i].SRight && (*PTRdirection) == 1){ (*PTRdirection) = 4; collision = 1; }
				if (fabs((*PTRbally) - BrickEdge[i].SBottom) <= 8 && (*PTRballx) >= BrickEdge[i].SLeft && (*PTRballx) <= BrickEdge[i].SRight && (*PTRdirection) == 5){ (*PTRdirection) = 6; collision = 1; }

				/*ÜST*/
				if (fabs((*PTRbally) - BrickEdge[i].STop) <= 8 && (*PTRballx) >= BrickEdge[i].SLeft && (*PTRballx) <= BrickEdge[i].SRight && (*PTRdirection) == 3){ (*PTRdirection) = 2; collision = 1; }
				if (fabs((*PTRbally) - BrickEdge[i].STop) <= 8 && (*PTRballx) >= BrickEdge[i].SLeft && (*PTRballx) <= BrickEdge[i].SRight && (*PTRdirection) == 4){ (*PTRdirection) = 1; collision = 1; }
				if (fabs((*PTRbally) - BrickEdge[i].STop) <= 8 && (*PTRballx) >= BrickEdge[i].SLeft && (*PTRballx) <= BrickEdge[i].SRight && (*PTRdirection) == 6){ (*PTRdirection) = 5; collision = 1; }
				//çarptýysa struct içindeki deðerler sýfýrlanacak.
				if (collision == 1){
					BrickEdge[i].SLeft = 0;
					BrickEdge[i].SRight = 0;
					BrickEdge[i].STop = 0;
					BrickEdge[i].SBottom = 0;
					BrickEdge[i].Bnum = 0;
					collision = 0;
					(*ptrscore) += 25;
					play_sample(bubble, 128, 128, 1000, 0);
				}
#endif			
			fprintf(FileBricks, " %d", BrickEdge[i].Bnum);//top tuðlaya çarptýysa 0 olarak deðiþtirecek tempi yeniden yazmýþ olacak.
			i++;
		}
	}
	fclose(FileBricks);
}

void SetLevel(int GameLevel,int Live){
	int i; FILE *FLEVELread;
	switch (GameLevel){
	case 1:
		FLEVELread = fopen("Levels/level1.txt", "r");
		break;
	case 2:
		FLEVELread = fopen("Levels/level2.txt", "r");
		break;
	case 3:
		FLEVELread = fopen("Levels/level3.txt", "r");
		break;
	case 4:
		FLEVELread = fopen("Levels/level4.txt", "r");
		break;
	case 5:
		FLEVELread = fopen("Levels/level5.txt", "r");
		break;
	case 6:
		FLEVELread = fopen("Levels/level6.txt", "r");
		break;
	}
	FILE *FLEVELwrite = fopen("Levels/Temp.txt", "w");
	while (!feof(FLEVELread)){
		fscanf(FLEVELread, "%d", &i);
		fprintf(FLEVELwrite, " %d", i);
	}
	fclose(FLEVELwrite);
	fclose(FLEVELread);
}
void GameOverScreen(BITMAP *buffer, BITMAP *gameover, int *TotalScore, FONT *font2) {

	while (!key[KEY_SPACE]) {
		blit(gameover, buffer, 0, 0, 0, 0, 1200, 900);
		textprintf_ex(buffer, font2, 350, 75, makecol(255, 255, 255), -1, "YOUR TOTAL SCORE IS: %d", (*TotalScore));
		textprintf_ex(buffer, font, 400, 600, makecol(255, 255, 255), -1, "Press 'SPACE' to continue. ");
		blit(buffer, screen, 0, 0, 0, 0, 1200, 900);
	}
}

void GameMainMenu(){
	BITMAP *gamemenu = load_bitmap("Images/gamemenu.bmp", NULL);
	BITMAP *credit = load_bitmap("Images/credit.bmp", NULL);
	BITMAP *buffer = create_bitmap(1000, 700);
	blit(gamemenu, buffer, 0, 0, 0, 0, 1000, 700);
	show_mouse(buffer); 
	int m = 0;
	while (m==0){
		if (mouse_b & 1){
			if (mouse_x > 650 && mouse_x < 950 && mouse_y > 150 && mouse_y < 200)m = 1;
			if (mouse_x > 650 && mouse_x < 950 && mouse_y > 265 && mouse_y < 325) {
				Credits(buffer, credit );
				blit(gamemenu, buffer, 0, 0, 0, 0, 1000, 700);
				blit(buffer, screen, 0, 0, 0, 0, 1000, 700);
			}
			if (mouse_x > 650 && mouse_x < 950 && mouse_y > 325 && mouse_y < 380)exit(1);
		}
		blit(buffer, screen, 0, 0, 0, 0, 1000, 700);
		textprintf_ex(screen, font, 400, 600, makecol(255, 255, 255), -1, "%d %d", mouse_x, mouse_y);
	}
	fflush(stdin);
	mouse_b = 0;
}

void LevelUp(BITMAP *buffer, BITMAP *levelup) {
	int i;
		for (i = 303; i < 697; i++)
		{
			blit(levelup, buffer, 0, 0, 0, 0, 1000, 700);
			rect(buffer, 300, 500, 700, 525, makecol(65, 31, 32));
			rectfill(buffer, 302, 503, i, 522, makecol(241, 77, 34));
			rest(5);
			blit(buffer, screen, 0, 0, 0, 0, 1000, 700);
		}

}
void Credits(BITMAP *buffer,BITMAP *credit) {

	while (!key[KEY_SPACE]) {
		blit(credit, buffer, 0, 0, 0, 0, 1000, 700);
		textprintf_ex(buffer, font, 400, 600, makecol(255, 255, 255), -1, "Press 'SPACE' to continue. ");
		blit(buffer, screen, 0, 0, 0, 0, 1000, 700);
		show_mouse(buffer);
	}
}