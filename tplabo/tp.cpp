#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
 
 
using namespace std;
 
// Estructura para representar un movimiento
struct Movimiento
{
    string tipo;
    string nombre;
    string actividad;
};
 
// Estructura para representar un plazo fijo
struct PlazoFijo
{
    float monto = 0;
    int dias = 0;
    float interes = 0;
};
 
// Estructura para representar una cuenta bancaria
struct Cuenta
{
    string nombre = "admin";
    string contrasenia = "admin";
    float saldo = 0;
    float dolares = 0;
    vector<Movimiento> movimientos;
    vector<PlazoFijo> plazos_fijos;
};
 
// Función para verificar usuario
bool verificar_usuario(string nombre, string contrasenia) {
    ifstream archivo("usuarios.txt");
    string linea;
    
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string user, pass;
        
        getline(ss, user, ':');
        getline(ss, pass);
        
        if (user == nombre && pass == contrasenia) {
            return true;
        }
    }
    
    return false;
}
 
// Función para registrar usuario
void registrar_usuario(Cuenta &usuario) {
    ofstream archivo("usuarios.txt", ios::app);
    archivo << usuario.nombre << ":" << usuario.contrasenia << endl;
    archivo.close();
    
    // Crear archivos de movimientos
    ofstream movimientos("movimientos_" + usuario.nombre + ".txt");
    movimientos << "Fecha|Tipo|Monto|Descripción" << endl;
    movimientos.close();
}
 
// Función para guardar movimiento
void guardar_movimiento(Cuenta &usuario, string tipo, float monto, string descripcion) {
    // Abrir archivo en modo append
    ofstream archivo("movimientos_" + usuario.nombre + ".txt", ios::app);
    archivo << tipo << "|" << monto << "|" << descripcion << endl;
    
    // Cerrar archivo
    archivo.close();
}
 
// Función para mostrar el menú principal
void menu_principal(Cuenta usuario)
{
    cout << endl;
    cout << "Bienvenido/a al sistema bancario, " << usuario.nombre << endl;
    cout << "Saldo en cuenta: $ " << usuario.saldo << endl;
    cout << "Dólares: USD$ " << usuario.dolares << endl;
    cout << "Plazos fijos armados: " << usuario.plazos_fijos.size() << endl;
    cout << endl;
    cout << "1 - Ingresar efectivo." << endl;
    cout << "2 - Retirar efectivo." << endl;
    cout << "3 - Comprar y/o vender dólares." << endl;
    cout << "4 - Retirar dólares." << endl;
    cout << "5 - Armar plazo fijo." << endl;
    cout << "6 - Cancelar plazo fijo." << endl;
    cout << "7 - Pedir préstamo." << endl;
    cout << "8 - Ver movimientos." << endl;
    cout << "9 - Actualizar datos." << endl;
    cout << "10 - Salir del sistema." << endl;
    cout << endl;
    cout << "Elegir una opcion del menú: " << endl;
}
 
// 1 - Ingresar efectivo
void ingresar_efectivo(Cuenta &usuario, int opcion) {
    float monto;
    cout << "Ingrese el monto de pesos a depositar: ";
    cin >> monto;
    
    if (monto > 0) {
        usuario.saldo += monto;
        
        // Registro de movimiento
        Movimiento movimiento;
        movimiento.tipo = "Ingreso";
        movimiento.nombre = usuario.nombre;
        movimiento.actividad = "Depósito en efectivo";
        usuario.movimientos.push_back(movimiento);
        
        // Cambio aquí: pasar el monto correcto
        guardar_movimiento(usuario, "Ingreso", monto, "Depósito en efectivo");
        
        cout << "Depósito exitoso. Nuevo saldo: $" << usuario.saldo << endl;
    }
}
 
