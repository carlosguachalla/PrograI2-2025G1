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
#include <limits>

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
void moverIA(vector<vector<string>>& mapa, vector<UnidadInst>& instancias);
int distanciaManhattan(const UnidadInst& a, const UnidadInst& b);
bool esUnidadIA(const UnidadInst& u);
int tiradaD6(std::mt19937 &rng);
std::vector<std::pair<int,int>> parseArmasCadenaToVec(const std::string &s);
const Arma* armaPorId(const std::vector<Arma>& armas, int id);
const ArmaCuerpo* armaCuerpoPorId(const std::vector<ArmaCuerpo>& armasC, int id);
int calcularDisparosPorArma(const Arma &a, int numTropas, std::mt19937 &rng);
bool lineaDespejada(const std::vector<std::vector<std::string>>& mapa, int r1, int c1, int r2, int c2);
int umbralParaHervir(int fuerza, int resistencia);
void aplicarDanioAUnidad(UnidadInst &target, int danio);
void ejecutarDisparosDeUnidad(std::vector<std::vector<std::string>>& mapa, UnidadInst &shooter, UnidadInst &target, const std::vector<Unidad>& catalogo, const std::vector<Arma>& armasDisponibles, std::mt19937 &rng);
void faseDisparo(std::vector<std::vector<std::string>>& mapa, std::vector<UnidadInst>& instancias, const std::vector<Unidad>& catalogo, const std::vector<Arma>& armasDisponibles, const std::vector<ArmaCuerpo>& armasCDisponibles, bool jugadorTurno);
void faseCuerpoACuerpo(std::vector<std::vector<std::string>>& mapa, std::vector<UnidadInst>& instancias, const std::vector<Unidad>& catalogo, const std::vector<ArmaCuerpo>& armasCDisponibles, bool jugadorTurno);
void modoBatalla(vector<vector<string>>& mapa, vector<UnidadInst>& instancias, const vector<Unidad>& catalogo, const vector<Arma>& armas, const vector<ArmaCuerpo>& armasC);
void menuGestionArmasDistancia(vector<Arma>& armas);
void menuGestionArmasCuerpo(vector<ArmaCuerpo>& armasC);
void menuArmeria(vector<Arma>& armas, vector<ArmaCuerpo>& armasC);
void menuGestionUnidades(vector<Unidad>& catalogo, const vector<Arma>& armas, const vector<ArmaCuerpo>& armasC);
void menuGestionUsuarios(vector<Usuario>& usuarios);
void menuMiEjercito(Usuario& usuario, vector<int>& ejercito, vector<Unidad>& catalogo, vector<UnidadInst>& instancias);
void menuZonaBatalla(vector<vector<string>>& mapa, vector<UnidadInst>& instancias, vector<int>& ejercito, vector<Unidad>& catalogo, vector<Arma>& armas, vector<ArmaCuerpo>& armasC);
void manejarErrorDeIngreso();

