#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7; //nextBlock[2]를 랜덤으로 설정히여 초기화
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
	DrawBox(11,WIDTH+10,4,8);  //nextBlock[2]에 대한 박스를 새로 그려준다.
	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(18,WIDTH+10);  //nextBlock[2]에 대한 박스가 생겼으므로 score 박스를 밑으로 내려준다.
	printw("SCORE");
	DrawBox(19,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(20,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	//nextBlock[2]을 nextblock[1]밑에 추가로 그려준다
	for (i = 0; i < 4; i++) {
		move(12 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[2]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int i = 0;
	while (i < BLOCK_HEIGHT) {
		for (int j = 0; j < BLOCK_WIDTH; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				int y = i + blockY;
				int x = j + blockX;
				if (y >= HEIGHT || y < 0 || x >= WIDTH || x < 0)
					return 0;
				if (f[y][x] == 1)
					return 0;
			}
		}
		i++;
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다. 
	int before_rotate = blockRotate;
	int beforeX = blockX;
	int beforeY = blockY;
	int i, j;
	switch (command) {
	case KEY_UP:
		if (blockRotate < 1) before_rotate = 3;
		else before_rotate = blockRotate - 1;
		break;
	case KEY_DOWN:
		beforeY = blockY - 1;
		break;
	case KEY_LEFT:
		beforeX = blockX + 1;
		break;
	case KEY_RIGHT:
		beforeX = blockX - 1;
		break;
	default:
		break;
	}
	for (i = 0; i < BLOCK_HEIGHT; i++)
		for (j = 0; j < BLOCK_WIDTH; j++)
		{
			if (block[currentBlock][before_rotate][i][j]) {
				move(beforeY + i + 1, beforeX + j + 1);
				printw(".");
			}
		}

	int PosY = BlockShadowPos(beforeY, beforeX, currentBlock, before_rotate);
	//이전 그림자 블록을 지우기
	for (i = 0; i < BLOCK_HEIGHT; i++)
		for (j = 0; j < BLOCK_WIDTH; j++)
		{
			if (block[currentBlock][before_rotate][i][j]) {
				move(PosY + i + 1, beforeX + j + 1);
				printw(".");
			}
		}

	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
	return;
}

void BlockDown(int sig){
	// user code

	//강의자료 p26-27의 플로우차트를 참고한다.
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX) == 1) {
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
		timed_out = 0;
		return;
	}
	else {
		if (blockY == -1) {
			gameOver = 1;
			return;
		}
		//score에 블록이 아래에 닿은 면적의 수*10의 값을 더한다
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);
		PrintScore(score);
		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand() % 7; //nextBlock[2]를 랜덤으로 새로 설정
		DrawNextBlock(nextBlock);
		blockRotate = 0;
		blockX = WIDTH / 2 - 2;
		blockY = -1;
		DrawField();
		timed_out = 0;
		return;
	}
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//Block이 추가된 영역의 필드값을 바꾼다.
	int area = 0; //닿는 면적의 수를 나타내는 변수 설정
	for (int i = 0; i < BLOCK_HEIGHT; i++)
		for (int j = 0; j < BLOCK_WIDTH; j++) {
			if (block[currentBlock][blockRotate][i][j]) {
				f[blockY + i][blockX + j] = 1;
				//블록을 필드에 쌓을 때 쌓여질 블록 밑에 이미 블록이 쌓여있거나 혹은 필드의 바닥일 때 area변수에 1추가
				if (f[blockY + i + 1][blockX + j] == 1 || (blockY + i) == (HEIGHT - 1))
					area += 1;
			}
		}
	return area * 10; //블록이 아래에 닿은 면적의 수에 10을 곱한 값 반환
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	int i = HEIGHT - 1;
	int cnt = 0;
	while (i >= 0) {
		int line_flag = 1;
		for (int j = 0; j < WIDTH; j++) {
			if (f[i][j] == 0) {
				line_flag = 0;
				break;
			}
		}
		if (line_flag == 1) {
			for (int k = i - 1; k >= 1; k--) {
				for (int m = 0; m < WIDTH; m++)
					f[k + 1][m] = f[k][m];
				for (int n = 0; n < WIDTH; n++)
					f[0][n] = 0;
			}
			cnt++;
		}
		i--;
	}
	return (cnt * cnt) * 100;
}

///////////////////////////////////////////////////////////////////////////

int BlockShadowPos(int y, int x, int blockID, int blockRotate) {
	for (int i = 0; i < HEIGHT; i++) {
		if (CheckToMove(field, blockID, blockRotate, y + 1, x) == 0)
			break;
		y++;
	}
	return y;
}

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	int PosY = BlockShadowPos(y, x, blockID, blockRotate);
	DrawBlock(PosY, x, blockID, blockRotate, '/');
}
void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	// user code
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawShadow(y, x, blockID, blockRotate);
}

/////////////////////////////////////////////////////////////////////////

void createRankList(){
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
