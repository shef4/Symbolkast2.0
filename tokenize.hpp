#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <istream>
#include <deque>

typedef std::deque<std::string> TokenSequenceType;

const char OPEN = '(';
const char CLOSE = ')';
const char COMMENT = ';';


std::string replace(std::string program);
TokenSequenceType parse_delim(std::string program);
TokenSequenceType merge_tokens(TokenSequenceType curr_tokens,TokenSequenceType new_tokens);
// split string into a list of tokens where a token is one of
// OPEN or CLOSE or a space-delimited string
// ignores any whitespace and from any ";" to end-of-line
TokenSequenceType tokenize(std::istream & seq);
#endif
