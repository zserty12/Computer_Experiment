/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.

	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
#include <stack>
#include <queue>
using namespace std;
class vertex {
public:
	int idx;
	int idy;
};
vertex spath[1000];
vertex qpath[1000];
stack<vertex> Stack;
queue<vertex> Queue;

int gapx[4] = { 0, 1, 0, -1 };
int gapy[4] = { 1, 0, -1, 0 };

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	// Load a font rather than the default
	//myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked

	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS", false, false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	hPopup = menu->AddPopupMenu(hMenu, " DFS");
	menu->AddPopupItem(hPopup, "Show DFS", false, false);
	hPopup = menu->AddPopupMenu(hMenu, " BFS");
	menu->AddPopupItem(hPopup, "Show BFS", false, false);

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if (title == "Open") {
		readFile();
	}
	if (title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if (title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;

	}

	if (title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly
		if (isOpen) {
			BFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;

	}

	if (title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if (title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int x = 0, y = 0;
	int i, j;

	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			if (input[i][j] == '-') {
				ofDrawLine(x, y, x + 40, y);
				x += 40;
			}
			if (input[i][j] == '|') {
				ofDrawLine(x, y, x, y + 40);
				if (j == numCols - 1)
					y += 40;
			}
			if (input[i][j] == ' ') {
				if (i % 2 == 0) {
					x += 40;
				}
				else {
					if (j % 2 == 1) { x += 40; }
				}
			}
		}
		x = 0;
	}
	


	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}

	if (isbfs) {
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			bfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if (bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		//myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if (bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else {
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if (bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if (bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Escape key exit has been disabled but it can be checked here
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if (key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		cout << "file name is " << fileName << endl;
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;


			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다

			for (auto line : buffer.getLines()) {
				numCols = line.size();
				cnt++;
			}
			
			numRows = cnt;
			input = new char* [numRows];
			for (int i = 0; i < numRows; i++)
				input[i] = new char[numCols];

			for (auto line : buffer.getLines()) {
				for (int j = 0; j < numCols; j++) {
					input[idx][j] = line[j];
				}
				idx++;
			}

			/*for (int a = 0; a < numRows; a++) {
				for (int b = 0; b < numCOls; b++) {
					cout << input[a][b];
				}
				cout << endl; //미로가 잘 나오는지 확인하려고 작성한 코드입니다
			}*/
			HEIGHT = (numRows - 1) / 2;
			WIDTH = (numCols - 1) / 2;

			visited = new int* [HEIGHT];
			for (int j = 0; j < HEIGHT; j++)
				visited[j] = new int[WIDTH];

			isdfs = false;
			//isbfs = false;
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() {

	//TO DO
	// malloc한 memory를 free해주는 함수
	for (int i = 0; i < numRows; i++)
		delete[] input[i];
	delete[] input;

	/*for (int j = 0; j < HEIGHT; j++)
		delete[] visited[j];
	delete[] visited;*/

}


bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)
	int i, j;
	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++)
			visited[i][j] = 0;
	vertex start;
	vertex target;
	start.idx = 0; start.idy = 0;
	target.idx = HEIGHT - 1; target.idy = WIDTH - 1;
	Stack.push(start);
	visited[0][0] = 1;
	vertex node; int flag = 0;
	int end = 0;
	while (!Stack.empty()) {
		node = Stack.top();
		flag = 0;
		if (node.idx == target.idx && node.idy == target.idy) {
			end = 1;
			isdfs = true;
			break;
		}
		for (i = 0; i < 4; i++) {
			int x = 2 * node.idx + 1 +  gapx[i]; int y = 2 * node.idy + 1 + gapy[i];
			int ix = node.idx + gapx[i]; int iy = node.idy + gapy[i];
			if (x < 1 || x >= numRows - 1 || y < 1 || y >= numCols - 1) continue;
			if (visited[ix][iy] == 0 && input[x][y] == ' ') {
				visited[ix][iy] = 1;
				vertex temp; temp.idx = ix; temp.idy = iy;
				Stack.push(temp);
				flag = 1;
				break;
			}
		}
		if (flag != 1)
			Stack.pop();

	}
	/*for (int a = 0; a < HEIGHT; a++) {
		for (int b = 0; b < WIDTH; b++) {
			cout << visited[a][b];
		}
		cout << endl;
	}*/
	
	size = Stack.size();
	for (int k = size - 1; k >= 0; k--) {
		vertex tmp;
		tmp = Stack.top(); Stack.pop();
		spath[k] = tmp;
	}
	//for (int a = 0; a < size; a++)
		//cout << path[a].idx << " " << path[a].idy << endl;

	if (end) {
		return true;
	}
	else
		return false;
}
void ofApp::dfsdraw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)


	ofSetColor(ofColor::lightGreen);

	int i, j, k;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (visited[i][j] == 1) {
				for (k = 0; k < 4; k++) {
					int tx = 2 * i + 1 + gapx[k]; int ty = 2 * j + 1 + gapy[k];
					int x = i + gapx[k]; int y = j + gapy[k];
					if (x < 0 || x > HEIGHT - 1 || y < 0 || y > WIDTH - 1) continue;
					if (visited[x][y] == 1 && input[tx][ty] == ' ') {
						ofDrawLine(40 * j + 20, 40 * i + 20, 40 * y + 20, 40 * x + 20);
					}
				}
			}
		}
	}

	ofSetColor(ofColor::darkGreen);
	for (int a = 0; a < size - 1; a++) {
		ofDrawLine(40 * spath[a].idy + 20, 40 * spath[a].idx + 20, 40 * spath[a + 1].idy + 20, 40 * spath[a + 1].idx + 20);
	}
}

