#include "tokenize.hpp"
#include <cctype>

#include <iostream>
#include <string>
#include <algorithm>

std::string replace(std::string program){
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
    
    //std::cout << "replaced_program = " << new_program << std::endl;
    return new_program;
}

TokenSequenceType parse_delim(std::string program){
  std::string token;
  TokenSequenceType tokens = TokenSequenceType(); 
  bool inComment = false;

  //check for space at front
  if (!program.empty() && program[0]!=' ' && program[0]!=';' && program[0]!='\n') {
    token = program[0];
  } 
  //find new deliter
  for (std::size_t i = 1; i < program.size(); i++) {
    //cureent char is not the end of token and char not in comment
    if (program[i] != ' ') {
      token += std::string(1,program[i]);

    //current char is end of token and previous char is not and chars not in comment
    } else if (program[i]==' ' && program[i-1]!=' ') {
      tokens.push_back(token);
      // if (tokens.size() == 1){
      //   std::cout << token;
      // }else{
      //   std::cout << ", "<< token;
      // }
      token = "";
    } 
  }

  if (!token.empty()) {
    tokens.push_back(token);
    // if (tokens.size() == 1){
    //   std::cout << token<< std::endl;
    // }else{
    //   std::cout << ", "<< token<< std::endl;
    // }
    token = "";
  }
  return tokens;
}

TokenSequenceType merge_tokens(TokenSequenceType curr_tokens,TokenSequenceType new_tokens){
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
  while (std::getline(seq, token)) {
    //std::cout << std::endl << token<< std::endl;
    program = token;
    new_tokens = parse_delim(replace(program));
    curr_tokens = merge_tokens(curr_tokens,new_tokens);
  }
  return curr_tokens;

}