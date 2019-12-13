#include "functions.h"

char* load_buffer(std::string file_name, std::vector<Sign>& collection, unsigned int& total_number_of_signs) {                              // zadaniem funkcji jest stworzenie bufora, zaladowanie kolekcji znakami oraz policzenia calkowitej ilosci znakow
    std::fstream file(file_name, std::ios::in | std::ios::binary); // wczytanie pliku

    if (file.good()) { // jesli z plikiem wszystko w porzadku
        file.seekg(0, std::ios_base::end);
        total_number_of_signs = static_cast<unsigned int>(file.tellg()); //dlugosc bufora/pliku
        if(total_number_of_signs == 0) throw -1; // jesli pusty plik
        file.seekg(0, std::ios_base::beg); //wczytaj zawartosc pliku
        char* buffer = new char[total_number_of_signs]; //utworzenie bufora
        file.read(buffer, total_number_of_signs); // wczytanie do bufora
        file.close();
        unsigned int number_of_signs = 0;
        unsigned int null_number_of_occurence = 0;
        bool occurrence = false; // wystepowanie

        for (unsigned int i = 0; i < total_number_of_signs; ++i) { // petla po buforze
            for (unsigned int j = 0; j < 256; ++j) { // znakow jest maksymalnie 256
                if (collection[j] == buffer[i]) { // jesli znaleziony znak jest juz w kolekcji
                    if (buffer[i] == '\0') { // poniewaz null jest domyslnie w wektorze licze wystapienia nulla osobno
                        null_number_of_occurence++;
                        occurrence = true;
                        break;
                    }
                    else {
                        occurrence = true; // flaga pojawienia sie
                        ++collection[j]; // ilosc wystapien + 1
                        break;
                    }
                }
            }
            if (!occurrence) { // jesli znaku nie ma w kolekcji
                collection[number_of_signs].setSign(buffer[i]); // wpisanie znaku do kolekcji
                number_of_signs++; // ilosc znakow w kolekcji + 1
            }
            occurrence = false; // zerowanie flagi
        }
        if (null_number_of_occurence) { // jezeli pojawil sie jakikolwiek null dodaje go na koniec kolekcji
            collection[number_of_signs].setSign('\0');
            collection[number_of_signs].setNumber_of_occurrence(null_number_of_occurence);
            number_of_signs++;
        }
        collection.resize(number_of_signs); // resize na rozmiar kolekcji
        collection.shrink_to_fit();
        return buffer;
    }
    else throw -1;
}

void make_huffman_tree(std::vector<Sign>& collection) { // celem funkcji jest zbudowanie drzewa do kodowania Huffmana
    unsigned int i = 0;
    unsigned int number_of_nodes = collection.size();
    std::vector<Sign*> huffman_tree(number_of_nodes); // w tym wektorze powstanie drzewo do kodowania Huffmana
    std::vector<Sign> internal_node(number_of_nodes - 1); // wektor wezlow wewnetrznych
    for (unsigned int i = 0; i < collection.size(); ++i) huffman_tree[i] = &collection[i]; // adres wezla

    while (number_of_nodes != 1) { // dopoty, dopoki nie zostanie jeden wezel
        sort(huffman_tree.begin(), huffman_tree.end(), [](Sign* x, Sign* y)->bool {return (x->getNumber_of_occurrence() > y->getNumber_of_occurrence()); }); // sortowanie wezlow
        internal_node[i].setLeft(huffman_tree.back()); // ustawienie lewego syna wewnetrznego wezla
        huffman_tree.back() = nullptr; // wyzerowanie wskaznika
        huffman_tree.pop_back(); // wyrzucenie powyzszego wezla z wektora wezlow
        internal_node[i].setRight(huffman_tree.back()); // ustawienie prawego syna
        huffman_tree.back() = &internal_node[i]; // przepiecie wskaznika na nowy element w liscie wezlow
        internal_node[i].setNumber_of_occurrence(internal_node[i].getLeft_pointer()->getNumber_of_occurrence() + internal_node[i].getRight_pointer()->getNumber_of_occurrence()); // ustawienie ilosci wystepowania dla wezla wewnetrznego (suma wystapien synow)
        internal_node[i].setInternal_node(true); // ustawienie flagi informujacej, ze jest to wezel wewnetrzny
        number_of_nodes = huffman_tree.size(); // aktualizacja ilosci wezlow
        ++i;
    }
    std::vector<bool> code(0); // tworzenie wektora na kod
    Sign* root = huffman_tree.front(); // adres korzenia
    code_nodes(root, code, false, root); // rozpoczecie kodowania rekurencyjnego
}

