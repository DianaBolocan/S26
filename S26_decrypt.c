#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int notIn(char* word,char trigrams[20][3]){
  for(int i=0;i<20;i++)
	if(strcmp(word,trigrams[i]) == 0)
		return 0;
  return 1;
}

void getBestFit(char** trigrams,int* freq,int frecv,char* word){
  int position = 19;
  //find best place
  while(freq[position] < frecv && position > 0){
	printf("for %s, %d < %d\n",word,freq[position],frecv);
	position--;
  }

  //shift the other freqs and trigrams
  for(int index = 19;index > position;index--){
	freq[index] = freq[index-1];
	memset(trigrams[index],0,3);
	strcpy(trigrams[index],trigrams[index-1]);
  }
  
  //place the new trigrams
  freq[position] = frecv;
  memset(trigrams[position],0,3);
  strcpy(trigrams[position],word);
  printf("on %d got %s with %d:\n",position,trigrams[position],freq[position]);
}

void setTrigramsFrequency(char* word,char** trigrams,int position,int* freq,FILE* text,int size){
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
	if(wordFreq > freq[19]){
		getBestFit(trigrams,freq,wordFreq,word);
	}
  }
}

void rewriteKey(char key[26],char** trigrams,int* freq){
  char usualTri[20][3] ={ "the", "and", "ing", "her", "hat", "his", "tha", "ere", "for", "ent", "ion", "ter", "was", "you", "ith", "ver", "all", "wit", "thi", "tio"};
  
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
  char key[26] = "";
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
  fseek(encryptedText,0,SEEK_SET);
  ch = fgetc(encryptedText);
  while(ch != EOF){
	fputc(key[ch - 65],decryptedText);
	ch = fgetc(encryptedText);
  }

  for(int i=0;i<26;i++){
	printf("%c",key[i]);
  }
  printf("\n");  

  char trigram[20][3];
  for(int i=0;i<19;i++)
	memset(trigram[i],0,3);
  int freq[20] = { 0 };
  int position = 0;
  fseek(encryptedText,position,SEEK_SET);
  while(position < 50){
	char* word;
	word = (char*)malloc(4);
	if(fgets(word,4,encryptedText) == NULL){
		printf("fgets() error.\n");
		return 0;
	}
	setTrigramsFrequency(word,trigram,position,freq,encryptedText,(int)textStatus.st_size);
	position++;
	fseek(encryptedText,position,SEEK_SET);
  }
  for(int i =0;i<20;i++)
	printf("%s ",trigram[i]);
  printf("\n");
  rewriteKey(key,trigram,freq);

  printf("\nSuccesfuly decrypted the text.\n");
  return 0;
}
