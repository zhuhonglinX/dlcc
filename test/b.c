int add_one(int x) {
    return x + 1;
}

int main() {
    int a;
    a = 10;
    a = add_one(a);
    printf("a is %d\n", a);
    return 0;
}