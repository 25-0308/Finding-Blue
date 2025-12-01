//--- 필요한 헤더파일 선언
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
//--- 아래 5개 함수는 사용자 정의 함수 임
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);


//내가 추가한 함수임
void mouseCallback(int button, int state, int x, int y);
void initBuffer();
void TimerFunction(int value);
GLvoid KeyboardDown(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
void mouseWheel(int button, int dir, int x, int y);
void MouseMove(int x, int y);

//내가 추가한 변수임
bool map_loaded = true;
AK_47* rifle;
FIELD* field;
std::vector<BUTTON*>buttons;

CLUB* club;
CLAYMORE* claymore;
PISTOL* pistol;
MINIGUN* minigun;
INTRO* intro;
int current_step = 0;
//아이템관련
std::vector<ITEM*> items;

//총알 저장할 벡터
std::vector<BULLET>* bullets = new std::vector<BULLET>();

//플레이어
Player player;
Camera camera(player);
//적
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
int item_list[] = { 0,0,1,2,2,2,2 }; //1:고기 2:썩은고기 3:탄약
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
	{75.0f,0.0f,75.0f}
};
float button_rot[2] = {
	glm::radians(180.0f),
	glm::radians(0.0f)
};
//조명 일단 하나만
Lighting light1;


//델타타임을 위한것들
auto lastTime = std::chrono::high_resolution_clock::now();
float deltaTime = 0.0f;



