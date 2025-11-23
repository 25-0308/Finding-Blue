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
#include"Collision.h"

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
void MouseMove(int x, int y);

//내가 추가한 변수임
AK_47* rifle;
FIELD* field;
CLUB* club;
//플레이어
Player player;
Camera camera(player);
//적
ENEMY* enemy;


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
	field = new FIELD();
	field->init();
	enemy = new ENEMY();
	enemy->init();
	glutMouseFunc(mouseCallback);
	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);
	glutMotionFunc(MouseMove);
	glutPassiveMotionFunc(MouseMove);

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
	

	//플레이어==카메라
	glm::mat4 view = camera.getView();
	

	GLuint viewLoc = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	//투영행렬
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 150.0f);
	GLuint projLoc = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	

	//rifle->draw(shaderProgramID);
	club->draw(shaderProgramID);
	field->draw(shaderProgramID);


	//적
	enemy->draw(shaderProgramID);

	//Debug_Draw::Render();

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
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

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
	case'-':
		camera.sensitivity -= 5.0f;
		if (camera.sensitivity < 5.0f)
			camera.sensitivity = 5.0f;
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

	camera.updateDirection(dx, dy, deltaTime);

	glutWarpPointer(centerX, centerY);
}




void TimerFunction(int value)
{
	//델타타임 계산
	auto currentTime = std::chrono::high_resolution_clock::now();
	deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
	lastTime = currentTime;
	player.move(deltaTime);
	rifle->update(deltaTime, player.position,camera.yaw,camera.pitch);
	club->update(deltaTime, player.position, camera.yaw, camera.pitch);
	enemy->update();
	drawScene();

	glutTimerFunc(value, TimerFunction, value);
}




