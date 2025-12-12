#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <sstream>
#include <random>
#include <cctype>
#include <unordered_map>
#include <cmath>
#include <queue>
#include <climits>
using namespace std;
const int H_CAMPO = 44;
const int W_CAMPO = 90;
struct Usuario {
    string username;
    string password;
    int tipo;
};
struct Arma {
    int id;
    string nombre;
    string rango;
    string disparos;
    int impacto;
    int penetracion;
    string dano;
    int fuerza;
};
struct ArmaCuerpo {
    int id;
    string nombre;
    int impacto;
    int penetracion;
    string dano;
    int fuerza;
    int golpes;
};
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
    string armasCuerpo;
};
struct UnidadInst {
    int idBase;
    string codigo;
    string nombre;
    bool desplegada;
    int fila;
    int col;
    int sizeH;
    int sizeW;
    int tropasActuales;
    int tropasMax;
    int hp;
    int hpMax;
    int movimiento;
    int salvacion;
};
bool cargarUsuarios(vector<Usuario>& usuarios);
void guardarUsuarios(const vector<Usuario>& usuarios);
int buscarUsuario(const vector<Usuario>& usuarios, const string& username);
bool registrarUsuarioPublico(vector<Usuario>& usuarios);
int login(const vector<Usuario>& usuarios);
void registrarSiVacio(vector<Usuario>& usuarios);
void listarUsuarios(const vector<Usuario>& usuarios);
bool cargarArmas(vector<Arma>& armas);
void guardarArmas(const vector<Arma>& armas);
bool cargarArmasCuerpo(vector<ArmaCuerpo>& armasC);
void guardarArmasCuerpo(const vector<ArmaCuerpo>& armasC);
void mostrarArmas(const vector<Arma>& armas);
void mostrarArmasCuerpo(const vector<ArmaCuerpo>& armasC);
void mostrarArmasSimple(const vector<Arma>& armas);
void mostrarArmasCuerpoSimple(const vector<ArmaCuerpo>& armasC);
bool cargarCatalogoGeneral(vector<Unidad>& catalogo);
void guardarCatalogoGeneral(const vector<Unidad>& catalogo);
void mostrarCatalogo(const vector<Unidad>& catalogo);
string archivoPersonal(const string& username);
bool cargarCatalogoPersonal(const string& username, vector<int>& ejercito);
void guardarCatalogoPersonal(const string& username, const vector<int>& ejercito);
void agregarAlEjercito(const string& username, vector<int>& ejercito, const vector<Unidad>& catalogo);
void eliminarDelEjercito(const string& username, vector<int>& ejercito);
void mostrarEjercito(const vector<int>& ejercito, const vector<Unidad>& catalogo);
vector<vector<string>> generarMapaObstaculos();
void mostrarMapa(const vector<vector<string>>& map);
string toLowerCopy(const string& s);
vector<UnidadInst> generarInstanciasDesdeEjercito(const vector<int>& ejercito, const vector<Unidad>& catalogo);
int buscarInstanciaPorCodigo(vector<UnidadInst>& inst, const string& codigo);
bool dentroMapa(int fila, int col, int H, int W);
bool celdasLibres(const vector<vector<string>>& map, int fila, int col, int H, int W);
void colocarUnidadEnMapa(vector<vector<string>>& map, const UnidadInst& u);
void quitarUnidadDelMapa(vector<vector<string>>& map, const UnidadInst& u);
bool filaEnZonaDespliegueJugador(int fila);
void modoDespliegue(vector<vector<string>>& map, vector<UnidadInst>& instancias);
void mostrarPanelEstado(const vector<UnidadInst>& instancias);
void modoMovimiento(vector<vector<string>>& map, vector<UnidadInst>& instancias);
int sumaArmasEnCadena(const string& armasCadena);
string asignarArmasAUnidad(const vector<Arma>& armasDisponibles, int numTropas);
string asignarArmasCuerpoAUnidad(const vector<ArmaCuerpo>& armasDisponibles, int numTropas);
void agregarArma(vector<Arma>& armas);
void editarArma(vector<Arma>& armas);
void eliminarArma(vector<Arma>& armas);
void agregarArmaCuerpo(vector<ArmaCuerpo>& armasC);
void editarArmaCuerpo(vector<ArmaCuerpo>& armasC);
void eliminarArmaCuerpo(vector<ArmaCuerpo>& armasC);
void agregarUnidad(vector<Unidad>& catalogo, const vector<Arma>& armasDisponibles, const vector<ArmaCuerpo>& armasCDisponibles);
void editarUnidad(vector<Unidad>& catalogo, const vector<Arma>& armasDisponibles, const vector<ArmaCuerpo>& armasCDisponibles);
void eliminarUnidad(vector<Unidad>& catalogo);
string seleccionarFaccionIA();
int calcularPuntosEjercito(const vector<int>& ejercito, const vector<Unidad>& catalogo);
vector<int> generarEjercitoIA(const vector<Unidad>& catalogo, const string& faccion, int puntosObjetivo);
void guardarEjercitoIA(const string& faccion, const vector<int>& ejercitoIA, const vector<Unidad>& catalogo);
void desplegarEjercitoIAEnMapa(vector<vector<string>>& mapa, vector<UnidadInst>& instanciasIA);
vector<int> idsDeFaccion(const vector<Unidad>& catalogo, const string& faccion);

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

bool cargarArmas(vector<Arma>& armas) {
    armas.clear();
    ifstream f("armas.txt");
    if (!f.is_open()) return false;
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Arma a;
        a.id = 0; a.nombre = "-"; a.rango = "-"; a.disparos = "-"; a.impacto = 0; a.penetracion = 0; a.dano = "-"; a.fuerza = 0;
        if (!(ss >> a.id >> a.nombre >> a.rango >> a.disparos >> a.impacto >> a.penetracion >> a.dano)) continue;
        if (!(ss >> a.fuerza)) a.fuerza = 0;
        armas.push_back(a);
    }
    return true;
}

void guardarArmas(const vector<Arma>& armas) {
    ofstream f("armas.txt", ios::trunc);
    if (!f.is_open()) return;
    for (const auto& a : armas) {
        f << a.id << " " << a.nombre << " " << a.rango << " " << a.disparos << " "
          << a.impacto << " " << a.penetracion << " " << a.dano << " " << a.fuerza << '\n';
    }
}

bool cargarArmasCuerpo(vector<ArmaCuerpo>& armasC) {
    armasC.clear();
    ifstream f("armas_cuerpo.txt");
    if (!f.is_open()) return false;
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        ArmaCuerpo a;
        a.id = 0; a.nombre = "-"; a.impacto = 0; a.penetracion = 0; a.dano = "-"; a.fuerza = 0; a.golpes = 1;
        if (!(ss >> a.id >> a.nombre >> a.impacto >> a.penetracion >> a.dano)) continue;
        if (!(ss >> a.fuerza)) a.fuerza = 0;
        if (!(ss >> a.golpes)) a.golpes = 1;
        armasC.push_back(a);
    }
    return true;
}

void guardarArmasCuerpo(const vector<ArmaCuerpo>& armasC) {
    ofstream f("armas_cuerpo.txt", ios::trunc);
    if (!f.is_open()) return;
    for (const auto& a : armasC) {
        f << a.id << " " << a.nombre << " " << a.impacto << " " << a.penetracion << " " << a.dano << " " << a.fuerza << " " << a.golpes << '\n';
    }
}

void mostrarArmas(const vector<Arma>& armas) {
    cout << "===== LISTA DE ARMAS A DISTANCIA =====\n";
    for (const auto& a : armas) {
        cout << "ID: " << a.id
             << " | Nombre: " << a.nombre
             << " | Rango: " << a.rango
             << " | Disparos: " << a.disparos
             << " | Impacto(0=directo): " << a.impacto
             << " | Penetracion: " << a.penetracion
             << " | Dao: " << a.dano
             << " | Fuerza: " << a.fuerza
             << "\n";
    }
}

void mostrarArmasCuerpo(const vector<ArmaCuerpo>& armasC) {
    cout << "===== LISTA DE ARMAS CUERPO A CUERPO =====\n";
    for (const auto& a : armasC) {
        cout << "ID: " << a.id
             << " | Nombre: " << a.nombre
             << " | Impacto(0=directo): " << a.impacto
             << " | Penetracion: " << a.penetracion
             << " | Dao: " << a.dano
             << " | Fuerza: " << a.fuerza
             << " | Golpes: " << a.golpes
             << "\n";
    }
}

void mostrarArmasSimple(const vector<Arma>& armas) {
    cout << "----- ARMAS A DISTANCIA DISPONIBLES (ID : Nombre) -----\n";
    for (const auto& a : armas) {
        cout << a.id << " : " << a.nombre << "\n";
    }
}

