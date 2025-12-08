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
using namespace std;

const int H_CAMPO = 44;
const int W_CAMPO = 90;

// ----------------- ESTRUCTURAS BASE -----------------
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
    string armas;       // formato: id:count,id:count
    string armasCuerpo; // formato: id:count,id:count
};

// Instancia de unidad en el campo (por escuadrón)
struct UnidadInst {
    int idBase;         // id del catálogo
    string codigo;      // ejemplo "1A"
    string nombre;
    bool desplegada;
    int fila;           // ancla (esquina superior-izquierda para tanques)
    int col;
    int sizeH;          // altura (filas)
    int sizeW;          // ancho (columnas)
    int tropasActuales; // para infantería
    int tropasMax;
    int hp;             // para vehículos (si aplica)
    int hpMax;
    int movimiento;     // puntos de movimiento por turno
};

// ----------------- DECLARACIONES (prototipos para organización) -----------------
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

// ----------------- UTILIDADES I/O USUARIOS -----------------
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

// ----------------- UTILIDADES ARMAS -----------------
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
             << " | Daño: " << a.dano
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
             << " | Daño: " << a.dano
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

// ----------------- CATALOGO GENERAL -----------------
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

// ----------------- CATALOGO PERSONAL -----------------
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

// ----------------- GENERADOR DE MAPA (OBSTACULOS) -----------------
// Devuelve un grid de strings: "." vacío, "#" obstáculo
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

// Mostrar mapa (cada celda se imprime en 2 caracteres para poder ver códigos como "1A")
void mostrarMapa(const vector<vector<string>>& map) {
    cout << "===== MAPA (cada celda 2 chars) =====\n";
    for (int i = 0; i < H_CAMPO; ++i) {
        for (int j = 0; j < W_CAMPO; ++j) {
            string c = map[i][j];
            if (c == ".") cout << ". ";
            else if (c == "#") cout << "# ";
            else {
                // mostrar el código (si es más largo, cortar a 2)
                if (c.size() >= 2) cout << c.substr(0,2);
                else { cout << c << ' '; }
            }
        }
        cout << "\n";
    }
}

// ----------------- FUNCIONES DE INSTANCIAS Y LÓGICA -----------------
// lowercase helper
string toLowerCopy(const string& s) {
    string out = s;
    for (char &c : out) c = (char)tolower((unsigned char)c);
    return out;
}