void code_nodes(Sign* node, std::vector<bool> code, bool bit, Sign* root) { // funkcja ma za zadanie kodować wezly, w ktorych wystepuja znaki (funkcja rekurencyjna)
    if (node != root) code.push_back(bit); // jesli to nie pierwsze wywolanie funkcji to wtedy nastapi dodanie bitu na koniec wektora kodowego
    if (!node->getInternal_node()) node->setCode(code); // ustawienie kodu jesli wezel nie jest wezlem wewnetrznym
    if (node->getLeft_pointer() != nullptr) code_nodes(node->getLeft_pointer(), code, 1, nullptr); // jesli istnieje syn, nastepuje ponowne wywolanie funkcji
    if (node->getRight_pointer() != nullptr) code_nodes(node->getRight_pointer(), code, 0, nullptr);
}

void write_binary(std::fstream& file, std::vector<bool>& code, unsigned int number_of_bytes, unsigned int& count, char& temp) { // funkcja zapisu binarnego
    for (unsigned int i = 0; i < number_of_bytes; ++i) { // petla zalezna od ilosciu bajtow na ktorej zapisany jest kod
        temp += code.at(i) << (8 - ++count); // zapisanie do tempa za pomoca przesuniecia bitowego po kolei kodu od lewej do prawej
        if (count == 8) { // jesli bitow jest 8 nastapi zapisanie do pliku
            file.write(&temp, 1); // zapis do pliku
            count = temp = 0;
        }
    }
}

char dec2bin(unsigned int dec) { // funkcja do konwersji z dziesietnej liczby na binarna zapisanej w charze
    char bin = 0;
    int move = -1;
    while (dec) {
        bin += (dec % 2) << ++move;
        dec /= 2;
    }
    return bin;
}

std::string chars2string(std::vector<uint8_t> tab) { // funkcja zamieniajaca chary na string (jeden bit jedna szufladka stringa)
    std::string temp = "";
    int count = 0;
    unsigned int i = tab.size() - 1; // ostatni indeks tablicy
    while (i + 1) { // az do przejscia calej tablicy
        while (tab[i]) {
            temp += (tab[i] % 2);
            tab[i] /= 2;
            ++count;
        }
        while (count != 8) {
            temp += '\0'; // dodanie zer jesli nie bedzie do oktetu
            ++count;
        }
        count = 0;
        --i;
    }
    std::reverse(temp.begin(), temp.end()); // odwrocenie kolejnosci zeby byla dobra kolejnosc
    return temp;
}

unsigned int bin2dec(std::string bin) { // funkcja zamieniajaca string liczb binarnych na liczbe decymalna
    unsigned int dec = 0;
    unsigned int i = bin.size() - 1;
    while (i + 1) {
        dec += static_cast<unsigned int>(pow(2, (bin.size()-1 - i)) * bin[i]); // rzutowanie na unsigned int, 2^indeks * 0 lub 1 czyli klasyczna konwersja
        --i;
    }
    return dec;
}

