typedef enum {
    BAD_TOKEN,
    NUMBER_TOKEN,
    ADD_OPERATOR_TOKEN,
    SUB_OPERATOR_TOKEN,
    MUL_OPERATOR_TOKEN,
    DIV_OPERATOR_TOKEN,
    END_OF_LINE_TOKEN,
    LEFT_PAREN_TOKEN,
    RIGHT_PAREN_TOKEN
} TokenKind;

#define MAX_TOKEN_SIZE (100)

typedef struct {
    TokenKind   kind;
    double      value;
    char        str[MAX_TOKEN_SIZE];
} Token;


void set_input_str(char *str);

void get_token(Token *token);