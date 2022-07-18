#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>

#define ESC 27
#define ENTER 13
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77


int TOTAL_FILMS_COUNT = 0;


struct filmStruct
{
    char name[20];
    int length;
    int year;
    char style[20];
    char actor[20];
};


//Ожидание отклика
void waitForRequest();

//Добавить фильм
void addFilm(filmStruct *films);

//Сохранение и загрузка БД
void saveDataBase(filmStruct *films);
void loadDataBase(filmStruct *films);

//Глобальная обработка (Сортировка и поиск)
void commandTreatment(filmStruct *films, int type);

//Обработка поиска
void searchChooseTreatment(filmStruct *films, int type);

//Обработка сортировки
void sortChooseTreatment(filmStruct *films, int type);

//Показать все фильмы
void showAllFilms(filmStruct *films);

//Удаление фильмов
void deleteFilm(filmStruct *films);

//Перерисовка окна
void repaintFrame(char item[5][100], int currentType);


int main()
{

    SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
    char menu[][100] =
            {
                    "Добавить запись     ",
                    "Сохранить           ",
                    "Загрузить           ",
                    "Показать все записи ",
                    "Удалить запись      ",
                    "Сортировка          ",
                    "Поиск               ",
                    "Выход               ",

            };

    //Создаем структуру
    filmStruct films[100];

    int current = 0, code;

    cprintf("Выход в меню <ESC>");

    do
    {
        textbackground(BLACK);
        clrscr();
        textbackground(BLUE);
        textcolor(WHITE);

		for (int i = 0; i < 8; i++)
        {
			gotoxy(1, i + 1);
			cputs(menu[i]);
        }

        textbackground(WHITE);
        textcolor(BLUE);

        gotoxy(1, current + 1);
        cputs(menu[current]);

		code = getch();

		switch (code)
        {
            case UP:
				if (current > 0)
                    current--;
                else
                    current = 7;
                break;

            case DOWN:
                if (current < 7)
                    current++;
                else
                    current = 0;
                break;

            case ESC:
                return 0;

            case ENTER:
                switch (current)
                {
                    case 0:
                        addFilm(films);
                        break;

                    case 1:
                        saveDataBase(films);
                        break;

                    case 2:
                        loadDataBase(films);
                        break;

                    case 3:
                        showAllFilms(films);
                        break;

                    case 4:
                        deleteFilm(films);
                        break;

                    case 5:
                        commandTreatment(films, 1);
                        break;

                    case 6:
                        commandTreatment(films , 0);
                        break;

                    case 7:
                        return 0;
                }
        }

    }while (code != 27);
    return 0;
}


void waitForRequest()
{
    gotoxy(1,25);
    cputs("press any key");
	getch();
}


void addFilm(filmStruct *films)
{
	gotoxy(1,15);
    puts("");

	cprintf("Название, Продолжительность(в минутах), Год, Стиль, Актер :");
	gotoxy(1,17);
	scanf("%s%d%d%s%s", films[TOTAL_FILMS_COUNT].name, &films[TOTAL_FILMS_COUNT].length,
		  &films[TOTAL_FILMS_COUNT].year,films[TOTAL_FILMS_COUNT].style, films[TOTAL_FILMS_COUNT].actor);

    TOTAL_FILMS_COUNT++;

    waitForRequest();
}


void saveDataBase(filmStruct *films)
{
    FILE* file = fopen("DataBase.txt", "wb");
    fwrite(films, sizeof(filmStruct), TOTAL_FILMS_COUNT, file);
    fclose(file);

    waitForRequest();
}


void loadDataBase(filmStruct *films)
{
    gotoxy(1,15);

    FILE* file = fopen("DataBase.txt", "rb");
    TOTAL_FILMS_COUNT = fread(films, sizeof(filmStruct), 100, file);
    fclose(file);

    cprintf("Кол-во записей о фильмах : %i", TOTAL_FILMS_COUNT);

    waitForRequest();
}


void showAllFilms(filmStruct *films)
{
    if (TOTAL_FILMS_COUNT == 0)
    {
        gotoxy(1,15);
        cputs("Нет записей");
    }
    else
    {
        gotoxy(1,17);

		for (int i=0; i < TOTAL_FILMS_COUNT; i++)
		{
			gotoxy(1, i + 15);
			cprintf("%s %d %d %s %s\n", films[i].name, films[i].length, films[i].year,
                    films[i].style, films[i].actor);
        }
    }


    waitForRequest();
}


