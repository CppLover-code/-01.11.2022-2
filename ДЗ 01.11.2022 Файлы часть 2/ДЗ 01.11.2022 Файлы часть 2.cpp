/*
    Пользователь вводит путь к папке и слово для поиска.
    Программа ищет слово в папке и её подпапках. После 
    работы программы формируется отчет с информацией о 
    том, где было найдено слово, кол-во совпадений.
    Добавьте возможность замены или удаления слова для
    поиска из файлов.
*/
#include <iostream>
#include<io.h>
#include <fstream>
#include <string>

int main()
{
    std::cout << " Word search program\n" << std::endl;

    _finddata_t file_info;                                          // структура с результатом поиска - инф о найденном файле
    intptr_t hSearch;                                               // handle of search - дескриптор поискового ресурса

    const size_t SIZE = 20;
    char pattern[SIZE];                                                
    char folder[SIZE];
    char term[SIZE] = { '*' };
    char word[SIZE];

    int fl = 0;       // переменная, которая считает кол-во совпадений среди названий файлов
    int dr = 0;       // переменная, которая считает кол-во совпадений среди названий папок

    std::cout << " Enter folder path (for example, ./files/):\n";
    std::cin >> pattern;                                            // ./files/   - в папке files
    std::cout << " Enter a search term(for example, *file*):\n";
    std::cin >> word; 

    strcat_s(term, word);                                          // выполняем конкатенацию, чтоб добавить * в слово для поиска 
    strcat_s(term, "*");                                           // *file*     - в имени есть file в любом месте
    std::cout << term;

    strcpy_s(folder, pattern);                                      // копируем путь в folder для дальнейшей работы
    strcat_s(pattern, term);                                        // соединяем путь и имя
    //std::cout << pattern;
    
    hSearch = _findfirst(pattern, &file_info);

    if (hSearch == -1L)                                             // проверка - нашлись ли данные по запросу пользователя
    {
        std::cout << " Data not found " << pattern << std::endl;
        return -1;
    }
    int search_status;                                              // переменная для зранения статуса поиска (итерации)
    do
    {
        std::cout << file_info.name << "\n";


        char str[SIZE];
        char str1[260];
        strcpy_s(str1, folder);
        strcat_s(str1, file_info.name);
        //std::cout << str1 << "\n";

        if ((file_info.attrib & _A_SUBDIR) != _A_SUBDIR)  // если найденный элемент является файлом, то мы его читаем
        {
            fl++; // увеличиваем для названий файлов
            std::ifstream reader(str1);        // сокращенная форма - сразу open(...)
            if (!reader)                           // проверка нужна и для сокр формы
            {
                std::cout << "Reader open error";
                return -1;
            }
            while (!reader.eof())                         // пока не конец файла
            {
                reader.getline(str, SIZE);             // считываем строку из файла в str
                std::cout << str;
            }
            std::cout << std::endl;

            reader.close();
        }
        else
        {
            dr++; // увеличиваем для названий папок
        }

        search_status = _findnext(hSearch, &file_info);             // вызываем ф-ю _findnext (продолжение поиска)
        

    } while (search_status == 0);                                   // возвращает успешную операцию

    std::cout << " Count of file name " << fl << "\n"
        << " Count of folder name " << dr << "\n";

    _findclose(hSearch);






    return 0;
}


