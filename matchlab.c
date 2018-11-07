/*Osama Kergaye
 * u0767339
 * cs 4400
 * Master Flatt*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int aMode(char * string){
	// printf("amodeDebug: %s \n", string);
	int eCount = 0;
	int vCount = 0;
	int decimalCount = 0;
	
	if(string[0] != 'e'){return -1;}

	int i;
	for(i = 0; string[i] != '_'; i++){
		if(string[i] == 'e'){
			eCount++; 
		}else{return -1;} 
		
	}
	if(eCount % 2 != 1){return -1;}
	if(string[i+1] == '_'){i++;i++;}else{return -1;}
	while(string[i] != '_'){
		if(string[i] == 'v'){vCount++;}
		i++;
	}
	if(vCount < 1 || vCount > 5 ){return -1;}
	if(string[i+1] == '_'){i++;i++;}else{return -1;}
	while(string[i] != '\0'){
		if(isdigit(string[i])){
			decimalCount++;
			if(string[i] != '0'){
			
			string[i] = ((string[i] - '0')-1) + '0';
		//	printf(" decrament: %c \n", string[i]);
			}
		}else{return -1;}
		i++;
	}
	
	if(decimalCount < 1 || decimalCount > 3 ){return -1;}


return 0;
}



int bMode(char * string){

	int dCount = 0;
	int upperCount = 0;
	int decimalCount = 0;
	int wCount = 0;

	int eCount = 0;
	
	if(string[0] != 'd' && string[1] != 'd'){return -1;}
	int i;
	for(i = 0; string[i] != ','; i++){
		if(string[i] == 'd'){
			dCount++; 
		}else{return -1;} 
	}

	if(dCount < 2 || dCount > 4 ){return -1;}
	
	if(string[i] != ','){return -1;}
	i++;
	int j = i;
	while(isupper(string[i])){
		if(string[i] == 'E'){eCount++;}
		upperCount++;
		i++;
	}
	if(upperCount % 2 != 1 ){return -1;}

	char X[upperCount];
	int damn;
	for(damn = 0; damn < upperCount;damn++){

		X[damn] = string[j];
		j++;

	}


	while(string[i] == 'w'){
		wCount++;
		i++;
	}


	if(wCount < 2){return -1;}

	if(string[i] != ','){return -1;}
	i++;

	while(isdigit(string[i])){
		
		decimalCount++;
		i++;
	}
	if(decimalCount < 1 || decimalCount > 3 ){return -1;}

	int it;
	while(string[i] != '\0'){
		for(it = 0; it < upperCount; it++){
		if(string[i] != X[it]){return -1;}
		i++;

		}
		
		

	}
	

	eCount += eCount*4;

return eCount;
}

int cMode(char * string){

	int dCount = 0;
	int decCount = 0;
	int xCount = 0;
	int upperCount = 0;
	int i = 0;
	int Dcount = 0;

	if(string[0] != 'd'){return -1;}

	while(string[i] != '_'){
		if(string[i] != 'd'){return -1;}

		dCount++;
		i++;

	}
	if(dCount % 2 != 1 ){return -1;}

	if(string[i] != '_' && string[i+1] != '_'){return -1;}
	i++;
	i++;

	int j = i;

	while(isdigit(string[i])){

		decCount++;
		i++;


	}

	if(decCount < 1 || decCount > 3){return -1;}


	char X[decCount];
	int damn;
	for(damn = 0; damn < decCount;damn++){

		X[damn] = string[j];
		j++;

	}



	if(string[i] != 'x'){return -1;}
	while(string[i] != '='){
		if(string[i] != 'x'){return -1;}
		xCount++;
		i++;

	}
	if(xCount % 2 != 1){return -1;}
	if(string[i] != '='){return -1;}
	i++;
	if(!(isupper(string[i]))){return -1;}

	while(isupper(string[i])){
		if(string[i] == 'D' && Dcount < 2){Dcount++;}
		upperCount++;
		i++;
	}

	if(!(isdigit(string[i]))){return -1;}

    int it = 1;
	while(string[i] != '\0'){

		if(string[i] != X[it]){return -1;}
		i++;
		it++;
		it++;
	}





	return Dcount;

}



/*maybe ask if two string args will be provided*/
int main(int argc, char *argv[]){
/*set up arg holders*/
	char a = '!';
	char b = '!';
	char c = '!';
	char t = '!';
	int error = 0;


	

	/*get mode, not knowing where args are.
 * Also grab the string I neede */
	int i;
	int z;
	for(z = 1; z < argc; z++){
		char* matchString;

		if(error == 2 && t == '!'){
			return 1;
		}
		if(argv[z][0] == '-'){
			if(argv[z][1] == 'a'){
				if(a == '!'){a = 'a';error++;}else{return 1;}
			}else if(argv[z][1] == 'b'){
				if(b == '!'){b = 'b';error++;}else{return 1;}
			}else if(argv[z][1] == 'c'){
				if(c == '!'){c = 'c';error++;}else{return 1;}
			}else if(argv[z][1] == 't'){
				if(t == '!'){t = 't';error++;}else{return 1;}
			}else{
				/*after the - flag no valid input was provided, time to bomb*/
				return 1;
			} 
			
		}else{
			matchString = argv[z];








	if(error == 0 || (error == 1 && t == 't')){
		a = 'a';
	}

	//	printf("test for input string: %s, Flag a: %c Flag b: %c Flag c: %c Flag t: %c \n\n", matchString, a, b, c, t);
	/*switch for inputs starts here*/
	int worked = -1;
	//char* some = malloc(strlen(matchString));
	//some = matchString;


	if(a == 'a'){

		worked = aMode(matchString);

		if(worked >= 0 && t == 't'){
		printf("%s\n",matchString);
		}
		
	}else if(b == 'b'){
		
		worked = bMode(matchString);
		char answer[(strlen(matchString)+worked)];
		int j = 0;
		i = 0;
		while(i < (strlen(matchString))){
			if(matchString[i] == 'E'){
				answer[j] = matchString[i];
				j++;
				answer[j] = 'F';
				j++;
				i++;
			}else{
			answer[j] = matchString[i];
			j++;
			i++;
			}
			
			
		}

		if(worked >= 0 && t == 't'){
		printf("%s\n",answer);
		}

	}else if(c == 'c'){

		worked = cMode(matchString);
		//printf("%d \n", worked);
		char answer[(strlen(matchString)-worked)];

		int j = 0;
		i = 0;

		int first = 0;
		int last = 0;
		
		while(i < (strlen(matchString))){
			if(matchString[i] == 'D' && first == 0){
				first = i;				
			}else if(matchString[i] == 'D'){last = i;}
			i++;
		}

	    j = 0;
		i = 0;

		while(i <= (strlen(matchString))){
			
			if(i == first && worked == 1){
				i++;
				continue;

			}else if(i == first && worked == 2){
			    i++;
				if(i == last){i++;continue;}
				continue;

			}else if(i == last && worked == 2){
				i++;
				continue;
			}
			
			answer[j] = matchString[i];
			i++;
			j++;
			
		}

		if(worked >= 0 && t == 't'){
			printf("%s\n",answer);	
		}
	}
		
	if(worked >= 0 && t == '!'){
		printf("yes\n");
	}else if(worked == -1 && t == 't'){
		printf("\n");
	}else if(worked == -1 && t == '!'){
		printf("no\n");
	}




















		}
	//	printf("each argument: %s \n", argv[i]);
	}



	//printf("debug: %s\n", matchString);


//free(some);

return 0;
}