// Genera instancias (UnidadInst) desde el vector<int> ejercito (IDs base) y el catalogo
vector<UnidadInst> generarInstanciasDesdeEjercito(const vector<int>& ejercito, const vector<Unidad>& catalogo) {
    vector<UnidadInst> inst;
    // Contador por idBase para asignar letras
    unordered_map<int,int> contador;
    for (int baseId : ejercito) {
        // buscar en catalogo
        const Unidad* ub = nullptr;
        for (const auto& u : catalogo) if (u.id == baseId) { ub = &u; break; }
        if (!ub) continue;

        int idx = contador[baseId]++; // 0-based
        char letra = 'A' + (idx % 26); // Si hay >26, volverá a A..Z pero ok por ahora
        string code = to_string(baseId) + letra;

        UnidadInst ui;
        ui.idBase = baseId;
        ui.codigo = code;
        ui.nombre = ub->nombre;
        ui.desplegada = false;
        ui.fila = -1; ui.col = -1;
        ui.movimiento = ub->movimiento;
        ui.tropasMax = ub->numTropas;
        ui.tropasActuales = ub->numTropas;
        ui.hp = 0; ui.hpMax = 0;

        // determinar tamaño según tipo o heurística
        string tipoLow = toLowerCopy(ub->tipo);
        if (tipoLow.find("infanter") != string::npos || tipoLow.find("infantry") != string::npos) {
            ui.sizeH = 1; ui.sizeW = 1;
            // tropas: ub->numTropas
            ui.tropasActuales = ub->numTropas;
            ui.hp = 0; ui.hpMax = 0;
        } else if (tipoLow.find("tanque") != string::npos || tipoLow.find("tank") != string::npos || tipoLow.find("tanke") != string::npos) {
            ui.sizeH = 1; ui.sizeW = 2; // por defecto horizontal 1x2
            ui.tropasActuales = 1;
            ui.hp = ub->numTropas * 10; // heurística: numTropas define "resistencia"
            ui.hpMax = ui.hp;
        } else {
            // heurística: si numTropas>1 -> infanteria, else tanque
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

// Buscar instancia por codigo
int buscarInstanciaPorCodigo(vector<UnidadInst>& inst, const string& codigo) {
    for (size_t i = 0; i < inst.size(); ++i) if (inst[i].codigo == codigo) return (int)i;
    return -1;
}

// Verifica si un rectángulo (fila..fila+H-1, col..col+W-1) cabe dentro del mapa
bool dentroMapa(int fila, int col, int H, int W) {
    if (fila < 0 || col < 0) return false;
    if (fila + H > H_CAMPO) return false;
    if (col + W > W_CAMPO) return false;
    return true;
}

// Verifica si las celdas están libres ('.') en el mapa (no obstáculo '#' ni otra unidad)
bool celdasLibres(const vector<vector<string>>& map, int fila, int col, int H, int W) {
    if (!dentroMapa(fila, col, H, W)) return false;
    for (int r = 0; r < H; ++r) for (int c = 0; c < W; ++c) {
        string val = map[fila + r][col + c];
        if (val != ".") return false;
    }
    return true;
}

// Coloca la unidad en el mapa (escribe codigo en las celdas)
void colocarUnidadEnMapa(vector<vector<string>>& map, const UnidadInst& u) {
    if (u.fila < 0 || u.col < 0) return;
    for (int r = 0; r < u.sizeH; ++r) for (int c = 0; c < u.sizeW; ++c) {
        map[u.fila + r][u.col + c] = u.codigo;
    }
}

// Remueve la unidad (pone '.' en sus celdas actuales)
void quitarUnidadDelMapa(vector<vector<string>>& map, const UnidadInst& u) {
    if (u.fila < 0 || u.col < 0) return;
    for (int r = 0; r < u.sizeH; ++r) for (int c = 0; c < u.sizeW; ++c) {
        // Sólo limpiar si el contenido coincide con su codigo (evita borrar obstáculos)
        if (map[u.fila + r][u.col + c] == u.codigo) map[u.fila + r][u.col + c] = ".";
    }
}

// ----------------- INTERFACES: DESPLIEGUE / MOVIMIENTO / PANEL -----------------
// Zona despliegue: últimas 5 filas (lado inferior)
bool filaEnZonaDespliegueJugador(int fila) {
    return (fila >= (H_CAMPO - 5) && fila < H_CAMPO);
}

// Modo despliegue interactivo: permite desplegar cada instancia no desplegada
void modoDespliegue(vector<vector<string>>& map, vector<UnidadInst>& instancias) {
    cout << "=== MODO DESPLIEGUE ===\n";
    cout << "Zona de despliegue: filas " << (H_CAMPO - 5) << " a " << (H_CAMPO - 1) << " (inclusive)\n";
    // mostrar lista de instancias no desplegadas
    while (true) {
        cout << "\nInstancias:\n";
        for (const auto& u : instancias) {
            cout << u.codigo << " - " << u.nombre << " - " << (u.desplegada ? "DESPLEGADA" : "POR DESPLEGAR");
            if (u.desplegada) cout << " Pos: (" << u.fila << "," << u.col << ")";
            cout << " Tamaño: " << u.sizeH << "x" << u.sizeW;
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
            // verificar que las celdas estén libres
            if (!dentroMapa(fila, col, instancias[idx].sizeH, instancias[idx].sizeW)) {
                cout << "La unidad no cabe en esas coordenadas (fuera del mapa).\n";
                continue;
            }
            bool ok = true;
            for (int r = 0; r < instancias[idx].sizeH && ok; ++r)
                for (int c = 0; c < instancias[idx].sizeW; ++c)
                    if (map[fila + r][col + c] != ".") { ok = false; break; }
            if (!ok) { cout << "Hay un obstáculo o unidad en la posición indicadas. Intenta otra.\n"; continue; }
            // colocar
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

// Panel lateral / estado del ejército
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

// Mover unidad ya desplegada
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
            if (!instancias[idx].desplegada) { cout << "Unidad no está desplegada.\n"; continue; }
            cout << "Destino fila: "; cin >> fd;
            cout << "Destino col: "; cin >> cd;
            // valida dentro mapa
            if (!dentroMapa(fd, cd, instancias[idx].sizeH, instancias[idx].sizeW)) {
                cout << "Destino fuera del mapa o la unidad no cabe allí.\n"; continue;
            }
            // chequear distancia manhattan desde ancla actual
            int dist = abs(instancias[idx].fila - fd) + abs(instancias[idx].col - cd);
            if (dist > instancias[idx].movimiento) {
                cout << "Destino demasiado lejos. Movimiento max: " << instancias[idx].movimiento << " (dist " << dist << ")\n";
                continue;
            }
            // chequear celdas libres (ignorando sus propias celdas)
            // quitar temporalmente del mapa
            quitarUnidadDelMapa(map, instancias[idx]);
            bool ok = true;
            for (int r = 0; r < instancias[idx].sizeH && ok; ++r)
                for (int c = 0; c < instancias[idx].sizeW; ++c)
                    if (map[fd + r][cd + c] != ".") { ok = false; break; }
            if (!ok) {
                cout << "No se puede mover: destino ocupado por obstaculo o unidad.\n";
                // volver a colocar en su lugar original
                colocarUnidadEnMapa(map, instancias[idx]);
                continue;
            }
            // todo ok: actualizar pos y colocar
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

// ----------------- NUEVA FUNCIÓN: MODO BATALLA -----------------
// Modo batalla: muestra mapa y debajo el panel de estado.
// Desde aquí se accede a modoDespliegue y modoMovimiento (ya no están en el menu principal del jugador).
void modoBatalla(vector<vector<string>>& mapa, vector<UnidadInst>& instancias) {
    cout << "=== MODO BATALLA ===\n";
    while (true) {
        // mostrar mapa
        mostrarMapa(mapa);
        // mostrar panel debajo
        mostrarPanelEstado(instancias);

        cout << "\nComandos de BATALLA:\n";
        cout << "1) Entrar a Modo Despliegue (colocar unidades)\n";
        cout << "2) Entrar a Modo Movimiento (mover unidades desplegadas)\n";
        cout << "3) Mostrar mapa (solo)\n";
        cout << "4) Mostrar panel estado (solo)\n";
        cout << "5) Salir de BATALLA\n";
        cout << "Elige: ";
        int op; cin >> op;
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
        } else {
            cout << "Opcion invalida.\n";
        }
    }
}

// ----------------- FUNCIONES AUXILIARES RESTANTES (ARMAS / UNIDADES) -----------------
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
        cout << "No hay armas a distancia cargadas en el sistema. Se asignará '-' (ninguna).\n";
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
        cout << "No hay armas cuerpo a cuerpo cargadas en el sistema. Se asignará '-' (ninguna).\n";
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

// ----------------- FUNCIONES FALTANTES (AGREGAR/EDITAR/ELIMINAR ARMAS/UNIDADES) -----------------

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
            cout << "Nuevo daño: "; cin >> a.dano;
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
    cout << "Daño por ataque (numero o D6): "; cin >> a.dano;
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
            cout << "Nuevo daño: "; cin >> a.dano;
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

    // Asignar armas a distancia
    u.armas = asignarArmasAUnidad(armasDisponibles, u.numTropas);
    // Asignar armas cuerpo a cuerpo
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
            cout << "¿Deseas re-asignar armas a distancia? 1=Si 0=No: ";
            int ra; cin >> ra;
            if (ra == 1) {
                u.armas = asignarArmasAUnidad(armasDisponibles, u.numTropas);
            }

            cout << "Asignacion de armas cuerpo a cuerpo actual: " << (u.armasCuerpo.empty() ? "-" : u.armasCuerpo) << "\n";
            cout << "¿Deseas re-asignar armas cuerpo a cuerpo? 1=Si 0=No: ";
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

// ----------------- MAIN -----------------
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

        // generar instancias desde ejercito
        vector<UnidadInst> instancias = generarInstanciasDesdeEjercito(ejercito, catalogo);

        // generar mapa con obstaculos (vacío por defecto)
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
                    // Entrar al modo batalla: aquí se muestran mapa y panel abajo y se puede entrar a despliegue/movimiento
                    modoBatalla(mapa, instancias);
                }
                else if (op == 1) mostrarCatalogo(catalogo);
                else if (op == 2) mostrarEjercito(ejercito, catalogo);
                else if (op == 3) { agregarAlEjercito(actual.username, ejercito, catalogo);
                    // regenerar instancias cuando el jugador modifica el ejercito
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
