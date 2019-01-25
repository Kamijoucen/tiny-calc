#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "state.h"

static char *input_str;
static int cur_p;
// 词法状态
typedef enum {
    // 初始状态
    INITIAL_STATUS,
    // 数字状态
    IN_INT_PART_STATUS,
    // 小数点状态
    DOT_STATUS,
    // 小数状态
    IN_FRAC_PART_STATUS
} LexerStatus;

void
set_input_str(char *str)
{
    input_str = str;
    cur_p = 0;
}

void get_token(Token *token)
{
    LexerStatus status = INITIAL_STATUS;
    token->kind = BAD_TOKEN;
    int out_pos = 0;
    for (;;) {
        char ch = input_str[cur_p];
        if ((status == IN_INT_PART_STATUS || status == IN_FRAC_PART_STATUS) && !isdigit(ch) && ch != '.') {
            token->kind = NUMBER_TOKEN;
            sscanf(token->str, "%lf", &token->value);
            return;
        }
        if (isspace(ch)) {
            if (ch == '\n') {
                token->kind = END_OF_LINE_TOKEN;
                return;
            }
            cur_p++;
            continue;
        }
        token->str[out_pos] = ch;
        cur_p++;
        out_pos++;
        token->str[out_pos] = '\0';

        if (ch == '+') {
            token->kind = ADD_OPERATOR_TOKEN;
            return;
        }
        else if (ch == '-') {
            token->kind = SUB_OPERATOR_TOKEN;
            return;
        }
        else if (ch == '*') {
            token->kind = MUL_OPERATOR_TOKEN;
            return;
        }
        else if (ch == '/') {
            token->kind = DIV_OPERATOR_TOKEN;
            return;
        }
        else if (ch == '(') {
            token->kind = LEFT_PAREN_TOKEN;
            return;
        }
        else if (ch == ')') {
            token->kind = RIGHT_PAREN_TOKEN;
            return;
        }
        // 如果当前字符是数字
        else if (isdigit(ch)) {
            // 而且当前状态是起始状态则读入字符后当前状态进入整数状态
            if (status == INITIAL_STATUS) {
                status = IN_INT_PART_STATUS;
            }
                // 如果是小数点状态则进入小数状态
            else if (status == DOT_STATUS) {
                status = IN_FRAC_PART_STATUS;
            }
        } else if (ch == '.') {
            if (status == IN_INT_PART_STATUS) {
                status = DOT_STATUS;
            } else {
                exit(-1);
            }
        } else {
            fprintf(stderr, "bad character(%c)\n", ch);
            exit(1);
        }

    }
}

