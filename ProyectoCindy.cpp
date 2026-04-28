
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <limits>
using namespace std;

// Clases dedicadas al manejo de excepciones
class PacientenoencontradoException: public runtime_error{
    public:
    PacientenoencontradoException(): runtime_error("Error: El paciente no esta en el sistema."){}
};

class AccesodenegadoException: public runtime_error{
    public:
    AccesodenegadoException(): runtime_error("Error: Acceso denegado, falta de credenciales."){}
};

// Clase para los registros medicos
template <typename T>
class RegistroMedico{
    private:
    string fecharegistro;
    string descripcion;
    T valor;
    public:
    RegistroMedico(string f, string d, T v): fecharegistro(f), descripcion(d), valor(v){}
    void mostrarRegistro() const{
        cout << "Fecha: " << fecharegistro << "\n" << descripcion << ": " << valor << endl;
    }
};
// Clase base
class Persona{
    protected:
    string nombre;
    string cedula;
    int edad;
    public:
    Persona(string n, string c, int e): nombre(n), cedula(c), edad(e){}
    virtual ~Persona(){}
    string getcedula() const{
        return cedula;
    }
    string getnombre() const {
        return nombre;
    }
    virtual void mostrardetalles() const =0;
};
// Jerarquia con herencia
class Empleado: virtual public Persona{
    protected:
    string idempleado;
    double salariobase;
    public:
    Empleado(string n, string c, int e, string i, double s): Persona(n, c, e), idempleado(i), salariobase(s){}
    string getid() const {
        return idempleado;
    }
    virtual double calcularpagomensual() const{
        return salariobase;
    }
    void mostrardetalles() const override{
        cout << "Empleado: " << nombre << "\nId: " << idempleado << endl;
    }
};

// id agregado en metodo 
class Administrativo: virtual public Empleado{
    protected:
    string departamento;
    int nivelacceso;
    public:
    Administrativo(string n, string c, int e, string i, double s, string d, int ni): Persona(n, c, e), Empleado(n, c, e, i, s), departamento(d), nivelacceso(ni){}
    void mostrardetalles() const override{
        cout << "Admin: " << nombre << "\nId: " << idempleado << "\nDepto: " << departamento << "\nNivel de acceso: " << nivelacceso << endl;
    }
    void emitirfactura() const{
        cout << "El personal administrativo " << nombre << " ha emitido la factura." << endl;
    }
};

// id agregado en metodo 
class Medico: virtual public Empleado{
    protected:
    string especialidad;
    int numeroconsultorio;
    public:
    Medico(string n, string c, int e, string i, double s, string es, int co): Persona(n, c, e), Empleado(n, c, e, i, s), especialidad(es), numeroconsultorio(co){}
    void mostrardetalles() const override {
        cout << "Medico: " << nombre << "\nId: " << idempleado << "\nEspecialidad: " << especialidad << "\nN consultorio: " << numeroconsultorio << endl;
    }
    string getespecialidad() const{
        return especialidad;
    }
    int getconsultorio() const{
        return numeroconsultorio;
    }
};

class DirectorMedico: public Medico, public Administrativo{
    private:
    double presupuesto;
    public:
    DirectorMedico(string nombre, string cedula, int edad, string idempleado, double salario, string especialidad, int consultorio, string depto, int nivel, double presupuesto):
    Persona(nombre, cedula, edad), Empleado(nombre, cedula, edad, idempleado, salario), Medico(nombre, cedula, edad, idempleado, salario, especialidad, consultorio), Administrativo(nombre, cedula, edad, idempleado, salario, depto, nivel),
    presupuesto(presupuesto){}
    void mostrardetalles() const override{
        cout << "Director medico: " << nombre << "\nId: " << idempleado << "\nEspecialidad: " << especialidad << "\nDepto: " << departamento << endl;
    }
};

