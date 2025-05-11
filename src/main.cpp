#include "ordered.h"
#include "unordered.h"

int main() {
	OrderedTable<int, std::string> ordered;
	ordered.insert(5, "five");
	ordered.insert(2, "two");
	ordered.insert(8, "eight");

	auto it_or = ordered.begin();
	for (size_t i = 0; i < ordered.size(); i++) {
		std::cout << it_or.value() << " ";  // �������: 2 5 8
		it_or++;
	}
	std::cout << std::endl;



	UnOrderedTable<int, std::string> unordered;
	unordered.insert(5, "five");
	unordered.insert(2, "two");
	unordered.insert(8, "eight");

	auto it_un = unordered.begin();
	for (size_t i = 0; i < unordered.size(); i++) {
		std::cout << it_un.value() << " ";  // ������� ����� ����: 5 2 8 ��� 8 5 2 � ��� ��������
		it_un++;
	}

	return 1;
}