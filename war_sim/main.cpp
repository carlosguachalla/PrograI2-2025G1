#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <sstream>
using namespace std;
struct Usuario {
    string username;
    string password;
    int tipo;
};

bool cargarUsuarios(vector<Usuario>& usuarios) {
    usuarios.clear();
    ifstream file("usuarios.txt");
    if (!file.is_open()) return false;
    Usuario u;
    while (file >> u.username >> u.password >> u.tipo) {
        usuarios.push_back(u);
    }
    return true;
}

void guardarUsuarios(const vector<Usuario>& usuarios) {
    ofstream file("usuarios.txt", ios::trunc);
    if (!file.is_open()) return;
    for (const auto& u : usuarios) {
        file << u.username << " " << u.password << " " << u.tipo << '\n';
    }
}

int buscarUsuario(const vector<Usuario>& usuarios, const string& username) {
    for (size_t i = 0; i < usuarios.size(); ++i)
        if (usuarios[i].username == username) return (int)i;
    return -1;
}

bool registrarUsuarioPublico(vector<Usuario>& usuarios) {
    string user, pass;
    cout << "--- Registro de nuevo usuario (tipo 1 por defecto) ---\n";
    cout << "Nombre de usuario: "; cin >> user;

    if (buscarUsuario(usuarios, user) != -1) {
        cout << "El usuario ya existe.\n";
        return false;
    }

    cout << "Contrasena: "; cin >> pass;
    Usuario u{user, pass, 1};
    usuarios.push_back(u);
    guardarUsuarios(usuarios);
    cout << "Usuario '" << user << "' creado con exito (tipo 1).\n";
    return true;
}

int login(const vector<Usuario>& usuarios) {
    string user, pass;
    cout << "Usuario: "; cin >> user;
    cout << "Contrasena: "; cin >> pass;

    for (int i = 0; i < (int)usuarios.size(); i++) {
        if (usuarios[i].username == user && usuarios[i].password == pass) {
            return i;
        }
    }
    return -1;
}

void registrarSiVacio(vector<Usuario>& usuarios) {
    if (usuarios.size() == 0) {
        cout << "Creando admin por defecto: adm / adm (tipo 3)\n";
        Usuario a{"adm", "adm", 3};
        usuarios.push_back(a);
        guardarUsuarios(usuarios);
    }
}

void listarUsuarios(const vector<Usuario>& usuarios) {
    cout << "===== LISTA DE USUARIOS =====\n";
    for (const auto& u : usuarios) {
        cout << "Usuario: " << u.username
             << " | Tipo: " << u.tipo << "\n";
    }
}

struct Arma {
    int id;
    string nombre;
    string rango;
    string disparos;
    int impacto;
    int penetracion;
    string dano;
};

bool cargarArmas(vector<Arma>& armas) {
    armas.clear();
    ifstream f("armas.txt");
    if (!f.is_open()) return false;
    Arma a;
    while (f >> a.id >> a.nombre >> a.rango >> a.disparos >> a.impacto >> a.penetracion >> a.dano) {
        armas.push_back(a);
    }
    return true;
}

void guardarArmas(const vector<Arma>& armas) {
    ofstream f("armas.txt", ios::trunc);
    if (!f.is_open()) return;
    for (const auto& a : armas) {
        f << a.id << " " << a.nombre << " " << a.rango << " " << a.disparos << " "
          << a.impacto << " " << a.penetracion << " " << a.dano << '\n';
    }
}

void mostrarArmas(const vector<Arma>& armas) {
    cout << "===== LISTA DE ARMAS =====\n";
    for (const auto& a : armas) {
        cout << "ID: " << a.id
             << " | Nombre: " << a.nombre
             << " | Rango: " << a.rango
             << " | Disparos: " << a.disparos
             << " | Impacto(0=directo): " << a.impacto
             << " | Penetracion: " << a.penetracion
             << " | Daño: " << a.dano << "\n";
    }
}

void mostrarArmasSimple(const vector<Arma>& armas) {
    cout << "----- ARMAS DISPONIBLES (ID : Nombre) -----\n";
    for (const auto& a : armas) {
        cout << a.id << " : " << a.nombre << "\n";
    }
}

void agregarArma(vector<Arma>& armas) {
    Arma a;
    cout << "ID del arma: "; cin >> a.id;
    for (const auto& ex : armas) {
        if (ex.id == a.id) { cout << "ID ya existe. Cancelado.\n"; return; }
    }
    cout << "Nombre del arma (sin espacios): "; cin >> a.nombre;
    cout << "Rango de disparo (ej: 12\\\" o corto): "; cin >> a.rango;
    cout << "Cantidad de disparos (numero o D6): "; cin >> a.disparos;
    cout << "Impacto necesario (0 si impacto directo): "; cin >> a.impacto;
    cout << "Penetracion: "; cin >> a.penetracion;
    cout << "Daño por disparo (numero o D6): "; cin >> a.dano;

    armas.push_back(a);
    guardarArmas(armas);
    cout << "Arma agregada.\n";
}