// 2 - Retirar efectivo
void retirar_efectivo(Cuenta &usuario, int opcion) {
    float monto_original;
    cout << "Ingrese el monto de pesos a retirar (múltiplo de 100): ";
    cin >> monto_original;
    
    // Validaciones
    if (monto_original > usuario.saldo) {
        cout << "Saldo insuficiente." << endl;
        return;
    }
    
    if (int(monto_original) % 100 != 0) {
        cout << "El monto debe ser múltiplo de 100." << endl;
        return;
    }
    
    float monto = monto_original;
    
    // Billetes disponibles
    int billetes_1000 = monto / 1000;
    monto -= billetes_1000 * 1000;
    
    int billetes_500 = monto / 500;
    monto -= billetes_500 * 500;
    
    int billetes_200 = monto / 200;
    monto -= billetes_200 * 200;
    
    int billetes_100 = monto / 100;
    
    // Actualizar saldo
    usuario.saldo -= monto_original;
    
    // Mostrar billetes
    cout << "Billetes entregados:" << endl;
    cout << "Billetes de $1000: "<< billetes_1000 << endl;
    cout << "Billetes de $500: " << billetes_500 << endl;
    cout << "Billetes de $200: " << billetes_200 << endl;
    cout << "Billetes de $100: " << billetes_100 << endl;
    
    // Registro de movimiento
    Movimiento movimiento;
    movimiento.tipo = "Retiro";
    movimiento.nombre = usuario.nombre;
    movimiento.actividad = "Extracción de efectivo";
    usuario.movimientos.push_back(movimiento);
    
    // Cambio aquí: pasar el monto original retirado
    guardar_movimiento(usuario, "Retiro", monto_original, "Extracción de efectivo");
    
    cout << "Nuevo saldo: $" << usuario.saldo << endl;
}
// 3 - Comprar y/o vender dólares
void manejar_dolares(Cuenta &usuario, int opcion) {
    int opcion_dolares;
    float monto;
    const float TIPO_CAMBIO = 1000.0; // 1 USD = $1000
 
    cout << "Seleccione una opción:" << endl;
    cout << "1 - Comprar dólares" << endl;
    cout << "2 - Vender dólares" << endl;
    cin >> opcion_dolares;
 
    switch (opcion_dolares) {
        case 1: // Comprar dólares
        {
            cout << "Ingrese la cantidad de dólares a comprar: ";
            cin >> monto;
 
            // Calcular costo en pesos
            float costo_pesos = monto * TIPO_CAMBIO;
 
            // Verificar si tiene suficiente saldo
            if (costo_pesos <= usuario.saldo) {
                usuario.saldo -= costo_pesos;
                usuario.dolares += monto;
 
                // Registrar movimiento
                Movimiento movimiento;
                movimiento.tipo = "Compra";
                movimiento.nombre = usuario.nombre;
                movimiento.actividad = "Compra de dólares";
                usuario.movimientos.push_back(movimiento);
 
                // Guardar movimiento en archivo
                guardar_movimiento(usuario, "Compra Dolares", costo_pesos, "Compra de " + to_string(monto) + " USD");
 
                cout << "Compra exitosa." << endl;
                cout << "Monto en pesos: $" << costo_pesos << endl;
                cout << "Nuevo saldo en pesos: $" << usuario.saldo << endl;
                cout << "Nuevo saldo en dólares: USD$" << usuario.dolares << endl;
            } else {
                cout << "Saldo insuficiente. Necesita $" << costo_pesos << endl;
                float max_dolares = usuario.saldo / TIPO_CAMBIO;
                cout << "Puede comprar hasta USD$" << max_dolares << endl;
            }
            break;
        }
 
        case 2: // Vender dólares
        {
            // Validar que tenga más de 10 dólares
            if (usuario.dolares < 10) {
                cout << "No tiene suficientes dólares para vender. Requiere más de 10 USD." << endl;
                return;
            }
 
            cout << "Ingrese la cantidad de dólares a vender: ";
            cin >> monto;
 
            // Verificar si tiene suficientes dólares
            if (monto <= usuario.dolares) {
                float ingreso_pesos = monto * TIPO_CAMBIO;
                usuario.dolares -= monto;
                usuario.saldo += ingreso_pesos;
 
                // Registrar movimiento
                Movimiento movimiento;
                movimiento.tipo = "Venta";
                movimiento.nombre = usuario.nombre;
                movimiento.actividad = "Venta de dólares";
                usuario.movimientos.push_back(movimiento);
 
                // Guardar movimiento en archivo
                guardar_movimiento(usuario, "Venta Dolares", ingreso_pesos, "Venta de " + to_string(monto) + " USD");
 
                cout << "Venta exitosa." << endl;
                cout << "Monto en pesos recibido: $" << ingreso_pesos << endl;
                cout << "Nuevo saldo en pesos: $" << usuario.saldo << endl;
                cout << "Nuevo saldo en dólares: USD$" << usuario.dolares << endl;
            } else {
                cout << "No tiene suficientes dólares para vender." << endl;
                cout << "Puede vender hasta USD$" << usuario.dolares << endl;
            }
            break;
        }
 
        default:
            cout << "Opción inválida." << endl;
            break;
    }
}
 