void manejarErrorDeIngreso() {
    if (cin.fail()) {
        cout << "Error de ingreso. Por favor, ingrese un numero valido.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

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
    cout << "--- Registro de Nuevo Usuario ---\n";
    cout << "Ingrese Nombre de Usuario: "; cin >> user;
    if (buscarUsuario(usuarios, user) != -1) {
        cout << "Error: El usuario ya existe.\n";
        return false;
    }
    cout << "Ingrese Contrasena: "; cin >> pass;
    Usuario u{user, pass, 1};
    usuarios.push_back(u);
    guardarUsuarios(usuarios);
    cout << "Usuario '" << user << "' registrado exitosamente.\n";
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
    cout << "===== LISTADO DE USUARIOS REGISTRADOS =====\n";
    for (const auto& u : usuarios) {
        cout << "Usuario: " << u.username << " | Nivel de Acceso: " << u.tipo << "\n";
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
        cout << "ID: " << a.id << " | Nombre: " << a.nombre << " | Rango: " << a.rango
             << " | Disparos: " << a.disparos << " | Impacto: " << a.impacto
             << " | Penetracion: " << a.penetracion << " | Dano: " << a.dano
             << " | Fuerza: " << a.fuerza << "\n";
    }
}

void mostrarArmasCuerpo(const vector<ArmaCuerpo>& armasC) {
    cout << "===== LISTA DE ARMAS CUERPO A CUERPO =====\n";
    for (const auto& a : armasC) {
        cout << "ID: " << a.id << " | Nombre: " << a.nombre << " | Impacto: " << a.impacto
             << " | Penetracion: " << a.penetracion << " | Dano: " << a.dano
             << " | Fuerza: " << a.fuerza << " | Golpes: " << a.golpes << "\n";
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
    cout << "===== CATALOGO GENERAL DE UNIDADES =====\n";
    for (const auto& u : catalogo) {
        cout << "ID: " << u.id << " | Nombre: " << u.nombre << " | Faccion: " << u.faccion
             << " | Pts: " << u.puntaje << " | Tropas: " << u.numTropas << "\n";
        cout << "  Armas Distancia: " << (u.armas.empty() ? "(ninguna)" : u.armas) << "\n";
        cout << "  Armas Cuerpo a Cuerpo: " << (u.armasCuerpo.empty() ? "(ninguna)" : u.armasCuerpo) << "\n";
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
    cout << "Ingrese ID de la unidad a agregar: "; 
    cin >> id;
    manejarErrorDeIngreso();
    if (cin.fail()) return;
    
    for (const auto& u : catalogo) {
        if (u.id == id) {
            ejercito.push_back(id);
            guardarCatalogoPersonal(username, ejercito);
            cout << "Unidad agregada exitosamente.\n";
            return;
        }
    }
    cout << "Error: ID no encontrado en el catalogo general.\n";
}

void eliminarDelEjercito(const string& username, vector<int>& ejercito) {
    int id;
    cout << "Ingrese ID de la unidad a eliminar de su ejercito: ";
    cin >> id;
    manejarErrorDeIngreso();
    if (cin.fail()) return; 

    for (size_t i = 0; i < ejercito.size(); i++) {
        if (ejercito[i] == id) {
            ejercito.erase(ejercito.begin() + i);
            guardarCatalogoPersonal(username, ejercito);
            cout << "Unidad eliminada exitosamente.\n";
            return;
        }
    }
    cout << "Error: Esa unidad no esta en su ejercito actual.\n";
}

void mostrarEjercito(const vector<int>& ejercito, const vector<Unidad>& catalogo) {
    cout << "===== MI EJERCITO PERSONAL =====\n";
    for (int id : ejercito) {
        bool found = false;
        for (const auto& u : catalogo) {
            if (u.id == id) {
                cout << "ID: " << u.id << " | " << u.nombre << "\n";
                found = true;
                break;
            }
        }
        if (!found) cout << "ID: " << id << " | (Datos no encontrados en catalogo)\n";
    }
}

vector<vector<string>> generarMapaObstaculos() {
    struct Estructura { int h, w; };
    vector<Estructura> types = {
        {1, 3}, {4, 4}, {3, 1}
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
    cout << "===== MAPA TACTICO ('.' = Vacio, '#' = Obstaculo) =====\n";
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
        } else if (tipoLow.find("tanque") != string::npos || tipoLow.find("tank") != string::npos) {
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
    cout << "=== FASE DE DESPLIEGUE ===\n";
    cout << "Zona de despliegue: filas " << (H_CAMPO - 5) << " a " << (H_CAMPO - 1) << " (inclusive)\n";
    while (true) {
        cout << "\n--- Unidades Disponibles ---\n";
        for (const auto& u : instancias) {
            cout << u.codigo << " - " << u.nombre << " - " << (u.desplegada ? "DESPLEGADA" : "POR DESPLEGAR");
            if (u.desplegada) cout << " Pos: (" << u.fila << "," << u.col << ")";
            cout << " Tam: " << u.sizeH << "x" << u.sizeW;
            if (u.tropasMax > 1) cout << " Tropas: " << u.tropasActuales << "/" << u.tropasMax;
            else if (u.hpMax > 0) cout << " HP: " << u.hp << "/" << u.hpMax;
            cout << "\n";
        }
        cout << "\nOpciones:\n";
        cout << "1) Desplegar Unidad en Coordenadas\n";
        cout << "2) Visualizar Mapa\n";
        cout << "3) Finalizar Despliegue\n";
        cout << "Elige: ";
        int op; 
        cin >> op;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;

        if (op == 1) {
            string code; int fila, col;
            cout << "Codigo unidad: "; cin >> code;
            int idx = buscarInstanciaPorCodigo(instancias, code);
            if (idx == -1) { cout << "Codigo no encontrado.\n"; continue; }
            if (instancias[idx].desplegada) { cout << "Unidad ya desplegada.\n"; continue; }
            
            cout << "Fila: "; 
            cin >> fila;
            manejarErrorDeIngreso();
            if (cin.fail()) continue;

            cout << "Columna: "; 
            cin >> col;
            manejarErrorDeIngreso();
            if (cin.fail()) continue;
            
            if (!filaEnZonaDespliegueJugador(fila)) {
                cout << "Fila fuera de la zona de despliegue del jugador.\n";
                continue;
            }
            if (!dentroMapa(fila, col, instancias[idx].sizeH, instancias[idx].sizeW)) {
                cout << "La unidad no cabe en esas coordenadas.\n";
                continue;
            }
            bool ok = true;
            for (int r = 0; r < instancias[idx].sizeH && ok; ++r)
                for (int c = 0; c < instancias[idx].sizeW; ++c)
                    if (map[fila + r][col + c] != ".") { ok = false; break; }
            if (!ok) { cout << "Posicion ocupada.\n"; continue; }
            instancias[idx].fila = fila;
            instancias[idx].col = col;
            instancias[idx].desplegada = true;
            colocarUnidadEnMapa(map, instancias[idx]);
            cout << "Unidad desplegada.\n";
        }
        else if (op == 2) {
            mostrarMapa(map);
        }
        else if (op == 3) {
            break;
        }
        else cout << "Opcion invalida.\n";
    }
}

void mostrarPanelEstado(const vector<UnidadInst>& instancias) {
    cout << "===== PANEL DE ESTADO DEL EJERCITO =====\n";
    for (const auto& u : instancias) {
        cout << u.codigo << " | " << u.nombre << " | ";
        if (u.desplegada) cout << "Pos(" << u.fila << "," << u.col << ") ";
        else cout << "No desplegada ";
        if (u.tropasMax > 1) cout << "Tropas: " << u.tropasActuales << "/" << u.tropasMax;
        else if (u.hpMax > 0) cout << "HP: " << u.hp << "/" << u.hpMax;
        else cout << "Unidades: " << u.tropasActuales << "/" << u.tropasMax;
        cout << " | Mov: " << u.movimiento << "\n";
    }
}

void modoMovimiento(vector<vector<string>>& map, vector<UnidadInst>& instancias) {
    cout << "=== FASE DE MOVIMIENTO ===\n";
    while (true) {
        cout << "\nOpciones:\n1) Mover Unidad\n2) Visualizar Mapa\n3) Ver Estado de Tropas\n4) Finalizar Movimiento\nElige: ";
        int op; 
        cin >> op;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;

        if (op == 1) {
            string code; int fd, cd;
            cout << "Codigo unidad: "; cin >> code;
            int idx = buscarInstanciaPorCodigo(instancias, code);
            if (idx == -1) { cout << "Codigo no encontrado.\n"; continue; }
            if (!instancias[idx].codigo.empty() && instancias[idx].codigo[0] == 'I') {
                cout << "No puedes mover unidades de la IA.\n"; continue;
            }
            if (!instancias[idx].desplegada) { cout << "Unidad no desplegada.\n"; continue; }
            
            cout << "Destino fila: "; 
            cin >> fd;
            manejarErrorDeIngreso();
            if (cin.fail()) continue;

            cout << "Destino col: "; 
            cin >> cd;
            manejarErrorDeIngreso();
            if (cin.fail()) continue;

            if (!dentroMapa(fd, cd, instancias[idx].sizeH, instancias[idx].sizeW)) {
                cout << "Destino invalido.\n"; continue;
            }
            int dist = abs(instancias[idx].fila - fd) + abs(instancias[idx].col - cd);
            if (dist > instancias[idx].movimiento) {
                cout << "Destino demasiado lejos. Max mov: " << instancias[idx].movimiento << "\n"; continue;
            }
            quitarUnidadDelMapa(map, instancias[idx]);
            bool ok = true;
            for (int r = 0; r < instancias[idx].sizeH && ok; ++r)
                for (int c = 0; c < instancias[idx].sizeW; ++c)
                    if (map[fd + r][cd + c] != ".") { ok = false; break; }
            if (!ok) {
                cout << "Destino ocupado.\n";
                colocarUnidadEnMapa(map, instancias[idx]);
                continue;
            }
            instancias[idx].fila = fd;
            instancias[idx].col = cd;
            colocarUnidadEnMapa(map, instancias[idx]);
            cout << "Unidad movida exitosamente.\n";
        }
        else if (op == 2) mostrarMapa(map);
        else if (op == 3) mostrarPanelEstado(instancias);
        else if (op == 4) break;
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
    if (armasDisponibles.empty()) return string("-");
    mostrarArmasSimple(armasDisponibles);
    cout << "Asigne armas a distancia (Formato: 'id cantidad', ingrese '0 0' para terminar).\n";
    int idSel = -1, cntSel = -1, suma = 0;
    vector<pair<int,int>> asignadas;
    while (true) {
        cout << "id cantidad: "; 
        cin >> idSel >> cntSel;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;

        if (idSel == 0 && cntSel == 0) break;
        if (cntSel < 0) continue;
        bool found = false;
        for (const auto& a : armasDisponibles) if (a.id == idSel) { found = true; break; }
        if (!found) { cout << "ID incorrecto.\n"; continue; }
        if (suma + cntSel > numTropas) { cout << "Excede numTropas.\n"; continue; }
        bool merged = false;
        for (auto& p : asignadas) if (p.first == idSel) { p.second += cntSel; merged = true; break; }
        if (!merged) asignadas.emplace_back(idSel, cntSel);
        suma += cntSel;
        cout << "Asignado. Total: " << suma << "/" << numTropas << ". Tropas sin arma: " << (numTropas - suma) << "\n";
        if (suma == numTropas) { cout << "Todas las tropas tienen arma asignada.\n"; break; }
    }
    if (asignadas.empty()) {
        cout << "Ninguna tropa tiene arma a distancia.\n";
        return string("-");
    }
    if (suma < numTropas) cout << "Finalizado. " << (numTropas - suma) << " tropas quedaron sin arma a distancia.\n";
    stringstream out;
    for (size_t i = 0; i < asignadas.size(); ++i) {
        out << asignadas[i].first << ":" << asignadas[i].second;
        if (i + 1 < asignadas.size()) out << ",";
    }
    return out.str();
}

string asignarArmasCuerpoAUnidad(const vector<ArmaCuerpo>& armasDisponibles, int numTropas) {
    if (armasDisponibles.empty()) return string("-");
    mostrarArmasCuerpoSimple(armasDisponibles);
    cout << "Asigne armas cuerpo a cuerpo (Formato: 'id cantidad', ingrese '0 0' para terminar).\n";
    int idSel = -1, cntSel = -1, suma = 0;
    vector<pair<int,int>> asignadas;
    while (true) {
        cout << "id cantidad: "; 
        cin >> idSel >> cntSel;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;

        if (idSel == 0 && cntSel == 0) break;
        if (cntSel < 0) continue;
        bool found = false;
        for (const auto& a : armasDisponibles) if (a.id == idSel) { found = true; break; }
        if (!found) { cout << "ID incorrecto.\n"; continue; }
        if (suma + cntSel > numTropas) { cout << "Excede numTropas.\n"; continue; }
        bool merged = false;
        for (auto& p : asignadas) if (p.first == idSel) { p.second += cntSel; merged = true; break; }
        if (!merged) asignadas.emplace_back(idSel, cntSel);
        suma += cntSel;
        cout << "Asignado. Total: " << suma << "/" << numTropas << ". Tropas sin arma: " << (numTropas - suma) << "\n";
        if (suma == numTropas) { cout << "Todas las tropas tienen arma asignada.\n"; break; }
    }
    if (asignadas.empty()) {
        cout << "Ninguna tropa tiene arma cuerpo a cuerpo.\n";
        return string("-");
    }
    if (suma < numTropas) cout << "Finalizado. " << (numTropas - suma) << " tropas quedaron sin arma cuerpo a cuerpo.\n";
    stringstream out;
    for (size_t i = 0; i < asignadas.size(); ++i) {
        out << asignadas[i].first << ":" << asignadas[i].second;
        if (i + 1 < asignadas.size()) out << ",";
    }
    return out.str();
}

void agregarArma(vector<Arma>& armas) {
    mostrarArmas(armas);
    Arma a;
    cout << "ID Arma: "; cin >> a.id;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    for (const auto& ex : armas) if (ex.id == a.id) { cout << "Error: ID ya existe.\n"; return; }
    cout << "Nombre: "; cin >> a.nombre;
    cout << "Rango: "; cin >> a.rango;
    cout << "Disparos: "; cin >> a.disparos;
    
    cout << "Impacto: "; cin >> a.impacto;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    cout << "Penetracion: "; cin >> a.penetracion;
    manejarErrorDeIngreso();
    if (cin.fail()) return;
    
    cout << "Dano: "; cin >> a.dano;
    
    cout << "Fuerza: "; cin >> a.fuerza;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    armas.push_back(a);
    guardarArmas(armas);
    cout << "Arma registrada exitosamente.\n";
}

void editarArma(vector<Arma>& armas) {
    int id; cout << "ID del arma a editar: "; cin >> id;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    for (auto& a : armas) {
        if (a.id == id) {
            cout << "Nombre: "; cin >> a.nombre;
            cout << "Rango: "; cin >> a.rango;
            cout << "Disparos: "; cin >> a.disparos;
            
            cout << "Impacto: "; cin >> a.impacto;
            manejarErrorDeIngreso();
            if (cin.fail()) return;

            cout << "Penetracion: "; cin >> a.penetracion;
            manejarErrorDeIngreso();
            if (cin.fail()) return;
            
            cout << "Dano: "; cin >> a.dano;
            
            cout << "Fuerza: "; cin >> a.fuerza;
            manejarErrorDeIngreso();
            if (cin.fail()) return;

            guardarArmas(armas);
            cout << "Arma actualizada.\n";
            return;
        }
    }
    cout << "Arma no encontrada.\n";
}

void eliminarArma(vector<Arma>& armas) {
    int id; cout << "ID del arma a eliminar: "; cin >> id;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    for (size_t i = 0; i < armas.size(); i++) {
        if (armas[i].id == id) {
            armas.erase(armas.begin() + i);
            guardarArmas(armas);
            cout << "Arma eliminada.\n";
            return;
        }
    }
    cout << "Arma no encontrada.\n";
}

void agregarArmaCuerpo(vector<ArmaCuerpo>& armasC) {
    mostrarArmasCuerpo(armasC);
    ArmaCuerpo a;
    cout << "ID Arma Cuerpo a Cuerpo: "; cin >> a.id;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    for (const auto& ex : armasC) if (ex.id == a.id) { cout << "Error: ID ya existe.\n"; return; }
    cout << "Nombre: "; cin >> a.nombre;
    
    cout << "Impacto: "; cin >> a.impacto;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    cout << "Penetracion: "; cin >> a.penetracion;
    manejarErrorDeIngreso();
    if (cin.fail()) return;
    
    cout << "Dano: "; cin >> a.dano;
    
    cout << "Fuerza: "; cin >> a.fuerza;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    cout << "Golpes: "; cin >> a.golpes;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    armasC.push_back(a);
    guardarArmasCuerpo(armasC);
    cout << "Arma cuerpo a cuerpo agregada.\n";
}

void editarArmaCuerpo(vector<ArmaCuerpo>& armasC) {
    int id; cout << "ID del arma cuerpo a cuerpo a editar: "; cin >> id;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    for (auto& a : armasC) {
        if (a.id == id) {
            cout << "Nombre: "; cin >> a.nombre;
            
            cout << "Impacto: "; cin >> a.impacto;
            manejarErrorDeIngreso();
            if (cin.fail()) return;

            cout << "Penetracion: "; cin >> a.penetracion;
            manejarErrorDeIngreso();
            if (cin.fail()) return;
            
            cout << "Dano: "; cin >> a.dano;
            
            cout << "Fuerza: "; cin >> a.fuerza;
            manejarErrorDeIngreso();
            if (cin.fail()) return;

            cout << "Golpes: "; cin >> a.golpes;
            manejarErrorDeIngreso();
            if (cin.fail()) return;

            guardarArmasCuerpo(armasC);
            cout << "Arma cuerpo a cuerpo actualizada.\n";
            return;
        }
    }
    cout << "Arma no encontrada.\n";
}

void eliminarArmaCuerpo(vector<ArmaCuerpo>& armasC) {
    int id; cout << "ID del arma cuerpo a cuerpo a eliminar: "; cin >> id;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    for (size_t i = 0; i < armasC.size(); i++) {
        if (armasC[i].id == id) {
            armasC.erase(armasC.begin() + i);
            guardarArmasCuerpo(armasC);
            cout << "Arma cuerpo a cuerpo eliminada.\n";
            return;
        }
    }
    cout << "Arma no encontrada.\n";
}

void agregarUnidad(vector<Unidad>& catalogo, const vector<Arma>& armasDisponibles, const vector<ArmaCuerpo>& armasCDisponibles) {
    Unidad u;
    cout << "ID Unidad: "; cin >> u.id;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    for (const auto& ex : catalogo) if (ex.id == u.id) { cout << "Error: ID ya existe.\n"; return; }
    cout << "Tipo: "; cin >> u.tipo;
    cout << "Faccion: "; cin >> u.faccion;
    cout << "Nombre: "; cin >> u.nombre;
    
    cout << "Puntaje: "; cin >> u.puntaje;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    cout << "Num Tropas: "; cin >> u.numTropas;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    cout << "Movimiento: "; cin >> u.movimiento;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    cout << "Salvacion: "; cin >> u.salvacion;
    manejarErrorDeIngreso();
    if (cin.fail()) return;
    
    cout << "Heridas: "; cin >> u.heridas;
    
    cout << "Impacto: "; cin >> u.impacto;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    u.armas = asignarArmasAUnidad(armasDisponibles, u.numTropas);
    u.armasCuerpo = asignarArmasCuerpoAUnidad(armasCDisponibles, u.numTropas);
    catalogo.push_back(u);
    guardarCatalogoGeneral(catalogo);
    cout << "Unidad registrada exitosamente.\n";
}

void editarUnidad(vector<Unidad>& catalogo, const vector<Arma>& armasDisponibles, const vector<ArmaCuerpo>& armasCDisponibles) {
    int id; cout << "ID de unidad a editar: "; cin >> id;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    for (auto& u : catalogo) {
        if (u.id == id) {
            cout << "Tipo: "; cin >> u.tipo;
            cout << "Faccion: "; cin >> u.faccion;
            cout << "Nombre: "; cin >> u.nombre;
            
            cout << "Puntaje: "; cin >> u.puntaje;
            manejarErrorDeIngreso();
            if (cin.fail()) return;

            cout << "Num Tropas: "; cin >> u.numTropas;
            manejarErrorDeIngreso();
            if (cin.fail()) return;

            cout << "Movimiento: "; cin >> u.movimiento;
            manejarErrorDeIngreso();
            if (cin.fail()) return;

            cout << "Salvacion: "; cin >> u.salvacion;
            manejarErrorDeIngreso();
            if (cin.fail()) return;
            
            cout << "Heridas: "; cin >> u.heridas;
            
            cout << "Impacto: "; cin >> u.impacto;
            manejarErrorDeIngreso();
            if (cin.fail()) return;

            cout << "Reasignar armas dist? 1=Si: "; int ra; cin >> ra;
            manejarErrorDeIngreso();
            if (cin.fail()) return;

            if (ra == 1) u.armas = asignarArmasAUnidad(armasDisponibles, u.numTropas);
            
            cout << "Reasignar armas cuerpo a cuerpo? 1=Si: "; int rab; cin >> rab;
            manejarErrorDeIngreso();
            if (cin.fail()) return;
            
            if (rab == 1) u.armasCuerpo = asignarArmasCuerpoAUnidad(armasCDisponibles, u.numTropas);
            
            guardarCatalogoGeneral(catalogo);
            cout << "Unidad actualizada.\n";
            return;
        }
    }
    cout << "Unidad no encontrada.\n";
}

void eliminarUnidad(vector<Unidad>& catalogo) {
    int id; cout << "ID de unidad a eliminar: "; cin >> id;
    manejarErrorDeIngreso();
    if (cin.fail()) return;

    for (size_t i = 0; i < catalogo.size(); i++) {
        if (catalogo[i].id == id) {
            catalogo.erase(catalogo.begin() + i);
            guardarCatalogoGeneral(catalogo);
            cout << "Unidad eliminada.\n";
            return;
        }
    }
    cout << "Unidad no encontrada.\n";
}

string seleccionarFaccionIA() {
    cout << "Seleccione faccion enemiga:\n1.Astartes\n2.Orkos\n3.Eldar\n4.Tiranidos\n5.Tau\nOpcion: ";
    int op; 
    cin >> op;
    manejarErrorDeIngreso();
    if (cin.fail()) return "Astartes";

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
    for (const auto& u : catalogo) if (u.faccion == faccion) ids.push_back(u.id);
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
    for (int attempt = 0; attempt < 2000; ++attempt) {
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
    for (int id : ejercitoIA) f << id << '\n';
    f.close();
}

void desplegarEjercitoIAEnMapa(vector<vector<string>>& mapa, vector<UnidadInst>& instanciasIA) {
    int zonaAltura = 5;
    for (auto &ui : instanciasIA) {
        bool placed = false;
        for (int r = 0; r <= zonaAltura - ui.sizeH && !placed; ++r) {
            for (int c = 0; c <= W_CAMPO - ui.sizeW && !placed; ++c) {
                if (celdasLibres(mapa, r, c, ui.sizeH, ui.sizeW)) {
                    ui.fila = r; ui.col = c;
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
                        ui.fila = r; ui.col = c;
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
                    if (dentroMapa(r, c, uia.sizeH, uia.sizeW)) goals.emplace_back(r, c);
                }
            }
        }
        if (goals.empty()) continue;
        quitarUnidadDelMapa(mapa, uia);
        vector<vector<char>> goalGrid(H_CAMPO, vector<char>(W_CAMPO, 0));
        for (auto &g : goals) {
            if (celdasLibres(mapa, g.first, g.second, uia.sizeH, uia.sizeW)) goalGrid[g.first][g.second] = 1;
        }
        bool anyGoalFree = false;
        for (auto &g : goals) if (goalGrid[g.first][g.second]) { anyGoalFree = true; break; }
        if (!anyGoalFree) for (auto &g : goals) goalGrid[g.first][g.second] = 1;
        vector<vector<char>> visited(H_CAMPO, vector<char>(W_CAMPO, 0));
        vector<vector<pair<int,int>>> parent(H_CAMPO, vector<pair<int,int>>(W_CAMPO, {-1,-1}));
        queue<pair<int,int>> q;
        if (!dentroMapa(uia.fila, uia.col, uia.sizeH, uia.sizeW)) {
            colocarUnidadEnMapa(mapa, uia); continue;
        }
        visited[uia.fila][uia.col] = 1;
        parent[uia.fila][uia.col] = {-1,-1};
        q.push({uia.fila, uia.col});
        bool found = false;
        pair<int,int> goalPos = {-1,-1};
        while (!q.empty() && !found) {
            auto cur = q.front(); q.pop();
            int cr = cur.first, cc = cur.second;
            if (goalGrid[cr][cc]) { found = true; goalPos = cur; break; }
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
                if (goalGrid[cr][cc]) { found = true; goalPos = cur; break; }
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
        if (!found) { colocarUnidadEnMapa(mapa, uia); continue; }
        vector<pair<int,int>> path;
        pair<int,int> cur = goalPos;
        while (!(cur.first == -1 && cur.second == -1)) {
            path.push_back(cur);
            cur = parent[cur.first][cur.second];
        }
        reverse(path.begin(), path.end());
        int maxSteps = min((int)path.size() - 1, uia.movimiento);
        for (int step = 1; step <= maxSteps; ++step) {
            int nf = path[step].first;
            int nc = path[step].second;
            if (!celdasLibres(mapa, nf, nc, uia.sizeH, uia.sizeW)) break;
            uia.fila = nf; uia.col = nc;
        }
        colocarUnidadEnMapa(mapa, uia);
    }
}

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
    } catch(...) { return 0; }
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
        if (target.hp == 0 && target.tropasActuales > 0) target.tropasActuales = 0;
    } else {
        target.tropasActuales -= danio;
        if (target.tropasActuales < 0) target.tropasActuales = 0;
    }
}

void ejecutarDisparosDeUnidad(std::vector<std::vector<std::string>>& mapa, UnidadInst &shooter, UnidadInst &target, const std::vector<Unidad>& catalogo, const std::vector<Arma>& armasDisponibles, std::mt19937 &rng) {
    if (!shooter.desplegada || !target.desplegada) return;
    if (shooter.tropasActuales <= 0) return;
    int numTropas = shooter.tropasActuales;
    int resistencia = 1;
    for (const auto &u_cat : catalogo) if (u_cat.id == target.idBase) { resistencia = u_cat.impacto; break; }
    std::string armasCadena = "-";
    for (const auto &u_cat : catalogo) if (u_cat.id == shooter.idBase) { armasCadena = u_cat.armas; break; }
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
        std::string dig;
        for (char ch: a->rango) if (std::isdigit((unsigned char)ch)) dig.push_back(ch);
        if (!dig.empty()) try { rangoEnt = std::stoi(dig); } catch(...) { rangoEnt = 0; }
        int dist = abs(shooter.fila - target.fila) + abs(shooter.col - target.col);
        if (rangoEnt > 0 && dist > rangoEnt) {
            std::cout << "Arma " << a->nombre << " fuera de rango.\n"; continue;
        }
        if (!lineaDespejada(mapa, shooter.fila, shooter.col, target.fila, target.col)) {
            std::cout << "Disparo bloqueado.\n"; continue;
        }
        int impactos = 0;
        for (int t = 0; t < tiros; ++t) {
            if (a->impacto == 0) { impactos++; continue; }
            int roll = tiradaD6(rng);
            if (roll >= a->impacto) impactos++;
        }
        if (impactos == 0) continue;
        int fuerza = a->fuerza;
        int umbral = umbralParaHervir(fuerza, resistencia);
        int heridas = 0;
        for (int i = 0; i < impactos; ++i) {
            int r = tiradaD6(rng);
            if (r >= umbral) heridas++;
        }
        if (heridas == 0) continue;
        int salvados = 0;
        int salvacion = target.salvacion;
        if (salvacion < 1) salvacion = 7;
        for (int i = 0; i < heridas; ++i) {
            int r = tiradaD6(rng);
            if (r >= salvacion) salvados++;
        }
        int heridasNoSalvadas = heridas - salvados;
        if (heridasNoSalvadas <= 0) continue;
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
            cout << "Unidad " << target.codigo << " eliminada.\n";
            quitarUnidadDelMapa(mapa, target);
            target.desplegada = false;
        }
        std::cout << "Arma " << a->nombre << ": " << heridasNoSalvadas << " heridas entraron. Dano total: " << danioTotal << "\n";
    }
}

void faseDisparo(std::vector<std::vector<std::string>>& mapa, std::vector<UnidadInst>& instancias, const std::vector<Unidad>& catalogo, const std::vector<Arma>& armasDisponibles, const std::vector<ArmaCuerpo>& armasCDisponibles, bool jugadorTurno) {
    std::random_device rd;
    std::mt19937 rng(rd());
    if (jugadorTurno) {
        std::cout << "=== FASE DE DISPARO (JUGADOR) ===\n";
        while (true) {
            std::cout << "Codigo unidad que dispara (0 fin): ";
            std::string code; std::cin >> code;
            if (code == "0") break;
            int sidx = buscarInstanciaPorCodigo(instancias, code);
            if (sidx == -1 || esUnidadIA(instancias[sidx]) || !instancias[sidx].desplegada || instancias[sidx].tropasActuales <= 0) {
                std::cout << "Unidad invalida.\n"; continue;
            }
            std::cout << "Codigo objetivo: "; std::string targetCode; std::cin >> targetCode;
            int tidx = buscarInstanciaPorCodigo(instancias, targetCode);
            if (tidx == -1 || !esUnidadIA(instancias[tidx]) || instancias[tidx].tropasActuales <= 0) {
                std::cout << "Objetivo invalido o destruido.\n"; continue;
            }
            ejecutarDisparosDeUnidad(mapa, instancias[sidx], instancias[tidx], catalogo, armasDisponibles, rng);
        }
    } else {
        std::cout << "=== FASE DE DISPARO (IA) ===\n";
        std::vector<int> idxIA, idxPlayer;
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
            if (best != -1) ejecutarDisparosDeUnidad(mapa, instancias[id], instancias[best], catalogo, armasDisponibles, rng);
        }
    }
}

void faseCuerpoACuerpo(std::vector<std::vector<std::string>>& mapa, std::vector<UnidadInst>& instancias, const std::vector<Unidad>& catalogo, const std::vector<ArmaCuerpo>& armasCDisponibles, bool jugadorTurno) {
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
            if (d <= 1 && i < j) combates.emplace_back((int)i,(int)j);
        }
    }
    if (combates.empty()) { std::cout << "Sin combates cuerpo a cuerpo.\n"; return; }
    for (auto &c : combates) {
        int a = c.first, b = c.second;
        std::cout << "Cuerpo a cuerpo: " << instancias[a].codigo << " vs " << instancias[b].codigo << "\n";
        std::string armasA = "-", armasB = "-";
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
                int sumaF = 0, cuentas = 0;
                for (auto &p : vecArmas) {
                    const ArmaCuerpo* ac = armaCuerpoPorId(armasCDisponibles, p.first);
                    if (!ac) continue;
                    sumaF += ac->fuerza; cuentas++;
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
        if (instancias[b].tropasActuales <= 0 && instancias[b].hp <= 0) {
            quitarUnidadDelMapa(mapa, instancias[b]);
            instancias[b].desplegada = false;
            std::cout << instancias[b].codigo << " eliminada.\n";
            continue;
        }
        procesarAtaques(b,a,vecB);
        if (instancias[a].tropasActuales <= 0 && instancias[a].hp <= 0) {
            quitarUnidadDelMapa(mapa, instancias[a]);
            instancias[a].desplegada = false;
            std::cout << instancias[a].codigo << " eliminada.\n";
        }
    }
}

