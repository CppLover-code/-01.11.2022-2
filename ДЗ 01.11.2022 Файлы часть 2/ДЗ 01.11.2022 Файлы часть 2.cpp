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
    char pattern[SIZE] = {};
    char folder[SIZE] = {};
    char term[SIZE] = { '*' };
    char word[SIZE] = {};

    int fl = 0;       // переменная, которая считает кол-во совпадений среди названий файлов
    int dr = 0;       // переменная, которая считает кол-во совпадений среди названий папок
    int in = 0;       // переменная, которая считает кол-во совпадений внутри самих файлов

    std::cout << " Enter folder path (for example, ./files/):\n";
    std::cin >> pattern;                                            // ./files/   - в папке files
    std::cout << " Enter a search term(for example, file):\n";
    std::cin >> word; 

    strcat_s(term, word);                                          // выполняем конкатенацию, чтоб добавить * в слово для поиска 
    strcat_s(term, "*");                                           // *file*     - в имени есть file в любом месте
    //std::cout << term;

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

    std::cout << " \nList of found objects:\n";

    do
    {
        std::cout << file_info.name << "\n";


        char str[SIZE] = {};  // переменная для хранения строки из файла
        char str1[260] = {};  // хранит весь путь и название 
        char str2[SIZE] = {}; // название нового измененного файла
        strcpy_s(str1, folder);
        strcat_s(str1, file_info.name);

        //std::cout << str1 << "\n";

        if ((file_info.attrib & _A_SUBDIR) != _A_SUBDIR)  // если найденный элемент является файлом, увеличиваем счетчик
        {
            fl++; // увеличиваем для названий файлов
            std::ifstream reader(str1);                   // файловые переменные для чтения
            if (!reader)                                        // проверка открытия файла для чтения
            {
                std::cout << "Reader open error";
                return -1;
            }
            char* pos;                                          // указатель на первое совпадение подстроки в строке

            while (!reader.eof())                               // пока не конец файла "Text.txt"
            {
                reader.getline(str, SIZE);                      // считываем строку из файла в str

                pos = strstr(str, word);                        // указатель на первое совпадение подстроки в строке
                if (pos != NULL)                                // если совпадение найдено
                {
                    in++;                                       // увеличиваем кол-во совп внутри файлов
                }
            }
            reader.close();                                     // закрываем файлы
        }
        else
        {
            dr++; // увеличиваем для названий папок
        }

        search_status = _findnext(hSearch, &file_info);             // вызываем ф-ю _findnext (продолжение поиска)
        

    } while (search_status == 0);                                   // возвращает успешную операцию

    std::cout << " \nReport \n" 
        << " Number of matches in filenames    - " << fl << "\n"
        << " Number of matches in folder names - " << dr << "\n"
        << " Number of matches in file content - " << in << "\n";

    int c = 0;  // переменная для выбора пользователя
    std::cout << "Would you like to change or remove a search word from files?\n"
        << " 1 - change\n"
        << " 2 - remove\n"
        << " 3 - exit\n";
    std::cin >> c;

    std::ifstream reader; // файловые переменные для чтения
    std::ofstream writer; // и записи результата

    switch (c)
    {
    case 1:
    {
        char str_cin[SIZE] = {}; // "a_file.txt" };         // переменная для хранения имени файла 
        char str_f[SIZE] = {}; // "./files/a_file.txt" };         // переменная для хранения имени  и пути файла
        char str_mod[SIZE] = {};     // название нового измененного файла
        char str_ins[SIZE] = {};  // слово для замены (ввод пользователя)

        std::cout << " Enter file name:\n";
        std::cin >> str_cin;
 
        strcpy_s(str_f, folder);
        strcat_s(str_f, str_cin);  // переменная для хранения имени  и пути файла

        strcpy_s(str_mod, str_f);  // название нового измененного файла
        strcat_s(str_mod, "_mod");

        puts("\nEnter the word to be inserted: \n");
        std::cin >> str_ins;

        reader.open(str_f);    // файловые переменные для чтения
        writer.open(str_mod);  // и записи результата

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

        char* p;                                          // указатель на первое совпадение подстроки в строке

        size_t index = 0;
        size_t len1 = strnlen(word, SIZE);              // длина заменяемого слова
        size_t len2 = strnlen(str_ins, SIZE);               // длина слова для замены

        while (!reader.eof())                               // пока не конец файла "Text.txt"
        {
            reader.getline(str_f, SIZE);                      // считываем строку из файла в str
            size_t length = strnlen(str_f, SIZE);             // определяем ее длину

            p = strstr(str_f, word);                    // указатель на первое совпадение подстроки в строке
            if (p != NULL)                                // если совпадение найдено
            {
                for (size_t i = 0; i < length; i++)         // исходный массив строки str
                {
                    index = p - str_f;                        // индекс элемента, с которого начинаем замену (адрес подстроки минус нулевой индекс строки)

                    if (i != index)                           // если индекс исходного массива не совпал с индексом начала замены
                    {
                        writer << str_f[i];                   // записываем символы исходного файла в новый
                    }
                    else                                    // иначе, если индексы совпали, 
                    {
                        for (size_t x = 0; x < len2; x++)   // масссив с символами, которые необходимо вставить вместо введенного пользователем слова
                        {
                            writer << str_ins[x];           // записываем символы слова для замены в новый файл
                        }
                        i = i + len1 - 1;                   // возвращаемся в исходный массив строки str, перескочив индексы слова, которое нужно было заменить
                        p = strstr(p + 1, word);        // продолжаем искать совпадения
                    }
                }
            }
            else                                            // иначе, если совпадение не найдено
            {
                for (size_t i = 0; i < length; i++)         // исходный массив строки str
                {
                    writer << str_f[i];                       // записываем символы исходного файла в новый
                }
            }
            writer << "\n"; // записываем переход на новую строку в новый файл после окончания str

            reader.close();                                     // закрываем файлы
            writer.flush();
            writer.close();
        }
        std::cout << " The modified file is located in the project folder!\n";
        break;
    }

    case 2:
    {
        char str_f1[SIZE] = {};         // переменная для хранения имени файла
        char str_mod1[SIZE] = {};     // название нового измененного файла
        char str_del[SIZE] = { "" };

        std::cout << " Enter file name:\n";
        std::cin >> str_f1;

        strcpy_s(str_mod1, str_f1);
        strcat_s(str_mod1, "_mod");

        reader.open(str_f1);    // файловые переменные для чтения
        writer.open(str_mod1);  // и записи результата

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

        char* pos1 = {};                                          // указатель на первое совпадение подстроки в строке

        size_t ind1 = 0;
        size_t len11 = strnlen(word, SIZE);              // длина заменяемого слова
        size_t len22 = strnlen(str_del, SIZE);               // длина слова для замены

        while (!reader.eof())                               // пока не конец файла 
        {
            reader.getline(str_f1, SIZE);                      // считываем строку из файла в str
            size_t length = strnlen(str_f1, SIZE);             // определяем ее длину

            pos1 = strstr(str_f1, word);                    // указатель на первое совпадение подстроки в строке
            if (pos1 != NULL)                                // если совпадение найдено
            {
                for (size_t i = 0; i < length; i++)         // исходный массив строки str
                {
                    ind1 = pos1 - str_f1;                        // индекс элемента, с которого начинаем замену (адрес подстроки минус нулевой индекс строки)

                    if (i != ind1)                           // если индекс исходного массива не совпал с индексом начала замены
                    {
                        writer << str_f1[i];                   // записываем символы исходного файла в новый
                    }
                    else                                    // иначе, если индексы совпали, 
                    {
                        for (size_t x = 0; x < len22; x++)   // масссив с символами, которые необходимо вставить вместо введенного пользователем слова
                        {
                            writer << str_del[x];           // записываем символы слова для замены в новый файл
                        }
                        i = i + len11 - 1;                   // возвращаемся в исходный массив строки str, перескочив индексы слова, которое нужно было заменить
                        pos1 = strstr(pos1 + 1, word);        // продолжаем искать совпадения
                    }
                }
            }
            else                                            // иначе, если совпадение не найдено
            {
                for (size_t i = 0; i < length; i++)         // исходный массив строки str
                {
                    writer << str_f1[i];                       // записываем символы исходного файла в новый
                }
            }
            writer << "\n"; // записываем переход на новую строку в новый файл после окончания str

            reader.close();                                     // закрываем файлы
            writer.flush();
            writer.close();
        }
        std::cout << " The modified file is located in the project folder!\n";
        break;
    }

    case 3:
        exit(0);
    }

    _findclose(hSearch);

    
    return 0;
}