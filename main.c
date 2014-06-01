#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> 
#include <time.h> 
#include "game.h"

int main(int argc, char const *argv[]){
  srand(time(NULL)); 
  return start_game(); 
}
