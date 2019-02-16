typedef struct{
	int SRight;
	int SLeft;
	int STop;
	int SBottom;
	int Bnum;
}EDGE;
void CreateGraph();
void EndOfProg();
void SetBackgrounds(BITMAP*, BITMAP*, BITMAP*);// stick siyah ve yeþil ekran
void BallMovement(float *, float *, float *);
void DrawBrick(BITMAP *, BITMAP *BrckImage[10], EDGE *);
void SetLevel(int,int);
void GameOverScreen(BITMAP *, BITMAP *,int*,FONT*);
void GameMainMenu();
void CheckBrick(float *, float *, float *, EDGE *, int*, SAMPLE*);
void LevelUp(BITMAP*,BITMAP*);
void Credits(BITMAP*, BITMAP*);