bool ofApp::BFS() {
	int i, j;
	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++)
			visited[i][j] = 0;
	vertex start;
	vertex target;
	start.idx = 0; start.idy = 0;
	target.idx = HEIGHT - 1; target.idy = WIDTH - 1;
	Queue.push(start);
	visited[0][0] = 1;
	vertex node; cnt = 0;
	int end = 0; 
	while (!Queue.empty()) {
		node = Queue.front();
		Queue.pop();
		for (i = 0; i < 4; i++) {
			int x = 2 * node.idx + 1 + gapx[i]; int y = 2 * node.idy + 1 + gapy[i];
			int ix = node.idx + gapx[i]; int iy = node.idy + gapy[i];
			if (x < 1 || x >= numRows - 1 || y < 1 || y >= numCols - 1) continue;
			if (visited[ix][iy] == 0 && input[x][y] == ' ') {
				visited[ix][iy] = 1;
				vertex temp; temp.idx = ix; temp.idy = iy;
				if (temp.idx == target.idx && temp.idy == target.idy) {
					end = 1; 
					isbfs = true;
					break;
				}
				Queue.push(temp);
			}
		}
		qpath[cnt++] = node; //목표지점까지 도달할때까지 거쳐가는 parent노드들 저장
		if (end == 1) break;
	}
	qpath[cnt++] = target;

	if (end)
		return true;
	else
		return false;
}

void ofApp::bfsdraw() {
	ofSetColor(ofColor::lightGreen);

	int i, j, k;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (visited[i][j] == 1) {
				for (k = 0; k < 4; k++) {
					int tx = 2 * i + 1 + gapx[k]; int ty = 2 * j + 1 + gapy[k];
					int x = i + gapx[k]; int y = j + gapy[k];
					if (x < 0 || x > HEIGHT - 1 || y < 0 || y > WIDTH - 1) continue;
					if (visited[x][y] == 1 && input[tx][ty] == ' ') {
						ofDrawLine(40 * j + 20, 40 * i + 20, 40 * y + 20, 40 * x + 20);  //경로를 탐색하면서 방문한 지점 모두 그리기 
					}
				}
			}
		}
	}
	ofSetColor(ofColor::darkGreen);
	int a = cnt - 1; int b = cnt - 2;
	while (1) {
		int ax = qpath[a].idx; int ay= qpath[a].idy;
		int bx = qpath[b].idx; int by = qpath[b].idy;
		int tx1 = 2 * ax + 1; int ty1 = 2 * ay + 1;
		int tx2 = 2 * bx + 1; int ty2 = 2 * by + 1;
		if (ax == 0 && ay == 0) break;  //경로를 다 그렸을 때 while문 종료
		if (((abs(ax - bx) == 0 && abs(ay - by) == 1) || (abs(ax - bx) == 1 && abs(ay - by) == 0)) && input[(tx1 + tx2) / 2][(ty1 + ty2) / 2] == ' ') {
			ofDrawLine(40 * qpath[a].idy + 20, 40 * qpath[a].idx + 20, 40 * qpath[b].idy + 20, 40 * qpath[b].idx + 20);
			a = b; b--; //qpath의 a인덱스의 값과 b인덱스의 값을 비교해서 각각의 인덱스 값들의 차이가 0또는 1일때와 서로 연결되어있을때만 line을 그린다
		}
		else
			b--; 
	}
}