void code(std::string in_file_name, std::string out_file_name, std::vector<Sign>& collection) { // funkcja kodujaca plik
    std::fstream coded_file(out_file_name, std::ios::out | std::ios::binary);

if(coded_file.good()){
    unsigned int total_number_of_signs = 0; // calkowita ilosc znakow (rozmiar bufora)
    char* buffer = load_buffer(in_file_name, collection, total_number_of_signs); // bufor
    make_huffman_tree(collection); // kodowanie znakow

    char temp = 0; // tymczasowy char
    unsigned int total_number_of_bits = 0; // ilosc bitow informacji
    unsigned int dictionary_count = 0; // ilosc bajtow slownika
    unsigned int count = 0; // licznik
    unsigned int temp_int = 0; // tymczasowy int
    std::vector<bool> dictionary_bites(8); // wektor bitow rozmiaru slownika

    for (unsigned int i = 0; i < collection.size(); ++i) dictionary_count += static_cast<unsigned int>(std::ceil(collection[i].getCode().size() / 8.0f)) + 2; // +2 poniewaz znak oraz ilosc bitow kodu, obliczanie dlugosci slownika
    temp_int = dictionary_count;
    while (temp_int) { // petla do skonczenia konwersji dec na bin
        if (count < 8) dictionary_bites[count] = temp_int % 2; // jesli wielkosc slownika miesci sie na 8 bitach
        else dictionary_bites.push_back(temp_int % 2); // jesli sie nie miesci
        temp_int /= 2;
        ++count;
    }
    while (dictionary_bites.size() % 8 != 0) dictionary_bites.push_back(0); // jesli rozmiar wektora nie jest wielokrotnoscia 8

    std::reverse(dictionary_bites.begin(), dictionary_bites.end()); // odwrocenie wektora aby dostac dobra kolejnosc
    temp_int = dictionary_bites.size() / 8; // ilosc bajtow wielkosci slownika (dec)
    temp = dec2bin(temp_int); // ilosc bajtow rozmiaru slownika
    coded_file.write(&temp, 1); // zapis ilosci bajtow rozmiaru slownika
    count = temp = 0; //zerowanie
    write_binary(coded_file, dictionary_bites, dictionary_bites.size(), count, temp); // zapis dlugosci slownika
    count = temp = 0;
    for (unsigned int i = 0; i < collection.size(); ++i) { // petla do zapisu slownika
        temp = dec2bin(collection[i].getCode().size()); // kodowanie dlugosci kodu znaku
        total_number_of_bits += (collection[i].getCode().size()*collection[i].getNumber_of_occurrence()); // obliczanie ilosci bitow pliku
        coded_file.write(&temp, 1); // zapis dlugosci znaku
        temp = collection[i].getSign(); // zapis znaku
        coded_file.write(&temp, 1); // ^
        temp = 0;
        write_binary(coded_file, collection[i].getCode(), collection[i].getCode().size(), count, temp);
        if (count != 0) { // dobicie zerami do konca bajtu
            while (count != 8) temp += 0 << (8 - ++count);
            coded_file.write(&temp, 1);
        }
        count = temp = 0;
    }
    int extra_bits = total_number_of_bits % 8;
    if (extra_bits != 0) temp = dec2bin(8 - (total_number_of_bits % 8)); // ilosc dodatkowych bitow na koncu wiadomosci
    coded_file.write(&temp, 1);
    temp = 0;

    for (unsigned int i = 0; i < total_number_of_signs; ++i) { // zapis informacji
        for (unsigned int j = 0; j < collection.size(); ++j) {
            if (buffer[i] == collection[j].getSign()) // jesli znak zgodny z znakiem w buforze
                write_binary(coded_file, collection[j].getCode(), collection[j].getCode().size(), count, temp); // zapis kodu
        }
    }
    if (count != 0) { // dobicie zerami do konca bajtu
        while (count != 8) temp += 0 << (8 - ++count);
        coded_file.write(&temp, 1);
    }
    delete[] buffer;
    } else throw -1;
}


