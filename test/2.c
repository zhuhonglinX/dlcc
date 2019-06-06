int main() {
    int a = 10;
    if (a > 1) {
        printf("a > 1\n");
    }
    int b = 10;
    if (a == 10 && b == 10) {
        printf("a == 10 and b == 10\n");
    }
    if (a < 10 || b >= 10) {
        printf("a < 10 or b >= 10\n");
    } 

    int c;
    c = a == b;
    printf("c is %d\n", c);

    b = 0;
    c = a == b;
    printf("c is %d\n", c);

    return 0;
}