void mostrarArmasCuerpoSimple(const vector<ArmaCuerpo>& armasC) {
    cout << "----- ARMAS CUERPO A CUERPO DISPONIBLES (ID : Nombre) -----\n";
    for (const auto& a : armasC) {
        cout << a.id << " : " << a.nombre << "\n";
    }
}

bool cargarCatalogoGeneral(vector<Unidad>& catalogo) {
    catalogo.clear();
    ifstream f("catalogo_general.txt");
    if (!f.is_open()) return false;
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Unidad u;
        if (!(ss >> u.id >> u.tipo >> u.faccion >> u.nombre >> u.puntaje >>
              u.numTropas >> u.movimiento >> u.salvacion >> u.heridas >> u.impacto)) {
            continue;
        }
        if (!(ss >> u.armas)) u.armas = "-";
        if (!(ss >> u.armasCuerpo)) u.armasCuerpo = "-";
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
          << u.salvacion << " " << u.heridas << " " << u.impacto << " "
          << (u.armas.empty() ? "-" : u.armas) << " "
          << (u.armasCuerpo.empty() ? "-" : u.armasCuerpo) << '\n';
    }
}

void mostrarCatalogo(const vector<Unidad>& catalogo) {
    cout << "===== CATALOGO GENERAL =====\n";
    for (const auto& u : catalogo) {
        cout << "ID: " << u.id << " | Nombre: " << u.nombre << " | Faccion: " << u.faccion
             << " | Pts: " << u.puntaje << " | Tropas: " << u.numTropas << "\n";
        if (!u.armas.empty() && u.armas != "-") {
            cout << "  Armas a distancia asignadas: " << u.armas << "\n";
        } else {
            cout << "  Armas a distancia asignadas: (ninguna)\n";
        }
        if (!u.armasCuerpo.empty() && u.armasCuerpo != "-") {
            cout << "  Armas cuerpo a cuerpo asignadas: " << u.armasCuerpo << "\n";
        } else {
            cout << "  Armas cuerpo a cuerpo asignadas: (ninguna)\n";
        }
    }
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
                    cout << "  Armas a distancia: " << u.armas << "\n";
                } else {
                    cout << "  Armas a distancia: (ninguna)\n";
                }
                if (!u.armasCuerpo.empty() && u.armasCuerpo != "-") {
                    cout << "  Armas cuerpo a cuerpo: " << u.armasCuerpo << "\n";
                } else {
                    cout << "  Armas cuerpo a cuerpo: (ninguna)\n";
                }
                found = true;
                break;
            }
        }
        if (!found) cout << "ID: " << id << " | (no encontrado en catalogo)\n";
    }
}
vector<vector<string>> generarMapaObstaculos() {
    struct Estructura { int h, w; };
    vector<Estructura> types = {
        {1, 3},
        {4, 4},
        {3, 1}
    };

    vector<vector<string>> map(H_CAMPO, vector<string>(W_CAMPO, "."));

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> distType(0, (int)types.size() - 1);

    int totalCells = H_CAMPO * W_CAMPO;
    int minCells = totalCells * 20 / 100;
    int maxCells = totalCells * 30 / 100;
    int usedCells = 0;

    int intentos = 0;
    const int MAX_INTENTOS = 100000;

    while (usedCells < minCells && intentos < MAX_INTENTOS) {
        intentos++;
        Estructura e = types[distType(rng)];
        uniform_int_distribution<int> distH(0, H_CAMPO - e.h);
        uniform_int_distribution<int> distW(0, W_CAMPO - e.w);
        int r = distH(rng);
        int c = distW(rng);

        bool puede = true;
        for (int i = 0; i < e.h; i++) {
            for (int j = 0; j < e.w; j++) {
                if (map[r+i][c+j] == "#") { puede = false; break; }
            }
            if (!puede) break;
        }
        if (!puede) continue;

        for (int i = 0; i < e.h; i++) {
            for (int j = 0; j < e.w; j++) {
                map[r+i][c+j] = "#";
                usedCells++;
            }
        }

        if (usedCells > maxCells) {
            for (int i = 0; i < e.h; i++) {
                for (int j = 0; j < e.w; j++) {
                    if (map[r+i][c+j] == "#") { map[r+i][c+j] = "."; usedCells--; }
                }
            }
            break;
        }
    }

    return map;
}