void modoBatalla(vector<vector<string>>& mapa, vector<UnidadInst>& instancias, const vector<Unidad>& catalogo, const vector<Arma>& armas, const vector<ArmaCuerpo>& armasC) {
    cout << "=== SIMULACION DE BATALLA ===\n";
    int turno = 0;
    while (true) {
        mostrarMapa(mapa);
        mostrarPanelEstado(instancias);
        cout << "\nTurno Actual: " << (turno == 0 ? "JUGADOR" : "IA (ENEMIGO)") << "\n";
        cout << "1) Fase de Despliegue\n";
        cout << "2) Fase de Movimiento\n";
        cout << "3) Ver Mapa Tactico\n";
        cout << "4) Ver Estado de Tropas\n";
        cout << "5) Salir de la Simulacion\n";
        cout << "6) Terminar Turno\n";
        cout << "7) Fase de Disparo\n";
        cout << "8) Fase de Combate Cuerpo a Cuerpo\n";
        cout << "Opcion: ";
        int op; 
        cin >> op;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;

        if (turno == 0) {
            if (op == 1) modoDespliegue(mapa, instancias);
            else if (op == 2) modoMovimiento(mapa, instancias);
            else if (op == 3) mostrarMapa(mapa);
            else if (op == 4) mostrarPanelEstado(instancias);
            else if (op == 5) break;
            else if (op == 6) {
                cout << "Fin de turno Jugador. Inicia IA.\n";
                moverIA(mapa, instancias);
                turno = 1;
            } else if (op == 7) faseDisparo(mapa, instancias, catalogo, armas, armasC, true);
            else if (op == 8) faseCuerpoACuerpo(mapa, instancias, catalogo, armasC, true);
        } else {
            if (op == 3) mostrarMapa(mapa);
            else if (op == 4) mostrarPanelEstado(instancias);
            else if (op == 5) break;
            else if (op == 6) {
                moverIA(mapa, instancias);
                faseDisparo(mapa, instancias, catalogo, armas, armasC, false);
                faseCuerpoACuerpo(mapa, instancias, catalogo, armasC, false);
                turno = 0;
            } else if (op == 7) faseDisparo(mapa, instancias, catalogo, armas, armasC, false);
            else if (op == 8) faseCuerpoACuerpo(mapa, instancias, catalogo, armasC, false);
            else cout << "Accion no disponible en turno de IA.\n";
        }
    }
}

