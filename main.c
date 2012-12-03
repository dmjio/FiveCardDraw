#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> 
#include "poker.h"
#include "test.h"

int main(int argc, char const *argv[]){
	srand(time(NULL)); 
	return start_game(); 
}