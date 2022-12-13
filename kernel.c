//Name: Jonathan Quigley
//Worked with: Josh Sproul
void printString(char*);
void printChar(char);
void readString(char*);
void readSector();
void readFile();
void executeProgram();
void terminate();
void handleTimerInterrupt(int segment, int sp);
void handleInterrupt21();
void killProcess(char*);

int processActive[8];
int processSP[8];
int currentProcess;

void main(){
	int i;
	makeInterrupt21();
	for(i = 0; i < 8; i++){
		processActive[i] = 0;
		processSP[i] =0xff00;
	}
	currentProcess = -1;
	interrupt(0x21, 4, "shell", 0, 0);
	makeTimerInterrupt();
	while(1);

}

void printString(char* chars){

	int c = 0;
	while(chars[c] != 0x0){
		printChar(chars[c]);
		c++;
	}

}

void printChar(char c){

	c= interrupt(0x10, 0xe*256+c, 0, 0, 0);

}

void readString(char* line){

	char c;
	int n = 0;
	while(1){
		c = interrupt(0x16, 0, 0, 0, 0);
		if(c == 0xd){
			break;
		}
		if(c == 0x8){
			line[n--];
			printChar(0x8);
			printChar(' ');
			printChar(0x8);
		}
		else{
			line[n++] = c;
			printChar(c);
		}
	}
	line[n++] = 0xd;
	line[n++] = 0xa;
	line[n++] = 0x0;
	printChar(0xd);
	printChar(0xa);

}

void readSector(char* buffer, int sector){
/*
	int AH = 2;
	int AL = 1;
	int CH = 0;
	int CL = (sector +1);
	int DH = 0;
	int DL = 0x80;
*/
//optimized code with less integers (AL, AH, CL, CH, etc.)
	int AX = 2*256 + 1;
	int BX = buffer;
	int CX = 0*256 + (sector+1);
	int DX = 0*256 + (0x80);
	interrupt(0x13, AX, BX, CX, DX);

}

void readFile(char* name, char* buffer, int* sector){
	int e = 0;
	int i = 0;
	int j = 0;
	char dir[512];
	readSector(dir, 2);
	*sector = 0;
	for(e = 0; e < 512; e=e+32){
		for(i = 0; i < 6; i++){
			if(name[i] != dir[e + i]){
				break;
			}
			else if(name[i] == 0 && dir[e + i] == 0|| i ==5){
				while(dir[e + 6 + j] != 0){
					readSector(buffer, dir[e + 6 + j]);
					buffer = buffer + 512;
					*sector = *sector + 1;
					j++;
				}
				return;
			}
		}
	}
}

void executeProgram(char* name){
	int x = 0, y = 0, i = 0, seg = 0, sectors = 0;
	char buffer[13312];
	readFile(name, buffer, &sectors);
	if(sectors != 0){
		x = setKernelDataSegment();
		while(processActive[i] != 0){
			i++;
			if(i == 8){
				i = 0;
			}
		}
/*		for(i = 0; i < 8; i++){
			if(processActive[i] == 0){
				break;
			}
			else if(i == 8){
				i = 0;
			}
		}*/
		restoreDataSegment(x);
		seg = 0x1000*(i+2);
		for(y = 0; y<13312; y++){
			putInMemory(seg, y, buffer[y]);
		}
		initializeProgram(seg);
		x = setKernelDataSegment();
		processSP[i] = 0xff00;
		processActive[i] =1;
		restoreDataSegment(x);
	}
	else{
		printString("No file found");
	}

}

void killProcess(char* processNumber){
//i have ran out of ideas
/*	if(processNumber == '0')
		processActive[0] = 0;
	if(processNumber == '1')
		processActive[1] = 0;
	if(processNumber == '2') 
                processActive[2] = 0;
        if(processNumber == '3')
                processActive[3] = 0;
 	if(processNumber == '4') 
                processActive[4] = 0;
        if(processNumber == '5')
                processActive[5] = 0;
 	if(processNumber == '6') 
                processActive[6] = 0;
        if(processNumber == '7')
                processActive[7] = 0;
*/
	if(processNumber[0] == '0'){
		setKernelDataSegment();
		processActive[0] = 0;
		restoreDataSegment();
	}
	else if(processNumber[0] == '1'){
                setKernelDataSegment();
                processActive[1] = 0;
                restoreDataSegment();
        }
    	else if(processNumber[0] == '2'){
                setKernelDataSegment();
                processActive[2] = 0;
                restoreDataSegment();
        }
    	else if(processNumber[0] == '3'){
                setKernelDataSegment();
                processActive[3] = 0;
                restoreDataSegment();
        }
    	else if(processNumber[0] == '4'){
                setKernelDataSegment();
                processActive[4] = 0;
                restoreDataSegment();
        }
    	else if(processNumber[0] == '5'){
                setKernelDataSegment();
                processActive[5] = 0;
                restoreDataSegment();
        }
    	else if(processNumber[0] == '6'){
                setKernelDataSegment();
                processActive[6] = 0;
                restoreDataSegment();
        }
    	else if(processNumber[0] == '7'){
                setKernelDataSegment();
                processActive[7] = 0;
                restoreDataSegment();
        }

}

void terminate(){
/*	char shellname[6];
	shellname[0] = 's';
	shellname[1] = 'h';
	shellname[2] = 'e';
	shellname[3] = 'l';
	shellname[4] = 'l';
	shellname[5] = '\0';
	executeProgram(shellname);
	while(1);
*/
	setKernelDataSegment();
	processActive[currentProcess] = 0;
	restoreDataSegment();
	while(1);
}

void handleTimerInterrupt(int segment, int sp){

	/*printChar('T');
	printChar('i');
	printChar('c');*/
	int x, i;
	x =  setKernelDataSegment();
	for(i=0; i<8; i++)
        {
                putInMemory(0xb800,60*2+i*4,i+0x30);
                if(processActive[i]==1)
                        putInMemory(0xb800,60*2+i*4+1,0x20);
                else
                        putInMemory(0xb800,60*2+i*4+1,0);
        }
	if(currentProcess != -1){
		processSP[currentProcess] = sp;
	}
	currentProcess++;
	if(currentProcess == 8){
		currentProcess  = 0;
	}
	while(processActive[currentProcess] != 1){
		currentProcess++;
		if(currentProcess == 8){
			currentProcess = 0;
		}
	}
/*	for(i = currentProcess; i < 8; i++){
		if(processActive[i] == 1){
			break;
		}
		else if(i == 8){
			i = 0;
		}
	}*///lesson learned for-loops suck
	segment= (2+currentProcess)*0x1000;
	sp = processSP[currentProcess];
	restoreDataSegment(x);
	returnFromTimer(segment, sp);

}

void handleInterrupt21(int ax, int bx, int cx, int dx){

	//printString("Hello world");
	if(ax == 0){
		printString(bx);
	}
	else if(ax == 1){
		readString(bx);
	}
	else if(ax == 2){
		readSector(bx, cx);
	}
	else if(ax == 3){
		readFile(bx, cx, dx);
	}
	else if(ax == 4){
		executeProgram(bx);
	}
	else if(ax == 5){
		terminate();
	}
	else if(ax == 6){
		killProcess(bx);
	}
	else if(ax == 7){
		printChar(bx);
	}
	else{
		printString("Out of Bounds exception for interrupt");
	}

}









