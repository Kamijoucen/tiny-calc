#include <stdio.h>
#include <stdlib.h>
#include "state.h"

#define INPUT_BUF_SIZE 1024

// 这里相当于一个缓冲区用户可以预读一个Token
static Token st_look_ahead_token;
static int st_look_ahead_token_exists;

double parse_expression();

void
my_get_token(Token *token)
{
    // 先查询缓存是否存在，存在则读取缓存，并把标记置为不存在。否则就使用get_token进行读取
    if (st_look_ahead_token_exists) {
        *token = st_look_ahead_token;
        st_look_ahead_token_exists = 0;
    } else {
        get_token(token);
    }
}



void
// 回退Token
unget_token(Token *token)
{
    st_look_ahead_token = *token;
    st_look_ahead_token_exists = 1;
}


double
parse_primary_expression()
{
    double value;
    Token token;
    int minus_flag = 0;

    my_get_token(&token);

    if (token.kind == SUB_OPERATOR_TOKEN) {
        minus_flag = 1;
    } else {
        unget_token(&token);
    }


    my_get_token(&token);
    if (token.kind == NUMBER_TOKEN){
        value = token.value;
    }
    else if (token.kind == LEFT_PAREN_TOKEN) {
        value = parse_expression();
        my_get_token(&token);

        if (token.kind != RIGHT_PAREN_TOKEN) {
            printf("%s", "没有与之匹配的右括号");
            exit(-1);
        }
    }
    else {
        printf("%s", "parse_primary_expression没有发现数字Token");
        exit(-1);
    }

    if (minus_flag) {
        value = -value;
    }

    return value;
}

double
parse_term()
{
    double value1;
    double value2;

    value1 = parse_primary_expression();
    for (;;) {
        Token token;
        my_get_token(&token);

        if (token.kind != MUL_OPERATOR_TOKEN && token.kind != DIV_OPERATOR_TOKEN) {
            unget_token(&token);
            break;
        }

        value2 = parse_primary_expression();
        if (token.kind == MUL_OPERATOR_TOKEN) {
            value1 *= value2;
        }
        else if (token.kind == DIV_OPERATOR_TOKEN) {
            value1 /= value2;
        }
    }
    return value1;
}

double
parse_expression()
{
    double value1;
    double value2;
    value1 = parse_term();
    for(;;) {
        Token token;
        my_get_token(&token);
        if (token.kind != ADD_OPERATOR_TOKEN && token.kind != SUB_OPERATOR_TOKEN) {
            unget_token(&token);
            break;
        }
        value2 = parse_term();
        if (token.kind == ADD_OPERATOR_TOKEN) {
            value1 += value2;
        }
        else if (token.kind == SUB_OPERATOR_TOKEN) {
            value1 -= value2;
        }
    }
    return value1;
}

double
parse_line()
{
    double value = 0;
    // 将预读缓存标记置为假
    st_look_ahead_token_exists = 0;
    value = parse_expression();
    Token token;
    my_get_token(&token);
    if (token.kind != END_OF_LINE_TOKEN) {
        printf("error token:%s", token.str);
        exit(1);
    }
    return value;
}


int main()
{
    char buf[INPUT_BUF_SIZE];
    while (fgets(buf, INPUT_BUF_SIZE, stdin) != NULL)
    {
        set_input_str(buf);
        printf(">>> %lf\n", parse_line());
    }
}