void menuGestionArmasDistancia(vector<Arma>& armas) {
    while (true) {
        cout << "\n--- GESTION DE ARMAS A DISTANCIA ---\n";
        cout << "1. Listar Armas a Distancia\n";
        cout << "2. Registrar Nueva Arma a Distancia\n";
        cout << "3. Modificar Arma a Distancia\n";
        cout << "4. Eliminar Arma a Distancia\n";
        cout << "0. Volver al Menu Anterior\n";
        cout << "Opcion: ";
        int op; 
        cin >> op;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;

        if (op == 1) mostrarArmas(armas);
        else if (op == 2) { agregarArma(armas); guardarArmas(armas); }
        else if (op == 3) { editarArma(armas); guardarArmas(armas); }
        else if (op == 4) { eliminarArma(armas); guardarArmas(armas); }
        else if (op == 0) break;
    }
}

void menuGestionArmasCuerpo(vector<ArmaCuerpo>& armasC) {
    while (true) {
        cout << "\n--- GESTION DE ARMAS CUERPO A CUERPO ---\n";
        cout << "1. Listar Armas Cuerpo a Cuerpo\n";
        cout << "2. Registrar Nueva Arma Cuerpo a Cuerpo\n";
        cout << "3. Modificar Arma Cuerpo a Cuerpo\n";
        cout << "4. Eliminar Arma Cuerpo a Cuerpo\n";
        cout << "0. Volver al Menu Anterior\n";
        cout << "Opcion: ";
        int op; 
        cin >> op;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;
        
        if (op == 1) mostrarArmasCuerpo(armasC);
        else if (op == 2) { agregarArmaCuerpo(armasC); guardarArmasCuerpo(armasC); }
        else if (op == 3) { editarArmaCuerpo(armasC); guardarArmasCuerpo(armasC); }
        else if (op == 4) { eliminarArmaCuerpo(armasC); guardarArmasCuerpo(armasC); }
        else if (op == 0) break;
    }
}