void mostrarMapa(const vector<vector<string>>& map) {
    cout << "===== MAPA (cada celda 2 chars) =====\n";
    for (int i = 0; i < H_CAMPO; ++i) {
        for (int j = 0; j < W_CAMPO; ++j) {
            string c = map[i][j];
            if (c == ".") cout << ". ";
            else if (c == "#") cout << "# ";
            else {

                if (c.size() >= 2) cout << c.substr(0,2);
                else { cout << c << ' '; }
            }
        }
        cout << "\n";
    }
}
string toLowerCopy(const string& s) {
    string out = s;
    for (char &c : out) c = (char)tolower((unsigned char)c);
    return out;
}
vector<UnidadInst> generarInstanciasDesdeEjercito(const vector<int>& ejercito, const vector<Unidad>& catalogo) {
    vector<UnidadInst> inst;
    unordered_map<int,int> contador;
    for (int baseId : ejercito) {
        const Unidad* ub = nullptr;
        for (const auto& u : catalogo) if (u.id == baseId) { ub = &u; break; }
        if (!ub) continue;

        int idx = contador[baseId]++;
        char letra = 'A' + (idx % 26);
        string code = to_string(baseId) + letra;

        UnidadInst ui;
        ui.idBase = baseId;
        ui.codigo = code;
        ui.nombre = ub->nombre;
        ui.desplegada = false;
        ui.fila = -1; ui.col = -1;
        ui.movimiento = ub->movimiento;
        ui.salvacion = ub->salvacion;
        ui.tropasMax = ub->numTropas;
        ui.tropasActuales = ub->numTropas;
        ui.hp = 0; ui.hpMax = 0;

        string tipoLow = toLowerCopy(ub->tipo);
        if (tipoLow.find("infanter") != string::npos || tipoLow.find("infantry") != string::npos) {
            ui.sizeH = 1; ui.sizeW = 1;

            ui.tropasActuales = ub->numTropas;
            ui.hp = 0; ui.hpMax = 0;
        } else if (tipoLow.find("tanque") != string::npos || tipoLow.find("tank") != string::npos || tipoLow.find("tanke") != string::npos) {
            ui.sizeH = 1; ui.sizeW = 2;
            ui.tropasActuales = 1;
            ui.hp = ub->numTropas * 10;
            ui.hpMax = ui.hp;
        } else {
            if (ub->numTropas > 1) {
                ui.sizeH = 1; ui.sizeW = 1;
                ui.tropasActuales = ub->numTropas;
                ui.hp = 0; ui.hpMax = 0;
            } else {
                ui.sizeH = 1; ui.sizeW = 2;
                ui.tropasActuales = 1;
                ui.hp = ub->numTropas * 10;
                ui.hpMax = ui.hp;
            }
        }

        inst.push_back(ui);
    }
    return inst;
}
int buscarInstanciaPorCodigo(vector<UnidadInst>& inst, const string& codigo) {
    for (size_t i = 0; i < inst.size(); ++i) if (inst[i].codigo == codigo) return (int)i;
    return -1;
}
bool dentroMapa(int fila, int col, int H, int W) {
    if (fila < 0 || col < 0) return false;
    if (fila + H > H_CAMPO) return false;
    if (col + W > W_CAMPO) return false;
    return true;
}
bool celdasLibres(const vector<vector<string>>& map, int fila, int col, int H, int W) {
    if (!dentroMapa(fila, col, H, W)) return false;
    for (int r = 0; r < H; ++r) for (int c = 0; c < W; ++c) {
        string val = map[fila + r][col + c];
        if (val != ".") return false;
    }
    return true;
}
void colocarUnidadEnMapa(vector<vector<string>>& map, const UnidadInst& u) {
    if (u.fila < 0 || u.col < 0) return;
    for (int r = 0; r < u.sizeH; ++r) for (int c = 0; c < u.sizeW; ++c) {
        map[u.fila + r][u.col + c] = u.codigo;
    }
}
void quitarUnidadDelMapa(vector<vector<string>>& map, const UnidadInst& u) {
    if (u.fila < 0 || u.col < 0) return;
    for (int r = 0; r < u.sizeH; ++r) for (int c = 0; c < u.sizeW; ++c) {
        if (map[u.fila + r][u.col + c] == u.codigo) map[u.fila + r][u.col + c] = ".";
    }
}
bool filaEnZonaDespliegueJugador(int fila) {
    return (fila >= (H_CAMPO - 5) && fila < H_CAMPO);
}
void modoDespliegue(vector<vector<string>>& map, vector<UnidadInst>& instancias) {
    cout << "=== MODO DESPLIEGUE ===\n";
    cout << "Zona de despliegue: filas " << (H_CAMPO - 5) << " a " << (H_CAMPO - 1) << " (inclusive)\n";
    while (true) {
        cout << "\nInstancias:\n";
        for (const auto& u : instancias) {
            cout << u.codigo << " - " << u.nombre << " - " << (u.desplegada ? "DESPLEGADA" : "POR DESPLEGAR");
            if (u.desplegada) cout << " Pos: (" << u.fila << "," << u.col << ")";
            cout << " Tamao: " << u.sizeH << "x" << u.sizeW;
            if (u.tropasMax > 1) cout << " Tropas: " << u.tropasActuales << "/" << u.tropasMax;
            else if (u.hpMax > 0) cout << " HP: " << u.hp << "/" << u.hpMax;
            cout << "\n";
        }
        cout << "\nComandos:\n";
        cout << "1) Desplegar unidad (ej: codigo fila columna)\n";
        cout << "2) Mostrar mapa\n";
        cout << "3) Terminar despliegue\n";
        cout << "Elige: ";
        int op; cin >> op;
        if (op == 1) {
            string code; int fila, col;
            cout << "Codigo unidad (ej 1A): "; cin >> code;
            int idx = buscarInstanciaPorCodigo(instancias, code);
            if (idx == -1) { cout << "Codigo no encontrado.\n"; continue; }
            if (instancias[idx].desplegada) { cout << "Unidad ya desplegada.\n"; continue; }
            cout << "Fila: "; cin >> fila;
            cout << "Columna: "; cin >> col;
            if (!filaEnZonaDespliegueJugador(fila)) {
                cout << "Fila fuera de la zona de despliegue del jugador.\n";
                continue;
            }
            if (!dentroMapa(fila, col, instancias[idx].sizeH, instancias[idx].sizeW)) {
                cout << "La unidad no cabe en esas coordenadas (fuera del mapa).\n";
                continue;
            }
            bool ok = true;
            for (int r = 0; r < instancias[idx].sizeH && ok; ++r)
                for (int c = 0; c < instancias[idx].sizeW; ++c)
                    if (map[fila + r][col + c] != ".") { ok = false; break; }
            if (!ok) { cout << "Hay un obstculo o unidad en la posicin indicadas. Intenta otra.\n"; continue; }
            instancias[idx].fila = fila;
            instancias[idx].col = col;
            instancias[idx].desplegada = true;
            colocarUnidadEnMapa(map, instancias[idx]);
            cout << "Unidad " << instancias[idx].codigo << " desplegada en (" << fila << "," << col << ").\n";
        }
        else if (op == 2) {
            mostrarMapa(map);
        }
        else if (op == 3) {
            cout << "Saliendo modo despliegue.\n";
            break;
        }
        else cout << "Opcion invalida.\n";
    }
}
void mostrarPanelEstado(const vector<UnidadInst>& instancias) {
    cout << "===== PANEL - ESTADO DEL EJERCITO =====\n";
    for (const auto& u : instancias) {
        cout << u.codigo << " | " << u.nombre << " | ";
        if (u.desplegada) cout << "Pos(" << u.fila << "," << u.col << ") ";
        else cout << "No desplegada ";
        if (u.tropasMax > 1) {
            cout << "Tropas: " << u.tropasActuales << "/" << u.tropasMax;
        } else if (u.hpMax > 0) {
            cout << "HP: " << u.hp << "/" << u.hpMax;
        } else {
            cout << "Unidades: " << u.tropasActuales << "/" << u.tropasMax;
        }
        cout << " | Mov: " << u.movimiento;
        cout << "\n";
    }
}
void modoMovimiento(vector<vector<string>>& map, vector<UnidadInst>& instancias) {
    cout << "=== MODO MOVIMIENTO ===\n";
    while (true) {
        cout << "\nComandos:\n";
        cout << "1) Mover unidad (codigo filaDest colDest)\n";
        cout << "2) Mostrar mapa\n";
        cout << "3) Mostrar panel estado\n";
        cout << "4) Salir modo movimiento\n";
        cout << "Elige: ";
        int op; cin >> op;
        if (op == 1) {
            string code; int fd, cd;
            cout << "Codigo unidad: "; cin >> code;
            int idx = buscarInstanciaPorCodigo(instancias, code);
            if (idx == -1) { cout << "Codigo no encontrado.\n"; continue; }

            if (!instancias[idx].codigo.empty() && instancias[idx].codigo[0] == 'I') {
                cout << "No puedes mover unidades de la IA manualmente.\n";
                continue;
            }
            if (!instancias[idx].desplegada) { cout << "Unidad no est desplegada.\n"; continue; }
            cout << "Destino fila: "; cin >> fd;
            cout << "Destino col: "; cin >> cd;

            if (!dentroMapa(fd, cd, instancias[idx].sizeH, instancias[idx].sizeW)) {
                cout << "Destino fuera del mapa o la unidad no cabe all.\n"; continue;
            }
            int dist = abs(instancias[idx].fila - fd) + abs(instancias[idx].col - cd);
            if (dist > instancias[idx].movimiento) {
                cout << "Destino demasiado lejos. Movimiento max: " << instancias[idx].movimiento << " (dist " << dist << ")\n";
                continue;
            }
            quitarUnidadDelMapa(map, instancias[idx]);
            bool ok = true;
            for (int r = 0; r < instancias[idx].sizeH && ok; ++r)
                for (int c = 0; c < instancias[idx].sizeW; ++c)
                    if (map[fd + r][cd + c] != ".") { ok = false; break; }
            if (!ok) {
                cout << "No se puede mover: destino ocupado por obstaculo o unidad.\n";
                colocarUnidadEnMapa(map, instancias[idx]);
                continue;
            }
            instancias[idx].fila = fd;
            instancias[idx].col = cd;
            colocarUnidadEnMapa(map, instancias[idx]);
            cout << "Unidad " << instancias[idx].codigo << " movida a (" << fd << "," << cd << ").\n";
        }
        else if (op == 2) mostrarMapa(map);
        else if (op == 3) mostrarPanelEstado(instancias);
        else if (op == 4) { cout << "Saliendo modo movimiento.\n"; break; }
        else cout << "Opcion invalida.\n";
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
        int cnt = 0;
        try { cnt = stoi(cnts); } catch(...) { cnt = 0; }
        total += cnt;
    }
    return total;
}

