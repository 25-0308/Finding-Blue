//--- �ʿ��� ������� ����
#define _CRT_SECURE_NO_WARNINGS 
#include "ModelLoader.h"
#include"ak_47.h"
#include"club.h"
#include"field.h"
#include"Enemy.h"
#include<iostream>
#include<chrono>
#include"player.h"
#include"camera.h"
#include"light.h"
#include"bullet.h"
#include"claymore.h"
#include"pistol.h"
#include"minigun.h"
#include"intro.h"
#include"item.h"
#include"button.h"
#include"firecannon.h"
#include"airplane.h"
#include"missile.h"
//--- �Ʒ� 5�� �Լ��� ����� ���� �Լ� ��
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);


//���� �߰��� �Լ���
void mouseCallback(int button, int state, int x, int y);
void initBuffer();
void TimerFunction(int value);
GLvoid KeyboardDown(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
void mouseWheel(int button, int dir, int x, int y);
void MouseMove(int x, int y);

//���� �߰��� ������
bool map_loaded = true;
AK_47* rifle;
FIELD* field;
std::vector<BUTTON*>buttons;

CLUB* club;
CLAYMORE* claymore;
PISTOL* pistol;
MINIGUN* minigun;
FIRECANNON* firecannon;
INTRO* intro;
int current_step = 0;
//�����۰���
std::vector<ITEM*> items;

//�Ѿ� ������ ����
std::vector<BULLET>* bullets = new std::vector<BULLET>();

//�÷��̾�
Player player;
Camera camera(player);

//�����
AIRPLANE* airplane;
//�̻���
std::vector<MISSILE*> missiles;

//��
std::vector<ENEMY>* enemies = new std::vector<ENEMY>();
glm::vec3 E_pos_list[11] = {
	{20.0,-0.7f,20.0f},
	{50.0,-0.7,20.f},
	{10.0f,-0.7f,40.0f},
	{10.0f,-0.7f,30.0f},
	{40.0f,-0.7f,30.0f},
	{40.0f,-0.7f,50.0f},
	{40.0f,-0.7f,40.0f},
	{40.0f,-0.7f,50.0f},
	{45.0f,-0.7f,45.0f},
	{45.0f,-0.7f,40.0f},
	{},

};
int item_list[] = { 0,0,1,2,2,2,2 }; //1:���� 2:�������� 3:ź��
glm::vec3 item_pos[7] = {
	{9.0,0.1f,10.0f },
	{ 10.0,0.1f,48.0f },
	{ 45.0,0.1f,45.0f },
	{ 14.0,0.1f,5.0f },
	{ 42.0,0.1f,42.0f },
	{ 50.0,0.1f,90.0f },
	{ 90.0,0.1f,90.0f },
};
glm::vec3 button_pos[2] = {
	{2.1,0.5f,8.0f},
	{55.3f,0.0f,50.0f}
};
float button_rot[2] = {
	glm::radians(180.0f),
	glm::radians(0.0f)
};
//���� �ϴ� �ϳ���
Lighting light1;


//��ŸŸ���� ���Ѱ͵�
auto lastTime = std::chrono::high_resolution_clock::now();
float deltaTime = 0.0f;



float obj_angle = 0.0f;
//--- �ʿ��� ���� ����
GLint width = 800, height = 800;
int centerX = width / 2;
int centerY = height / 2;
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}
//--- ���� �Լ�

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{

	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 0);
	glutInitWindowSize(width, height);
	//glutSetCursor(GLUT_CURSOR_NONE);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	//--- ���̴� �о�ͼ� ���̴� ���α׷� �����: ����� �����Լ� ȣ��
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	shaderProgramID = make_shaderProgram();
	initBuffer(); 
	glEnable(GL_DEPTH_TEST);
	rifle = new AK_47();
	rifle->init();
	club = new CLUB();
	club->init();
	claymore = new CLAYMORE();
	claymore->init();
	pistol = new PISTOL();
	pistol->init();
	minigun = new MINIGUN();
	minigun->init();
	firecannon = new FIRECANNON();
	firecannon->init();
	field = new FIELD();
	field->init();
	player.set_field(field); //�÷��̾� �ʵ� ���� ����
	//�ʵ� ���������ϱ� ��ư�� ����
	for (int i = 0;i < 2;++i) {
		BUTTON* button = new BUTTON();
		button->init(button_pos[i], button_rot[i]);
		buttons.push_back(button);
	}

	intro = new INTRO();
	intro->init();
	//������ �ʱ�ȭ
	for (int i = 0;i < 7;++i) {
		ITEM* item = new ITEM(ItemType(item_list[i]));
		item->init(item_pos[i]);
		items.push_back(item);
	}


	//�����
	airplane = new AIRPLANE();
	airplane->init();

	enemies->reserve(13);
	//�������ε�
	for (int i = 0;i < 10;++i) {
		enemies->emplace_back();                  // ���� �ȿ� ���� ����
		enemies->back().init(E_pos_list[i]);      // �ٷ� �ʱ�ȭ
	}
	
	//�����ʱ�ȭ

	light1.lightPos = glm::vec3(45.0f, 25.0f, 45.0f);
	//light1.lightPos = glm::vec3(10.0, 1.0f, 10.0);
	glutMouseFunc(mouseCallback);
	glutKeyboardFunc(KeyboardDown);	
	glutKeyboardUpFunc(KeyboardUp);
	glutMotionFunc(MouseMove);
	glutPassiveMotionFunc(MouseMove);
	glutMouseWheelFunc(mouseWheel);

	//--- ���̴� ���α׷� �����
	glutDisplayFunc(drawScene); //--- ��� �ݹ� �Լ�
	//glutIdleFunc(drawScene);
	glutTimerFunc(1, TimerFunction, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	


}
//--- ���ؽ� ���̴� ��ü �����
void make_vertexShaders()
{
	GLchar* vertexSource;
	//--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
	//--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}
//--- �����׸�Ʈ ���̴� ��ü �����
void make_fragmentShaders()
{
	GLchar* fragmentSource;
	//--- �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�
	fragmentSource = filetobuf("fragment.glsl"); // �����׼��̴� �о����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}
//--- ���̴� ���α׷� ����� ���̴� ��ü ��ũ�ϱ�
GLuint make_shaderProgram()
{
	GLint result;
	GLchar* errorLog = NULL;
	GLuint shaderID;
	shaderID = glCreateProgram(); //--- ���̴� ���α׷� �����
	glAttachShader(shaderID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(shaderID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
	glLinkProgram(shaderID); //--- ���̴� ���α׷� ��ũ�ϱ�
	glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
	glDeleteShader(fragmentShader);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(shaderID); //--- ������� ���̴� ���α׷� ����ϱ�
	//--- ���� ���� ���̴����α׷� ���� �� �ְ�, �� �� �Ѱ��� ���α׷��� ����Ϸ���
	//--- glUseProgram �Լ��� ȣ���Ͽ� ��� �� Ư�� ���α׷��� �����Ѵ�.
	//--- ����ϱ� ������ ȣ���� �� �ִ�.
	return shaderID;
}

//--- ��� �ݹ� �Լ�
GLvoid drawScene() {
	//glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);


	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	light1.apply(shaderProgramID);

	//��������
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform1f(glGetUniformLocation(shaderProgramID, "alpha"), 1.0f);
	light1.apply(shaderProgramID);
	//�÷��̾�==ī�޶�
	glm::mat4 view = camera.getView();
	if (current_step == 0) {
		
		view = glm::lookAt(
			glm::vec3(0.0f, 1.0f, 5.0f), //ī�޶� ��ġ
			glm::vec3(0.0f, 1.0f, 0.0f), //ī�޶� �ٶ󺸴� ��ġ
			glm::vec3(0.0f, 1.0f, 0.0f)  //ī�޶��� ������
		);
	}
	//���⿡ ����� �ٶ󺸰��ϴ°� �߰�
	if (airplane->get_is_active()) {
		if (airplane->get_airplane_state() == 0 && current_step != 0) {
			//�ü��� pos ����� ��
			glm::vec3 a_pos_b = airplane->airplane.position;
			a_pos_b.z = 30.0f;
			view = glm::lookAt(
				a_pos_b,//ī�޶� ��ġ
				airplane->airplane.position, //ī�޶� �ٶ󺸴� ��ġ

				glm::vec3(0.0f, 1.0f, 0.0f)  //ī�޶��� ������
			);
		}
		else if (airplane->get_airplane_state() == 1 && current_step != 0) {
			view = glm::lookAt(
				player.position,//ī�޶� ��ġ
				airplane->airplane.position, //ī�޶� �ٶ󺸴� ��ġ

				glm::vec3(0.0f, 1.0f, 0.0f)  //ī�޶��� ������
			);
		}
	}


	GLuint viewLoc = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	//�������
	glm::mat4 projection = glm::perspective(glm::radians(player.FOV), (float)width / (float)height, 0.1f, 150.0f);
	GLuint projLoc = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//��������
	light1.apply(shaderProgramID);
	if (current_step != 0) {

		if (!rifle->get_is_get())
			rifle->draw(shaderProgramID);
		if (!club->get_is_get())
			club->draw(shaderProgramID);
		if (!claymore->get_is_get())
			claymore->draw(shaderProgramID);
		if (!pistol->get_is_get())
			pistol->draw(shaderProgramID);
		if (!minigun->get_is_get())
			minigun->draw(shaderProgramID);
		if (!firecannon->get_is_get())
			firecannon->draw(shaderProgramID);
		//�÷��̾� ���� �׸���
		


		//������ �׸���
		for (auto& it : items) {
			if (!it->get_is_get())
				it->draw(shaderProgramID);
		}
	}

	if (map_loaded) {
		field->draw(shaderProgramID);
		//��ư�׸���
		
	}
	for (auto& b : buttons) {
		b->draw(shaderProgramID);
	}
	//��
	for (auto& e : *enemies) {
		e.draw(shaderProgramID);
	}


	if (current_step == 0)
	intro->draw(shaderProgramID);

	//�����
	airplane->draw(shaderProgramID);
	if (!missiles.empty()) {
		for (auto& m : missiles) {
			m->draw(shaderProgramID);
		}
	}


	player.draw_weapon(shaderProgramID);
	glm::mat4 MVP = glm::mat4(1.0);
	glUniformMatrix4fv(
		glGetUniformLocation(shaderProgramID, "modelTransform"),
		1, GL_FALSE,
		glm::value_ptr(MVP)
	);
	
	Debug_Draw::Render();

	glutSwapBuffers();
}
//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	width = w;      
	height = h;
	glViewport(0, 0, w, h);
}
void initBuffer() {
	
}


void mouseCallback(int button, int state, int x, int y) {
	float mx = (2.0f * x) / width - 1.0f;
	float my = 1.0f - (2.0f * y) / height;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		glutPostRedisplay(); // �ٽ� �׸��� ��û
		player.mouses[0] = true;
		
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		player.mouses[0] = false;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//�ܸ��
		player.zoom_mode = true;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		player.zoom_mode = false;
	}
}
GLvoid KeyboardDown(unsigned char key, int x, int y) {
	switch (key) {

	case'w':
		player.keys[key] = true;
		break;
	case'a':
		player.keys[key] = true;
		break;
	case's':
		player.keys[key] = true;
		break;
	case'd':
		player.keys[key] = true;
		break;
	case'+':
		camera.sensitivity += 5.0f;
		break;
	case'f':
		if (rifle->get_weapon(player.position)) {
			player.weapons.push_back(rifle);
			player.change_weapon(player.weapons.size() - 1);
		}
		if (club->get_weapon(player.position)) {
			player.weapons.push_back(club);
			player.change_weapon(player.weapons.size() - 1);
		}
		if (claymore->get_weapon(player.position)) {
			player.weapons.push_back(claymore);
			player.change_weapon(player.weapons.size() - 1);
		}
		if (pistol->get_weapon(player.position)) {
			player.weapons.push_back(pistol);
			player.change_weapon(player.weapons.size() - 1);
		}
		if (minigun->get_weapon(player.position)) {
			player.weapons.push_back(minigun);
			player.change_weapon(player.weapons.size() - 1);
		}
		if (firecannon->get_weapon(player.position)) {
			player.weapons.push_back(firecannon);
			player.change_weapon(player.weapons.size() - 1);
		}
		//���⼭���� �����۰� �浹ó��
		for (auto& it : items) {
			if (it->get_item(player.position)) {
				//���⼭ ���� ȿ��

			}
		}
		break;
	case'-':
		camera.sensitivity -= 5.0f;
		if (camera.sensitivity < 5.0f)
			camera.sensitivity = 5.0f;
		break;
	case'm':
		map_loaded = !map_loaded;
		break;
	case'x':
		if (current_step == 0) {
			current_step = 1;
			glutWarpPointer(centerX, centerY);
		}
		break;
	case' ':

		player.jump();
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}
GLvoid KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case'w':
		player.keys[key] = false;
		break;
	case'a':
		player.keys[key] = false;
		break;
	case's':
		player.keys[key] = false;
		break;
	case'd':
		player.keys[key] = false;
		break;

	}
	glutPostRedisplay();
}
void MouseMove(int x, int y) {
	int dx = x - centerX;
	int dy = y - centerY;
	if (current_step == 0)
		return;
	camera.updateDirection(dx, dy, deltaTime);

	glutWarpPointer(centerX, centerY);
}




void TimerFunction(int value)
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
	lastTime = currentTime;
	if (current_step == 0) {
		intro->update(deltaTime);
	}
	else if (current_step != 0) {
		//��ŸŸ�� ���
		
		player.move(deltaTime);
		rifle->update(deltaTime, player.position, camera.yaw, camera.pitch);
		club->update(deltaTime, player.position, camera.yaw, camera.pitch);
		claymore->update(deltaTime, player.position, camera.yaw, camera.pitch);
		pistol->update(deltaTime, player.position, camera.yaw, camera.pitch);
		minigun->update(deltaTime, player.position, camera.yaw, camera.pitch);
		firecannon->update(deltaTime, player.position, camera.yaw, camera.pitch);
		player.zoom_in(deltaTime);
		if (player.mouses[0] && !player.weapons.empty()) {

			//���� �ѱ���� �ݵ�
			if (rifle == (player.weapons[player.currentWeapon])) {
				player.weapons[player.currentWeapon]->attack(deltaTime);
				camera.pitch += (rand() % 100 / 100.0f) * 40.0f * deltaTime; //�¿�ణ��鸲
			}
			if (firecannon == (player.weapons[player.currentWeapon])) {
				player.weapons[player.currentWeapon]->attack(deltaTime);
			
			}
			//��� �������� �ٽ� ���ƿ;��ϴϱ�
			if (club == (player.weapons[player.currentWeapon])) {
				player.weapons[player.currentWeapon]->on_attak = true;
			}
			if (claymore == (player.weapons[player.currentWeapon])) {
				player.weapons[player.currentWeapon]->on_attak = true;
			}
			if (minigun == (player.weapons[player.currentWeapon]))
			{
				player.weapons[player.currentWeapon]->attack(deltaTime);
				camera.pitch += ((rand() % 100 / 100.0f) - 0.5f) * 80.0f * deltaTime; //����
				camera.yaw += ((rand() % 100 / 100.0f) - 0.5f) * 80.0f * deltaTime; //�¿�
			}
			//���ѵ� �ܹ߽����� �����
			if (pistol == (player.weapons[player.currentWeapon]))
			{
				if (pistol->get_shoot_cooldown() <= 0.0f) {
					player.weapons[player.currentWeapon]->on_attak = true;
					camera.pitch += (rand() % 100 / 100.0f) * 200.0f * deltaTime; //�¿�ణ��鸲
					//pistol->set_shoot_cooldown(1.0f);
				}
			}


		}
		if (!player.weapons.empty() && (club == (player.weapons[player.currentWeapon]) || claymore == (player.weapons[player.currentWeapon]) || pistol == (player.weapons[player.currentWeapon])))
		{
			if (player.weapons[player.currentWeapon]->on_attak)
				player.weapons[player.currentWeapon]->attack(deltaTime);
			if (pistol == (player.weapons[player.currentWeapon]))
				pistol->set_shoot_cooldown(pistol->get_shoot_cooldown() - deltaTime);

		}
		if (!airplane->get_is_active()) {
			for (auto& e : *enemies) {
				if (e.update(deltaTime, player.position)) {
					//�� ����Ϸ�
					e.~ENEMY();
				}
			}
		}
		for (auto& enemy : *enemies) {
			// AK-47 �Ѿ� �˻�
			if (rifle && rifle->get_is_get()) {
				for (size_t i = 0; i < rifle->bullets.size(); ++i) {
					BULLET* bullet = rifle->bullets[i];
					if (enemy.collision.check_collision(bullet->collision)) {
						std::cout << "ENEMY과 AK-47 BULLET 충돌!" << std::endl;
						enemy.take_damage(10);
						delete bullet;
						rifle->bullets.erase(rifle->bullets.begin() + i);
						break;
					}
				}
			}
			//pistol �Ѿ� �˻�
			if (pistol && pistol->get_is_get()) {
				for (size_t i = 0; i < pistol->bullets.size(); ++i) {
					BULLET* bullet = pistol->bullets[i];
					if (enemy.collision.check_collision(bullet->collision)) {
						std::cout << "ENEMY과 Pistol BULLET 충돌" << std::endl;
						enemy.take_damage(40);
						delete bullet;
						pistol->bullets.erase(pistol->bullets.begin() + i);
						break;
					}
				}
			}
			//minigun �Ѿ� �˻�
			if (minigun && minigun->get_is_get()) {
				for (size_t i = 0; i < minigun->bullets.size(); ++i) {
					BULLET* bullet = minigun->bullets[i];
					if (enemy.collision.check_collision(bullet->collision)) {
						std::cout << "ENEMY과 MINIGUN BULLET 충돌!" << std::endl;
						enemy.take_damage(10);
						delete bullet;
						minigun->bullets.erase(minigun->bullets.begin() + i);
						break;
					}
				}
			}
			if (firecannon && firecannon->get_is_get()) {
				for (size_t i = 0; i < firecannon->fires.size(); ++i) {
					FIRE* fire = firecannon->fires[i];
					if (enemy.collision.check_collision(fire->collision)) {
						std::cout << "ENEMY과 FIRECANNON FIRE 충돌!" << std::endl;
						enemy.take_damage(50);
						delete fire;
						firecannon->fires.erase(firecannon->fires.begin() + i);
						break;
					}
				}
			}
			// Ŭ�� ��Ʈ �ݶ��̴� �˻�
			if (club && club->get_is_get() && club->hit_active) {
				if (enemy.collision.check_collision(club->collision)) {
					std::cout << "ENEMY과 CLUB HIT 충돌!" << std::endl;
					enemy.hit(player.position);
					enemy.take_damage(35);
					club->hit_active = false;  // ������ ����
					break;
				}
			}
			if (claymore && claymore->get_is_get() && claymore->hit_active) {
				if (enemy.collision.check_collision(claymore->collision)) {
					std::cout << "ENEMY과 CLUB HIT 충돌!" << std::endl;
					enemy.hit(player.position);
					enemy.take_damage(35);
					claymore->hit_active = false;  // ������ ����
					break;
				}
			}
			if (club && club->get_is_get() && club->hit_active) {
				for (int i = 0;i < buttons.size();++i) {
					if (buttons[i]->collision.check_collision(club->collision)) {
						std::cout << "��ư�� CLUB HIT �浹!" << std::endl;
						club->hit_active = false;  // ������ ����
						buttons[i]->set_is_hit(true);

						break;
					}
				}
			}
		}

	}
	//����� ���� ������Ʈ
	if (current_step != 0) {
		airplane->update(deltaTime);
		airplane->is_player_in_range(player.position);
		if (airplane->missile_ready(deltaTime)) {
			//�̻��� ����
			MISSILE* missile = new MISSILE();
			missile->init(airplane->get_position());
			missiles.push_back(missile);
			std::cout << "�̻��� �߻�!" << std::endl;
		}
		//�̻��� ������Ʈ
		if (!missiles.empty()) {
			for (auto& m : missiles)
				if(m->update(deltaTime)){
					//����
					m->~MISSILE();
					delete m;
					missiles.erase(missiles.begin());

				}
		}
	}
	//button and button walls
	for (int i = 0;i < buttons.size();++i) {
		if (buttons[i]->get_is_hit()) {
			field->update(deltaTime, i);
		}
	}


	drawScene();

	glutTimerFunc(value, TimerFunction, value);
}




void mouseWheel(int button, int dir, int x, int y) {
	if (dir > 0) {
		//�ٿø�
		player.change_weapon((player.currentWeapon + 1) % player.weapons.size());
	}
	else {
		//�ٳ���
		int new_index = player.currentWeapon - 1;
		if (new_index < 0)
			new_index = player.weapons.size() - 1;
		player.change_weapon(new_index);
	}
}