void menuArmeria(vector<Arma>& armas, vector<ArmaCuerpo>& armasC) {
    while (true) {
        cout << "\n===== MENU DE ARMERIA =====\n";
        cout << "1. Gestionar Armas a Distancia\n";
        cout << "2. Gestionar Armas Cuerpo a Cuerpo\n";
        cout << "0. Volver al Menu Principal\n";
        cout << "Opcion: ";
        int op; 
        cin >> op;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;

        if (op == 1) menuGestionArmasDistancia(armas);
        else if (op == 2) menuGestionArmasCuerpo(armasC);
        else if (op == 0) break;
    }
}

void menuGestionUnidades(vector<Unidad>& catalogo, const vector<Arma>& armas, const vector<ArmaCuerpo>& armasC) {
    while (true) {
        cout << "\n===== BASE DE DATOS DE UNIDADES =====\n";
        cout << "1. Ver Catalogo Completo\n";
        cout << "2. Crear Nueva Unidad\n";
        cout << "3. Editar Unidad Existente\n";
        cout << "4. Eliminar Unidad del Sistema\n";
        cout << "0. Volver al Menu Principal\n";
        cout << "Opcion: ";
        int op; 
        cin >> op;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;

        if (op == 1) mostrarCatalogo(catalogo);
        else if (op == 2) { agregarUnidad(catalogo, armas, armasC); guardarCatalogoGeneral(catalogo); }
        else if (op == 3) { editarUnidad(catalogo, armas, armasC); guardarCatalogoGeneral(catalogo); }
        else if (op == 4) { eliminarUnidad(catalogo); guardarCatalogoGeneral(catalogo); }
        else if (op == 0) break;
    }
}

