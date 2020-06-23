int asciiConverter(int num) {
	if(num == 48)
		return 0;
	else if(num == 49)
		return 1;
	else if(num == 50)
		return 2;

	else if(num == 51)
		return 3;

	else if(num == 52)
		return 4;

	else if(num == 53)
		return 5;

	else if(num == 54)
		return 6;

	else if(num == 55)
		return 7;
	else if(num == 56)
		return 8;
	else if(num == 57)
		return 9;

	else 
		return -1;


}

int charToNum(char ch) {
	if(ch == 'a')
		return 0;
	else if(ch == 'b')
		return 1;
	else if(ch == 'c')
		return 2;

	else if(ch == 'd')
		return 3;

	else if(ch == 'e')
		return 4;

	else if(ch == 'f')
		return 5;
	else if(ch == 'g')
		return 6;
	else if(ch == 'h')
		return 7;
	else 
		return -1;
}