#include <iostream>
#include <time.h>


using namespace std;

int main(){
	srand(time(NULL));
	int answer = rand() % 10 + 1;
	int guess = 0;
	while (guess != answer){
		cout << "Enter an integer between 1 and 10: ";
		cin >> guess;
		if (guess < answer)
			cout <<"Higher\n\n";
		else if (guess > answer)
			cout <<"Lower\n\n";
		else
			cout <<"Congratulations! You guessed the correct number!";
	}
	return 0;
}