// 4 - Retirar dólares
void retirar_dolares(Cuenta &usuario, int opcion) {
    float monto_original;
    cout << "Ingrese el monto de dólares a retirar (múltiplo de 10): ";
    cin >> monto_original;
    
    // Validaciones
    if (monto_original > usuario.dolares) {
        cout << "Saldo insuficiente en dólares." << endl;
        return;
    }
    
    if (int(monto_original) % 10 != 0) {
        cout << "El monto debe ser múltiplo de 10." << endl;
        return;
    }
    
    float monto = monto_original;
    
    // Billetes disponibles en orden descendente
    int billetes_100 = monto / 100;
    monto -= billetes_100 * 100;
    
    int billetes_50 = monto / 50;
    monto -= billetes_50 * 50;
    
    int billetes_20 = monto / 20;
    monto -= billetes_20 * 20;
    
    int billetes_10 = monto / 10;
    
    // Actualizar saldo en dólares
    usuario.dolares -= monto_original;
    
    // Mostrar billetes
    cout << billetes_100 + billetes_50 + billetes_20 + billetes_10<< endl;
    
    // Registro de movimiento
    Movimiento movimiento;
    movimiento.tipo = "Retiro";
    movimiento.nombre = usuario.nombre;
    movimiento.actividad = "Extracción de dólares";
    usuario.movimientos.push_back(movimiento);
    
    // Preparar descripción detallada de billetes
    stringstream descripcion;
    descripcion << billetes_100 + billetes_50 + billetes_20 + billetes_10<<endl;
    
    // Guardar movimiento
    guardar_movimiento(usuario, "Retiro Dolares", monto_original, descripcion.str());
    
    cout << "Nuevo saldo en dólares: USD$" << usuario.dolares << endl;
}
 
