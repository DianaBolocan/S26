#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int notIn(char* word,char** trigrams){
  for(int i=0;i<20;i++)
	if(strcmp(word,trigrams[i]) == 0)
		return 0;
  return 1;
}

void setTrigramsFrequency(char* word,char** trigrams,int position,int** freq,FILE* text,int size){
  int wordFreq = 0;
  
  //check number or appearances in text
  if(notIn(word,trigrams)){
	  while(position < size - 2){
		fseek(text,position,SEEK_SET);
		char* reference;
		reference = (char*)malloc(4);
		if(fgets(reference,4,text) == NULL){
			printf("fgets() error subpr\n");
			return;
		}
		//crapa la ultima trigrama
		if(strcmp(reference,word) == 0)
			wordFreq++;
		position++;
	  }

	//if freq is higher then rewrite the trigrams and freqs
	if(wordFreq > (*freq)[19]){
		int position = 19;
  //find best place
  while((*freq)[position] < wordFreq && position > 0){
	position--;
  }
  //shift the other freqs and trigrams
  for(int index = 19;index > position+1;index--){
	(*freq)[index] = (*freq)[index-1];
	free(trigrams[index]);
	trigrams[index] = (char*)malloc(4*sizeof(char));
	strcpy(trigrams[index],trigrams[index-1]);
  }
  
  //place the new trigrams
  (*freq)[position+1] = wordFreq;
  free(trigrams[position+1]);
  trigrams[position+1] = (char*)malloc(4*sizeof(char));
  strcpy(trigrams[position+1],word);
	}
  }
}

void rewriteKey(char key[26],char** trigrams,int** freq){
  char usualTri[3][3] ={ "THE", "AND", "ING"};
  int changed[26] = {0};
  int index = 0 ;
  for(int index = 0 ;index < 3; index++){
	//if(index == 0 || index == 1 || index == 8)
	for(int letter=0;letter<3;letter++)
		if(changed[usualTri[index][letter] - 65] == 0 ){
			key[trigrams[index+1][letter] - 65] = usualTri[index][letter];
			changed[usualTri[index][letter] - 65] = 1;
		}
  }
}

int getPosition(int frequencyText[26]){
  int max = 0;
  int position = 0;
  for(int i=0;i<26;i++)
	if(max < frequencyText[i]){
		max = frequencyText[i];
		position = i;
	}
  frequencyText[position] = 0;
  return position;
}

void getKey(int frequencyText[26], char statistics[26], char key[26]){
  int count = 0;
  int position = getPosition(frequencyText);
  while(count < 26){
	key[position] = statistics[count];
	position = getPosition(frequencyText);
	count++;
  }
}

int main(){
  struct stat textStatus;
  char frequency[26] = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
  char* key;
  key = (char*)malloc(26*sizeof(char));
  FILE *encryptedText = fopen("S26_encryptedText","r");
  if(encryptedText == NULL){
 	printf("fopen() encrypted text error.\n");
	return 0;
  }
  FILE *decryptedText = fopen("S26_decryptedText","w");
  if(decryptedText == NULL){
	printf("fopen() decrypted text error.\n");
	return 0;
  }
  if(stat("S26_encryptedText",&textStatus) == -1){
	printf("stat() error.\n");
	return 0;
  }
  int frequencyText[26] = { 0 };
  char ch;
  fseek(encryptedText,0,SEEK_SET);
  ch = fgetc(encryptedText);
  while(ch != EOF){
	frequencyText[ch - 65]++;
	ch = fgetc(encryptedText);
  }

  getKey(frequencyText,frequency,key);

  for(int i=0;i<26;i++){
	printf("%c",key[i]);
  }
  printf("\n");  

  char** trigram = (char**)malloc(20*sizeof(char*));
  for(int index = 0 ;index < 20; index++)
	trigram[index] = (char*)malloc(4*(sizeof(char)));
  int* freq = (int*)malloc(20*sizeof(int));
  for(int index=0;index<20;index++)
	freq[index] = 0;
  int position = 0;
  fseek(encryptedText,position,SEEK_SET);
  while(position < 1000){
	printf("%d out of 1000\n",position+1);
	char* word;
	word = (char*)malloc(4);
	if(fgets(word,4,encryptedText) == NULL){
		printf("fgets() error.\n");
		return 0;
	}
	setTrigramsFrequency(word,trigram,position,&freq,encryptedText,(int)textStatus.st_size);
	position++;
	fseek(encryptedText,position,SEEK_SET);
  }
  rewriteKey(key,trigram,&freq);
  for(int i=0;i<26;i++){
	printf("%c",key[i]);
  }
  printf("\n");
  fseek(encryptedText,0,SEEK_SET);
  ch = fgetc(encryptedText);
  while(ch != EOF){
	fputc(key[ch - 65],decryptedText);
	ch = fgetc(encryptedText);
  }
  printf("\nSuccesfuly decrypted the text.\n");
  return 0;
}