string asignarArmasAUnidad(const vector<Arma>& armasDisponibles, int numTropas) {
    if (armasDisponibles.empty()) {
        cout << "No hay armas a distancia cargadas en el sistema. Se asignar '-' (ninguna).\n";
        return string("-");
    }
    mostrarArmasSimple(armasDisponibles);
    cout << "Asigne armas a la unidad (armas a distancia).\n";
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

string asignarArmasCuerpoAUnidad(const vector<ArmaCuerpo>& armasDisponibles, int numTropas) {
    if (armasDisponibles.empty()) {
        cout << "No hay armas cuerpo a cuerpo cargadas en el sistema. Se asignar '-' (ninguna).\n";
        return string("-");
    }
    mostrarArmasCuerpoSimple(armasDisponibles);
    cout << "Asigne armas cuerpo a cuerpo a la unidad.\n";
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
            cout << "ID de arma cuerpo a cuerpo no encontrado. Vuelva a intentar.\n";
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
    cout << "Dao por disparo (numero o D6): "; cin >> a.dano;
    cout << "Fuerza (opcional, 0 por defecto): "; cin >> a.fuerza;

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
            cout << "Nuevo dao: "; cin >> a.dano;
            cout << "Nueva fuerza: "; cin >> a.fuerza;
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

void agregarArmaCuerpo(vector<ArmaCuerpo>& armasC) {
    ArmaCuerpo a;
    cout << "ID del arma cuerpo a cuerpo: "; cin >> a.id;
    for (const auto& ex : armasC) {
        if (ex.id == a.id) { cout << "ID ya existe. Cancelado.\n"; return; }
    }
    cout << "Nombre del arma (sin espacios): "; cin >> a.nombre;
    cout << "Impacto necesario (0 si impacto directo): "; cin >> a.impacto;
    cout << "Penetracion: "; cin >> a.penetracion;
    cout << "Dao por ataque (numero o D6): "; cin >> a.dano;
    cout << "Fuerza (opcional, 0 por defecto): "; cin >> a.fuerza;
    cout << "Golpes por asalto (opcional, 1 por defecto): "; cin >> a.golpes;

    armasC.push_back(a);
    guardarArmasCuerpo(armasC);
    cout << "Arma cuerpo a cuerpo agregada.\n";
}

void editarArmaCuerpo(vector<ArmaCuerpo>& armasC) {
    int id; cout << "ID del arma cuerpo a cuerpo a editar: "; cin >> id;
    for (auto& a : armasC) {
        if (a.id == id) {
            cout << "Editando arma cuerpo a cuerpo: " << a.nombre << "\n";
            cout << "Nuevo nombre (sin espacios): "; cin >> a.nombre;
            cout << "Nuevo impacto (0 si directo): "; cin >> a.impacto;
            cout << "Nueva penetracion: "; cin >> a.penetracion;
            cout << "Nuevo dao: "; cin >> a.dano;
            cout << "Nueva fuerza: "; cin >> a.fuerza;
            cout << "Nuevos golpes por asalto: "; cin >> a.golpes;
            guardarArmasCuerpo(armasC);
            cout << "Arma cuerpo a cuerpo actualizada.\n";
            return;
        }
    }
    cout << "ID no encontrado.\n";
}

void eliminarArmaCuerpo(vector<ArmaCuerpo>& armasC) {
    int id; cout << "ID del arma cuerpo a cuerpo a eliminar: "; cin >> id;
    for (size_t i = 0; i < armasC.size(); i++) {
        if (armasC[i].id == id) {
            armasC.erase(armasC.begin() + i);
            guardarArmasCuerpo(armasC);
            cout << "Arma cuerpo a cuerpo eliminada.\n";
            return;
        }
    }
    cout << "ID no encontrado.\n";
}

void agregarUnidad(vector<Unidad>& catalogo, const vector<Arma>& armasDisponibles, const vector<ArmaCuerpo>& armasCDisponibles) {
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

    u.armasCuerpo = asignarArmasCuerpoAUnidad(armasCDisponibles, u.numTropas);

    catalogo.push_back(u);
    guardarCatalogoGeneral(catalogo);
    cout << "Unidad agregada al catalogo.\n";
}

void editarUnidad(vector<Unidad>& catalogo, const vector<Arma>& armasDisponibles, const vector<ArmaCuerpo>& armasCDisponibles) {
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
            cout << "Nuevo salvacion: "; cin >> u.salvacion;
            cout << "Nuevas heridas: "; cin >> u.heridas;
            cout << "Nuevo impacto: "; cin >> u.impacto;

            cout << "Asignacion de armas a distancia actual: " << (u.armas.empty() ? "-" : u.armas) << "\n";
            cout << "Deseas re-asignar armas a distancia? 1=Si 0=No: ";
            int ra; cin >> ra;
            if (ra == 1) {
                u.armas = asignarArmasAUnidad(armasDisponibles, u.numTropas);
            }

            cout << "Asignacion de armas cuerpo a cuerpo actual: " << (u.armasCuerpo.empty() ? "-" : u.armasCuerpo) << "\n";
            cout << "Deseas re-asignar armas cuerpo a cuerpo? 1=Si 0=No: ";
            int rab; cin >> rab;
            if (rab == 1) {
                u.armasCuerpo = asignarArmasCuerpoAUnidad(armasCDisponibles, u.numTropas);
            }

            cout << "Unidad actualizada.\n";
            guardarCatalogoGeneral(catalogo);
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
            guardarCatalogoGeneral(catalogo);
            return;
        }
    }
    cout << "ID no encontrado.\n";
}
string seleccionarFaccionIA() {
    cout << "Contra que faccion quieres pelear?\n";
    cout << "1. Astartes\n";
    cout << "2. Orkos\n";
    cout << "3. Eldar\n";
    cout << "4. Tiranidos\n";
    cout << "5. Tau\n";
    cout << "Opcion: ";
    int op; cin >> op;
    if (op == 1) return "Astartes";
    if (op == 2) return "Orkos";
    if (op == 3) return "Eldar";
    if (op == 4) return "Tiranidos";
    if (op == 5) return "Tau";
    return "Astartes";
}

int calcularPuntosEjercito(const vector<int>& ejercito, const vector<Unidad>& catalogo) {
    int total = 0;
    for (int id : ejercito) {
        for (const auto& u : catalogo) {
            if (u.id == id) { total += u.puntaje; break; }
        }
    }
    return total;
}

vector<int> idsDeFaccion(const vector<Unidad>& catalogo, const string& faccion) {
    vector<int> ids;
    for (const auto& u : catalogo) {
        if (u.faccion == faccion) ids.push_back(u.id);
    }
    return ids;
}

vector<int> generarEjercitoIA(const vector<Unidad>& catalogo, const string& faccion, int puntosObjetivo) {
    vector<int> result;
    if (puntosObjetivo <= 0) return result;
    vector<Unidad> candidatos;
    for (const auto& u : catalogo) if (u.faccion == faccion) candidatos.push_back(u);
    if (candidatos.empty()) return result;

    random_device rd;
    mt19937 rng(rd());
    int bestGap = puntosObjetivo;
    vector<int> bestSol;
    const int MAX_ATTEMPTS = 2000;
    for (int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) {
        vector<int> sol;
        int acum = 0;

        for (int step = 0; step < 1000 && acum < puntosObjetivo; ++step) {

            vector<int> idxs;
            for (size_t i = 0; i < candidatos.size(); ++i) {
                if (candidatos[i].puntaje <= (puntosObjetivo - acum)) idxs.push_back((int)i);
            }
            if (idxs.empty()) break;
            uniform_int_distribution<int> d(0, (int)idxs.size() - 1);
            int pick = idxs[d(rng)];
            sol.push_back(candidatos[pick].id);
            acum += candidatos[pick].puntaje;
        }
        int gap = puntosObjetivo - acum;
        if (gap >= 0 && gap < bestGap) {
            bestGap = gap;
            bestSol = sol;
            if (bestGap == 0) break;
        }
    }
    if (bestSol.empty()) {
        vector<Unidad> sorted = candidatos;
        sort(sorted.begin(), sorted.end(), [](const Unidad& a, const Unidad& b){ return a.puntaje > b.puntaje; });
        int acum = 0;
        for (size_t i = 0; i < sorted.size() && acum < puntosObjetivo; ++i) {
            while (acum + sorted[i].puntaje <= puntosObjetivo) {
                bestSol.push_back(sorted[i].id);
                acum += sorted[i].puntaje;
            }
        }
    }
    return bestSol;
}

void guardarEjercitoIA(const string& faccion, const vector<int>& ejercitoIA, const vector<Unidad>& catalogo) {
    string fname = "ia_" + faccion + ".txt";
    ofstream f(fname, ios::trunc);
    if (!f.is_open()) return;
    for (int id : ejercitoIA) {
        f << id << '\n';
    }
    f.close();
}

void desplegarEjercitoIAEnMapa(vector<vector<string>>& mapa, vector<UnidadInst>& instanciasIA) {
    int zonaAltura = 5;
    for (auto &ui : instanciasIA) {
        bool placed = false;
        for (int r = 0; r <= zonaAltura - ui.sizeH && !placed; ++r) {
            for (int c = 0; c <= W_CAMPO - ui.sizeW && !placed; ++c) {
                if (celdasLibres(mapa, r, c, ui.sizeH, ui.sizeW)) {
                    ui.fila = r;
                    ui.col = c;
                    ui.desplegada = true;
                    colocarUnidadEnMapa(mapa, ui);
                    placed = true;
                }
            }
        }
        if (!placed) {
            for (int r = 0; r <= H_CAMPO - ui.sizeH && !placed; ++r) {
                for (int c = 0; c <= W_CAMPO - ui.sizeW && !placed; ++c) {
                    if (celdasLibres(mapa, r, c, ui.sizeH, ui.sizeW)) {
                        ui.fila = r;
                        ui.col = c;
                        ui.desplegada = true;
                        colocarUnidadEnMapa(mapa, ui);
                        placed = true;
                    }
                }
            }
        }
    }
}

bool esUnidadIA(const UnidadInst& u) {
    return (!u.codigo.empty() && u.codigo[0] == 'I');
}

int distanciaManhattan(const UnidadInst& a, const UnidadInst& b) {
    if (a.fila < 0 || a.col < 0 || b.fila < 0 || b.col < 0) return INT_MAX/2;
    return abs(a.fila - b.fila) + abs(a.col - b.col);
}