// 5 - Armar plazo fijo
void armar_plazo_fijo(Cuenta &usuario, int opcion) {
    float monto;
    int dias;
 
    cout << "Ingrese el monto de pesos a invertir: ";
    cin >> monto;
 
    // Validar que el monto no supere el saldo en cuenta
    if (monto > usuario.saldo) {
        cout << "Saldo insuficiente. No puede invertir más de lo que tiene en cuenta." << endl;
        return;
    }
 
    cout << "Ingrese los días que dejará el dinero (mínimo 30 días): ";
    cin >> dias;
 
    // Validar mínimo de 30 días
    if (dias < 30) {
        cout << "El plazo fijo debe ser de al menos 30 días." << endl;
        return;
    }
 
    // Calcular interés
    float interes = (monto * dias * 70.0) / 36500.0;
    float total = monto + interes;
 
    // Crear estructura de plazo fijo
    PlazoFijo nuevo_plazo;
    nuevo_plazo.monto = monto;
    nuevo_plazo.dias = dias;
    nuevo_plazo.interes = interes;
 
    // Agregar plazo fijo a la lista de plazos fijos
    usuario.plazos_fijos.push_back(nuevo_plazo);
 
    // Descontar monto del saldo
    usuario.saldo -= monto;
 
    // Mostrar información del plazo fijo
    cout << "Detalles del Plazo Fijo:" << endl;
    cout << "Monto invertido: $" << monto << endl;
    cout << "Días de inversión: " << dias << endl;
    cout << "Interés generado: $" << interes << endl;
    cout << "Total a recibir al vencimiento: $" << total << endl;
 
    // Registrar movimiento
    Movimiento movimiento;
    movimiento.tipo = "Plazo Fijo";
    movimiento.nombre = usuario.nombre;
    movimiento.actividad = "Inversión en plazo fijo";
    usuario.movimientos.push_back(movimiento);
 
    // Preparar descripción detallada
    stringstream descripcion;
    descripcion << "Plazo Fijo. Monto: $" << monto
                << ", Dias: " << dias
                << ", Interes: $" << interes
                << ", Total a pagar: $" << total;
 
    // Guardar movimiento
    guardar_movimiento(usuario, "Plazo Fijo", monto, descripcion.str());
 
    cout << "Plazo fijo creado exitosamente." << endl;
    cout << "Nuevo saldo en cuenta: $" << usuario.saldo << endl;
}
void cancelar_plazo_fijo(Cuenta &usuario, int opcion) {
    // Verificar si hay plazos fijos
    if (usuario.plazos_fijos.empty()) {
        cout << "No tiene ningún plazo fijo armado." << endl;
        return;
    }
 
    // Mostrar plazos fijos existentes
    cout << "Plazos Fijos Vigentes:" << endl;
    for (size_t i = 0; i < usuario.plazos_fijos.size(); ++i) {
        cout << i + 1 << ". Monto: $" << usuario.plazos_fijos[i].monto
             << ", Días: " << usuario.plazos_fijos[i].dias
             << ", Interés generado: $" << usuario.plazos_fijos[i].interes << endl;
    }
 
    // Solicitar el plazo fijo a cancelar
    int seleccion;
    cout << "Seleccione el número de plazo fijo a cancelar (0 para salir): ";
    cin >> seleccion;
 
    // Validar selección
    if (seleccion < 1 || seleccion > usuario.plazos_fijos.size()) {
        cout << "Selección inválida." << endl;
        return;
    }
 
    // Obtener el plazo fijo seleccionado (ajustar índice)
    PlazoFijo &plazo = usuario.plazos_fijos[seleccion - 1];
 
    // Calcular monto a devolver (sin intereses)
    float monto_devolver = plazo.monto;
 
    // Devolver el monto al saldo
    usuario.saldo += monto_devolver;
 
    // Registrar movimiento
    Movimiento movimiento;
    movimiento.tipo = "Cancelación Plazo Fijo";
    movimiento.nombre = usuario.nombre;
    movimiento.actividad = "Cancelación anticipada de plazo fijo";
    usuario.movimientos.push_back(movimiento);
 
    // Preparar descripción detallada
    stringstream descripcion;
    descripcion << "Cancelación Plazo Fijo. Monto original: $" << plazo.monto
                << ", Días: " << plazo.dias
                << ", Interés perdido: $" << plazo.interes;
 
    // Guardar movimiento
    guardar_movimiento(usuario, "Cancelación Plazo Fijo", monto_devolver, descripcion.str());
 
    // Eliminar el plazo fijo de la lista
    usuario.plazos_fijos.erase(usuario.plazos_fijos.begin() + (seleccion - 1));
 
    cout << "Plazo fijo cancelado. Se ha devuelto $" << monto_devolver << " a su cuenta." << endl;
    cout << "Nuevo saldo: $" << usuario.saldo << endl;
}
// 7 - Solicitar Préstamo
void solicitar_prestamo(Cuenta &usuario, int opcion) {
    // Mostrar opciones de préstamo
    cout << "Opciones de Préstamo:" << endl;
    cout << "1. Préstamo del 5% (requiere saldo > $20.000)" << endl;
    cout << "2. Préstamo del 10% (requiere saldo > $50.000)" << endl;
    cout << "3. Préstamo del 15% (requiere saldo > $100.000)" << endl;
    cout << "4. Cancelar" << endl;
    
    int opcion_prestamo;
    cout << "Seleccione una opción de préstamo: ";
    cin >> opcion_prestamo;
    
    float monto_prestamo = 0;
    string descripcion_prestamo;
    
    // Validar y procesar préstamo según la opción seleccionada
    switch (opcion_prestamo) {
        case 1:
            // Préstamo del 5% con saldo > $20.000
            if (usuario.saldo > 20000) {
                monto_prestamo = usuario.saldo * 0.05;
                descripcion_prestamo = "Préstamo del 5% (saldo > $20.000)";
            } else {
                cout << "No cumple con el requisito de saldo para este préstamo." << endl;
                return;
            }
            break;
        
        case 2:
            // Préstamo del 10% con saldo > $50.000
            if (usuario.saldo > 50000) {
                monto_prestamo = usuario.saldo * 0.10;
                descripcion_prestamo = "Préstamo del 10% (saldo > $50.000)";
            } else {
                cout << "No cumple con el requisito de saldo para este préstamo." << endl;
                return;
            }
            break;
        
        case 3:
            // Préstamo del 15% con saldo > $100.000
            if (usuario.saldo > 100000) {
                monto_prestamo = usuario.saldo * 0.15;
                descripcion_prestamo = "Préstamo del 15% (saldo > $100.000)";
            } else {
                cout << "No cumple con el requisito de saldo para este préstamo." << endl;
                return;
            }
            break;
        
        case 4:
            // Cancelar
            return;
        
        default:
            cout << "Opción inválida." << endl;
            return;
    }
    
    // Agregar el préstamo al saldo
    usuario.saldo += monto_prestamo;
    
    // Registrar movimiento
    Movimiento movimiento;
    movimiento.tipo = "Préstamo";
    movimiento.nombre = usuario.nombre;
    movimiento.actividad = "Solicitud de préstamo";
    usuario.movimientos.push_back(movimiento);
    
    // Guardar movimiento
    guardar_movimiento(usuario, "Préstamo", monto_prestamo, descripcion_prestamo);
    
    // Mostrar detalles del préstamo
    cout << "Préstamo aprobado." << endl;
    cout << "Monto del préstamo: $" << monto_prestamo << endl;
    cout << "Nuevo saldo en cuenta: $" << usuario.saldo << endl;
}
// 8 - Ver movimientos
void ver_movimientos(Cuenta &usuario, int opcion) {
    ifstream archivo("movimientos_" + usuario.nombre + ".txt");
    string linea;
    vector<string> movimientos;
    
    // Saltar encabezado
    getline(archivo, linea);
    
    // Guardar movimientos
    while (getline(archivo, linea)) {
        movimientos.push_back(linea);
    }
    archivo.close();
    
    while (true) {
        cout << "\nMovimientos realizados:" << endl;
        for (size_t i = 0; i < movimientos.size(); ++i) {
            cout << i + 1 << ". " << movimientos[i] << endl;
        }
        
        string opcion_movimiento;
        cout << "Ingrese el número de un movimiento para ver detalles, o escriba 'menu' para salir: ";
        cin >> opcion_movimiento;
        
        if (opcion_movimiento == "menu") {
            break;
        }
        
        int indice = stoi(opcion_movimiento) - 1;
        
        if (indice >= 0 && indice < movimientos.size()) {
            cout << "\nDetalles del Movimiento:" << endl;
            cout << movimientos[indice] << endl;
        } else {
            cout << "Número de movimiento inválido." << endl;
        }
    }
}
void actualizar_archivo_usuarios(string nombre_antiguo, Cuenta &usuario) {
    ifstream archivo_original("usuarios.txt");
    ofstream archivo_temporal("usuarios_temp.txt");
    
    string linea;
    bool encontrado = false;
    
    while (getline(archivo_original, linea)) {
        stringstream ss(linea);
        string user, pass;
        
        getline(ss, user, ':');
        getline(ss, pass);
        
        if (user == nombre_antiguo) {
            archivo_temporal << usuario.nombre << ":" << usuario.contrasenia << endl;
            encontrado = true;
        } else {
            archivo_temporal << linea << endl;
        }
    }
    
    archivo_original.close();
    archivo_temporal.close();
    
    remove("usuarios.txt");
    rename("usuarios_temp.txt", "usuarios.txt");
}
 
