#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#pragma once
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include "sign.h"

char* load_buffer(std::string file_name, std::vector<Sign>& collection, unsigned int& total_number_of_signs);
void make_huffman_tree(std::vector<Sign>& collection);
void code_nodes(Sign* node, std::vector<bool> code, bool bit, Sign* root);
void write_binary(std::fstream& file, std::vector<bool>& code, unsigned int number_of_bytes, unsigned int& count, char& temp);
char dec2bin(unsigned int dec);
unsigned int bin2dec(std::string bin);
std::string chars2string(std::vector<uint8_t> tab);
void code(std::string in_file_name, std::string out_file_name, std::vector<Sign>& collection);
void decode(std::string in_file_name, std::string out_file_name, std::vector<Sign>& collection);

#endif // FUNCTIONS_H
