#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <unordered_map>
#include <stdexcept>

using namespace std;

enum TokenType
{
    T_INT,
    T_FLOAT,
    T_DOUBLE,
    T_CHAR,
    T_STRING,
    T_BOOL,
    T_ID,
    T_NUM,
    T_IF,
    T_ELSE,
    T_RETURN,
    T_ASSIGN,
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,
    T_DOT,
    T_SEMICOLON,
    T_GT,
    T_EOF,
    T_TRUE,
    T_FALSE,
    T_WHILE,
    T_FOR,
    T_AND,
    T_OR,
    T_EQ,
    T_NEQ,
    T_LT,
    T_LEQ,
    T_GEQ,
};

struct Token
{
    TokenType type;
    string value;
    int line;
};

class Lexer
{

private:
    string src;
    size_t pos;
    int line;

public:
    Lexer(const string &src)
    {
        this->src = src;
        this->pos = 0;
        this->line = 1; // added line valiable in the constructor ad initialized it to 0
    }
    string consumeNumber()
    {
        size_t start = pos;
        while (pos < src.size() && (isdigit(src[pos]) || src[pos] == '.'))
        {
            pos++;
        }
        return src.substr(start, pos - start);
    }

    string consumeWord()
    {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos]))
            pos++;
        return src.substr(start, pos - start);
    }

    string consumeString() // task 3 adding string
    {
        pos++;
        size_t start = pos;
        while (pos < src.size() && src[pos] != '"')
        {
            pos++;
        }
        if (pos >= src.size())
        {
            cout << "syntax error of string at line " << line << endl;
            exit(1);
        }
        string strLiteral = src.substr(start, pos - start);
        pos++;
        return strLiteral;
    }
    void consumeComment() // additional task at adding comments
    {
        if (src[pos] == '/' && src[pos + 1] == '/') // single line comment
        {
            while (pos < src.size() && src[pos] != '\n')
            {
                pos++;
            }
            line++;
        }
        else
        {
            cout << "comment errror at line " << line << endl;
            exit(1);
        }
    }

    string consumeChar() // task 3 adding char
    {
        pos++;
        if (pos >= src.size() || src[pos] == '\'')
        {
            cout << "syntax error of char at line: " << line << endl;
            exit(1);
        }
        char charLiteral = src[pos];
        pos++;
        if (pos >= src.size() || src[pos] != '\'')
        {
            cout << "syntax error of char at line: " << line << endl;
            exit(1);
        }
        pos++;
        return string(1, charLiteral);
    }

    vector<Token> tokenizer()
    {
        vector<Token> tokens;
        while (pos < src.size())
        {
            char current = src[pos];
            if (isspace(current))
            {
                if (current == '\n')
                    line++; // when the sorce code goes to next line then there will be a liune increment
                pos++;
                continue;
            }

            if (current == '/' && pos + 1 < src.size() && (src[pos + 1] == '/')) // when ever the comments are detected the lexer function of conumecomment is called
            {
                consumeComment();
                continue;
            }

            if (current == '"') // string double qutation handling
            {
                string strqot = consumeString();
                tokens.push_back(Token{T_STRING, strqot, line});
                continue;
            }

            if (current == '\'') // char single quote handling
            {
                string charqot = consumeChar();
                tokens.push_back(Token{T_CHAR, charqot, line});
                continue;
            }

            if (isdigit(current))
            {
                string number = consumeNumber();
                tokens.push_back(Token{T_NUM, number, line});
                continue;
            }
            if (isalpha(current))
            {
                string word = consumeWord();
                if (word == "int")
                    tokens.push_back(Token{T_INT, word, line});
                else if (word == "float")
                    tokens.push_back(Token{T_FLOAT, word, line});
                else if (word == "char")
                    tokens.push_back(Token{T_CHAR, word, line});
                else if (word == "bool")
                    tokens.push_back(Token{T_BOOL, word, line});
                else if (word == "string")
                    tokens.push_back(Token{T_STRING, word, line});
                else if (word == "double")
                    tokens.push_back(Token{T_DOUBLE, word, line});
                else if (word == "Agar")
                    tokens.push_back(Token{T_IF, word, line}); // task 5
                else if (word == "Magar")
                    tokens.push_back(Token{T_ELSE, word, line});
                else if (word == "return")
                    tokens.push_back(Token{T_RETURN, word, line});
                else if (word == "ture")
                    tokens.push_back(Token{T_TRUE, word, line});
                else if (word == "false")
                    tokens.push_back(Token{T_FALSE, word, line});
                else if (word == "while")
                    tokens.push_back(Token{T_WHILE, word, line});
                else if (word == "for")
                    tokens.push_back(Token{T_FOR, word, line});
                else
                    tokens.push_back(Token{T_ID, word, line});
                continue;
            }
            switch (current)
            {
            case '+':
                tokens.push_back(Token{T_PLUS, "+", line});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-", line});
                break;
            case '*':
                tokens.push_back(Token{T_MUL, "*", line});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/", line});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "(", line});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")", line});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{", line});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}", line});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", line});
                break;
            case '.':
                tokens.push_back(Token{T_DOT, ".", line});
                break;
            case '&':
                if (pos + 1 < src.size() && src[pos + 1] == '&') // task 7
                {
                    pos += 2;
                    tokens.push_back(Token{T_AND, "&&", line});
                }
                else
                {
                    cout << "Unexpected character: " << current << " at line " << line << endl;
                    exit(1);
                }
                break;
            case '|':
                if (pos + 1 < src.size() && src[pos + 1] == '|')
                {
                    pos += 2;
                    tokens.push_back(Token{T_OR, "||", line});
                }
                else
                {
                    cout << "Unexpected character: " << current << " at line " << line << endl;
                    exit(1);
                }
                break;
            case '=':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    pos += 2;
                    tokens.push_back(Token{T_EQ, "==", line});
                }
                else
                {
                    tokens.push_back(Token{T_ASSIGN, "=", line});
                    pos++;
                }
                break;
            case '!':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    pos += 2;
                    tokens.push_back(Token{T_NEQ, "!=", line});
                }
                else
                {
                    cout << "Unexpected character: " << current << " at line " << line << endl;
                    exit(1);
                }
                break;
            case '<':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    pos += 2;
                    tokens.push_back(Token{T_LEQ, "<=", line});
                }
                else
                {
                    tokens.push_back(Token{T_LT, "<", line});
                    pos++;
                }
                break;

            case '>':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    pos += 2;
                    tokens.push_back(Token{T_GEQ, ">=", line});
                }
                else
                {
                    tokens.push_back(Token{T_GT, ">", line});
                    pos++;
                }
                break;

            default:
                cout << "Unexpected character: " << current << "at line" << line << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line});
        return tokens;
    }
};
class SymbolTable
{
public:
    void declareVariable(const string &name, const string &type)
    {
        if (symbolTable.find(name) != symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' is already declared.");
        }
        symbolTable[name] = type;
    }

