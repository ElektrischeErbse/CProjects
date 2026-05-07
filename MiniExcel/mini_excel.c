#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_view.h>

char *read_csv(const char *csv, size_t *size)
{
    char *buffer = NULL;
    FILE *f = fopen(csv, "rb");
    if (!f) {
        goto error;
    }
    if (fseek(f, 0, SEEK_END) == -1) {
        goto error;
    }
    long s = ftell(f);
    if (s == -1) {
        goto error;
    }
    fseek(f, 0, SEEK_SET);
    buffer = malloc(sizeof(char) * s);
    if (!buffer) {
        goto error;
    }
    size_t read_bytes = fread(buffer, 1, s, f);
    if (read_bytes < s && ferror(f)) {
        goto error;
    }
    fclose(f);
    *size = s;
    return buffer;
error:
    if (f) {
        fclose(f);
    }
    if (buffer) {
        free(buffer);
    }
    *size = 0;
    return NULL;
}

typedef struct Expr Expr;

typedef enum {
    EXPR_KIND_NUMBER,
    EXPR_KIND_CELL,
    EXPR_KIND_PLUS
} ExprKind;

typedef struct {
    Expr *left;
    Expr *right;
} ExprPlus;

typedef struct {
    size_t row;
    size_t col;
} Expr_Cell;

typedef union {
    double number;
    Expr_Cell cell;
    ExprPlus plus;
} ExprAs;

struct Expr {
    ExprKind expr_kind;
    ExprAs expr_as;
};

typedef enum {
    CELL_KIND_TEXT,
    CELL_KIND_NUMBER,
    CELL_KIND_EXPR
} CellKind;

typedef union {
    StringView text;
    double number;
    Expr *expr;
} CellData;

typedef struct {
    CellKind cell_kind;
    CellData cell_data;
} Cell;

typedef struct {
    Cell *cells;
    size_t rows;
    size_t cols;
} Table;

const char *cell_kind_to_str(CellKind kind)
{
    switch (kind) {
        case CELL_KIND_TEXT:
            return "text";
        case CELL_KIND_EXPR:
            return "expr";
        case CELL_KIND_NUMBER:
            return "number";
        default:
            return "";
    }
}

void estimate_table_size(StringView content, size_t *out_rows, size_t *out_cols)
{
    size_t rows = 0;
    size_t cols = 0;
    for (; content.size > 0; ++rows) {
        StringView line = split_sv(&content, '\n');
        size_t col = 0;
        for (; line.size > 0; ++col) {
            StringView cell = split_sv(&line, '|');
        }
        if (cols < col) {
            cols = col;
        }
    }
    if (out_rows) {
        *out_rows = rows;
    }
    if (out_cols) {
        *out_cols = cols;
    }
}

Table create_table(size_t rows, size_t cols)
{
    Table table;
    table.rows = rows;
    table.cols = cols;
    table.cells = malloc(sizeof(Cell) * rows * cols);
    if (!table.cells) {
        printf("ERROR: Could not allocate memory for the table\n");
        exit(1);
    }
    memset(table.cells, 0, sizeof(Cell) * rows * cols);
    return table;
}

Cell *table_cell_at(Table *table, size_t row, size_t col)
{
    assert(table != NULL && table->cells != NULL);
    assert(row < table->rows);
    assert(col < table->cols);
    return &table->cells[row * table->cols + col];
}

int is_name(char c)
{
    return isalnum(c) || c == '_';
}

StringView next_token(StringView *source)
{
    *source = trim_sv(*source);
    if (source->size == 0) {
        return create_sv("", 0);
    }
    if (*source->data == '+') {
        return remove_prefix_sv(source, 1);
    }
    // if (isdigit(*source->data)) {
    //     size_t n = 0;
    //     const char *p = source->data;
    //     while (isdigit(*p)) {
    //         ++p;
    //         ++n;
    //     }
    //     return remove_prefix_sv(source, n);
    // }
    if (is_name(*source->data)) {
        size_t n = 0;
        const char *p = source->data;
        while (is_name(*p)) {
            ++p;
            ++n;
        }
        return remove_prefix_sv(source, n);
    }
    fprintf(stderr, "ERROR: unknow token starts with '%c'", *source->data);
    exit(1);
}

Expr *expr_parser(StringView *source)
{
    while (source->size > 0) {
        StringView token = next_token(source);
        printf("token: " SV_FMT "\n", SV_ARGS(token));
    }
    return NULL;
}

void table_from_content(StringView content, Table *table)
{
    assert(table != NULL && table->cells != NULL);
    size_t row = 0;
    for (; content.size > 0; ++row) {
        StringView line = split_sv(&content, '\n');
        size_t col = 0;
        for (; line.size > 0; ++col) {
            Cell *cell = table_cell_at(table, row, col);
            StringView cell_sv = trim_sv(split_sv(&line, '|'));
            if (starts_with_sv(cell_sv, "=")) {
                cell->cell_kind = CELL_KIND_EXPR;
                cell->cell_data.expr = expr_parser(&cell_sv);
            } else {
                const char *tmp = c_str_sv(cell_sv);
                char *endptr = NULL;
                cell->cell_data.number = strtod(tmp, &endptr);
                // convert success
                if (endptr != tmp && *endptr == '\0') {
                    cell->cell_kind = CELL_KIND_NUMBER;
                } else {
                    cell->cell_kind = CELL_KIND_TEXT;
                    cell->cell_data.text = cell_sv;
                }
                free((void *) tmp);
            }
        }
    }
}

void print_cell(StringView sv)
{
    while (sv.size > 0) {
        StringView line = split_sv(&sv, '\n');
        while (line.size > 0) {
            StringView cell = split_sv(&line, '|');
            print_sv(cell);
        }
    }
}

int main(int argc, char **argv)
{
    const char *str = "A1 +    69   + + B1 + C1";
    StringView sv;
    sv.data = str;
    sv.size = strlen(str);
    expr_parser(&sv);
    return 0;
}

int main1(int argc, char **argv)
{
    // if (argc < 2) {
    //     printf("ERROR: Usage mini_excel <csv>\n");
    //     exit(1);
    // }
    size_t content_size = 0;
    // char *content = read_csv(argv[1], &content_size);
    char *content = read_csv("input.csv", &content_size);
    if (!content) {
        printf("Read csv file failed\n");
        exit(1);
    }

    StringView sv = create_sv(content, content_size);
    // print_cell(sv);
    size_t rows, cols;
    estimate_table_size(sv, &rows, &cols);
    Table table = create_table(rows, cols);
    table_from_content(sv, &table);
    for (size_t i = 0; i < table.rows; ++i) {
        for (size_t j = 0; j < table.cols; ++j) {
            Cell *cell = table_cell_at(&table, i, j);
            switch (cell->cell_kind) {
                case CELL_KIND_TEXT:
                    printf("TEXT(" SV_FMT ")", SV_ARGS(cell->cell_data.text));
                    break;
                case CELL_KIND_NUMBER:
                    printf("NUMBER(%f)", cell->cell_data.number);
                    break;
                case CELL_KIND_EXPR:
                    printf("EXPR");
                    break;
            }
            putchar('|');
        }
        putchar('\n');
    }
    return 0;
}