class Paciente: public Persona{
    private:
    string tiposangre;
    vector<RegistroMedico<string>> historialmedico;
    vector<string> alergias;
    public:
    Paciente(string n, string c, int e, string s): Persona(n, c, e), tiposangre(s){}
    void agregaralergia(string alergia){
        alergias.push_back(alergia);
    }
    void agregarregistro(string fecha, string desc, string valor){
        historialmedico.push_back(RegistroMedico<string>(fecha, desc, valor));
    }
    void mostrarhistorial() const{
        cout << "\n--- Historial clinico de " << nombre << " ---" << endl;
        cout << "Tipo de sangre: " << tiposangre << endl;
        cout << "Alergias conocidas: ";
        if(alergias.empty()) cout << "Ninguna." << endl;
        else{
            for(const auto& a: alergias) cout << a << ", ";
            cout << endl;
        }
        cout << "Registros previos:" << endl;
        if(historialmedico.empty()) cout << "Sin registros previos." << endl;
        for(const auto& r: historialmedico) r.mostrarRegistro();
        cout << "-----------------------------------" << endl;
    }
    void mostrardetalles() const override{
        cout << "Paciente: " << nombre << "\nCedula: " << cedula << "\nTipo de sangre: " << tiposangre << endl;
    }
};

//Clase Factura
class Factura{
    private:
    string idfactura;
    double montototal;
    shared_ptr<Paciente> pacientefacturado;
    public:
    Factura(string i, double m, shared_ptr<Paciente> p): idfactura(i), montototal(m), pacientefacturado(p){}
    void mostrarfactura() const{
        cout << "Factura:" << endl;
        cout << "Id factura: " << idfactura << endl;
        if(pacientefacturado) {
            cout << "Paciente: " << pacientefacturado->getnombre() << "\nCi: " << pacientefacturado->getcedula() << endl;
        }
        cout << "Monto total: " << montototal << endl;
    }
};

//Clase cita medica para modulo 2
class Citamedica{
    private:
    string fecha;
    string hora;
    shared_ptr<Paciente> pacienteasignado;
    shared_ptr<Medico> medicoasignado;
    string estado;
    public:
    Citamedica(string f, string h, shared_ptr<Paciente> p, shared_ptr<Medico> m): fecha(f), hora(h), pacienteasignado(p), medicoasignado(m), estado("Programada"){}
    void confirmarcita(){
        estado = "Confirmada";
    }
    void cancelarcita(){
        estado = "Cancelada";
    }
    string getfecha() const{
        return fecha;
    }
    string gethora() const{
        return hora;
    }
    string getestado() const{
        return estado;
    }
    shared_ptr<Medico> getmedico() const{
        return medicoasignado;
    }
    shared_ptr<Paciente> getpaciente()const {
        return pacienteasignado;
    }
};
// Sistema
class SistemaHospital{
    private:
    vector<shared_ptr<Empleado>> empleados;
    vector<shared_ptr<Paciente>> pacientes;
    vector<shared_ptr<Citamedica>> citas;
    vector<shared_ptr<Factura>> facturas;
    
    shared_ptr<Paciente> getPacienteEstricto(string cedula){
        for(auto& p: pacientes){
            if(p->getcedula() == cedula) return p;
        }
        throw PacientenoencontradoException();
    }
    
    public:
    // Función de Login
    shared_ptr<Empleado> iniciarSesion(string idBuscado) {
        for (const auto& emp : empleados) {
            if (emp->getid() == idBuscado) {
                return emp;
            }
        }
        return nullptr; 
    }

    // Integracion del modulo 1
    void registrarEmpleado(shared_ptr<Empleado> nuevo) {
        empleados.push_back(nuevo);
    }
    
    shared_ptr<Paciente> buscaroregistrarpaciente(string cedula) {
        for(auto& paciente: pacientes) {
            if(paciente->getcedula() == cedula) {
                cout << "Paciente encontrado." << endl;
                return paciente;
            }
        }
        cout << "Paciente no encontrado, se necesita registrar como nuevo paciente." << endl;
        string nombre, sangre;
        int edad=0;
        
        cout << "Ingrese el nombre: "; 
        cin >> ws; getline(cin, nombre); 
        cout << "Ingrese la edad: "; cin >> edad;
        cout << "Ingrese el tipo de sangre: "; cin >> sangre;
        
        shared_ptr<Paciente> nuevopaciente = make_shared<Paciente>(nombre, cedula, edad, sangre);
        pacientes.push_back(nuevopaciente);
        return nuevopaciente;
    }
    
