#include <stdio.h>
#include <allegro.h>
#include "funcheaders.h"
#include <math.h>

int main(){
	CreateGraph();
	float *PTRballx, *PTRbally, ballx = 300, bally = 642, MouseX, BallDirection = 5, *PTRBallDirection, FirstMovoment = 0;
	int m = 0, live = 3, GameLevel = 1, TotalScore=0, *PTRTotalScore;
	PTRballx = &ballx; PTRbally = &bally; PTRBallDirection = &BallDirection; PTRTotalScore = &TotalScore;
	BITMAP *buffer = create_bitmap(1000, 700);//
	BITMAP *stickbuffer = create_bitmap(1000, 700);//
	BITMAP *stick = load_bitmap("Images/stick.bmp", NULL);
	BITMAP *stickblack = load_bitmap("Images/stickblack.bmp", NULL);
	BITMAP *background = load_bitmap("Images/backgreen.bmp", NULL);
	BITMAP *background2 = load_bitmap("Images/gamebackground.bmp", NULL);
	BITMAP *levelup = load_bitmap("Images/levelup.bmp", NULL);
	BITMAP *gameover = load_bitmap("Images/gameover.bmp", NULL);
	BITMAP *GamePause = load_bitmap("Images/gamepause.bmp", NULL);
	SAMPLE *Levelupsound = load_sample("Music/levelup.wav");
	SAMPLE *GameMenuSound = load_sample("Music/gamemenusound.wav");
	SAMPLE *PlayGameSound = load_sample("Music/playgamesound.wav");
	SAMPLE *StickMusic = load_sample("Music/stick.wav");
	SAMPLE *GameOver = load_sample("Music/gameover.wav");
	SAMPLE *blop = load_sample("Music/blop.wav");
	SAMPLE *Bubble = load_sample("Music/laser.wav");
	SAMPLE *StickBreak = load_sample("Music/stickbreak.wav");
	BITMAP *BrckImage[10];
	int i,count=0;
	float tempx = 1, tempy = 1;
	for (i = 0; i < 10; i++){
		BrckImage[i] = (BITMAP*)malloc(sizeof(BITMAP));
	}
	BrckImage[1] = load_bitmap("Images/b1.bmp", NULL);
	BrckImage[2] = load_bitmap("Images/b2.bmp", NULL);
	BrckImage[3] = load_bitmap("Images/b3.bmp", NULL);
	BrckImage[4] = load_bitmap("Images/b4.bmp", NULL);
	BrckImage[5] = load_bitmap("Images/b5.bmp", NULL);
	BrckImage[6] = load_bitmap("Images/b6.bmp", NULL);
	BrckImage[7] = load_bitmap("Images/b7.bmp", NULL);
	BrckImage[8] = load_bitmap("Images/b8.bmp", NULL);
	BrckImage[9] = load_bitmap("Images/b9.bmp", NULL);
	FONT *font1 = load_font("font1.pcx", NULL, NULL);
	FONT *font2 = load_font("font2.pcx", NULL, NULL);
	EDGE *BrickEdge;
	BrickEdge = (EDGE*)malloc(160 * sizeof(EDGE));//Tuglalarýn koordinatlarýný tutmasý için

	play_sample(GameMenuSound, 128, 128, 1000, 0);
	GameMainMenu();
	stop_sample(GameMenuSound);
	SetLevel(GameLevel, live);
	play_sample(PlayGameSound, 32, 128, 1000, 0);
	while (1){
		
		MouseX = mouse_x;// mouse ekrandan çýktýðýnda alttaki çubugu kenarlara yanaþtýrýyor.
		if (mouse_x < 800 && mouse_x>125){ MouseX = mouse_x; }//alttaki cubugun mousenin x ine baglanmasý.
		if (mouse_x > 800)MouseX = 800;
		if (mouse_x < 125)MouseX = 125;//Farenin oyun ekraný dýþýna çýkmasýný engelliyor dolayýsýyla çubukta dýþarý çýkmýyor
		SetBackgrounds(buffer, background, background2);//siyah ve yeþil ekran


		//--------------------TOPUN HAREKETE BAÞLAMASI-------------
		if (FirstMovoment == 0){
			ballx = MouseX - 63;//mouse kordinatýndan 75px geriden baslayacak
			bally = 642;
			BallDirection = 1;
			//if (GameLevel==0)GameLevel = 1;
			
			if (mouse_b & 1)FirstMovoment = 1;//mouse buton basýldýgý anda top harekete baslayacak(mouse_b & 1  mousenin sol tuþuna týklandýðý anlamýna geliyor)
		}
		else if (FirstMovoment == 1){
			//--------------------TOPUN SUREKLI HAREKET ETMESI---------------
			BallMovement(PTRballx, PTRbally, PTRBallDirection); //topun hareketini saðlayan fonksiyon
		}

		for (i = 0; i <100; i++)
		{
			if (BrickEdge[i].Bnum == 0)count++;
		}
		if (i == count){
			stop_sample(PlayGameSound);
			play_sample(Levelupsound, 128, 128, 1000, 0);
			LevelUp(buffer, levelup);
			stop_sample(Levelupsound);
			ballx = MouseX - 63;//mouse kordinatýndan 75px geriden baslayacak
			bally = 642;
			BallDirection = 1;
			//first movementin içerigini yukarýya attým cunku 0 lamazsa top son patlattýgý yerdeki tuglayý sonraki bölümdede patlatýyor
			FirstMovoment = 0;
			GameLevel++;
			if (GameLevel == 7)GameLevel = 1;
			SetLevel(GameLevel, live);
			play_sample(PlayGameSound, 32, 128, 1000, 0);
		}
		textprintf_ex(buffer, font, 50, 550, makecol(255, 255, 255), -1, "i=%d count=%d", i, count);
		count = 0; i = 0;

		//ses
		if (bally< 655 && (bally) > 650 && (ballx > (MouseX - 130)) && (ballx < (MouseX + 5))){
			play_sample(StickMusic, 128, 128, 1000, 0);
		}

		//---------------TUGLALARI EKRANA CEKEN FONKSIYON------------------
		DrawBrick(buffer, BrckImage, BrickEdge);
		//--------------------TUGLAYA ÇARPMA DURUMUNU KONTROL EDEN FONKSÝYON-----------
		CheckBrick(PTRballx, PTRbally, PTRBallDirection, BrickEdge,PTRTotalScore,Bubble);


		blit(stick, buffer, 0, 0, MouseX - 125, 650, 125, 15);
		circlefill(buffer, ballx, bally, 8, makecol(124, 205, 124));


		textprintf_ex(buffer, font1, 835, 50, makecol(0, 0, 0), -1, "LEVEL: %d",GameLevel );
		textprintf_ex(buffer, font2, 835, 150, makecol(0, 0, 0), -1, "SCORE: %d", TotalScore);
		textprintf_ex(buffer, font2, 835, 600, makecol(0, 0, 0), -1, "LIFE: %d", live);
		textprintf_ex(buffer, font, 50, 50, makecol(255, 255, 255), -1, "x:%.2lf-%.2lf y:%.2lf-%.2lf", ballx, tempx, bally, tempy);
		textprintf_ex(buffer, font, 50, 100, makecol(255, 255, 255), -1, "x:%.2lf", sqrt(((ballx - tempx)*(ballx - tempx)) + ((bally - tempy)*(bally - tempy))));
		

		//--------------------TOPUN CIZGIYI GECMESI HAREKETIN BITMESI--------------
		if ((bally) > 675 && (bally) < 680){
			live--;
			stop_sample(PlayGameSound);
			play_sample(StickBreak, 128, 128, 1000, 0);
			for (i = 0; i < 200; i++)
			{
				blit(stickblack, buffer, 0, 0, MouseX - 125, 650, 125, 15);
				circlefill(buffer, ballx, bally, 8, makecol(0, 0, 0));
				rest(2);
				blit(buffer, screen, 0, 0, 0, 0, 1000, 700);
			}
			play_sample(PlayGameSound, 32, 128, 1000, 0);
			if (live == 0) {
				stop_sample(PlayGameSound);
				play_sample(GameOver, 128, 128, 1000, 0);
				GameOverScreen(buffer, gameover, PTRTotalScore, font2);
				stop_sample(GameOver);
				TotalScore = 0;
				live = 3;
				GameLevel = 1;
				SetLevel(GameLevel, live); 
				play_sample(GameMenuSound, 128, 128, 1000, 0);
				GameMainMenu();
				stop_sample(GameMenuSound);
				play_sample(PlayGameSound, 32, 128, 1000, 0);
			}
			FirstMovoment = 0;
		}//top çubuðu geçerse OYUNUN 
		if (key[KEY_ESC]){
			while (1){
				blit(GamePause, buffer, 0, 0, 250, 250, 360, 240);
				show_mouse(buffer);
				textprintf_ex(screen, font1, 835, 600, makecol(255, 225, 0), -1, "x:%d-y:%d", mouse_x, mouse_y);
				blit(buffer, screen, 0, 0, 0, 0, 1000, 700);
				if (mouse_b & 1){
					if (mouse_x > 275 && mouse_x < 500 && mouse_y>270 && mouse_y < 330)break;
					if (mouse_x>275 && mouse_x < 500 && mouse_y>350 && mouse_y < 410)exit(-1);
				}
			}
			remove_mouse();
			install_mouse();
		}
		blit(buffer, screen, 0, 0, 0, 0, 1000, 700);

		clear_bitmap(buffer);
		rest(0); tempx = ballx; tempy = bally;
	}


	free(BrickEdge);
	free(*BrckImage);
	EndOfProg();
	return 0;
}
END_OF_MAIN();