void menuGestionUsuarios(vector<Usuario>& usuarios) {
    while (true) {
        cout << "\n===== ADMINISTRACION DE USUARIOS =====\n";
        cout << "1. Listar Todos los Usuarios\n";
        cout << "2. Registrar Nuevo Usuario\n";
        cout << "3. Modificar Nivel de Acceso\n";
        cout << "4. Eliminar Usuario del Sistema\n";
        cout << "0. Volver al Menu Principal\n";
        cout << "Opcion: ";
        int op; 
        cin >> op;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;

        if (op == 1) listarUsuarios(usuarios);
        else if (op == 2) registrarUsuarioPublico(usuarios);
        else if (op == 3) {
            string umod; cout << "Usuario: "; cin >> umod;
            int idx = buscarUsuario(usuarios, umod);
            if (idx == -1) cout << "No existe.\n";
            else {
                int nt; cout << "Nuevo nivel (1=Jugador, 2=Editor, 3=Admin): "; cin >> nt;
                manejarErrorDeIngreso();
                if (cin.fail()) continue;
                if (nt >= 1 && nt <= 3) { usuarios[idx].tipo = nt; guardarUsuarios(usuarios); }
            }
        }
        else if (op == 4) {
            string udel; cout << "Usuario: "; cin >> udel;
            int idx = buscarUsuario(usuarios, udel);
            if (idx != -1) {
                remove(archivoPersonal(udel).c_str());
                usuarios.erase(usuarios.begin() + idx);
                guardarUsuarios(usuarios);
            }
        }
        else if (op == 0) break;
    }
}

