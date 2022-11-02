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
    char pattern[SIZE];                                             //./files/*file*   
    char term[SIZE];    

    std::cout << " Enter folder path (for example, ./files/):\n";
    std::cin >> pattern;                                            // ./files/   - в папке files
    std::cout << " Enter a search term(for example, *file*):\n";
    std::cin >> term;                                               // *file*      - в имени есть file в любом месте

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

        




        search_status = _findnext(hSearch, &file_info);             // вызываем ф-ю _findnext (продолжение поиска)
        if ((file_info.attrib & _A_SUBDIR) == 0) // если найден файл, то копаем дальше по файлам
        {
            std::ifstream reader;
            const std::string filename = file_info.name;
            std::cout << filename << "\n";

            reader.open(filename);
            if (!reader)  // открылся ли файл
            {
                std::cout << "Reader open error" << std::endl;
                return -2;
            }

            std::string str;

            while (!reader.eof())  // пока не конец файла
            {
                //reader >> str;
                std::getline(reader, str);
                std::cout << str << "\n";
            }
            reader.close();
        }

    } while (search_status == 0);                                   // возвращает успешную операцию

    _findclose(hSearch);






    return 0;
}


