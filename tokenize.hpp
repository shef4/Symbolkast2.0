#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <istream>
#include <deque>

typedef std::deque<std::string> TokenSequenceType;

const char OPEN = '(';
const char CLOSE = ')';
const char COMMENT = ';';

//filters out at comment character: COMMENT and adds buffer to break characters: OPEN, CLOSE 
std::string program_filter(std::string program);
//create tokens by using a space buffer (" ") as delimeter 
TokenSequenceType make_tokens(std::string program);
//appends new tokens to the end of curr tokens
TokenSequenceType append_tokens(TokenSequenceType curr_tokens,TokenSequenceType new_tokens);
//creates dequeue list of tokens from string sequence processed per line
TokenSequenceType tokenize(std::istream & seq);
//get next line in s=input stream using \r and \n
std::istream& safeGetline(std::istream& is, std::string& t);

#endif