void menuMiEjercito(Usuario& usuario, vector<int>& ejercito, vector<Unidad>& catalogo, vector<UnidadInst>& instancias) {
    while (true) {
        cout << "\n===== GESTION DE MI EJERCITO =====\n";
        cout << "1. Ver Mi Ejercito Actual\n";
        cout << "2. Reclutar Unidad (Agregar)\n";
        cout << "3. Licenciar Unidad (Eliminar)\n";
        cout << "4. Consultar Catalogo General\n";
        cout << "0. Volver al Menu Principal\n";
        cout << "Opcion: ";
        int op; 
        cin >> op;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;

        if (op == 1) mostrarEjercito(ejercito, catalogo);
        else if (op == 2) {
            agregarAlEjercito(usuario.username, ejercito, catalogo);
            instancias = generarInstanciasDesdeEjercito(ejercito, catalogo);
        }
        else if (op == 3) {
            eliminarDelEjercito(usuario.username, ejercito);
            instancias = generarInstanciasDesdeEjercito(ejercito, catalogo);
        }
        else if (op == 4) mostrarCatalogo(catalogo);
        else if (op == 0) break;
    }
}

void menuZonaBatalla(vector<vector<string>>& mapa, vector<UnidadInst>& instancias, vector<int>& ejercito, vector<Unidad>& catalogo, vector<Arma>& armas, vector<ArmaCuerpo>& armasC) {
    while (true) {
        cout << "\n===== ZONA DE COMBATE =====\n";
        cout << "1. Iniciar Nueva Batalla\n";
        cout << "2. Ver Mapa Tactico\n";
        cout << "0. Volver al Menu Principal\n";
        cout << "Opcion: ";
        int op; 
        cin >> op;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;

        if (op == 1) {
            int pts = calcularPuntosEjercito(ejercito, catalogo);
            if (pts <= 0) { cout << "Error: Ejercito vacio o sin valor en puntos.\n"; continue; }
            string fac = seleccionarFaccionIA();
            vector<int> ejercitoIA = generarEjercitoIA(catalogo, fac, pts);
            vector<UnidadInst> copiaInstancias = instancias;
            if (!ejercitoIA.empty()) {
                guardarEjercitoIA(fac, ejercitoIA, catalogo);
                vector<UnidadInst> instIA = generarInstanciasDesdeEjercito(ejercitoIA, catalogo);
                for (auto& u : instIA) u.codigo = "I" + u.codigo;
                copiaInstancias.insert(copiaInstancias.end(), instIA.begin(), instIA.end());
            }
            modoBatalla(mapa, copiaInstancias, catalogo, armas, armasC);
            mapa = generarMapaObstaculos();
            instancias = generarInstanciasDesdeEjercito(ejercito, catalogo);
        }
        else if (op == 2) mostrarMapa(mapa);
        else if (op == 0) break;
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
    vector<Unidad> catalogo;
    cargarCatalogoGeneral(catalogo);

    while (true) {
        cout << "\n=== SISTEMA DE ACCESO ===\n";
        cout << "1. Iniciar Sesion\n";
        cout << "2. Registrar Nuevo Usuario\n";
        cout << "3. Salir del Programa\n";
        cout << "Seleccione una opcion: ";
        int preop; 
        cin >> preop;
        manejarErrorDeIngreso();
        if (cin.fail()) continue;

        if (preop == 3) return 0;
        if (preop == 2) { registrarUsuarioPublico(usuarios); continue; }
        if (preop != 1) continue;

        int idUser = login(usuarios);
        if (idUser == -1) { cout << "Credenciales incorrectas.\n"; continue; }

        Usuario& actual = usuarios[idUser];
        vector<int> ejercito;
        cargarCatalogoPersonal(actual.username, ejercito);
        vector<UnidadInst> instancias = generarInstanciasDesdeEjercito(ejercito, catalogo);
        vector<vector<string>> mapa = generarMapaObstaculos();

        while (true) {
            cout << "\n=== MENU PRINCIPAL ===\n";
            cout << "1. JUGAR (Zona de Batalla)\n";
            cout << "2. GESTIONAR MI EJERCITO\n";
            if (actual.tipo >= 2) {
                cout << "3. BASE DE DATOS: UNIDADES\n";
                cout << "4. BASE DE DATOS: ARMERIA\n";
            }
            if (actual.tipo == 3) cout << "5. ADMINISTRACION DE USUARIOS\n";
            cout << "0. CERRAR SESION\n";
            cout << "Seleccione una opcion: ";
            int op; 
            cin >> op;
            manejarErrorDeIngreso();
            if (cin.fail()) continue;

            if (op == 0) break;
            else if (op == 1) menuZonaBatalla(mapa, instancias, ejercito, catalogo, armas, armasC);
            else if (op == 2) menuMiEjercito(actual, ejercito, catalogo, instancias);
            else if (op == 3 && actual.tipo >= 2) menuGestionUnidades(catalogo, armas, armasC);
            else if (op == 4 && actual.tipo >= 2) menuArmeria(armas, armasC);
            else if (op == 5 && actual.tipo == 3) menuGestionUsuarios(usuarios);
        }
        guardarUsuarios(usuarios);
        guardarCatalogoGeneral(catalogo);
        guardarArmas(armas);
        guardarArmasCuerpo(armasC);
    }
}