#include <iostream>
#include <Windows.h>
#include <ctime>
#include <random>
#include <string>

class player {
public:
	player(std::string type) : inputType(type) {}
	void reset() {
		three[2] = three[1] = three[0] = -1;
		two[1] = two[0] = -1;
		one = -1;
		lastPos = 0;
		a0 = mult / 100;
		a1 = (mult - a0 * 100) / 10;
		a2 = mult - a0 * 100 - a1 * 10;
	}
	
	int readInput() {
		if (inputType == "keyboard") {
			for (int i = 0; i <= 9; i++) {
				if (GetKeyState(i + 0x30) & 0x8000) return i;
			}
		}
		else if (inputType == "numpad") {
			int key = 0x60;
			for (int i = 0; i <= 9; i++) {
				if (GetKeyState(key + i) & 0x8000) {
					return i;
				}
			}
		}
		return -1;
	}
	bool check() {
		if (tripleSize == 3) {
			if (three[0] == a0 && three[1] == a1 && three[2] == a2 ||
				three[1] == a0 && three[2] == a1 && three[0] == a2 ||
				three[2] == a0 && three[0] == a1 && three[1] == a2)
			{
				return 1;
			}
		} else if(tripleSize == 2) {
			if (two[0] == a1 && two[1] == a2 ||
				two[1] == a1 && two[0] == a2) {
				return 1;
			}
		}
		else if (one == a2) return 1;
		return 0;
	}
	void setInput(int data) {
		if (tripleSize == 3) {
			three[lastPos++] = data;
			if (lastPos == 3) lastPos = 0;
		} else if(tripleSize == 2) {
			two[lastPos++] = data;
			if (lastPos == 2) lastPos = 0;
		} else {
			one = data;
		}
	}
	static int tripleSize;
	static void setAnswer(int _mult) {
		mult = _mult;
		if (mult > 99) {
			tripleSize = 3;
		}
		else if (mult > 9) {
			tripleSize = 2;
		}
		else tripleSize = 1;

	}
	int score = 0;
private:
	static int mult;
	int a0 = -1, a1 = -1, a2 = -1;
	size_t lastPos = 0;

	int one = -1;
	int two[2] = { -1, -1 };
	int three[3] = { -1, -1, -1 };
	std::string inputType;
};

const int min = 1, max = 9;
int player::mult = 0;
int player::tripleSize = -1;

int main() {

	int a, b, mult;
	bool shouldEnd = false;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distrib(min, max);

	player A("keyboard");
	player B("numpad");

	int lastA = -1, lastB = -1;
	while (!shouldEnd) {
		a = distrib(gen);
		b = distrib(gen);
		mult = a * b;
		player::setAnswer(mult);
		std::cout << std::endl << a << " * " << b << "=";
		int right = false;
		A.reset();
		B.reset();
		while (!right) {
			if (GetKeyState(VK_ESCAPE) & 0x8000) {
				shouldEnd = true;
				break;
			};
			if (GetKeyState('S') & 0x8000) {
				std::cout << std::endl << "Score: " << A.score << " : " << B.score << std::endl;
			}

			int inA = A.readInput();
			int inB = B.readInput();
			if (inA != -1 && inA != lastA) {
				A.setInput(inA);
				if(A.check()) {
					A.score++;
					right = true;
					std::cout << "keyboard scores\nAnswer is " << mult << std::endl;
				}
			}
			if(inB != -1 && inB != lastB) {
				B.setInput(inB);
				if (B.check()) {
					B.score++;
					right = true;
					std::cout << "numpad scores\nAnswer is " << mult << std::endl;
				}
			}
			lastA = inA;
			lastB = inB;
		}
	}
	std::cout << "scores:\n" << A.score << ",\n" << B.score << std::endl;
	return 0;
}