void deleteFilm(filmStruct *films)
{
    gotoxy(1,17);

    int current;

    cprintf("Введите номер по списку : ");
    scanf("%i", &current);

    if (current > TOTAL_FILMS_COUNT || current < 0)
    {
        cprintf("Нет такой записи в списке");
        waitForRequest();
        return;
    }
    else
    {
        for(int i = current ; i < TOTAL_FILMS_COUNT - 1 ; i++)
            films[i] = films[i+1];

        TOTAL_FILMS_COUNT--;
        showAllFilms(films);

        waitForRequest();
    }
}


void commandTreatment(filmStruct *films, int type)
{
    gotoxy(1,15);

    int currentType = 0, code;

    char item[5][100] =
            {
					"Название                 ",
					"Продолжительность (в мин)",
					"Год                      ",
					"Стиль                    ",
					"Актер                    ",
            };

    do
    {
        repaintFrame(item, currentType);

        code = getch();

        switch(code)
        {
            case UP:
                if (currentType > 0)
                    currentType--;
                else
                    currentType = 4;
                break;

            case DOWN:
                if (currentType < 4)
                    currentType++;
                else
                    currentType = 0;
                break;

            case ESC:
                return;

            case ENTER:
                if(type == 0)
                    searchChooseTreatment(films, currentType);
                else
                    sortChooseTreatment(films, currentType);
                break;
        }

    } while(code != 27);
}


void sortChooseTreatment(filmStruct *films, int type)
{
    if(type == 1 || type == 2)
    {
        int bufferInt;
        int nextBufferInt;


		for(int i = 0 ; i < TOTAL_FILMS_COUNT - 1 ; i++)
			for(int j = 0 ; j < TOTAL_FILMS_COUNT - 1 ; j++)
            {
                if(type == 1)
                {
                    bufferInt = films[j].length;
					nextBufferInt = films[j+1].length;

                }
                else
                {
                    bufferInt = films[j].year;
					nextBufferInt = films[j+1].year;
                }

                if(nextBufferInt < bufferInt)
                {
                    filmStruct newFilm = films[j+1];
                    films[j+1] = films[j];
                    films[j] = newFilm;
                }
            }
    }
    else
    {
        char buffer[20];
        char nextBuffer[20];

        for(int i = 0 ; i < TOTAL_FILMS_COUNT - 1 ; i++)
            for(int j = 0 ; j < TOTAL_FILMS_COUNT - 1 ; j++)
            {
                if(type == 0)
                {
                    strncpy(buffer, films[j].name, 20);
                    strncpy(nextBuffer, films[j+1].name, 20);
                }
                else if(type == 3)
                {
                    strncpy(buffer, films[j].style, 20);
                    strncpy(nextBuffer, films[j+1].style, 20);
                }
                else if(type == 4)
                {
                    strncpy(buffer, films[j].actor, 20);
                    strncpy(nextBuffer, films[j+1].actor, 20);
                }

                if(strcmp(nextBuffer, buffer) < 0)
                {
                    filmStruct newFilm = films[j+1];
                    films[j+1] = films[j];
                    films[j] = newFilm;
                }
            }

    }

    showAllFilms(films);

    waitForRequest();
}


void searchChooseTreatment(filmStruct *films, int type)
{
    gotoxy(1, 15);

	int index = -1;



	if(type == 1 || type == 2)
	{
		int userEnterInt;
		int bufferInt;

		scanf("%d", &userEnterInt);


		for (int i = 0; i < TOTAL_FILMS_COUNT; i++)
        {
			if(type == 1)
				bufferInt = films[i].length;
			else
				bufferInt = films[i].year;

            if (bufferInt == userEnterInt)
            {
                index = i;
                break;
            }
        }
	}
	else
	{
		char userEnter[20];
		char buffer[20];

		scanf("%s", userEnter);



		for (int i = 0; i < TOTAL_FILMS_COUNT; i++)
		{


			if(type == 0)
				strncpy(buffer, films[i].name, 20);
			else if(type == 3)
				strncpy(buffer, films[i].style, 20);
			else if(type == 4)
				strncpy(buffer, films[i].actor, 20);

			if (strcmp(buffer, userEnter) == 0)
			{
                index = i;
                break;
			}

        }
    }


    if (index == -1)
        cprintf("Не удалось найти");
	else
		cprintf("%i %s %d %d %s %s", index, films[index].name, films[index].length, films[index].year,
                films[index].style, films[index].actor);

    waitForRequest();
}


void repaintFrame(char item[5][100], int currentType)
{
    textbackground(BLACK);
    clrscr();
    textbackground(BLUE);
    textcolor(WHITE);

    for (int i = 0; i < 5; i++)
    {
        gotoxy(1, i + 1);
        cputs(item[i]);
    }

    textbackground(WHITE);
    textcolor(BLUE);

    gotoxy(1, currentType + 1);
    cputs(item[currentType]);
    gotoxy(1, 20);

    cprintf("Выход в меню <ESC>");
}
