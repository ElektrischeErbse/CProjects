#include <string.h>
#include <string_view.h>

int main()
{
    const char* create_text = "hello world";
    StringView sv1 = create_sv(create_text, strlen(create_text));
    print_sv(sv1);

    const char* trim_text = "  hello  ";
    StringView sv2 = create_sv(trim_text, strlen(trim_text));
    print_sv(trim_sv(sv2));

    const char* line_text = "hello\nworld";
    StringView sv3 = create_sv(line_text, strlen(line_text));
    while(sv3.size > 0){
        StringView line = parse_line_sv(&sv3, '\n');
        print_sv(line);
    }
    return 0;
}