    string getVariableType(const string &name)
    {
        if (symbolTable.find(name) == symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' is not declared.");
        }
        return symbolTable[name];
    }

    bool isDeclared(const string &name) const
    {
        return symbolTable.find(name) != symbolTable.end();
    }

private:
    map<string, string> symbolTable;
};
class IntermediateCodeGenerator
{
public:
    vector<string> instructions;
    int tempCount = 0;

    string newTemp()
    {
        return "t" + to_string(tempCount++);
    }

    void addInstruction(const string &instr)
    {
        instructions.push_back(instr);
    }

    void printInstructions()
    {
        for (const auto &instr : instructions)
        {
            cout << instr << endl;
        }
    }
};
class Parser
{
public:
    Parser(const vector<Token> &tokens, SymbolTable &symTable, IntermediateCodeGenerator &icg)
        : tokens(tokens), pos(0), symTable(symTable), icg(icg) {}

    void parseProgram()
    {
        while (tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        cout << "Parsing completed successfully! No Syntax Errors" << endl;
    }

private:
    vector<Token> tokens;
    size_t pos;
    SymbolTable &symTable;
    IntermediateCodeGenerator &icg;

    // Utility functions
    void expect(TokenType type)
    {
        if (tokens[pos].type == type)
        {
            pos++;
        }
        else
        {
            cerr << "Syntax error: expected token type " << type
                 << " but found '" << tokens[pos].value << "' at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    string expectAndReturnValue(TokenType type)
    {
        if (tokens[pos].type == type)
        {
            string value = tokens[pos].value;
            pos++;
            return value;
        }
        else
        {
            cerr << "Syntax error: expected token type " << type
                 << " but found '" << tokens[pos].value << "' at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    // Parsing methods
    void parseStatement()
    {
        if (tokens[pos].type == T_INT || tokens[pos].type == T_CHAR || tokens[pos].type == T_BOOL ||
            tokens[pos].type == T_STRING || tokens[pos].type == T_DOUBLE || tokens[pos].type == T_FLOAT)
        {
            parseDeclaration();
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else if (tokens[pos].type == T_IF)
        {
            parseIfStatement();
        }
        else if (tokens[pos].type == T_WHILE)
        {
            parseWhileStatement();
        }
        else if (tokens[pos].type == T_FOR)
        {
            parseForStatement();
        }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else if (tokens[pos].type == T_LBRACE)
        {
            parseBlock();
        }
        else
        {
            cerr << "Syntax error: unexpected token '" << tokens[pos].value
                 << "' at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void parseDeclaration()
    {
        string type = expectAndReturnValue(tokens[pos].type);
        string varName = expectAndReturnValue(T_ID);

        symTable.declareVariable(varName, type);

        if (tokens[pos].type == T_ASSIGN)
        {
            expect(T_ASSIGN);
            string expr = parseExpression();
            icg.addInstruction(varName + " = " + expr);
        }

        expect(T_SEMICOLON);
    }

    void parseAssignment()
    {
        string varName = expectAndReturnValue(T_ID);
        symTable.getVariableType(varName); // Ensure the variable is declared

        expect(T_ASSIGN);
        string expr = parseExpression();
        icg.addInstruction(varName + " = " + expr);

        expect(T_SEMICOLON);
    }

    void parseBlock()
    {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        expect(T_RBRACE);
    }
    void parseCharacter()
    {
        expect(T_ID);
        expect(T_ASSIGN);
        parseAssignment();
    }

    void parseIfStatement()
    {
        expect(T_IF);
        expect(T_LPAREN);                
        string cond = parseExpression(); 
        expect(T_RPAREN);

        string temp = icg.newTemp();             
        icg.addInstruction(temp + " = " + cond); 

        icg.addInstruction("Agar " + temp + " goto L1"); 
        icg.addInstruction("goto L2");                  
        icg.addInstruction("L1:");                       

        parseStatement();

        if (tokens[pos].type == T_ELSE)
        { 
            icg.addInstruction("goto L3");
            icg.addInstruction("L2:");
            expect(T_ELSE);
            parseStatement(); 
            icg.addInstruction("L3:");
        }
        else
        {
            icg.addInstruction("L2:");
        }
    }

    void parseWhileStatement()
    {
        expect(T_WHILE);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
    }

    void parseForStatement()
    {
        expect(T_FOR);
        expect(T_LPAREN);

        if (tokens[pos].type == T_INT || tokens[pos].type == T_CHAR || tokens[pos].type == T_BOOL ||
            tokens[pos].type == T_STRING || tokens[pos].type == T_DOUBLE || tokens[pos].type == T_FLOAT)
        {
            parseDeclaration();
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }

        expect(T_SEMICOLON);
        parseExpression();
        expect(T_SEMICOLON);

        if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }

        expect(T_RPAREN);
        parseStatement();
    }

    void parseReturnStatement()
    {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    string parseExpression()
    {
        // Parse simple expressions and return intermediate representation
        string temp = parseTerm();

        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS || tokens[pos].type == T_GT || tokens[pos].type == T_LT || tokens[pos].type == T_GEQ || tokens[pos].type == T_LEQ || tokens[pos].type == T_EQ || tokens[pos].type == T_NEQ || tokens[pos].type == T_AND || tokens[pos].type == T_OR)
        {
            string op = tokens[pos].value;
            pos++;
            string rhs = parseTerm();
            string result = icg.newTemp();
            icg.addInstruction(result + " = " + temp + " " + op + " " + rhs);
            temp = result;
        }

        return temp;
    }

    string parseTerm()
    {
        string temp = parseFactor();

        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            string op = tokens[pos].value;
            pos++;
            string rhs = parseFactor();
            string result = icg.newTemp();
            icg.addInstruction(result + " = " + temp + " " + op + " " + rhs);
            temp = result;
        }

        return temp;
    }

    string parseFactor()
    {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID)
        {
            return expectAndReturnValue(tokens[pos].type);
        }
        else if (tokens[pos].type == T_LPAREN)
        {
            expect(T_LPAREN);
            string expr = parseExpression();
            expect(T_RPAREN);
            return expr;
        }
        else
        {
            cerr << "Syntax error: unexpected token '" << tokens[pos].value
                 << "' at line " << tokens[pos].line << endl;
            exit(1);
        }
    }
};

int main(int argc, char *argv[])
{ // to pass the code file as an argument TASK1
    string input;

    for (int i = 1; i < argc; i++)
    {
        string filename = argv[i];
        fstream myfile;
        myfile.open(filename);
        if (myfile.is_open())
        {
            string line;
            while (getline(myfile, line))
            {
                input += line + "\n";
            }
            myfile.close();
        }
        else
        {
            cerr << "Failed to open file: " << filename << endl;
        }
    }

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenizer();
    SymbolTable symTable;
    IntermediateCodeGenerator icg;
    Parser parser(tokens, symTable, icg);

    parser.parseProgram();
    icg.printInstructions();
}
