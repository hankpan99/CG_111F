#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#include<time.h>
#include<vector>
using namespace std;

struct PosInfo{
	float r, x, y;
	float dx, dy;
};

struct Image_info{
	GLint ImageWidth, ImageHeight, PixelLength;
	GLubyte* PixelData; 
};

// game info
int cur_w = 1920, cur_h = 1080;
int game_step_cnt = 0;
int invincible_cnt = 300;
int coin_threshold = 200, monster_threshold = 200, shield_threshold = 450, health_threshold = 600;
int kill_monster_cnt = 0;
float speedup_ratio = 1.0f;

// player info
float player_tl[2] = {1.0f, -3.0f}, player_tr[2] = {2.0f, -3.0f}, 
	  player_dr[2] = {2.0f, -3.1f}, player_dl[2] = {1.0f, -3.1f};
float player_move = 0.0f;
bool invincible_flag = false;

// coin info
int add_coin = 0;
vector<PosInfo> coininfo_vec;
Image_info coin_img;

// monster info
int add_monster = 0;
Image_info monster_img;
vector<PosInfo> monsterinfo_vec;

// health info
int add_health = 1;
Image_info health_img;
vector<PosInfo> healthinfo_vec;

// shield info
int add_shield = 1;
Image_info shield_img;
vector<PosInfo> shieldinfo_vec;

// display info
int display_score = 0;
int display_life = 3;
int display_level = 1;

void ReadBmpFile(char* filename, Image_info* image_info){
	FILE* pFile = fopen(filename, "rb");
	if(pFile == 0)
		exit(0);

	fseek(pFile, 0x0012, SEEK_SET);
	fread(&image_info->ImageWidth, sizeof(image_info->ImageWidth), 1, pFile);
	fread(&image_info->ImageHeight, sizeof(image_info->ImageHeight), 1, pFile);

	image_info->PixelLength = image_info->ImageWidth * 3;
	while(image_info->PixelLength % 4 != 0)
		++image_info->PixelLength;
	image_info->PixelLength *= image_info->ImageHeight;

	image_info->PixelData = (GLubyte *) malloc(image_info->PixelLength);
	if(image_info->PixelData == 0)
		exit(0);

	fseek(pFile, 54, SEEK_SET);
	fread(image_info->PixelData, image_info->PixelLength, 1, pFile);
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

// render function
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
	vector<PosInfo> valid_coin_vec;

	for(int i = 0; i < coininfo_vec.size(); i++){
		coininfo_vec[i].y -= (0.02 * speedup_ratio);

		if(coininfo_vec[i].y > -4)
			valid_coin_vec.push_back(coininfo_vec[i]);
	}

	coininfo_vec = valid_coin_vec;
}

void collision_check(vector<PosInfo> *vec, int mode){
	vector<PosInfo> uncolli_vec;

	for(int i = 0; i < (*vec).size(); i++){
		if(	((*vec)[i].x + (*vec)[i].r > player_tl[0]) && ((*vec)[i].x - (*vec)[i].r < player_tr[0]) && 
			((*vec)[i].y - (*vec)[i].r < player_tr[1]) && ((*vec)[i].y + (*vec)[i].r > player_dr[1])){
			
			if(mode == 0){
				display_score += 100;
			}
			else if(mode == 1){
				if(invincible_flag){
					display_score += 500;
					kill_monster_cnt++;
				}
				else{
					display_score -= 200;
					display_life--;
				}
			}
			else if(mode == 2){
				display_score += 200;
				display_life++;
			}
			else if(mode == 3){
				invincible_flag = true;
				invincible_cnt = 300;
				continue;
			}

			if(display_life <= 0){
				printf("---------- Game over ----------\n\n");
				printf("Your score = %d\n\n", display_score);
				printf("Level you achieve = %d\n\n", display_level);
				printf("Monster you kill = %d\n\n", kill_monster_cnt);
				printf("-------------------------------\n");
				exit(0);
			}
		}
		else
			uncolli_vec.push_back((*vec)[i]);
	}

	(*vec) = uncolli_vec;
}