float obj_angle = 0.0f;
//--- 필요한 변수 선언
GLint width = 800, height = 800;
int centerX = width / 2;
int centerY = height / 2;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
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
//--- 메인 함수

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{

	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 0);
	glutInitWindowSize(width, height);
	//glutSetCursor(GLUT_CURSOR_NONE);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	//--- 세이더 읽어와서 세이더 프로그램 만들기: 사용자 정의함수 호출
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
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
	field = new FIELD();
	field->init();
	player.set_field(field); //플레이어 필드 참조 설정
	//필드 생성했으니까 버튼도 생성
	for (int i = 0;i < 1;++i) {
		BUTTON* button = new BUTTON();
		button->init(button_pos[i], button_rot[i]);
		buttons.push_back(button);
	}

	intro = new INTRO();
	intro->init();
	//아이템 초기화
	for (int i = 0;i < 7;++i) {
		ITEM* item = new ITEM(ItemType(item_list[i]));
		item->init(item_pos[i]);
		items.push_back(item);
	}

	enemies->reserve(20);
	//적생성인데
	for (int i = 0;i < 10;++i) {
		enemies->emplace_back();                  // 벡터 안에 직접 생성
		enemies->back().init(E_pos_list[i]);      // 바로 초기화
	}
	
	//조명초기화
	light1.lightPos = glm::vec3(45.0f, 25.0f, 45.0f);
	//light1.lightPos = glm::vec3(10.0, 1.0f, 10.0);
	glutMouseFunc(mouseCallback);
	glutKeyboardFunc(KeyboardDown);	
	glutKeyboardUpFunc(KeyboardUp);
	glutMotionFunc(MouseMove);
	glutPassiveMotionFunc(MouseMove);
	glutMouseWheelFunc(mouseWheel);

	//--- 세이더 프로그램 만들기
	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
	//glutIdleFunc(drawScene);
	glutTimerFunc(1, TimerFunction, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	


}
//--- 버텍스 세이더 객체 만들기
void make_vertexShaders()
{
	GLchar* vertexSource;
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
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
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
//--- 프래그먼트 세이더 객체 만들기
void make_fragmentShaders()
{
	GLchar* fragmentSource;
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
//--- 세이더 프로그램 만들고 세이더 객체 링크하기
GLuint make_shaderProgram()
{
	GLint result;
	GLchar* errorLog = NULL;
	GLuint shaderID;
	shaderID = glCreateProgram(); //--- 세이더 프로그램 만들기
	glAttachShader(shaderID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(shaderID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(shaderID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(shaderID); //--- 만들어진 세이더 프로그램 사용하기
	//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
	//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
	//--- 사용하기 직전에 호출할 수 있다.
	return shaderID;
}

//--- 출력 콜백 함수
GLvoid drawScene() {
	//glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	light1.apply(shaderProgramID);

	//플레이어==카메라
	glm::mat4 view = camera.getView();
	if (current_step == 0) {
		
		view = glm::lookAt(
			glm::vec3(0.0f, 1.0f, 5.0f), //카메라 위치
			glm::vec3(0.0f, 1.0f, 0.0f), //카메라가 바라보는 위치
			glm::vec3(0.0f, 1.0f, 0.0f)  //카메라의 업벡터
		);
	}

	GLuint viewLoc = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	//투영행렬
	glm::mat4 projection = glm::perspective(glm::radians(player.FOV), (float)width / (float)height, 0.1f, 150.0f);
	GLuint projLoc = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	

	//조명적용
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
		//플레이어 무기 그리기
		player.draw_weapon(shaderProgramID);


		//아이템 그리기
		for (auto& it : items) {
			if (!it->get_is_get())
				it->draw(shaderProgramID);
		}
	}

	if (map_loaded) {
		field->draw(shaderProgramID);
		//버튼그리기
		
	}
	for (auto& b : buttons) {
		b->draw(shaderProgramID);
	}
	//적
	for (auto& e : *enemies) {
		e.draw(shaderProgramID);
	}

	glm::mat4 MVP = glm::mat4(1.0);
	glUniformMatrix4fv(
		glGetUniformLocation(shaderProgramID, "modelTransform"),
		1, GL_FALSE,
		glm::value_ptr(MVP)
	);
	if (current_step == 0)
	intro->draw(shaderProgramID);

	Debug_Draw::Render();

	glutSwapBuffers();
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
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

		glutPostRedisplay(); // 다시 그리기 요청
		player.mouses[0] = true;
		
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		player.mouses[0] = false;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//줌모드
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
		//여기서부턴 아이템과 충돌처리
		for (auto& it : items) {
			if (it->get_item(player.position)) {
				//여기서 이제 효과

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
		//델타타임 계산
		
		player.move(deltaTime);
		rifle->update(deltaTime, player.position, camera.yaw, camera.pitch);
		club->update(deltaTime, player.position, camera.yaw, camera.pitch);
		claymore->update(deltaTime, player.position, camera.yaw, camera.pitch);
		pistol->update(deltaTime, player.position, camera.yaw, camera.pitch);
		minigun->update(deltaTime, player.position, camera.yaw, camera.pitch);
		player.zoom_in(deltaTime);
		if (player.mouses[0] && !player.weapons.empty()) {

			//만약 총기류면 반동
			if (rifle == (player.weapons[player.currentWeapon])) {
				player.weapons[player.currentWeapon]->attack(deltaTime);
				camera.pitch += (rand() % 100 / 100.0f) * 40.0f * deltaTime; //좌우약간흔들림
			}
			//곤봉 근접류면 다시 돌아와야하니까
			if (club == (player.weapons[player.currentWeapon])) {
				player.weapons[player.currentWeapon]->on_attak = true;
			}
			if (claymore == (player.weapons[player.currentWeapon])) {
				player.weapons[player.currentWeapon]->on_attak = true;
			}
			if (minigun == (player.weapons[player.currentWeapon]))
			{
				player.weapons[player.currentWeapon]->attack(deltaTime);
				camera.pitch += ((rand() % 100 / 100.0f) - 0.5f) * 80.0f * deltaTime; //상하
				camera.yaw += ((rand() % 100 / 100.0f) - 0.5f) * 80.0f * deltaTime; //좌우
			}
			//권총도 단발식으로 만들거
			if (pistol == (player.weapons[player.currentWeapon]))
			{
				if (pistol->get_shoot_cooldown() <= 0.0f) {
					player.weapons[player.currentWeapon]->on_attak = true;
					camera.pitch += (rand() % 100 / 100.0f) * 200.0f * deltaTime; //좌우약간흔들림
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
		for (auto& e : *enemies) {
			if (e.update(deltaTime, player.position)) {
				//적 사망완료
				e.~ENEMY();
			}
		}
		for (auto& enemy : *enemies) {
			// AK-47 총알 검사
			if (rifle && rifle->get_is_get()) {
				for (size_t i = 0; i < rifle->bullets.size(); ++i) {
					BULLET* bullet = rifle->bullets[i];
					if (enemy.collision.check_collision(bullet->collision)) {
						std::cout << "ENEMY와 AK-47 BULLET 충돌!" << std::endl;
						enemy.take_damage(10);
						delete bullet;
						rifle->bullets.erase(rifle->bullets.begin() + i);
						break;
					}
				}
			}
			//pistol 총알 검사
			if (pistol && pistol->get_is_get()) {
				for (size_t i = 0; i < pistol->bullets.size(); ++i) {
					BULLET* bullet = pistol->bullets[i];
					if (enemy.collision.check_collision(bullet->collision)) {
						std::cout << "ENEMY와 Pistol BULLET 충돌!" << std::endl;
						enemy.take_damage(40);
						delete bullet;
						pistol->bullets.erase(pistol->bullets.begin() + i);
						break;
					}
				}
			}
			//minigun 총알 검사
			if (minigun && minigun->get_is_get()) {
				for (size_t i = 0; i < minigun->bullets.size(); ++i) {
					BULLET* bullet = minigun->bullets[i];
					if (enemy.collision.check_collision(bullet->collision)) {
						std::cout << "ENEMY와 MINIGUN BULLET 충돌!" << std::endl;
						enemy.take_damage(10);
						delete bullet;
						minigun->bullets.erase(minigun->bullets.begin() + i);
						break;
					}
				}
			}
			// 클럽 히트 콜라이더 검사
			if (club && club->get_is_get() && club->hit_active) {
				if (enemy.collision.check_collision(club->collision)) {
					std::cout << "ENEMY와 CLUB HIT 충돌!" << std::endl;
					enemy.hit(player.position);
					enemy.take_damage(35);
					club->hit_active = false;  // 때리면 중지
					break;
				}
			}
			if (claymore && claymore->get_is_get() && claymore->hit_active) {
				if (enemy.collision.check_collision(claymore->collision)) {
					std::cout << "ENEMY와 CLUB HIT 충돌!" << std::endl;
					enemy.hit(player.position);
					enemy.take_damage(35);
					claymore->hit_active = false;  // 때리면 중지
					break;
				}
			}
			if (club && club->get_is_get() && club->hit_active) {
				if (buttons[0]->collision.check_collision(club->collision)) {
					std::cout << "버튼과 CLUB HIT 충돌!" << std::endl;
					club->hit_active = false;  // 때리면 중지
					field->walls[field->opening_walls_idx[0]].position.y = -10.0f; //벽 내리기
					field->collisions[3].center.y = -10.0f; //벽 충돌처리 내리기
					break;
				}
			}
		}

	}
	drawScene();

	glutTimerFunc(value, TimerFunction, value);
}




void mouseWheel(int button, int dir, int x, int y) {
	if (dir > 0) {
		//휠올림
		player.change_weapon((player.currentWeapon + 1) % player.weapons.size());
	}
	else {
		//휠내림
		int new_index = player.currentWeapon - 1;
		if (new_index < 0)
			new_index = player.weapons.size() - 1;
		player.change_weapon(new_index);
	}
}