void editarArma(vector<Arma>& armas) {
    int id; cout << "ID del arma a editar: "; cin >> id;
    for (auto& a : armas) {
        if (a.id == id) {
            cout << "Editando arma: " << a.nombre << "\n";
            cout << "Nuevo nombre (sin espacios): "; cin >> a.nombre;
            cout << "Nuevo rango: "; cin >> a.rango;
            cout << "Nueva cantidad de disparos: "; cin >> a.disparos;
            cout << "Nuevo impacto (0 si directo): "; cin >> a.impacto;
            cout << "Nueva penetracion: "; cin >> a.penetracion;
            cout << "Nuevo daño: "; cin >> a.dano;
            guardarArmas(armas);
            cout << "Arma actualizada.\n";
            return;
        }
    }
    cout << "ID no encontrado.\n";
}

void eliminarArma(vector<Arma>& armas) {
    int id; cout << "ID del arma a eliminar: "; cin >> id;
    for (size_t i = 0; i < armas.size(); i++) {
        if (armas[i].id == id) {
            armas.erase(armas.begin() + i);
            guardarArmas(armas);
            cout << "Arma eliminada.\n";
            return;
        }
    }
    cout << "ID no encontrado.\n";
}
struct Unidad {
    int id;
    string tipo;
    string faccion;
    string nombre;
    int puntaje;
    int numTropas;
    int movimiento;
    int salvacion;
    string heridas;
    int impacto;
    string armas;
};
bool cargarCatalogoGeneral(vector<Unidad>& catalogo) {
    catalogo.clear();
    ifstream f("catalogo_general.txt");
    if (!f.is_open()) return false;

    Unidad u;
    while (f >> u.id >> u.tipo >> u.faccion >> u.nombre >> u.puntaje >>
           u.numTropas >> u.movimiento >> u.salvacion >>
           u.heridas >> u.impacto >> u.armas) {

        catalogo.push_back(u);
    }
    return true;
}

void guardarCatalogoGeneral(const vector<Unidad>& catalogo) {
    ofstream f("catalogo_general.txt", ios::trunc);
    if (!f.is_open()) return;

    for (const auto& u : catalogo) {
        f << u.id << " " << u.tipo << " " << u.faccion << " " << u.nombre << " "
          << u.puntaje << " " << u.numTropas << " " << u.movimiento << " "
          << u.salvacion << " " << u.heridas << " " << u.impacto << " " << u.armas << '\n';
    }
}

void mostrarCatalogo(const vector<Unidad>& catalogo) {
    cout << "===== CATALOGO GENERAL =====\n";
    for (const auto& u : catalogo) {
        cout << "ID: " << u.id << " | Nombre: " << u.nombre << " | Faccion: " << u.faccion
             << " | Pts: " << u.puntaje << " | Tropas: " << u.numTropas << "\n";
        if (!u.armas.empty() && u.armas != "-") {
            cout << "  Armas asignadas: ";
            cout << u.armas << "\n"; // formato id:count,id:count
        } else {
            cout << "  Armas asignadas: (ninguna)\n";
        }
    }
}
int sumaArmasEnCadena(const string& armasCadena) {
    if (armasCadena.empty() || armasCadena == "-") return 0;
    int total = 0;
    stringstream ss(armasCadena);
    string token;
    while (getline(ss, token, ',')) {
        if (token.empty()) continue;
        size_t pos = token.find(':');
        if (pos == string::npos) continue;
        string cnts = token.substr(pos + 1);
        int cnt = stoi(cnts);
        total += cnt;
    }
    return total;
}
string asignarArmasAUnidad(const vector<Arma>& armasDisponibles, int numTropas) {
    if (armasDisponibles.empty()) {
        cout << "No hay armas cargadas en el sistema. Se asignará '-' (ninguna).\n";
        return string("-");
    }

    mostrarArmasSimple(armasDisponibles);
    cout << "Asigne armas a la unidad.\n";
    cout << "Reglas: la suma total de cantidades no puede exceder numTropas (" << numTropas << ").\n";
    cout << "Ingrese pares 'id cantidad'. Para terminar, ingrese '0 0'.\n";

    int idSel = -1, cntSel = -1;
    int suma = 0;
    vector<pair<int,int>> asignadas;

    while (true) {
        cout << "id cantidad: ";
        cin >> idSel >> cntSel;
        if (!cin) {
            cin.clear();
            string tmp;
            getline(cin, tmp);
            cout << "Entrada invalida. Intentelo de nuevo.\n";
            continue;
        }
        if (idSel == 0 && cntSel == 0) break;
        if (cntSel < 0) {
            cout << "Cantidad invalida.\n";
            continue;
        }
        bool found = false;
        for (const auto& a : armasDisponibles) {
            if (a.id == idSel) { found = true; break; }
        }
        if (!found) {
            cout << "ID de arma no encontrado. Vuelva a intentar.\n";
            continue;
        }
        if (suma + cntSel > numTropas) {
            cout << "No puedes asignar esa cantidad: excede numTropas. (" << suma << " + " << cntSel << " > " << numTropas << ")\n";
            continue;
        }

        bool merged = false;
        for (auto& p : asignadas) {
            if (p.first == idSel) { p.second += cntSel; merged = true; break; }
        }
        if (!merged) asignadas.emplace_back(idSel, cntSel);
        suma += cntSel;
        cout << "Asignado. Total asignado ahora: " << suma << " / " << numTropas << "\n";
        if (suma == numTropas) {
            cout << "Has alcanzado el maximo (numTropas). Terminando asignacion.\n";
            break;
        }
    }

    if (asignadas.empty()) return string("-");

    stringstream out;
    for (size_t i = 0; i < asignadas.size(); ++i) {
        out << asignadas[i].first << ":" << asignadas[i].second;
        if (i + 1 < asignadas.size()) out << ",";
    }
    return out.str();
}

