#include <iostream>
using namespace std;
int sumar(int a, int b);
int restar(int a, int b);
int multiplicar(int a, int b);
int dividir(int a, int b);
int main() {
    int num1, num2, opcion;
    cin >> num1;
    cin >> num2;
    cin >> opcion;

    if (opcion == 1) {
        cout << "Resultado de la suma: " << sumar(num1, num2) << endl;
    } else if (opcion == 2) {
        cout << "Resultado de la resta: " << restar(num1, num2) << endl;
    } else {
        cout << "Opcion no valida." << endl;
    }

    return 0;
}
int sumar(int a, int b) {
    return a + b;
}

int restar(int a, int b) {
    return a - b;
}
int multiplicar(int a, int b) {
    return a * b;
}

int dividir(int a, int b) {
    return a / b;
}