void actualizar_datos(Cuenta &usuario, int opcion) {
    int opcion_actualizacion;
    
    cout << "Seleccione qué desea actualizar:" << endl;
    cout << "1. Nombre de usuario" << endl;
    cout << "2. Contraseña" << endl;
    cout << "3. Cancelar" << endl;
    
    cin >> opcion_actualizacion;
    
    switch (opcion_actualizacion) {
        case 1: {
            string nombre_antiguo = usuario.nombre;
            
            cout << "Ingrese nuevo nombre de usuario: ";
            cin >> usuario.nombre;
            
            actualizar_archivo_usuarios(nombre_antiguo, usuario);
            
            string nombre_archivo_antiguo = "movimientos_" + nombre_antiguo + ".txt";
            string nombre_archivo_nuevo = "movimientos_" + usuario.nombre + ".txt";
            rename(nombre_archivo_antiguo.c_str(), nombre_archivo_nuevo.c_str());
            
            cout << "Nombre de usuario actualizado exitosamente." << endl;
            break;
        }
        
        case 2: {
            string contrasenia1, contrasenia2;
            
            while (true) {
                cout << "Ingrese nueva contraseña: ";
                cin >> contrasenia1;
                
                cout << "Repita la nueva contraseña: ";
                cin >> contrasenia2;
                
                if (contrasenia1 == contrasenia2) {
                    usuario.contrasenia = contrasenia1;
                    
                    actualizar_archivo_usuarios(usuario.nombre, usuario);
                    
                    cout << "Contraseña actualizada exitosamente." << endl;
                    break;
                } else {
                    cout << "Las contraseñas no coinciden. Intente nuevamente." << endl;
                }
            }
            break;
        }
        
        case 3:
            return;
        
        default:
            cout << "Opción inválida." << endl;
            break;
    }
}
 
