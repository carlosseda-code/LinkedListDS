//
//  main.cpp
//  Algoritmos fundamentales
//
//  Created by Carlos Seda and Rodrigo H on 04/10/20.
//  Copyright © 2020 Carlos Seda. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <sstream>
using namespace std;

// Clase donde tendremos almacenado cada linea del archivo
class Usuarios{
public:
    string linea;
    long long ip;
};

// Nodo simple
class NodeDouble{
public:
    Usuarios data;
    NodeDouble *next;
    NodeDouble *prev;
    NodeDouble(Usuarios data){
        this->data = data;
        next = NULL;
        prev = NULL;
    }
};

// Doubly LinkedList
class LinkedList{
public:
    NodeDouble *head;
    LinkedList(){
        head = NULL;
    }
    
    void addLast(NodeDouble* n){
        if(head == NULL)
            head = n;
        else {
            NodeDouble* prev = head;
            while (prev->next != NULL)
                prev = prev->next;
            prev->next = n;
            n->prev = prev;
        }
    }
    void print(){
        if (head == NULL)
            cout << "Lista vacia" << endl;
        else{
            NodeDouble *last = head;
            while(last != NULL){
                cout << last->data.ip << endl;
                last = last->next;
            }
            cout << endl;
        }
    }
};

// Declaracion de funciones
void lecturaVariables(LinkedList& ll);
Usuarios dividirLinea(string linea);
long long crearIP(string ip);
NodeDouble* merge(NodeDouble* left, NodeDouble* rigth);
NodeDouble* ordenaMerge(NodeDouble* head);
NodeDouble* binarySearch(NodeDouble *head, long long value);

// Menu principal
int main(int argc, const char * argv[]) {
    LinkedList ll;
    int cont = 0, numero = 1;
    string ip1, ip2;
    ofstream f("registro.txt");
    
    lecturaVariables(ll);
    ordenaMerge(ll.head);
    ll.print();
    cout << "------------Registros de acceso------------" << endl;
    cout << "¿Desde que IP le gustaria ver los registro? (000.00.000.00 - 999.99.999.99)" << endl;
    while(cont == 0){
    cout << "IP de inicio: ";
    cin >> ip1;
    if (ip1.length()>= 11 && ip1.length() <= 13)
        cont++;
    }
    cont = 0;
    cout << "¿Y hasta que IP? (000.00.000.00 - 999.99.999.99)" << endl;
    while(cont == 0){
    cout << "IP de fin: ";
    cin >> ip2;
    if (ip2.length()>= 11 && ip2.length() <= 13)
        cont++;
    }
    cout << "-------------Buscando archivos-------------" << endl;
    
    ip1 = ip1 + ':';
    ip2 = ip2 + ':';
    long long ipinicio = crearIP(ip1);
    long long ipfin = crearIP(ip2);
    
    if(ipfin < ipinicio || ipfin < ll.head->data.ip){
        cout << "Mal ingreso de IP's" << endl;
        cout << "Saliendo del sistema...." << endl;
        return 1;
    }
    
    NodeDouble* first = binarySearch(ll.head, ipinicio);
    NodeDouble* last = binarySearch(ll.head, ipfin);
    
    while(first != last){
        cout << "Registro #"<< numero << ": " << first->data.linea << endl;
        if (f.is_open())
        f << first->data.linea << endl;
        else cerr << "Error de apertura del archivo." << endl;
        numero++;
        first = first->next;
    }
}

// Leer todo el archivo y asignar IDs a cada usuario
void lecturaVariables(LinkedList& ll){
    string linea;
    ifstream archivo;
    Usuarios persona;
    archivo.open("bitacora.txt");
    while (getline(archivo, linea)){
        persona = dividirLinea(linea);
        NodeDouble* n= new NodeDouble(persona);
        ll.addLast(n);
    }
        archivo.close();
}

// Divir la linea del archivo y guardar en variables
Usuarios dividirLinea(string linea) {
  vector<string> variables;
  stringstream ss(linea);
  string dato;
  string ip;
  Usuarios nuevo;
  char delimiter = ' ';
  while(getline(ss, dato, delimiter)) {
    variables.push_back(dato);
  }
    ip = variables[3];
    nuevo.linea = linea;
    nuevo.ip = crearIP(ip);
    return nuevo;
}

// Crear el ID del usuario recibiendo los datos necesarios
long long crearIP(string ip){
    string ipnuevo, ultimo, temp;
    vector<string> variables;
    stringstream ss(ip);
    string dato;
    int i = 0;
    char delimiter = '.';
    while(getline(ss, dato, delimiter))
        variables.push_back(dato);
    
    ipnuevo = ipnuevo + variables[0];
    if (stoi(variables[1]) >= 10 || variables[1] == "00")
        ipnuevo = ipnuevo + variables[1];
    else ipnuevo = ipnuevo + "0" + variables[1];
    if (stoi(variables[2]) >= 10 || variables[2] == "00" || variables[2] == "000")
        ipnuevo = ipnuevo + variables[2];
    else ipnuevo = ipnuevo + "0" + variables[2];
    ultimo = variables[3];
    
    while(ultimo[i] != ':'){
        temp = temp + ultimo[i];
        i++;
    }
    
    if (stoi(temp) >= 10 || temp == "00")
        ipnuevo = ipnuevo + temp;
    else ipnuevo = ipnuevo + "0" + temp;
    
    stringstream geek(ipnuevo);
    long long x = 0;
    geek >> x;
    return x;
}

// Funcion 2 de ordenamiento merge
NodeDouble* merge(NodeDouble* left, NodeDouble* right){
    Usuarios persona;
    NodeDouble* ordenado = new NodeDouble(persona);
    NodeDouble* actual = ordenado;
    
    while(left != NULL && right != NULL){
        if(left->data.ip < right->data.ip){
            actual->next = left;
            actual->next->prev = actual;
            left = left->next;
            actual->prev = NULL;
        } else {
            actual->next = right;
            actual->next->prev = actual;
            right = right->next;
            actual->prev = NULL;
        }
        actual = actual->next;
    }
    
    if(left != NULL){
        actual->next = left;
        actual->next->prev = actual;
        left = left->next;
        actual->prev = NULL;
    }
    if(right != NULL){
        actual->next = right;
        actual->next->prev = actual;
        right = right->next;
        actual->prev = NULL;
    }
    return ordenado->next;
}

// Con esta funcion ordenaremos todos los ids de forma ascendente
NodeDouble* ordenaMerge(NodeDouble* head){
    if(head == NULL || head->next == NULL)
        return head;
    NodeDouble* temp = head;
    NodeDouble* slow = head;
    NodeDouble* fast = head;
    
    while(fast != NULL && fast->next != NULL){
        temp = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
   temp->next = NULL;
    
    NodeDouble* left = ordenaMerge(head);
    NodeDouble* right = ordenaMerge(slow);
    
   return merge(left,right);
    
}

// Funcion binary search para encontrar la IP de la fecha esperada con mayor rapidez
NodeDouble* binarySearch(NodeDouble *head, long long value){
    NodeDouble* start = head;
    NodeDouble* last = NULL;
    NodeDouble* first_ocurr = NULL;
    while(start != last){
        if (start == NULL)
           return NULL;
        NodeDouble* slow = start;
        NodeDouble* fast = start -> next;
        while (fast != last){
           fast = fast -> next;
           if (fast != last){
              slow = slow -> next;
              fast = fast -> next;
           }
        }
        NodeDouble* mid = slow;
        if(mid->data.ip >= value){
            first_ocurr = mid;
            last = mid;
        }else start = mid -> next;
    }
  
       return first_ocurr;
   
}
