int get_abs(int x) {
    if (x < 0) {
        x = 0 - x;
    }
    return x;
}

int main() {
    int a = 0 - 8;  // int a = -8;
    a = get_abs(a);
    printf("%d\n", a);
}