    // Integracion del modulo 2
    void visualizardisponibilidad(string especialidadbuscada, string fecha){
        cout << "\nDisponibilidad: " << especialidadbuscada << "\nFecha: " << fecha << endl;
        bool encontrarespecialidad = false;
        for(const auto& e: empleados){
            shared_ptr<Medico> med = dynamic_pointer_cast<Medico>(e);
            if(med && med->getespecialidad() == especialidadbuscada){
                encontrarespecialidad = true;
                cout << "Dr. " << med->getnombre() << "\nConsultorio: " << med->getconsultorio() << endl;
                bool tienecitas = false;
                for(const auto& c: citas){
                    if(c->getmedico() == med && c->getfecha() == fecha && c->getestado() != "Cancelada"){
                        cout << " - Ocupado: " << c->gethora() << endl;
                        tienecitas = true;
                    }
                }
                if (!tienecitas) cout << " - Ninguno (libre)" << endl;   
            }
        }
        if (!encontrarespecialidad) cout << "No hay medicos registrados con esta especialidad" << endl;
    }
    
    void solicitarcita(shared_ptr<Paciente> paciente, string especialidad, string fecha, string hora) {
        shared_ptr<Medico> medicodisponible = nullptr;
        for (const auto& e: empleados) {
            shared_ptr<Medico> med = dynamic_pointer_cast<Medico>(e);
            if (med && med->getespecialidad() == especialidad) {
                bool ocupado = false;
                for (const auto& cita: citas) {
                    if (cita->getmedico() == med && cita->getfecha() == fecha && cita->gethora() == hora && cita->getestado() != "Cancelada"){
                        ocupado = true;
                        break; 
                    }
                }
                if (!ocupado) {
                    medicodisponible = med;
                    break;
                }
            }
        }
        if (medicodisponible){
            shared_ptr<Citamedica> nuevacita = make_shared<Citamedica>(fecha, hora, paciente, medicodisponible);
            citas.push_back(nuevacita);
            cout << "Cita confirmada para " << paciente->getnombre() << " con Dr. " << medicodisponible->getnombre() << " para el " << fecha << " a las " << hora << endl;
        } else{
            cout << "No hay disponibilidad para " << especialidad << " el " << fecha << " a las " << hora << endl;
        }
    }
    
    // Integracion modulo 3 al sistema
    void consultarHistorial(string cedula, shared_ptr<Empleado> solicitante){
        try{
            if (!dynamic_pointer_cast<Medico>(solicitante)){
                throw AccesodenegadoException();
            }
            shared_ptr<Paciente> paciente = getPacienteEstricto(cedula);
            paciente->mostrarhistorial();
        } catch(const exception& e){
            cout << e.what() << " (Se requiere rol de Medico)" << endl;
        }
    }
    
    void registrardiagnostico(string cedula, string fecha, string diagnostico, string tratamiento, shared_ptr<Empleado> solicitante){
        try{
            if(!dynamic_pointer_cast<Medico>(solicitante)){
                throw AccesodenegadoException();
            }
            shared_ptr<Paciente> paciente = getPacienteEstricto(cedula);
            paciente->agregarregistro(fecha, "Diagnostico: " + diagnostico, "Tratamiento: " + tratamiento);
            cout << "Diagnostico guardado para el paciente " << paciente->getnombre() << endl;
        } catch(const exception& e){
            cout << e.what() << " (Se requiere rol de Medico)" << endl;
        }
    }
    
    void revocaracceso(string idempleadobaja, shared_ptr<Empleado> solicitante){
        try {
            if (!dynamic_pointer_cast<DirectorMedico>(solicitante)){
                throw AccesodenegadoException();
            }
            auto it = remove_if(empleados.begin(), empleados.end(), [&idempleadobaja](const shared_ptr<Empleado>& e){
                return e->getid() == idempleadobaja;
            });
            if (it != empleados.end()){
                empleados.erase(it, empleados.end());
                cout << "Exito: Acceso revocado y empleado con id " << idempleadobaja << " eliminado." << endl;
            } else{
                cout << "Error: Empleado no encontrado." << endl;
            }
        } catch(const exception& e){
            cout << e.what() << " (Solo el Director Medico puede revocar accesos)." << endl;
        }
    }
    
