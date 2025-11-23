#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Unidad {
    int id;
    string tipo;
    string faccion;
    string nombre;
    int puntaje;
    int numTropas;
    int movimiento;

    bool disparoEsD6;
    int disparosFijos;

    int tiradaSalvacion;
    int heridasPorMini;

    bool heridasD6;
    int heridasFijas;

    int tiradaImpactar;
    int fuerzaDisparo;
    int penetracion;
    int rango;
};

void guardarUnidad(const Unidad& u) {
    ofstream file("unidades.txt", ios::app);
    if (!file) {
        cout << "Error al crear unidades.txt\n";
        return;
    }

    file << u.id << "|"
         << u.tipo << "|"
         << u.faccion << "|"
         << u.nombre << "|"
         << u.puntaje << "|"
         << u.numTropas << "|"
         << u.movimiento << "|"

         << (u.disparoEsD6 ? "D6" : to_string(u.disparosFijos)) << "|"

         << u.tiradaSalvacion << "|"
         << u.heridasPorMini << "|"

         << (u.heridasD6 ? "D6" : to_string(u.heridasFijas)) << "|"

         << u.tiradaImpactar << "|"
         << u.fuerzaDisparo << "|"
         << u.penetracion << "|"
         << u.rango << "\n";

    file.close();
    cout << "Unidad guardada correctamente.\n";
}

void listarUnidades() {
    ifstream file("unidades.txt");
    if (!file) {
        cout << "No existe unidades.txt\n";
        return;
    }

    cout << "\n===== UNIDADES REGISTRADAS =====\n";
    string linea;
    while (getline(file, linea)) {
        cout << linea << "\n";
    }
    cout << "================================\n\n";
}

bool preguntarSiEsD6(const string& texto) {
    char opc;
    cout << texto << " ¿Es D6? (s/n): ";
    cin >> opc;
    return (opc == 's' || opc == 'S');
}

Unidad ingresarUnidad() {
    Unidad u;

    cout << "ID: ";
    cin >> u.id;
    cin.ignore();

    cout << "Tipo: ";
    getline(cin, u.tipo);

    cout << "Faccion: ";
    getline(cin, u.faccion);

    cout << "Nombre del escuadron: ";
    getline(cin, u.nombre);

    cout << "Puntaje: ";
    cin >> u.puntaje;

    cout << "Numero de miniaturas: ";
    cin >> u.numTropas;

    cout << "Movimiento (casillas): ";
    cin >> u.movimiento;

    u.disparoEsD6 = preguntarSiEsD6("¿Los disparos por miniatura son?");
    if (!u.disparoEsD6) {
        cout << "Cantidad fija de disparos: ";
        cin >> u.disparosFijos;
    }

    cout << "Tirada de salvacion (3 para 3+): ";
    cin >> u.tiradaSalvacion;

    cout << "Heridas por miniatura: ";
    cin >> u.heridasPorMini;

    u.heridasD6 = preguntarSiEsD6("¿Las heridas que causa cada disparo son?");
    if (!u.heridasD6) {
        cout << "Heridas fijas por disparo: ";
        cin >> u.heridasFijas;
    }

    cout << "Tirada para impactar (4 para 4+): ";
    cin >> u.tiradaImpactar;

    cout << "Fuerza del disparo: ";
    cin >> u.fuerzaDisparo;

    cout << "Penetracion AP: ";
    cin >> u.penetracion;

    cout << "Rango del disparo: ";
    cin >> u.rango;

    return u;
}

int main() {
    int opcion;

    while (true) {
        cout << "======= BASE DE DATOS WARHAMMER 40K =======\n";
        cout << "1. Ingresar nueva unidad\n";
        cout << "2. Listar unidades\n";
        cout << "3. Salir\n";
        cout << "Elige opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            Unidad u = ingresarUnidad();
            guardarUnidad(u);
        }
        else if (opcion == 2) {
            listarUnidades();
        }
        else if (opcion == 3) {
            break;
        }
        else {
            cout << "Opcion invalida.\n";
        }
    }

    return 0;
}
