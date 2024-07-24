#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));
	createRankList();
	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
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
			newRank(score);
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
	FILE* fp;
	int i, j;

	//1. 파일 열기
	fp = fopen("rank.txt", "r");
	if (fp == NULL) {
		fp = fopen("rank.txt", "w");
		rank_num = 0;
		return;
	}
	headnode = (Node*)malloc(sizeof(Node));
	int txtscore; 
	char txtname[NAMELEN];
	headnode->link = NULL;
	Node* currentnode = headnode;
	if (fscanf(fp, "%d", &rank_num) != EOF) {
		for (i = 0; i < rank_num; i++) {
			Node* newnode = (Node*)malloc(sizeof(Node));
			fscanf(fp, "%s %d", txtname, &txtscore);
			newnode->score = txtscore;
			strcpy(newnode->name, txtname);
			newnode->link = NULL;
			currentnode->link = newnode;
			currentnode = newnode;
		}
	}
	else {
		return;
	}
	// 4. 파일닫기
	fclose(fp);
	return;
}

void rank(){
	// user code
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	int X = 1, Y = rank_num, ch, i;
	Node* currentnode = headnode;
	prevnode = NULL;
	clear();

	//2. printw()로 3개의 메뉴출력
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");
	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
	ch = wgetch(stdscr);
	
	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	if (ch == '1') {
		echo();
		printw("X: ");
		scanw("%d", &X);
		printw("Y: ");
		scanw("%d", &Y);
		noecho();
		if (Y > rank_num) Y = rank_num;
		printw("       name       |   score   \n");
		printw("------------------------------\n");
		int count = 0;
		if (X >= 1 && Y <= rank_num && X < Y) {
			while (1) {
				if (count >= X && count <= Y) {
					printw(" %-17s| %-10d\n", currentnode->name, currentnode->score);
				}
				else {
					if (count > Y || currentnode == NULL) break;
				}
				currentnode = currentnode->link;
				count += 1;
			}
		}
		else
			printw("search failure: no rank in the list\n");
	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if (ch == '2') {
		char str[NAMELEN + 1];  //입력받을 문자열 선언
		int check = 0; //같은 문자열이 있는지 비교하기 위한 flag변수
		echo();
		printw("input the name: ");  
		scanw("%s", str);  //문자열 입력받기
		noecho();
		printw("       name       |   score   \n");
		printw("------------------------------\n");
		currentnode = currentnode->link;  //currentnode를 첫번째 노드로 이동
		while (currentnode != NULL) {  //currentnode가 NULL이 될때까지 문자열과 같은 이름이 저장된 노드의 이름과 점수 출력
			if (strcmp(currentnode->name, str) == 0) {
				printw(" %-17s| %-10d\n", currentnode->name, currentnode->score); //일치하면 형식에 맞춰 이름과 점수 출력
				check++;  //flag변수인 check값에 1 더하기
			}
			currentnode = currentnode->link; //다음 노드로 이동
		}
		if (check == 0) //check변수가 0이면 입력받은 문자열과 일치하는 이름이 없음을 나타내므로 예외상황 처리
			printw("search failure: no rank in the list\n");

	}
	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if (ch == '3') {
		int num;  //입력받을 수
		echo();
		printw("input the rank: ");
		scanw("%d", &num);
		noecho();
		if (num > rank_num || num <= 0)  //입력받은 값이 0이하이거나 랭크의 수보다 크게되면 예외 상황 처리
			printw("search failure: the rank not in the list\n");
		else {
			for (i = 0; i < num; i++) {  //입력받은 수만큼 currentnode와 prevnode 이동
				prevnode = currentnode;
				currentnode = currentnode->link; //currentnode를 삭제할 노드에 위치시킴
			}
			prevnode->link = currentnode->link; //prevnode의 link가 삭제할 노드의 다음 노드를 가리키게함
			free(currentnode);  //삭제할 노드를 가리키는 currentnode를 삭제
			rank_num -= 1; //랭크목록에서 한 개의 랭크를 삭제했으므로 랭크의 수 1만큼 줄이기
			printw("\n");
			printw("result: the rank deleted\n");
			writeRankFile();  //삭제할 노드를 삭제한 후의 리스트로 다시 rank.txt파일 작성
		}
	}
	getch();
}

void writeRankFile(){
	// user code
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	//1. "rank.txt" 연다
	FILE* fp = fopen("rank.txt", "w");
	Node* currentnode = headnode;
	//2. 랭킹 정보들의 수를 "rank.txt"에 기록
	fprintf(fp, "%d\n", rank_num);
	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	currentnode = currentnode->link;
	while (currentnode != NULL) {
		fprintf(fp, "%s %d\n", currentnode->name, currentnode->score);
		currentnode = currentnode->link;
	}
	fclose(fp);
	return;
}

void newRank(int score){
	// user code
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN + 1];
	int i, j;
	clear();
	//1. 사용자 이름을 입력받음
	Node* currentnode = headnode;
	Node* newnode = (Node*)malloc(sizeof(Node));
	printw("your name: ");
	echo();
	scanw("%s", str);
	noecho();
	strcpy(newnode->name, str);
	newnode->score = score;
	prevnode = NULL;
	//2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
	while (currentnode != NULL) {
		if (score > currentnode->score) {
			prevnode->link = newnode;
			newnode->link = currentnode;
			rank_num += 1;
			break;
		}
		prevnode = currentnode;
		currentnode = currentnode->link;
	}
	writeRankFile();
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
