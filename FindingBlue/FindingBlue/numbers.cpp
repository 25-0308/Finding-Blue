#include"numbers.h"
#include <iostream>
void NUMBER::init() {
    digits.reserve(20);

  //  for (int i = 0; i < 10; i++) {
  //      std::string path = "asset/numbers/N" + std::to_string(i) + ".obj";
  //      std::string tex = "asset/numbers/numbers.png";
  //      
  //      Object num(path.c_str(), tex.c_str());
		//num.init();
  //      num.scale = glm::vec3(1.0f);
  //      num.position = glm::vec3(0.0f, 0.0f, 0.0f);  
  //      num.rotation.y = glm::radians(270.0f);
  //      digits.push_back(num);
  //      //디버깅출력
		//std::cout << "버개" << digits.back().obj.vertexCount << std::endl;
		//
  //  }
	n0.init();
	n1.init();
	n2.init();
	n3.init();
	n4.init();
	n5.init();
	n6.init();
	n7.init();
	n8.init();
	n9.init();
	//푸시백
	digits.push_back(n0);
	digits.push_back(n1);
	digits.push_back(n2);
	digits.push_back(n3);
	digits.push_back(n4);
	digits.push_back(n5);
	digits.push_back(n6);
	digits.push_back(n7);
	digits.push_back(n8);
	digits.push_back(n9);



}

void NUMBER::draw_number(GLuint shader, int number, glm::vec3 startPos) {
    if (number < 0) number = 0;

    // 숫자를 문자열로 변환  자리수 분해
    std::string s = std::to_string(number);

    float offsetZ = 2.0f; // 숫자 간 간격 

    for (int i = 0; i < s.size(); i++) {
	
        int digit = s[i] - '0';

        Object& d = digits[digit];

        d.position = startPos + glm::vec3(0.0, 0.0f, i * offsetZ);
        d.rotation = glm::vec3(0.0f);
		d.rotation.y = glm::radians(270.0f);
        d.scale = glm::vec3(5.0f);

        d.draw(shader);
    }
}
