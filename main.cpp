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
class Node{
public:
    Usuarios data;
    Node *next;
    Node(Usuarios data){
        this->data = data;
        next = NULL;
    }
};

// Singly LinkedList
class LinkedList{
public:
    Node *head;
    LinkedList(){
        head = NULL;
    }
    
    void addLast(Node* n){
        if(head == NULL)
            head = n;
        else {
            Node* prev = head;
            while (prev->next != NULL)
                prev = prev->next;
            prev->next = n;
        }
    }
    void print(){
        if (head == NULL)
            cout << "Lista vacia" << endl;
        else{
            Node *last = head;
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
Node* merge(Node* left, Node* rigth);
Node* ordenaMerge(Node* head);
Node* binarySearch(Node *head, long long value);

// Menu principal
int main(int argc, const char * argv[]) {
    LinkedList ll;
    int cont = 0, numero = 1;
    string ip1, ip2;
    ofstream f("registro.txt");
    
    lecturaVariables(ll);
    ordenaMerge(ll.head);
    
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
    
    Node* first = binarySearch(ll.head, ipinicio);
    Node* last = binarySearch(ll.head, ipfin);
    
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
        Node* n= new Node(persona);
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
    if (stoi(variables[2]) >= 10 || variables[2] == "000")
        ipnuevo = ipnuevo + variables[2];
    else ipnuevo = ipnuevo + "0" + variables[2];
    ultimo = variables[3];
    
    while(ultimo[i] != ':'){
        temp = temp + ultimo[i];
        i++;
    }
    if (stoi(temp) >= 10 || temp == "00")
        ipnuevo = ipnuevo + temp;
    else ipnuevo = ipnuevo + "00" + temp;
    
    //cout << "ipnuevo: "<< ipnuevo << endl;
    stringstream geek(ipnuevo);
    long long x = 0;
    geek >> x;
    return x;
}

// Funcion 2 de ordenamiento merge
Node* merge(Node* left, Node* right){
    Usuarios persona;
    Node* ordenado = new Node(persona);
    Node* actual = ordenado;
    
    while(left != NULL && right != NULL){
        if(left->data.ip < right->data.ip){
            actual->next = left;
            left = left->next;
        } else {
            actual->next = right;
            right = right->next;
        }
        actual = actual->next;
    }
    
    if(left != NULL){
        actual->next = left;
        left = left->next;
    }
    if(right != NULL){
        actual->next = right;
        right = right->next;
    }
    return ordenado->next;
}

// Con esta funcion ordenaremos todos los ids de forma ascendente
Node* ordenaMerge(Node* head){
    if(head == NULL || head->next == NULL)
        return head;
    Node* temp = head;
    Node* slow = head;
    Node* fast = head;
    
    while(fast != NULL && fast->next != NULL){
        temp = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
   temp->next = NULL;
   Node* left = ordenaMerge(head);
   Node* right = ordenaMerge(slow);
    
   return merge(left,right);
    
}

// Funcion binary search para encontrar la IP de la fecha esperada con mayor rapidez
Node* binarySearch(Node *head, long long value){
    Node* start = head;
    Node* last = NULL;
    Node* first_ocurr = NULL;
    while(start != last){
        if (start == NULL)
           return NULL;
        struct Node* slow = start;
        struct Node* fast = start -> next;
        while (fast != last){
           fast = fast -> next;
           if (fast != last){
              slow = slow -> next;
              fast = fast -> next;
           }
        }
       Node* mid = slow;
        if(mid->data.ip >= value){
            first_ocurr = mid;
            last = mid;
        }else start = mid -> next;
    }
  
       return first_ocurr;
   
}