void decode(std::string in_file_name, std::string out_file_name, std::vector<Sign>& collection) { // funkcja odpowiadajaca za dekodowanie pliku binarnego
    std::ifstream coded_file(in_file_name, std::ios::binary); // plik wejsciowy
    std::ofstream decoded_file(out_file_name, std::ios::binary); // plik wyjsciowy

    if (coded_file.good() && decoded_file.good()) {
        std::vector<char> buffer(std::istreambuf_iterator<char>(coded_file), {}); // wczytanie danych do wektora
        if(buffer.empty()) throw - 1;
        unsigned int number_of_bytes_of_bytes_of_dictionary = static_cast<unsigned int>(buffer.at(0)); // ilosc bajtow dlugosci slownika
        std::vector<uint8_t> temp_vec(number_of_bytes_of_bytes_of_dictionary); // tymczasowy wektor
        for (unsigned int i = 0; i < number_of_bytes_of_bytes_of_dictionary; ++i) temp_vec.at(i) = static_cast<uint8_t>(buffer.at(i + 1)); // wpisanie bajtow oraz obliczenie liczby bajtow slownika
        unsigned int number_of_bytes_of_dictionary = bin2dec(chars2string(temp_vec)); // ilosc bajtow slownika
        temp_vec.clear(); // wyczyszczenie wektora tymczasowego

        unsigned int size_of_code = 0; // dlugosc kodu
        unsigned int max_size_of_code = 1; // dlugosc najdluzszego kodu
        unsigned int min_size_of_code = 256; // dlugosc najkrotszego kodu
        unsigned int number_of_bytes_of_code = 0; // ilosc bajtow kodu
        unsigned int collection_count = 0; // ilosc znakow w kolekcji
        std::string temp_string = ""; // tymczasowy string
        unsigned int buffer_iterator = number_of_bytes_of_bytes_of_dictionary + 1; // iterator po buforze

        while (buffer_iterator != number_of_bytes_of_bytes_of_dictionary + number_of_bytes_of_dictionary + 1) { // petla wczytujaca slownik
            size_of_code = static_cast<unsigned int>(buffer.at(buffer_iterator)); // zakladam ze kod nie jest dluzszy niz 255 bitow!
            if (size_of_code > max_size_of_code) max_size_of_code = size_of_code; // szukanie najdluzszego kodu
            if (size_of_code < min_size_of_code) min_size_of_code = size_of_code; // szukanie najkrotszego kodu
            number_of_bytes_of_code = static_cast<unsigned int>(std::ceil(size_of_code / 8.0f)); // ilosc bajtow kodu
            collection[collection_count].setSign(buffer.at(++buffer_iterator)); // zapisanie znaku do kolekcji
            temp_vec.resize(number_of_bytes_of_code); // zmienienie rozmiaru tempa na odpowiednia dlugosc
            std::vector<bool> code(size_of_code); // wektor dla kodu
            for (unsigned int i = 0; i < number_of_bytes_of_code; ++i) temp_vec.at(i) = static_cast<uint8_t>(buffer.at(++buffer_iterator)); // przejscie od bajtow do uint8_t
            temp_string = chars2string(temp_vec); // z uint8_t do stringa
            for (unsigned int i = 0; i < size_of_code; ++i) code.at(i) = static_cast<bool>(temp_string[i]); // z stringa do kodu
            collection[collection_count].setCode(code); // ustawienie kodu znaku
            ++buffer_iterator;
            ++collection_count;
        }
        collection.resize(collection_count);
        collection.shrink_to_fit();
        temp_vec.clear();
        temp_string = ""; // do zapisu do stringa
        unsigned int number_of_extra_bits = static_cast<unsigned int>(buffer.at(buffer_iterator)); // ilosc dodatkowych bitow na ostatnim oktecie

        unsigned int code_iterator = 0;
        temp_vec.resize(buffer.size() - buffer_iterator - 1);
        for (unsigned int i = 0; i < temp_vec.size(); ++i) temp_vec.at(i) = static_cast<uint8_t>(buffer.at(++buffer_iterator)); // zapis informacji do tymczasowego wektora
        temp_string = chars2string(temp_vec); // przejscie na string
        std::vector<bool> data(temp_string.size()); // wektor zawierajacy przygotowana do zdekodowania informacje
        std::vector<bool> temp_code(max_size_of_code); // tymczasowa zmienna, w ktorej bedzie przechowywany kod do zdekodowania
        char temp = 0;
        bool match_flag = 0; // flaga oznaczajaca zgodnosc kodu z pliku z kodem znaku w kolekcji
        unsigned int match_iterator = 0; // iterator zgodnosci
        unsigned int data_iterator = 0; // iterator informacji
        unsigned int begin_of_searching = 0; // poczatek szukania w kolekcji
        unsigned int actual_searching_size = min_size_of_code; // akutalny rozmiar kodu, w ktorym szukamy dopasowania

        for (unsigned int i = 0; i < temp_string.size(); ++i) data.at(i) = static_cast<bool>(temp_string[i]); // wpisanie informacji
        temp_string = "";
        std::sort(collection.begin(), collection.end(), [](Sign& x, Sign& y)->bool {return (x.getCode().size() < y.getCode().size()); }); // posortowanie w kolejnosci od najkrotszego kodu do najdluzszego

        while (data_iterator != (data.size() - number_of_extra_bits)) { // petla dekodujaca informacje i zapisujaca ja do pliku
            while (code_iterator != min_size_of_code) { // petla dobijajaca porownywany kod do minimalnego rozmiaru kodu
                temp_code.at(code_iterator) = data.at(data_iterator); // wpisanie bitu do tymczasowego wektora kodowego
                ++code_iterator;
                ++data_iterator;
            }
            while (!match_flag) { // petla trwa dopoty, dopoki nie zostanie znaleziony zakodowany znak
                for (unsigned int j = begin_of_searching; code_iterator <= actual_searching_size; ++j) { // petla zmieniajaca ilosc iteracji zaleznie od dlugosci porownywanych kodow
                    if (j > collection_count - 1) throw -1; // jesli przekroczymy kolekcje (a nie powinnismy)
                    for (unsigned int k = 0; k < actual_searching_size; ++k) { // petla porownujaca kod w wektorze tymczasowym z kodem znaku
                        if (temp_code.at(k) == collection[j].getCode().at(k)) ++match_iterator;
                    }
                    if (match_iterator == collection[j].getCode().size()) { // jesli iterator zgodnosci jest rowny z dlugoscia znaku
                        temp_string += collection[j].getSign(); // dodanie znaku do konca stringu
                        for (unsigned int l = 0; l < max_size_of_code; ++l) temp_code.at(l) = 0; // wypelnianie wektora tymczasowego zerami
                        match_flag = 1; // flaga zgodnosci true
                        break; // przerwanie petli
                    }
                    match_iterator = 0;
                    if (collection[j].getCode().size() != collection[j + 1].getCode().size() && max_size_of_code != collection[j].getCode().size()) { // jesli kolejny znak z kolekcji ma dluzsza dlugosc niz aktualny
                        actual_searching_size = collection[j + 1].getCode().size(); // nowy rozmiar kodow porownywanych
                        begin_of_searching = j + 1; // poczatek szukania
                        for (unsigned int o = 0; o < (actual_searching_size - collection[j].getCode().size()); ++o) { // petla powiekszajaca kod w wektorze tymczasowym o wyzej okreslona wartosc
                            temp_code.at(code_iterator) = data.at(data_iterator);
                            ++code_iterator;
                            ++data_iterator;
                        }
                        break;
                    }
                }
            }
            actual_searching_size = min_size_of_code;
            code_iterator = begin_of_searching = match_flag = match_iterator = temp = 0;
        }
        decoded_file << temp_string;
    }
    else throw - 1;
}