void agregarUnidad(vector<Unidad>& catalogo, const vector<Arma>& armasDisponibles) {
    Unidad u;
    cout << "ID: "; cin >> u.id;

    for (const auto& ex : catalogo) {
        if (ex.id == u.id) { cout << "ID ya existe. Cancelado.\n"; return; }
    }

    cout << "Tipo: "; cin >> u.tipo;
    cout << "Faccion: "; cin >> u.faccion;
    cout << "Nombre: "; cin >> u.nombre;
    cout << "Puntaje: "; cin >> u.puntaje;
    cout << "Cantidad de tropas: "; cin >> u.numTropas;
    cout << "Movimiento: "; cin >> u.movimiento;
    cout << "Salvacion: "; cin >> u.salvacion;
    cout << "Heridas: "; cin >> u.heridas;
    cout << "Impacto: "; cin >> u.impacto;

    u.armas = asignarArmasAUnidad(armasDisponibles, u.numTropas);

    catalogo.push_back(u);
    guardarCatalogoGeneral(catalogo);
    cout << "Unidad agregada al catalogo.\n";
}

void editarUnidad(vector<Unidad>& catalogo, const vector<Arma>& armasDisponibles) {
    int id;
    cout << "ID de la unidad a editar: ";
    cin >> id;

    for (auto& u : catalogo) {
        if (u.id == id) {
            cout << "Editando unidad: " << u.nombre << "\n";
            cout << "Nuevo tipo: "; cin >> u.tipo;
            cout << "Nueva faccion: "; cin >> u.faccion;
            cout << "Nuevo nombre: "; cin >> u.nombre;
            cout << "Nuevo puntaje: "; cin >> u.puntaje;
            cout << "Nueva cantidad de tropas: "; cin >> u.numTropas;
            cout << "Nuevo movimiento: "; cin >> u.movimiento;
            cout << "Nuevos disparos: (Ya no aplica a unidad, se ignora)\n";
            cout << "Nueva salvacion: "; cin >> u.salvacion;
            cout << "Nuevas heridas: "; cin >> u.heridas;
            cout << "Nuevo impacto: "; cin >> u.impacto;

            cout << "Asignacion de armas actual: " << (u.armas.empty() ? "-" : u.armas) << "\n";
            cout << "¿Deseas re-asignar armas? 1=Si 0=No: ";
            int ra; cin >> ra;
            if (ra == 1) {
                u.armas = asignarArmasAUnidad(armasDisponibles, u.numTropas);
            }

            cout << "Unidad actualizada.\n";
            return;
        }
    }
    cout << "ID no encontrado.\n";
}

void eliminarUnidad(vector<Unidad>& catalogo) {
    int id;
    cout << "ID de la unidad a eliminar: ";
    cin >> id;

    for (size_t i = 0; i < catalogo.size(); i++) {
        if (catalogo[i].id == id) {
            catalogo.erase(catalogo.begin() + i);
            cout << "Unidad eliminada del catalogo.\n";
            return;
        }
    }
    cout << "ID no encontrado.\n";
}

