#include <stdio.h>

int main(void) {
	while (1) {
		int c = getchar();
		if (c == -1) {
			break;
		}
		printf("0x%02x, ", c);
	}
	return 0;
}