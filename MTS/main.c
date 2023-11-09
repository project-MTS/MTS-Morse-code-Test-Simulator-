#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>

#define SAFE_DELETE(ptr)                do { free(ptr); (ptr) = NULL; } while(0)
#define CAPPING(char_array)             strupr(char_array)
#define DELETE_SLASH_N(text_array)      text_array[strcspn(text_array, "\n")] = 0

#define WORD_MAX                        20

typedef unsigned int                    fileLine_t;

typedef struct {
    char text[WORD_MAX];
} wordText_t;
#define TEXT(wordText)                  (wordText).text

fileLine_t wordFile_line                = 1;

typedef enum
{
    Listen=1
}program_e;

typedef enum
{
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z, 
    No0, No1, No2, No3, No4,
    No5, No6, No7, No8, No9,
    MkPeriod, MkExclam, MkQuest, MkComma
}codeNum_e;

typedef struct code_t 
{
	char alphabet;
	const char* morseCode;
}code_t;

code_t codeList[40] = {
    { 'A', ".-"     }, { 'B', "-..."   }, { 'C', "-.-."   }, { 'D', "-.."    },
    { 'E', "."      }, { 'F', "..-."   }, { 'G', "--."    }, { 'H', "...."   },
    { 'I', ".."     }, { 'J', ".---"   }, { 'K', "-.-"    }, { 'L', ".-.."   },
    { 'M', "--"     }, { 'N', "-."     }, { 'O', "---"    }, { 'P', ".--."   },
    { 'Q', "--.-"   }, { 'R', ".-."    }, { 'S', "..."    }, { 'T', "-"      },
    { 'U', "..-"    }, { 'V', "...-"   }, { 'W', ".--"    }, { 'X', "-..-"   },
    { 'Y', "-.--"   }, { 'Z', "--.."   }, { '0', "-----"  }, { '1', ".----"  },
    { '2', "..---"  }, { '3', "...--"  }, { '4', "....-"  }, { '5', "....."  },
    { '6', "-...."  }, { '7', "--..."  }, { '8', "---.."  }, { '9', "----."  },
    { '.', ".-.-.-" }, { '!', "-.-.--" }, { '?', "..--.." }, { ',', "--..--" }
};

char* convertA2M(char alphabet)
{
    for (int x = 0; x < sizeof(codeList) / sizeof(code_t); x++)
    {
        if (alphabet == codeList[x].alphabet)
            return codeList[x].morseCode;
    }
    return " ";
}
char convertM2A(const char* morseCode)
{
    for (int x = 0; x < sizeof(codeList) / sizeof(code_t); x++)
    {
        if (strcmp(morseCode, codeList[x].morseCode))
            return codeList[x].alphabet;
    }
    return ' ';
}


#define TIME_NOW                        (unsigned int)time(NULL)
#define SHUFFLE_BIT_1                   0b10101010101010101010101010101010
#define SHUFFLE_BIT_2                   0b01010000100001000010000100001010

int randomDrop(unsigned short maxVal)
{
    srand((SHUFFLE_BIT_1 >> 1 ^ TIME_NOW << 8) >> 10 ^
        (SHUFFLE_BIT_2 << 1 & TIME_NOW >> 3) >> 10 ^
        (SHUFFLE_BIT_1 << 0 ^ TIME_NOW << 15) >> 3 ^
        (SHUFFLE_BIT_2 >> 1 & TIME_NOW << 3) << 10 ^
        (SHUFFLE_BIT_1 >> 1 ^ TIME_NOW >> 11) >> 1);
    return (rand() % maxVal + 1);
}


wordText_t getFileTexts(const char* fileName, fileLine_t fileLine)
{
    FILE* fp = fopen(fileName, "r");
    int counter = randomDrop(fileLine);
    //printf("%d: ", counter);
    wordText_t buffer;
    if (fp == NULL)
    {
        strcpy(TEXT(buffer), "ERROR");
        return buffer;
    }

    for (int x = 1; x <= fileLine; x++)
    {
        fgets(TEXT(buffer), sizeof(TEXT(buffer)), fp);
        if (counter <= x)
        {
            fclose(fp);
            DELETE_SLASH_N(TEXT(buffer));
            CAPPING(TEXT(buffer));
            return buffer;
        }
    }

    strcpy(TEXT(buffer), "ERROR");
    fclose(fp);
    return buffer;
}

fileLine_t getFileLine(const char* fileName)
{
    FILE* fp = fopen(fileName, "r");;
    char buffer[20];
    fileLine_t lines = 0;

    if (fp == NULL)
        return 1;

    while (fgets(buffer, sizeof(buffer), fp)) {
        lines++;
    }

    fclose(fp);
    return lines;
}