string archivoPersonal(const string& username) {
    return "personal_" + username + ".txt";
}

bool cargarCatalogoPersonal(const string& username, vector<int>& ejercito) {
    ejercito.clear();
    ifstream f(archivoPersonal(username));
    if (!f.is_open()) return false;

    int id;
    while (f >> id) ejercito.push_back(id);
    return true;
}

void guardarCatalogoPersonal(const string& username, const vector<int>& ejercito) {
    ofstream f(archivoPersonal(username), ios::trunc);
    if (!f.is_open()) return;

    for (int id : ejercito) f << id << '\n';
}

void agregarAlEjercito(const string& username, vector<int>& ejercito, const vector<Unidad>& catalogo) {
    int id;
    cout << "ID de unidad a agregar: "; cin >> id;

    for (const auto& u : catalogo) {
        if (u.id == id) {
            ejercito.push_back(id);
            guardarCatalogoPersonal(username, ejercito);
            cout << "Unidad agregada.\n";
            return;
        }
    }
    cout << "ID no encontrado.\n";
}

void eliminarDelEjercito(const string& username, vector<int>& ejercito) {
    int id;
    cout << "ID de la unidad a eliminar del ejercito: ";
    cin >> id;

    for (size_t i = 0; i < ejercito.size(); i++) {
        if (ejercito[i] == id) {
            ejercito.erase(ejercito.begin() + i);
            guardarCatalogoPersonal(username, ejercito);
            cout << "Unidad eliminada.\n";
            return;
        }
    }
    cout << "Esa unidad no esta en tu ejercito.\n";
}

void mostrarEjercito(const vector<int>& ejercito, const vector<Unidad>& catalogo) {
    cout << "===== EJERCITO PERSONAL =====\n";
    for (int id : ejercito) {
        bool found = false;
        for (const auto& u : catalogo) {
            if (u.id == id) {
                cout << "ID: " << u.id << " | " << u.nombre << "\n";
                if (!u.armas.empty() && u.armas != "-") {
                    cout << "  Armas: " << u.armas << "\n";
                } else {
                    cout << "  Armas: (ninguna)\n";
                }
                found = true;
                break;
            }
        }
        if (!found)
            cout << "ID: " << id << " | (no encontrado en catalogo)\n";
    }
}

