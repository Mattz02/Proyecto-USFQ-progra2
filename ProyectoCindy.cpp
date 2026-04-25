// Modulo 1: Gestion de usuarios
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

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
class Administrativo: virtual public Empleado{
    protected:
    string departamento;
    int nivelacceso;
    public:
    Administrativo(string n, string c, int e, string i, double s, string d, int ni): Persona(n, c, e), Empleado(n, c, e, i, s), departamento(d), nivelacceso(ni){}
    void mostrardetalles() const override{
        cout << "Admin: " << nombre << "\nDepto: " << departamento << "\nNivel de acceso: " << nivelacceso << endl;
    }
};
class Medico: virtual public Empleado{
    protected:
    string especialidad;
    int numeroconsultorio;
    public:
    Medico(string n, string c, int e, string i, double s, string es, int co): Persona(n, c, e), Empleado(n, c, e, i, s), especialidad(es), numeroconsultorio(co){}
    void mostrardetalles() const override {
        cout << "Medico: " << nombre << "\nEspecialidad: " << especialidad << "\nN consultorio: " << numeroconsultorio << endl;
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
// Clase paciente para modulo 1
class Paciente: public Persona{
    private:
    string tiposangre;
    vector<RegistroMedico<string>> historialmedico;
    public:
    Paciente(string n, string c, int e, string s): Persona(n, c, e), tiposangre(s){}
    void agregarregistro(string fecha, string desc, string valor){
        historialmedico.push_back(RegistroMedico<string>(fecha, desc, valor));
    }
    void mostrardetalles() const override{
        cout << "Paciente: " << nombre << "\nCedula: " << cedula << "\nTipo de sangre: " << tiposangre << endl;
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
    public:
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
    cout << "Ingrese el nombre: "; cin >> nombre;
    cout << "Ingrese la edad: "; cin >> edad;
    cout << "Ingrese el tipo de sangre: "; cin >> sangre;
    shared_ptr<Paciente> nuevopaciente = make_shared<Paciente>(nombre, cedula, edad, sangre);
    pacientes.push_back(nuevopaciente);
    return nuevopaciente;
    }
    // Integracion del modulo 2
    void visualizardisponibilidad(string especialidadbuscada, string fecha){
        cout << "Disponibilidad: " << especialidadbuscada << "\nFecha: " << fecha << endl;
        bool encontrarespecialidad = false;
        for(const auto& e: empleados){
            shared_ptr<Medico> med = dynamic_pointer_cast<Medico>(e);
            if(med && med->getespecialidad() == especialidadbuscada){
                encontrarespecialidad = true;
                cout << "Dr " << med->getnombre() << "\nConsultorio: " << med->getconsultorio() << endl;
                bool tienecitas = false;
                for(const auto& c: citas){
                    if(c->getmedico() == med && c->getfecha() == fecha && c->getestado() != "Cancelada"){
                        cout << c->gethora() << endl;
                        tienecitas = true;
                    }
                }
                if (!tienecitas) cout << "Ninguno(libre)" << endl;   
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
            cout << "Cita confirmada para " << paciente->getnombre() << " con dr " << medicodisponible->getnombre() << " para el " << fecha << " a las " << hora << endl;
        } else{
            cout << "No hay disponibilidad para " << especialidad << " el " << fecha << " a las " << hora << endl;
        }
    }

    void revocaracceso(string idempleadobaja, const DirectorMedico& solicitante){
        auto it = remove_if(empleados.begin(), empleados.end(), [&idempleadobaja](const shared_ptr<Empleado>& e){
            return e->getid() == idempleadobaja;
        });
        if (it !=empleados.end()){
            empleados.erase(it, empleados.end());
            cout << "Acceso revocado y eliminado del sistema empleado con id " << idempleadobaja << endl;
        } else{
            cout << "Error: Empleado no encontrado" << endl;
        }
    }
    void mostrarempleados() const{
        cout << "Lista de empleados:" << endl;
        for (const auto& i: empleados) i->mostrardetalles();
        cout << endl;
    }
};
// Prueba del sistema en el main
int main () {
    SistemaHospital sis;
    
    //Prueba modulo 1
    cout << "Pruebas del modulo 1:" << endl;
    shared_ptr<Administrativo> recepcionista = make_shared<Administrativo>("Ana Gunn", "1712345678", 28, "EMP-001", 600.0, "Recepcion", 1);
    shared_ptr<Medico> drwhite = make_shared<Medico>("Walter White", "1722334455", 50, "MED-001", 3000.0, "Diagnostico", 201);
    shared_ptr<Medico> drpinkman = make_shared<Medico>("Jesse Pinkman", "1733445566", 40, "MED-002", 2800.0, "Cirugia", 305);

    sis.registrarEmpleado(recepcionista);
    sis.registrarEmpleado(drwhite);
    sis.registrarEmpleado(drpinkman);

    cout << "Lista de empleados:" << endl;
    sis.mostrarempleados();

    cout << "Ejemplo de revocar acceso" << endl;

    DirectorMedico director("Gustavo Fring", "1709998887", 55, "DIR-001", 5000.0, "Gerencia Hospitalaria", 500, "Administracion", 5, 100000.0);

    cout << "El director va a dar de baja al dr Pinkman por usar los medicamentos para el dolor para el mismo" << endl;
    sis.revocaracceso("MED-002", director);

    cout << "\nLista de empleados actualizada:" << endl;
    sis.mostrarempleados();

    cout << "Recepcionista gestionando pacientes:" << endl;

    cout << "Primer caso: buscar un paciente nuevo(1799887766)" << endl;

    shared_ptr<Paciente> pnuevo = sis.buscaroregistrarpaciente("1799887766");

    if (pnuevo) {
        pnuevo->agregarregistro("2026-04-20", "Motivo de consulta", "Revision general completada");
        pnuevo->mostrardetalles();
    }

    cout << "Segundo caso: si el paciente ya esta registrado(1799887766)" << endl;

    shared_ptr<Paciente> pexistente = sis.buscaroregistrarpaciente("1799887766");

    if(pexistente) {
        pexistente->mostrardetalles();
        cout << "Al ya existir el paciente, el sistema omitio el proceso de registro" << endl;
    }
    cout << endl;


    //Prueba modulo 2
    cout << "pruebas modulo 2:" << endl;
    shared_ptr<Medico> drcardio1 = make_shared<Medico>("Pepito Peres", "1701", 45, "CAR-01", 3000, "Cardiologia", 101);
    shared_ptr<Medico> drcardio2 = make_shared<Medico>("Pepita Lopez", "1702", 38, "CAR-02", 3000, "Cardiologia", 102);
    shared_ptr<Paciente> p1 = make_shared<Paciente>("Carlos Gomes", "1755", 30, "O+");
    shared_ptr<Paciente> p2 = make_shared<Paciente>("Maria Juana", "1766", 25, "A-");

    sis.registrarEmpleado(drcardio1);
    sis.registrarEmpleado(drcardio2);

    sis.visualizardisponibilidad("Cardiologia", "2026-05-10");

    cout << "\nPaciente Carlos solicita cita de Cardiologia a las 10:00" << endl;
    sis.solicitarcita(p1, "Cardiologia", "2026-05-10", "10:00");

    cout << "\n[Paciente Maria solicita cita de Cardiologia a la misma hora 10:00]" << endl;
    sis.solicitarcita(p2, "Cardiologia", "2026-05-10", "10:00");
    sis.visualizardisponibilidad("Cardiologia", "2026-05-10");
    cout << endl;
    return 0;
}