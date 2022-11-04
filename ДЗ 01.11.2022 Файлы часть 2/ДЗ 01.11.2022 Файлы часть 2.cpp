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

    const size_t SIZE = 50;
    char pattern[SIZE];                                                
    char folder[SIZE] = {};
    char term[SIZE] = { '*' };
    char word[SIZE];

    int fl = 0;       // переменная, которая считает кол-во совпадений среди названий файлов
    int dr = 0;       // переменная, которая считает кол-во совпадений среди названий папок
    int in = 0;       // переменная, которая считает кол-во совпадений внутри самих файлов

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
    int search_status;                                              // переменная для хранения статуса поиска (итерации)
    do
    {
        std::cout << file_info.name << "\n";


        char str[SIZE];  // переменная для хранения строки из файла
        char str1[260] = {};  // хранит весь путь и название 
        char str2[SIZE] = {}; // название нового измененного файла
        strcpy_s(str1, folder);
        strcat_s(str1, file_info.name);

        strcpy_s(str2, file_info.name);
        strcat_s(str2, "_mod");

        //std::cout << str1 << "\n";
        //std::cout << str2 << "\n";

        if ((file_info.attrib & _A_SUBDIR) != _A_SUBDIR)  // если найденный элемент является файлом, увеличиваем счетчик
        {
            fl++; // увеличиваем для названий файлов
            int c;  // переменная для выбора пользователя
            std::cout << "Would you like to change or remove a search word from files?\n"
                << " 1 - change\n"
                << " 2 - remove\n"
                << " 3 - no\n";
            std::cin >> c;

            std::ifstream reader(str1);                   // файловые переменные для чтения
            std::ofstream writer(str2);                // и записи результата

           
                char str_ins[SIZE] = {};                                 // слово для замены (ввод пользователя)
                char str_del[SIZE] = { ' ' };
                

                if (!reader)                                        // проверка открытия файла для чтения
                {
                    std::cout << "Reader open error";
                    return -1;
                }

                if (!writer)                                        // проверка открытия файла для записи
                {
                    std::cout << "Writer open error";
                    return -1;
                }

                char* pos;                                          // указатель на первое совпадение подстроки в строке

                size_t ind = 0;
                size_t len1 = strnlen(word, SIZE);              // длина заменяемого слова
                size_t len2 = strnlen(str_ins, SIZE);               // длина слова для замены

                while (!reader.eof())                               // пока не конец файла "Text.txt"
                {
                    reader.getline(str, SIZE);                      // считываем строку из файла в str
                    size_t length = strnlen(str, SIZE);             // определяем ее длину

                    pos = strstr(str, word);                    // указатель на первое совпадение подстроки в строке
                    if (pos != NULL)                                // если совпадение найдено
                    {
                        in++; // увеличиваем кол-во совп внутри файлов
                    }
                    switch (c) 
                    {
                    case 1:
                        puts("\nEnter the word to be inserted: \n");
                        std::cin >> str_ins;

                        if (pos != NULL)                                // если совпадение найдено
                        {
                            for (size_t i = 0; i < length; i++)         // исходный массив строки str
                            {
                                ind = pos - str;                        // индекс элемента, с которого начинаем замену (адрес подстроки минус нулевой индекс строки)

                                if (i != ind)                           // если индекс исходного массива не совпал с индексом начала замены
                                {
                                    writer << str[i];                   // записываем символы исходного файла в новый
                                }
                                else                                    // иначе, если индексы совпали, 
                                {
                                    for (size_t x = 0; x < len2; x++)   // масссив с символами, которые необходимо вставить вместо введенного пользователем слова
                                    {
                                        writer << str_ins[x];           // записываем символы слова для замены в новый файл
                                    }
                                    i = i + len1 - 1;                   // возвращаемся в исходный массив строки str, перескочив индексы слова, которое нужно было заменить
                                    pos = strstr(pos + 1, word);        // продолжаем искать совпадения
                                }
                            }
                        }
                        else                                            // иначе, если совпадение не найдено
                        {
                            for (size_t i = 0; i < length; i++)         // исходный массив строки str
                            {
                                writer << str[i];                       // записываем символы исходного файла в новый
                            }
                        }
                        writer << "\n"; // записываем переход на новую строку в новый файл после окончания str

                    case 2:
                        if (pos != NULL)                                // если совпадение найдено
                        {
                            for (size_t i = 0; i < length; i++)         // исходный массив строки str
                            {
                                ind = pos - str;                        // индекс элемента, с которого начинаем замену (адрес подстроки минус нулевой индекс строки)

                                if (i != ind)                           // если индекс исходного массива не совпал с индексом начала замены
                                {
                                    writer << str[i];                   // записываем символы исходного файла в новый
                                }
                                else                                    // иначе, если индексы совпали, 
                                {
                                    for (size_t x = 0; x < len2; x++)   // масссив с символами, которые необходимо вставить вместо введенного пользователем слова
                                    {
                                        writer << str_del[x];           // записываем символы слова для замены в новый файл
                                    }
                                    i = i + len1 - 1;                   // возвращаемся в исходный массив строки str, перескочив индексы слова, которое нужно было заменить
                                    pos = strstr(pos + 1, word);        // продолжаем искать совпадения
                                }
                            }
                        }
                        else                                            // иначе, если совпадение не найдено
                        {
                            for (size_t i = 0; i < length; i++)         // исходный массив строки str
                            {
                                writer << str[i];                       // записываем символы исходного файла в новый
                            }
                        }
                        writer << "\n"; // записываем переход на новую строку в новый файл после окончания str
                    //case 3:
                        
                    }
                }


                reader.close();                                     // закрываем файлы
                writer.flush();
                writer.close();
            
        }
        else
        {
            dr++; // увеличиваем для названий папок
        }

        search_status = _findnext(hSearch, &file_info);             // вызываем ф-ю _findnext (продолжение поиска)
        

    } while (search_status == 0);                                   // возвращает успешную операцию

    std::cout << " Count of file name " << fl << "\n"
        << " Count of folder name " << dr << "\n"
        << " Count in files " << in << "\n";

    _findclose(hSearch);






    return 0;
}