    void mostrarempleados() const{
        cout << "\n--- Lista de Empleados ---" << endl;
        for (const auto& i: empleados) {
            i->mostrardetalles();
            cout << "-" << endl;
        }
    }
    void generarfactura(string cedula, string id, double monto, shared_ptr<Empleado> solicitante) {
        try{
            shared_ptr<Administrativo> admin = dynamic_pointer_cast<Administrativo>(solicitante);
            if(!admin){
                throw AccesodenegadoException();
            }
            shared_ptr<Paciente> paciente = getPacienteEstricto(cedula);
            shared_ptr<Factura> nuevafactura = make_shared<Factura>(id, monto, paciente);
            facturas.push_back(nuevafactura);
            admin->emitirfactura();
            nuevafactura->mostrarfactura();
        } catch(const exception& e){
            cout << e.what() << " (Se requiere el rol de administrador)" << endl;
        }
    }
    // Metodo para calcular salario de empleados
    void calcularSalarioEmpleado(string idBuscado) const {
        bool encontrado = false;
        for (const auto& emp : empleados) {
            if (emp->getid() == idBuscado) {
                cout << "-> El salario mensual calculado para " << emp->getnombre() 
                     << " (ID: " << emp->getid() << ") es: $" << emp->calcularpagomensual() << endl;
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            cout << "Error: No se encontro ningun empleado con el ID '" << idBuscado << "'." << endl;
        }
    }
    // Metodo para case 10
    void registrarEmpleadoPorDirector(shared_ptr<Empleado> solicitante) {
        try {
            if (!dynamic_pointer_cast<DirectorMedico>(solicitante)) {
                throw AccesodenegadoException();
            }

            int tipo;
            string n, c, i, d, es;
            int e, ni, co;
            double s, pre;

            cout << "\n--- Formulario de Registro de Empleado ---" << endl;
            cout << "1. Medico\n2. Administrativo\n3. Director Medico\nSeleccione: ";
            cin >> tipo;

            cout << "Nombre: "; cin >> ws; getline(cin, n);
            cout << "Cedula: "; cin >> c;
            cout << "Edad: "; cin >> e;
            cout << "ID Empleado: "; cin >> i;
            cout << "Salario Base: "; cin >> s;

            if (tipo == 1) {
                cout << "Especialidad: "; cin >> es;
                cout << "Consultorio: "; cin >> co;
                registrarEmpleado(make_shared<Medico>(n, c, e, i, s, es, co));
            } else if (tipo == 2) {
                cout << "Departamento: "; cin >> d;
                cout << "Nivel de Acceso: "; cin >> ni;
                registrarEmpleado(make_shared<Administrativo>(n, c, e, i, s, d, ni));
            } else if (tipo == 3) {
                cout << "Especialidad: "; cin >> es;
                cout << "Consultorio: "; cin >> co;
                cout << "Departamento: "; cin >> d;
                cout << "Nivel de Acceso: "; cin >> ni;
                cout << "Presupuesto: "; cin >> pre;
                registrarEmpleado(make_shared<DirectorMedico>(n, c, e, i, s, es, co, d, ni, pre));
            }
            cout << "¡Empleado " << n << " registrado con exito!" << endl;

        } catch (const exception& e) {
            cout << e.what() << " (Solo el Director Medico tiene permisos de contratacion)." << endl;
        }
    }
};
int main() {
    SistemaHospital hospital;

    // Precargar datos para no empezar con el sistema vacio
    shared_ptr<DirectorMedico> director = make_shared<DirectorMedico>("Dr. House", "1111", 50, "EMP01", 5000.0, "Diagnostico", 101, "General", 5, 100000.0);
    shared_ptr<Medico> medico1 = make_shared<Medico>("Dra. Cameron", "2222", 35, "EMP02", 3000.0, "Inmunologia", 102);
    shared_ptr<Administrativo> admin1 = make_shared<Administrativo>("Juan Perez", "3333", 28, "EMP03", 1500.0, "Recepcion", 1);
    
    hospital.registrarEmpleado(director);
    hospital.registrarEmpleado(medico1);
    hospital.registrarEmpleado(admin1);

    // --- SISTEMA DE LOGIN ---
    shared_ptr<Empleado> usuarioActual = nullptr;
    string idLogin;

    cout << "========================================" << endl;
    cout << "    BIENVENIDO AL SISTEMA DEL HOSPITAL  " << endl;
    cout << "========================================" << endl;
    cout << " IDs de prueba:\n EMP01 (Director)\n EMP02 (Medico)\n EMP03 (Administrativo)" << endl;
    cout << "========================================\n" << endl;

    while (usuarioActual == nullptr) {
        cout << "Ingrese su ID de empleado para iniciar sesion: ";
        cin >> idLogin;
        usuarioActual = hospital.iniciarSesion(idLogin);

        if (usuarioActual == nullptr) {
            cout << "Error: ID no reconocido. Intente de nuevo.\n" << endl;
        }
    }

    cout << "\nInicio de sesion exitoso. Bienvenido, " << usuarioActual->getnombre() << "!" << endl;

    // --- MENU INTERACTIVO ---
    int opcion;
    string cedula, especialidad, fecha, hora, diagnostico, tratamiento;

    do {
        cout << "\n========================================" << endl;
        cout << "        MENU PRINCIPAL v1.0             " << endl;
        cout << " Usuario actual: " << usuarioActual->getnombre() << endl;
        cout << "========================================" << endl;
        cout << "1. Buscar o Registrar Paciente" << endl;
        cout << "2. Mostrar Empleados Registrados" << endl;
        cout << "3. Ver Disponibilidad Medica" << endl;
        cout << "4. Solicitar Cita Medica" << endl;
        cout << "5. Registrar Diagnostico (Requiere Medico)" << endl;
        cout << "6. Consultar Historial (Requiere Medico)" << endl;
        cout << "7. Revocar Acceso Empleado (Solo Director)" << endl;
        cout << "8. Emitir factura (Administrativo requerido)" << endl;
        cout << "9. Calcular salario de empleado" << endl;
        cout << "10. Agregar empleado al sistema (solo director)" << endl;
        cout << "0. Salir" << endl;
        cout << "========================================" << endl;
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) { 
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = -1;
        }

        switch (opcion) {
            case 1:
                cout << "Ingrese la cedula del paciente: ";
                cin >> cedula;
                hospital.buscaroregistrarpaciente(cedula);
                break;
                
            case 2:
                hospital.mostrarempleados();
                break;

            case 3:
                cout << "Ingrese especialidad buscada (ej. Inmunologia): ";
                cin >> especialidad;
                cout << "Ingrese fecha (DD/MM/AAAA): ";
                cin >> fecha;
                hospital.visualizardisponibilidad(especialidad, fecha);
                break;

            case 4: {
                cout << "Ingrese cedula del paciente: ";
                cin >> cedula;
                try {
                    shared_ptr<Paciente> p = hospital.buscaroregistrarpaciente(cedula);
                    cout << "Ingrese especialidad: "; cin >> especialidad;
                    cout << "Ingrese fecha (DD/MM/AAAA): "; cin >> fecha;
                    cout << "Ingrese hora (HH:MM): "; cin >> hora;
                    hospital.solicitarcita(p, especialidad, fecha, hora);
                } catch (...) {
                    cout << "Error al procesar la cita." << endl;
                }
                break;
            }

            case 5:
                cout << "Ingrese cedula del paciente: "; cin >> cedula;
                cout << "Ingrese fecha (DD/MM/AAAA): "; cin >> fecha;
                cout << "Ingrese diagnostico: ";
                cin >> ws; getline(cin, diagnostico);
                cout << "Ingrese tratamiento: ";
                getline(cin, tratamiento);
                hospital.registrardiagnostico(cedula, fecha, diagnostico, tratamiento, usuarioActual);
                break;

            case 6:
                cout << "Ingrese cedula del paciente: "; cin >> cedula;
                hospital.consultarHistorial(cedula, usuarioActual);
                break;

            case 7: {
                string idDespido;
                cout << "Ingrese el ID del empleado a dar de baja: ";
                cin >> idDespido;
                hospital.revocaracceso(idDespido, usuarioActual);
                break;
            }
            case 8:{
                string idfactura;
                double montototal;
                cout << "Ingrese la cedula del paciente a facturar:";
                cin >> cedula;
                cout << "Ingrese el id para la nueva factura:";
                cin >> idfactura;
                cout << "Ingrese el monto total:";
                if(!(cin >> montototal)){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Error: monto invalido" << endl;
                    break;
                }
                hospital.generarfactura(cedula, idfactura, montototal, usuarioActual);
                break;
            }
            case 9:{
            string idEmpleadoSalario;
            cout << "Ingrese el ID del empleado para calcular su salario: ";
            cin >> idEmpleadoSalario;
            hospital.calcularSalarioEmpleado(idEmpleadoSalario);
            break;
            }
            case 10:
            hospital.registrarEmpleadoPorDirector(usuarioActual);
            break;
            case 0:
                cout << "Cerrando sesion... ¡Hasta luego, " << usuarioActual->getnombre() << "!" << endl;
                break;

            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcion != 0);

    return 0;
}