void moverIA(vector<vector<string>>& mapa, vector<UnidadInst>& instancias) {

    vector<int> idxIA;
    vector<int> idxPlayer;
    for (size_t i = 0; i < instancias.size(); ++i) {
        if (!instancias[i].desplegada) continue;
        if (esUnidadIA(instancias[i])) idxIA.push_back((int)i);
        else idxPlayer.push_back((int)i);
    }

    if (idxIA.empty()) return;

    const int dr[4] = {1, -1, 0, 0};
    const int dc[4] = {0, 0, 1, -1};

    for (int id : idxIA) {
        UnidadInst &uia = instancias[id];
        if (uia.movimiento <= 0) continue;
        if (uia.fila < 0 || uia.col < 0) continue;

        int bestIdx = -1;
        int bestDist = INT_MAX;
        for (int pid : idxPlayer) {
            int d = distanciaManhattan(uia, instancias[pid]);
            if (d < bestDist) { bestDist = d; bestIdx = pid; }
        }

        vector<pair<int,int>> goals;

        if (bestIdx != -1) {
            UnidadInst &tgt = instancias[bestIdx];

            for (int rf = tgt.fila - 1; rf <= tgt.fila + 1; ++rf) {
                for (int cf = tgt.col - 1; cf <= tgt.col + 1; ++cf) {
                    if (!dentroMapa(rf, cf, uia.sizeH, uia.sizeW)) continue;

                    if (abs(rf - tgt.fila) + abs(cf - tgt.col) > 2) continue;

                    goals.emplace_back(rf, cf);
                }
            }

            if (goals.empty() && dentroMapa(tgt.fila, tgt.col, uia.sizeH, uia.sizeW))
                goals.emplace_back(tgt.fila, tgt.col);
        } else {

            for (int r = H_CAMPO - 5; r < H_CAMPO; ++r) {
                for (int c = 0; c < W_CAMPO; ++c) {
                    if (dentroMapa(r, c, uia.sizeH, uia.sizeW)) {
                        goals.emplace_back(r, c);
                    }
                }
            }
        }

        if (goals.empty()) continue;
        quitarUnidadDelMapa(mapa, uia);

        vector<vector<char>> goalGrid(H_CAMPO, vector<char>(W_CAMPO, 0));
        for (auto &g : goals) {
            int gf = g.first, gc = g.second;
            if (celdasLibres(mapa, gf, gc, uia.sizeH, uia.sizeW)) {
                goalGrid[gf][gc] = 1;
            }
        }
        bool anyGoalFree = false;
        for (auto &g : goals) if (goalGrid[g.first][g.second]) { anyGoalFree = true; break; }

        if (!anyGoalFree) {

            for (auto &g : goals) {
                goalGrid[g.first][g.second] = 1;
            }
        }

        vector<vector<char>> visited(H_CAMPO, vector<char>(W_CAMPO, 0));
        vector<vector<pair<int,int>>> parent(H_CAMPO, vector<pair<int,int>>(W_CAMPO, {-1,-1}));
        queue<pair<int,int>> q;

        if (!dentroMapa(uia.fila, uia.col, uia.sizeH, uia.sizeW)) {

            colocarUnidadEnMapa(mapa, uia);
            continue;
        }
        visited[uia.fila][uia.col] = 1;
        parent[uia.fila][uia.col] = {-1,-1};
        q.push({uia.fila, uia.col});

        bool found = false;
        pair<int,int> goalPos = {-1,-1};

        while (!q.empty() && !found) {
            auto cur = q.front(); q.pop();
            int cr = cur.first, cc = cur.second;
            if (goalGrid[cr][cc]) {
                found = true;
                goalPos = cur;
                break;
            }
            for (int k = 0; k < 4; ++k) {
                int nr = cr + dr[k];
                int nc = cc + dc[k];
                if (nr < 0 || nc < 0 || nr >= H_CAMPO || nc >= W_CAMPO) continue;
                if (visited[nr][nc]) continue;
                if (!dentroMapa(nr, nc, uia.sizeH, uia.sizeW)) continue;
                if (!celdasLibres(mapa, nr, nc, uia.sizeH, uia.sizeW)) continue;
                visited[nr][nc] = 1;
                parent[nr][nc] = {cr, cc};
                q.push({nr, nc});
            }
        }

        if (!found) {

            while (!q.empty()) q.pop();
            for (int r = 0; r < H_CAMPO; ++r) for (int c = 0; c < W_CAMPO; ++c) { visited[r][c] = 0; parent[r][c] = {-1,-1}; }
            visited[uia.fila][uia.col] = 1;
            parent[uia.fila][uia.col] = {-1,-1};
            q.push({uia.fila, uia.col});
            while (!q.empty() && !found) {
                auto cur = q.front(); q.pop();
                int cr = cur.first, cc = cur.second;
                if (goalGrid[cr][cc]) {
                    found = true;
                    goalPos = cur;
                    break;
                }
                for (int k = 0; k < 4; ++k) {
                    int nr = cr + dr[k];
                    int nc = cc + dc[k];
                    if (nr < 0 || nc < 0 || nr >= H_CAMPO || nc >= W_CAMPO) continue;
                    if (visited[nr][nc]) continue;
                    if (!dentroMapa(nr, nc, uia.sizeH, uia.sizeW)) continue;

                    visited[nr][nc] = 1;
                    parent[nr][nc] = {cr, cc};
                    q.push({nr, nc});
                }
            }
        }

        if (!found) {

            colocarUnidadEnMapa(mapa, uia);
            continue;
        }


        vector<pair<int,int>> path;
        pair<int,int> cur = goalPos;
        while (!(cur.first == -1 && cur.second == -1)) {
            path.push_back(cur);
            cur = parent[cur.first][cur.second];
        }
        reverse(path.begin(), path.end());
        int maxSteps = min((int)path.size() - 1, uia.movimiento);
        int stepsMoved = 0;
        bool blocked = false;
        for (int step = 1; step <= maxSteps; ++step) {
            int nf = path[step].first;
            int nc = path[step].second;
            if (!celdasLibres(mapa, nf, nc, uia.sizeH, uia.sizeW)) {
                blocked = true;
                break;
            }
            uia.fila = nf;
            uia.col = nc;
            stepsMoved++;

            colocarUnidadEnMapa(mapa, uia);

            quitarUnidadDelMapa(mapa, uia);
        }


        colocarUnidadEnMapa(mapa, uia);


    }
}



void eliminarUnidadSiMuerta(vector<vector<string>>& mapa, vector<UnidadInst>& instancias, int idx) {
    if (instancias[idx].tropasActuales <= 0 && instancias[idx].hp <= 0) {
        cout << ">>> La unidad " << instancias[idx].codigo << " ha muerto y es eliminada del campo.\n";
        quitarUnidadDelMapa(mapa, instancias[idx]);
        instancias.erase(instancias.begin() + idx);
    }
}
#include <random>
#include <climits>

