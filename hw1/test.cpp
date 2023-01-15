#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#include<time.h>
#include<vector>
using namespace std;

struct CoinXY{
	float r, x, y;
};

// windows info
int cur_w = 1920, cur_h = 1080;

// player info
float player_tl[2] = {1.0f, -3.0f}, player_tr[2] = {2.0f, -3.0f}, 
	  player_dr[2] = {2.0f, -3.1f}, player_dl[2] = {1.0f, -3.1f};
float player_move = 0.0f;

// coin info
int add_coin = 0;
vector<CoinXY> coininfo_vec;
static GLint ImageWidth, ImageHeight, PixelLength;
static GLubyte* PixelData; 

// display info
int display_score = 0;
int display_life = 3;

void ReadBmpFile(char* filename){
	FILE* pFile = fopen(filename, "rb");
	if(pFile == 0)
		exit(0);

	fseek(pFile, 0x0012, SEEK_SET);
	fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
	fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);

	PixelLength = ImageWidth * 3;
	while(PixelLength % 4 != 0)
		++PixelLength;
	PixelLength *= ImageHeight;

	PixelData = (GLubyte *) malloc(PixelLength);
	if(PixelData == 0)
		exit(0);

	fseek(pFile, 54, SEEK_SET);
	fread(PixelData, PixelLength, 1, pFile);
	fclose(pFile);
}

void changeSize(int w, int h) {

	cur_w = w, cur_h = h;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;

    // Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

    // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void draw_player(){
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
		glVertex2f(player_tl[0], player_tl[1]);
		glVertex2f(player_tr[0], player_tr[1]);
		glVertex2f(player_dr[0], player_dr[1]);
		glVertex2f(player_dl[0], player_dl[1]);
	glEnd();
}

void draw_circle(float r, float x, float y) {
	float i = 0.0f;
	
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y);
		for(i = 0.0f; i <= 360; i++)
			glVertex2f(r*cos(M_PI * i / 180.0) + x, r*sin(M_PI * i / 180.0) + y);
	glEnd();
}

void update_coin_pos(){
	vector<CoinXY> valid_coin_vec;

	for(int i = 0; i < coininfo_vec.size(); i++){
		coininfo_vec[i].y -= 0.02;

		if(coininfo_vec[i].y > -4)
			valid_coin_vec.push_back(coininfo_vec[i]);
	}

	coininfo_vec = valid_coin_vec;
}

void collision_check(){
	vector<CoinXY> uncolli_coin_vec;

	for(int i = 0; i < coininfo_vec.size(); i++){
		// printf("------- i = %d -------\n", i);
		// printf("r = %f, x = %f, y = %f\n", coininfo_vec[i].r, coininfo_vec[i].x, coininfo_vec[i].y);
		// printf("tl = %f, %f\n", player_tl[0], player_tl[1]);
		// printf("tr = %f, %f\n", player_tr[0], player_tr[1]);
		// printf("dr = %f, %f\n", player_dr[0], player_dr[1]);
		// printf("dl = %f, %f\n", player_dl[0], player_dl[1]);
		// printf("coininfo_vec[i].x + coininfo_vec[i].r > player_tl[0] = %d\n", coininfo_vec[i].x + coininfo_vec[i].r > player_tl[0]);
		// printf("coininfo_vec[i].x - coininfo_vec[i].r < player_tr[0] = %d\n", coininfo_vec[i].x - coininfo_vec[i].r < player_tr[0]);
		// printf("coininfo_vec[i].y - coininfo_vec[i].r < player_tl[1] = %d\n", coininfo_vec[i].y - coininfo_vec[i].r < player_tl[1]);
		// printf("coininfo_vec[i].y + coininfo_vec[i].r > player_dl[1] = %d\n", coininfo_vec[i].y + coininfo_vec[i].r > player_dl[1]);
		// printf("\n");
		if(	(coininfo_vec[i].x + coininfo_vec[i].r > player_tl[0]) && 
			(coininfo_vec[i].x - coininfo_vec[i].r < player_tr[0]) && 
			(coininfo_vec[i].y - coininfo_vec[i].r < player_tr[1]) &&
			(coininfo_vec[i].y + coininfo_vec[i].r > player_dr[1])){
			display_score += 100;
		}
		else
			uncolli_coin_vec.push_back(coininfo_vec[i]);
	}

	coininfo_vec = uncolli_coin_vec;
}

void draw_coin(){
	update_coin_pos();

	collision_check();

	for(int i = 0; i < coininfo_vec.size(); i++){
		// draw_circle(coininfo_vec[i].r, coininfo_vec[i].x, coininfo_vec[i].y);
		glPixelZoom((float) cur_w / 1920.0, (float) cur_h / 1080.0);
		glRasterPos2f(coininfo_vec[i].x - coininfo_vec[i].r, coininfo_vec[i].y - coininfo_vec[i].r);
		glDrawPixels(ImageWidth, ImageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, PixelData);
	}

	if(!(add_coin % 100)){
		add_coin = 0;
		CoinXY new_coin;
		new_coin.r = 0.35f, new_coin.x = 14 * (rand() / (float) RAND_MAX) - 7, new_coin.y = 3;
		coininfo_vec.push_back(new_coin);
		// draw_circle(new_coin.r, new_coin.x, new_coin.y);
		glPixelZoom((float) cur_w / 1920.0, (float) cur_h / 1080.0);
		glRasterPos2f(new_coin.x - new_coin.r, new_coin.y - new_coin.r);
		glDrawPixels(ImageWidth, ImageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, PixelData);
	}
	
	add_coin++;
}

void draw_text(float x, float y, void *font, char *string, int outinfo){
	char buffer[30];
	snprintf(buffer, 29, "%s = %d\n", string, outinfo);

	glRasterPos2f(x, y);
	for(char c = 0; buffer[c] != '\0'; c++)
		glutBitmapCharacter(font, buffer[c]);
}

void renderScene(void) {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Set the camera
	gluLookAt(	0.0f, 0.0f, 10.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);

	// draw coin
	draw_coin();

	// draw catcher
	draw_player();

	// draw score
	draw_text(-7, 3.5, GLUT_BITMAP_TIMES_ROMAN_24, (char *)"Score", display_score);

	// draw life
	draw_text(-7, 3, GLUT_BITMAP_TIMES_ROMAN_24, (char *)"Remain Health", display_life);

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

	if(key == 97)
		player_tl[0] -= 0.3, player_tr[0] -= 0.3, player_dl[0] -= 0.3, player_dr[0] -= 0.3;
	else if(key == 100)
		player_tl[0] += 0.3, player_tr[0] += 0.3, player_dl[0] += 0.3, player_dr[0] += 0.3;

	if(key == 27)
		exit(0);
}

int main(int argc, char **argv) {

	srand(time(NULL));

	ReadBmpFile((char *) "./texture_file/coin_2.bmp");
	ReadBmpFile((char *) "./texture_file/monster.bmp");

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1920, 1080);
	glutCreateWindow("Coin Catcher");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// here are the new entries
	glutKeyboardFunc(processNormalKeys);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}