int main() {
    vector<Usuario> usuarios;
    cargarUsuarios(usuarios);
    registrarSiVacio(usuarios);

    vector<Arma> armas;
    cargarArmas(armas);

    while (true) {
        cout << "=== SISTEMA DE LOGIN / REGISTRO ===\n"
             << "1) Iniciar sesion\n"
             << "2) Registrar nuevo usuario (tipo 1)\n"
             << "3) Salir\nElige: ";

        int preop; cin >> preop;

        if (preop == 2) {
            registrarUsuarioPublico(usuarios);
            continue;
        }
        else if (preop == 3) {
            cout << "Saliendo...\n";
            return 0;
        }
        else if (preop != 1) {
            cout << "Opcion invalida.\n";
            continue;
        }

        int idUser = login(usuarios);
        if (idUser == -1) {
            cout << "Login incorrecto.\n";
            continue;
        }

        Usuario &actual = usuarios[idUser];
        cout << "Bienvenido " << actual.username << " (Tipo: " << actual.tipo << ")\n";

        vector<Unidad> catalogo;
        cargarCatalogoGeneral(catalogo);

        vector<int> ejercito;
        cargarCatalogoPersonal(actual.username, ejercito);

        int op;
        while (true) {

            if (actual.tipo == 1) {
                cout << "\n===== MENU JUGADOR =====\n"
                     << "1. Ver catalogo general\n"
                     << "2. Ver catalogo personal\n"
                     << "3. Agregar unidad al ejercito\n"
                     << "4. Eliminar unidad del ejercito\n"
                     << "5. Salir\n";
                cout << "Opcion: "; cin >> op;

                if (op == 1) mostrarCatalogo(catalogo);
                else if (op == 2) mostrarEjercito(ejercito, catalogo);
                else if (op == 3) agregarAlEjercito(actual.username, ejercito, catalogo);
                else if (op == 4) eliminarDelEjercito(actual.username, ejercito);
                else if (op == 5) break;
                else cout << "Opcion invalida.\n";
            }

            else if (actual.tipo == 2) {
                cout << "\n===== MENU EDITOR =====\n"
                     << "1. Ver catalogo general\n"
                     << "2. Agregar unidad al catalogo\n"
                     << "3. Editar unidad\n"
                     << "4. Eliminar unidad\n"
                     << "5. Ver catalogo personal\n"
                     << "6. Agregar unidad al ejercito\n"
                     << "----- ARMERIA -----\n"
                     << "7. Ver armas\n"
                     << "8. Agregar arma\n"
                     << "9. Editar arma\n"
                     << "10. Eliminar arma\n"
                     << "11. Salir\n";

                cout << "Opcion: "; cin >> op;

                if (op == 1) mostrarCatalogo(catalogo);
                else if (op == 2) agregarUnidad(catalogo, armas);
                else if (op == 3) editarUnidad(catalogo, armas);
                else if (op == 4) eliminarUnidad(catalogo);
                else if (op == 5) mostrarEjercito(ejercito, catalogo);
                else if (op == 6) agregarAlEjercito(actual.username, ejercito, catalogo);
                else if (op == 7) mostrarArmas(armas);
                else if (op == 8) { agregarArma(armas); }
                else if (op == 9) { editarArma(armas); }
                else if (op == 10) { eliminarArma(armas); }
                else if (op == 11) break;
                else cout << "Opcion invalida.\n";

                guardarCatalogoGeneral(catalogo);
                guardarArmas(armas);
            }

            else if (actual.tipo == 3) {
                cout << "\n===== MENU ADMIN =====\n"
                     << "1. Ver catalogo general\n"
                     << "2. Agregar unidad al catalogo\n"
                     << "3. Administrar usuarios\n"
                     << "4. Ver catalogo personal\n"
                     << "5. Agregar unidad al ejercito\n"
                     << "6. Editar unidad del catalogo\n"
                     << "7. Eliminar unidad del catalogo\n"
                     << "8. Ver lista de usuarios\n"
                     << "9. (reservado)\n"
                     << "----- ARMERIA -----\n"
                     << "10. Ver armas\n"
                     << "11. Agregar arma\n"
                     << "12. Editar arma\n"
                     << "13. Eliminar arma\n"
                     << "14. Salir\n";

                cout << "Opcion: "; cin >> op;

                if (op == 1) mostrarCatalogo(catalogo);
                else if (op == 2) agregarUnidad(catalogo, armas);
                else if (op == 3) {

                    cout << "--- Administracion de usuarios ---\n"
                         << "1) Crear usuario\n"
                         << "2) Eliminar usuario\n"
                         << "3) Cambiar tipo de usuario\n"
                         << "Elige: ";
                    int aop; cin >> aop;

                    if (aop == 1) registrarUsuarioPublico(usuarios);

                    else if (aop == 2) {
                        cout << "Usuario a eliminar: ";
                        string udel; cin >> udel;

                        int idx = buscarUsuario(usuarios, udel);
                        if (idx == -1) cout << "Usuario no existe.\n";
                        else {
                            string filename = archivoPersonal(udel);
                            remove(filename.c_str());

                            usuarios.erase(usuarios.begin() + idx);
                            guardarUsuarios(usuarios);
                            cout << "Usuario eliminado.\n";
                        }
                    }

                    else if (aop == 3) {
                        cout << "Usuario a modificar: ";
                        string umod; cin >> umod;

                        int idx = buscarUsuario(usuarios, umod);
                        if (idx == -1) cout << "Usuario no existe.\n";
                        else {
                            cout << "Nuevo tipo (1,2,3): ";
                            int nt; cin >> nt;

                            if (nt < 1 || nt > 3)
                                cout << "Tipo invalido.\n";
                            else {
                                usuarios[idx].tipo = nt;
                                guardarUsuarios(usuarios);
                                cout << "Tipo actualizado.\n";
                            }
                        }
                    }

                    else cout << "Opcion invalida.\n";

                }
                else if (op == 4) mostrarEjercito(ejercito, catalogo);
                else if (op == 5) agregarAlEjercito(actual.username, ejercito, catalogo);
                else if (op == 6) editarUnidad(catalogo, armas);
                else if (op == 7) eliminarUnidad(catalogo);
                else if (op == 8) listarUsuarios(usuarios);
                else if (op == 10) mostrarArmas(armas);
                else if (op == 11) { agregarArma(armas); }
                else if (op == 12) { editarArma(armas); }
                else if (op == 13) { eliminarArma(armas); }
                else if (op == 14) break;
                else cout << "Opcion invalida.\n";

                guardarCatalogoGeneral(catalogo);
                guardarUsuarios(usuarios);
                guardarArmas(armas);
            }
        }

        guardarUsuarios(usuarios);
        guardarCatalogoGeneral(catalogo);
        guardarArmas(armas);
    }

    return 0;
}
