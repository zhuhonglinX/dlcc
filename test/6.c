int fibonacci(int i) {
    if (i <= 1) {
        return 1;
    }
    return fibonacci(i - 1) + fibonacci(i - 2);
}

int main() {
    int i = 0;
    while (i < 10) {
        printf("第%d个数: %d\n", i + 1, fibonacci(i));
        i = i + 1;
    }
    return 0;
}