void draw_coin(){
	update_coin_pos();

	collision_check(&coininfo_vec, 0);

	for(int i = 0; i < coininfo_vec.size(); i++){
		// draw_circle(coininfo_vec[i].r, coininfo_vec[i].x, coininfo_vec[i].y);
		glPixelZoom((float) cur_w / 1920.0, (float) cur_h / 1080.0);
		glRasterPos2f(coininfo_vec[i].x - coininfo_vec[i].r, coininfo_vec[i].y - coininfo_vec[i].r);
		glDrawPixels(coin_img.ImageWidth, coin_img.ImageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, coin_img.PixelData);
	}

	if(!(add_coin % (int) (coin_threshold / (2 * speedup_ratio)))){
		add_coin = 0;
		PosInfo new_coin;
		new_coin.r = 0.35f, new_coin.x = 14 * (rand() / (float) RAND_MAX) - 7, new_coin.y = 3;
		coininfo_vec.push_back(new_coin);
		// draw_circle(new_coin.r, new_coin.x, new_coin.y);
		glPixelZoom((float) cur_w / 1920.0, (float) cur_h / 1080.0);
		glRasterPos2f(new_coin.x - new_coin.r, new_coin.y - new_coin.r);
		glDrawPixels(coin_img.ImageWidth, coin_img.ImageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, coin_img.PixelData);
	}
	
	add_coin++;
}

void update_monster_pos(){
	vector<PosInfo> valid_monster_vec;

	for(int i = 0; i < monsterinfo_vec.size(); i++){
		monsterinfo_vec[i].x += (monsterinfo_vec[i].dx * speedup_ratio), monsterinfo_vec[i].y += (monsterinfo_vec[i].dy * speedup_ratio);

		if((monsterinfo_vec[i].y > -7) && (monsterinfo_vec[i].y < 7) && (monsterinfo_vec[i].y > -4) && (monsterinfo_vec[i].y < 4))
			valid_monster_vec.push_back(monsterinfo_vec[i]);
	}

	monsterinfo_vec = valid_monster_vec;
}

void draw_monster(){
	update_monster_pos();

	collision_check(&monsterinfo_vec, 1);

	for(int i = 0; i < monsterinfo_vec.size(); i++){
		glPixelZoom((float) cur_w / 1920.0, (float) cur_h / 1080.0);
		glRasterPos2f(monsterinfo_vec[i].x - monsterinfo_vec[i].r, monsterinfo_vec[i].y - monsterinfo_vec[i].r);
		glDrawPixels(monster_img.ImageWidth, monster_img.ImageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, monster_img.PixelData);
	}

	if(!(add_monster % (int) (monster_threshold / (speedup_ratio * speedup_ratio * speedup_ratio)))){
		add_monster = 0;
		PosInfo new_monster;
		new_monster.r = 0.35f, new_monster.x = 14 * (rand() / (float) RAND_MAX) - 7, new_monster.y = 6 * (rand() / (float) RAND_MAX) - 3;
		new_monster.dx = (rand() / (float) RAND_MAX) / 10 - 0.05, new_monster.dy = (rand() / (float) RAND_MAX) / 10 - 0.05;  
		monsterinfo_vec.push_back(new_monster);
		glPixelZoom((float) cur_w / 1920.0, (float) cur_h / 1080.0);
		glRasterPos2f(new_monster.x - new_monster.r, new_monster.y - new_monster.r);
		glDrawPixels(monster_img.ImageWidth, monster_img.ImageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, monster_img.PixelData);
	}
	
	add_monster++;
}

void update_health_pos(){
	vector<PosInfo> valid_health_vec;

	for(int i = 0; i < healthinfo_vec.size(); i++){
		healthinfo_vec[i].y -= (0.05  * speedup_ratio);

		if(healthinfo_vec[i].y > -4)
			valid_health_vec.push_back(healthinfo_vec[i]);
	}

	healthinfo_vec = valid_health_vec;
}

void draw_health(){
	update_health_pos();

	collision_check(&healthinfo_vec, 2);

	for(int i = 0; i < healthinfo_vec.size(); i++){
		glPixelZoom((float) cur_w / 1920.0, (float) cur_h / 1080.0);
		glRasterPos2f(healthinfo_vec[i].x - healthinfo_vec[i].r, healthinfo_vec[i].y - healthinfo_vec[i].r);
		glDrawPixels(health_img.ImageWidth, health_img.ImageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, health_img.PixelData);
	}

	if(!(add_health % (int) (health_threshold / (2 * speedup_ratio)))){
		add_health = 0;
		PosInfo new_health;
		new_health.r = 0.35f, new_health.x = 14 * (rand() / (float) RAND_MAX) - 7, new_health.y = 3;
		healthinfo_vec.push_back(new_health);
		glPixelZoom((float) cur_w / 1920.0, (float) cur_h / 1080.0);
		glRasterPos2f(new_health.x - new_health.r, new_health.y - new_health.r);
		glDrawPixels(health_img.ImageWidth, health_img.ImageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, health_img.PixelData);
	}

	add_health++;
}

