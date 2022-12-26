# Поиск пользователей из списка с помощью динамической библиотеки

Программа предназначена для поиска всех пользователей с указанной фамилией из списка, который представлен в виде файла.

## Начало работы

Следуя инструкциям описанным ниже, вы сможете установить на свой компьютер программу и протестировать ее функционал. Также, до установки вы можете посмотреть код программы и разобраться в том, как она работает.
Сначала проверьте соответствует ли компьютер условиям установки, затем, следуя инструкции, установите программу.
Скопируйте ссылку репозитория, чтобы использовать ее при клонировании на этапе установки.

### Необходимые условия

Для тестирования программы необходимо иметь на компьютере среду разработки 'Visual Studio'.

### Установка

Для установки программы необходимо проделать следующие действия:

+ Открыть 'Visual Studio' и выбрать пункт 'Клонирование репозитория';
+ В открывшемся окне вставить ссылку на репозиторий;
+ Нажать на кнопку 'Клонировать';
+ Дождать клонирования репозитория и начать работу.

### Работа программы

Программа производит поиск следующим образом:
+ В главном методе программы происходит подключение DLL-библиотеки и объявление массива структуры для считанных данных:
```
HINSTANCE hMyDLL;
if ((hMyDLL = LoadLibrary(PATH)) == NULL) return 1;
forReadData myFun = (forReadData)GetProcAddress(hMyDLL, "readData");
struct users* user = calloc(5000, sizeof(struct users));
myFun(user);
```
+ Весь функционал описан в DLL-библиотеке
	+ Самым первым выполняемым методом в библиотеке является считываение данных из файла
		+ Сначала проиходит считывание информации из файла в строковую переменную (буфер)
		```
    		HANDLE fileStart;
    		fileStart = CreateFile(READPATH,
     		   GENERIC_READ,
    		    FILE_SHARE_READ,
   		     NULL,
   		     OPEN_EXISTING,
   		     FILE_ATTRIBUTE_NORMAL,
   	 	    NULL);
  	 	 if (fileStart == INVALID_HANDLE_VALUE)
  	 	 {
  	 	    MessageBox(NULL, L"При открытии файла возникла ошибка!", L"Окно программы", MB_OK);
   	 	    return 0;
  	 	 }
  	  	DWORD countFileSymbols;
  	  	LPWSTR argumentsFromFile = calloc(3099, sizeof(CHAR));
   	 	if (!ReadFile(fileStart, argumentsFromFile, 3099, &countFileSymbols, NULL))
 	  	{
  	      	MessageBox(NULL, L"При чтении данных из файла возникла ошибка!", L"Окно программы", MB_OK);
  	      	return 0;
  	  	}
		```
		+ После закрываем дескриптор и заносим полученные данные в создыннй ранее массив структур
		```
    		CloseHandle(fileStart);

		char* dataConvertToStruct = strtok(argumentsFromFile, ";");
    		int countRecords;
    		for (int i = 0; i < 100; i++)
    		{
    	    		countRecords = i;
   	     		people[i].surname = dataConvertToStruct;
    	    		dataConvertToStruct = strtok(NULL, ";\r\n");
    	    		people[i].name = dataConvertToStruct;
    	    		dataConvertToStruct = strtok(NULL, ";\r\n");
    	    		people[i].midname = dataConvertToStruct;
    	    		dataConvertToStruct = strtok(NULL, ";\r\n");
    	   		people[i].age = atoi(dataConvertToStruct);
     	   		dataConvertToStruct = strtok(NULL, ";\r\n");
    		}
		```
		+ Последним действием вызывается метод для поиска пользователей по фамилиям
		```
		searchSurname(people, "Иванов");
		```
	+ Метод для поиска по фамилиям принимает два аргумента - это массив структуры пользователей и фамилия в виде строковой переменной
		+ Первым делом объявляется новые массивы структур для записи в них пользователей с искомой фамилией
		```
		struct users* currentPeople = malloc(sizeof(struct users));
    		struct users* forCurrentPeople;
    		int j = 0;
    		char* forComprasion = surnameForSearch;
		```
		+ Далее выполняется цикл, проходящий по всем пользователям, чтобы найти совпадения по фамилиям
		```
		for (int i = 0; i < 100; i++)
    		{
        		if (strcmp(people[i].surname, forComprasion) == 0)
        		{
            		currentPeople[j] = people[i];
            		forCurrentPeople = realloc(currentPeople, (j + 2) * sizeof(struct users));
            		j++;
            		currentPeople = forCurrentPeople;
        		}
    		}
		```
		+ Заключительным действием в данном методе является вызов другого метода, необходимого для записи результатов в файл
		```
		writeToFile(currentPeople, j);
		```
	+ Послений метод необходим для записи результата в отдельный от исходной информации файл
		+ Изначально необходимо открыть файл для записи
		```
		HANDLE fileResult = CreateFile(L"ResultIvanova.csv",
        		GENERIC_WRITE,
        		FILE_SHARE_WRITE,
        		NULL,
        		CREATE_ALWAYS,
        		FILE_ATTRIBUTE_NORMAL,
        		NULL);
    		DWORD countFileSymbols;
		```
		+ Объявляем строку и запускаем цикл по массиву найденых записей, чтобы записать в вышеуказанную строку данные в указанном формате
		```
		char* dataForWritting = calloc(1000, sizeof(char));
    		for (int i = 0; i < countCurrentPeople; i++)
    		{
        		sprintf(dataForWritting, "%s;%s;%s;%d\n", currentPeople[i].surname, currentPeople[i].name, currentPeople[i].midname, currentPeople[i].age);
        		WriteFile(fileResult, dataForWritting, strlen(dataForWritting), &countFileSymbols, NULL);
    		}
		```
		+ Заключительным действием записываем данные в файл, освобождаем память, выделенную под строку и закрываем дескриптор файла
		```
		WriteFile(fileResult, dataForWritting, strlen(dataForWritting), &countFileSymbols, NULL);
    		free(dataForWritting);
    		CloseHandle(fileResult);
		```
+ После выполнения программы в папке с проектом появится новый файл под названием "Result.csv"
___

После запуска программы вы увидите следующий результат в появившемся файле:

![](https://github.com/Kr33T/DLL/blob/master/res.png)

## Авторы

* **Morozov Andrew**

Для связи перейдите по следующей ссылке:
+ ВК: https://vk.com/4eva1one