#ifndef LEXER_H
#define LEXER_H

class lexer {
public:
	// Create the lexer with the specifiec language specification for this lexer to use
	lexer(specification* spec);
	~lexer();
	// Spits out the next token that the lexer reads
	token next_token();
	// Sets the source file for this lexer to tokenize
	bool set_source(std::string path_to_file);
	bool has_more_tokens();
private:
	// The state transiton table tokenizer that holds all the rules for this lexer
	std::shared_ptr<dfa> spec;
	// a token look ahead so we can avoid having to return a null token value at the end
	token lookahead_token;

	// Returns the token that is evaluated from this state
	token create_token(std::string lexeme, state state);
	// Returns the next char pointer in the stream for the tokenizer to read from
	char* next_char();
	// moves the pointer of the current source_index back by one
	void backup_char();
	// the current line of the source file we are at
	int current_line_index;
	// the current char index for that line of the source file we are at
	int current_char_index;
	// Flag to see if all the tokens have been consumed
	bool out_of_tokens;
	// The file path to the current source file
	std::string source_file_path;
	// The buffer to hold the source file
	std::vector<char> source;
	// the size of the source file in bytes
	int source_size;
	// the current index of the source buffer we are indexing into
	int source_index;
};

#endif