void update_shield_pos(){
	vector<PosInfo> valid_shield_vec;

	for(int i = 0; i < shieldinfo_vec.size(); i++){
		shieldinfo_vec[i].y -= (0.05  * speedup_ratio);

		if(shieldinfo_vec[i].y > -4)
			valid_shield_vec.push_back(shieldinfo_vec[i]);
	}

	shieldinfo_vec = valid_shield_vec;
}

void draw_shield(){
	update_shield_pos();

	collision_check(&shieldinfo_vec, 3);

	for(int i = 0; i < shieldinfo_vec.size(); i++){
		glPixelZoom((float) cur_w / 1920.0, (float) cur_h / 1080.0);
		glRasterPos2f(shieldinfo_vec[i].x - shieldinfo_vec[i].r, shieldinfo_vec[i].y - shieldinfo_vec[i].r);
		glDrawPixels(shield_img.ImageWidth, shield_img.ImageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, shield_img.PixelData);
	}

	if(!(add_shield % (int) (shield_threshold / (2 * speedup_ratio)))){
		add_shield = 0;
		PosInfo new_shield;
		new_shield.r = 0.35f, new_shield.x = 14 * (rand() / (float) RAND_MAX) - 7, new_shield.y = 3;
		shieldinfo_vec.push_back(new_shield);
		glPixelZoom((float) cur_w / 1920.0, (float) cur_h / 1080.0);
		glRasterPos2f(new_shield.x - new_shield.r, new_shield.y - new_shield.r);
		glDrawPixels(shield_img.ImageWidth, shield_img.ImageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, shield_img.PixelData);
	}

	add_shield++;
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

	// draw monster
	draw_monster();

	// draw health
	draw_health();

	// draw shield
	draw_shield();

	// draw catcher
	draw_player();

	// draw level
	draw_text(-7, 3.5, GLUT_BITMAP_TIMES_ROMAN_24, (char *) "Level", display_level);

	// draw score
	draw_text(-7, 3, GLUT_BITMAP_TIMES_ROMAN_24, (char *) "Score", display_score);

	// draw life
	draw_text(-7, 2.5, GLUT_BITMAP_TIMES_ROMAN_24, (char *) "Remain Health", display_life);

	// invinsible or not
	if(invincible_flag){
		draw_text(-7, 2, GLUT_BITMAP_TIMES_ROMAN_24, (char *) "Invincible Remain", invincible_cnt);
		invincible_cnt--;

		if(invincible_cnt == 0)
			invincible_flag = false;
	}

	game_step_cnt++;
	if(!(game_step_cnt % 1000)){
		game_step_cnt = 0;
		display_level++;
		speedup_ratio += 0.1;
	}

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

	if(key == 97){
		if(player_tl[0] - (0.3 * speedup_ratio) < -7.5)
			return;
	
		player_tl[0] -= (0.3 * speedup_ratio), player_tr[0] -= (0.3 * speedup_ratio), player_dl[0] -= (0.3 * speedup_ratio), player_dr[0] -= (0.3 * speedup_ratio);
	}
	else if(key == 100){
		if(player_tr[0] + (0.3 * speedup_ratio) > 7.5)
			return;

		player_tl[0] += (0.3 * speedup_ratio), player_tr[0] += (0.3 * speedup_ratio), player_dl[0] += (0.3 * speedup_ratio), player_dr[0] += (0.3 * speedup_ratio);
	}
	if(key == 27)
		exit(0);
}

int main(int argc, char **argv) {

	srand(time(NULL));

	ReadBmpFile((char *) "./texture_file/coin.bmp", &coin_img);
	ReadBmpFile((char *) "./texture_file/monster.bmp", &monster_img);
	ReadBmpFile((char *) "./texture_file/health.bmp", &health_img);
	ReadBmpFile((char *) "./texture_file/shield.bmp", &shield_img);

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