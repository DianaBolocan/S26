#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
  char key[26] = "OPQRSTUVWXYZABCDEFGHIJKLMN";
  FILE* normalText = fopen("S26_text","r");
  if(normalText == NULL){
	printf("fopen() text error.\n");
	return 0;
  }
  FILE* encryptedText = fopen("S26_encryptedText","w"); 
  if(encryptedText == NULL){
	printf("fopen() encrypted text error.\n");
	return 0;
  }
  fseek(normalText,0,SEEK_SET);
  char ch;
  ch = fgetc(normalText);
  while(ch != EOF){
	if (ch >= 65 && ch <= 90){
		ch = key[ch - 65];
		fputc(ch,encryptedText);
	}
	else if (ch >= 97 && ch <= 122){
		ch = key[ch - 97];
		fputc(ch,encryptedText);
	}
	ch = fgetc(normalText);
  }
 printf("Succesfuly encrypted the text.\n");
 fseek(encryptedText,0,SEEK_SET);
 return 0; 
}