#define SND_FREQUENCY					440
#define SND_GAP							165
#define SND_SEP_CODE					26
#define SND_SEP_LETTER					450
#define SND_SEP_WORD					680
#define SND_BAR_RCV						{Beep(SND_FREQUENCY, SND_GAP * 3);	Sleep(SND_SEP_CODE / 4);}
#define SND_DOT_RCV						{Beep(SND_FREQUENCY, SND_GAP * 1);	Sleep(SND_SEP_CODE);}
#define SND_END_OF_LETTER				Sleep(SND_SEP_LETTER);
#define SND_END_OF_WORD					Sleep(SND_SEP_WORD);


void morseSound(const char* message) {
    for (int i = 0; *(message + i) != '\0'; i++) {
        const char* morseMessage = convertA2M(*(message + i));

        printf("%c: %s\n", *(message + i), morseMessage);

        for (int j = 0; *(morseMessage + j) != NULL; j++) {
            if (*(morseMessage + j) == '-')
                SND_BAR_RCV
            else if (*(morseMessage + j) == '.')
                SND_DOT_RCV
            else
                SND_END_OF_WORD
        }
        SND_END_OF_LETTER
    }
}


void runProgram1()
{
    printf("영문을 작성하세요\n");
    printf("만약 프로그램 선택창으로 돌아가고 싶다면 exit를 입력하세요\n");

    char text[] = "CQD CQD SOS SOS DE MGY POSITION 41.46N 50.14W";
    while (1)
    {
        printf(">");
        gets(text);
        if ((strcmp(CAPPING(text), "EXIT") == 0))
            return;
        morseSound(CAPPING(text));
    }
    return;
}


#define WORD_FILE_PATH                  "EnglishWords.data"
#define WORD_FILE_LINE                  wordFile_line

void init();
int main()
{
	int programCode = 0;
    init();
    
    printf("%s\n", TEXT(getFileTexts(WORD_FILE_PATH, WORD_FILE_LINE)));
    Sleep(1200);
    printf("%s\n", TEXT(getFileTexts(WORD_FILE_PATH, WORD_FILE_LINE)));
    Sleep(1200);
    printf("%s\n", TEXT(getFileTexts(WORD_FILE_PATH, WORD_FILE_LINE)));
    Sleep(1200);
    printf("%s\n", TEXT(getFileTexts(WORD_FILE_PATH, WORD_FILE_LINE)));
    Sleep(1200);
    printf("%s\n", TEXT(getFileTexts(WORD_FILE_PATH, WORD_FILE_LINE)));

ProgramStart_Point:
    /*system("cls");
	do
	{
		printf("1. 모스부호로 변환\n");
		printf("Insert Number>>");
		scanf("%d", &programCode);
		system("cls");
	} while (!(0 < programCode && programCode < 2));

    switch (programCode)
    {
    case Listen:
        runProgram1();
        break;
    }*/

	goto ProgramStart_Point;
}

void init()
{
    wordFile_line = getFileLine(WORD_FILE_PATH);
}











#define TT_1 200
#define TEST_MUSIC_1                    {Beep(262, TT_1);Sleep(TT_1);Beep(262, TT_1);Sleep(TT_1);Beep(247, TT_1*3);Beep(262, TT_1);Sleep(TT_1*2);Beep(294, TT_1*2);Beep(330, TT_1);Sleep(TT_1*3);\
                                        Beep(349, TT_1*2);Beep(330, TT_1*2);Sleep(TT_1*2);Beep(294, TT_1*4);Beep(262, TT_1*2);Beep(247, TT_1*4);}
#define TT2 250
#define TEST_MUSIC_2 {Beep(659, TT2*8); Beep(587, TT2*3);Beep(554, TT2*3);Beep(659, TT2*2);Beep(587, TT2*8);Beep(587, TT2*3);Beep(523, TT2*3);Beep(466, TT2*2);\
                     Beep(440, TT2*7);Beep(415, TT2*1);Beep(440, TT2*5);Beep(329, TT2*1);Beep(415, TT2*1);Beep(440, TT2*1);Beep(493, TT2*5);Beep(329, TT2*1);Beep(415, TT2*1);Beep(493, TT2*1);Beep(554, TT2*4);Beep(493, TT2*4);\
                     Beep(659, TT2*8);Beep(587, TT2*3);Beep(554, TT2*3);Beep(493, TT2*2);Beep(440, TT2*7);Beep(440, TT2/2);Beep(415, TT2/2);Beep(440, TT2*1.5);Beep(392, TT2*1.5);Beep(349, TT2*2);Beep(392, TT2*1);Beep(349, TT2*2);\
                     Beep(329, TT2*7);Beep(415, TT2*1);Beep(440, TT2*4);Beep(329, TT2*1.5);Beep(293, TT2*1.5);Beep(277, TT2*1);Beep(293, TT2*8);Beep(293, TT2*1.5);Beep(277, TT2*1.5);Beep(493/2, TT2*3);Beep(415/2, TT2*2);Beep(440/2, TT2*8);}