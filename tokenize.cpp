#include "tokenize.hpp"
#include <cctype>

#include <iostream>
#include <string>
#include <algorithm>

std::string program_filter(std::string program){
    std::string new_program;
    for(int i=0;i<program.length();i++) 
    {
      if(program[i]!=OPEN && program[i]!=CLOSE && program[i]!=COMMENT){
        new_program = new_program +program[i];
      } else if(program[i]==COMMENT){
        if (!new_program.empty() && new_program[0]==' ') {
          new_program.erase(0,1);
        }
        return new_program;
      } else {
        if (new_program.back()!=' ') {
          new_program += std::string(1,' ');
        }
        new_program += std::string(1,program[i]);
        if (i+1<program.size() && program[i+1]!=' ') {
          new_program += std::string(1,' ');
        }
      }
    }
    if (!new_program.empty() && new_program[0]==' ') {
      new_program.erase(0,1);
    }
    
    return new_program;
}

TokenSequenceType make_tokens(std::string program){
  std::string token;
  TokenSequenceType tokens = TokenSequenceType(); 
  bool inComment = false;
  //check for space at front
  if (!program.empty() && program[0]!=' ') {
    token = program[0];
  } 
  //find new delimeter
  for (std::size_t i = 1; i < program.size(); i++) {
    //cureent char is not the end of token and char not in comment
    if (program[i] != ' ') {
      token += std::string(1,program[i]);

    //current char is end of token and previous char is not and chars not in comment
    } else if (program[i]==' ' && program[i-1]!=' ') {
      tokens.push_back(token);
      token = "";
    } 
  }

  if (!token.empty()) {
    tokens.push_back(token);
    token = "";
  }
  return tokens;
}

TokenSequenceType append_tokens(TokenSequenceType curr_tokens,TokenSequenceType new_tokens){
  while (!new_tokens.empty()) {
    curr_tokens.push_back(new_tokens.front());
    new_tokens.pop_front();
  }
  return curr_tokens;
}

TokenSequenceType tokenize(std::istream & seq){
  TokenSequenceType new_tokens, curr_tokens;
  if (seq.rdbuf()->in_avail() == 0){
    return curr_tokens;
  }
  std::string token,program;
  while (safeGetline(seq, token)) {
    program = token;
    program = program_filter(program);
    if (!program.empty()){
      new_tokens = make_tokens(program);
      curr_tokens = append_tokens(curr_tokens,new_tokens);
    }
  }
  return curr_tokens;
}

std::istream& safeGetline(std::istream& is, std::string& t)
{
  t.clear();

  // The characters in the stream are read one-by-one using a std::streambuf.
  // That is faster than reading them one-by-one using the std::istream.
  // Code that uses streambuf this way must be guarded by a sentry object.
  // The sentry object performs various tasks,
  // such as thread synchronization and updating the stream state.

  std::istream::sentry se(is, true);
  std::streambuf* sb = is.rdbuf();

  for(;;) {
    int c = sb->sbumpc();
    switch (c) {
      case '\n':
          return is;
      case '\r':
        if (sb->sgetc() == '\n') {
            sb->sbumpc();
        }
        return is;
      case std::streambuf::traits_type::eof():
          // Also handle the case when the last line has no line ending
          if(t.empty()) {
              is.setstate(std::ios::eofbit);
          }
          return is;
      default:
        t += (char)c;
    }
  }
}