int tiradaD6(std::mt19937 &rng) {
    std::uniform_int_distribution<int> d(1,6);
    return d(rng);
}
std::vector<std::pair<int,int>> parseArmasCadenaToVec(const std::string &s) {
    std::vector<std::pair<int,int>> out;
    if (s.empty() || s == "-") return out;
    std::stringstream ss(s);
    std::string tok;
    while (std::getline(ss, tok, ',')) {
        if (tok.empty()) continue;
        size_t p = tok.find(':');
        if (p == std::string::npos) continue;
        int id = 0, cnt = 0;
        try { id = std::stoi(tok.substr(0,p)); } catch(...) { continue; }
        try { cnt = std::stoi(tok.substr(p+1)); } catch(...) { cnt = 0; }
        out.emplace_back(id, cnt);
    }
    return out;
}
const Arma* armaPorId(const std::vector<Arma>& armas, int id) {
    for (const auto &a: armas) if (a.id == id) return &a;
    return nullptr;
}
const ArmaCuerpo* armaCuerpoPorId(const std::vector<ArmaCuerpo>& armasC, int id) {
    for (const auto &a: armasC) if (a.id == id) return &a;
    return nullptr;
}
int calcularDisparosPorArma(const Arma &a, int numTropas, std::mt19937 &rng) {
    if (a.disparos.empty() || a.disparos == "-") return 0;
    if (a.disparos.size() >= 2 && (a.disparos[0]=='D' || a.disparos[0]=='d')) {
        int n = 6;
        try { n = std::stoi(a.disparos.substr(1)); } catch(...) { n = 6; }
        int total = 0;
        for (int i=0;i<numTropas;i++) {
            std::uniform_int_distribution<int> d(1,n);
            total += d(rng);
        }
        return total;
    }
    try {
        int per = std::stoi(a.disparos);
        return per * numTropas;
    } catch(...) {
        return 0;
    }
}
bool lineaDespejada(const std::vector<std::vector<std::string>>& mapa, int r1, int c1, int r2, int c2) {
    int x0 = c1, y0 = r1, x1 = c2, y1 = r2;
    int dx = abs(x1-x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int x = x0, y = y0;
    while (!(x == x1 && y == y1)) {
        int e2 = 2*err;
        if (e2 >= dy) { err += dy; x += sx; }
        if (e2 <= dx) { err += dx; y += sy; }
        if (x == x1 && y == y1) break;
        if (y >=0 && y < (int)mapa.size() && x >=0 && x < (int)mapa[0].size()) {
            if (mapa[y][x] == "#") return false;
        }
    }
    return true;
}
int umbralParaHervir(int fuerza, int resistencia) {
    if (fuerza >= 2*resistencia) return 2;
    if (fuerza > resistencia) return 3;
    if (fuerza == resistencia) return 4;
    if (fuerza < resistencia && fuerza*2 > resistencia) return 5;
    return 6;
}
void aplicarDanioAUnidad(UnidadInst &target, int danio) {
    if (danio <= 0) return;
    if (target.hpMax > 0) {
        target.hp -= danio;
        if (target.hp < 0) target.hp = 0;
        if (target.hp == 0 && target.tropasActuales > 0) {
            target.tropasActuales = 0;
        }
    } else {
        target.tropasActuales -= danio;
        if (target.tropasActuales < 0) target.tropasActuales = 0;
    }
}
void ejecutarDisparosDeUnidad(
    std::vector<std::vector<std::string>>& mapa,
    UnidadInst &shooter,
    UnidadInst &target,
    const std::vector<Unidad>& catalogo,
    const std::vector<Arma>& armasDisponibles,
    std::mt19937 &rng
) {
    if (!shooter.desplegada || !target.desplegada) return;
    if (shooter.tropasActuales <= 0) return;

    int numTropas = shooter.tropasActuales;
    int resistencia = 1;
    for (const auto &u_cat : catalogo) {
        if (u_cat.id == target.idBase) { resistencia = u_cat.impacto; break; }
    }
    std::string armasCadena = "-";
    for (const auto &u_cat : catalogo) {
        if (u_cat.id == shooter.idBase) { armasCadena = u_cat.armas; break; }
    }
    auto armasVec = parseArmasCadenaToVec(armasCadena);
    if (armasVec.empty()) return;

    for (auto &p : armasVec) {
        int armaId = p.first;
        int asignadas = p.second;
        const Arma* a = armaPorId(armasDisponibles, armaId);
        if (!a) continue;

        int modelos = (asignadas>0? asignadas : numTropas);
        int tiros = calcularDisparosPorArma(*a, modelos, rng);
        if (tiros <= 0) continue;
        int rangoEnt = 0;
        {
            std::string r = a->rango;
            std::string dig;
            for (char ch: r) if (std::isdigit((unsigned char)ch)) dig.push_back(ch);
            if (!dig.empty()) try { rangoEnt = std::stoi(dig); } catch(...) { rangoEnt = 0; }
        }
        int dist = abs(shooter.fila - target.fila) + abs(shooter.col - target.col);
        if (rangoEnt > 0 && dist > rangoEnt) {
            std::cout << "Arma " << a->nombre << " fuera de rango (dist " << dist << " rango " << rangoEnt << ").\n";
            continue;
        }

        if (!lineaDespejada(mapa, shooter.fila, shooter.col, target.fila, target.col)) {
            std::cout << "Disparo de " << a->nombre << " bloqueado por estructura.\n";
            continue;
        }

        int impactos = 0;
        for (int t = 0; t < tiros; ++t) {
            if (a->impacto == 0) { impactos++; continue; }
            int roll = tiradaD6(rng);
            if (roll >= a->impacto) impactos++;
        }
        if (impactos == 0) {
            std::cout << "Arma " << a->nombre << " no consigui impactos (" << tiros << " tiros -> 0 aciertos).\n";
            continue;
        }

        int fuerza = a->fuerza;
        int umbral = umbralParaHervir(fuerza, resistencia);
        int heridas = 0;
        for (int i = 0; i < impactos; ++i) {
            int r = tiradaD6(rng);
            if (r >= umbral) heridas++;
        }
        if (heridas == 0) {
            std::cout << "Arma " << a->nombre << " impact " << impactos << " pero no consigui herir.\n";
            continue;
        }

        int salvados = 0;
        int salvacion = target.salvacion;
        if (salvacion < 1) salvacion = 7;
        for (int i = 0; i < heridas; ++i) {
            int r = tiradaD6(rng);
            if (r >= salvacion) salvados++;
        }
        int heridasNoSalvadas = heridas - salvados;
        if (heridasNoSalvadas <= 0) {
            std::cout << "El objetivo salv " << salvados << " heridas de " << heridas << ".\n";
            continue;
        }

        int danioTotal = 0;
        for (int i = 0; i < heridasNoSalvadas; ++i) {
            if (a->dano.size() >= 2 && (a->dano[0] == 'D' || a->dano[0] == 'd')) {
                int n = 6;
                try { n = std::stoi(a->dano.substr(1)); } catch(...) { n = 6; }
                std::uniform_int_distribution<int> d(1,n);
                danioTotal += d(rng);
            } else {
                try { danioTotal += std::stoi(a->dano); } catch(...) { danioTotal += 1; }
            }
        }

        aplicarDanioAUnidad(target, danioTotal);
        if (target.tropasActuales <= 0 && target.hp <= 0) {
            cout << ">>> La unidad " << target.codigo << " ha muerto y es eliminada del campo.\n";
            quitarUnidadDelMapa(mapa, target);
            target.desplegada = false;
        }
        std::cout << "Arma " << a->nombre << ": tiros " << tiros << " -> impactos " << impactos
             << " -> heridas " << heridas << " (salvadas " << salvados << ") -> dano " << danioTotal << ".\n";
    }
}
void faseDisparo(
    std::vector<std::vector<std::string>>& mapa,
    std::vector<UnidadInst>& instancias,
    const std::vector<Unidad>& catalogo,
    const std::vector<Arma>& armasDisponibles,
    const std::vector<ArmaCuerpo>& armasCDisponibles,
    bool jugadorTurno
) {
    std::random_device rd;
    std::mt19937 rng(rd());

    if (jugadorTurno) {
        std::cout << "=== FASE DE DISPARO (JUGADOR) ===\n";
        std::vector<int> idxs;
        for (size_t i=0;i<instancias.size();++i) {
            if (instancias[i].desplegada && !esUnidadIA(instancias[i]) && instancias[i].tropasActuales>0) idxs.push_back((int)i);
        }
        if (idxs.empty()) { std::cout << "No tienes unidades desplegadas que puedan disparar.\n"; return; }

        std::cout << "Unidades disponibles para disparar:\n";
        for (int id : idxs) {
            std::cout << instancias[id].codigo << " - " << instancias[id].nombre
                 << " Pos(" << instancias[id].fila << "," << instancias[id].col << ") "
                 << " T:" << instancias[id].tropasActuales << "\n";
        }

        while (true) {
            std::cout << "Ingrese el codigo de la unidad que disparara (o 0 para terminar fase): ";
            std::string code; std::cin >> code;
            if (code == "0") break;
            int sidx = buscarInstanciaPorCodigo(instancias, code);
            if (sidx == -1) { std::cout << "Codigo no encontrado.\n"; continue; }
            if (esUnidadIA(instancias[sidx])) { std::cout << "No es una unidad del jugador.\n"; continue; }
            if (!instancias[sidx].desplegada) { std::cout << "Unidad no desplegada.\n"; continue; }
            if (instancias[sidx].tropasActuales <= 0) { std::cout << "Unidad sin tropas.\n"; continue; }

            std::cout << "Objetivos enemigos disponibles:\n";
            std::vector<int> enem;
            for (size_t i=0;i<instancias.size();++i) {
                if (instancias[i].desplegada && esUnidadIA(instancias[i]) && instancias[i].tropasActuales>0) {
                    std::cout << instancias[i].codigo << " - " << instancias[i].nombre << " Pos("<<instancias[i].fila<<","<<instancias[i].col<<") T:"<<instancias[i].tropasActuales << "\n";
                    enem.push_back((int)i);
                }
            }
            if (enem.empty()) { std::cout << "No hay enemigos desplegados.\n"; break; }
            std::cout << "Ingresa codigo objetivo: "; std::string targetCode; std::cin >> targetCode;
            int tidx = buscarInstanciaPorCodigo(instancias, targetCode);
            if (tidx == -1) { std::cout << "Objetivo no encontrado.\n"; continue; }
            if (!esUnidadIA(instancias[tidx])) { std::cout << "Ese objetivo no es enemigo.\n"; continue; }
            ejecutarDisparosDeUnidad(mapa, instancias[sidx], instancias[tidx], catalogo, armasDisponibles, rng);
        }
    } else {
        std::cout << "=== FASE DE DISPARO (IA) ===\n";
        std::vector<int> idxIA;
        std::vector<int> idxPlayer;
        for (size_t i=0;i<instancias.size();++i) {
            if (!instancias[i].desplegada) continue;
            if (esUnidadIA(instancias[i]) && instancias[i].tropasActuales>0) idxIA.push_back((int)i);
            if (!esUnidadIA(instancias[i]) && instancias[i].tropasActuales>0) idxPlayer.push_back((int)i);
        }
        for (int id : idxIA) {
            int best = -1, bestd = INT_MAX;
            for (int pid : idxPlayer) {
                int d = abs(instancias[id].fila - instancias[pid].fila) + abs(instancias[id].col - instancias[pid].col);
                if (d < bestd) { bestd = d; best = pid; }
            }
            if (best != -1) {
                ejecutarDisparosDeUnidad(mapa, instancias[id], instancias[best], catalogo, armasDisponibles, rng);
            }
        }
    }
}

void faseCuerpoACuerpo(
    std::vector<std::vector<std::string>>& mapa,
    std::vector<UnidadInst>& instancias,
    const std::vector<Unidad>& catalogo,
    const std::vector<ArmaCuerpo>& armasCDisponibles,
    bool jugadorTurno
) {
    std::random_device rd;
    std::mt19937 rng(rd());

    std::vector<std::pair<int,int>> combates;
    for (size_t i=0;i<instancias.size();++i) {
        for (size_t j=0;j<instancias.size();++j) {
            if (i==j) continue;
            if (!instancias[i].desplegada || !instancias[j].desplegada) continue;
            if (instancias[i].tropasActuales<=0 || instancias[j].tropasActuales<=0) continue;
            if (esUnidadIA(instancias[i]) == esUnidadIA(instancias[j])) continue;
            int d = abs(instancias[i].fila - instancias[j].fila) + abs(instancias[i].col - instancias[j].col);
            if (d <= 1) {
                if (i < j) combates.emplace_back((int)i,(int)j);
            }
        }
    }
    if (combates.empty()) { std::cout << "No hay combates cuerpo a cuerpo cercanos.\n"; return; }

    for (auto &c : combates) {
        int a = c.first, b = c.second;
        std::cout << "Cuerpo a cuerpo entre " << instancias[a].codigo << " y " << instancias[b].codigo << "\n";

        std::string armasA = "-"; std::string armasB = "-";
        for (const auto &u_cat : catalogo) {
            if (u_cat.id == instancias[a].idBase) armasA = u_cat.armasCuerpo;
            if (u_cat.id == instancias[b].idBase) armasB = u_cat.armasCuerpo;
        }
        auto vecA = parseArmasCadenaToVec(armasA);
        auto vecB = parseArmasCadenaToVec(armasB);

        auto procesarAtaques = [&](int atacanteIdx, int defensorIdx, const std::vector<std::pair<int,int>>& vecArmas)->void {
            UnidadInst &atk = instancias[atacanteIdx];
            UnidadInst &def = instancias[defensorIdx];
            int numAtacantes = std::max(1, atk.tropasActuales);
            int totalGolpes = 0;
            if (vecArmas.empty()) totalGolpes = numAtacantes; else {
                for (auto &p : vecArmas) {
                    const ArmaCuerpo* ac = armaCuerpoPorId(armasCDisponibles, p.first);
                    if (!ac) continue;
                    int nmodelos = (p.second>0? p.second : atk.tropasActuales);
                    totalGolpes += nmodelos * ac->golpes;
                }
            }
            if (totalGolpes <= 0) return;
            int impactos = 0;
            for (int g=0; g<totalGolpes; ++g) {
                int roll = tiradaD6(rng);
                if (roll >= 4) impactos++;
            }
            if (impactos==0) return;
            int fuerza = 1;
            if (!vecArmas.empty()) {
                int sumaF = 0; int cuentas = 0;
                for (auto &p : vecArmas) {
                    const ArmaCuerpo* ac = armaCuerpoPorId(armasCDisponibles, p.first);
                    if (!ac) continue;
                    sumaF += ac->fuerza;
                    cuentas++;
                }
                if (cuentas>0) fuerza = std::max(1, sumaF / cuentas);
            }
            int resistencia = 1;
            for (const auto &u_cat : catalogo) if (u_cat.id == def.idBase) { resistencia = u_cat.impacto; break; }
            int umbral = umbralParaHervir(fuerza, resistencia);
            int heridas = 0;
            for (int i=0;i<impactos;++i) {
                int r = tiradaD6(rng);
                if (r >= umbral) heridas++;
            }
            if (heridas==0) return;
            int salvacion = 7;
            for (const auto &u_cat : catalogo) if (u_cat.id == def.idBase) { salvacion = u_cat.salvacion; break; }
            int salvados = 0;
            for (int i=0;i<heridas;++i) {
                int r = tiradaD6(rng);
                if (r >= salvacion) salvados++;
            }
            int noSalv = heridas - salvados;
            if (noSalv>0) {
                aplicarDanioAUnidad(def, noSalv);
                std::cout << atk.codigo << " hizo " << noSalv << " heridas a " << def.codigo << ".\n";
            }
        };

        procesarAtaques(a,b,vecA);
        procesarAtaques(b,a,vecB);
    }
}
void modoBatalla(vector<vector<string>>& mapa, vector<UnidadInst>& instancias, const vector<Unidad>& catalogo, const vector<Arma>& armas, const vector<ArmaCuerpo>& armasC) {
    cout << "=== MODO BATALLA ===\n";

    int turno = 0;

    while (true) {

        mostrarMapa(mapa);

        mostrarPanelEstado(instancias);

        cout << "\nComandos de BATALLA (Turno " << (turno == 0 ? "JUGADOR" : "IA") << "):\n";
        if (turno == 0) {
            cout << "1) Entrar a Modo Despliegue (colocar unidades) [Jugador]\n";
            cout << "2) Entrar a Modo Movimiento (mover unidades desplegadas) [Jugador]\n";
            cout << "3) Mostrar mapa (solo)\n";
            cout << "4) Mostrar panel estado (solo)\n";
            cout << "5) Salir de BATALLA\n";
            cout << "6) Terminar Turno (pasar a IA)\n";
            cout << "7) Iniciar Fase de Disparo (Jugador)\n";
            cout << "8) Iniciar Combate Cuerpo a Cuerpo (resolver combates adyacentes)\n";
        } else {
            cout << "1) (turno IA) - no disponible\n";
            cout << "2) (turno IA) - no disponible\n";
            cout << "3) Mostrar mapa (solo)\n";
            cout << "4) Mostrar panel estado (solo)\n";
            cout << "5) Salir de BATALLA\n";
            cout << "6) (turno IA) - Ejecutar movimiento IA ahora\n";
            cout << "7) (turno IA) - Ejecutar fase de disparo IA\n";
            cout << "8) (turno IA) - Ejecutar combate cuerpo a cuerpo IA\n";
        }

        cout << "Elige: ";
        int op; cin >> op;

        if (turno == 0) {
            if (op == 1) {
                modoDespliegue(mapa, instancias);
            } else if (op == 2) {
                modoMovimiento(mapa, instancias);
            } else if (op == 3) {
                mostrarMapa(mapa);
            } else if (op == 4) {
                mostrarPanelEstado(instancias);
            } else if (op == 5) {
                cout << "Saliendo de BATALLA.\n";
                break;
            } else if (op == 6) {
                cout << "Turno del jugador finalizado. Ahora mueve la IA...\n";
                moverIA(mapa, instancias);
                cout << "IA ha movido sus unidades.\n";
                turno = 1;
            } else if (op == 7) {
                faseDisparo(mapa, instancias, catalogo, armas, armasC, true);
            } else if (op == 8) {
                faseCuerpoACuerpo(mapa, instancias, catalogo, armasC, true);
            } else {
                cout << "Opcion invalida.\n";
            }
        } else {
            if (op == 3) {
                mostrarMapa(mapa);
            } else if (op == 4) {
                mostrarPanelEstado(instancias);
            } else if (op == 5) {
                cout << "Saliendo de BATALLA.\n";
                break;
            } else if (op == 6) {
                cout << "Ejecutando movimiento de la IA...\n";
                moverIA(mapa, instancias);
                cout << "IA ha movido. Ahora es el turno del jugador.\n";
                turno = 0;
            } else if (op == 7) {
                cout << "IA ejecutando fase de disparo...\n";
                faseDisparo(mapa, instancias, catalogo, armas, armasC, false);
                cout << "IA dispar.\n";
            } else if (op == 8) {
                cout << "IA resolviendo combates cuerpo a cuerpo...\n";
                faseCuerpoACuerpo(mapa, instancias, catalogo, armasC, false);
            } else {
                cout << "Opcion invalida o no disponible en turno IA.\n";
            }
        }
    }
}
int main() {
    vector<Usuario> usuarios;
    cargarUsuarios(usuarios);
    registrarSiVacio(usuarios);
    vector<Arma> armas;
    cargarArmas(armas);
    vector<ArmaCuerpo> armasC;
    cargarArmasCuerpo(armasC);
    while (true) {
        cout << "=== SISTEMA DE LOGIN / REGISTRO ===\n"
             << "1) Iniciar sesion\n"
             << "2) Registrar nuevo usuario (tipo 1)\n"
             << "3) Salir\nElige: ";
        int preop; cin >> preop;
        if (preop == 2) { registrarUsuarioPublico(usuarios); continue; }
        else if (preop == 3) { cout << "Saliendo...\n"; return 0; }
        else if (preop != 1) { cout << "Opcion invalida.\n"; continue; }
        int idUser = login(usuarios);
        if (idUser == -1) { cout << "Login incorrecto.\n"; continue; }
        Usuario &actual = usuarios[idUser];
        cout << "Bienvenido " << actual.username << " (Tipo: " << actual.tipo << ")\n";
        vector<Unidad> catalogo;
        cargarCatalogoGeneral(catalogo);
        vector<int> ejercito;
        cargarCatalogoPersonal(actual.username, ejercito);
        vector<UnidadInst> instancias = generarInstanciasDesdeEjercito(ejercito, catalogo);
        vector<vector<string>> mapa = generarMapaObstaculos();
        int op;
        while (true) {
            if (actual.tipo == 1) {
                cout << "\n===== MENU JUGADOR =====\n"
                     << "0. Batalla (mapa + panel inferior)\n"
                     << "1. Ver catalogo general\n"
                     << "2. Ver catalogo personal\n"
                     << "3. Agregar unidad al ejercito\n"
                     << "4. Eliminar unidad del ejercito\n"
                     << "5. Salir\n";
                cout << "Opcion: "; cin >> op;

                if (op == 0) {

                    string fac = seleccionarFaccionIA();
                    int puntosJugador = calcularPuntosEjercito(ejercito, catalogo);
                    cout << "Puntos del ejercito del jugador: " << puntosJugador << "\n";
                    if (puntosJugador <= 0) {
                        cout << "Tu ejercito no tiene puntos (esta vacio). No se generara IA.\n";

                        modoBatalla(mapa, instancias, catalogo, armas, armasC);
                    } else {
                        vector<int> ejercitoIA = generarEjercitoIA(catalogo, fac, puntosJugador);
                        if (ejercitoIA.empty()) {
                            cout << "No se pudo generar un ejercito IA para la faccion " << fac << ".\n";
                            modoBatalla(mapa, instancias, catalogo, armas, armasC);
                        } else {
                            guardarEjercitoIA(fac, ejercitoIA, catalogo);
                            vector<UnidadInst> instanciasIA = generarInstanciasDesdeEjercito(ejercitoIA, catalogo);
                            for (auto &ui : instanciasIA) ui.codigo = "I" + ui.codigo;
                            desplegarEjercitoIAEnMapa(mapa, instanciasIA);
                            for (auto &ui : instanciasIA) instancias.push_back(ui);
                            cout << "Ejercito IA de faccion " << fac << " creado y desplegado en la parte superior del mapa.\n";
                            modoBatalla(mapa, instancias, catalogo, armas, armasC);
                            for (const auto &ui : instanciasIA) {
                                quitarUnidadDelMapa(mapa, ui);
                            }
                            for (const auto &ui : instanciasIA) {
                                for (auto it = instancias.begin(); it != instancias.end();) {
                                    if (it->codigo == ui.codigo) it = instancias.erase(it);
                                    else ++it;
                                }
                            }
                            cout << "Limpieza: ejercito IA removido del mapa.\n";
                        }
                    }
                }
                else if (op == 1) mostrarCatalogo(catalogo);
                else if (op == 2) mostrarEjercito(ejercito, catalogo);
                else if (op == 3) { agregarAlEjercito(actual.username, ejercito, catalogo);

                    instancias = generarInstanciasDesdeEjercito(ejercito, catalogo);
                }
                else if (op == 4) { eliminarDelEjercito(actual.username, ejercito);
                    instancias = generarInstanciasDesdeEjercito(ejercito, catalogo);
                }
                else if (op == 5) break;
                else cout << "Opcion invalida.\n";
            }

            else if (actual.tipo == 2) {
                cout << "\n===== MENU EDITOR =====\n"
                     << "0. Mostrar mapa\n"
                     << "1. Ver catalogo general\n"
                     << "2. Agregar unidad al catalogo\n"
                     << "3. Editar unidad\n"
                     << "4. Eliminar unidad\n"
                     << "5. Ver catalogo personal\n"
                     << "6. Agregar unidad al ejercito\n"
                     << "----- ARMERIA (A DISTANCIA) -----\n"
                     << "7. Ver armas\n"
                     << "8. Agregar arma\n"
                     << "9. Editar arma\n"
                     << "10. Eliminar arma\n"
                     << "----- ARMERIA (CUERPO A CUERPO) -----\n"
                     << "11. Ver armas cuerpo a cuerpo\n"
                     << "12. Agregar arma cuerpo a cuerpo\n"
                     << "13. Editar arma cuerpo a cuerpo\n"
                     << "14. Eliminar arma cuerpo a cuerpo\n"
                     << "15. Salir\n";
                cout << "Opcion: "; cin >> op;
                if (op == 0) mostrarMapa(mapa);
                else if (op == 1) mostrarCatalogo(catalogo);
                else if (op == 2) { agregarUnidad(catalogo, armas, armasC); }
                else if (op == 3) { editarUnidad(catalogo, armas, armasC); }
                else if (op == 4) eliminarUnidad(catalogo);
                else if (op == 5) mostrarEjercito(ejercito, catalogo);
                else if (op == 6) { agregarAlEjercito(actual.username, ejercito, catalogo); instancias = generarInstanciasDesdeEjercito(ejercito, catalogo); }
                else if (op == 7) mostrarArmas(armas);
                else if (op == 8) agregarArma(armas);
                else if (op == 9) editarArma(armas);
                else if (op == 10) eliminarArma(armas);
                else if (op == 11) mostrarArmasCuerpo(armasC);
                else if (op == 12) agregarArmaCuerpo(armasC);
                else if (op == 13) editarArmaCuerpo(armasC);
                else if (op == 14) eliminarArmaCuerpo(armasC);
                else if (op == 15) break;
                else cout << "Opcion invalida.\n";

                guardarCatalogoGeneral(catalogo);
                guardarArmas(armas);
                guardarArmasCuerpo(armasC);
            }

            else if (actual.tipo == 3) {
                cout << "\n===== MENU ADMIN =====\n"
                     << "0. Mostrar mapa\n"
                     << "1. Ver catalogo general\n"
                     << "2. Agregar unidad al catalogo\n"
                     << "3. Administrar usuarios\n"
                     << "4. Ver catalogo personal\n"
                     << "5. Agregar unidad al ejercito\n"
                     << "6. Editar unidad del catalogo\n"
                     << "7. Eliminar unidad del catalogo\n"
                     << "8. Ver lista de usuarios\n"
                     << "9. (reservado)\n"
                     << "----- ARMERIA (A DISTANCIA) -----\n"
                     << "10. Ver armas\n"
                     << "11. Agregar arma\n"
                     << "12. Editar arma\n"
                     << "13. Eliminar arma\n"
                     << "----- ARMERIA (CUERPO A CUERPO) -----\n"
                     << "14. Ver armas cuerpo a cuerpo\n"
                     << "15. Agregar arma cuerpo a cuerpo\n"
                     << "16. Editar arma cuerpo a cuerpo\n"
                     << "17. Eliminar arma cuerpo a cuerpo\n"
                     << "18. Salir\n";
                cout << "Opcion: "; cin >> op;
                if (op == 0) mostrarMapa(mapa);
                else if (op == 1) mostrarCatalogo(catalogo);
                else if (op == 2) { agregarUnidad(catalogo, armas, armasC); }
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
                            if (nt < 1 || nt > 3) cout << "Tipo invalido.\n";
                            else { usuarios[idx].tipo = nt; guardarUsuarios(usuarios); cout << "Tipo actualizado.\n"; }
                        }
                    }
                    else cout << "Opcion invalida.\n";
                }
                else if (op == 4) mostrarEjercito(ejercito, catalogo);
                else if (op == 5) { agregarAlEjercito(actual.username, ejercito, catalogo); instancias = generarInstanciasDesdeEjercito(ejercito, catalogo); }
                else if (op == 6) editarUnidad(catalogo, armas, armasC);
                else if (op == 7) eliminarUnidad(catalogo);
                else if (op == 8) listarUsuarios(usuarios);
                else if (op == 10) mostrarArmas(armas);
                else if (op == 11) agregarArma(armas);
                else if (op == 12) editarArma(armas);
                else if (op == 13) eliminarArma(armas);
                else if (op == 14) mostrarArmasCuerpo(armasC);
                else if (op == 15) agregarArmaCuerpo(armasC);
                else if (op == 16) editarArmaCuerpo(armasC);
                else if (op == 17) eliminarArmaCuerpo(armasC);
                else if (op == 18) break;
                else cout << "Opcion invalida.\n";

                guardarCatalogoGeneral(catalogo);
                guardarUsuarios(usuarios);
                guardarArmas(armas);
                guardarArmasCuerpo(armasC);
            }
        }

        guardarUsuarios(usuarios);
        guardarCatalogoGeneral(catalogo);
        guardarArmas(armas);
        guardarArmasCuerpo(armasC);
    }

    return 0;
}