int main()
{
    Cuenta usuario;
    string nombre, contrasenia;
    int opcion = 0;
    
    // Login
    while (true) {
        cout << "1. Iniciar Sesión" << endl;
        cout << "2. Registrarse" << endl;
        cout << "3. Salir" << endl;
        cin >> opcion;
        
        if (opcion == 3) break;
        
        if (opcion == 1) {
            cout << "Ingrese nombre de usuario: ";
            cin >> nombre;
            cout << "Ingrese contraseña: ";
            cin >> contrasenia;
            
            if (verificar_usuario(nombre, contrasenia)) {
                usuario.nombre = nombre;
                usuario.contrasenia = contrasenia;
                break;
            } else {
                cout << "Usuario o contraseña incorrectos." << endl;
            }
        }
        
        if (opcion == 2) {
            cout << "Ingrese nuevo nombre de usuario: ";
            cin >> usuario.nombre;
            cout << "Ingrese nueva contraseña: ";
            cin >> usuario.contrasenia;
            
            registrar_usuario(usuario);
            cout << "Usuario registrado exitosamente." << endl;
        }
    }
    
    // Menú principal
    while (opcion != 10)
    {
        menu_principal(usuario);
        cin >> opcion;
        
        switch (opcion)
        {
        case 1:
            ingresar_efectivo(usuario, opcion);
            break;
        case 2:
            retirar_efectivo(usuario, opcion);
            break;
        case 3:
            manejar_dolares(usuario,opcion);
            break;
 
        case 4:
            retirar_dolares(usuario,opcion);
            break;
        
        case 5:
             armar_plazo_fijo(usuario,opcion);
            break;
        case 6:
             cancelar_plazo_fijo(usuario,opcion);
            break;
        case 7:
             solicitar_prestamo(usuario,opcion);
            break;
        case 8:
             ver_movimientos(usuario,opcion);
            break;
        case 9:
            actualizar_datos(usuario,opcion);
            break;
        case 10:
            cout << "Gracias por utilizar nuestro sistema " << usuario.nombre << ", vuelva pronto." << endl;
            break;
        default:
            cout << "Opción no válida. Intente nuevamente." << endl;
            break;
        }
    }
   
    return 0;
}
 